Todo List 
===

<a href="https://github.com/D4nielStone/todo-list/releases/">![GitHub Tag](https://img.shields.io/github/v/tag/D4nielStone/todo-list?label=latest)</a>

A multi-platform ToDo List application featuring a C++ desktop client and a full-stack modern web application ecosystem.

<center><img width="600" height="400" alt="image" src="https://github.com/user-attachments/assets/16040f72-8bac-4f2f-884f-3d18664f0a12"/></center>

# 📁 Repository Structure

```text
todo-list/
├── desktop/          # C++ GUI Client (CMake, Bubble GUI, SQLite)
└── web/
    ├── backend/      # Node.js API Server (Express, PostgreSQL, CORS)
    └── frontend/     # Web User Interface
```

---

# 🖥️ 1. Desktop Client (`/desktop`)

The desktop version is a native C++ client built using the custom <a href="https://github.com" target="_blank">Bubble GUI</a> framework and SQLite.

### Common Setup Steps
```bash
git submodule update --init --recursive
cmake -B out && cmake --build
```

---

# ⚙️ 2. Web Backend (`/web/backend`)

The API server handles task management, storing relational data inside a PostgreSQL database.

### Installation
```bash
cd web/backend
npm install
```

### Environment Config (`web/backend/.env`)
Create a `.env` file in the backend folder:
```env
PORT=5000
DB_USER=your_postgres_username
DB_PASSWORD=your_postgres_password
DB_HOST=localhost
DB_PORT=5432
DB_NAME=todo_database
```

### Database Schema Setup
```sql
CREATE DATABASE todo_database;
\c todo_database;

CREATE TABLE todos (
    id SERIAL PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    completed BOOLEAN DEFAULT false,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### Running Server
```bash
npm run dev   # Development (Nemon)
npm start     # Production (Node)
```

---

# 🌐 3. Web Frontend (`/web/frontend`)

The responsive web user interface that communicates directly with the Node.js API server.

### Installation
```bash
cd web/frontend
npm install
```

### Execution
```bash
npm start
```
*Note: Ensure your backend server is active on `http://localhost:5000` so the frontend can successfully retrieve and update your tasks.*

---

# 🛠️ Troubleshooting

- **CORS Blocked Errors:** Confirm that `cors()` middleware is configured before routes in your backend `index.js`.
- **Desktop Submodules:** If the desktop build fails, verify your git submodules are completely initialized with `git submodule status --recursive`.
- **Database Refused:** Double-check your PostgreSQL system service status and `.env` match port `5432`.
