<?php

/**
 * Implementation of hook_openlayers_layers().
 */
function beautifulmapsdemo_openlayers_layers() {
  $export = array();
  $openlayers_layers = new stdClass;
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'mapbox_world_dark';
  $openlayers_layers->title = 'Mapbox World Dark';
  $openlayers_layers->description = 'Mapbox World Dark';
  $openlayers_layers->data = array(
    'layername' => 'world-dark',
    'layer_type' => 'openlayers_layer_type_mapbox',
    'osm' => 1,
    'baselayer' => 1,
    'type' => 'png',
    'resolutions' => array(
      0 => 156543.0339,
      1 => 78271.51695,
      2 => 39135.758475,
      3 => 19567.8792375,
      4 => 9783.93961875,
      5 => 4891.96980938,
      6 => 2445.98490469,
      7 => 1222.99245234,
      8 => 611.496226172,
      9 => 305.748113086,
      10 => 152.874056543,
      11 => 76.4370282715,
      12 => 38.2185141357,
      13 => 19.1092570679,
      14 => 9.55462853394,
      15 => 4.77731426697,
      16 => 2.38865713348,
      17 => 1.19432856674,
      18 => 0.597164283371,
    ),
    'projection' => array(
      0 => '900913',
    ),
  );

  $export['mapbox_world_dark'] = $openlayers_layers;
  return $export;
}
