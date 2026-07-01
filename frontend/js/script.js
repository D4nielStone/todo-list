async function createTask() {
    const inputElement = document.getElementById('ipt');
    let txt = inputElement.value;

    if (txt.trim() !== "") {
        try {
            const response = await fetch("http://localhost:3000/tasks", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    name: txt,
                    completed: false
                })
            });

            if (!response.ok) {
                throw new Error(`Server responded with status: ${response.status}`);
            }

            console.log(`[${new Date().toISOString()}] Frontend: Task created successfully.`);
            inputElement.value = "";
            loadTasks();
        } catch (error) {
            console.error(`[${new Date().toISOString()}] Frontend Error (createTask):`, error.message);
        }
    }
}

// Read and update tasks states
async function loadTasks() {
    try {
        const response = await fetch("http://localhost:3000/tasks");
        
        if (!response.ok) {
            throw new Error(`Server responded with status: ${response.status}`);
        }

        const tasks = await response.json();
        const container = document.getElementById("tasks");

        // Clean container
        container.innerHTML = "";

        console.log(`[${new Date().toISOString()}] Frontend: Loaded ${tasks.length} tasks.`);

        tasks.forEach(task => {
            const newDiv = document.createElement("div");
            newDiv.style.display = "flex";
            newDiv.style.alignItems = "center";
            newDiv.style.justifyContent = "space-between";
            newDiv.style.marginBottom = "8px";

            // Container to align to the left
            const leftGroup = document.createElement("div");
            leftGroup.style.display = "flex";
            leftGroup.style.alignItems = "center";

            // Using a button instead of input[type="checkbox"] for better custom text rendering ("✓")
            const chkContent = document.createElement("button");
            chkContent.style.width = "16px";
            chkContent.style.height = "16px";
            chkContent.style.border = "none";
            chkContent.style.borderRadius = "2px";
            chkContent.style.cursor = "pointer";
            chkContent.style.marginRight = "12px";
            chkContent.style.display = "inline-flex";
            chkContent.style.alignItems = "center";
            chkContent.style.justifyContent = "center";
            chkContent.style.outline = "none";
            chkContent.style.padding = "0";

            // State tracker tied to the button dataset
            chkContent.dataset.completed = task.completed;

            // Color based on state
            function updateCheckboxStyle(button, isCompleted) {
                if (isCompleted) {
                    button.style.backgroundColor = "#aaa"; // Light gray
                    button.textContent = "✓";
                    button.style.color = "#222"; // Check color
                    button.style.fontSize = "12px";
                    button.style.fontWeight = "bold";
                } else {
                    button.style.backgroundColor = "#444"; // Dark gray if unchecked
                    button.textContent = "";
                }
            }

            // Apply initial style
            updateCheckboxStyle(chkContent, task.completed);

            chkContent.addEventListener("click", async () => {
                // Toggle current state
                const newStatus = chkContent.dataset.completed !== "true";
                chkContent.dataset.completed = newStatus;
                
                // Update visual instantly for better UX
                updateCheckboxStyle(chkContent, newStatus);

                try {
                    const updateResponse = await fetch(`http://localhost:3000/tasks/${task.id}`, {
                        method: "PUT",
                        headers: {
                            "Content-Type": "application/json"
                        },
                        body: JSON.stringify({
                            completed: newStatus
                        })
                    });

                    if (!updateResponse.ok) {
                        throw new Error(`Server responded with status: ${updateResponse.status}`);
                    }
                    
                    console.log(`[${new Date().toISOString()}] Frontend: Task ${task.id} completion updated to ${newStatus}.`);
                } catch (error) {
                    console.error(`[${new Date().toISOString()}] Frontend Error (updateTask):`, error.message);
                    // Rollback visual if request fails
                    chkContent.dataset.completed = !newStatus;
                    updateCheckboxStyle(chkContent, !newStatus);
                }
            });

            const txtContent = document.createElement("span");
            txtContent.textContent = task.name;
            txtContent.style.color = "white";
            txtContent.style.fontFamily = "sans-serif";

            // Delete button
            const deleteBtn = document.createElement("button");
            deleteBtn.textContent = "Remove";
            deleteBtn.style.background = "none";
            deleteBtn.style.border = "none";
            deleteBtn.style.color = "#888";
            deleteBtn.style.cursor = "pointer";
            deleteBtn.style.fontStyle = "italic";

            deleteBtn.onclick = async () => {
                try {
                    const deleteResponse = await fetch(`http://localhost:3000/tasks/${task.id}`, {
                        method: "DELETE"
                    });

                    if (!deleteResponse.ok) {
                        throw new Error(`Server responded with status: ${deleteResponse.status}`);
                    }

                    console.log(`[${new Date().toISOString()}] Frontend: Task ${task.id} deleted.`);
                    loadTasks();
                } catch (error) {
                    console.error(`[${new Date().toISOString()}] Frontend Error (deleteTask):`, error.message);
                }
            };

            // Structure setting
            leftGroup.appendChild(chkContent);
            leftGroup.appendChild(txtContent);
            
            newDiv.appendChild(leftGroup);
            newDiv.appendChild(deleteBtn);

            container.appendChild(newDiv);
        });
    } catch (error) {
        console.error(`[${new Date().toISOString()}] Frontend Error (loadTasks):`, error.message);
    }
}

// Initial fetch when script loads
loadTasks();