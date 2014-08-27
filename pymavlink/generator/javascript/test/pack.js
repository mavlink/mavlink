var should = require('should');
var jspack = require('../lib/node-jspack-int64/jspack.js').jspack;
var Long = require('long');

describe('Test long integration (examples)', function() {

	// Demonstrating the use together with Long.js (https://github.com/dcodeIO/Long.js)
	//
	// Because jspack doesn't care about signedness, we have to tell Long.js what we want our selfs.
	// Taking care of signedness in jspack would mean to introduce a dependency on something like
	// Long.js.
	//
	// Test number u            228290380562207 (BE: 0x00, 0x00, 0xcf, 0xa0, 0xff, 0x09, 0xff, 0x1f)
	//                                          (LE: 0x1f, 0xff, 0x09, 0xff, 0xa0, 0xcf, 0x00, 0x00)
	// Test number s           -228290380562207 (BE: 0xff, 0xff, 0x30, 0x5f, 0x00, 0xf6, 0x00, 0xe1)
	//                                          (LE: 0xe1, 0x00, 0xf6, 0x00, 0x5f, 0x30, 0xff, 0xff)

	it('pack <Q', function() {
		//var num = new Long(0xff09ff1f, 0x0000cfa0, true);
		var num = Long.fromNumber(228290380562207, true);

        var buf = jspack.Pack('<Q', [[num.getHighBitsUnsigned(), num.getLowBitsUnsigned()]]);
        buf.should.be.eql([0x1f, 0xff, 0x09, 0xff, 0xa0, 0xcf, 0x00, 0x00]);
    });

    it('unpack <Q', function() {
    	var num = new Long(0xff09ff1f, 0x0000cfa0, true);
        var buf = jspack.Unpack('<Q', [0x1f, 0xff, 0x09, 0xff, 0xa0, 0xcf, 0x00, 0x00]);
        buf.length.should.be.eql(1);
        buf[0].length.should.be.eql(2);
        var res = new Long(buf[0][1], buf[0][0], true);
        num.equals(res).should.be.true;
        res.toNumber().should.be.eql(228290380562207);
    });

    it('pack <q', function() {
		//var num = new Long(0x00f600e1, 0xffff305f, true);
		var num = Long.fromNumber(-228290380562207); // signed

        var buf = jspack.Pack('<q', [[num.getHighBitsUnsigned(), num.getLowBitsUnsigned()]]);
        buf.should.be.eql([0xe1, 0x00, 0xf6, 0x00, 0x5f, 0x30, 0xff, 0xff]);
    });

    it('unpack <q', function() {
    	var num = new Long(0x00f600e1, 0xffff305f); // signed
        var buf = jspack.Unpack('<q', [0xe1, 0x00, 0xf6, 0x00, 0x5f, 0x30, 0xff, 0xff]);
        buf.length.should.be.eql(1);
        buf[0].length.should.be.eql(2);
        var res = new Long(buf[0][1], buf[0][0]);
        num.equals(res).should.be.true;
        res.toNumber().should.be.eql(-228290380562207);
    });

});

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