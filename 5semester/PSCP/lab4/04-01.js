const http = require('http');
const url = require('url');
const db = require('./db');

http.createServer(function(req, resp){
    const parsedUrl = url.parse(req.url, true);

    if(parsedUrl.pathname === '/api/db'){

        switch(req.method){
            case 'GET':{

                resp.writeHead(200, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'});
                resp.end(JSON.stringify(db.select()));

                break;
            }
            case 'POST':{
                let body = '';

                req.on('data', chunk => {
                    body += chunk.toString();
                });

                req.on('end', () => {
                    try{
                        let parsedBody = JSON.parse(body);
                        let newItem = db.insert(parsedBody);

                        resp.writeHead(200, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'});
                        resp.end(`${JSON.stringify(newItem)}\n inserted successfully`);
                        console.log(db.select());
                    }catch (error) {
                        resp.writeHead(400, { 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*' });
                        resp.end(JSON.stringify({ error: 'Invalid JSON' }));
                    }
                })
                break;
            }
            case 'PUT':{
                let body = '';

                req.on('data', chunk => {
                    body += chunk.toString();
                });

                req.on('end', () => {
                    try{
                        let parsedBody = JSON.parse(body);
                        db.update(parsedBody);

                        resp.writeHead(200, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'});
                        resp.end("updated successfully");
                        console.log(db.select());
                    }catch (error) {
                        resp.writeHead(400, { 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*' });
                        resp.end(JSON.stringify({ error: 'Invalid JSON' }));
                    }
                })
                break;
                break;
            }
            case 'DELETE':{

                let id = parsedUrl.query.id;
                let item = db.delete(id);

                resp.writeHead(200, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'});
                resp.end(`${JSON.stringify(item)}\n deleted successfully`);
                console.log(db.select());
                break;
            }
            default:{
                resp.writeHead(404, {'Content-Type': 'text/plain'});
                resp.end('404 Not Found');
            }
        }

    }else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(5000, '127.0.0.1', () => {
    console.log("Server listening on localhost:5000");
})
