/**
 * @file
 * Main JS file for the mapping architecture
 * module.  This kicks off the JS processing.
 */
 
/**
 * Namespacing for jQuery
 */
(function($) {

  /**
   * Initial behavior to start the map processing.
   * The data for the mapping is stored in the jQuery.data()
   * storage mechanism.
   */
  Drupal.behaviors.mapping = {
    'attach': function(context, settings) {
      // Check if we have mapping data and that it has
      // not been processed yet.  Processed is marked
      // both by a class and .data().
      if (typeof(Drupal.settings.mapping) === 'object' && Drupal.settings.mapping.maps && !$(context).data('mapping')) {
        $('.mapping-map:not(.mapping-map-processed)').each(function() {
          $(this).addClass('mapping-map-processed');
          var mapId = $(this).attr('id');
console.log(Drupal.settings.mapping);
          // Use try..catch for a little smoother error handling.
          try {
            // Attach data to map DOM object
            $(this).data('mapping', { 
              'mapping_data': Drupal.settings.mapping.maps[mapId]
            });
  
            // Attach behaviors to let map processors
            // handle things.
            Drupal.attachBehaviors(this);
          }
          catch (e) {
            if (typeof console != 'undefined') {
              console.log(e);
            }
            else {
              // Is this actually a good message for the screen?
              $(this).text('Error during map rendering: ' + e);
            }
          }
        });
      }
    }
  };

})(jQuery);