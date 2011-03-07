<?php

/**
 * Mapping Map Type plugin for the mapping_example
 * module.  Provides a basic map type for Google maps.
 */

/**
 * Define the Ctools plugin options.
 */
$plugin = array(
  'title' => t('Google Map'),
  'description' => t('Google Map v3 example type.'),
  'map_type' => array(
    'class' => 'mapping_example_google_map_type',
    'parent' => 'mapping_map_type',
  ),
);

/**
 * Google Map Class
 *
 * This provides a map for the example.
 */
class mapping_example_google_map_type extends mapping_map_type {

  function __construct($export = array(), $map = array()) {
    parent::__construct($export, $map);
    $this->data = $this->merge_options($this->data, $this->options_default());
  }
  
  function options_default() {
    return array(
      'center' => array(0, 0),
      'sensor' => FALSE,
    );
  }

  function options_form() {
    return array(
      'center' => array(
        '#type' => 'textfield',
        '#title' => t('Map center'),
        '#description' => t('The default center of the map.'),
        '#options' => array(),
        '#default_value' => $this->data['center'],
      ),
    );
  }

  function render() {
    // Build URL
    $url = 'http://maps.google.com/maps/api/js';
    $url .= ($this->data['sensor']) ? '?sensor=true' : '?sensor=false';
    drupal_add_js($url, 'external');
    
    // Add mapping JS
    drupal_add_js(drupal_get_path('module', 'mapping_example') . '/plugins/mapping_map_types/mapping_example_google_map_type.js');
  }
}