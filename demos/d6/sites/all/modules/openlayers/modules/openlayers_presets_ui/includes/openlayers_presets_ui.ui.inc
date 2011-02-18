<?php
// $Id: openlayers_presets_ui.ui.inc,v 1.3.2.17 2010/05/11 18:35:14 zzolo Exp $

/**
 * @file
 * This file holds the functions for the openlayers presets ui
 *
 * @ingroup openlayers
 */

/**
 * Menu Callback for Preset List
 */
function openlayers_presets_ui_presets_manage_list() {
  $rows = array();

  // Make table of presets and actions
  $header = array(
    t('Title'),
    t('Description'),
    t('Actions'),
  );

  // Get presets
  $presets = openlayers_get_presets('full', TRUE);

  // Create table rows
  foreach ($presets as $name => $preset) {
    $row = array();
    $links = array();
    $row[] = $preset['preset_title'];
    $row[] = $preset['preset_description'];
    // Check preset type and create links accordingly.
    if ($preset['type'] == OPENLAYERS_STORAGE_OVERRIDE) {
      $links[] = l(t('Edit'), 'admin/settings/openlayers/presets/' . $preset['preset_name'] . '/edit');
      $links[] = l(t('Revert'), 'admin/settings/openlayers/presets/' . $preset['preset_name'] .'/delete');
    }
    elseif ($preset['type'] == OPENLAYERS_STORAGE_NORMAL) {
      $links[] = l(t('Edit'), 'admin/settings/openlayers/presets/' . $preset['preset_name'] . '/edit');
      $links[] = l(t('Delete'), 'admin/settings/openlayers/presets/' . $preset['preset_name'] .'/delete');
    }
    $links[] = l(t('Export'), 'admin/settings/openlayers/presets/' . $preset['preset_name'] . '/export');
    $links[] = l(t('Clone'), 'admin/settings/openlayers/presets/add/' . $preset['preset_name']);
    $row[] = implode(' | ', $links);
    $rows[] = $row;
  }

  // Output themed table
  $output = theme('table', $header, $rows);
  return $output;
}

/**
 * Menu Callback for Preset Routing
 *
 * Takes general arguments from menu and routed to
 * appropriate place.
 *
 * @param $preset_id
 *   Preset ID passed in from URL
 * @param
 *   Action to do with preset
 */
function openlayers_presets_ui_presets_manage($preset = array(), $action = 'edit') {
  $output = '';

  // Switch action
  switch ($action) {
    case 'delete':
      $output = drupal_get_form('openlayers_presets_ui_presets_manage_delete', $preset);
      break;

    case 'export':
      drupal_set_title(t('Export: %title', array('%title' => $preset['preset_title'])));
      $output = drupal_get_form('openlayers_presets_ui_presets_manage_export', $preset);
      break;

    case 'edit':
    default:
      drupal_set_title(t('Edit: %title', array('%title' => $preset['preset_title'])));
      $output = drupal_get_form('openlayers_presets_ui_presets_manage_add', $preset, TRUE);
      break;
  }

  return $output;
}

/**
 * Menu Callback for Add Preset
 */
