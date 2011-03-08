<?php

/**
 * Implementation of hook_openlayers_presets().
 */
function beautifulmapsdemo_openlayers_presets() {
  $export = array();
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'demo_attr_replace';
  $openlayers_presets->title = 'Demo for Attr Replace';
  $openlayers_presets->description = 'Demo for attribute replacements in styles';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'roundedCorner' => 1,
        'roundedCornerColor' => 'darkblue',
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
      ),
      'openlayers_behavior_panzoombar' => array(),
    ),
    'default_layer' => 'osm_mapnik',
    'layers' => array(
      'osm_mapnik' => 'osm_mapnik',
      'osm_cycle' => 'osm_cycle',
      'demo_attr_replace_openlayers_1' => 'demo_attr_replace_openlayers_1',
    ),
    'layer_styles' => array(
      'demo_attr_replace_openlayers_1' => array(
        'default' => 'demo_attr_replace',
        'select' => 'demo_attr_replace',
        'temporary' => 'demo_attr_replace',
      ),
    ),
    'layer_activated' => array(
      'demo_attr_replace_openlayers_1' => 'demo_attr_replace_openlayers_1',
    ),
    'layer_switcher' => array(
      'demo_attr_replace_openlayers_1' => 'demo_attr_replace_openlayers_1',
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'demo_attr_replace',
      'select' => 'demo_attr_replace',
      'temporary' => 'demo_attr_replace',
    ),
  );

  $export['demo_attr_replace'] = $openlayers_presets;
  return $export;
}
