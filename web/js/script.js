function createTask() {
    let txt = document.getElementById('ipt').value;
    
    // Verifica se o campo não está vazio ou apenas com espaços
    if(txt.trim() !== "") {
        const newDiv = document.createElement("div");
        newDiv.className = "linear horizontal";
        newDiv.style.display = "flex";
        newDiv.style.textAlign = "flex-start";
        newDiv.style.justifyContent = "flex-start";

        const chkContent = document.createElement("input");
        chkContent.type = "checkbox";
        chkContent.class = "linear";
        chkContent.style.marginRight = "8px"; // Adiciona um espaço entre o checkbox e o texto

        const txtContent = document.createElement("span");
        chkContent.textContent = txt;
        txtContent.style.color = "white";

        newDiv.appendChild(chkContent)
        newDiv.appendChild(txtContent);

        const currentDiv = document.getElementById("div1");
        document.body.insertBefore(newDiv, currentDiv);
        
        // Clear input
        document.getElementById('ipt').value = "";
    }
}