function openlayers_presets_ui_presets_manage_add(&$form_state, $preset = array(), $edit = FALSE) {
  $form = array();
  $layers = openlayers_layers_get_info();
  $baselayers = array();
  $overlays = array();
  $available_controls = _openlayers_presets_ui_available_controls();
  $styles = openlayers_styles_get_info();
  $available_styles = array();

  // Ensure we are not editing any coded presets
  if ($preset['preset_coded'] && $edit) {
    drupal_set_message(t('Cannot edit default preset.'), 'error');
    drupal_goto('admin/settings/openlayers/presets');
  }

  // If cloning or editing, attempt to get preset
  if (empty($preset)) {
    $default_map = openlayers_get_default_map();
    // Unset preset name
    unset($default_map['preset_name']);
  }
  else {
    $default_map = $preset['preset_data'];
    // Push the preset data into the map
    $default_map['preset_title'] = $preset['preset_title'];
    $default_map['preset_description'] = $preset['preset_description'];
  }

  // Update map for new form
  if (!empty($form_state['values'])) {
    $form_map = _openlayers_presets_ui_convert_form_to_map($form_state['values']);
    $default_map = openlayers_merge_maps($default_map, $form_map);
  }

  // Change map to form values
  $defaults = _openlayers_presets_ui_convert_map_to_form($default_map);
  // Merge with form values
  $defaults = (is_array($form_state['values'])) ? $form_state['values'] + $defaults : $defaults;
  $rendered_map = (is_array($default_map)) ? openlayers_render_map($default_map) : array();

  // Get layer options
  $layer_options = array(
    'overlays' => array(),
    'baselayers' => array(),
  );

  // Determine which layer is a baselayer, and which is an overlay, and add
  // approrpriate descriptions to the options array.
  foreach ($layers as $layer_key => $layer) {
    $description = theme('openlayers_presets_ui_form_layer_description', $layer['name'], $layer['description']);
    if ($layer['baselayer']) {
      $layer_options['baselayers'][$layer_key] = $description;
    }
    else {
      $layer_options['overlays'][$layer_key] = $description;
    }
  }

  // Define list of projections
  $sorted_projections = _openlayers_presets_ui_get_projections($layers);
  $projections = array();
  foreach ($sorted_projections as $projection => $available_layers) {
    $projections[$projection] = theme('openlayers_presets_ui_form_projection_description', $projection, $available_layers, $layers);
  }
  // Set the "Other" option for our easy projection selector
  $projections['other'] = t("Other");
  // Unset All
  unset($projections['all']);

  // Default projection
  $default_proj = $defaults['projections']['projection'];

  // Check for other, which will show all layers available
  if (!$default_proj || $defaults['projections']['easy_projection'] == 'other') {
    $base_options = $layer_options['baselayers'];
    $overlay_options = $layer_options['overlays'];
  }
  else {
    // Create specific layer options for given projection
    foreach ($sorted_projections[$default_proj] as $p => $l) {
      if ($layer_options['baselayers'][$l]) {
        $base_options[$l] = $layer_options['baselayers'][$l];
      }
      if ($layer_options['overlays'][$l]) {
        $overlay_options[$l] = $layer_options['overlays'][$l];
      }
    }
  }

  // Options
  if ($defaults['options']['automatic_options']) {
    $defaults['options']['displayProjection'] = '4326';

    // Check project
    if ($default_proj == '900913' || $default_proj == '3785') {
      $defaults['options']['maxResolution'] = '156543.0339';
      $defaults['options']['maxExtent']['left'] = '-20037508.34';
      $defaults['options']['maxExtent']['right'] = '20037508.34';
      $defaults['options']['maxExtent']['bottom'] = '-20037508.34';
      $defaults['options']['maxExtent']['top'] = '20037508.34';
    }
    else {
      $defaults['options']['maxResolution'] = '';
      $defaults['options']['maxExtent']['left'] = '';
      $defaults['options']['maxExtent']['right'] = '';
      $defaults['options']['maxExtent']['bottom'] = '';
      $defaults['options']['maxExtent']['top'] = '';
    }
  }

  // Make style options array
  foreach ($styles as $k_style => $style) {
    $available_styles[$k_style] = $style['name'];
  }

  // Centering map
  $centering_map = _openlayers_presets_ui_form_center_map($defaults);

  // Form Properties
  $form['#tree'] = TRUE;
  $form['#cache'] = TRUE;

  // Pass editing along
  $form['preset_edit'] = array(
    '#type' => 'value',
    '#value' => $edit,
  );

  // Pass along map data, since it can hold stuff
  // that the UI cannot account for
  $form['default_map'] = array(
    '#type' => 'value',
    '#value' => $default_map,
  );

  // Preview of map
  $form['map_preview'] = array(
    '#type' => 'item',
    '#title' => t('Example Map'),
    '#description' => t('This is a preview of current map before saving.'),
    '#value' => $rendered_map['themed'],
  );

  // Preset properties
  $form['preset_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset Name'),
    '#description' => t('This is the machine readable identifier.  This should be all lowercase characters, numbers, or underscores (_).'),
    '#maxlength' => 255,
    '#default_value' => $defaults['preset_name'],
    '#disabled' => $edit,
  );
  // If edit, set value
  if ($edit) {
    $form['preset_name']['#value'] = $defaults['preset_name'];
  }

  $form['preset_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset Title'),
    '#description' => t('This is the descriptive title of the preset and will show up most often in the interface.'),
    '#maxlength' => 255,
    '#default_value' => $defaults['preset_title'],
  );
  $form['preset_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Preset Description'),
    '#description' => t('This is full description of the preset and is mostly used on the preset overview list page.'),
    '#rows' => 2,
    '#default_value' => $defaults['preset_description'],
  );

  // Map general properties
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Please use a CSS Width value.'),
    '#default_value' => $defaults['width'],
    '#maxlength' => 128,
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Please use a CSS Height value.'),
    '#default_value' => $defaults['height'],
    '#maxlength' => 128,
  );
  $form['image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Path'),
    '#description' => t('The path to a directory for OpenLayers to look for UI graphics. If blank, default graphics are used.'),
    '#default_value' => $defaults['image_path'],
  );

  // Map center properties
  $form['center'] = array(
    '#type' => 'fieldset',
    '#title' => t('Center'),
    '#description' => t('Where the map will center itself initially. You may use the small map to help you set your center and zoom.  Pan and zoom where you want the centering defaults to be.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['center']['helpmap'] = array(
    '#value' => '<div class="form-item openlayers-center-helpmap" style="display:block">'. $centering_map['themed'] .'</div>'
  );
  $form['center']['lat'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#description' => t('Latitude or X value for centering.'),
    '#default_value' => $defaults['center']['lat'],
    '#attributes' => array('class' => 'openlayers-form-lat'),
    '#size' => 25,
  );
  $form['center']['lon'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#description' => t('Longitude or Y value for centering.'),
    '#default_value' => $defaults['center']['lon'],
    '#attributes' => array('class' => 'openlayers-form-lon'),
    '#size' => 25,
  );
  $form['center']['zoom'] = array(
    '#type' => 'textfield',
    '#title' => t('Zoom Level'),
    '#description' => t('Zoom level for centering.'),
    '#default_value' => $defaults['center']['zoom'],
    '#attributes' => array('class' => 'openlayers-form-zoom'),
    '#size' => 25,
  );

  // Map controls
  $form['controls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls'),
    '#description' => t('Controls on the map that allow the user to interface with the map.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Go through avilable controls
  foreach ($available_controls as $c_key => $control) {
    $form['controls'][$c_key] = array(
      '#type' => 'checkbox',
      '#title' => $control['title'],
      '#description' => $control['description'],
      '#default_value' => $defaults['controls'][$c_key],
    );
  }

  // Vector Styles
  $form['styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vector Styles'),
    '#description' => t('Styles used on features in vector layers.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['styles']['default'] = array(
    '#type' => 'select',
    '#title' => t('Default Style'),
    '#description' => t('Default style for features in a vector.'),
    '#options' => $available_styles,
    '#default_value' => $defaults['styles']['default'],
  );
  $form['styles']['select'] = array(
    '#type' => 'select',
    '#title' => t('Select Style'),
    '#description' => t('Default style for features in a vector that are selected.'),
    '#options' => $available_styles,
    '#default_value' => $defaults['styles']['select'],
  );
  $form['styles']['temporary'] = array(
    '#type' => 'select',
    '#title' => t('Temporary Style'),
    '#description' => t('Default style for any temporary features in a vector.  This will also be used for rollovers for things like Tooltips.'),
    '#options' => $available_styles,
    '#default_value' => $defaults['styles']['temporary'],
  );

  // Projections
  $form['projections'] = array(
    '#type' => 'fieldset',
    '#title' => t('Projection'),
    '#description' => t('Select projection for map.  This will affect which layers are available to choose from.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // This will create an easy way to select projections and have layers change accordingly
  $form['projections']['easy_projection'] = array(
    '#type' => 'radios',
    '#title' => t('Projection'),
    '#description' => t('Select the EPSG code of the !link_proj for your map. The list next to each projection is the layers that support this projection.', array(
      '!link_proj' => l(t('geographical projection'), 'http://en.wikipedia.org/wiki/Map_projection'),
    )),
    '#default_value' => $defaults['projections']['easy_projection'],
    '#options' => $projections,
    '#attributes' => array('class' => 'openlayers-form-easy-projection'),
  );
  $form['projections']['projection'] = array(
    '#type' => 'textfield',
    '#title' => t('Projection'),
    '#description' => t('The EPSG code of the geographical projection for your map.'),
    '#attributes' => array('class' => 'openlayers-form-projection'),
    '#default_value' => $defaults['projections']['projection'],
    '#maxlength' => 16,
    '#ahah' => array(
      'path' => 'openlayers/ahah/preset',
      'wrapper' => 'openlayers-layers-select',
      'event' => 'change',
    ),
  );

  // Start AHAH Wrapper
  $form['openlayers-ahah-wrapper-start'] = array(
    '#value' => '<div id="openlayers-layers-select">',
  );

  // Layers
  $form['layers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layers'),
    '#description' => t('Layer settings.  The Layer options will change based on the projection chosen above.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['layers']['baselayers'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Base Layers'),
    '#description' => t('Select the base layers to make available for your map. This list is determined by the projection you select for your map.'),
    '#options' => $base_options,
    '#attributes' => array('class' => 'openlayers-form-baselayers'),
    '#multiple' => TRUE,
    '#default_value' => $defaults['layers']['baselayers'] ? $defaults['layers']['baselayers'] : array(),
  );
  $form['layers']['default_layer'] = array(
    '#type' => 'radios',
    '#title' => t('Default Base Layer'),
    '#description' => t('The default base layer to use when rendering maps.  This will be included whether it is in the base layers or not.'),
    '#options' => $base_options,
    '#attributes' => array('class' => 'openlayers-form-default-layer'),
    '#default_value' => $defaults['layers']['default_layer'],
  );
  $form['layers']['overlays'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Overlay Layers'),
    '#description' => t('Select the overlay layers to make available for your map.'),
    '#options' => $overlay_options,
    '#attributes' => array('class' => 'openlayers-form-overlays'),
    '#multiple' => TRUE,
    '#default_value' => $defaults['layers']['overlays'] ? $defaults['layers']['overlays'] : array(),
  );

  // Map options properties
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#description' => t('Set additional options'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['options']['automatic_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic Options'),
    '#description' => t('If this is checked, options will be automatically filled for you.'),
    '#default_value' => isset($defaults['options']['automatic_options']) ?$defaults['options']['automatic_options'] : TRUE,
    '#maxlength' => 6
  );
  $form['options']['displayProjection'] = array(
    '#type' => 'textfield',
    '#title' => t('Display Projection'),
    '#description' => t('This is the projection that is presented to your users as the interface for viewing / editing location data. You will most likely want use 4326 for lat/lon coordinates.'),
    '#default_value' => $defaults['options']['displayProjection'],
    '#maxlength' => 6
  );
  $form['options']['maxResolution'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Resolution'),
    '#description' => t('The maximum number of pixels / map unit at the highest zoom.'),
    '#default_value' => $defaults['options']['maxResolution'],
    '#attributes' => array('class' => 'openlayers-form-maxResolution'),
  );
  $form['options']['maxExtent'] = array(
    '#type' => 'fieldset',
    '#title' => t('Maximum Extent'),
    '#description' => t('Set the bounds for the edges of your map. Set them according to the units of your projection.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['options']['maxExtent']['left'] = array(
    '#type' => 'textfield',
    '#title' => t('Left'),
    '#default_value' => $defaults['options']['maxExtent']['left'],
    '#attributes' => array('class' => 'openlayers-form-maxExtent-left'),
  );
  $form['options']['maxExtent']['right'] = array(
    '#type' => 'textfield',
    '#title' => t('Right'),
    '#default_value' => $defaults['options']['maxExtent']['right'],
    '#attributes' => array('class' => 'openlayers-form-maxExtent-right'),
  );
  $form['options']['maxExtent']['bottom'] = array(
    '#type' => 'textfield',
    '#title' => t('Bottom'),
    '#default_value' => $defaults['options']['maxExtent']['bottom'],
    '#attributes' => array('class' => 'openlayers-form-maxExtent-bottom'),
  );
  $form['options']['maxExtent']['top'] = array(
    '#type' => 'textfield',
    '#title' => t('Top'),
    '#default_value' => $defaults['options']['maxExtent']['top'],
    '#attributes' => array('class' => 'openlayers-form-maxExtent-top'),
  );

  // End AHAH Wrapper
  $form['openlayers-ahah-wrapper-end'] = array(
    '#value' => '</div>',
  );

  // Add a submit button that is the submit handler for the
  // AHAH change event on projections
  $form['openlayers_projection_ahah'] = array(
    '#type' => 'submit',
    '#value' => t('Update Projections'),
    '#submit' => array('openlayers_presets_manage_add_projection_submit'),
  );

  // Save button
  $form['openlayers_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  // Cancel button
  $form['openlayers_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel')
  );

  // Add JavaScript
  drupal_add_js(drupal_get_path('module', 'openlayers_presets_ui') .'/js/openlayers_presets_ui.ui.js', 'module');
  // Add CSS
  drupal_add_css(drupal_get_path('module', 'openlayers_presets_ui') .'/openlayers_presets_ui.css');

  return $form;
}

/**
 * OpenLayers Preset Form Validate
 *
 * Validates a preset form submission.
 *
 * @param $map_form
 *   Array of values to validate
 * @return
 *   Does not return anything. Uses form_set_error() to communicate errors.
 */
function openlayers_presets_ui_presets_manage_add_validate($form, &$form_state) {
  $values = $form_state['values'];
  $found_error = FALSE;

  // Check if ahah submitting
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-projection-ahah') {
    return TRUE;
  }

  // Check for cancel
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-cancel') {
    return TRUE;
  }

  // Check for values.  We manually do required fields because, it would otherwise
  // mess with the AHAH stuff.  Maybe a way around it.
  if (empty($values['preset_name'])) {
    form_set_error('preset_name', t('Preset Name is required.'));
    $found_error = TRUE;
  }
  if (empty($values['preset_title'])) {
    form_set_error('preset_title', t('Preset Title is required.'));
    $found_error = TRUE;
  }
  if (empty($values['preset_description'])) {
    form_set_error('preset_description', t('Preset Description is required.'));
    $found_error = TRUE;
  }
  if (empty($values['width'])) {
    form_set_error('width', t('Width is required.'));
    $found_error = TRUE;
  }
  if (empty($values['height'])) {
    form_set_error('height', t('Height is required.'));
    $found_error = TRUE;
  }

  // Check preset name first
  if (!preg_match('!^[a-z0-9_]+$!', $values['preset_name'])) {
    form_set_error('preset_name', t('Preset Name must contain only lowercase letters, numbers, and underscores.'));
    $found_error = TRUE;
  }

  // Check if adding and name already exists
  $existing = openlayers_get_preset($values['preset_name'], TRUE);
  if (!empty($existing) && (!$form_state['values']['preset_edit']) && $existing['type'] != OPENLAYERS_STORAGE_DEFAULT) {
    form_set_error('preset_name', t('The Preset Name already exists.'));
    $found_error = TRUE;
  }

  // Convert form to map array
  $map = _openlayers_presets_ui_convert_form_to_map($form_state['values']);

  // Attempt to render map to find any errors
  $map = openlayers_render_map($map, FALSE);

  // Check if any errors found
  if (is_array($map['errors']) && count($map['errors']) > 0) {
    foreach ($map['errors'] as $error) {
      form_set_error('openlayers', t('OpenLayers Map Rendering Error: !error', array('!error' => $error)));
      $found_error = TRUE;
    }
  }

  // If found error, rebuild form
  if ($found_error) {
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Form submit for preset add form, for the projection add ahah
 */
function openlayers_presets_ui_presets_manage_add_projection_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['rebuild'] = TRUE;
}

/**
 * Form submit for preset add form
 */
function openlayers_presets_ui_presets_manage_add_submit($form, &$form_state) {
  // Check for cancel
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-cancel') {
    $form_state['redirect'] = 'admin/settings/openlayers/presets/list';
  }

  // Only save if save button is pressed
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-save') {
    // Merge with original map data to account for data
    // that cannot be accounted for UI
    if (!empty($form_state['values']['default_map'])) {
      // Convert form, but keep empty values, so merging is subtractive
      $map = _openlayers_presets_ui_convert_form_to_map($form_state['values'], FALSE);
      $map = _openlayers_presets_ui_merge_maps($map, $form_state['values']['default_map']);
    }
    else {
      $map = _openlayers_presets_ui_convert_form_to_map($form_state['values']);
    }

    // Create preset array
    $preset = array(
      'preset_name' => $form_state['values']['preset_name'],
      'preset_title' => $form_state['values']['preset_title'],
      'preset_description' => $form_state['values']['preset_description'],
      'preset_data' => $map,
    );

    // Save preset
    $success = openlayers_save_preset($preset);
    // Redirect to edit page
    if ($success) {
      drupal_set_message(t('Map saved.'));
      $form_state['redirect'] = 'admin/settings/openlayers/presets/' . $preset['preset_name'] . '/edit';
    }
    else {
      form_set_error('openlayers', t('Error trying to save map'));
    }
  }
}

