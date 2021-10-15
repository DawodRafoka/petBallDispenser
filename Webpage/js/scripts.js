window.addEventListener('scroll', function() {
    const parallax = document.querySelector('.CoverPicture');
    let scrollPosition = window.scrollY;    //keep track of the scrolling position

    parallax.scroll = 'translateY(' + scrollPosition * .5 + 'px)';

    // console.log(scrollPosition);
});