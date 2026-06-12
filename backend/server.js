const express = require('express');

const app = express();

app.use(express.json());

app.get('/', (req, res) => {
    res.json({
        message: 'API online'
    })
})

app.listen(3000, () => {
    console.log('Server started on port 3000.')
})

const users =  [];

app.post('/register', (req, res) => {
    const { email, password } = req.body
    users.push({
        email,
        password
    })
    
    res.json({
        success: true
    })
})