/**
 * Menu Callback for Preset Delete Form
 */
function openlayers_presets_ui_presets_manage_delete(&$form_state, $preset = array()) {
  // Ensure we are not deleting any coded presets
  if ($preset['type'] == OPENLAYERS_STORAGE_DEFAULT) {
    drupal_set_message(t('Cannot delete default preset.'), 'error');
    drupal_goto('admin/settings/openlayers/presets');
  }

  // Create delete form
  $form = array();
  $form['preset_name'] = array('#type' => 'value', '#value' => $preset['preset_name']);
  if ($preset['type'] == OPENLAYERS_STORAGE_OVERRIDE) {
    return confirm_form(
      $form,
      t('Are you sure you want to revert the preset %preset?',
        array('%preset' => $preset['preset_title'])
      ),
      'admin/settings/openlayers/presets',
      t('This will delete the preset in the database and revert to the preset in a feature or code. This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );

  }
  else {
    return confirm_form(
      $form,
      t('Are you sure you want to delete the preset %preset?',
        array('%preset' => $preset['preset_title'])
      ),
      'admin/settings/openlayers/presets',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
}

/**
 * Form Submit Callback for Preset Delete Form
 */
function openlayers_presets_ui_presets_manage_delete_submit($form, &$form_state) {
  $preset = openlayers_get_preset($form_state['values']['preset_name'], TRUE);

  // Check if coded
  if ($preset['type'] == OPENLAYERS_STORAGE_DEFAULT) {
    drupal_set_message(t('Cannot delete %name because it is provided by a module.', array('%name' => $preset['preset_name'])));
  }
  else {
    // Create query
    $query = "DELETE FROM {openlayers_map_presets} WHERE preset_id = %d";
    $result = db_query($query, $preset['preset_id']);

    // Check query results
    if ($result) {
      drupal_set_message(t('Preset %name was deleted.', array('%name' => $preset['preset_name'])));
    }
    else {
      drupal_set_message(t('Error when attempting to delete preset.'), 'error');
    }
  }

  // Redirect
  $form_state['redirect'] = 'admin/settings/openlayers/presets';
}

/**
 * Menu callback for Preset Export Form
 */
function openlayers_presets_ui_presets_manage_export(&$form_state, $preset = array()) {
  // Create output
  $exported = '$map_preset = array();';
  $exported .= "\n";
  $exported .= '$map_preset[\''. $preset['preset_name'] .'\'] = ';
  $exported .= var_export($preset, TRUE) .';';
  $exported .= "\n";
  $exported .= 'return $map_preset;';
  $rows = substr_count($exported, "\n") + 1;

  // Create form
  $form = array();
  $form['export'] = array(
    '#type' => 'textarea',
    '#default_value' => $exported,
    '#rows' => $rows,
    '#resizable' => FALSE,
  );
  $form['done'] = array(
    '#type' => 'submit',
    '#value' => t('Done'),
  );
  $form['#redirect'] = 'admin/settings/openlayers/presets';

  return $form;
}

/**
 * Menu callback for Preset Import Form
 */
function openlayers_presets_ui_presets_import(&$form_state) {
  // Create form
  $form = array();

  $form['warning'] = array(
    '#value' => '<strong>NOT WORKING</strong>',
  );
  // Put in name field if needed
  if ($form_state['needs_name']) {
    $form['new_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Preset Name'),
      '#description' => t('Identifier of new preset if one is not provided or already exists.'),
      '#default_value' => $defaults['new_name'],
      '#maxlength' => 255,
      '#required' => TRUE,
    );
  }
  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Import Code'),
    '#description' => t('Input the code that was produced from the Export function.  If you put in bad code, this could break your site, please be careful.'),
    '#default_value' => $form_state['import'], // Not consisten where value is
    '#rows' => 20,
    '#required' => TRUE,
    '#disabled' => TRUE,
  );
  $form['openlayers_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  $form['openlayers_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel')
  );

  return $form;
}

/**
 * Form Validate Callback for Preset Import Form
 */
function openlayers_presets_ui_presets_import_validate($form, &$form_state) {
  $values = $form_state['values'];

  // If Cancel redirect
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-cancel') {
    return TRUE;
  }

  // Get data
  $presets = eval($values['import']);
  if (!is_array($presets)) {
    form_set_error('import', t('Format is bad'));
  }
  else {
    // Go through each preset
    foreach ($presets as $k => $preset) {
      // Check various things
      if (!is_array($preset)) {
        form_set_error('import', t('Format is bad for key: %key', array('%k' => $k)));
      }
      if (empty($preset['preset_title'])) {
        form_set_error('import', t('No Preset Title Found for key: %key', array('%k' => $k)));
      }
      if (empty($preset['preset_description'])) {
        form_set_error('import', t('No Preset Description Found for key: %key', array('%k' => $k)));
      }
      if (empty($preset['preset_data'])) {
        form_set_error('import', t('No Map Data Found for key: %key', array('%k' => $k)));
      }
      if (empty($preset['preset_data']['width'])) {
        form_set_error('import', t('No Map Width Found for key: %key', array('%k' => $k)));
      }
      if (empty($preset['preset_data']['height'])) {
        form_set_error('import', t('No Map Height Found for key: %key', array('%k' => $k)));
      }
    }
  }
}

/**
 * Form Submit Callback for Preset Import Form
 */
function openlayers_presets_ui_presets_import_submit($form, &$form_state) {
  $values = $form_state['values'];

  // If Cancel redirect
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-cancel') {
    $form_state['redirect'] = 'admin/settings/openlayers/presets/list';
  }
  else {
    // Get data
    $preset = eval($values['import']);
  }
}

