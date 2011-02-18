<?php
// $Id: openlayers.admin.inc,v 1.5.2.12 2010/02/12 00:34:51 zzolo Exp $

/**
 * @file
 * This file holds the functions for the openlayers Admin settings.
 *
 * @ingroup openlayers
 */

/**
 * Menu callback; displays the openlayers module settings page.
 *
 * @see system_settings_form()
 */
function openlayers_admin_settings() {
  // Initialize
  openlayers_initialize();

  // Let user know about advanced help
  $help = array();
  if (module_exists('advanced_help')) {
    $help['openlayers_advanced_help'] = array(
      '#type' => 'item',
      '#title' => t('OpenLayers Help') . theme('advanced_help_topic', 'openlayers', 'openlayers-about'),
      '#value' => t('Access the advanced help here: !link or click the small help icon.', array(
        '!link' => l(t('Advanced Help'), 'admin/advanced_help/openlayers'),
      )),
    );
  }
  else {
    drupal_set_message(t('This module has extensive help text available with the <a href="http://drupal.org/project/advanced_help">Advanced Help module</a>'));
  }

  // Get settings elements
  $settings = _openlayers_settings_form();
  $form = array_merge($help, $settings);

  // Make a system setting form and return
  return system_settings_form($form);
}

/**
 * Abstracted Form Elements for OpenLayers Settings
 *
 * Since the Domain module would like to know about our form elements,
 * we abstract them into this function so that we can re-use code
 * properly.
 *
 * @return
 *   From array
 */
function _openlayers_settings_form() {
  $form = array();

  // Source description
  $source_description = t('The source for the OpenLayers library can be one of two things:')
    .'<ul><li>'
    . t('URL: This means that the OpenLayers JS library is not hosted on this site.  OpenLayers provides a hosted JS file.  By default the Open Layers module will use this, since the JS file cannot be included with the module.  This is @ol_api_url.  This may effect performance as it is not hosted on your site. Also, depending on how the OpenLayers JS API changes, this module may not be compatible with this version.', array('@ol_api_url' => 'http://openlayers.org/api/OpenLayers.js'))
    .'</li><li>'
    . t('Drupal Path: This is a path relative to the Drupal base.  For instance, if you <a href="!ol_url">Download OpenLayers</a> manually to the OpenLayers module folder and extract it, you may use a value like: @suggested_path.  It is important to know that you must <strong>download the full library</strong> with all the Javascript and CSS files, since the OpenLayers library will look for the CSS and image files in that directory as well.  It is suggested you use a compressed version of the JS file as Drupal will not aggregate it.  You may also want to look at using either <a href="!build_url">OpenLayers Build Process</a> or <a href="!openlayerer_url">OpenLayerer</a> to build a custom library for better performance.',
      array(
        '!ol_url' => 'http://trac.openlayers.org/wiki/HowToDownload',
        '@suggested_path' => drupal_get_path('module', 'openlayers') . '/OpenLayers-2.8/OpenLayers.js',
        '!build_url' => 'http://docs.openlayers.org/library/deploying.html',
        '!openlayerer_url' => 'http://openlayerer.appspot.com/',
      )
    )
    .'</li></ul>';

  // Define Form
  $form['openlayers_source'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenLayers Source'),
    '#description' => $source_description,
    '#default_value' => variable_get('openlayers_source', 'http://openlayers.org/api/OpenLayers.js'),
  );
  $form['openlayers_default_preset'] = array(
    '#type' => 'select',
    '#title' => t('OpenLayers Default Preset'),
    '#description' => t('This is the default preset that will be used in cases where one is not defined.  It will also be used as the default when creating new presets.'),
    '#options' => openlayers_get_presets(),
    '#default_value' => variable_get('openlayers_default_preset', 'default'),
  );

  // Allow debug options
  $form['openlayers_debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('OpenLayers Debug Options'),
    '#description' => t('Options to allow for different debugging.  Enabled the devel module to allow for screen output.'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  // Screen output is based on dpm() function
  if (function_exists('dpm')) {
    $form['openlayers_debug']['openlayers_debug_screen_pre_render'] = array(
      '#type' => 'checkbox',
      '#title' => t('Pre-Render Screen'),
      '#description' => t('This will output each map array to the screen with dpm() before it is rendered.'),
      '#default_value' => variable_get('openlayers_debug_screen_pre_render', FALSE),
    );
    $form['openlayers_debug']['openlayers_debug_screen_post_render'] = array(
      '#type' => 'checkbox',
      '#title' => t('Post-Render Screen'),
      '#description' => t('This will output each map array to the screen with dpm() after it is rendered.'),
      '#default_value' => variable_get('openlayers_debug_screen_post_render', FALSE),
    );
  }
  $form['openlayers_debug']['openlayers_debug_watchdog_pre_render'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pre-Render Watchdog'),
    '#description' => t('This will output each map array to the watchdog log before it is rendered.'),
    '#default_value' => variable_get('openlayers_debug_watchdog_pre_render', FALSE),
  );
  $form['openlayers_debug']['openlayers_debug_watchdog_post_render'] = array(
    '#type' => 'checkbox',
    '#title' => t('Post-Render Watchdog'),
    '#description' => t('This will output each map array to the watchdog log after it is rendered.'),
    '#default_value' => variable_get('openlayers_debug_watchdog_post_render', FALSE),
  );

  return $form;
}
