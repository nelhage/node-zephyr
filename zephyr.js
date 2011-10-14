var _zephyr = require('./build/default/_zephyr');


var connection = null;

module.exports.get_connection = function () {
    if (connection == null) {
        connection = new _zephyr.Connection
        connection.on("zephyr_error", function (errno) {
            connection.emit('error', {
                errno: errno,
                error: _zephyr.error_message(errno)
            })
        })
    }
    return connection;
}
