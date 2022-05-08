const FormData = require('form-data');
const http = require('http');
const fs = require('fs');

exports.del = function(data, done) {
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/storages/' + data.storage_id,
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

exports.create = function(data, done) {
    const form = new FormData();
    form.append('data', fs.createReadStream(process.env.DATASET));
    form.append('database_id', data.database_id);
    form.append('addinfo', data.addinfo || '');
    form.submit({
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/storages',
        method: 'POST'
    }, function(err, res) {
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
        res.resume();
    });
}

exports.getList = function(data, done) {
    const options = {
        hostname: process.env.HOSTNAME,
        port: process.env.PORT,
        path:  process.env.APIPATH + '/storages',
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
