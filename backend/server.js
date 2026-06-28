const express = require('express');
const cors = require('cors'); 
const app = express();
require('dotenv').config();
const { MongoClient, ServerApiVersion } = require('mongodb');

const uri = process.env.URI;
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

// Configuração do Cliente MongoDB
const client = new MongoClient(uri, {
  serverApi: {
    version: ServerApiVersion.v1,
    strict: true,
    deprecationErrors: true,
  }
});

// Variável global para armazenar a referência da coleção do banco de dados
let tasksCollection;

// Função para conectar ao banco de dados antes de iniciar as rotas
async function connectDB() {
    try {
        await client.connect();
        // Cria ou acessa o banco "todo_list" e a coleção "tasks"
        const database = client.db("todo_list");
        tasksCollection = database.collection("tasks");
        console.log("-> Conectado ao MongoDB com sucesso! 🎉");
    } catch (error) {
        console.error("Erro ao conectar no MongoDB:", error);
        process.exit(1); // Fecha o servidor se não conseguir conectar ao banco
    }
}

// Inicializa a conexão
connectDB();

// ====================================================================
// ROTAS DA API (Agora todas usam async/await porque o banco é externo)
// ====================================================================

// GET - listar tarefas
app.get('/tasks', async (req, res) => {
    try {
        // Busca todas as tarefas do banco e transforma em um Array
        const tasks = await tasksCollection.find({}).toArray();
        console.log(`[GET] /tasks - tarefas encontradas: ${tasks.length}`);
        res.json(tasks);
    } catch (error) {
        res.status(500).json({ error: "Erro ao buscar tarefas" });
    }
});

// POST - criar uma nova tarefa
app.post('/tasks', async (req, res) => {
    try {
        const { title } = req.body;
        
        const newTask = {
            id: Date.now().toString(), // mantendo seu id baseado em timestamp
            title: title,
            completed: false
        };

        // Insere o objeto direto na coleção do MongoDB
        await tasksCollection.insertOne(newTask);
        
        console.log(`[POST] /tasks - tarefa criada no banco.`);
        res.status(201).json(newTask);
    } catch (error) {
        res.status(500).json({ error: "Erro ao criar tarefa" });
    }
});

// PUT - atualizar o status da tarefa
app.put('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const { completed } = req.body;
        
        // Atualiza o documento onde o "id" seja igual ao passado na URL
        const result = await tasksCollection.findOneAndUpdate(
            { id: id },
            { $set: { completed: completed } },
            { returnDocument: 'after' } // Retorna a tarefa já atualizada
        );
        
        // No driver nativo do Mongo, se não achar, o value vem vazio
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

// DELETE - remover tarefa
app.delete('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
        
        // Deleta o documento que possui o id correspondente
        const result = await tasksCollection.deleteOne({ id: id });
        
        // deletedCount indica quantos registros foram apagados
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