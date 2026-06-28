const express = require('express');
const cors = require('cors'); // Importante para permitir que o front-end acesse a API
const app = express();

app.use(cors());
app.use(express.json());

// Banco de dados temporário
let tasks = [];

// GET - Listar todas as tarefas
app.get('/tasks', (req, res) => {
    console.log(`[GET] /tasks - Solicitando lista de tarefas. Total atual: ${tasks.length}`);
    res.json(tasks);
});

// POST - Criar uma nova tarefa
app.post('/tasks', (req, res) => {
    const { title } = req.body;
    
    const newTask = {
        id: Date.now().toString(), // ID único baseado no timestamp
        title: title,
        completed: false
    };
    
    tasks.push(newTask);
    
    console.log(`[POST] /tasks - Tarefa criada com sucesso!`);
    console.log(` -> ID: ${newTask.id}`);
    console.log(` -> Título: "${newTask.title}"`);
    
    res.status(201).json(newTask);
});

// PUT - Atualizar o status da tarefa (completada ou não)
app.put('/tasks/:id', (req, res) => {
    const { id } = req.params;
    const { completed } = req.body;
    
    const task = tasks.find(t => t.id === id);
    
    if (!task) {
        console.log(`[PUT] /tasks/${id} - Erro: Tarefa não encontrada.`);
        return res.status(404).json({ error: 'Tarefa não encontrada' });
    }
    
    task.completed = completed;
    
    console.log(`[PUT] /tasks/${id} - Status alterado.`);
    console.log(` -> Título: "${task.title}"`);
    console.log(` -> Concluída: ${task.completed}`);
    
    res.json(task);
});

// DELETE - Remover uma tarefa
app.delete('/tasks/:id', (req, res) => {
    const { id } = req.params;
    
    const taskIndex = tasks.findIndex(t => t.id === id);
    
    if (taskIndex === -1) {
        console.log(`[DELETE] /tasks/${id} - Erro: Tarefa não encontrada.`);
        return res.status(404).json({ error: 'Tarefa não encontrada' });
    }
    
    const removedTask = tasks.splice(taskIndex, 1);
    
    console.log(`[DELETE] /tasks/${id} - Tarefa removida.`);
    console.log(` -> Título: "${removedTask[0].title}"`);
    
    res.status(204).send();
});

app.listen(3000, () => {
    console.log('====================================');
    console.log(' Server started on port 3000.');
    console.log(' Aguardando ações do front-end...');
    console.log('====================================');
});