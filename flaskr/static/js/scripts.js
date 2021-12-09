
function validateForm() {
    if((document.forms["suggestionsForm"]["fname"].value == "") ||
       (document.forms["suggestionsForm"]["comments"].value == ""))
    {
        alert("Please input at least your first name and suggestions");
        return false;
    }
}

function showForm() {
    var popupForm = document.getElementById("popupForm");
    var blur = document.getElementById("bg");
    
    popupForm.style.display = "block";
    // popupForm.classList.toggle("active");
    blur.classList.toggle("active");
    // }
    // else
    // {
    //     x.style.display = "none";
    // }
}

function closeForm() {
    // document.getElementById("popupForm").style.display = "none";
    var blur = document.getElementById("bg");
    popupForm.style.display = "none";
    // popupForm.classList.toggle("active");
    blur.classList.toggle("active");
}


// referenced from:
// https://www.prowaretech.com/Computer/JavaScript/AddLineNumbersToPre
function addLineClass (pre) {
    var lines = pre.innerHTML.split("\n");
    // console.log(pre.childNodes.length , lines.length);
    // console.log(lines.length); //511
    // console.log(pre.childNodes.length); //3
    while(pre.childNodes.length > 0) {
        pre.removeChild(pre.childNodes[0]);
    }
    //different indicies to accomidate for xmp and code
    for(var i = 2; i < lines.length-3; i++) {
        var span = document.createElement("span");
        span.className = "line";
        span.innerHTML = lines[i]; 
        // console.log(span.innerHTML);
        pre.appendChild(span);
        pre.appendChild(document.createTextNode("\n"));
    }
}
// window.addEventListener("load", function () {
function showLines() {
    var pres = document.getElementsByTagName("pre");
    for (var i = 0; i < pres.length; i++) {
        addLineClass(pres[i]);
    }
}
