<?php

/**
 * Implementation of hook_openlayers_styles().
 */
function beautifulmapsdemo_openlayers_styles() {
  $export = array();
  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'demo_attr_replace';
  $openlayers_styles->title = 'Demo Attribute Replace';
  $openlayers_styles->description = 'Uses CCK field to replace colors';
  $openlayers_styles->data = array(
    'pointRadius' => 8,
    'fillColor' => '${field_color_value}',
    'strokeColor' => '${field_color_value}',
    'strokeWidth' => 1,
    'fillOpacity' => 0.5,
    'strokeOpacity' => 1,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );

  $export['demo_attr_replace'] = $openlayers_styles;
  return $export;
}
