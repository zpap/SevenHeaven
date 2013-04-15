var express = require("express");
var app = express();

// ----------------------------------------- XML --------------------------------

var fs = require('fs'),
xml2js = require('xml2js');
jquery = require('jquery');

var parser = new xml2js.Parser({ explicitArray: true, attrkey: "SHAttr", charkey: "SHCharKey" });
// storing data based on xml
var availableDates = new Array();
var longitudes;
var latitudeList = new Array();

console.log("Parsing CO2.xml ...");

// parsing the xml
fs.readFile('CO2.xml', function(err, data) {
    parser.parseString(data, function (err, result) {

        var jsonString = JSON.stringify(result);
        var json_parsed = jquery.parseJSON(jsonString);
        var root = json_parsed.Eco_Bodhi;
        longitudes = root.Longitudes[0].split(",");
        var dates = root.Date;

        for (i = 0; i < dates.length; i++) {
            var dateData = dates[i];
            // storing latitudes list for every date, and updating date list
            latitudeList[i] = dateData.Latitude;
            var date = dateData.SHAttr.value;
            availableDates[i] = date;
        }

        console.log("Total dates: " + dates.length + " parsed sucessfully!");

        // Use middleware to parse POST data and use custom HTTP methods
        app.use(express.bodyParser());
        app.use(express.methodOverride());

        var port = process.env.PORT || 5000;

        app.listen(port, function() {
            console.log("Starting Service. Listening on " + port + " ...");
        });
    });
});

app.use(express.static(__dirname + '/../webapp'));

// ----------------------------- Links to Quotes, Tips and Facts json files  -----------------------------

var quotesjson = require("./data/quotes.json");
var tipsjson = require("./data/tips.json");
var factsjson = require("./data/facts.json");

// ------------------------------------ Carbon Footprint -------------------------

function CarbonData(x,y,value) {
    this.x = x;
    this.y = y;
    this.value = value;
}

function CarbonFootprintRepository() {}

CarbonFootprintRepository.prototype.find = function(x,y) {

    var randomValue = Math.floor((Math.random()*400)+1);
    return new CarbonData(x,y, randomValue);
}

CarbonFootprintRepository.prototype.findAll = function() {


    var results = new Array (30);

    for (i = 0; i < 30; i++) {
        var randomX = Math.floor((Math.random()*100)+1);
        var randomY = Math.floor((Math.random()*100)+1);
        var randomNumber = Math.floor((Math.random()*400)+1);
        results[i] = new CarbonData(randomX, randomY, randomNumber);
    }

    return results;
}

CarbonFootprintRepository.prototype.carbonValues = function(long, lat) {

    // finding latitude index

    var indexLong = longitudes.indexOf(long);

    console.log ("Longitude index requested " + indexLong + " for " + long);

    console.log("Displaying carbon values" + latitudeList.length);
    var result = new Array();

    for (i = 0; i < latitudeList.length; i++) {

        var latitudes = latitudeList[i];

        for (j = 0; j < latitudes.length; j++) {

            var latitude = latitudes[j].SHCharKey;
            var latitudeValue = latitudes[j].SHAttr.value;

            if (latitudeValue == lat) {

                var latitudeItems = latitude.split(",");

                var value = latitudeItems[indexLong];

                result[i] = value;
            }
        }
   }

    return result;
}

// ------------------------------------------------------------------------------

var carbonFootprintRepository = new CarbonFootprintRepository();

app.use(express.logger());

app.configure(function() {
    app.use(express.bodyParser()); // used to parse JSON object given in the request body
});

// --------------------------------------- REST -----------------------------------


app.get('/carbonFootprints', function(request, response) {
    try {
        response.json(carbonFootprintRepository.findAll());
    } catch (exception) {
        response.send(404);
    }
});

app.get('/quotes', function(request, response) {
    response.json(quotesjson);
});

app.get('/tips', function(request, response) {
    response.json(tipsjson);
});

app.get('/facts', function(request, response) {
    response.json(factsjson);
});


app.get('/availableDates', function(request, response) {
    try {
        response.json(availableDates);
    } catch (exception) {
        response.send(404);
    }
});

app.get('/longitudes', function(request, response) {
    try {

        var errorMessage = "no error";
        var result = "success";

        response.json({data: longitudes, message: errorMessage, result: result});
    } catch (exception) {
        response.send(404);
    }
});

app.get('/carbonFootprint/:long/:lat', function(request, response) {
    var long = request.params.long;
    var lat = request.params.lat;
    try {
        response.json(carbonFootprintRepository.carbonValues(long,lat));
    } catch (exception) {
        response.send(404);
    }
});

// ------------------------------------------------------------------------------


