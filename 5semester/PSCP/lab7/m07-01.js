let fs = require('fs');
const path = require('path');

function Stat(sfn = '/static'){
    this.STATIC_FILDER = sfn;
    let pathStatic = (fn) => {return `${__dirname}${this.STATIC_FILDER}${fn}`;}

    this.writeHTTP404 = (res) => {
        res.statusCode = 404;
        res.statusMessage = 'Resourse not found';
        res.end("Resourse not found");
    }

    this.isStatic = (ext, fn) => {let reg = new RegExp(`^\/.+\.${ext}$`); return reg.test(fn);}

    this.sendFile = (req, resp, headers) => {

        console.log(pathStatic(req.url));
        fs.readFile(pathStatic(req.url), (err, file)=>{
            if(err){
                this.writeHTTP404(resp)
            }
            else{
                resp.writeHead(200, 'OK', headers);
                resp.end(file);
            }
        })
    }
}

module.exports = (parm)=> {return new Stat(parm);}
