<?php

/**
 * @file
 * Base class for layer types.
 */

/**
 * Default Mapping Layer Type Class
 *
 * This provides a default layer type class so that layer type
 * plugins can be extended with ctools.  See mapping_base_type class.
 */
class mapping_layer_type extends mapping_base_type {
  var $base_type = 'layer';
  var $export_fields = array('name', 'title', 'description', 'data', 'object_type');
}