require('dotenv').config();

const cors = require('cors')

const db = require('./db')

const port = process.env.PORT;

const express = require('express');

const app = express();

app.use(express.json())

app.use(cors) 

// routes
app.get("/", (req, res) => {
    res.json({
        message: "working"
    })
});
// list all the tasks
app.get("/tasks", async (req, res) => {
    const tasks = await db.selectTasks();
    res.json(tasks)
})
// front creates a new task
app.post("/tasks", async (req, res) => {
    await db.insertTask(req.body);
    res.sendStatus(201)
})
// front delete a new task
app.delete("/tasks/:id", async (req, res) => {
    await db.dropTask(req.params.id);
    res.sendStatus(200)
})
// front updates task status
app.put("/tasks/:id", async (req, res) => {
    await db.updateStatus(req.params.id, req.body.completed); 
})

app.listen(port);

console.log(`Server open on port ${port}`)