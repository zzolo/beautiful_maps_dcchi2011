<?php
// $Id: openlayers.layers.inc,v 1.7.2.2 2009/10/28 19:26:25 phayes Exp $

/**
 * @file
 * This file contains layer instances for the default openlayers WMS
 *
 * @ingroup openlayers
 */

/**
 * Process Layers
 *
 * Call back to process layers provided by the
 * openlayers_openlayers_layers_info() function
 *
 * @param $layer
 *   String ID for the layer
 * @return
 *   Array formated for an OpenLayers map layer
 */
function openlayers_process_layers($layer = NULL) {
  $layer_data = array();

  // Make sure layer is a string
  if (!is_string($layer)) {
    return FALSE;
  }

  // Get info array to reference title
  $layer_info = openlayers_openlayers_layers_info();

  // Determine what data to send back
  switch ($layer) {
    case 'openlayers_default_wms':
      $layer_data = array(
        'id' => $layer,
        'type' => 'WMS',
        'name' => $layer_info[$layer]['name'],
        'description' => $layer_info[$layer]['description'],
        'projection' => $layer_info[$layer]['projection'],
        'baselayer' => $layer_info[$layer]['baselayer'],
        'url' => 'http://labs.metacarta.com/wms/vmap0',
        'params' => array(
          'layers' => 'basic',
        ),
        'options' => array(),
        'events' => array(),
      );
    
    // If baselayer is set to false, we need to ask WMS for a transparent layer.
    // If the user has set transparency explicitly, we do not ovverride.
    if ($layer_data['baselayer'] == FALSE && !isset($layer_data['params']['transparent'])) {
      $layer_data['params']['transparent'] = TRUE;
    }
    
    break;

  }

  return $layer_data;
}
