<?php

/**
 * @file
 * Hooks provided by the Mapping module.  Please see the
 * mapping_example module for more in-depth examples of
 * these hooks.
 * 
 * This file allows hooks to be documented automatically with 
 * Doxygen, like on api.drupal.org.
 *
 * @ingroup mapping
 */

/**
 * Mapping Map Hook
 *
 * This hook provides default maps to the mapping module.  It
 * is a standard Ctools implementation.
 *
 * @return
 *   An array of map objects.
 */
function hook_mapping_maps() {
  $maps = array();
  
  // This is an example map export for the example Google
  // map type provided with this module.
  $map = new stdClass();
  $map->api_version = 1;
  $map->name = 'mapping_example_google_map_example';
  $map->title = t('Google Map (Example)');
  $map->description = t('This is an example map using the Mapping framework.');
  $map->type = 'mapping_example_google_map_type';
  $map->layers = array(
    'mapping_example_google_terrain_layer_type' => array(),
  );
  $map->styles = array(
    'mapping_example_google_marker_style_type' => array(),
  );
  $map->behaviors = array(
    'mapping_example_google_control_behavior' => array(),
  );
  $map->data = array(
    'center' => array(0, 0),
  );
  $maps[$map->name] = $map;
  
  return $maps;
}