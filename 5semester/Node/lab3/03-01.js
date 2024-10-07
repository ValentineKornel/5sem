const http = require('http');
const readline = require('readline');

let state = "norm";

http.createServer(function(req, resp){

    if(req.url ===  && req.method === 'GET'){


        resp.writeHead(200, {'Content-Type': 'text/html'});
        resp.end(
            `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">'/'
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
<h2>${state}</h2>
    
</body>
</html>`
        );
    }else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(5000, '127.0.0.1', () => {
    console.log("Server listening on localhost:5000");
})

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

const askQuestion = () => {
    rl.setPrompt(`${state}->`);
    rl.prompt();
    rl.removeAllListeners('line');

    rl.on('line', (answer) => {
        switch (answer) {
            case 'norm':
            case 'stop':
            case 'test':
            case 'idle':
                console.log(`reg = ${state}--> ${answer}`);
                state = answer;
                break;
            case 'exit':
                rl.close();
                process.exit(0);
            default:
                console.log(`${answer}`);
        }
        askQuestion();
    });
};

askQuestion();