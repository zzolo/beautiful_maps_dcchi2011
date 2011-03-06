<?php

/**
 * @file
 * Base class for style types.
 */

/**
 * Default Mapping Style Type Class
 *
 * This provides a default style type class so that style type
 * plugins can be extended with ctools.  See mapping_base_type class.
 */
class mapping_style_type extends mapping_base_type {
  var $base_type = 'style';
  var $export_fields = array('name', 'title', 'description', 'data', 'object_type'); 
}