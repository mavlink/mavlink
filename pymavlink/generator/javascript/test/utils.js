var mavutil = require('../lib/mavutil.js'),
  should = require('should');

describe("MAVUtil library", function() {

	beforeEach(function() {
		// Message header + payload, lacks initial MAVLink flag (FE) and CRC.
		this.heartbeatMessage = new Buffer([0x09, 0x03, 0xff , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x08 , 0x00 , 0x00 , 0x03]);
//fe 09 03 ff 00 00 00 00 00 00 06 08 00 00 03 9f 5c
	});

	// This test matches the output directly taken by inspecting what the Python implementation
	// generated for the above packet.
	it('implements x25crc function', function() {
	    mavutil.x25Crc(this.heartbeatMessage).should.equal(27276);
	});

	// Heartbeat crc_extra value is 50.
	it('can accumulate further bytes as needed (crc_extra)', function() {
	    var crc = mavutil.x25Crc(this.heartbeatMessage);
	    crc = mavutil.x25Crc([50], crc);
	    crc.should.eql(23711)
	});

});