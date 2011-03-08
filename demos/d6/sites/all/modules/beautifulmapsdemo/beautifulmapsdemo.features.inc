<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function beautifulmapsdemo_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "openlayers" && $api == "openlayers_presets") {
    return array("version" => 1);
  }
  elseif ($module == "openlayers" && $api == "openlayers_styles") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function beautifulmapsdemo_node_info() {
  $items = array(
    'openlayers_test_type' => array(
      'name' => t('OpenLayers Test Type'),
      'module' => 'features',
      'description' => t('This is a test content type for OpenLayers.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function beautifulmapsdemo_views_api() {
  return array(
    'api' => '2',
  );
}
