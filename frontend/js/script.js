async function createTask() {

    let txt =
        document.getElementById('ipt').value;

    if (txt.trim() !== "") {

        await fetch(
            "http://localhost:3000/tasks",
            {
                method: "POST",

                headers: {
                    "Content-Type":
                        "application/json"
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

    const response =
        await fetch(
            "http://localhost:3000/tasks"
        );

    const tasks =
        await response.json();

    const container =
        document.getElementById("tasks");

    container.innerHTML = "";

    tasks.forEach(task => {

        const newDiv =
            document.createElement("div");

        newDiv.style.display = "flex";

        const chkContent =
            document.createElement("input");

        chkContent.type = "checkbox";

        chkContent.checked =
            task.completed;

        chkContent.style.marginRight = "8px";

        chkContent.addEventListener(
            "change",
            async () => {

                await fetch(
                    `http://localhost:3000/tasks/${task.id}`,
                    {
                        method: "PUT",

                        headers: {
                            "Content-Type":
                                "application/json"
                        },

                        body: JSON.stringify({
                            completed:
                                chkContent.checked
                        })
                    }
                );
            }
        );

        const txtContent =
            document.createElement("span");

        txtContent.textContent =
            task.title;

        txtContent.style.color = "white";

        const deleteBtn =
            document.createElement("button");

        deleteBtn.textContent = "X";

        deleteBtn.onclick = async () => {

            await fetch(
                `http://localhost:3000/tasks/${task.id}`,
                {
                    method: "DELETE"
                }
            );

            loadTasks();
        };

        newDiv.appendChild(chkContent);
        newDiv.appendChild(txtContent);
        newDiv.appendChild(deleteBtn);

        container.appendChild(newDiv);
    });
}

loadTasks();