/**
 * OpenLayers AHAH
 *
 * Function to handle the AHAH request of the openlayers form
 */
function openlayers_presets_ui_preset_ahah() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);

  // Get variables
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;

  // Reprocess form with new form state
  drupal_process_form($form_id, $form, $form_state);
  // Rebuild form and remove any submot handlers
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  // Unset wrapper and create output
  unset($form['openlayers-ahah-wrapper-start'], $form['openlayers-ahah-wrapper-end']);
  $output = theme('status_messages') . drupal_render($form['layers']) . drupal_render($form['options']);

  // Final rendering callback.
  drupal_json(array('status' => TRUE, 'data' => $output));
}

/**
 * OpenLayers Form to Map
 *
 * Converst Form submission to map array.
 *
 * @param $values
 *   Array of values to convert
 * @param $clear_empty
 *   Boolean of whether empty values should be cleared
 * @return
 *   Array of form items
 */
function _openlayers_presets_ui_convert_form_to_map($values = array(), $clear_empty = TRUE) {
  // If the form values are empty then we have nothing to return.
  if (empty($values)) {
    return array();
  }
  $processed = array();

  // Put projection in right place.
  $processed['projection'] = ($values['projections']['easy_projection'] != 'other') ? $values['projections']['easy_projection'] : $values['projections']['projection'];

  // Put the default layer in the right place.
  $processed['default_layer'] = $values['layers']['default_layer'];

  // Merge our different layer sections together
  $baselayers = array_filter($values['layers']['baselayers']);
  $overlays = array_filter($values['layers']['overlays']);
  $processed['layers'] = array_merge($baselayers, $overlays);

  // Take out form values that are no good
  $throw = array('op', 'submit', 'form_build_id', 'form_token', 'form_id', 'projections', 'layers', 'openlayers_save', 'preset_name', 'preset_title', 'preset_description', 'map_preview', 'openlayers_projection_ahah', 'openlayers_cancel', 'default_map');
  foreach ($values as $k => $v) {
    if (!in_array($k, $throw)) {
      $processed[$k] = $v;
    }
  }

  // Recursively unset any empty values
  if ($clear_empty) {
    $processed = _openlayers_presets_ui_unset_empty_values($processed);
  }
  return $processed;
}

