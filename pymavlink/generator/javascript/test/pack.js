var should = require('should');
var jspack = require('../lib/node-jspack-int64/jspack.js').jspack;

describe('Test int64', function() {

    // Number 0xffa0ffe1ffff, packed with Python struct:
    // little endian:
	// 0xff, 0xff, 0xe1, 0xff, 0xa0, 0xff, 0x00, 0x00
	// big endian:
	// 0x00, 0x00, 0xff, 0xa0, 0xff, 0xe1, 0xff, 0xff


    it('pack <Q', function() {
        var buf = jspack.Pack('<Q', [[0xffa0, 0xffe1ffff]]);
        buf.should.be.eql([0xff, 0xff, 0xe1, 0xff, 0xa0, 0xff, 0x00, 0x00]);
    });

    it('pack >Q', function() {
        var buf = jspack.Pack('>Q', [[0xffa0, 0xffe1ffff]]);
        buf.should.be.eql([0x00, 0x00, 0xff, 0xa0, 0xff, 0xe1, 0xff, 0xff]);
    });

    it('unpack <Q', function() {
        var buf = jspack.Unpack('<Q', [0xff, 0xff, 0xe1, 0xff, 0xa0, 0xff, 0x00, 0x00]);
        buf.length.should.be.eql(1);
        buf[0].length.should.be.eql(2);
        buf[0][0].should.be.eql(0xffa0);
        buf[0][1].should.be.eql(0xffe1ffff);
    });

    it('unpack >Q', function() {
        var buf = jspack.Unpack('>Q', [0x00, 0x00, 0xff, 0xa0, 0xff, 0xe1, 0xff, 0xff]);
        buf.length.should.be.eql(1);
        buf[0].length.should.be.eql(2);
        buf[0][0].should.be.eql(0xffa0);
        buf[0][1].should.be.eql(0xffe1ffff);
    });

    it('unpack >Q full', function() {
        var buf = jspack.Unpack('>Q', [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]);
        buf.length.should.be.eql(1);
        buf[0].length.should.be.eql(2);
        buf[0][0].should.be.eql(0xffffffff);
        buf[0][1].should.be.eql(0xffffffff);
    });

    // Test lower-case q as well. This only test the matching of the caracter,
    // the parsing is the same as for upper-case Q (since we don't actually convert to a number).
    it('pack >q', function() {
        var buf = jspack.Pack('>q', [[0xffa0, 0xffe1ffff]]);
        buf.should.be.eql([0x00, 0x00, 0xff, 0xa0, 0xff, 0xe1, 0xff, 0xff]);
    });

    it('unpack <q', function() {
        var buf = jspack.Unpack('<q', [0xff, 0xff, 0xe1, 0xff, 0xa0, 0xff, 0x00, 0x00]);
        buf.length.should.be.eql(1);
        buf[0].length.should.be.eql(2);
        buf[0][0].should.be.eql(0xffa0);
        buf[0][1].should.be.eql(0xffe1ffff);
    });

});