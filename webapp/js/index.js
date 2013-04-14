$(document).ready(function() {

var menu = menu = $('#menu');

$('.tab').click(function(event) {

    event.preventDefault();
    /*
     var full_url = this.href,
     parts = full_url.split('#'),
     trgt = parts[1],
     target_offset = $('#'+trgt).offset(),
     target_top = target_offset.top;

     $('html, body').animate({scrollTop:target_top}, 500);   */

    /* Remove active class on any li when an anchor is clicked */

    menu.children().removeClass("active");

    /* Add active class to clicked anchor's parent li */
    $(this).addClass('active');

});
});