/**
 * OpenLayers Map to Form
 *
 * Converts Map array to form defaults array.
 *
 * @param $map
 *   Array of map values to convert
 * @return
 *   Array of default form values
 */
function _openlayers_presets_ui_convert_map_to_form($map = array()) {
  $processed = array();
  // Check input
  if (!is_array($map) || empty($map)) {
    return $processed;
  }
  $processed = $map;
  $layer_defs = openlayers_layers_get_info();

  // Manual changes
  $processed['layers'] = array();
  $processed['layers']['default_layer'] = $map['default_layer'];
  $processed['layers']['baselayers'] = array();
  $processed['layers']['overlays'] = array();

  // Run Image path through url()
  if ($processed['image_path']) {
    $processed['image_path'] = check_url($processed['image_path']);
  }

  // Set projection
  $processed['projections']['easy_projection'] = $processed['projection'];
  $processed['projections']['projection'] = $processed['projection'];

  // Get layers
  if (is_array($map['layers'])) {
    foreach ($map['layers'] as $layer_id => $layer) {
      if ($layer_defs[$layer_id]['baselayer']) {
        $processed['layers']['baselayers'][] = $layer_id;
      }
      else {
        $processed['layers']['overlays'][] = $layer_id;
      }
    }
  }

  // Return processed
  return $processed;
}

