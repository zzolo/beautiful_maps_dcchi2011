<?php

/**
 * Mapping Layer Type plugin for the mapping_example
 * module.  Provides a basic layer for Google maps.
 */

/**
 * Define the Ctools plugin options.
 */
$plugin = array(
  'title' => t('Google Layer'),
  'description' => t('Provides a layer type for Google maps.'),
  'map_type' => 'mapping_example_google_map_type',
  'layer_type' => array(
    'class' => 'mapping_example_google_type',
    'parent' => 'mapping_layer_type',
  ),
);

/**
 * Google Layer Class
 *
 * This provides a layer for the example.
 */
class mapping_example_google_type extends mapping_layer_type {

  function __construct($export = array(), $map = array()) {
    parent::__construct($export, $map);
    $this->data = $this->merge_options($this->data, $this->options_default());
  }

  function options_default() {
    return array(
      'type' => '',
    );
  }

  function options_form() {
    return array(
      'type' => array(
        '#type' => 'select',
        '#title' => t('Type of layer'),
        '#description' => t('The type of Google layer.'),
        '#options' => array(),
        '#default_value' => $this->data['type'],
      ),
    );
  }

  function render(&$map) {
    // Include some JS.
  }
}