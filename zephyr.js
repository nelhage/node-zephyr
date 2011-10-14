var _zephyr = require('./build/default/_zephyr');


var connection = null;

module.exports.get_connection = function () {
    if (connection == null)
        connection = new _zephyr.Connection
    return connection;
}
