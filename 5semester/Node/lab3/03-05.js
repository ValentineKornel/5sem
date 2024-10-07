const http = require('http');
const url = require('url');
const fs = require('fs');

function asyncFactorial(n, callback) {
    if (n < 0) {
        callback(new Error('Факториал не определён для отрицательных чисел'), null);
        return;
    }

    function computeFactorial(x, acc, cb) {
        if (x === 0 || x === 1) {
            cb(null, acc);
            return;
        }

        setImmediate(() => {
            computeFactorial(x - 1, acc * x, cb);
        });
    }

    computeFactorial(n, 1, callback);
}

const server = http.createServer((req, resp) => {
    const parsedUrl = url.parse(req.url, true);

    if (parsedUrl.pathname === '/' && req.method === 'GET') {
        fs.readFile('./factorial.html', 'utf8', (err, data) => {
            if (err) {
                resp.writeHead(500, {'Content-Type': 'text/plain'});
                resp.end('Internal Server Error');
            } else {
                resp.writeHead(200, {'Content-Type': 'text/html'});
                resp.end(data);
            }
        });
    } else if (parsedUrl.pathname === '/fact' && req.method === 'GET') {
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
});

const PORT = 5000;
server.listen(PORT, '127.0.0.1', () => {
    console.log(`Server listening on http://localhost:${PORT}`);
});
