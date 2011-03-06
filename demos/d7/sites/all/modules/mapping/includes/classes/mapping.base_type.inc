<?php

/**
 * @file
 * Base class for all object types.
 */

/**
 * Default Object Type Class
 *
 * This provides a default object type class that are common
 * for all object types.
 */
class mapping_base_type {
  var $map = array();
  var $base_type = '';
  var $export_fields = array(); 

  /**
   * Constructor to set properties of the object.
   */
  function __construct($export = array(), $map = array()) {
    // Go through exportable fields to set properties of object.
    foreach ($this->export_fields as $k) {
      if (isset($export->{$k})) {
        $this->{$k} = $export->{$k};
      }
    }

    // Determine any existing options in the data array
    // and set defaults if necessary
    if (isset($this->data) && is_array($this->data)) {
      $this->data += $this->default_options();
    }
    
    // Set map property (not needed for maps)
    $this->map = $map;
  }

  /**
   * Merge options.
   *
   * @param $data
   *   A keyed array of object specific options.
   * @param $defaults
   *   A keyed array of default options.
   *
   * @return
   *   A keyed array of merged options.
   */
  function merge_options($data, $defaults) {
    $merged = array();
    if (is_array($defaults) && is_array($data)) {
      $merged = $data + $defaults;
    }
    return $merged;
  }

  /**
   * Default function for intializing any options for
   * the type.  If the exteneded types has
   * options, override this and set here.
   *
   * @return
   *   A keyed array of options.
   */
  function default_options() {
    return array();
  }

  /**
   * Default function for the form to use when creating
   * specific instances of this object.
   *
   * @return
   *   A Drupal form array.
   */
  function options_form() {
    return array();
  }

  /**
   * Default function for the form to use for options
   * that are for all objects of this specific type.
   *
   * @return
   *   A Drupal form array.
   */
  function type_form() {
    return array();
  }

  /**
   * Function to create an export for the object instance.
   *
   * @return
   *   A version of this object which can be saved,
   *   when attempting to save a modified object
   */
  function create_export() {
    $return = array();
    foreach ($this->export_fields as $k) {
      $return[$k] = $this->{$k};
    }
    return $return;
  }

  /**
   * Save the export instance to the database.
   *
   * @return
   *   Success value on saving this export.
   */
  function save() {
    $table = 'mapping_' . $this->base_type . 's';
  
    if (!empty($this->name)) {
      return (db_select($table)
          ->fields($table, array('name'))
          ->condition('name', $this->name)
          ->execute()
          ->fetchCol()) ?
        drupal_write_record($table, $this->create_export(), 'name') :
        drupal_write_record($table, $this->create_export());
    }
  }

  /**
   * Render the object.  This is usually where javascript
   * or other files can be loaded and changes to the main map
   * array can be altered if need.
   */
  function render(&$map) {
    // No return.
  }
}