/**
 * Sort Projections
 *
 * Given layers, sort projections out
 *
 * @param $layers
 *   Array of layers
 * @return
 *   Return sorted array
 */
function _openlayers_presets_ui_get_projections($layers = array()) {
  $all_projections = array();
  $projection_layers = array();

  // Check $layers
  if (!is_array($layers)) {
    return $projection_layers;
  }

  // Go through layers
  foreach ($layers as $layer_key => $layer) {
    // Check projections
    if ($layer['projection']) {
      // Go through projections
      foreach ($layer['projection'] as $projection) {
        if (!array_key_exists($projection, $projection_layers)) {
          $projection_layers[$projection] = array();
        }
        $projection_layers[$projection][] = $layer_key;
      }
    }
  }

  // Add any layers that do not have a projection specified to all
  foreach ($layers as $layer_key => $layer) {
    // Check projections
    if (!$layer['projection']) {
      // Go through projections
      foreach ($projection_layers as $k => $v) {
        $projection_layers[$k][] = $layer_key;
      }
    }
  }

  // Return array
  return $projection_layers;
}

/**
 * Recursively unset empty values
 *
 * Go through an array recursively and unset empty strings and arrays
 *
 * @param $values
 *   Array
 * @return
 *   Array with empty values unset
 */
function _openlayers_presets_ui_unset_empty_values($array) {
  foreach ($array as $key => $value) {
    // If it is an array then recursively check it
    if (is_array($value)) {
      $array[$key] = _openlayers_presets_ui_unset_empty_values($value);
    }
    // If it is an array then check if it is empty. We don't use $value so that if it
    // is emptied by the previous check then it will still unset.
    if (is_array($array[$key])) {
      if (empty($array[$key])) {
        unset($array[$key]);
      }
    }

    // If it is an empty string then unset it
    if ($value == "") {
      unset($array[$key]);
    }
  }
  return $array;
}

