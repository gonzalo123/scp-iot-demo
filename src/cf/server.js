var http = require('http'),
    io = require('socket.io'),
    request = require('request'),
    auth = "Basic " + new Buffer(process.env.USER + ":" + process.env.PASS).toString("base64"),
    url = process.env.IOT_ODATA,
    INTERVAL = process.env.INTERVAL,
    socket,
    value;

server = http.createServer();
server.listen(process.env.PORT || 3000);

socket = io.listen(server);

setInterval(function () {
    request.get({
        url: url,
        headers: {
            "Authorization": auth,
            "Accept": "application/json"
        }
    }, function (error, response, body) {
        var newValue = JSON.parse(body).d.results[0].C_VALUE;
        if (value !== newValue) {
            value = newValue;
            socket.sockets.emit('value', value);
        }
    });
}, INTERVAL);