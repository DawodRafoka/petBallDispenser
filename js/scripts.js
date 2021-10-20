// $("textarea").each(function () {
//     this.setAttribute("style", "height:" + (this.scrollHeight) + "px;overflow-y:hidden;");
//   }).on("input", function () {
//     this.style.height = "auto";
//     this.style.height = (this.scrollHeight) + "px";
//   });

// window.addEventListener('scroll', function() {
//     const parallax = document.querySelector('.CoverPicture');
//     let scrollPosition = window.scrollY;    //keep track of the scrolling position

//     parallax.scroll = 'translateY(' + scrollPosition * .5 + 'px)';

//     // console.log(scrollPosition);
// });

// function scrollToTop(){
//     window.scroll({top:0 , behavior: 'smooth'}, 0);
//     alert("Scrolled Up");
// }

// $('.formContainer').delegate( 'textarea', 'keydown', function (){
//     $(this).height( 0 );
//     $(this).height( this.scrollHeight );
// });
// $('.formContainer').find( 'textarea' ).keydown();

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