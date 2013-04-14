$(document).ready(function() {

//
var menu = menu = $('#menu');
$('.tab').click(function(event) {

    event.preventDefault();

    menu.children().removeClass("active");

    /* Add active class to clicked anchor's parent li */
    $(this).addClass('active');

});


    <!-- Test data -->
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

    var gdpData = {
        "IN": 5,
        "DE": 5,
        "RU": 5,
        "US": 5
    };
    <!-- end of Test data -->

    <!-- Hardcoded country coordinates -->
    var countryCoordinates = {
        "DE": {
            "lat": "50N",
            "long": "10E"
        },
        "RU": {
            "lat": "60N",
            "long": "97.5E"
        },
        "US": {
            "lat": "40N",
            "long": "105W"
        },
        "IN": {
            "lat":"22N",
            "long":"80E"
        }
    };
    <!-- end of Hardcoded country coordinates -->

    var map;
    var categoryNames = {
        "warming": "Global Warming",
        "carbon": "Carbon Footprint",
        "sealevel": "Sea Level"
    }
    var selectedCountries = [];
    var selectedCoordinates = {};
    var defaultCategory = "warming";
    var MAX_COUNTRY_SELECTION = 3;
    var selectedCategory = defaultCategory;
    var lastCountryClicked = '';

    <!-- Creating page flip effect object -->
    var effect = kendo.fx("#container").flipHorizontal($("#face"), $("#back")).duration(300),
        reverse = false;

    $("#btnHome").click(function() {
        flipToPage("homePage");
    });
    $("#btnMap").click(function() {
        flipToPage("mapPage");
    });
    $("#btnFacts").click(function() {
        flipToPage("factsPage");
    });
    $("#btnTips").click(function() {
        flipToPage("tipsPage");
    });
    $("#btnAbout").click(function() {
        flipToPage("aboutPage");
    });

    function flipToPage(pageName) {
        effect.stop();
        <!-- load pageName into hidden side and turn page -->
        var newContent = $('div#'+pageName).html();
        if (reverse) {
            $('div#face').html(newContent);
        } else {
            $('div#back').html(newContent);
        }

        if (pageName == "mapPage") {
            createMap();
            // link control handlers on the map page
            $("div.side #btnResetChart").click(function() {
                resetChart();
            });
            $("input[name='categorySelector']").change(categoryChanged);
            // Initialize the chart with a delay to make sure the initial animation is visible
            setTimeout(function() {
                createChart();
            }, 400);
        }

        <!-- Do the page flip -->
        reverse ? effect.reverse() : effect.play();
        reverse = !reverse;
    }

    <!-- Functions for the map and chart -->
    function createMap() {
        map = new jvm.WorldMap({
            container: $('div.side #world-map'),
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
                //console.log(' Clicked on country: '+code+', coordinates: '+JSON.stringify(countryCoordinates[code], null, "\t"));
                lastCountryClicked = code;      // used to set the countryCoordinates values
                addRemoveCountryToChart(code);
            }
        });

        map.container.click(function(e){
            var latLng = map.pointToLatLng(e.offsetX, e.offsetY),
                targetCls = $(e.target).attr('class');

            if (latLng && (!targetCls || (targetCls && $(e.target).attr('class').indexOf('jvectormap-marker') === -1))) {
                //console.log(' Clicked at lat: '+latLng.lat+ ' long: '+latLng.lng);
                setSelectedCoordinates(latLng);
            }
        });
    }

    function createChart() {
        $("div.side #chart").kendoChart({
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
                    format: "{0}"
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
            },
            tooltip: {
                visible: true,
                format: "{0}",
                template: "#= series.name #: #= value #"
            }
        });
    };

    // Generate acceptable coordinates for the REST from precise coordinates
    function setSelectedCoordinates(latLng) {
        //latLng.lat+ ' long: '+latLng.lng
        var lat = Math.round(latLng.lat);
        if (lat%2 == 1) {
            lat++;
        }
        if (lat > 0) {
            lat = lat + 'N';
        } else {
            lat = Math.abs(lat) + 'S';
        }
        var long = Math.round(latLng.lng);
        if (long%2.5 != 0) {
            long = long - long%2.5;
        }
        if (long > 0) {
            long = long + 'E';
        } else {
            long = Math.abs(long) + 'W';
        }
        selectedCoordinates.lat = lat;
        selectedCoordinates.long = long;
        countryCoordinates[lastCountryClicked] = selectedCoordinates;
        //console.log(' Calculated coordinates: '+JSON.stringify(selectedCoordinates, null, "\t"));
    };

// Adds country code to the chart, or if already added, remove is
    function addRemoveCountryToChart(code) {
        var newSelection = [];

        var alreadySelected = false;
        for (i=0; i<selectedCountries.length; i++) {
            if (selectedCountries[i] == code) {
                alreadySelected = true;
            } else {
                newSelection.push(selectedCountries[i]);
            }
        }
        if (!alreadySelected) {
            newSelection.push(code);
        }

        if (newSelection.length <= MAX_COUNTRY_SELECTION) {
            selectedCountries = newSelection.slice(0, newSelection.length);
        } else {
            selectedCountries = newSelection.slice(1, newSelection.length);
        }

        // Update a bit later, so we have time to set formatted coordinate for country
        setTimeout(function() {updateChart();}, 100);
    }

    function resetChart() {
        selectedCountries = [];
        updateChart();
    }

    function categoryChanged() {
        selectedCategory = $(this).val();
        updateChart();
    }

    function updateChart() {
        console.log('Update chart called');
        var chart = $("div.side #chart").data("kendoChart");

        // Change Title of the Chart
        chart.options.title.text = categoryNames[selectedCategory];

        // Update chart data
        chart.options.series = [];
        for (i=0; i<selectedCountries.length; i++) {
            // TODO: call REST for real data
            console.log(' TODO: call REST for real data, for country '+selectedCountries[i]);
            console.log('       coordinates are: '+JSON.stringify(countryCoordinates[selectedCountries[i]], null, "\t"));

            // TODO: remove this, once we have real data
            if (globalWarmingData[selectedCountries[i]] !== undefined) {
                chart.options.series.push(globalWarmingData[selectedCountries[i]]);
            }
        }
        chart.refresh();
    }

    <!-- Initialization on startup -->
    $(document).ready(function() {
        flipToPage("mapPage");
    });




});