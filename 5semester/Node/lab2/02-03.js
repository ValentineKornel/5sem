const http = require('http');

http.createServer(function(req, resp){

    if(req.url === '/api/name' && req.method === 'GET'){


        resp.writeHead(200, {'Content-Type': 'text/plain'});
        resp.end('Korneliuk Valentine Vladimirovich');
    }else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(3000, '127.0.0.1', () => {
    console.log("Server listening on localhost:3000");
})