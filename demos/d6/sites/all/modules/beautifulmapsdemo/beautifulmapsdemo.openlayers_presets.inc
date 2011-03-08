<?php

/**
 * Implementation of hook_openlayers_presets().
 */
function beautifulmapsdemo_openlayers_presets() {
  $export = array();
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'cloudmade_demo';
  $openlayers_presets->title = 'Cloudmade Demo';
  $openlayers_presets->description = 'Demo of Cloudmade Layer and openlayers plus behaviors';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => '',
    'css_path' => 'http://openlayers.org/api/theme/default/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-21.61010742102, 30.263811839658',
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
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_plus_behavior_blockswitcher' => array(
        'map' => array(
          'enabled' => 1,
          'open' => 0,
          'overlay_style' => 'checkbox',
          'position' => 'se',
        ),
      ),
      'openlayers_plus_behavior_legend' => array(),
      'openlayers_plus_behavior_permalink' => array(),
      'openlayers_plus_behavior_tooltips' => array(
        'positioned' => 1,
      ),
    ),
    'default_layer' => 'cloudmade_demo',
    'layers' => array(
      'cloudmade_demo' => 'cloudmade_demo',
      'osm_mapnik' => 'osm_mapnik',
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'layer_styles' => array(
      'mapbox_demo_openlayers_2' => array(
        'default' => 'sports',
        'select' => 'sports',
        'temporary' => 'sports',
      ),
      'mapbox_demo_openlayers_1' => array(
        'default' => 'garden',
        'select' => 'garden',
        'temporary' => 'garden',
      ),
    ),
    'layer_activated' => array(
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'layer_switcher' => array(
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'airport',
      'select' => 'airport',
      'temporary' => 'airport',
    ),
  );

  $export['cloudmade_demo'] = $openlayers_presets;
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
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'demo_image_path';
  $openlayers_presets->title = 'Demo for Image Path';
  $openlayers_presets->description = 'Demo for how the image path settings can work';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => 'http://js.mapbox.com/theme/dark/',
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
    ),
    'layer_styles' => array(),
    'layer_activated' => array(),
    'layer_switcher' => array(),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'demo_attr_replace',
      'select' => 'demo_attr_replace',
      'temporary' => 'demo_attr_replace',
    ),
  );

  $export['demo_image_path'] = $openlayers_presets;
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'mapbox_demo';
  $openlayers_presets->title = 'Mapbox Demo';
  $openlayers_presets->description = 'Demo of MapBox Layer and openlayers plus behaviors';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => '',
    'css_path' => 'http://openlayers.org/api/theme/default/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-23.016357420964, 27.493654052689',
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
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => 'mapbox_world_dark',
        'point_zoom_level' => '5',
      ),
      'openlayers_plus_behavior_blockswitcher' => array(
        'map' => array(
          'enabled' => 1,
          'open' => 0,
          'overlay_style' => 'checkbox',
          'position' => 'se',
        ),
      ),
      'openlayers_plus_behavior_legend' => array(),
      'openlayers_plus_behavior_permalink' => array(),
      'openlayers_plus_behavior_tooltips' => array(
        'positioned' => 1,
      ),
    ),
    'default_layer' => 'mapbox_world_dark',
    'layers' => array(
      'mapbox_world_dark' => 'mapbox_world_dark',
      'osm_mapnik' => 'osm_mapnik',
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'layer_styles' => array(
      'mapbox_demo_openlayers_2' => array(
        'default' => 'sports',
        'select' => 'sports',
        'temporary' => 'sports',
      ),
      'mapbox_demo_openlayers_1' => array(
        'default' => 'garden',
        'select' => 'garden',
        'temporary' => 'garden',
      ),
    ),
    'layer_activated' => array(
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'layer_switcher' => array(
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'airport',
      'select' => 'airport',
      'temporary' => 'airport',
    ),
  );

  $export['mapbox_demo'] = $openlayers_presets;
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'openlayersplus_demo';
  $openlayers_presets->title = 'OpenLayers Plus Demo';
  $openlayers_presets->description = 'Demo of OpenLayers Layer and openlayers plus behaviors';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => '',
    'css_path' => 'http://openlayers.org/api/theme/default/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-23.016357420964, 27.493654052689',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(),
      'openlayers_plus_behavior_blockswitcher' => array(
        'map' => array(
          'enabled' => 1,
          'open' => 0,
          'overlay_style' => 'checkbox',
          'position' => 'se',
        ),
      ),
      'openlayers_plus_behavior_permalink' => array(),
      'openlayers_plus_behavior_popup' => array(),
    ),
    'default_layer' => 'mapbox_world_dark',
    'layers' => array(
      'mapbox_world_dark' => 'mapbox_world_dark',
      'osm_mapnik' => 'osm_mapnik',
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'layer_styles' => array(
      'mapbox_demo_openlayers_2' => array(
        'default' => 'sports',
        'select' => 'sports',
        'temporary' => 'sports',
      ),
      'mapbox_demo_openlayers_1' => array(
        'default' => 'garden',
        'select' => 'garden',
        'temporary' => 'garden',
      ),
    ),
    'layer_activated' => array(
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'layer_switcher' => array(
      'mapbox_demo_openlayers_2' => 'mapbox_demo_openlayers_2',
      'mapbox_demo_openlayers_1' => 'mapbox_demo_openlayers_1',
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'airport',
      'select' => 'airport',
      'temporary' => 'airport',
    ),
  );

  $export['openlayersplus_demo'] = $openlayers_presets;
  return $export;
}
