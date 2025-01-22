const http = require('http');
const fs = require('fs');


let httpServer = http.createServer((req, resp) => {
    if(req.method === 'GET' && req.url === '/start'){
        resp.writeHead(200, {'content-type': 'text/html; charset=utf-8'});
        resp.end(fs.readFileSync('./lab10/10-01.html'));
    }else{
        resp.writeHead(400);
        resp.end();
    }
})
httpServer.listen(3000, () => {console.log('http server listening port 3000');});

const WebSocket = require('ws');
let wsServer = new WebSocket.Server({port: 4000, host: 'localhost', path: '/wsserver'});


let m = 0;
wsServer.on('connection', (ws) => {
    console.log(`new connection`);
    let serverId = ++m; 
    let k = 0;
    let n;

    ws.on('message', (message) => {
        console.log(`Received message: ${message}`);
        n = message.toString().split(':')[1].trim();
    })

    setInterval(() => ws.send(`10-01-server${serverId}: ${n}->${++k}`), 5000);

    ws.on('close', () => console.log('connection closed'));
})


wsServer.on('error', (e) => {console.log('ws server error ', e)});
console.log(`ws server: host: ${wsServer.options.host}, port: ${wsServer.options.port}, path: ${wsServer.options.path}`);