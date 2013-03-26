var _ = require("underscore");

// Add a convenience method to Buffer
Buffer.prototype.toByteArray = function () {
  return Array.prototype.slice.call(this, 0)
}

function mavutil() { };

mavutil.x25Crc = function(buffer, crc) {

    var bytes = buffer;
    var crc = crc || 0xffff;
    _.each(bytes, function(e) {
        var tmp = e ^ (crc & 0xff);
        tmp = (tmp ^ (tmp << 4)) & 0xff;
        crc = (crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4);
        crc = crc & 0xffff;
    });
    return crc;
}

module.exports = mavutil;