
/*var io = require('socket.io');

var express = require('express');

var app = express(),
 server = require('http').createServer(app) ,
  io = io.listen(server);

server.listen(3050);
*/

/*
var io = require('socket.io').listen(8000);

io.sockets.on('connection', function (socket) {

  socket.emit('news', { hello: 'world' });

  socket.on('REG', function (data) {
    console.log("Rec from client REG "+data);
  });

  socket.on('MSG', function (data) {
      console.log("Rec from client MSG "+JSON.stringify(data));
  });

});
*/

var express = require('express');
var app = express();
var path = require('path');
var server = require('http').createServer(app);
//var io = require('socket.io')(server);
var io = require('socket.io');
io = io.listen(server);
var port = process.env.PORT || 3060;

server.listen(port, function () {
  console.log('Server listening at port %d', port);
});

app.use(express.static(path.join(__dirname, 'public')));

io.sockets.on('connection', function (socket) {

  console.log("conectado");
  socket.emit('news', { hello: 'world hola mundo xd' });

  socket.on('REG', function (data) {
    console.log("Rec from client REG "+data);
    //socket.emit('prueba', { hello: 'prueba hola mundo xd emitsion' });
    socket.emit('prueba','prueba hola mundo xd emitsion string');
  });

  socket.on('MSG', function (data) {
      console.log("Rec from client MSG "+JSON.stringify(data));
  });

});