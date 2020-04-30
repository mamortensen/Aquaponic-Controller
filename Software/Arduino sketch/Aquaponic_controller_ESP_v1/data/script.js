
// Requiring fs module in which  
// readFile function is defined. 
//  
//fs.readFile('data.txt', (err, data) => { 
//    if (err) throw err; 
//  
//    console.log(data.toString()); 
//}) 


//temperature chart
var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature' },
  title: { text: 'Water Temperature' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#059e8a' }
  },
  xAxis: { type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
//    title: { text: 'Temperature (Celsius)' }
    title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime(),
          y = parseFloat(this.responseText);
      if(isNaN(y)){}
      else{         
        if(chartT.series[0].data.length > 40) {
            chartT.series[0].addPoint([x, y], true, true, true);
        } else {
            chartT.series[0].addPoint([x, y], true, false, true);
        }
      }
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
//}, 30000 ) ;
}, 10000 ) ;

//water level chart
var chartH = new Highcharts.Chart({
  chart:{ renderTo:'chart-level' },
  title: { text: 'Water Level' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    }
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Water Level (in)' }
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime(),
          y = parseFloat(this.responseText);
      if(isNaN(y)){}
      else{
          if(chartH.series[0].data.length > 40) {
            chartH.series[0].addPoint([x, y], true, true, true);
          } else {
            chartH.series[0].addPoint([x, y], true, false, true);
          }
      }
    }
  };
  xhttp.open("GET", "/level", true);
  xhttp.send();
//}, 30000 ) ;
}, 10000 ) ;

//power consumption chart
var chartP = new Highcharts.Chart({
  chart:{ renderTo:'chart-power' },
  title: { text: 'Power Consumption' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#18009c' }
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Power (W)' }
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime(),
          y = parseFloat(this.responseText);
      if(isNaN(y)){}
      else{
          if(chartP.series[0].data.length > 40) {
            chartP.series[0].addPoint([x, y], true, true, true);
          } else {
            chartP.series[0].addPoint([x, y], true, false, true);
          }
      }
    }
  };
  xhttp.open("GET", "/power", true);
  xhttp.send();
//}, 30000 ) ;
}, 10000 ) ;