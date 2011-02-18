<?php
// $Id: openlayers.module_builder.inc,v 1.1.2.3 2009/10/01 22:54:03 phayes Exp $

/**
 * @file
 * This file holds the hook implementations for the Module Builder module.
 */

/**
 * Implementation of hook_module_builder_info().
 */
function openlayers_module_builder_info() {
  $data = array(
    'openlayers' => array(
      'url' => 'http://cvs.drupal.org/viewvc.py/drupal/contributions/modules/openlayers/docs/openlayers.api.php?view=co&pathrev=DRUPAL-6--1',
      'branch' => 'DRUPAL-6--1',
      'hook_files' => array(
        'openlayers.api.php' => '%module.module',
      ),
    ),
  );

  return $data;
}
