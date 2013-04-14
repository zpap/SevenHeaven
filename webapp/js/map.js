
var globalWarmingData = {
    "IN": {
    name: "India",
    data: [3.907, 7.943, 7.848, 9.284, 9.263, 9.801, 3.890, 8.238, 9.552, 6.855]
    },
"RU": {
    name: "Russian Federation",
    data: [4.743, 7.295, 7.175, 6.376, 8.153, 8.535, 5.247, -7.832, 4.3, 4.3]
    },
"US": {
    name: "United States of America",
    data: [1.988, 2.733, 3.994, 3.464, 4.001, 3.939, 1.333, -2.245, 4.339, 2.727]
    },
"DE": {
    name: "Germany",
    data: [-0.253, 0.362, -3.519, 1.799, 2.252, 3.343, 0.843, 2.877, -5.416, 5.590]
    }
};

$(function(){

    <!-- Testing Data -->
    var gdpData = {
    "IN": 132.04,
    "DE": 3300.54,
    "RU": 1456.87,
    "US": 14511.05
    };



map = new jvm.WorldMap({
    container: $('#world-map'),
    map: 'world_mill_en',
    series: {
    regions: [{
    values: gdpData,
    scale: ['#C8EEFF', '#0071A4'],
    normalizeFunction: 'polynomial'
    }]
},
onRegionLabelShow: function(e, el, code){
    //el.html(el.html()+' (GDP - '+gdpData[code]+')');
    },

onRegionClick: function(event, code) {
    console.log(' Clicked on country: '+code);
    addCountryToChart(code);
    }
});

map.container.click(function(e){
    var latLng = map.pointToLatLng(e.offsetX, e.offsetY),
    targetCls = $(e.target).attr('class');

    if (latLng && (!targetCls || (targetCls && $(e.target).attr('class').indexOf('jvectormap-marker') === -1))) {
    console.log(' Clicked at lat: '+latLng.lat+ ' long: '+latLng.lng);
    }
});

});

function createChart() {
    $("#chart").kendoChart({
        title: {
            text: "Global Warming"
        },
legend: {
    position: "bottom"
    },
chartArea: {
    background: ""
    },
seriesDefaults: {
    type: "line"
    },
valueAxis: {
    labels: {
    format: "{0}%"
},
line: {
    visible: false
    },
axisCrossingValue: -10
},
categoryAxis: {
    categories: [2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011],
    majorGridLines: {
    visible: false
    }
}
});
};

function addCountryToChart(code) {
    var chart = $("#chart").data("kendoChart");
    var series = chart.options.series;

    series.push(globalWarmingData[code]);
    chart.refresh();
    }

$(document).ready(function() {
    setTimeout(function() {
        // Initialize the chart with a delay to make sure
        // the initial animation is visible
        createChart();
    }, 400);
});
