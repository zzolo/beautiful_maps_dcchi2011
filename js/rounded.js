/**
 *
 */
$(document).ready(function() {
  var round = '3px';
  $('.demo-links a').corner(round);
  $('.nav-prev-next a').corner('bottom ' + round);
  $('.nav-pager a').corner('top ' + round);
});