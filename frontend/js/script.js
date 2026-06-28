async function createTask() {

    let txt = document.getElementById('ipt').value;

    if (txt.trim() !== "") {

        await fetch(
            "http://localhost:3000/tasks",
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

async function loadTasks() {

    const response = await fetch("http://localhost:3000/tasks");
    const tasks = await response.json();
    const container = document.getElementById("tasks");

    container.innerHTML = "";

    tasks.forEach(task => {

        const newDiv = document.createElement("div");
        newDiv.style.display = "flex";
        newDiv.style.alignItems = "center";
        newDiv.style.justifyContent = "space-between";
        newDiv.style.marginBottom = "8px";

        // Container para alinhar checkbox e texto à esquerda
        const leftGroup = document.createElement("div");
        leftGroup.style.display = "flex";
        leftGroup.style.alignItems = "center";

        const chkContent = document.createElement("input");
        chkContent.type = "checkbox";
        chkContent.checked = task.completed;

        // --- ESTILIZAÇÃO DO CHECKBOX (IGUAL AO DESKTOP) ---
        chkContent.style.appearance = "none";
        chkContent.style.webkitAppearance = "none";
        chkContent.style.width = "14px";
        chkContent.style.height = "14px";
        chkContent.style.border = "none";
        chkContent.style.borderRadius = "0px"; // Quadrado perfeito
        chkContent.style.cursor = "pointer";
        chkContent.style.marginRight = "12px";
        chkContent.style.display = "inline-flex";
        chkContent.style.alignItems = "center";
        chkContent.style.justifyContent = "center";
        chkContent.style.outline = "none";

        // Função interna para alternar as cores baseadas no estado
        function updateCheckboxStyle(checkbox) {
            if (checkbox.checked) {
                checkbox.style.backgroundColor = "#aaa"; // Cinza claro se marcado
                checkbox.innerText = "✓";
                checkbox.style.color = "#222"; // Cor do "visto"
                checkbox.style.fontSize = "10px";
                checkbox.style.fontWeight = "bold";
            } else {
                checkbox.style.backgroundColor = "#444"; // Cinza escuro se desmarcado
                checkbox.innerText = "";
            }
        }

        // Aplica o estilo inicial
        updateCheckboxStyle(chkContent);

        chkContent.addEventListener("change", async () => {
            // Atualiza o visual imediatamente na tela
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

        const deleteBtn = document.createElement("button");
        deleteBtn.textContent = "Remove";
        deleteBtn.style.background = "none";
        deleteBtn.style.border = "none";
        deleteBtn.style.color = "#888";
        deleteBtn.style.cursor = "pointer";
        deleteBtn.style.fontStyle = "italic";

        deleteBtn.onclick = async () => {
            await fetch(
                `http://localhost:3000/tasks/${task.id}`,
                {
                    method: "DELETE"
                }
            );
            loadTasks();
        };

        // Montagem da estrutura
        leftGroup.appendChild(chkContent);
        leftGroup.appendChild(txtContent);
        
        newDiv.appendChild(leftGroup);
        newDiv.appendChild(deleteBtn);

        container.appendChild(newDiv);
    });
}

loadTasks();
