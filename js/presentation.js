/**
 *
 */
$(document).ready(function(){
  $('#slides').presentation({
    slide: '.slide', //Reference to each individual slide
    pagerClass: 'nav-pager', //Class to put on the unordered list that contains links to each slide
    prevNextClass: 'nav-prev-next', //Class to put on the unordered list that contains the previous and next links
    prevText: 'Previous', //Text for the Previous link
    nextText: 'Next', //Text for the Next link
    transition: 'slide'
  });
});