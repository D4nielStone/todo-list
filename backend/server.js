const express = require('express');
const cors = require('cors'); // front-to-back end bridge
const app = express();
require('dotenv').config();

const PORT = process.env.PORT || 3000;
app.use(cors());
app.use(express.json());

// temporary database
let tasks = [];

// GET - list tasks
app.get('/tasks', (req, res) => {
    console.log(`[GET] /tasks - tasks lenght: ${tasks.length}`);
    res.json(tasks);
});

// POST - creates a new task
app.post('/tasks', (req, res) => {
    const { title } = req.body;
    
    const newTask = {
        id: Date.now().toString(), // uid based on timestep
        title: title,
        completed: false
    };
    
    tasks.push(newTask);
    
    console.log(`[POST] /tasks - task created.`);
    console.log(` -> ID: ${newTask.id}`);
    console.log(` -> Title: "${newTask.title}"`);
    
    res.status(201).json(newTask);
});

// PUT - update task status
app.put('/tasks/:id', (req, res) => {
    const { id } = req.params;
    const { completed } = req.body;
    
    const task = tasks.find(t => t.id === id);
    
    if (!task) {
        console.log(`[PUT] /tasks/${id} - Error: task not found`);
        return res.status(404).json({ error: 'task not found' });
    }
    
    task.completed = completed;
    
    console.log(`[PUT] /tasks/${id} - Status changed`);
    console.log(` -> Title: "${task.title}"`);
    console.log(` -> Status: ${task.completed}`);
    
    res.json(task);
});

// DELETE - remove task
app.delete('/tasks/:id', (req, res) => {
    const { id } = req.params;
    
    const taskIndex = tasks.findIndex(t => t.id === id);
    
    if (taskIndex === -1) {
        console.log(`[DELETE] /tasks/${id} - Error: task not found`);
        return res.status(404).json({ error: 'Task not found' });
    }
    
    const removedTask = tasks.splice(taskIndex, 1);
    
    console.log(`[DELETE] /tasks/${id} - Task removed.`);
    console.log(` -> Title: "${removedTask[0].title}"`);
    
    res.status(204).send();
});

app.listen(PORT, () => {
    console.log('====================================');
    console.log(` Server started on port "${PORT}"".`);
    console.log('====================================');
});