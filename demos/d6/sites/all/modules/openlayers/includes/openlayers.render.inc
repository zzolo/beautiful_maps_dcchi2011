<?php
// $Id: openlayers.render.inc,v 1.1.2.4 2009/08/20 23:48:53 binarybill Exp $

/**
 * @file
 * This file holds the functions the extra processing of rendering a map
 *
 * @ingroup openlayers
 */

/**
 * Process Layers
 *
 * Get full data for any layers and add handlers
 *
 * @param $layers
 *   Array of layers to process
 * @param $map
 *   Map array
 * @return
 *   Array of processed layers
 */
function _openlayers_layers_process($layers = array(), &$map = array()) {
  if (!$layers) {
    $layers = array();
  }
  if (!is_array($map)) {
    $map = array();
  }
  $processed = array();

  // Get layer info array
  $layer_info = openlayers_layers_get_info();

  // Check to make sure our default layer is present, if it isn't then add it.
  if (!array_key_exists($map['default_layer'], $layers) && ($map['default_layer'])) {
    $layers[$map['default_layer']] = $map['default_layer'];
  }

  // Go through layers
  foreach ($layers as $k => $layer) {
    // Check if array, if array, just pass on
    if (is_array($layer)) {
      $processed[$k] = $layer;
    }
    else {
      // If not array, we want to include the file and call the function
      if (($info = $layer_info[$layer]) && is_array($layer_info[$layer])) {
        // Check if file exists
        if (is_file('./'. $info['file'])) {
          require_once './'. $info['file'];
          // Check for function
          if (function_exists($info['callback'])) {
            // Call function and give it the layer name
            $result = $info['callback']($layer, $map);
            // Check for result
            if (isset($result) && is_array($result)) {
              $processed[$layer] = $result;
            }
          }
        }
      }
    }
  }

  // Add Handlers
  $handlers = module_invoke_all('openlayers_layers_handler_info', $map);
  // Go through processed
  foreach ($processed as $k => $l) {
    // Check for handler
    if (is_string($handlers[$l['type']]['layer_handler'])) {
      $processed[$k]['layer_handler'] = $handlers[$l['type']]['layer_handler'];
      // Include JS file if there is one
      if (is_string($handlers[$l['type']]['js_file'])) {
        drupal_add_js($handlers[$l['type']]['js_file'], 'module');
      }
    }
  }

  // Return processed
  return $processed;
}

/**
 * Process Behaviors
 *
 * Get full data for any behaviors and add handlers
 *
 * @param $behaviors
 *   Array of behaviors to process
 * @param $map
 *   Map array
 * @return
 *   Array of processed behaviors
 */
function _openlayers_behaviors_process($behaviors = array(), &$map = array()) {
  // Check input
  if (!is_array($behaviors)) {
    $behaviors = array();
  }
  if (!is_array($map)) {
    $map = array();
  }

  // Initialized variables
  $processed = array();
  // Get behavior info array
  $behavior_info = openlayers_behaviors_get_info();

  // Go through behaviors
  foreach ($behaviors as $k => $behavior) {
    // Check if array, if array, just pass on
    if (is_array($behaviors)) {
      $processed[$behavior['id']] = $behavior;
    }
    else {
      $processed[$behavior] = array(
        'id' => $behavior,
        'type' => $behavior,
      );
    }
  }

  // Go through processed
  foreach ($processed as $bkey => $behavior) {
    $info = $behavior_info[$behavior['type']];

    if (is_file($info['file'])) {
      require_once $info['file'];
      // Check for function
      if (function_exists($info['callback'])) {
        // Call function and give it the behavior array
        $result = $info['callback']($behavior, $map);

        // Check for result
        if (isset($result) && is_array($result)) {
          $result['js_callback'] = $info['js_callback'];
          $processed[$bkey] = $result;
        }
        else {
          unset($processed[$bkey]);
        }
      }
      // Include JavaScript
      if ($info['js_file']) {
        drupal_add_js($info['js_file'], 'module');
      }
    }
  }

  // Return processed
  return $processed;
}

/**
 * Process Styles
 *
 * Get full data for any styles
 *
 * @param $styles
 *   Array of styles to process
 * @param $map
 *   Map array
 * @return
 *   Array of processed styles
 */
function _openlayers_styles_process($styles = array(), &$map = array()) {
  // Check input
  if (!is_array($styles)) {
    $styles = array();
  }
  if (!is_array($map)) {
    $map = array();
  }

  // Initialized variables
  $processed = array();
  // Get behavior info array
  $styles_info = openlayers_styles_get_info();
  // Go through styles
  foreach ($styles as $k => $style) {
    // Check if array, if array, just pass on
    if (is_array($style)) {
      $processed[$k] = $style;
    }
    else {
      // If not array, we want to include the file and call the function
      if (($info = $styles_info[$style]) && is_array($styles_info[$style])) {
        // Check if file exists
        if (is_file('./'. $info['file'])) {
          require_once './'. $info['file'];
          // Check for function
          if (function_exists($info['callback'])) {
            // Call function and give it the style name
            $result = $info['callback']($style, $map);
            // Check for result
            if (isset($result) && is_array($result)) {
              $processed[$k] = $result;
            }
          }
        }
      }
    }
  }

  // Run through theme function
  $processed = theme('openlayers_vector_styles', $processed, $map);

  // Return processed
  return $processed;
}

/**
 * Create Map ID
 *
 * Create a unique ID for any maps that are not assigned an ID
 *
 * @note
 *   Technically someone can assign a map ID identical
 *   to the one that is created
 * @return
 *   New map id
 */
function _openlayers_create_map_id() {
  // Set up variables
  $map_id = '';
  static $map_count = 0;
  // Put together ID
  $map_id = OPENLAYERS_MAP_ID_PREFIX .'-'. $map_count;
  // Add another
  $map_count += 1;
  // Return ID
  return $map_id;
}

/**
 * Debug Map
 *
 * Offers debugging if enabled.  Options are available in the
 * administration interface to enable different kinds of
 * debugging at different points of rendering.
 *
 * @param $op
 *   The operation that is getting carried out
 * @param $map
 *   map array to debug
 * @return
 *   none
 */
function _openlayers_render_debug_map($op = NULL, $map = array()) {
  // Check op
  switch ($op) {
    case 'pre-render':
      // Screen output, check function first
      if (function_exists('dpm')) {
        if (variable_get('openlayers_debug_screen_pre_render', FALSE)) {
          dpm($map);
        }
      }
      // Check watchdog
      if (variable_get('openlayers_debug_watchdog_pre_render', FALSE)) {
        watchdog('openlayers', '<p>Pre render output.</p><pre>!array_output</pre>', array('!array_output' => var_export($map, TRUE)),  WATCHDOG_DEBUG);
      }
      break;

    case 'post-render':
      // Screen output, check function first
      if (function_exists('dpm')) {
        if (variable_get('openlayers_debug_screen_post_render', FALSE)) {
          dpm($map);
        }
      }
      // Check watchdog
      if (variable_get('openlayers_debug_watchdog_post_render', FALSE)) {
        watchdog('openlayers', '<p>Post render output.</p><pre>!array_output</pre>', array('!array_output' => var_export($map, TRUE)),  WATCHDOG_DEBUG);
      }
      break;

  }
}
