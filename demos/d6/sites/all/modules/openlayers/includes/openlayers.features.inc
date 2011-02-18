<?php
// $Id: openlayers.features.inc,v 1.1.2.2 2009/08/06 04:42:05 binarybill Exp $

/**
 * @file
 * OpenLayers Features Support
 *
 * Provides a programmatic API to the exportable presets
 * that the OpenLayers module uses for map settings.
 */

/**
 * Implementation of hook_features_export().
 */
function openlayers_features_export($data, &$export, $module_name = '') {
  // Collect a module to preset map
  $map = array();
  $modules = module_implements('openlayers_presets');
  foreach ($modules as $module) {
    $presets = module_invoke($module, 'openlayers_presets');
    foreach ($presets as $preset) {
      $map[$preset['presetname']] = $module;
    }
  }
  foreach ($data as $preset) {
    // If another module provides this preset, add it as a dependency
    if (isset($map[$preset]) && $map[$preset] != $module_name) {
      $module = $map[$preset];
      $export['dependencies'][$module] = $module;
    }
    // Otherwise, export the preset
    else {
      $export['features']['openlayers'][$preset] = $preset;
    }
  }
}

/**
 * Implementation of hook_features_export_render().
 * @param $module_name The name of the module for which things are
 *  being exported
 * @param $data An array of key/value data to export, in the form
 *  array('presetname',... 'presetname2')
 */
function openlayers_features_export_render($module_name, $data) {
  $omit = array('presetid', 'storage');

  $items = array();
  foreach ($data as $key) {
    $preset = openlayers_get_preset($key);
    foreach ($omit as $i) {
      unset($preset[$i]);
    }
    $items[$key] = $preset;
  }
  $code = "  \$items = ". features_var_export($items, '  ') .";\n";
  $code .= '  return $items;';
  return array('openlayers_presets' => $code);
}

/**
 * Implementation of hook_features_export_options().
 * @return array of preset names we can export
 */
function openlayers_features_export_options() {
  $preset_list = array();
  $presets = openlayers_get_presets();
  foreach ($presets as $preset_name => $preset) {
    $preset_list[$preset_name] = $preset_name;
  }
  ksort($preset_list);
  return $preset_list;
}

/**
 * @param $preset The name of the preset to be deleted
 * @return None
 */
function openlayers_delete_preset($preset) {
  $preset = openlayers_get_preset($preset, TRUE);

  if ($preset['type'] == OPENLAYERS_STORAGE_DEFAULT) {
    drupal_set_message(t('Cannot delete %name because it is provided by a module.', array('%name' => $preset['preset_name'])));
  }
  else {
    // Create query
    $query = "DELETE FROM {openlayers_map_presets} WHERE preset_id = %d";
    $result = db_query($query, $preset['preset_id']);

    // Check query results
    if ($result) {
      drupal_set_message(t('Preset %name was deleted.', array('%name' => $preset['preset_name'])));
    }
    else {
      drupal_set_message(t('Error when attempting to delete preset.'), 'error');
    }
  }
}

/**
 * Implementation of hook_features_revert().
 *
 * @param $module
 * name of module to revert content for
 */
function openlayers_features_revert($module = NULL) {
  // Get all presets defined in this feature
  if (module_hook($module, 'openlayers_presets')) {
    $default_presets = module_invoke($module, 'openlayers_presets');

    // Get all DB defined presets
    $current_presets = openlayers_get_presets();
    // update all presets to their default values
    foreach ($default_presets as $default_preset) {
      foreach ($current_presets as $current_preset) {
        if ($current_preset['presetname'] == $default_preset['presetname']) {
          openlayers_delete_preset($current_preset);
        }
      }
    }
  }
  else {
    drupal_set_message(t('Could not load default OpenLayers presets.'), 'error');
    return FALSE;
  }
  return TRUE;
}
