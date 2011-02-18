// $Id: openlayers_presets_ui.ui.js,v 1.2.2.4 2010/07/14 19:52:12 phayes Exp $

/**
 * @file
 * This file holds the javascript functions for the preset UI
 *
 * @ingroup openlayers
 */

/**
 * Drupal beahvior for OL UI form
 */
Drupal.behaviors.OLUI = function(context) {
  var $projectSelect = $("input[name='projections[easy_projection]']:not(.projection-processed)");
  var $projectOther = $('#edit-projections-projection');
  var $projectOtherWrapper = $('#edit-projections-projection-wrapper');
  var $autoOptionsCheck = $('#edit-options-automatic-options');
  var $submitProjection = $('#edit-openlayers-projection-ahah');

  // When the user selects a projection, update the projectOther field.
  // projectOther is where we actually store projection values
  $projectSelect.click(function(event) {
    if (event.target.value != 'other') {
      $projectOtherWrapper.hide();
      $projectOther.val(event.target.value);
    }
    else {
      $projectOtherWrapper.show();
    }
    $projectOther.trigger('change');
  });
  //TODO: process above function on page load for the currently selected item
  $projectSelect.addClass('projection-processed');
  
  // Reproject center values when projection changes
  $projectOther.change(OL.updateCenterFormOnProjectionChange);

  // Hide submit button
  $submitProjection.hide();

  // Automatic options.  We do it here, instead of in Form API because
  // Form API enforces the disabled
  $autoOptionsCheck.change(function() {
    var $thisCheck = $(this);
    var $autoOptions = $thisCheck.parent().parent().parent().find('fieldset, div:not("#edit-options-automatic-options-wrapper")');
    if ($thisCheck.is(':checked')) {
      $autoOptions.hide();;
    }
    else {
      $autoOptions.show('disabled');
    }
  });

  // Trigger change
  $autoOptionsCheck.trigger('change');

  // Change event for helper map inputs
  $('#edit-center-lat').change(OL.updateHelpmapCenter);
  $('#edit-center-lon').change(OL.updateHelpmapCenter);
  $('#edit-center-zoom').change(OL.updateHelpmapCenter);

  // @@TODO: Reproject lat/lon values of center map. On the first load
  // this is redundent, but it is important for ahah updates so that when
  // the projection changes the lat/lon changes to fit the new units.

  // Initial trigger of updateHelpmapCenter
  OL.updateHelpmapCenter();
}

/**
 * Update the values of the centering form whent the user changes projections
 *
 */
OL.updateCenterFormOnProjectionChange = function(event) {
  
  var $formItem = $(event.target);
  var projection = event.target.value;
  var helpmap = OL.maps['openlayers-center-helpmap'].map;
  var zoom = helpmap.getZoom();
  var center = helpmap.getCenter();

  // Transform center
  center.transform(new OpenLayers.Projection('EPSG:4326'),new OpenLayers.Projection('EPSG:' + projection));

  // Get new lat and lon
  var lat = center.lat;
  var lon = center.lon;

  // Set new values in the field
  $('#edit-center-lat').val(lat);
  $('#edit-center-lon').val(lon);
}

/**
 * Update the center of the helpmap using the values from the form
 *
 * Take the center lat, lon and zoom values from the form and update
 * the helper map.
 */
OL.updateHelpmapCenter = function() {
  var projection = $('#edit-projections-projection').val();
  var zoom = $('#edit-center-zoom').val();
  var lat = $('#edit-center-lat').val();
  var lon = $('#edit-center-lon').val();

  // Check for lat and lon
  if (lat != '' && lon != '') {
    // Create new center
    var center = new OpenLayers.LonLat(lon, lat);
    // Transform for projection
    center.transform(new OpenLayers.Projection('EPSG:' + projection), new OpenLayers.Projection('EPSG:4326'));
    // Set center of map.
    OL.maps['openlayers-center-helpmap'].map.setCenter(center, zoom);
  }
}

/**
 * Update the values from the form using center of the helpmap.
 *
 * When a user pans and zooms our helper map, update the form values.
 */
OL.EventHandlers.updateCenterFormValues = function() {
  var helpmap = OL.maps['openlayers-center-helpmap'].map;
  var projection = $('#edit-projections-projection').val();
  var zoom = helpmap.getZoom();
  var center = helpmap.getCenter();

  // Transform center
  center.transform(new OpenLayers.Projection('EPSG:4326'),new OpenLayers.Projection('EPSG:' + projection));

  // Get new lat and lon
  var lat = center.lat;
  var lon = center.lon;

  // Set new values
  $('#edit-center-zoom').val(zoom);
  $('#edit-center-lat').val(lat);
  $('#edit-center-lon').val(lon);
}
