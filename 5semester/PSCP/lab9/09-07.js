let http = require("http");
let fs = require("fs");

let bound = 'kvv-kvv-kvv';
let bodyStart =  `--${bound}\r\n`;
    bodyStart += `Content-Disposition:form-data; name="file"; filename="dog.png"\r\n`;
    bodyStart += "Content-Type:image/png\r\n\r\n";
let bodyEnd = (`\r\n--${bound}--\r\n`);


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

req.write(bodyStart);
let stream = new fs.ReadStream('./lab9/dog.png');
stream.on('data', (chunk) =>{ req.write(chunk); console.log(Buffer.byteLength(chunk));});
stream.on('end', () => {req.end(bodyEnd);});
