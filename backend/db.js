async function connect() {
    if(global.connection)
        return global.connection.connect();

    const { Pool } = require('pg')
    const pool = new Pool({
        connectionString: process.env.CONNECTION_STRING
    }) 

    const client = await pool.connect()
    console.log('conected to postgree succesfuly')

    const res = await client.query("select now()");
    console.log(res.rows[0])
    client.release()

    global.connection = pool;
    return pool.connect();

    console.log('backend logged')
}

connect();

async function dropTask(id) {
    const client = await connect();
    const sql = "DELETE FROM tasks WHERE id=$1";
    const res = await client.query(sql, [id])
}

async function insertTask(task) {
    console.log(task)
    const client = await connect();
    const sql = "INSERT INTO tasks(name, completed) VALUES ($1, $2)";
    const res = await client.query(sql, [task.name, task.completed])
}

async function selectTasks() {
    const client = await connect();
    const res = await client.query("SELECT * FROM tasks")
    return res.rows;
} 

async function updateStatus(id, completed) {    
    console.log(id)
    const client = await connect();
    const res = await client.query("UPDATE tasks SET completed=$1 WHERE id=$2", [completed, id])
}

module.exports = {
    selectTasks,
    insertTask,
    dropTask,
    updateStatus
}