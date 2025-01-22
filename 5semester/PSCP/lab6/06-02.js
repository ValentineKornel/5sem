const fs = require('fs');
const url = require('url');
const http = require('http');
const sendmail = require('sendmail')({silent: true, smtpPort: 25, smtpHost: 'smtp.mailto.plus'});

let http_handler = (req, resp) => {
    const parsedUrl = url.parse(req.url, true);

    if(parsedUrl.pathname === '/' && req.method === 'GET'){

        fs.readFile('./form.html', (err, html) => {
            if(err){
                resp.writeHead(500, {'Content-Type': 'text/text'});
                resp.end('Server error');
            } else {
                resp.writeHead(200, {'Content-Type':'text/html; charset=utf-8'});
                resp.end(html);
            }
        })
    }
    else if(parsedUrl.pathname === '/' && req.method === 'POST'){
        let body = '';

        req.on('data', chunk => {
            body += chunk.toString();
        });

        req.on('end', () => {
            let parsedBody = JSON.parse(body);
            console.log(parsedBody);
            sendmail({
                from:parsedBody.from,
                to:parsedBody.to,
                subject:'test sendmail',
                html: `<h1>${parsedBody.message}</h1>`
            }, (err, reply) => {
                if (err) {
                    console.log(err && err.stack);
                    console.dir(reply);
                    resp.writeHead(500, {'Content-Type': 'text/text'});
                    resp.end('Server error');
                } else {
                    resp.writeHead(200, {'Content-Type': 'text/text'});
                    resp.end("message sent");
                }
            })
        })
    
    }else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }

}

let server = http.createServer(http_handler);
server.listen(5000, '127.0.0.1', () => {
    console.log("Server listening on localhost:5000");
})