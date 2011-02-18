<?php
// $Id: openlayers.styles.inc,v 1.1.2.2 2009/09/12 18:21:23 zzolo Exp $

/**
 * @file
 * This file contains styles instances used for defaults
 *
 * @ingroup openlayers
 */

/**
 * Process Styles
 *
 * Call back to process styles provided by the
 * openlayers_openlayers_styles_info() function
 *
 * @param $style
 *   String ID for the style
 * @param $map
 *   Array of current map being processed
 * @return
 *   Array formated for an OpenLayers map style
 */
function openlayers_process_styles($style = NULL, $map = array()) {
  $style_data = array();

  // Make sure layer is a string
  if (!is_string($style)) {
    return FALSE;
  }

  // Determine what data to send back
  switch ($style) {
    case 'default':
      $style_data = array(
        'pointRadius' => 5,
        'fillColor' => '#FFCC66',
        'strokeColor' => '#FF9933',
        'strokeWidth' => 2,
        'fillOpacity' => 0.5
      );
      break;

    case 'default_select':
      $style_data = array(
        'pointRadius' => 5,
        'fillColor' => '#66CCFF',
        'strokeColor' => '#3399FF',
        'strokeWidth' => 2,
        'fillOpacity' => 0.5
      );
      break;

  }

  return $style_data;
}
