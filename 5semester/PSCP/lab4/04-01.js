const http = require('http');
const url = require('url');
const events = require('events')
const db = require('./db.js');

db.on('GET', async (req, resp) => {
    resp.writeHead(200, {'Content-Type': 'application/json'});
    resp.end(JSON.stringify(await db.select()));
})

db.on('POST', async (req, resp) => {
    let body = '';

    req.on('data', async chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        try{
            let parsedBody = JSON.parse(body);
            await db.insert(parsedBody);

            resp.writeHead(200, {'Content-Type': 'application/json'});
            resp.end(body);

            console.log(await db.select());
        }catch (error) {
            resp.writeHead(400, { 'Content-Type': 'application/json'});
            resp.end(JSON.stringify({ error: 'Invalid JSON' }));
        }
    })
})

db.on('PUT', async (req, resp) => {
    let body = '';

    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        try{
            let parsedBody = JSON.parse(body);
            let updatedItem = await db.update(parsedBody);

            if(updatedItem){
                resp.writeHead(200, {'Content-Type': 'application/json'});
                resp.end("updated successfully");

                console.log(await db.select());
            }else{
                resp.writeHead(404, {'Content-Type': 'application/json'});
                resp.end(`Item with id ${parsedBody.id} not found`);
            }
        }catch (error) {
            resp.writeHead(400, { 'Content-Type': 'application/json'});
            resp.end(JSON.stringify({ error: 'Invalid JSON' }));
        }
    })
})

db.on('DELETE', async (req, resp) => {
    const parsedUrl = url.parse(req.url, true);
    let id = parsedUrl.query.id;

    let deletedItem = await db.delete(id);

    if (deletedItem) {
        resp.writeHead(200, {'Content-Type': 'application/json'});
        resp.end(JSON.stringify(deletedItem));
    } else {
        resp.writeHead(404, {'Content-Type': 'application/json'});
        resp.end(`Item with id ${id} not found`);
    }

    console.log(await db.select());
})

http.createServer(function(req, resp){
    const parsedUrl = url.parse(req.url, true);

    if(parsedUrl.pathname === '/api/db'){

        db.emit(req.method, req, resp);
    
    }else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(5000, '127.0.0.1', () => {
    console.log("Server listening on localhost:5000");
})
