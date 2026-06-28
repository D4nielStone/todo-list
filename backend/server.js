const express = require('express');
const cors = require('cors'); 
const app = express();
require('dotenv').config();
const { MongoClient, ServerApiVersion } = require('mongodb');

const uri = process.env.URI;
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

// MongoDB Client Configuration
const client = new MongoClient(uri, {
  serverApi: {
    version: ServerApiVersion.v1,
    strict: true,
    deprecationErrors: true,
  }
});

// Global variable to store the database collection reference
let tasksCollection;

// Function to connect to the database before starting the routes
async function connectDB() {
    try {
        await client.connect();
        // Connect to "todo-list" database and "tasks" collection
        const database = client.db("todo-list");
        tasksCollection = database.collection("tasks");
        console.log(`[${new Date().toISOString()}] -> Successfully connected to MongoDB! 🎉`);
    } catch (error) {
        console.error(`[${new Date().toISOString()}] CRITICAL: Error connecting to MongoDB:`, error);
        process.exit(1); // Shutdown server if database connection fails
    }
}

// Initialize database connection
connectDB();

// ====================================================================
// API ROUTES (Async/Await handled for external database operations)
// ====================================================================

// GET - List all tasks
app.get('/tasks', async (req, res) => {
    try {
        // Fetch all tasks from the collection and convert to an Array
        const tasks = await tasksCollection.find({}).toArray();
        console.log(`[${new Date().toISOString()}] GET /tasks - Success: Found ${tasks.length} tasks.`);
        res.json(tasks);
    } catch (error) {
        console.error(`[${new Date().toISOString()}] GET /tasks - Error:`, error.message);
        res.status(500).json({ error: "Internal Server Error while fetching tasks" });
    }
});

// POST - Create a new task
app.post('/tasks', async (req, res) => {
    try {
        const { title } = req.body;

        // Basic validation
        if (!title) {
            console.log(`[${new Date().toISOString()}] POST /tasks - BadRequest: Title missing.`);
            return res.status(400).json({ error: "Title is required" });
        }
        
        const newTask = {
            id: Date.now().toString(), // Keeping the timestamp-based ID
            title: title,
            completed: false
        };

        // Insert the task document into MongoDB
        await tasksCollection.insertOne(newTask);
        
        console.log(`[${new Date().toISOString()}] POST /tasks - Success: Task created with ID ${newTask.id}`);
        res.status(201).json(newTask);
    } catch (error) {
        console.error(`[${new Date().toISOString()}] POST /tasks - Error:`, error.message);
        res.status(500).json({ error: "Internal Server Error while creating task" });
    }
});

// PUT - Update task completion status
app.put('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const { completed } = req.body;
        
        // Basic validation to ensure 'completed' field is a boolean
        if (typeof completed !== 'boolean') {
            console.log(`[${new Date().toISOString()}] PUT /tasks/${id} - BadRequest: Invalid 'completed' value.`);
            return res.status(400).json({ error: "'completed' status must be a boolean" });
        }

        // Update the document matching the custom string "id"
        const updatedTask = await tasksCollection.findOneAndUpdate(
            { id: id },
            { $set: { completed: completed } },
            { returnDocument: 'after' } // Returns the modified document instead of the original
        );
        
        // Check if the task was found and updated
        if (!updatedTask) {
            console.log(`[${new Date().toISOString()}] PUT /tasks/${id} - NotFound: Task does not exist.`);
            return res.status(404).json({ error: 'Task not found' });
        }
        
        console.log(`[${new Date().toISOString()}] PUT /tasks/${id} - Success: Task status updated.`);
        res.json(updatedTask);
    } catch (error) {
        console.error(`[${new Date().toISOString()}] PUT /tasks/:id - Error:`, error.message);
        res.status(500).json({ error: "Internal Server Error while updating task" });
    }
});

// DELETE - Remove a task
app.delete('/tasks/:id', async (req, res) => {
    try {
        const { id } = req.params;
        
        // Delete the document matching the custom string "id"
        const result = await tasksCollection.deleteOne({ id: id });
        
        // deletedCount confirms if any record was removed
        if (result.deletedCount === 0) {
            console.log(`[${new Date().toISOString()}] DELETE /tasks/${id} - NotFound: Task does not exist.`);
            return res.status(404).json({ error: 'Task not found' });
        }
        
        console.log(`[${new Date().toISOString()}] DELETE /tasks/${id} - Success: Task removed.`);
        res.status(204).send();
    } catch (error) {
        console.error(`[${new Date().toISOString()}] DELETE /tasks/:id - Error:`, error.message);
        res.status(500).json({ error: "Internal Server Error while deleting task" });
    }
});

app.listen(PORT, () => {
    console.log('====================================');
    console.log(` Server running on port ${PORT}`);
    console.log('====================================');
});