const http = require('http');
const querystring = require('querystring');

exports.search = function(database_id, data, done) {
    const encodedQuery = querystring.stringify(data);
    
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/databases/' + database_id + '/search?' + encodedQuery,
        method: 'GET',
    };

    const req = http.request(options, function(res) {
        let output = null;

        res.on("data", function(chunk) {
            if (output === null) {
                output = chunk;
            } else {
                output += chunk;
            }
        });

        res.on("end", function() {
            done(null, output);
        });
    });

    req.on('error', function(error) {
        done(error, null);
    });

    req.end();
}

exports.scan = function(database_id, data, done) {
    const encodedQuery = querystring.stringify(data);
    
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/databases/' + database_id + '/scan?' + encodedQuery,
        method: 'GET',
    };

    const req = http.request(options, function(res) {
        let output = null;

        res.on("data", function(chunk) {
            if (output === null) {
                output = chunk;
            } else {
                output += chunk;
            }
        });

        res.on("end", function() {
            done(null, output);
        });
    });

    req.on('error', function(error) {
        done(error, null);
    });

    req.end();
}