
var counter = 0;
var descriptorCountToLoad = 6;
var variableName;
var dataSetName;
var badFlag;
var subSet;
var dateCount;

var longitudeValues;

// XML BUILD / parse
var et = require('elementtree');
var XML = et.XML;
var ElementTree = et.ElementTree;
var element = et.Element;
var subElement = et.SubElement;
var xmlRoot;
var dateNode;

function parseDescriptors(inputLine) {
    var line = inputLine.trim();

    counter += 1;

    var endOfDescriptor = line.indexOf(' : ');
    var descriptor =  line.substring(0, endOfDescriptor).trim();

    if (descriptor.toUpperCase() == "VARIABLE") {
        variableName = line.substring(endOfDescriptor + 3, line.length);
    }
    else if (descriptor.toUpperCase() == "DATA SET") {
        dataSetName = line.substring(endOfDescriptor + 3, line.length);
    }
    else if (descriptor.toUpperCase() == "BAD FLAG") {
        badFlag = line.substring(endOfDescriptor + 3, line.length);
    }
    else if (descriptor.toUpperCase() == "SUBSET") {
        subSet = line.substring(endOfDescriptor + 3, line.length);
        // not the best solution, but works for this specific data
        dateCount = subSet.split(" ")[4];
    }

    if (counter == descriptorCountToLoad)
        return true;
    return false;
}

function readLineByLine(fd, startposition, parserFunc) {

    var bufferLength = 100;
    var buffer = new Buffer(bufferLength);
    var lineBuffer = new Buffer(' ');
    var bytesRead = -1;
    var stopParsing = false;
    var currentPosition = startposition;

    while (!stopParsing) {

        bytesRead = fs.readSync(fd, buffer, 0, bufferLength, currentPosition);

        if (bytesRead == 0) {
            break;
        }
        lineBuffer = lineBuffer.concat(buffer);
        var breakPosition = -1;
        while ((breakPosition = lineBuffer.toString().indexOf('\n')) != -1) {
            if (breakPosition != -1) {
                var line = lineBuffer.toString().substring(0, breakPosition);
                stopParsing = parserFunc(line);
                var remainder = lineBuffer.toString().substring(breakPosition + 1, lineBuffer.toString().length);
                lineBuffer = new Buffer(remainder);
                if (stopParsing) {
                    currentPosition -= remainder.length;
                }
            }
        }
        currentPosition +=  bytesRead;
    }

    return currentPosition;
}

function loadLongitudes(inputLine) {
    var line = inputLine.trim();

    longitudeValues = line.split(/\s+/);

    for (var i = 0; i < longitudeValues.length; i++) {
        longitudeValues[i] = longitudeValues[i].trim();
    }

    return true;
}

function loadLatitudes(inputLine) {

    var line = inputLine.trim();

    var pointPos = line.indexOf('L:');

    // we have a point data
    if (pointPos != -1) {
        var datePos = line.indexOf('T:');
        var dateString = line.substring(datePos, line.length).trim();
        var date = new Date(dateString);
        dateNode = subElement(xmlRoot, "Date");
        dateNode.text = date.toDateString();
        console.log("New Date: " + dateNode.text);
    }
    else {
        var fullLine = line.split(/\s+/);
        var latitudeValues = fullLine.slice(2, fullLine.length - 1);

        for (var i = 0; i < latitudeValues.length; i++) {
            latitudeValues[i] = latitudeValues[i].trim();
            if (parseFloat(latitudeValues[i]) == parseFloat(badFlag)) {
                latitudeValues[i] = "NAN";
            }
        }
        var latitudeNode = subElement(dateNode, "Latitude");
        latitudeNode.set("value", fullLine[0]);
        latitudeNode.text = latitudeValues.toString();
    }

    // read more please!
    return false;
}

if (process.argv.length != 4) {
    console.log("Usage \"node ParseFile input_file output_file\"");
    process.exit(1);
}

var inputFile = process.argv[2];
var outputFile = process.argv[3];

var fs = require('fs');
require('buffertools');

var fd = fs.openSync(inputFile, 'r');

var currentPosition = readLineByLine(fd, 0, parseDescriptors);

console.log("Variable: " + variableName);
console.log("Data Set: " + dataSetName);
console.log("Bad Flag: " + badFlag);
console.log("Subset  : " + subSet);

// descriptors are in memory, now let's load the longitude values
currentPosition = readLineByLine(fd, currentPosition, loadLongitudes);

xmlRoot = element('Eco_Bodhi');

var variableNode = subElement(xmlRoot, "VariableName");
variableNode.text = "\"" + variableName + "\"";
var dataSetNode = subElement(xmlRoot, "DataSetName");
dataSetNode.text = "\"" + dataSetName + "\"";
var subSetNode = subElement(xmlRoot, "SubSet");
subSetNode.text = "\"" + subSet + "\"";
var numDatesNode = subElement(xmlRoot, "TotalDates");
numDatesNode.text = "\"" + dateCount + "\"";

var longitudesNode = subElement(xmlRoot, "Longitudes");
longitudesNode.text = longitudeValues.toString();

// load latitudes & dates
currentPosition = readLineByLine(fd, currentPosition, loadLatitudes);

etree = new ElementTree(xmlRoot);
xml = etree.write({'xml_declaration': true});

// happy write to file
var fdOut = fs.openSync(outputFile, 'w');
var str = xml.toString();
fs.writeSync(fdOut, new Buffer(str), 0, str.length, 0);
fs.closeSync(fdOut);

fs.closeSync(fd);
