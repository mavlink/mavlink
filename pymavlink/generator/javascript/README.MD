## Javascript MAVLink implementation ##

This code generates MAVLink modules that can be used with Node.js.  As with the other implementations in Python and C, the MAVLink protocol is specified in XML manifests which can be modified to add custom messages.

### Generating the JS implementation ###

Files in the ```implementations/``` directory are automatically generated from XML manifests that are in the (mavlink/mavlink)[https://github.com/mavlink/mavlink] project.  If you wish to generate custom MAVLink packets, you would need to follow the directions there.

You need to have Node.js and npm installed to build.  

To build the Javascript implementations:

```bash
cd mavlink/pymavlink/generator && make -f javascriptMakefile
```

### Usage in Node.js ###

The generated modules emit events when valid MAVLink messages are encountered.  The name of the event is the same as the name of the message: HEARTBEAT, FETCH_PARAM_LIST, REQUEST_DATA_STREAM, etc.  In addition, a generic _message_ event is emitted whenever a message is successfully decoded.

```javascript
var mavlink = require('mavlink_implementation'),
	net = require('net');

connection = net.createConnection(5760, '127.0.0.1');
connection.on('data', function(data) {
	mavlink.parseBuffer(data);
});

mavlink.on('message', function(message) {
	console.log('Got a message of any type!');
	console.log(message);
});

mavlink.on('HEARTBEAT', function(message) {
	console.log('Got a heartbeat message!');
	console.log(message);
});
```

### Gotchas and todo's ###

The JS library that is implementing the pack/unpack functions doesn't match Python's struct library.  For example, it doesn't support the ```q``` or ```Q``` flags representing ```int64_t``` and ```uint64_t```, respectively.  Those have been replaced with ```d```,  ```double```, for the moment.

This code isn't great idiomatic Javascript (yet!), instead, it's more of a line-by-line translation from Python as much as possible.

### Development ###

You need to have [mocha](http://visionmedia.github.com/mocha/) installed to run the unit tests.

To run tests, use the makefile in the parent directory (mavlink/pymavlink/generator/):

```bash
make -f javascriptMakefile test
```

Specific instructions for generating Jenkins-friendly output is done through the makefile as well:

```bash
make -f javascriptMakefile ci
```

