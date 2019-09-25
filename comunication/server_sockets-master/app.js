////////////////////////////////////////////////

var dgram = require('dgram');
var messageLeft = new Buffer('Left-1.0');
var messageRight = new Buffer('Right-1.0');
var messageClickPressed = new Buffer('Click-1');
var messageClickReleased = new Buffer('Click-0');

var client = dgram.createSocket('udp4');


//////////////////////////////////////////////////
var fs = require("fs");

var config = { secure: false, port: 8004 };
if (fs.existsSync("config.json")) {
  console.log("Using config file");
  var config = require("./config.json");
}
if (config.secure) {
  var app = require("https").createServer(
    {
      key: fs.readFileSync(config.privkey),
      cert: fs.readFileSync(config.fullchain)
    },
    handler
  );
} else {
  var app = require("http").createServer(handler);
}
var io = require("socket.io")(app);

var PORT = config.port;

app.listen(PORT);

function handler(req, res) {}

io.sockets.on("connection", function(socket) {
  
  socket.on("addJoystick", function(channel) {
    socket.channel = channel;
    socket.join(channel);
    console.log(socket.id + " joined " + channel);
    /*
    If we want to emit a message when someone is connected to the channel
    socket.broadcast
      .to(channel)
      .emit("handshake", { message: socket.id + " connected to " + channel });
    */
  });


  socket.on("toServer", function(data) {
    console.log(socket.id + " Emit to " + socket.channel);
    console.log(data);
    console.log(data.sender);
    console.log(data.message);
    //socket.broadcast.to(socket.channel).emit("fromServer", data);

    var PORTUDP = 33333;
    var HOSTUDP = '192.168.43.244';//127.0.0.1//'192.168.43.14';//
    var data_parsed_vector = data.message.split("-");
    console.log('data[0]: '+data_parsed_vector[0]);
    console.log('data[1]: '+data_parsed_vector[1]);

    if(data_parsed_vector[0] == 'X'){
      client.send(data.sender+"/"+data.message, 0, 1+data.sender.length+data.message.length, PORTUDP, HOSTUDP, function(err, bytes) {
      if (err) throw err;
      console.log('UDP message sent to ' + HOSTUDP +':'+ PORTUDP);
      });
    }
    else if(data_parsed_vector[0] == 'Click'){
      client.send(data.sender+"/"+messageClickPressed, 0, 1+data.sender.length+messageClickPressed.length, PORTUDP, HOSTUDP, function(err, bytes) {
      if (err) throw err;
      console.log('UDP message sent to ' + HOSTUDP +':'+ PORTUDP);
      });
    }

    });

  socket.on("disconnect", function() {
    //if(client)
    //client.close();
    //socket.leave(socket.channel);
    console.log('that was trying to disconnect. TODO Check this!');
  });

});
