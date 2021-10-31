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

// var lastScroll = 0;
// var footer = document.getElementById("footer");
// window.addEventListener("scroll", function(){
//     var scrollDown = window.scrollY || document.documentElement.scrollDown;
//     if(scrollDown > lastScroll)
//     {
//         footer.style.height = "-4rem";
//     }
//     else
//     {
//         footer.style.height = "0";
//     }
//     lastScroll = scrollDown;
// })


// window.onscroll = function(ev) {
//     if((window.innerHeight + window.scrollY) >= document.body.offsetHeight)
//     {
//         document.getElementsByClassName("footer").style.height = "-4rem";
        
//         console.log("innerHeight =" + window.innerHeight);
//         console.log("scrollY = " +window.scrollY);

//     }
// };

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

//Referenced from
//https://codepen.io/whins/pen/mPBREq
// var scrolled;
// var lastscrollTop = 0;
// var deltaValue = 5;
// var footerHeight = $('footer').outerHeight();

// $(window).scroll(function(event){
//     scrolled = true;
// });

// setInterval(funciton() {
//     if(scrolled)
//     {
//         hasScrolled();
//         scrolled = false;
//     }
// }, 250);

// function hasScrolled() {
//     var st = $(this).scrollTop();

//     //effect starts after deltaValue
//     if(Math.abs(lastscrollTop - st) <= deltaValue)
//         return;
    
//     if(st > lastscrollTop && st > footerHeight) {
//         $('footer').removeClass()
//     }
// }

// $(document).on('scrollstart scrolled', 'body', function(event) {
//     var $page = $(this);
//     if(event.type == 'scrollstart')
//         $page.addClass('is_scrolling');
//     else
//         $page.removeClass('is_scrolling');
// });
// console.log("Hello");
// $(window).scroll(function() {
//     if($(this).scrollTop() > 0) 
//     {
//         $('.footer').fadeOut();
//         console.log(scrollTop());
//     }
//     else
//     {
//         $('.footer').fadeIn();
//     }
// });

// var prevScrollpos = window.pageYOffset;
// window.onscroll = function() {
//   var currentScrollPos = window.pageYOffset;
//   if (prevScrollpos > currentScrollPos) {
//     document.getElementById("topNav").style.top = "0";
//   } else {
//     document.getElementById("topNav").style.top = "-50px";
//   }
//   prevScrollpos = currentScrollPos;
// }


// https://www.prowaretech.com/Computer/JavaScript/AddLineNumbersToPre
function addLineClass (pre) {
    var lines = pre.innerText.split("\n"); // can use innerHTML also
    // console.log(pre.childNodes.length);
    while(pre.childNodes.length > 0) {
        pre.removeChild(pre.childNodes[0]);
    }
    for(var i = 0; i < lines.length; i++) {
        var span = document.createElement("span");
        span.className = "line";
        span.innerText = lines[i]; // can use innerHTML also
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
// }, false);