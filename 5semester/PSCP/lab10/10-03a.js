const WebSocket = require('ws');

let parm2 = process.argv[2];

let clientMessage = parm2 === 'undefined' ? 'defauld message' : parm2;

const ws = new WebSocket('ws://localhost:4000/broadcast');

ws.on('open', () => {
    
    let intervalId = setInterval(() => {
        ws.send(`10-02-client: ${clientMessage}`);
    }, 3000);

    ws.on('message', (message) => {
        console.log(`Received message: ${message}`);
    })

    setTimeout(() => {
        clearInterval(intervalId);
        ws.close();
    }, 25000);
})
