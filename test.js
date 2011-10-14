var zephyr = require('./zephyr');

var conn = zephyr.get_connection();
conn.on('init', function () {
    console.log("Initialized zephyr");
});
conn.on('error', function (err) {
    console.log("Error: ", err);
});

conn.init();

function blah() {
    console.log('blah');
    setTimeout(blah, 1000);
}
setTimeout(blah, 1000);
