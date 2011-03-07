<?php

/**
 * @file
 * Base class for map types.
 */

/**
 * Default Mapping Map Type Class
 *
 * This provides a default map type class so that map type
 * plugins can be extended with ctools.  See mapping_base_type class.
 */
class mapping_map_type extends mapping_base_type {
  var $base_type = 'map';
  var $export_fields = array('name', 'title', 'description', 'data', 
    'object_type', 'layers', 'styles', 'behaviors'); 
}