const WebSocket = require('ws');

const ws = new WebSocket('ws://localhost:4000/wsserver');

ws.on('open', () => {
    let n = 0;

    let intervalId = setInterval(() => {
        ws.send(`10-02-client: ${++n}`);
    }, 3000);

    ws.on('message', (message) => {
        console.log(`Received message: ${message}`);
    })

    setTimeout(() => {
        clearInterval(intervalId);
        ws.close();
    }, 25000);
})