/**
 * Create Centering Map
 *
 * Create map for interactive default centering
 *
 * @param $defaults
 *   Array of defults to use for the map of centering and zooming
 * @return
 *   Themed map array
 */
function _openlayers_presets_ui_form_center_map($defaults = array()) {
  // Set up our map to help set center lat, lon and zoom
  // This map will always be projected as 4326 and use just the basic layer so that
  // even if the user is using screwed up map settings, this map will still function.
  $centermap_def = array(
    'id' => 'openlayers-center-helpmap',
    'projection' => '4326',
    'default_layer' => 'openlayers_default_wms',
    'width' => '400px',
    'height' => '300px',
    'center' => array(
      'lat' => ($defaults['center']['lat']) ? $defaults['center']['lat'] : 0,
      'lon' => ($defaults['center']['lon']) ? $defaults['center']['lon'] : 0,
      'zoom' => ($defaults['center']['zoom']) ? $defaults['center']['zoom'] : 2,
    ),
    'layers' => array(
      'openlayers_default_wms',
    ),
    'controls' => array(
      'LayerSwitcher' => TRUE,
      'Navigation' => TRUE,
      'PanZoomBar' => TRUE,
      'MousePosition' => TRUE,
    ),
    'options' => array(
      'displayProjection' => $defaults['projection'],
      /* This seems to break the centering map.  Not sure why we are
         defining this anyway.
      'maxResolution' => '1.40625',
      'maxExtent' => array(
        'top' => 90,
        'bottom' => -90,
        'left' => -180,
        'right' => 180
      ),
      */
    ),
    'events' => array(
      'moveend' => array('updateCenterFormValues'),
      'zoomend' => array('updateCenterFormValues'),
    ),
  );

  // Pass variables etc. to javascript
  $pass_values = array(
    'openlayersForm' => array(
      'projectionLayers' => _openlayers_presets_ui_get_projections($layers),
    ),
  );
  drupal_add_js($pass_values, 'setting');
  // Render map
  return openlayers_render_map($centermap_def);
}

