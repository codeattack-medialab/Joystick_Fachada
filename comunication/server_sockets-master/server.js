
////////////////////////////////////////////////
var dgram = require('dgram');
var PORTUDP = 33333;
var HOSTUDP = '127.0.0.1';//192.168.43.244//'192.168.43.14';//
//var messageClickPressed = new Buffer('Click/1');
//var messageClickReleased = new Buffer('Click/0');
var client;

client = dgram.createSocket('udp4');
client.on('close',function(){
  console.log('Client UDP socket closed : BYE!')
})

//////////////////////////////////////////////////

// HTTP Portion
var http = require('http');
// Path module
var path = require('path');

// Using the filesystem module
var fs = require('fs');

//////////////////////
//Control Vars
let connectCounter = 0;
let statusMachine = 0; //0 waiting, 1 ready to play, 2 playing, 3 finish

//First Server
var server = http.createServer(handleRequest);
console.log('Loading server...');
server.listen(8004, function(){
  console.log('Server started on port 8004');
});

//Then after server call Sockets
let io = require('socket.io').listen(server);
// Register a callback function to run when we have an individual connection
// This is run for each individual user that connects
io.sockets.on('connection', function (socket) {

  connectCounter++;

  console.log("We have a new client: " + socket.id);
  console.log("connectCounter ="+connectCounter);

  //prepare data to send to my recent connected client
  var dataConnection = {
    id: connectCounter,
    message: 'Wait until your tourn'
  };

  socket.emit('idConnectCounter', dataConnection);


  socket.on('disconnect', function() {
    console.log("Client has disconnected");
    connectCounter--;
    console.log("connectCounter ="+connectCounter);
  });

  socket.on('statusMachine', function(data) {
      // Data comes in as whatever was sent, including objects
      console.log("Received: 'statusMachine' " + data.sm + " " + data.id);      
      // Send it to all other clients
      //socket.broadcast.emit('mouse', data);
  });


  ///////////////////////////////////////////////////////////
  //The old CodeAttack id1 or id2 Joystick methods
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

    //TODO Comment this if that can slow down the connectivty. Check latency
    console.log(socket.id + " Emit to " + socket.channel);
    console.log(data);
    console.log(data.sender);
    console.log(data.message);
    //socket.broadcast.to(socket.channel).emit("fromServer", data);

    var PORTUDP = 33333;
    var HOSTUDP = '127.0.0.1';//192.168.43.244//'192.168.43.14';//
    var data_parsed_vector = data.message.split("/");
    console.log('data[0]: '+data_parsed_vector[0]);
    console.log('data[1]: '+data_parsed_vector[1]);

    if(data_parsed_vector[0] == 'X'){
      client.send(data.sender+"/"+data.message, 0, 1+data.sender.length+data.message.length, PORTUDP, HOSTUDP, function(err, bytes) {
      console.log('UDP message sent to ' + HOSTUDP +':'+ PORTUDP);
      if (err) throw err;
      });
    }
    else if(data_parsed_vector[0] == 'Click'){
      client.send(data.sender+"/"+data.message, 0, 1+data.sender.length+data.message.length, PORTUDP, HOSTUDP, function(err, bytes) {
      console.log('UDP message sent to ' + HOSTUDP +':'+ PORTUDP);
      if (err) throw err;
      });
    }

  });


  ///////////////////////////////////////////////////////////
  //Mobile Joystick Data
  socket.on('posJoystick', function(data) {
      // Data comes in as whatever was sent, including objects
      console.log("Received: id["+data.id+"] 'posJoystick' " + data.x + " " + data.y);      

      //TODO Who decide if we are #1 or #2...???
      sendPosJoystickData(data);
  });


  socket.on('clickJoystick', function(data) {
      // Data comes in as whatever was sent, including objects
      console.log("Received: id["+data.id+"] 'clickJoystick' " + data.x + " " + data.y);      

      //TODO Decide if we are #1 or #2...
      sendClickJoystickData(data);
  });

//=================================================
function sendClickJoystickData(data){
    // console.log("hello Click data");
    // console.log(socket.id + " Emit to " + socket.channel);
    // console.log(data);
    //console.log(data.sender);
    //console.log(data.message);

    let messageClickToSend = data.id+"/Click/"+data.a;

    console.log("send to localhost this message ->" + messageClickToSend);
    
    client.send(messageClickToSend, 0, messageClickToSend.length, PORTUDP, HOSTUDP, function(err, bytes) {
      //console.log('UDP message sent to ' + HOSTUDP +':'+ PORTUDP);
      if (err) throw err;
    });
}

//=================================================
function sendPosJoystickData(data){
    // console.log("hello Pos data");
    // console.log(socket.id + " Emit to " + socket.channel);
    // console.log(data);
    ////console.log(data.sender);
    ////console.log(data.message);

    let messageXToSend = data.id+"/X/"+data.x;
    let messageYToSend = data.id+"/Y/"+data.y;
    console.log("send to localhost this message ->" + messageXToSend);
    
    client.send(messageXToSend, 0, messageXToSend.length, PORTUDP, HOSTUDP, function(err, bytes) {
      //console.log('UDP message sent to ' + HOSTUDP +':'+ PORTUDP);
      if (err) throw err;
    });
}

});


//=================================================
function handleRequest(req, res) {
  // What did we request?
  let pathname = req.url;
  
  // If blank let's ask for index.html
  if (pathname == '/') {
    pathname = '/index.html';
  }
  
  // Ok what's our file extension
  let ext = path.extname(pathname);

  // Map extension to file type
  const typeExt = {
    '.html': 'text/html',
    '.js':   'text/javascript',
    '.css':  'text/css'
  };

  // What is it?  Default to plain text
  let contentType = typeExt[ext] || 'text/plain';

  // Now read and write back the file with the appropriate content type
  fs.readFile(__dirname + pathname,
    function (err, data) {
      if (err) {
        res.writeHead(500);
        return res.end('Error loading ' + pathname);
      }
      // Dynamically setting content type
      res.writeHead(200,{ 'Content-Type': contentType });
      res.end(data);
    }
  );
}