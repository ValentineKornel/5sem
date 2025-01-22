const sendmail = require('sendmail')({silent: true, smtpPort: 25, smtpHost: 'localhost'});

let send = function (message){
    sendmail({
        from:'valentine.korneliuk@mail.ru',
        to:'zetakg@mailto.plus',
        subject:'test sendmail',
        html: message
    }, (err, reply) => {
        if (err) {
            console.log('error sending message');
            console.log(err && err.stack);
            console.dir(reply);
        } else {
            console.log('message sent successfully');
        }
    })
}

module.exports = send;