/**
 * Get Available Controls
 *
 * Builds an array of available controls for the form.
 *
 * @return
 *   Array of available controls
 */
function _openlayers_presets_ui_available_controls() {
  $available = array();

  // Define array
  $available['Attribution'] = array(
    'title' => t('Attribution'),
    'description' => t('Allows layers to provide attribution to the map if it exists.'),
  );
  $available['KeyboardDefaults'] = array(
    'title' => t('Keyboard Defaults'),
    'description' => t('Provides the ability to use the keyboard to pan and zoom map.'),
  );
  $available['LayerSwitcher'] = array(
    'title' => t('Layer Switcher'),
    'description' => t('Provides the ability to switch Layers in the map interface.'),
  );
  $available['MousePosition'] = array(
    'title' => t('Mouse Position'),
    'description' => t('Provides a visual indication of the mouse position to the user.'),
  );
  $available['Navigation'] = array(
    'title' => t('Navigation'),
    'description' => t('Provides the ability to navigate the map interface.'),
  );
  $available['PanZoomBar'] = array(
    'title' => t('Pan Zoom Bar'),
    'description' => t('Provides the ability to pan and zoom in the map interface. The Navigation control needs to be enabled for this to function to work properly.'),
  );
  $available['PanZoom'] = array(
    'title' => t('Pan Zoom'),
    'description' => t('Provides a smaller version of the Pan Zoom Bar.  The Navigation control needs to be enabled for this to function.'),
  );
  $available['Permalink'] = array(
    'title' => t('Permanent Link'),
    'description' => t('Provides a URL that will link to a specific map position.'),
  );
  $available['ScaleLine'] = array(
    'title' => t('Scale Line'),
    'description' => t('Provides the user with a line of scale in the map interface.'),
  );
  $available['ZoomBox'] = array(
    'title' => t('Zoom Box'),
    'description' => t('Provides the ability for the user to draw a box on screen to zoom with Shift + Click.'),
  );
  $available['ZoomToMaxExtent'] = array(
    'title' => t('Zoom to Max Extent'),
    'description' => t('Provides a button to zoom to the maximum extent of the map.'),
  );

  return $available;
}

/**
 * Merge maps from form and original preset
 *
 * Takes data from form and data from original map and merges
 * them together, taking into account any manual changes
 *
 * @param $submitted_map
 *   Map array from form that has been convertted
 * @param $orig_map
 *   Map array form the original map
 * @return
 *   Merged map array
 */
function _openlayers_presets_ui_merge_maps($submitted_map = array(), $orig_map = array()) {
  $merged_map = array();

  // Check map arrays
  if (!is_array($submitted_map) || empty($submitted_map) || !is_array($orig_map) || empty($orig_map)) {
    return (is_array($submitted_map)) ? $submitted_map : array();
  }

  // Unset values in original map that can be altered by preset form
  unset($orig_map['preset_name']);
  unset($orig_map['preset_title']);
  unset($orig_map['preset_description']);

  // Any layer that is referenced by a string is an exposed / permanent layer and is visible to the UI.
  // We should therefore remove it from our $orig_map display since we will prefer selections from the UI.
  // Any layer that is an array will not have been exposed to the UI and therefore should be kept.
  if ($orig_map['layers']) {
    foreach ($orig_map['layers'] as $key => $layer) {
      if (is_string($layer)) {
        unset($orig_map['layers'][$key]);
      }
    }
  }

  // Merge maps.  The passed in submitted map has not had empty values removed.
  // This ensures that there is a subtractive merge
  $merged_map = openlayers_merge_maps($orig_map, $submitted_map);
  $merged_map = _openlayers_presets_ui_unset_empty_values($merged_map);
  return $merged_map;
}
