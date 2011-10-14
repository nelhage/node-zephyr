var _zephyr = require('./build/default/_zephyr');

var connection = null;

function ZephyrError (code) {
    this.message = _zephyr.error_message(code);
    this.code = code;
}

ZephyrError.prototype = new Error();
ZephyrError.prototype.constructor = ZephyrError;

module.exports.get_connection = function () {
    if (connection == null) {
        connection = new _zephyr.Connection
        connection.on("zephyr_error", function (errno) {
            connection.emit('error', new ZephyrError(errno));
        })
    }
    return connection;
}
