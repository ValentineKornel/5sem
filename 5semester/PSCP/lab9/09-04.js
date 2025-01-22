let http = require("http");

let params = JSON.stringify({
    __comment: "Запрос. Лабораторная работа 9/4",
    x: 3,
    y: 4,
    o: {
        name: "Valentine",
        surname: "Korneliuk"
    },
    m: [1, 2, 3, 4]
});

let optinons = {
    host: 'localhost',
    path: "/json",
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
        console.log(JSON.parse(data));
    });
});

req.on('error', (e) => {
    console.log("http request error: " + e.message);
})

req.write(params);
req.end();