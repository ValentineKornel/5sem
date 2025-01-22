const http = require('http');
const fs = require('fs');
const path = require('path');
const url = require('url');
const xml2js = require('xml2js');
const mp = require('multiparty');
const { json } = require('stream/consumers');

let server;
let connectionsCounter = 0;

server = http.createServer(function(req, resp){
    const parsedURL = url.parse(req.url, true);
    const pathName =  parsedURL.pathname;
    console.log("Received request:", pathName, req.method);

    if (pathName == '/connection' && req.method == 'GET') {
        let set = parsedURL.query.set;
        if(set != null){
            server.keepAliveTimeout = Number.parseInt(set);
            resp.writeHead(200, {'Content-Type':'text/text; charset=utf-8'});
            resp.end(`Current keepAliveTimeout: ${server.keepAliveTimeout}`);  
        }else{
            resp.writeHead(200, {'Content-Type':'text/text; charset=utf-8'});
            resp.end(`New keepAliveTimeout value set: ${server.keepAliveTimeout}`);   
        }
    }
    else if(pathName == '/headers' && req.method == 'GET'){
        const requestHeaders = JSON.stringify(req.headers, null, 2);
        resp.setHeader('Content-Type', 'application/json; charset=utf-8');
        resp.setHeader('X-Custom-Header', 'MyCustomHeaderValue');
        const responseHeaders = JSON.stringify(resp.getHeaders(), null, 2);

        resp.writeHead(200);
        resp.write(requestHeaders);
        resp.end(responseHeaders);
    }
    else if(pathName == '/parameter' && req.method == 'GET'){
        let x = Number.parseInt(parsedURL.query.x);
        let y = Number.parseInt(parsedURL.query.y);
        if(x != null && y != null && !isNaN(x) && !isNaN(y)){
            resp.writeHead(200, {'Content-Type':'text/plain; charset=utf-8'});
            resp.end(`sum: ${x + y}, diff: ${x - y}, mult: ${x * y}, div: ${x / y}`);  
        }else{
            resp.writeHead(400, {'Content-Type': 'text/text'});
            resp.end('Bad request');
        }
    }
    else if(pathName == '/parameter' && req.method == 'POST'){
        let data = '';

        req.on('data', async (chunk) => {
            data += chunk.toString("utf8");
        })
        req.on('end', async () => {
            console.log(data);
            resp.writeHead(200, {'Content-Type':'text/plain; charset=utf-8'});
            resp.end(data);
        });
        
    }
    else if(pathName.startsWith('/parameter') && req.method == 'GET'){
        const parts = pathName.split('/').filter(Boolean);
        let x = Number.parseInt(parts[1]);
        let y = Number.parseInt(parts[2]);
        if(!isNaN(x) && !isNaN(y)){
            resp.writeHead(200, {'Content-Type':'text/plain; charset=utf-8'});
            resp.end(`sum: ${x + y}, diff: ${x - y}, mult: ${x * y}, div: ${x / y}`);  
        }else{
            resp.writeHead(400, {'Content-Type': 'text/text'});
            resp.end(`Bad request. Path:${pathName}`);
        }
    }
    else if(pathName == '/close' && req.method == 'GET'){
        setTimeout(() => server.close(), 10 * 1000);
        resp.writeHead(200, {'Content-Type':'text/plain; charset=utf-8'});
        resp.end('server closed');  
    }
    else if(pathName == '/socket' && req.method == 'GET'){
        let clientAddress = req.socket.remoteAddress;
        let clientPort = req.socket.remotePort;

        let serverAddress = req.socket.localAddress;
        let serverPort = req.socket.localPort;
        resp.writeHead(200, {'Content-Type':'text/text; charset=utf-8'});
        resp.end(`Client address: ${clientAddress}\nClient port: ${clientPort}\nServer address: ${serverAddress}\nServer port: ${serverPort}`);

    }
    else if(pathName == '/req-data' && req.method == 'POST'){
        let body = '';
        let chunckCounter = 0;
        req.on('data', async chunk => {
            console.log(`\nchunk #${++chunckCounter} received: ${chunk.toString()}\n`);
            body += chunk.toString();
        })

        req.on('end', async () => {
            //console.log(`full request body: ${body}`);
            console.log(`chunk amount: ${chunckCounter}`);
            resp.writeHead(200, {'Content-Type':'text/text; charset=utf-8'});
            resp.end('ok');
        })
    }
    else if(pathName == '/resp-status' && req.method == 'GET'){
        let code = Number.parseInt(parsedURL.query.code);
        let mess = parsedURL.query.mess;
        if(code != null && code >= 100 && code <= 599 && mess != null){
            resp.statusCode = code;
            resp.statusMessage = mess;
            resp.end();
        }else{
            resp.writeHead(400, {'Content-Type': 'text/text'});
            resp.end('Bad request');
        }
    }
    else if(pathName == '/formparameter'){
        if(req.method == 'GET'){
            resp.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            resp.end(`
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Form Parameters</title>
            </head>
            <body>
                <h1>Submit Form</h1>
                <form action="/formparameter" method="POST">
                    <label for="textInput">Text Input:</label>
                    <input type="text" id="textInput" name="textInput"><br><br>

                    <label for="numberInput">Number Input:</label>
                    <input type="number" id="numberInput" name="numberInput"><br><br>

                    <label for="dateInput">Date Input:</label>
                    <input type="date" id="dateInput" name="dateInput"><br><br>

                    <label for="checkboxInput">Checkbox:</label>
                    <input type="checkbox" id="checkboxInput" name="checkboxInput" value="checked"><br><br>

                    <label>Radiobuttons:</label>
                    <input type="radio" name="radioInput" value="Option1" id="radio1"><label for="radio1">Option 1</label>
                    <input type="radio" name="radioInput" value="Option2" id="radio2"><label for="radio2">Option 2</label><br><br>

                    <label for="textarea">Textarea:</label><br>
                    <textarea id="textarea" name="textarea" rows="4" cols="50"></textarea><br><br>

                    <input type="submit" name="submitButton" value="Submit1">
                    <input type="submit" name="submitButton" value="Submit2">
                </form>
            </body>
            </html>
            `);
        }
        if(req.method == 'POST'){   
            let body = '';
            req.on('data', chunk => {
                body += chunk.toString();
            });

            req.on('end', () => {
                console.log(body);
                const parsedParams = {};
                body.split('&').forEach(param => {
                    const [key, value] = param.split('=');
                    parsedParams[key] = value;
                });

                resp.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                resp.end(`
                    <html>
                        <head>
                            <title>Form Results</title>
                        </head>
                        <body>
                            <h1>Form Parameters</h1>
                            <pre>${JSON.stringify(parsedParams, null, 2)}</pre>
                        </body>
                    </html>
                `);
            });
        }
    }
    else if(pathName == '/json' && req.method == 'POST'){
        let body = '';
            req.on('data', chunk => {
                body += chunk;
            });

            req.on('end', () => {
                let reqObj = JSON.parse(body);
                console.log(reqObj);
                let respObj = {};
                respObj.__comment = "Ответ. Лабораторная работа 8/10";
                respObj.x_plus_y = reqObj.x + reqObj.y;
                respObj.Concatination_s_o = `Сообщение: ${reqObj.o.surname}, ${reqObj.o.name}`;
                respObj.Length_m = reqObj.m.length;

                resp.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                resp.end(JSON.stringify(respObj, 0, 2));
            });
    }
    else if(pathName == '/xml' && req.method == 'POST'){
        let body = '';

        req.on('data', chunk => {
            body += chunk.toString();
        });

        req.on('end', () => {
            xml2js.parseString(body, { explicitArray: false }, (err, result) => {
                if (err) {
                    console.error('Error parsing XML:', err.message);
                    resp.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
                    resp.end('Invalid XML');
                    return;
                }

                try {
                   const request = result.request;
                    const xElements = Array.isArray(request.x) ? request.x : [request.x];
                    const sumX = xElements.reduce((sum, elem) => {
                        const value = parseInt(elem.$?.value || '0');
                        return sum + value;
                    }, 0);

                    const mElements = Array.isArray(request.m) ? request.m : [request.m];
                    const concatM = mElements.map(elem => elem.$?.value || '').join('');

                    const builder = new xml2js.Builder();
                    const responseObject = {
                        response: {
                            sum: {
                                $: { element: 'x', result: sumX.toString() }
                            },
                            concat: {
                                $: { element: 'm', result: concatM }
                            }
                        }
                    };

                    const responseXML = builder.buildObject(responseObject);

                    resp.writeHead(200, { 'Content-Type': 'application/xml' });
                    resp.end(responseXML);
                } catch (error) {
                    console.error('Error processing XML:', error.message);
                    resp.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
                    resp.end('Invalid XML structure');
                }
            });
        });
    }
    else if(pathName.startsWith('/files') && req.method == 'GET'){
        const staticDir = path.join(__dirname, 'static');
        const parts = pathName.split('/').filter(Boolean);

        if(parts[1] == undefined){
            fs.readdir(staticDir, (err, files) => {
                if (err) {
                    console.error('Error reading static directory:', err.message);
                    resp.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
                    resp.end('Internal Server Error');
                    return;
                }
    
                // Считаем только файлы
                const fileCount = files.filter(file => {
                    const fullPath = path.join(staticDir, file);
                    return fs.statSync(fullPath).isFile();
                }).length;
    
                resp.writeHead(200, {
                    'Content-Type': 'text/plain; charset=utf-8',
                    'X-static-files-count': fileCount
                });
                resp.end(`Number of files in static directory: ${fileCount}`);
            });
        }else{
            fs.readFile(path.join(staticDir, parts[1]), (err, file)=>{
                if(err){
                    resp.statusCode = 404;
                    resp.statusMessage = 'Resourse not found';
                    resp.end("Resourse not found");
                }
                else{
                    resp.writeHead(200);
                    // resp.writeHead(200, {
                    //     'Content-Disposition': 'attachment;'
                    // });
                    resp.end(file);
                }
            })
        }  
    }
    else if(pathName == '/upload'){
        if(req.method == 'GET'){
            resp.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
            resp.end(fs.readFileSync('./static/task14.html'));
        }
        if(req.method == 'POST'){
            let form = new mp.Form({uploadDir: './static'});
            form.on('file', (name, file) => {
                console.log('file received');
                let oldPath = path.join(__dirname, file.path);
                let newPath = path.join(__dirname, 'static', file.originalFilename);
                fs.renameSync(oldPath, newPath);
            })
            form.on('close', () => {
                resp.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
                resp.end('ok');
            })
            form.parse(req);
        }
    }

    else{
        resp.writeHead(500, {'Content-Type': 'text/text'});
        resp.end('Server error');
    }
})

server.on('connection', (socket) => {
    console.log(`new connection, connectionCounter #${++connectionsCounter}`);
    socket.on('close', () => {
        console.log(`connection #${connectionsCounter} has expired`);
        
    });
})

server.listen(5000);

