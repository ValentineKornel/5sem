const http = require('http');
const url = require('url');
const fs = require('fs');

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
    }else if(parsedUrl.pathname === '/fact' && req.method === 'GET'){

        let k = parsedUrl.query.k.toString();
        let respData;
        try{
            let fact = factorial(k);
            respData = {
            k: k, 
            fact: fact
            };
        }catch(err){
            respData = {
                k:k,
                fact: 0
            };
        }

        resp.writeHead(200, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'});
        resp.end(JSON.stringify(respData));
    }else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(5000, '127.0.0.1', () => {
    console.log("Server listening on localhost:5000");
})

function factorial(n) {
    if (n < 0) {
        throw new Error('Cant be counted for negative nubmers');
    }
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}