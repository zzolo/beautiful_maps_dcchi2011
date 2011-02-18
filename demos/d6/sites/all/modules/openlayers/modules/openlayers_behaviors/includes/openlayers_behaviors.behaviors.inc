<?php
// $Id: openlayers_behaviors.behaviors.inc,v 1.7.2.17 2009/12/02 23:08:57 zzolo Exp $

/**
 * @file
 * This file holds the main Drupal hook functions
 * and private functions for the openlayers_behaviors module.
 *
 * @ingroup openlayers
 */

/**
 * Callback for OpenLayers Behaviors (Zoom to Layer)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array
 */
function openlayers_behaviors_process_zoom_to_layer($behavior, &$map) {
  // Check to make sure the layer attribute is properly set.
  if ($behavior['layer']) {
    // If it's a sting, check to make sure the layer exists.
    if (is_string($behavior['layer'])) {
      if ($map['layers'][$behavior['layer']]) {
        return $behavior;
      }
    }
    // If it's an array, check to make sure all layers exist.
    if (is_array($behavior['layer'])) {
      $pass = TRUE;
      foreach ($behavior['layer'] as $layer) {
        if (!$map['layers'][$layer]) $pass = FALSE;
      }
      if ($pass) {
        return $behavior;
      }
    }
  }

  return FALSE;
}

/**
 * Callback for OpenLayers Behaviors (Zoom to Feature)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array
 */
function openlayers_behaviors_process_zoom_to_feature($behavior, &$map) {
  // Check to make sure the layer and feature attribute is properly set.
  if ($behavior['layer'] && $behavior['feature']) {
    // Check to make sure the layer is a string and exists.
    if (is_string($behavior['layer']) && $map['layers'][$behavior['layer']]) {
      // If it's a sting, check to make sure the feature exists.
      if (is_string($behavior['feature'])) {
        if ($map['layers'][$behavior['layer']]['features'][$behavior['feature']]) {
          return $behavior;
        }
      }
      // If it's an array, check to make sure all features exist.
      if (is_array($behavior['feature'])) {
        $pass = TRUE;
        foreach ($behavior['feature'] as $feature) {
          if (!$map['layers'][$behavior['layer']]['features'][$feature]) $pass = FALSE;
        }
        if ($pass) {
          return $behavior;
        }
      }
    }
  }

  return FALSE;
}


/**
 * Callback for OpenLayers Behaviors (Tooltip)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array or FALSE if not valid
 */
function openlayers_behaviors_process_tooltip($behavior, &$map) {
  // Check to make sure the layer attribute is properly set.
  if ($behavior['layer'] && $map['layers'][$behavior['layer']]) {
    drupal_add_css(drupal_get_path('module', 'openlayers_behaviors') . '/openlayers_behaviors.css', 'module');

    // Designate some default values that match up with our default theme
    $behavior['attribute'] = (!empty($behavior['attribute'])) ? $behavior['attribute'] : 'name';
    $behavior['tooltip_id'] = (!empty($behavior['tooltip_id'])) ? $behavior['tooltip_id'] : $map['id'] . '-tooltip';
    $behavior['container_id'] = (!empty($behavior['container_id'])) ? $behavior['container_id'] : $map['id'] . '-tooltip-container';
    $behavior['attribute_id'] = (!empty($behavior['attribute_id'])) ? $behavior['attribute_id'] : $map['id'] . '-tooltip-text';
    $behavior['offset_top'] = (!empty($behavior['offset_top'])) ? $behavior['offset_top'] : 'height';
    $behavior['offset_left'] = (!empty($behavior['offset_left'])) ? $behavior['offset_left'] : 0;

    // Create container
    $behavior['container'] = theme('openlayers_behaviors_tooltip_container', $behavior, $map);

    return $behavior;
  }

  return FALSE;
}

/**
 * Callback for OpenLayers Behaviors (Popup)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array
 */
function openlayers_behaviors_process_popup($behavior, &$map) {
  // Assume we want to display the name attribute if no attribute is specified
  if (!$behavior['attribute']) {
    $behavior['attribute'] = 'name';
  }

  return $behavior;
}

/**
 * Callback for OpenLayers Behaviors (Draw Features)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array or FALSE if not valid
 */
function openlayers_behaviors_process_draw_features($behavior, &$map) {
  // Check to make sure the layer attribute is properly set.
  if ($behavior['layer'] && $map['layers'][$behavior['layer']]) {
    // Add CSS
    drupal_add_css(drupal_get_path('module', 'openlayers_behaviors') . '/openlayers_behaviors.css', 'module');

    // Add mapReady Event
    $map['events'] = ($map['events']) ? $map['events'] : array();
    $map['events']['mapReady'] = ($map['events']['mapReady']) ? $map['events']['mapReady'] : array();
    $map['events']['mapReady']['drawFeaturesMapReady'] = 'drawFeaturesMapReady'; // We can only ever have one map control - even if it handles multiple draw behaviors

    return $behavior;
  }

  return FALSE;
}

/**
 * Callback for OpenLayers Behaviors (Fullscreen)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array
 */
function openlayers_behaviors_process_fullscreen($behavior, &$map) {
  drupal_add_css(drupal_get_path('module', 'openlayers_behaviors') .'/openlayers_behaviors.css', 'module');
  return $behavior;
}

/**
 * Callback for OpenLayers Behaviors (De-clutter)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array or FALSE if not valid
 */
function openlayers_behaviors_process_declutter($behavior, &$map) {
  if ($behavior['layer'] && $map['layers'][$behavior['layer']]) {

    // Add Events to map
    $map['events'] = ($map['events']) ? $map['events'] : array();
    $map['events']['zoomend'] = ($map['events']['zoomend']) ? $map['events']['zoomend'] : array();
    $map['events']['zoomend'][] = 'declutterZoomEnd';

    // @@TODO: Add Events to layer (features added + removed)

    return $behavior;
  }

  return FALSE;
}


/**
 * Callback for OpenLayers Behaviors (Cluster)
 *
 * @param $behavior
 *   Data about behavior
 * @param $map
 *   Map array
 * @return
 *   Behavior array or FALSE if not valid
 */
function openlayers_behaviors_process_cluster($behavior, &$map) {
  if ($behavior['layer'] && $map['layers'][$behavior['layer']]) {
    // Make some defaults
    $behavior['distance'] = empty($behavior['distance']) ? '20' : $behavior['distance'];
    $behavior['threshold'] = empty($behavior['threshold']) ? '1' : $behavior['threshold'];

    return $behavior;
  }

  return FALSE;
}
