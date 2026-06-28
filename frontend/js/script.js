// create task in the data base
async function createTask() {
    //get input text
    let txt = document.getElementById('ipt').value;

    // verify if txt input is empty
    if (txt.trim() !== "") {
        await fetch(
            "https://todo-list-bubble.onrender.com/tasks",
            {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    title: txt
                })
            }
        );

        document.getElementById('ipt').value = "";
        loadTasks();
    }
}

// read and update tasks states
async function loadTasks() {
    const response = await fetch("https://todo-list-bubble.onrender.com/tasks");
    const tasks = await response.json();
    const container = document.getElementById("tasks");

    // clean container
    container.innerHTML = "";

    tasks.forEach(task => {
        const newDiv = document.createElement("div");
        newDiv.style.display = "flex";
        newDiv.style.alignItems = "center";
        newDiv.style.justifyContent = "space-between";
        newDiv.style.marginBottom = "8px";

        // container to align to the left
        const leftGroup = document.createElement("div");
        leftGroup.style.display = "flex";
        leftGroup.style.alignItems = "center";

        const chkContent = document.createElement("input");
        chkContent.type = "checkbox";
        chkContent.checked = task.completed;

        // checkbox stabalization
        chkContent.style.appearance = "none"
        chkContent.style.width = "14px";
        chkContent.style.height = "14px";
        chkContent.style.border = "none";
        chkContent.style.borderRadius = "1px";
        chkContent.style.cursor = "pointer";
        chkContent.style.marginRight = "12px";
        chkContent.style.display = "inline-flex";
        chkContent.style.alignItems = "center";
        chkContent.style.justifyContent = "center";
        chkContent.style.outline = "none";

        // color based on state
        function updateCheckboxStyle(checkbox) {
            if (checkbox.checked) {
                checkbox.style.backgroundColor = "#aaa"; // light gray
                checkbox.innerText = "✓";
                checkbox.style.color = "#222"; // check color
                checkbox.style.fontSize = "10px";
                checkbox.style.fontWeight = "bold";
            } else {
                checkbox.style.backgroundColor = "#444"; // Cinza escuro se desmarcado
                checkbox.innerText = "";
            }
        }

        // apply initial style
        updateCheckboxStyle(chkContent);

        chkContent.addEventListener("change", async () => {
            // update visual
            updateCheckboxStyle(chkContent);

            await fetch(
                `http://localhost:3000/tasks/${task.id}`,
                {
                    method: "PUT",
                    headers: {
                        "Content-Type": "application/json"
                    },
                    body: JSON.stringify({
                        completed: chkContent.checked
                    })
                }
            );
        });

        const txtContent = document.createElement("span");
        txtContent.textContent = task.title;
        txtContent.style.color = "white";
        txtContent.style.fontFamily = "sans-serif";

        // delete button
        const deleteBtn = document.createElement("button");
        deleteBtn.textContent = "Remove";
        deleteBtn.style.background = "none";
        deleteBtn.style.border = "none";
        deleteBtn.style.color = "#888";
        deleteBtn.style.cursor = "pointer";
        deleteBtn.style.fontStyle = "italic";

        deleteBtn.onclick = async () => {
            await fetch(
                `https://todo-list-bubble.onrender.com/tasks/${task.id}`,
                {
                    method: "DELETE"
                }
            );
            loadTasks();
        };

        // struct set
        leftGroup.appendChild(chkContent);
        leftGroup.appendChild(txtContent);
        
        newDiv.appendChild(leftGroup);
        newDiv.appendChild(deleteBtn);

        container.appendChild(newDiv);
    });
}

loadTasks();