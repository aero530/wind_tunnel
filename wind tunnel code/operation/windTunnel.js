// This work contains valuable confidential and proprietary information.
// Disclosure, use or reproduction without the written authorization of
// Dedicated Computing, LLC is prohibited. This unpublished work by the company
// is protected by the laws of the United States and other countries.
// If publication of the work should occur the following notice shall apply:
//
// "c" Copyright Dedicated Computing LLC 2016 All Rights Reserved

// To run, please:
//  install node/npm
//  install the required packages by navigating to the containing directory
//    and running `npm install`
//  create a client using the appliance's web-based user interface
//  setup the applianceIPAddress and clientMacAddress constants below
//  from the containing directory, execute `node longRunningAgent`



const SerialPort = require('serialport')
const Delimiter = require('@serialport/parser-delimiter')
const fs = require('fs');

// makes the reboot call - install this using the directions above
const shell = require('shelljs');

// ----------------------------------------------------------------------------

let stepDuration = 30;
let stepSize = 5;
let percentMin = 0;
let percentMax = 100;
let delim = ',';

// ----------------------------------------------------------------------------

var today = new Date().toISOString();
today = today.replace(/\:/g,'-');
console.log('Running Test '+today);
let filename = today+'_dur'+stepDuration+'_size'+stepSize;

let writeStream = fs.createWriteStream(filename + '_Sensors.csv');
writeStream.write('timestamp'+delim+'pitotPressureZero'+delim+'pitotPressureRaw'+delim+'pitotTemperature'+delim+'pitotVelocity'+delim+'hotWireTemperature'+delim+'hotWireVelocity'+'\n');

// ----------------------------------------------------------------------------

var hotWire = new SerialPort('COM4', { baudRate: 9600, autoOpen: false })

hotWire.open(function (error) {
	if ( error ) {
		console.log('how wire failed to open: ' + error);
	}
})

const hotWireParser = hotWire.pipe(new Delimiter({ delimiter: '\r' }))

let hotWireTemperature = 0;
let hotWireVelocity = 0;

hotWireParser.on('data', data => {
	data = data.toString('utf-8');
	if (data[2]=='2') {
		hotWireTemperature = data.substr(11,4)/10;
	} else if (data[2]=='1') {
		hotWireVelocity = data.substr(11,4)*1;

	}
})


// ----------------------------------------------------------------------------

var loadCard = new SerialPort('COM7', { baudRate: 115200, autoOpen: false })

loadCard.open(function (error) {
	if ( error ) {
		console.log('load card failed to open: ' + error);
	}
})

const loadCardParser = loadCard.pipe(new Delimiter({ delimiter: '\n' }))

loadCardParser.on('data', data => {
	
	// strip out the null characters from the stream.
	for (const pair of data.entries()) {
		//console.log(pair);
		if (pair[1] == 0x00) {
			data[pair[0]] = 0x20;
		}
	}
	
	// convert buffer to string and split on tab
	data = data.toString('utf-8');
	array = data.split('\t');
	//console.log(data);
	
	// write to file
	try {
		writeStream.write((Date.now())/1000+delim+array[0].trim()+delim+array[1].trim()+delim+array[2].trim()+delim+array[3].trim()+delim+hotWireTemperature.toString()+delim+hotWireVelocity.toString()+'\n');
	} catch (error) {
		console.log(error);
	}

})

// ----------------------------------------------------------------------------

const rebootCmd = 'python runTest.py '+stepDuration+' '+stepSize+' '+percentMin+' '+percentMax+' '+filename;

/*
shell.exec(rebootCmd, (exitCode, stdout, stderr) => {
    if (exitCode !== 0) console.log('Could not run test\n' + stdout + '\n' + stderr);
});
*/


const test = shell.exec(rebootCmd, {async:true}, (exitCode, stdout, stderr) => {
	if (exitCode !== 0) console.log('Could not run test\n' + stdout + '\n' + stderr);
	
	loadCard.close(function (err) {
		console.log('load card port closed', err);
	});
	
	hotWire.close(function (err) {
		console.log('hot wire port closed', err);
	});

	// close the stream
	writeStream.end();
});



/*
(exitCode, stdout, stderr) => {
    if (exitCode !== 0) console.log('Could not run test\n' + stdout + '\n' + stderr);
}*/
/*
setTimeout(() => {
	loadCard.close(function (err) {
		console.log('load card port closed', err);
	});
	
	hotWire.close(function (err) {
		console.log('hot wire port closed', err);
	});

	// close the stream
	writeStream.end();

}, ((percentMax-percentMin)/stepSize+2)*stepDuration*1000);


*/