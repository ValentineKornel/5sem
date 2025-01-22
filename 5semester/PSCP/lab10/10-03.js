const WebSocket = require('ws');
let wsServer = new WebSocket.Server({port: 4000, host: 'localhost', path: '/broadcast'});


wsServer.on('connection', (ws) => {
    console.log(`new connection`);

    ws.on('message', (message) => {
        console.log(`Received message: ${message}`);
        
        wsServer.clients.forEach(client => {
                if(client.readyState == WebSocket.OPEN) client.send(`server: ${message}`);
        })
    })

    ws.on('close', () => console.log('connection closed'));
})

wsServer.on('error', (e) => {console.log('ws server error ', e)});
console.log(`ws server: host: ${wsServer.options.host}, port: ${wsServer.options.port}, path: ${wsServer.options.path}`);