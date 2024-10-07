const http = require('http');
const url = require('url');
const fs = require('fs');

function asyncFactorial(n, callback) {
    if (n < 0) {
        callback(new Error('Cant be counted for negative nubmers'), null);
        return;
    }

    function computeFactorial(x, acc, cb) {
        if (x === 0 || x === 1) {
            cb(null, acc);
            return;
        }

        process.nextTick(() => {
            computeFactorial(x - 1, acc * x, cb);
        });
    }

    computeFactorial(n, 1, callback);
}

http.createServer(function(req, resp){
    const parsedUrl = url.parse(req.url, true);

    if(req.url === '/' && req.method === 'GET'){
        fs.readFile('./factorial.html', 'utf8', (err, data) =>{
            if(err){
                resp.writeHead(500, {'Content-Type': 'text/plain'});
                resp.end('Internal Server Error');
            } else{
                resp.writeHead(200, {'Content-Type': 'text/html'});
                resp.end(data);
            }
        });       
    } else if(parsedUrl.pathname === '/fact' && req.method === 'GET'){
        let k = parseInt(parsedUrl.query.k, 10);

        if(k < 0){
            k = 0;
        }
        asyncFactorial(k, (err, fact) => {
            if (err) {
                resp.writeHead(400, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'});
                resp.end(JSON.stringify({ error: err.message }));
            } else {
                let respData = {
                    k: k, 
                    fact: fact
                };

                resp.writeHead(200, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'});
                resp.end(JSON.stringify(respData));
            }
        });

    } else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(5000, '127.0.0.1', () => {
    console.log("Server listening on localhost:5000");
});
