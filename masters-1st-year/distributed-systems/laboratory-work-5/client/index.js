const axios = require('axios')
const fs = require('fs')
const querystring = require('querystring');
const FormData = require('form-data')

class Application {

    url = "http://localhost:3000/api/v1"

    async del_repo(id) {
        console.log(`Removed repo: ${id}`)

        const response = await axios.delete(`${this.url}/repositories/${id}`)

        if (response.status !== 200) {
            return -1
        }

        console.log(response.data)
    }

    async clear_reps() {
        console.log("Clear repos!");

        const response = await axios.get(this.url + "/repositories")

        if (response.status !== 200) {
            return -1
        }

        for (const id of response.data.data.map(r => r.id)) {
            this.del_repo(id)
        }

        return 0
    }

    async create_repo(name, type) {
        let request = { "name": name, "type": type }

        const response = await axios.post(this.url + "/repositories", request);

        if (response.status !== 201) {
            return -1
        }

        console.log(response.data)

        let res = response.data.data.id
        console.log(`Create repo ${name}: ${res}`)

        return res
    }

    async commit(id) {
        console.log(`Commit repo: ${id}`);

        const response = await axios.post(`${this.url}/repositories/${id}/commit`)

        if (response.status !== 200) {
            return -1
        }

        console.log(response.data)
    }


    async create_db(id, name) {
        let request = { "name": name, "repository_id": id }

        const response = await axios.post(this.url + "/databases", request)

        if (response.status !== 201) {
            return -1
        }

        console.log(response.data)

        let res = response.data.data.id
        console.log(`Create db ${name}: ${res}`)
        
        return res
    }

    async create_storage(databaseId, filename) {
        const formData = new FormData();
        formData.append('database_id', databaseId);
        formData.append(filename, fs.createReadStream(filename));

        const response = await axios.post(this.url + "/storages", formData, {
            headers: formData.getHeaders()
        });
        
        if (response.status !== 201) {
            console.log(response.data)
            return -1
        }
        
        console.log(response.data)
        let res = response.data.data.id
        console.log(`Create a storage for ${filename}: ${res}`);
        
        return res
    }

    async update_db(db_id, storage_id) {
        console.log(`Update a db ${db_id} for storage ${storage_id}`);

        const response = await axios.post(`${this.url}/databases/${db_id}/update/${storage_id}`)
        
        if (response.status !== 200) {
            console.log(response.data)
            return -1
        }
        
        console.log(response.data)
    }

    async search(id, query, recordSchema) {
        console.log(`Search in db: ${id}`);

        const encodedQuery = querystring.stringify({ type: "PQF", query: query, recordSchema: recordSchema });

        const response = await axios.get(`${this.url}/databases/${id}/search?` + encodedQuery)

        if (response.status !== 200) {
            return null
        }

        if (response.data.data.success === false) {
            return response.data.data
        }

        return response.data.data.data
    }

    async scan(id, request) {
        console.log(`Scan in db: ${id}`);

        const encodedQuery = querystring.stringify({ type: "PQF", scanClause: request, number: "5", position: "1" });

        const response = await axios.get(`${this.url}/databases/${id}/scan?` + encodedQuery);

        if (response.status !== 200) {
            return null
        }

        if (response.data.data.success === false) {
            return response.data.data
        }

        return response.data.data.data
    }

    async update_record(id, record) {
        const response = await axios.post(`${this.url}/databases/${id}/updateRecord`, { record: record })

        if (response.status !== 200) {
            console.log(response.data)
            return null
        }

        if (response.data.data.success === false) {
            return response.data.data
        }
    }

    async sync(id_1, id_2) {
        console.log(`Sync dbs: ${id_1} and ${id_2}`);
        
        let times = await this.scan(id_1, "@1=1011 @5=1 1970")
    
        for (let t of times.terms.map(t => t.displayTerm)) {
            let result = await this.search(id_1, `@1=1011 @5=1 ${t}`, "dc")
            
            for (let r of result.records) {
                await this.update_record(id_2, r.recordData)
            }
        }
    }

}

async function start() {
    let app = new Application();

    await app.clear_reps()

    console.log("--------------- PREPARING ---------------")

    // prepare the first db
    let repo_1 = await app.create_repo("r1", "fit.nsu.ru.collection")
    let db_1 = await app.create_db(repo_1, "db1")
    let storage_1 = await app.create_storage(db_1, "./collection-1.xml")
    await app.update_db(db_1, storage_1)

    // prepare the second db
    let db_2 = await app.create_db(repo_1, "db2")
    let storage_2 = await app.create_storage(db_2, "./collection-2.xml")
    await app.update_db(db_2, storage_2)

    await app.commit(repo_1)

    // prepare the third db
    let repo_2 = await app.create_repo("r2", "fit.nsu.ru")
    let db_3 = await app.create_db(repo_2, "db3")

    console.log("--------------- SYNC ---------------")

    // sync dbs
    await app.sync(db_1, db_3)
    await app.sync(db_2, db_3)

    console.log("--------------- TEST ---------------")

    // test
    console.log(await app.search(db_3, "@1=4 нгу", "dc"))
    console.log(await app.scan(db_3, "@1=4 нгу"))

    console.log(await app.search(db_3, "@1=4 фит", "dc"))
    console.log(await app.scan(db_3, "@1=4 фит"))

    console.log("--------------- CLEAN ---------------")

    // clean
    await app.del_repo(repo_1)
    await app.del_repo(repo_2)
}

start().then(() => console.log(''))