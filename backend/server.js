const express = require('express');
const cors = require('cors'); 
const app = express();
require('dotenv').config();
const { MongoClient, ServerApiVersion } = require('mongodb');

const uri = process.env.URI;
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

const cors = require('cors'); 
const app = express();
require('dotenv').config();
const { MongoClient, ServerApiVersion } = require('mongodb');

const uri = process.env.URI;
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

const client = new MongoClient(uri, {
  serverApi: {
    version: ServerApiVersion.v1,
    strict: true,
    deprecationErrors: true,
  }
});

let tasksCollection;

async function connectDB() {
    try {
        await client.connect();
        const database = client.db("todo_list");
        tasksCollection = database.collection("tasks");
        console.log("-> conected to data base.");
    } catch (error) {
        console.error("-> error to conect to mongo db", error);
        process.exit(1);
    }
}

// init connect
connectDB();

// list tasks
app.get('/tasks', async (req, res) => {
    try {

        const tasks = await tasksCollection.find({}).toArray();
        console.log(`[GET] /tasks - tarefas encontradas: ${tasks.length}`);
        res.json(tasks);
    } catch (error) {
        res.status(500).json({ error: "Erro ao buscar tarefas" });
    }
});


app.post('/tasks', async (req, res) => {
    try {
        const { title } = req.body;
        
        const newTask = {
            id: Date.now().toString(),
            title: title,
            completed: false
        };


        await tasksCollection.insertOne(newTask);
        
        console.log(`[POST] /tasks - tarefa criada no banco.`);
        res.status(201).json(newTask);
    } catch (error) {
        res.status(500).json({ error: "Erro ao criar tarefa" });
    }
});


app.put('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const { completed } = req.body;
        

        const result = await tasksCollection.findOneAndUpdate(
            { id: id },
            { $set: { completed: completed } },
            { returnDocument: 'after' }
        );
        

        if (!result) {
            console.log(`[PUT] /tasks/${id} - Erro: tarefa não encontrada`);
            return res.status(404).json({ error: 'Tarefa não encontrada' });
        }
        
        console.log(`[PUT] /tasks/${id} - Status alterado no banco`);
        res.json(result);
    } catch (error) {
        res.status(500).json({ error: "Erro ao atualizar tarefa" });
    }
});


app.delete('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
        

        const result = await tasksCollection.deleteOne({ id: id });
        

        if (result.deletedCount === 0) {
            console.log(`[DELETE] /tasks/${id} - Erro: tarefa não encontrada`);
            return res.status(404).json({ error: 'Tarefa não encontrada' });
        }
        
        console.log(`[DELETE] /tasks/${id} - Tarefa removida do banco.`);
        res.status(204).send();
    } catch (error) {
        res.status(500).json({ error: "Erro ao deletar tarefa" });
    }
});

app.listen(PORT, () => {
    console.log('====================================');
    console.log(` Servidor rodando na porta ${PORT}`);
    console.log('====================================');
});

const client = new MongoClient(uri, {
  serverApi: {
    version: ServerApiVersion.v1,
    strict: true,
    deprecationErrors: true,
  }
});


let tasksCollection;


async function connectDB() {
    try {
        await client.connect();
        
        const database = client.db("todo_list");
        tasksCollection = database.collection("tasks");
        console.log("-> Conectado ao MongoDB com sucesso! 🎉");
    } catch (error) {
        console.error("Erro ao conectar no MongoDB:", error);
        process.exit(1);
    }
}
connectDB();

app.get('/tasks', async (req, res) => {
    try {

        const tasks = await tasksCollection.find({}).toArray();
        console.log(`[GET] /tasks - tarefas encontradas: ${tasks.length}`);
        res.json(tasks);
    } catch (error) {
        res.status(500).json({ error: "Erro ao buscar tarefas" });
    }
});

app.post('/tasks', async (req, res) => {
    try {
        const { title } = req.body;
        
        const newTask = {
            id: Date.now().toString(),
            title: title,
            completed: false
        };

        await tasksCollection.insertOne(newTask);
        
        console.log(`[POST] /tasks - tarefa criada no banco.`);
        res.status(201).json(newTask);
    } catch (error) {
        res.status(500).json({ error: "Erro ao criar tarefa" });
    }
});

app.put('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const { completed } = req.body;
        
        const result = await tasksCollection.findOneAndUpdate(
            { id: id },
            { $set: { completed: completed } },
            { returnDocument: 'after' }
        );
        
        if (!result) {
            console.log(`[PUT] /tasks/${id} - Erro: tarefa não encontrada`);
            return res.status(404).json({ error: 'Tarefa não encontrada' });
        }
        
        console.log(`[PUT] /tasks/${id} - Status alterado no banco`);
        res.json(result);
    } catch (error) {
        res.status(500).json({ error: "Erro ao atualizar tarefa" });
    }
});

app.delete('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
    
        const result = await tasksCollection.deleteOne({ id: id });
    
        if (result.deletedCount === 0) {
            console.log(`[DELETE] /tasks/${id} - Erro: tarefa não encontrada`);
            return res.status(404).json({ error: 'Tarefa não encontrada' });
        }
        
        console.log(`[DELETE] /tasks/${id} - Tarefa removida do banco.`);
        res.status(204).send();
    } catch (error) {
        res.status(500).json({ error: "Erro ao deletar tarefa" });
    }
});

app.listen(PORT, () => {
    console.log('====================================');
    console.log(` Servidor rodando na porta ${PORT}`);
    console.log('====================================');
});