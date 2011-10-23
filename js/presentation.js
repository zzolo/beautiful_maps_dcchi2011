/**
 *
 */
 
// General variables
var presentationFor = 'BAD Camp 2011';
var showAlan = true;
var showThomas = false;
var continueDiscussion = false;
 
$(document).ready(function(){
  // What is this presentation for.
  $('#presentation-for').html(presentationFor);
  
  // Show hide people depending on who is presenting
  if (!showAlan) {
    $('.slide-alan').remove();
  }
  if (!showThomas) {
    $('.slide-thomas').remove();
  }
  if (!continueDiscussion) {
    $('.continue-discussion').remove();
  }
  
  // Presentation
  $('#slides').presentation({
    slide: '.slide', //Reference to each individual slide
    pagerClass: 'nav-pager', //Class to put on the unordered list that contains links to each slide
    prevNextClass: 'nav-prev-next', //Class to put on the unordered list that contains the previous and next links
    prevText: 'Previous', //Text for the Previous link
    nextText: 'Next', //Text for the Next link
    transition: 'slide'
  });
});