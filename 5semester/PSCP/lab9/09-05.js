let http = require("http");
let { Builder } = require('xml2js');

let jsObject = {
    request: {
        x: [{ $: { value: 1 } }, { $: { value: 2 } }],
        m: [{ $: { value: 'a' } }, { $: { value: 'b' } }, { $: { value: 'c' } }]
    }
};
let builder = new Builder();
let xml = builder.buildObject(jsObject);

let optinons = {
    host: 'localhost',
    path: "/xml",
    port: 5000,
    method: 'POST'
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

req.write(xml.toString());
req.end();