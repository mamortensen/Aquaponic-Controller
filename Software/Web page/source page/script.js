// Our labels along the x-axis
//var years = [1500,1600,1700,1750,1800,1850,1900,1950,1999,2050];
var time = [12,1,2,3,4,5,6,7,8,9];
// For drawing the lines
var temp = [20,30,40,50,60,70,80,75,80,70];
var level = [50,60,50,40,50,60,50,40,50,60];
var power = [5,6,5,4,5,6,5,4,5,6];
var light = [0.5,0.5,0.5,1.5,1.5,1.5,0.5,0.5,1.5,1.5];
var northAmerica = [6,3,2,2,7,26,82,172,312,433];

var ctx = document.getElementById("tempChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time,
    datasets: [
      { 
        data: temp,
        label: "Temperature",
        borderColor: "#3e95cd",
		fill: false
      }
    ]
  }
});

var ctx = document.getElementById("levelChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time,
    datasets: [
	  { 
   		data: level,
   		label: "Water Level",
   		borderColor: "#8e5ea2",
   		fill: false
 	  }
    ]
  }
});

var ctx = document.getElementById("powerChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time,
    datasets: [
	  { 
   		data: power,
   		label: "Power Consumption",
   		borderColor: "#8e5ea2",
   		fill: false
 	  }
    ]
  }
});

var ctx = document.getElementById("lightChart");
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: time,
    datasets: [
	  { 
   		data: light,
   		label: "Lighting power Consumption",
   		borderColor: "#8e5ea2",
   		fill: false
 	  }
    ]
  }
});
