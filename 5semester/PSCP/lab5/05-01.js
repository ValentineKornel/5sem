const http = require('http');
const url = require('url');
const fs = require('fs');
const path = require('path');
const events = require('events');
const util = require('util');
const readline = require('readline');
const { clearTimeout } = require('timers');

const db = require('./db.js');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})

let timerId = -1
let intervalId = -1
let statisticTimerId = -1
let statisticCollectorActive = false

let statistics = {
    start: null,
    finish: null,
    request: 0,
    commit: 0,
}


rl.on('line',(input) => {
    let parseCommandLine = input.split(' ');

    let command = parseCommandLine[0];
    let arg = parseCommandLine[1] == null ? null : parseCommandLine[1];

    console.log(command + ' ' + arg);

    switch(command){
        case('sd'):{
            if(arg !== null){
                console.log(`Server will shutdown in ${arg} s`);
                clearTimeout(timerId);
                timerId = setTimeout(() => process.exit(), arg * 1000);
            }else{
                clearTimeout(timerId);
                console.log("server shutdown canseled");
            }
            break;
        }
        case('sc'):{
            if(arg !== null){
                console.log('Start commiting interval');
                intervalId = setInterval(() => {
                    if(statisticCollectorActive){
                        statistics.commit++;
                    }
                    db.commit();
                }, arg * 1000);
                intervalId.unref();
            } else {
                console.log('Stop commiting interval');
                clearTimeout(intervalId);
            }
            break;
        }
        case('ss'):{
            if(arg != null){
                statisticCollectorActive = true;
                console.log('Start collecting statistic');
                statistics.start = new Date().toISOString().split('T')[0];
                statistics.finish = null;
                statistics.request = 0;
                statistics.commit = 0;

                clearTimeout(statisticTimerId);
                statisticTimerId = setTimeout(() => {
                    statistics.finish = new Date().toISOString().split('T')[0];
                    console.log('End of statistic collecting');
                    console.log(statistics);
                    statisticCollectorActive = false;
                }, arg * 1000);
                statisticTimerId.unref();
            } else {
                clearTimeout(statisticTimerId);
                statistics.finish = new Date().toISOString().split('T')[0];
                console.log('End of statistic collecting');
                console.log(statistics);
                statisticCollectorActive = false;
            }
        } break;
        default:{
            console.log('Not a command');
        } break;
    }
})


db.on('GET', async (req, resp) => {

    if(statisticCollectorActive){
        statistics.request++;
    }
    resp.writeHead(200, {'Content-Type': 'application/json'});
    resp.end(JSON.stringify(await db.select()));
})

db.on('POST', async (req, resp) => {
    let body = '';

    if(statisticCollectorActive){
        statistics.request++;
    }

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

    if(statisticCollectorActive){
        statistics.request++;
    }

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
    if(statisticCollectorActive){
        statistics.request++;
    }

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
    let pathname = parsedUrl.pathname;


    if(pathname === '/' && req.method === 'GET'){

        fs.readFile(path.join(__dirname, '/index.html'), (err, html) => {
            if(err){
                resp.writeHead(500, {'Content-Type': 'text/text'});
                resp.end('Server error');
            } else {
                resp.writeHead(200, {'Content-Type':'text/html; charset=utf-8'});
                resp.end(html);
            }
        })
    }
    else if(pathname === '/api/db'){

        db.emit(req.method, req, resp);
    
    }
    else if (pathname === '/api/ss' && req.method === 'GET'){
        if(statisticCollectorActive){
            statistics.finish = null;
        }
        let jsonStat = JSON.stringify(statistics);
        resp.writeHead(200, {'Content-Type': 'aplication/json'});
        resp.end(jsonStat);
    }
    else {
        resp.writeHead(404, {'Content-Type': 'text/plain'});
        resp.end('404 Not Found');
    }
    
}).listen(5000, '127.0.0.1', () => {
    console.log("Server listening on localhost:5000");
})
