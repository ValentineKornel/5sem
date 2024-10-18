const http = require('http');
const fs = require('fs');

http.createServer(function(req, resp){

    if(req.url === '/html' && req.method === 'GET'){
        fs.readFile('./index.html', 'utf8', (err, data) =>{
            if(err){
                resp.writeHead(500, {'Content-Type': 'text/plain'});
                resp.end('Internal Server Error');
            } else{
                resp.writeHead(200, {'Content-Type': 'text/html'});
                resp.end(data);
            }
        });
    }else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(3000, '127.0.0.1', () => {
    console.log("Server listening on localhost:3000");
})