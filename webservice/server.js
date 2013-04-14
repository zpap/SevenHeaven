var express = require("express");
var app = express();

app.use(express.static(__dirname + '/../webapp'));



// ------------------------------------------- Tips  -----------------------------

function Tip (title, description) {
    this.title = title;
    this.description = description;
}

function TipsRepository() {}

TipsRepository.prototype.findAll = function () {

    var results = new Array (30);

    for (i = 0; i < 30; i++) {
        var title = "Some tip title here";
        var description = "Some description about the tip here"
        results[i] = new Tip(title, description);
    }

    return results;
}

// ------------------------------------------- Facts  -----------------------------

var factsjson = require("./data/facts.json");
/*
function Fact (title, description) {
    this.title = title;
    this.description = description;
}
function FactsRepository() {}
FactsRepository.prototype.findAll = function () {

    var results = new Array (30);

    for (i = 0; i < 30; i++) {
        var title = "Some title for the fact here";
        var description = "Some description for the fact here"
        results[i] = new Fact(title, description);
    }

    return results;
}      */

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

// ------------------------------------------------------------------------------

var carbonFootprintRepository = new CarbonFootprintRepository();
var tipsRepository = new TipsRepository();
//var factsRepository = new FactsRepository();

app.use(express.logger());

app.configure(function() {
    app.use(express.bodyParser()); // used to parse JSON object given in the request body
});

// --------------------------------------- REST -----------------------------------

//app.get('/', function(request, response) {
//    response.send('Welcome in Seven Heaven');
//});

app.get('/carbonFootprints', function(request, response) {
    try {
        response.json(carbonFootprintRepository.findAll());
    } catch (exception) {
        response.send(404);
    }
});

app.get('/tips', function(request, response) {
    try {

        var tips = tipsRepository.findAll();
        var errorMessage = "no error";
        var result = "success";
        response.json({data: tips, message: errorMessage, result: result});
    } catch (exception) {
        response.send(404);
    }
});

app.get('/facts', function(request, response) {
    /*try {
        var facts = factsRepository.findAll();
        var errorMessage = "no error";
        var result = "success";
        response.json({data: facts, message: errorMessage, result: result});
    } catch (exception) {
        response.send(404);
    }         */
    response.json(factsjson);
});


app.get('/carbonFootprints', function(request, response) {
    try {
        response.json(carbonFootprintRepository.findAll());
    } catch (exception) {
        response.send(404);
    }
});

app.get('/carbonFootprint/:x/:y', function(request, response) {
    var x = request.params.x;
    var y = request.params.y;
    try {
       response.json(carbonFootprintRepository.find(x,y));
    } catch (exception) {
        response.send(404);
    }
});

// ------------------------------------------------------------------------------

// Use middleware to parse POST data and use custom HTTP methods
app.use(express.bodyParser());
app.use(express.methodOverride());

var port = process.env.PORT || 5000;

app.listen(port, function() {
    console.log("Listening on " + port);
});

