<?php
// $Id: openlayers_layers.admin.inc,v 1.2.2.2 2009/10/06 01:36:12 zzolo Exp $

/**
 * @file
 * This file holds the functions for the openlayers layers Admin settings.
 *
 * @ingroup openlayers
 */

/**
 * Menu callback; displays the openlayers_layers module settings page.
 *
 * @see system_settings_form()
 */
function openlayers_layers_admin_settings() {
  // Get form items
  $form = _openlayers_layers_settings_form();
  return system_settings_form($form);
}

/**
 * Abstracted Form Elements for Layer Settings
 *
 * Since the Domain module would like to know about our form elements,
 * we abstract them into this function so that we can re-use code
 * properly.
 *
 * @return
 *   From array
 */
function _openlayers_layers_settings_form() {
  $form = array();

  // Google Map API Key
  $form['openlayers_google'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Layer Settings'),
    '#description' => t('The settings needed to get the Google layer working.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['openlayers_google']['openlayers_layers_google_api'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps API Key'),
    '#description' => t('Your personal Googlemaps API key.  You must get this for each separate website at <a href="!google_url">Google Map API website</a>.',
      array('!google_url' => 'http://www.google.com/apis/maps/')),
    '#default_value' => variable_get('openlayers_layers_google_api', variable_get('googlemap_api_key', '')),
  );

  // Yahoo Map API Key
  $form['openlayers_yahoo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yahoo Layer Settings'),
    '#description' => t('The settings needed to get the Yahoo layer working.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['openlayers_yahoo']['openlayers_layers_yahoo_api'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo Maps API Key'),
    '#description' => t('Your personal Yahoo API key.  You must get this for each separate website at <a href="!yahoo_url">Yahoo Maps API website</a>.',
      array('!yahoo_url' => 'http://developer.yahoo.com/maps/')),
    '#default_value' => variable_get('openlayers_layers_yahoo_api', ''),
  );

  // Cloud Made Settings
  $form['openlayers_cloudmade'] = array(
    '#type' => 'fieldset',
    '#title' => t('CloudMade Layer Settings'),
    '#description' => t('The settings needed to get the CloudMade layer working.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['openlayers_cloudmade']['openlayers_layers_cloudmade_api'] = array(
    '#type' => 'textfield',
    '#title' => t('CloudMade API Key'),
    '#description' => t('Your personal CloudMade API key.  You must get this for each separate website at <a href="!cloudmade_url">CloudMade Developer Page</a>.',
      array('!cloudmade_url' => 'http://developers.cloudmade.com/projects')),
    '#default_value' => variable_get('openlayers_layers_cloudmade_api', ''),

  );
  $form['openlayers_cloudmade']['openlayers_layers_cloudmade_style'] = array(
    '#type' => 'textfield',
    '#title' => t('CloudMade Style ID'),
    '#description' => t('CloudMade allows you to syle your map and assigns it an ID.  Create a new style at the  <a href="!cloudmade_style_url">CloudMade Style Editor</a>.',
      array('!cloudmade_style_url' => 'http://maps.cloudmade.com/editor')),
    '#default_value' => variable_get('openlayers_layers_cloudmade_style', ''),
  );
  $form['openlayers_cloudmade']['openlayers_layers_cloudmade_js'] = array(
    '#type' => 'textfield',
    '#title' => t('CloudMade OpenLayers JS Library Path'),
    '#description' => t('Enter the Drupal path to where the CloudMade OpenLayers JS library is located.  You down load it from the <a href="!cloudmade_js_url">CloudMade OpenLayers Page</a>.',
      array('!cloudmade_js_url' => 'http://developers.cloudmade.com/wiki/openlayers-api/CloudMade_Tiles')),
    '#default_value' => variable_get('openlayers_layers_cloudmade_js', ''),
  );

  return $form;
}
