const http = require('http');

exports.del = function(data, done) {
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/databases/' + data.id,
        method: 'DELETE'
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

exports.update = function(data, done) {
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/databases/' + data.database_id + "/update/" + data.storage_id,
        method: 'POST'
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

exports.create = function(data, done) {
    const content = new TextEncoder().encode(JSON.stringify(data));
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/databases',
        method: 'POST',
        headers: {
            'Content-type': 'application/json',
            'Content-length': content.length
        }
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

    req.write(content);
    req.end();
}

exports.getList = function(data, done) {
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/databases',
        method: 'GET'
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
