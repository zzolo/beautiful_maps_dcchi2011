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
  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'drupalcon';
  $openlayers_styles->title = 'DrupalCon';
  $openlayers_styles->description = 'DrupalCon Logo';
  $openlayers_styles->data = array(
    'externalGraphic' => 'http://beautiful_maps_dcchi2011/demos/d6/sites/all/modules/beautifulmapsdemo/images/drupalcon.png',
    'pointRadius' => 8,
    'strokeWidth' => 1,
    'fillOpacity' => 1,
    'strokeOpacity' => 1,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );

  $export['drupalcon'] = $openlayers_styles;
  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'openlayersblue';
  $openlayers_styles->title = 'OpenLayers Blue';
  $openlayers_styles->description = 'A blue style for point data';
  $openlayers_styles->data = array(
    'pointRadius' => 20,
    'fillColor' => '#000066',
    'strokeColor' => '#000066',
    'strokeWidth' => 4,
    'fillOpacity' => 0.5,
    'strokeOpacity' => 1,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );

  $export['openlayersblue'] = $openlayers_styles;
  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'openlayersred';
  $openlayers_styles->title = 'OpenLayers Red';
  $openlayers_styles->description = 'A red style for point data';
  $openlayers_styles->data = array(
    'pointRadius' => 10,
    'fillColor' => '#660000',
    'strokeColor' => '#660000',
    'strokeWidth' => 4,
    'fillOpacity' => 0.5,
    'strokeOpacity' => 1,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );

  $export['openlayersred'] = $openlayers_styles;
  return $export;
}
