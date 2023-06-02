const express = require('express');
const app = express();
const port = 3000;
const fs = require('fs');

const server = require('http').createServer(app);
const io = require('socket.io')(server);

io.on('connection', (socket) => {
  console.log('WebSocket connection established.');

  socket.on('disconnect', () => {
    console.log('WebSocket connection closed.');
  });
});

server.listen(port, () => {
  console.log('Server start: http://127.0.0.1:' + port);
});


const dgram = require('dgram');
const udpServer = dgram.createSocket('udp4');
const udpImgServer = dgram.createSocket('udp4');
const udpImgServer2 = dgram.createSocket('udp4');
const udpPORT = 8080;
const udpImgPORT = 8081;
const udpImgPORT2 = 8082;
const udpHOST = '127.0.0.1';

udpServer.on('message', (msg, rinfo) => {
  io.emit('udpData', JSON.parse(msg.toString()));
});

udpImgServer.on('message', (msg, rinfo) => {
  const buffer = Buffer.from(msg)
  fs.writeFileSync('./public/camera.png', buffer);
  io.emit('udpImgData', "ready");
});

udpImgServer2.on('message', (msg, rinfo) => {
  const buffer = Buffer.from(msg)
  fs.writeFileSync('./public/camera2.png', buffer);
  io.emit('udpImgData2', "ready");
});

udpServer.bind(udpPORT, udpHOST);

udpImgServer.bind(udpImgPORT, udpHOST);

udpImgServer2.bind(udpImgPORT2, udpHOST);

app.set('view engine', 'ejs');

app.use(express.static('public'));

app.get('/', (req, res) => {
  res.setHeader('Cache-Control', 'max-age=0.01');
  res.render('index');
});

// app.listen(port, () => {
//   console.log(`Server running on http://localhost:${port}`);
// });
