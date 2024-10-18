const http = require("http");

http.createServer(function(request, response){
    
    response.end("<h1>Hello World!</h1>");

}).listen(3000, "127.0.0.1", function(){
    console.log("Server listening port 3000");
});