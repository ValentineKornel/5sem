let http = require("http");
let fs = require("fs");

let file = fs.createWriteStream('./lab9/downloads/image.png');

let optinons = {
    host: 'localhost',
    path: "/files/dog.png",
    port: 5000,
    method: 'GET',
}

const req = http.request(optinons, (resp) => {
    resp.pipe(file);
});

req.on('error', (e) => {
    console.log("http request error: " + e.message);
})

req.end();
