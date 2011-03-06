/**
 * @file
 * Main JS Mapping example for Google Maps.
 */
 
/**
 * Namespacing for jQuery
 */
(function($) {

  /**
   * Behavior to handle map building for Example Google Maps.
   */
  Drupal.behaviors.mapping_example_google_map_type = {
    'attach': function(context, settings) {
      // Check if we have mapping data and that it has
      // not been processed by main renderer, but not
      // processed by this.
      if (Drupal.settings.mapping.maps && $(context).data('mapping')) {
        $('.mapping-map-processed:not(.mapping-example-map-processed)').each(function() {
          $(this).addClass('mapping-example-map-processed');
          var mapId = $(this).attr('id');

          // This is a hack just for demoing purposes
          var chicago = new google.maps.LatLng(41.85, -87.65);
          var styled = [
            {
              featureType: "road.local",
              elementType: "geometry",
              stylers: [
                { hue: "#00ff00" },
                { saturation:100 }
              ]
            },
            {
              featureType: "landscape",
              elementType: "geometry",
              stylers: [
                { lightness: -100 }
              ]
            }
          ];
        
          var mapOptions = {
            zoom: 11,
            center: chicago,
            mapTypeControlOptions: {
               mapTypeIds: [google.maps.MapTypeId.ROADMAP, 'testing']
            }
          };
          map = new google.maps.Map(document.getElementById(mapId),
              mapOptions);
        
          var styledMapOptions = {
              name: "Testing"
          }
        
          var testMapType = new google.maps.StyledMapType(
              styled, styledMapOptions);
        
          map.mapTypes.set('testing', testMapType);
          map.setMapTypeId('testing');
        
        });
      }
    }
  };

})(jQuery);