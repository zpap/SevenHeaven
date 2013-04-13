//fs = require('fs');
//fs.readFile('test.txt', 'utf8', function (err,data) {
//    if (err) {
//        return console.log(err);
//    }
//    console.log(data);
//});

var counter = 0;
var descriptorCountToLoad = 6;
var variableName;
var dataSetName;
var badFlag;
var subSet;

var longitudeValues;

function parseDescriptors(inputLine) {
    var line = inputLine.trim();

    console.log(counter.toString() + ' ' + line);
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

        if (bytesRead == -1) {
            break;
        }
        lineBuffer = lineBuffer.concat(buffer);
        var breakPosition = -1;
        while ((breakPosition = lineBuffer.toString().indexOf('\n')) != -1) {
            if (breakPosition != -1) {
                var line = lineBuffer.toString().substring(0, breakPosition);
                stopParsing = parserFunc(line);
                var remainder = lineBuffer.toString().substring(breakPosition + 1, lineBuffer.toString().length);
                console.log(remainder);
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

    var parts = line.split(" +");
    console.log("PARTS: " + parts[0]);
    return true;
}

//function parseData(inputFile) {
    //var inputFile = 'CO2.txt';
    var fs = require('fs');
    require('buffertools');
    var fd = fs.openSync('CO2.txt', 'r');

    var currentPosition = readLineByLine(fd, 0, parseDescriptors);

    console.log("Variable: " + variableName);
    console.log("Data Set: " + dataSetName);
    console.log("Bad Flag: " + badFlag);
    console.log("Subset  : " + subSet);

    // descriptors are in memory, now let's load the longitude values

    currentPosition = readLineByLine(fd, currentPosition, loadLongitudes);

    fs.closeSync(fd);
