<?php
// $Id: openlayers.api.php,v 1.1.2.5 2009/09/14 00:39:51 zzolo Exp $

/**
 * @file
 * Hooks provided by the OpenLayers suite of modules.
 *
 * @ingroup openlayers
 */

/**
 * OpenLayers Map Preprocess Alter
 *
 * Map array alter.  Fired before processing the array, and
 * before checking for errors.  The whole array is passed
 * along and will allow you to alter it in any way.  This
 * is a good place to alter the map, if the other hooks
 * do not provide the functionality you need.
 *
 * @param $map
 *   Map array
 */
function hook_openlayers_map_preprocess_alter(&$map = array()) {
  // Do something to the $map
}

/**
 * OpenLayers Map Alter
 *
 * Post-processing Map array alter.  Fired after processing the array, and
 * before checking for errors.  The whole array is passed
 * along and will allow you to alter it in any way.  Adding behaviors,
 * pre-defined layers here will not work. This is good for minor tweaks
 * after the map has been processed.
 *
 * @param $map
 *   Map array
 */
function hook_openlayers_map_alter(&$map = array()) {
  // Do something to the $map
}

/**
 * OpenLayers Layer Handler Info
 *
 * Provides information on layer handlers.  Every layer needs
 * to have a valid type (layer handler).
 *
 * @param $map
 *   Map array of map being rendered
 * @return
 *   Return a nested associative array with the top level
 *   being a unique string identifier key which corresponds to the
 *   layers' types.  The next level being an array of key/value
 *   pairs:
 *   - "layer_handler": This is the JS callback name that will
 *     belong to OL.Layers object.
 *   - "js_file": The JS file to include to look for the callback.
 */
function hook_openlayers_layers_handler_info($map = array()) {
  // Take from openlayers.module

  return array(
    'WMS' => array(
      'layer_handler' => 'WMS',
      'js_file' => drupal_get_path('module', 'openlayers') .'/js/openlayers.layers.js',
    ),
    'Vector' => array(
      'layer_handler' => 'Vector',
      'js_file' => drupal_get_path('module', 'openlayers') .'/js/openlayers.layers.js',
    ),
  );
}

/**
 * OpenLayers Layers Info
 *
 * This hook tells OpenLayers about the available layers
 * that can be used by name in maps.  Layers can still be defined
 * manually, but this allows for easy calling of layers,
 * and these will show up in the Preset UI.
 *
 * @return
 *   Return a nested associative array with the top level
 *   being a unique string identifier, and the nested array
 *   containing the following key/pairs:
 *   - "name": Translated name of the layer.  This will show up
 *     in the Preset UI.
 *   - "description": Translates description.
 *   - "file": The Drupal path for where the callback is stored
 *   - "callback": The name of the PHP function that will be called
 *     when the layer is rendered
 *   - "projection": An array of projections that the layer is
 *     compatible with.  Leave empty if compatible with all.
 *   - "baselayer": Boolean whether the layer is a base layer
 *     or not.
 */
function hook_openlayers_layers_info() {
  // Taken from openlayers.module

  // Define info array
  $info['openlayers_default_wms'] = array(
    'name' => t('Default OpenLayers WMS'),
    'description' => t('A simple basemap to get you started'),
    'file' => drupal_get_path('module', 'openlayers') .'/includes/openlayers.layers.inc',
    'callback' => 'openlayers_process_layers',
    'projection' => array('4326', '900913', '4269'),
    'baselayer' => TRUE,
  );

  return $info;
}

/**
 * OpenLayers Behaviors Info
 *
 * This hook tells OpenLayers about the available behaviors
 * that can be used by name in maps.
 *
 * @return
 *   Return a nested associative array with the top level
 *   being a unique string identifier, and the nested array
 *   containing the following key/pairs:
 *   - "name": Translated name of the behavior.
 *   - "description": Translates description.
 *   - "file": The Drupal path for where the PHP callback is stored
 *   - "callback": The name of the PHP function that will be called
 *     when the behavior is rendered
 *   - "js_file": The Drupal path for where the JS callback is stored
 *   - "js_callback": The name of the JS function that will be called
 *     when the behavior is rendered.  This will be a function of the
 *     OL.Behaviors object
 */
function hook_openlayers_behaviors_info() {
  // Taken from openlayers_behaviors.module

  $file = drupal_get_path('module', 'openlayers_behaviors') .'/includes/openlayers_behaviors.behaviors.inc';
  $js_file = drupal_get_path('module', 'openlayers_behaviors') .'/js/openlayers_behaviors.behaviors.js';
  $info = array();

  // Define info array
  $info['openlayers_behaviors_zoom_to_layer'] = array(
    'name' => t('Zoom to Layer'),
    'description' => t('When the map is finished loading, zoom to the features contained within the given layer'),
    'file' => $file,
    'callback' => 'openlayers_behaviors_process_zoom_to_layer',
    'js_file' => $js_file,
    'js_callback' => 'zoomToLayer',
  );

  return $info;
}

/**
 * OpenLayers Style Info
 *
 * This hook tells OpenLayers about the available styles
 * that can be used by name in maps.  These will show up
 * in the Preset UI.
 *
 * @return
 *   Return a nested associative array with the top level
 *   being a unique string identifier, and the nested array
 *   containing the following key/pairs:
 *   - "name": Translated name of the style.
 *   - "description": Translated description.
 *   - "file": The Drupal path for where the PHP callback is stored
 *   - "callback": The name of the PHP function that will be called
 *     when the behavior is rendered
 */
function hook_openlayers_styles_info() {
  // Taken from openlayers.module

  // Define info array
  $info['default'] = array(
    'name' => t('Default Style'),
    'description' => t('Basic default style.'),
    'file' => drupal_get_path('module', 'openlayers') .'/includes/openlayers.styles.inc',
    'callback' => 'openlayers_process_styles',
  );
  $info['default_select'] = array(
    'name' => t('Default Select Style'),
    'description' => t('Default style for selected geometries'),
    'file' => drupal_get_path('module', 'openlayers') .'/includes/openlayers.styles.inc',
    'callback' => 'openlayers_process_styles',
  );

  return $info;
}

/**
 * OpenLayers Presets
 *
 * This hook lets other modules define map presets that
 * the user can choose from in various places, or
 * clone.
 *
 * @return
 *   Return a nested associative array with the top level
 *   being a unique string identifier, and the nested array
 *   containing the following key/pairs:
 *   - "preset_name": Unique string with only lowercase characters
 *     and underscores.
 *   - "preset_title": Translated title to be used listing presets.
 *   - "preset_description": Translated description.
 *   - "preset_data": The unrenderd map array
 */
function hook_openlayers_presets() {
  // Taken from openlayers.module

  $presets = array();

  // Create map array
  $default_map = array(
    'projection' => '4326',
    'width' => 'auto',
    'default_layer' => 'openlayers_default_wms',
    'height' => '300px',
    'center' => array(
      'lat' => '0',
      'lon' => '0',
      'zoom' => '2',
    ),
    'options' => array(
      'displayProjection' => '4326',
    ),
    'controls' => array(
      'LayerSwitcher' => TRUE,
      'Navigation' => TRUE,
      'PanZoomBar' => TRUE,
      'MousePosition' => TRUE,
    ),
  );

  // Create full preset array
  $presets['default'] = array(
    'preset_name' => 'default',
    'preset_title' => t('Default Map'),
    'preset_description' => t('This is the default map preset that comes with the OpenLayers module.'),
    'preset_data' => $default_map,
  );

  return $presets;
}
