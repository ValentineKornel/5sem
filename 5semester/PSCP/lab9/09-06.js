let http = require("http");
let fs = require("fs");

let bound = 'kvv-kvv-kvv';
let body =  `--${bound}\r\n`;
    body += `Content-Disposition:form-data; name="file"; filename="MyFile.txt"\r\n`;
    body += "Content-Type:text/plain\r\n\r\n";
    body += fs.readFileSync('./lab9/MyFile.txt');
    body += (`\r\n--${bound}--\r\n`);


let optinons = {
    host: 'localhost',
    path: "/upload",
    port: 5000,
    method: 'POST',
    headers:{'Content-Type':'multipart/form-data; boundary='+bound}
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

req.end(body);