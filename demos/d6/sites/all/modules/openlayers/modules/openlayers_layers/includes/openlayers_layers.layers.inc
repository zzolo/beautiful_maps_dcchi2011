<?php
// $Id: openlayers_layers.layers.inc,v 1.18.2.2 2010/02/10 01:58:00 phayes Exp $

/**
 * @file
 * This file holds the main Drupal hook functions
 * and private functions for the openlayers module.
 *
 * @ingroup openlayers
 */

/**
 * Process Layers
 *
 * Call back to process layers provided by the
 * openlayers_layers_openlayers_layers_info() function
 *
 * @param $layer
 *   String ID for the layer
 * @param $map
 *   Map array
 * @return
 *   Array formated for an OpenLayers map layer
 */
function openlayers_layers_process_layers($layer = NULL) {
  $layer_data = array();

  // Make sure layer is a string
  if (!is_string($layer)) {
    return FALSE;
  }

  // Get info array to reference title
  $layer_info = openlayers_layers_openlayers_layers_info();

  // Determine what data to send back
  switch ($layer) {
    case 'openlayers_layers_nasa_global_mosaic':
      $layer_data = array(
        'id' => $layer,
        'type' => 'WMS',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://t1.hypercube.telascience.org/cgi-bin/landsat7',
        'params' => array(
          'layers' => 'landsat7',
        ),
        'options' => array(),
        'events' => array(),
      );
      break;

    case 'openlayers_layers_nasa_daily_planet':
      $layer_data = array(
        'id' => $layer,
        'type' => 'WMS',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://wms.jpl.nasa.gov/wms.cgi?',
        'params' => array(
          'layers' => 'daily_planet',
          'format' => 'image/png'
        ),
        'options' => array(),
        'events' => array(),
      );
      break;

    case 'openlayers_layers_nasa_blue_marble':
      $layer_data = array(
        'id' => $layer,
        'type' => 'WMS',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://wms.jpl.nasa.gov/wms.cgi?',
        'params' => array(
          'layers' => 'BMNG',
          'format' => 'image/png'
        ),
        'options' => array(),
        'events' => array(),
      );
      break;

    case 'openlayers_layers_open_aerial':
      $layer_data = array(
        'id' => $layer,
        'type' => 'XYZ',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://tile.openaerialmap.org/tiles/1.0.0/openaerialmap-900913/${z}/${x}/${y}.png',
        'options' => array(
          'sphericalMercator' => TRUE,
        ),
        'events' => array(),
      );
      break;

    case 'openlayers_layers_google_street':
      _openlayers_layers_include_map_api('google');
      $layer_data = array(
        'id' => $layer,
        'type' => 'Google',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'street'
        ),
      );
      break;

    case 'openlayers_layers_google_satellite':
      _openlayers_layers_include_map_api('google');
      $layer_data = array(
        'id' => $layer,
        'type' => 'Google',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'satellite'
        ),
      );
      break;

    case 'openlayers_layers_google_hybrid':
      _openlayers_layers_include_map_api('google');
      $layer_data = array(
        'id' => $layer,
        'type' => 'Google',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'hybrid'
        ),
      );
      break;

    case 'openlayers_layers_google_physical':
      _openlayers_layers_include_map_api('google');
      $layer_data = array(
        'id' => $layer,
        'type' => 'Google',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'physical'
        ),
      );
      break;

    case 'openlayers_layers_yahoo_street':
      _openlayers_layers_include_map_api('yahoo');
      $layer_data = array(
        'id' => $layer,
        'type' => 'Yahoo',
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'name' => $layer_info[$layer]['name'],
        'params' => array(
          'type' => 'street'
        ),
      );
      break;

    case 'openlayers_layers_yahoo_satellite':
      _openlayers_layers_include_map_api('yahoo');
      $layer_data = array(
        'id' => $layer,
        'type' => 'Yahoo',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'satellite'
        ),
      );
      break;

    case 'openlayers_layers_yahoo_hybrid':
      _openlayers_layers_include_map_api('yahoo');
      $layer_data = array(
        'id' => $layer,
        'type' => 'Yahoo',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'hybrid'
        ),
      );
      break;

    case 'openlayers_layers_virtual_earth_street':
      _openlayers_layers_include_map_api('virtual_earth');
      $layer_data = array(
        'id' => $layer,
        'type' => 'VirtualEarth',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'street'
        ),
      );
      break;

    case 'openlayers_layers_virtual_earth_satellite':
      _openlayers_layers_include_map_api('virtual_earth');
      $layer_data = array(
        'id' => $layer,
        'type' => 'VirtualEarth',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'satellite'
        ),
      );
      break;

    case 'openlayers_layers_virtual_earth_hybrid':
      _openlayers_layers_include_map_api('virtual_earth');
      $layer_data = array(
        'id' => $layer,
        'type' => 'VirtualEarth',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'params' => array(
          'type' => 'hybrid'
        ),
      );
      break;

    case 'openlayers_layers_osm_mapnik':
      $layer_data = array(
        'id' => $layer,
        'type' => 'XYZ',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://tile.openstreetmap.org/${z}/${x}/${y}.png',
      );
      break;

    case 'openlayers_layers_osm_tah':
      $layer_data = array(
        'id' => $layer,
        'type' => 'XYZ',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://tah.openstreetmap.org/Tiles/tile/${z}/${x}/${y}.png',
      );
      break;

    case 'openlayers_layers_osm_cycle':
      $layer_data = array(
        'id' => $layer,
        'type' => 'XYZ',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://andy.sandbox.cloudmade.com/tiles/cycle/${z}/${x}/${y}.png',
      );
      break;

    case 'openlayers_layers_osm_4326_hybrid':
      $layer_data = array(
        'id' => $layer,
        'type' => 'WMS',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://oam.hypercube.telascience.org/tiles',
        'params' => array(
          'layers' => 'osm-4326-hybrid',
        ),
        'options' => array(
          'isBaseLayer' => FALSE,
          'buffer' => 1,
        ),
      );
      break;

    case 'openlayers_layers_cloudmade':
      $layer_data = array(
        'id' => $layer,
        'type' => 'CloudMade',
        'name' => $layer_info[$layer]['name'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'options' => array(
          'key' => variable_get('openlayers_layers_cloudmade_api', ''),
          'styleId' => variable_get('openlayers_layers_cloudmade_style', ''),
        ),
      );

      // Include JS
      drupal_add_js(variable_get('openlayers_layers_cloudmade_js', ''));
      break;

    case 'openlayers_layers_local_kml':
      $layer_data = array(
        'id' => $layer,
        'type' => 'KML',
        'name' => $layer_info[$layer]['name'],
        'url' => url('kml/node'),
      );
      break;

    default:
      break;
  }

  return $layer_data;
}

/**
 * Include Mapi API
 *
 * Function to include any third-party APIs for layers
 *
 * @param $type
 *   String of type to include
 */
function _openlayers_layers_include_map_api($type = '') {
  static $included = array();

  // Check if already included
  if (!$included[$type] && $type) {
    switch ($type) {
      case 'google':
        // Include Google Map API JS (maybe getting key from GMap)
        drupal_set_html_head('<script src="'. check_url("http://maps.google.com/maps?file=api&v=2&sensor=false&key=" . variable_get('openlayers_layers_google_api', variable_get('googlemap_api_key', ''))) . '" type="text/javascript"></script>');
        break;

      case 'yahoo':
        drupal_set_html_head('<script src="' . check_url("http://api.maps.yahoo.com/ajaxymap?v=3.0&appid=" . variable_get('openlayers_layers_yahoo_api', '')) . '" type="text/javascript"></script>');
        break;

      case 'virtual_earth':
        drupal_set_html_head('<script src="' . check_url("http://dev.virtualearth.net/mapcontrol/mapcontrol.ashx?v=6.1") . '" type="text/javascript"></script>');
        break;

    }
    $included[$type] = TRUE;
  }
}
