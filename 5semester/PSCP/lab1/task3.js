const http = require("http");

http.createServer(function(request, response){
    let body = '';

    request.on('data', data => {
        body += data.toString();
    });
    

    request.on('end', () =>{
        response.write(`<p>Request method: ${request.method}</p>`);
        response.write(`<p>Uri: ${request.url}</p>`);
        response.write(`<p>Protocol version: ${request.httpVersion}</p>`);
        response.write(`<p>Headers: `);
        Object.entries(request.headers).map(([key, value]) => 
        response.write(`<li>${key}:${value}</li>`));
        response.write(`<p>Body: ${body}</p>`);
        response.end();
    })
    

}).listen(3000, "127.0.0.1", function(){
    console.log("Server listening port 3000");
});