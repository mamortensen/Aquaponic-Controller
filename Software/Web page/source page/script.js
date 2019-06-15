// Our labels along the x-axis
var time = "1,2,3,4,5,6,7,8,9,10,11,12";
// For drawing the lines
var temp = "20,30,40,50,60,70,80,75,80,70";
var level = "50,60,50,40,50,60,50,40,50,60";
var power = "5,6,5,4,5,6,5,4,5,6";
var light = "0.5,0.5,0.5,1.5,1.5,1.5,0.5,0.5,1.5,1.5";

// temperature chart
var ctx = document.getElementById("tempChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time.split(','),
    datasets: [
      { 
        data: temp.split(','),
//        data: %TEMPERATURE%.split(','),
        label: "Temperature",
        borderColor: "#FFC12B",
		fill: false
      }
    ]
  }
});

// water level chart
var ctx = document.getElementById("levelChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time.split(','),
    datasets: [
	  { 
   		data: level.split(','),
//      data: %LEVEL%.split(','),
   		label: "Water Level",
   		borderColor: "#9F3BFF",
   		fill: false
 	  }
    ]
  }
});

// Power chart
var ctx = document.getElementById("powerChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time.split(','),
    datasets: [
	  { 
   		data: power.split(','),
//      data: %POWER%.split(','),
   		label: "Power Consumption",
   		borderColor: "#37B0FF",
   		fill: false
 	  }
    ]
  }
});

// light power chart
var ctx = document.getElementById("lightChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time.split(','),
    datasets: [
	  { 
      data: light.split(','),
 //    data: %LIGHT%.split(','),
   		label: "Lighting power Consumption",
   		borderColor: "#FF2B30",
   		fill: false
 	  }
    ]
  }
});

// update interval
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temp").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temp", true);
  xhttp.send();
}, 60000 ) ; // repeat once a minute

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("level").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/level", true);
  xhttp.send();
}, 60000 ) ; // repeat once a minute

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("power").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/power", true);
  xhttp.send();
}, 60000 ) ; // repeat once a minute

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("light").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/light", true);
  xhttp.send();
}, 60000 ) ; // repeat once a minute
