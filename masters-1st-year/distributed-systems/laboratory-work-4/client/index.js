require('dotenv').config();

const repositories = require('./src/repositories');
const databases = require('./src/databases');
const storages = require('./src/storages');
const requests = require('./src/requests');

var repository;
var database;
var storage;

new Promise(function(resolve, reject) {
    repositories.getList(null, function(err, data) {
        if (err) {
            reject(err);
        } else {
            console.log("\nREPOS BEFORE:");
            console.dir(JSON.parse(data), {depth: null, colors: true})
            resolve(data);
        }
    });
}).then(function(data) {
    return new Promise(function(resolve, reject) {
        repositories.create({name: "r1", type: "fit.nsu.ru"}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}).then(function(data) {
    const p = JSON.parse(data.toString("UTF-8"));
    repository = p.data;
    return new Promise(function(resolve, reject) {
        databases.create({name: "db1", repository_id: repository.id}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}).then(function(data) {
    const p = JSON.parse(data.toString("UTF-8"));
    database = p.data;
    return new Promise(function(resolve, reject) {
        storages.create({database_id: database.id}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}).then(function(data) {
    const p = JSON.parse(data.toString("UTF-8"));
    storage = p.data;
    return new Promise(function(resolve, reject) {
        databases.update({database_id: database.id, storage_id : storage.id}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}).then(function(data) {
    return new Promise(function(resolve, reject) {
        requests.search(database.id, {type : "CQL", query: "dc.title=нгу", recordSchema : "dc"}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                console.log("\nSEARCH RESULT:");
                console.dir(JSON.parse(data), {depth: null, colors: true})
                resolve(data);
            }
        });
    });
}).then(function(data) {
    return new Promise(function(resolve, reject) {
        requests.scan(database.id, {type : "CQL", scanClause: "dc.title=нгу"}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                console.log("\nSCAN RESULT:");
                console.dir(JSON.parse(data), {depth: null, colors: true})
                resolve(data);
            }
        });
    });
}).then(function(data) {
    return new Promise(function(resolve, reject) {
        storages.del({storage_id: storage.id}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}).then(function(data) {
    return new Promise(function(resolve, reject) {
        databases.del({id: database.id}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}).then(function(data) {
    return new Promise(function(resolve, reject) {
        repositories.del({id: repository.id}, function(err, data) {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}).then(function(data) {
    return new Promise(function(resolve, reject) {
        repositories.getList(null, function(err, data) {
            if (err) {
                reject(err);
            } else {
                console.log("\nREPOS AFTER:");
                console.dir(JSON.parse(data), {depth: null, colors: true})
                resolve(data);
            }
        });
    });
}).catch(function(error) {
    console.error(error);
});