/*
  Hacker Lab Aquaponic System controller
  This runs on an Arduino Nano with an ESP-01 connected to the serial port. 
  It reads the water temperature, water level, and power used
  then sends the data to the ESP-01 over the serial port.
  
*/  

#include <RunningAverage.h>

#define TEMP_SENSOR A6
#define TRIG_PIN 7
#define ECHO_PIN 6
#define POWER_PIN A7
#define LIGHTS_PIN 4

// Measured band gap voltage for this device * 100.
#define MEASURED_BANDGAP 1082L // note: (bandgap * 1000)
#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

//#define DEBUG true
#define DEBUG false

String inputString = "";

/* function to read the voltage of the power source.
*/
RunningAverage avgSrcV(5);

int readVcc()
// Calculate current Vcc in mV from the 1.1V reference voltage
{
  long result;
  long average;

  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  // value of bandgap 1082 measured and refined by trial and error, measured = 1.05V
  // Note, this is specific to this arduino
  result = 1055L * 1024L / result; // Back-calculate AVcc in mV - correct at 4.91
  avgSrcV.addValue(result);
  average = avgSrcV.getAverage(); 
  if(DEBUG){
    Serial.print("sourceV: ");
    Serial.print(result);
    Serial.print(" Average: ");
    Serial.println(average);
  }
// return(result);
  return(average);
}


 RunningAverage avgTempValue(5);

float readTemp (float sourceV) {
  // read the sensor
  int sensorValue = analogRead(TEMP_SENSOR);
  sensorValue = analogRead(TEMP_SENSOR);
  sensorValue = analogRead(TEMP_SENSOR);
  avgTempValue.addValue(sensorValue);
  int aveTV = avgTempValue.getAverage();

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = aveTV * (sourceV / 1023.0);
  voltage = voltage +10.2; // correction factor?
    float waterTemp = (voltage + 5.0)/10.0 * 1.8 + 32.0;
  if (DEBUG) {
    Serial.print("Sensor v: ");
    Serial.print (sensorValue);
    Serial.print (" aveTV: ");
    Serial.print (aveTV);
    Serial.print (" Voltage: ");
    Serial.print (voltage);
    Serial.print (" Temperature :");
    Serial.println (waterTemp);
  }
  return (waterTemp);  
}

/* Read the water level sensor
 */
float readLevel (void) {
  int trigPin = 7;    // Trigger
  int echoPin = 6;    // Echo
  long duration, cm, inches;

  // sometimes we get random really high values so if we do then re-read
  // utill we get a reasonable result.
  do {
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
   
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    duration = pulseIn(ECHO_PIN, HIGH);
  } while (duration > 12000) ; 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74.0;   // Divide by 74 or multiply by 0.0135

  if (DEBUG) {
    Serial.print("duration: ");
    Serial.print(duration);
    Serial.print(" inches: ");
    Serial.print(inches);
    Serial.print(" cm: ");
    Serial.println(cm);
  }
  return(inches);
}

/* read the power sensor
 */
RunningAverage avgPP(10);
 
float readPower (float sourceV) {

  int readValue;              //value read from the sensor
  int maxValue = 0;           // store max value here
  int minValue = 1023;        // store min value here
  float average;

  uint32_t start_time = millis();
  //sample for 1 Sec
  while((millis()-start_time) < 3000){ 
    readValue = analogRead(POWER_PIN);
    // see if you have a new max or min Value
    if (readValue > maxValue) {
      /*record the maximum sensor value*/
      maxValue = readValue;
    }
    if (readValue < minValue) {
      /*record the minimum sensor value*/
      minValue = readValue;
    }
  }
  
  int PPvalue = maxValue - minValue;
  // Convert the digital data to a voltage
  float Vpp = map(PPvalue, 0, 1023, 0.0, sourceV)/1000.0;
  avgPP.addValue(Vpp);
  average = avgPP.getAverage();

  // 82 ohm resistor, turns ratio 395? by test.
  float iPeak = (average/82) * 395;
  float iRMS = iPeak * 0.707;
  if (iRMS < 0) {
    iRMS = 0;
  }
  float  Watts = iRMS * 120;
  if (DEBUG) {
    Serial.print("maxValue: ");
    Serial.print(maxValue);
    Serial.print(" minValue: ");
    Serial.print(minValue);
    Serial.print(" PPvalue: ");
    Serial.print(PPvalue);
    Serial.print(" peak Voltage: ");
    Serial.print(Vpp);
    Serial.print(" avePP: ");
    Serial.print(average);
    Serial.print(" I peak: ");
    Serial.print(iPeak);
    Serial.print(" I RMS: ");
    Serial.print(iRMS);
    Serial.print(" Watts: ");
    Serial.println(Watts);
  }
  return (Watts);  
}

void setup() {
  pinMode(TEMP_SENSOR, INPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(POWER_PIN, INPUT);
  pinMode(LIGHTS_PIN, OUTPUT);

  analogReference(DEFAULT);

  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  //reserve 200 bytes for an input string.
  inputString.reserve(200); 

  // initalize arrays for calculating averages to smooth inputs
  avgSrcV.clear();
  avgPP.clear();
  avgTempValue.clear();

}

  float sourceV = 0;

void loop() {
  if (DEBUG) {
    Serial.println("******************");
  }
    sourceV = readVcc();
    String Message;

  // read sensors and send data to the web
  Message = String(readTemp(sourceV)) 
            + ":" 
            + String(readLevel())
            + ":"
            + String(readPower(sourceV));
  Serial.println (Message);

  // Read data sent back from the web
  // property:value: format
  if (Serial.available()) {
    String property = Serial.readStringUntil(':');
    String value = Serial.readStringUntil(':');
    String junk = Serial.readStringUntil('\n');    
    if (DEBUG) {
        Serial.print (" property: ");
        Serial.print (property);
        Serial.print (" value: ");
        Serial.println (value);
    }
    if (property == "lights") {
      if (value == "on") {
        digitalWrite(LIGHTS_PIN, HIGH);
      }
      else {
        digitalWrite(LIGHTS_PIN, LOW);
      }
    }
    
    // clear the string:
    inputString = "";
  }
  
//  delay(1000); // wait a sec
  delay(30000); // sample twice a minute
//  delay(60000); // sample once a minute
}
