let http = require("http");
let query = require("querystring");

let params = query.stringify({x:3, y:4});
let path =  `/parameter?${params}`;

let optinons = {
    host: 'localhost',
    path: path,
    port: 5000,
    method: 'GET'
}

const req = http.request(optinons, (resp) => {
    console.log("response status code: " + resp.statusCode);

    let data = '';

    resp.on('data', (chunk) => {
        data += chunk.toString('utf8');
    });

    resp.on('end', () => {
        console.log(data);
    });
});

req.on('error', (e) => {
    console.log("http request error: " + e.message);
})
req.end();