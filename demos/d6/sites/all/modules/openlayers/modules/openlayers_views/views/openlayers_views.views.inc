<?php
// $Id: openlayers_views.views.inc,v 1.1.2.3 2009/08/06 04:42:06 binarybill Exp $

/**
 * @file
 * This file holds main views functions for the openlayers_views module.
 *
 * @ingroup openlayers
 */

/**
 * Implementation of hook_views_plugins().
 */
function openlayers_views_views_plugins() {
  return array(
    'module' => 'openlayers_views',
    'style' => array(
      'openlayers_views' => array(
        'title' => t('OpenLayers'),
        'help' => t('Displays nodes as an OpenLayers map.'),
        'handler' => 'openlayers_views_style_map',
        'theme' => 'openlayers_views_map',
        'theme file' => 'openlayers_views.views.theme.inc',
        'theme path' => drupal_get_path('module', 'openlayers_views') .'/views',
        'path' => drupal_get_path('module', 'openlayers_views') .'/views',
        'uses fields' => TRUE,
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );
}
