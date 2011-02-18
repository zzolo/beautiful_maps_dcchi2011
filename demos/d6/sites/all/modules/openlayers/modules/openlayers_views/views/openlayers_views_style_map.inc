<?php
// $Id: openlayers_views_style_map.inc,v 1.16.2.32 2010/04/28 09:38:19 zzolo Exp $

/**
 * @file
 * This file holds style plugin for OpenLayers Views
 *
 * @ingroup openlayers
 */

/**
 * @class
 * Extension of the Views Plugin Syle for OpenLayers
 *
 * This class extended the default views plugin class to provide
 * a style plugin for the Open Layers module.
 */
class openlayers_views_style_map extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    // Get parent options
    $options = parent::option_definition();

    return $options;
  }

  /**
   * Options form
   */
  function options_form(&$form, &$form_state) {
    $fields = array();
    $geo_field_options = array();
    $location_field_options = array();
    $data_source_options = array();

    // Get list of fields in this view & flag available geodata fields
    $handlers = $this->display->handler->get_handlers('field');

    // Check for any fields, as the view needs them
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need to enable at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );

      parent::options_form($form, $form_state);
      return;
    }

    // Go through fields
    foreach ($handlers as $field_id => $handler) {
      // Collect fields per types
      switch ($handler->content_field['type']) {
        case 'geo':
          $geo_field_options[$handler->content_field['field_name']] = $handler->content_field['widget']['label'];
          break;

        case 'location':
          $location_field_options[$handler->content_field['field_name']] = $handler->content_field['widget']['label'];
          break;

        default:
          $fields[$field_id] = $handler->label() ? $handler->label() : $handler->ui_name();
          break;

      }
    }

    // Check Geo fields found
    if (count($geo_field_options) > 0) {
      $data_source_options['geo'] = t('Geo Module');
    }
    // Check Node Location found
    if (module_exists('location_node')) {
      $data_source_options['node_locations'] = t('Node Locations');
    }
    // Default data source
    $data_source_options['other_latlon'] = t('Other: Lat/Lon Point');
    $data_source_options['other_wkt'] = t('Other: Well Known Text');

    // Data Source options
    $form['data_source'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Data Source'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['data_source']['value'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Map Data Sources'),
      '#description' => t('Choose which sources of data that the map will provide features for.'),
      '#options' => $data_source_options,
      '#default_value' => $this->options['data_source']['value'],
    );
    // Geo Data source options
    if ($data_source_options['geo']) {
      $form['data_source']['geo_fields'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('Geo Module Fields'),
        '#description' => t('Choose the fields provide by the Geo Module that will be used for feature data.'),
        '#options' => $geo_field_options,
        '#default_value' => $this->options['data_source']['geo_fields'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('geo')),
      );
    }
    // Location data source options
    if ($data_source_options['location']) {
      $form['data_source']['location_fields'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('Location Fields'),
        '#description' => t('Choose the fields provided by the Location module that will be used for feature data.'),
        '#options' => $location_field_options,
        '#default_value' => $this->options['data_source']['location_fields'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('location')),
      );
    }
    // Other Lat and Lon data sources
    if (count($fields > 0)) {
      $form['data_source']['other_lat'] = array(
        '#type' => 'select',
        '#title' => t('Latitude Field'),
        '#description' => t('Choose a field for Latitude.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_lat'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_latlon')),
      );
      $form['data_source']['other_lon'] = array(
        '#type' => 'select',
        '#title' => t('Longitude Field'),
        '#description' => t('Choose a field for Longitude.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_lon'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_latlon')),
      );
    }
    // Other Well Known Text
    if (count($fields > 0)) {
      $form['data_source']['other_wkt'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('WKT Field'),
        '#description' => t('Choose the fields for Well Known Text data.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_wkt'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_wkt')),
      );
    }

    // Map Preset
    $form['presets'] = array(
      '#type' => 'fieldset',
      '#title' => t('Map Presets'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['presets']['map_preset'] = array(
      '#type' => 'select',
      '#title' => t('Map Preset'),
      '#description' => t('This is map that will be used to render the view.'),
      '#options' => openlayers_get_presets(),
      '#default_value' => $this->options['presets']['map_preset'] ? $this->options['presets']['map_preset'] : variable_get('openlayers_default_preset', 'default'),
    );
    $form['presets']['not_display_empty_map'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do Not Display Empty Map'),
      '#description' => t('Check this is you do not want a map to show if there is no data available in the view.'),
      '#default_value' => $this->options['presets']['not_display_empty_map'],
    );

    // Behavior options
    if (module_exists('openlayers_behaviors')) {
      $form['behaviors'] = array(
        '#type' => 'fieldset',
        '#title' => t('Map Behaviors Options'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      // Tooltip and Pop-up options if fields
      if (count($fields) > 0) {
        $tooltip_popup_options = array_merge(array('0' => t('<none>'), '#all_fields' => t('<all> (render entire row)')), $fields);
        $form['behaviors']['tooltip'] = array(
          '#type' => 'select',
          '#title' => t('Tooltip Field'),
          '#title' => t('Select the field you would like to use as a tooltip'),
          '#options' => $tooltip_popup_options,
          '#default_value' => $this->options['behaviors']['tooltip'],
        );
        $form['behaviors']['popup'] = array(
          '#type' => 'select',
          '#title' => t('Pop-up Field'),
          '#title' => t('Select the field you would like to use as a popup'),
          '#options' => $tooltip_popup_options,
          '#default_value' => $this->options['behaviors']['popup'],
        );
      }
      $form['behaviors']['zoom_to_layer'] = array(
        '#type' => 'checkbox',
        '#title' => t('Zoom to Features'),
        '#description' => t('Automatically zoom to encompass views data (overrides map settings)'),
        '#default_value' => $this->options['behaviors']['zoom_to_layer'],
      );
      $form['behaviors']['fullscreen'] = array(
        '#type' => 'checkbox',
        '#title' => t('Fullscreen'),
        '#description' => t('Allow map to be viewed fullscreen'),
        '#default_value' => $this->options['behaviors']['fullscreen'],
      );
      $form['behaviors']['declutter'] = array(
        '#type' => 'checkbox',
        '#title' => t('Declutter'),
        '#description' => t('Automagically space points so they do not overlap.'),
        '#default_value' => $this->options['behaviors']['declutter'],
      );
      $form['behaviors']['declutter_adjustment'] = array(
        '#type' => 'textfield',
        '#title' => t('Declutter Distance Adjustment'),
        '#description' => t('Set to a negative number to have icons cluster closer together. Set to a positive number to have them furthur apart. Leave blank for default.'),
        '#default_value' => $this->options['behaviors']['declutter_adjustment'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-behaviors-declutter' => array('1')),
      );
      $form['behaviors']['cluster'] = array(
        '#type' => 'checkbox',
        '#title' => t('Cluster'),
        '#description' => t('Cluster nodes that are close together into a single node.'),
        '#default_value' => $this->options['behaviors']['cluster'],
      );
      $form['behaviors']['cluster_distance'] = array(
        '#type' => 'textfield',
        '#title' => t('Cluster distance'),
        '#description' => t('Pixel distance between features that should be considered a single cluster.  Default is 20 pixels.'),
        '#default_value' => $this->options['behaviors']['cluster_distance'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-behaviors-cluster' => array('1')),
      );
      $form['behaviors']['cluster_threshold'] = array(
        '#type' => 'textfield',
        '#title' => t('Cluster threshold'),
        '#description' => t('Optional threshold below which original features will be added to the layer instead of clusters.  For example, a threshold of 3 would mean that any time there are 2 or fewer features in a cluster, those features will be added directly to the layer instead of a cluster representing those features.  Default is 2 (meaning that clusters must contain at least two features).'),
        '#default_value' => $this->options['behaviors']['cluster_threshold'] ? $this->options['behaviors']['cluster_threshold'] : 2,
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-behaviors-cluster' => array('1')),
      );
      $form['behaviors']['cluster_popup'] = array(
        '#type' => 'checkbox',
        '#title' => t('Popups on clusters'),
        '#description' => t('Should clustered features permit popups?'),
        '#default_value' => $this->options['behaviors']['cluster_popup'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-behaviors-cluster' => array('1')),
      );
      $form['behaviors']['cluster_popup_callback'] = array(
        '#type' => 'textfield',
        '#title' => t('Callback for popup contents when clustering'),
        '#description' => t('Enter the name of a JavaScript callback that returns the contents of popups (a string) attached to clustered features.  The callback takes one argument: \'feature\'.  <em>feature.cluster</em> contains the features being clustered together.'),
        '#default_value' => $this->options['behaviors']['cluster_popup_callback'],
        '#process' => array('views_process_dependency'),
        '#dependency_count' => 2,
        '#dependency' => array('edit-style-options-behaviors-cluster' => array('1'), 'edit-style-options-behaviors-cluster-popup' => array('1')),
      );
      $form['behaviors']['cluster_tooltip'] = array(
        '#type' => 'checkbox',
        '#title' => t('Tooltips on clusters'),
        '#description' => t('Should clustered features permit tooltips?'),
        '#default_value' => $this->options['behaviors']['cluster_tooltip'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-behaviors-cluster' => array('1')),
      );
      $form['behaviors']['cluster_tooltip_callback'] = array(
        '#type' => 'textfield',
        '#title' => t('Callback for tooltip contents when clustering'),
        '#description' => t('Enter the name of a JavaScript callback that returns the contents of tolltips (a string) attached to clustered features.  The callback takes one argument: \'feature\'.  <em>feature.cluster</em> contains the features being clustered together.'),
        '#default_value' => $this->options['behaviors']['cluster_tooltip_callback'],
        '#process' => array('views_process_dependency'),
        '#dependency_count' => 2,
        '#dependency' => array('edit-style-options-behaviors-cluster' => array('1'), 'edit-style-options-behaviors-cluster-tooltip' => array('1')),
      );
    }

    // Grouping
    $form['grouping_set'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grouping'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $grouping_options = array_merge(array('0' => t('<none>')), $fields);
    $form['grouping_set']['grouping'] = array(
      '#type' => 'select',
      '#title' => t('Grouping Field'),
      '#options' => $grouping_options,
      '#default_value' => $this->options['grouping_set']['grouping'],
      '#description' => t('You can group features by field.  This means that each group will be a separate layer with those specific features.'),
    );

    // Variable Styles
    $form['style'] = array(
        '#type' => 'fieldset',
        '#title' => t('Feature Styles'),
        '#description' => t('For advanced users only'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['style']['style_context_callback'] = array(
        '#type' => 'textfield',
        '#title' => t('Style-context callback'),
        '#description' => t('If you want to be able to theme features on the fly with JavaScript, write a JS function that returns an associative array of functions and variables (a style context) and enter its name here.  You can then use these callbacks anywhere you define a style array (for example, in theme_openlayers_vector_styles) with "${func_name}".  For example: "pointRadius" => "${get_cluster_size}".  This is particularly useful when you have clustering enabled, as the clustering algorithm creates new features on the fly and it\'s difficult to theme them before runtime.  One caveat: if you define a style context, you cannot access feature.attributes with the ${} notation (although you can access feature.attributes directly from within your context\'s functions). <br /> The function takes the parameters func(map_id, layer_id, render_intent) and so can return different contexts based on the map, layer, and render intent.'),
        '#default_value' => $this->options['style']['style_context_callback'],
    );
    $form['style']['snippet'] = array(
      '#type' => 'textarea',
      '#title' => t('Feature Styles function'),
      '#description' => t('Enter PHP code that returns a style array for each feature. This will overrirde theme feature styling functions. <br/> $fields contains your defined fields, $record contains instances of those fields along with geometries. Array should be of the form \'styleProperty\' => \'styleValue\'. You may return false to use defaults. See documentation for available style Properties.'),
      '#default_value' => $this->options['style']['snippet'],
    );
  }

  /**
   * Map features
   */
  function map_features($records = array(), $group = NULL) {
    $features = array();
    $data_source = $this->options['data_source'];

    // Get list of fields in this view
    $handlers = $this->display->handler->get_handlers('field');
    $fields = array();
    foreach ($handlers as $field_id => $handler) {
      $fields[$field_id] = $handler->definition;
      $fields[$field_id]['field_alias'] = $handler->field_alias;
    }

    // Build feature. We create one feature per field per row.
    foreach ($records as $id => $record) {
      $this->view->row_index = $id;
      $feature = array();
      // In order for any kind of substitution/replacement
      // token/convert to link/trim/etc. functionality to work, we
      // have to call advanced_render on each field in the record
      // in order and use its output to generate the attributes
      // for features.
      $rendered_record = array();
      foreach ($handlers as $hid => $handler) {
        // Render record fields to add to attributes
        $rendered_record[$handler->field_alias] = $handler->advanced_render($record);
      }

      // Go through data sources
      // @@TODO: In theory, there could be multiple features per row., allow for multiple features per row

      $feature['wkt'] = array();

      foreach ($data_source['value'] as $data_source_value) {
        switch ($data_source_value) {
          case 'node_locations':
            // @@TODO: These fields are provided only if the fields are chosen in the interface.
            if (!empty($record->location_longitude) && !empty($record->location_latitude)) {
              $wkt_string = 'POINT(' . $record->location_longitude . ' ' . $record->location_latitude . ')';
              $feature['wkt'][] = $wkt_string;
              $feature['projection'] = '4326';
            }
            break;

          case 'geo':
            //@@TODO: Make it work on grouped multi-value fields geo. First http://drupal.org/node/446754 must be fixed.
            foreach ($data_source['geo_fields'] as $geo_field) {
              $geo_alias = $fields[$geo_field .'_geo']['field_alias'];
              $geo_record = $record->$geo_alias;
              $wkt_object = geo_wkb_get_data($geo_record['wkb'], 'wkt');
              $wkt = $wkt_object['value'];
              if (!empty($wkt)) {
                $feature['wkt'][] = $wkt;
                $feature['projection'] = $geo_record['srid'];
              }
            }
            break;

          case 'other_latlon':
            $lat_field = $fields[$data_source['other_lat']]['field_alias'];
            $lon_field = $fields[$data_source['other_lon']]['field_alias'];
            $lon = $record->$lon_field;
            $lat = $record->$lat_field;

            if (!empty($lat) && !empty($lon)) {
              $feature['wkt'][] = 'POINT(' . $lon . ' ' . $lat . ')';
              $feature['projection'] = '4326';
            }
            break;

          case 'other_wkt':
            // Go through selected field
            foreach ($data_source['other_wkt'] as $wkt_field) {
              $wkt_alias = $fields[$wkt_field]['field_alias'];

              if ($wkt_alias == 'node_vid') {
                // It's a grouped multi-value.
                $wkt = array();
                foreach ($this->view->field[$wkt_field]->field_values[$record->$wkt_alias] as $wkt_item) {
                  if ($wkt_item['openlayers_wkt']) {
                    $wkt[] = $wkt_item['openlayers_wkt'];
                  }
                }
              }
              else {
                // It's a normal field item;
                $wkt = array($record->$wkt_alias);
              }

              if (!empty($record->$wkt_alias)) {
                $feature['wkt'] = array_merge($feature['wkt'], $wkt);
                // @@TODO: Allow different projections
                $feature['projection'] ='4326';
              }
            }
            break;

        }
      }

      // Fill in all attributes
      foreach ($fields as $fid => $field) {
        $field_alias = $field['field_alias'];
        $feature['attributes'][$field_alias] = $record->$field_alias;
        $feature['attributes'][$field_alias . "_rendered"] = $rendered_record[$field_alias];
      }

      // Fill in tooltip attribute
      if (!empty($this->options['behaviors']['tooltip'])) {
        if ($this->options['behaviors']['tooltip'] == '#all_fields') {
          $feature['attributes']['openlayers_tooltip'] = '<div class="map-tooltip openlayers-map-tooltip"><div class="map-pop-up-inner">' . $this->row_plugin->render($record) . '</div></div>';
        }
        else {
          $tooltip_field = $this->options['behaviors']['tooltip'];
          $tooltip_field_alias = $fields[$tooltip_field]['field_alias'];
          $feature['attributes']['openlayers_tooltip'] = '<div class="map-tooltip openlayers-map-tooltip"><div class="map-pop-up-inner">' . $rendered_record[$tooltip_field_alias] . '</div></div>';
        }
      }

      // Fill in popup attribute
      if (!empty($this->options['behaviors']['popup'])) {
        if ($this->options['behaviors']['popup'] == '#all_fields') {
          $feature['attributes']['openlayers_popup'] = '<div class="map-pop-up openlayers-map-pop-up"><div class="map-pop-up-inner">' . $this->row_plugin->render($record) . '</div></div>';
        }
        else {
          $popup_field = $this->options['behaviors']['popup'];
          $popup_field_alias = $fields[$popup_field]['field_alias'];
          $feature['attributes']['openlayers_popup'] = '<div class="map-pop-up openlayers-map-pop-up"><div class="map-pop-up-inner">' . $rendered_record[$popup_field_alias] . '</div></div>';
        }
      }

      // Run feature styles through theme function
      $feature_style = theme('openlayers_views_feature_style', $this, $record, $group);
      if (is_array($feature_style) && !empty($feature_style)) {
        $feature['style'] = $feature_style;
      }

      // Run feature styles through custom php function
      if ($this->options['style']['snippet']) {
        $feature_style = eval($this->options['style']['snippet']);
        if (is_array($feature_style)) {
          $feature['style'] = $feature_style;
        }
      }

      // Only add features with WKT data
      if (!empty($feature['wkt'])) {
        $features[] = $feature;
      }
    }
    unset($this->view->row_index);
    return $features;
  }

  /**
   * Renders views (map)
   */
  function render() {
    $output = '';

    // Check for live preview.
    if (!empty($this->view->live_preview)) {
      return t('OpenLayers views are not compatible with live preview.');
    }

    // Check row plugin if using it.
    if (empty($this->row_plugin)) {
      drupal_set_message(t('Missing Row Plug-in'), 'error');
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    // Get selected preset
    $preset_name = $this->options['presets']['map_preset'];
    $preset = openlayers_get_preset($preset_name);
    $map = $preset['preset_data'];

    // Define ID
    $map['id'] = OPENLAYERS_VIEWS_MAP_ID_PREFIX . '-' . $this->view->name;

    // Get the name of the JS callback that generates style contexts
    if ($this->options['style']['style_context_callback'])
        $map['styleContextCallback'] = $this->options['style']['style_context_callback'];

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping_set']['grouping']);
    $grouped = !empty($this->options['grouping_set']['grouping']) ? TRUE : FALSE;

    // Render each group separately and concatenate.  Plugins may override this
    // method if they wish some other way of handling grouping.
    foreach ($sets as $group_title => $records) {
      // Make 'machine readable' format
      $group_name = _openlayers_views_clean($group_title);

      // @@TODO: Reduce and sort duplicates

      // Style the features based on group (can override this with the theme hook)
      $features = $this->map_features($records, $group_name);

      // Check if plugin wants to display empty map and if features
      if ($this->options['presets']['not_display_empty_map'] && count($features) == 0) {
        // Ensure regular views ops
        $output .= theme($this->theme_functions(), $this->view, $this->options, '', $group_name);
        return $output;
      }

      // Create name of layer and id
      $layer_name = $grouped ? $group_title : $this->display->display_title;
      $layer_id = 'openlayers_views_layer_' . $this->view->name;
      $layer_id .= $grouped ? '_' . $group_name : '';

      // Define a layer for the features
      $map['layers'] = is_array($map['layers']) ? $map['layers'] : array();
      $map['layers'][$layer_id] = array(
        'id' => $layer_id,
        'type' => 'Vector',
        'name' => $layer_name,
        'options' => array(),
        'events' => array(),
        'features' => $features,
      );

      // Set up per-layer behaviors
      if (module_exists('openlayers_behaviors')) {

        // Set up tooltip behavior
        if (!empty($this->options['behaviors']['tooltip'])) {
          $map['behaviors'] = is_array($map['behaviors']) ? $map['behaviors'] : array();
          $field_id = $this->view->display_handler->get_handler('field', $this->options['behaviors']['tooltip'])->field_alias;
          $map['behaviors']['openlayers_views_tooltip_' . $layer_id] = array(
            'id' => 'openlayers_views_tooltip_' . $layer_id,
            'type' => 'openlayers_behaviors_tooltip',
            'layer' => $layer_id,
            'attribute' => 'openlayers_tooltip',
          );
        }

        // Set up declutter behaviors
        if ($this->options['behaviors']['declutter']) {
          $map['behaviors'] = is_array($map['behaviors']) ? $map['behaviors'] : array();
          $map['behaviors']['openlayers_views_declutter_' . $layer_id] = array(
            'id' => 'openlayers_views_declutter_' . $layer_id,
            'type' => 'openlayers_behaviors_declutter',
            'layer' => $layer_id,
          );

          if ($this->options['behaviors']['declutter_adjustment']) {
            $map['behaviors']['openlayers_views_declutter_' . $layer_id]['adjustment'] = intval($this->options['behaviors']['declutter_adjustment']);
          }
        }

        // Set up clustering behaviors
        if ($this->options['behaviors']['cluster']) {
          $map['behaviors'] = is_array($map['behaviors']) ? $map['behaviors'] : array();
          $map['behaviors']['openlayers_views_cluster_' . $layer_id] = array(
            'id' => 'openlayers_views_cluster_' . $layer_id,
            'type' => 'openlayers_behaviors_cluster',
            'layer' => $layer_id,
          );

          // Add cluster values
          if ($this->options['behaviors']['cluster_distance'])
            $map['behaviors']['openlayers_views_cluster_' . $layer_id]['cluster_distance'] = $this->options['behaviors']['cluster_distance'];
          if ($this->options['behaviors']['cluster_threshold'])
            $map['behaviors']['openlayers_views_cluster_' . $layer_id]['cluster_threshold'] = $this->options['behaviors']['cluster_threshold'];
          if ($this->options['behaviors']['cluster_popup'])
            $map['behaviors']['openlayers_views_cluster_' . $layer_id]['cluster_popup'] = $this->options['behaviors']['cluster_popup'];
          if ($this->options['behaviors']['cluster_popup_callback'])
            $map['behaviors']['openlayers_views_cluster_' . $layer_id]['cluster_popup_callback'] = $this->options['behaviors']['cluster_popup_callback'];
          if ($this->options['behaviors']['cluster_tooltip'])
            $map['behaviors']['openlayers_views_cluster_' . $layer_id]['cluster_tooltip'] = $this->options['behaviors']['cluster_tooltip'];
          if ($this->options['behaviors']['cluster_tooltip_callback'])
            $map['behaviors']['openlayers_views_cluster_' . $layer_id]['cluster_tooltip_callback'] = $this->options['behaviors']['cluster_tooltip_callback'];
        }
      }
    } // end foreach views-group/layer

    // Sort layers by id
    ksort($map['layers']);

    // Check other behavior options
    if (module_exists('openlayers_behaviors')) {
      $map['behaviors'] = is_array($map['behaviors']) ? $map['behaviors'] : array();

      // Popups, which apply to all Vector layers when activated
      // @@TODO: Put checkboxes in the style plugin interface to choose layers to use popups with?
      if ($this->options['behaviors']['popup']) {
        $map['behaviors'] = is_array($map['behaviors']) ? $map['behaviors'] : array();
        $map['behaviors']['openlayers_views_popup'] = array(
          'id' => 'openlayers_views_popup',
          'type' => 'openlayers_behaviors_popup',
          'attribute' => 'openlayers_popup',
        );
      }

      // Zoom to layer
      if ($this->options['behaviors']['zoom_to_layer']) {
        $map['behaviors']['openlayers_views_zoom_to_layer'] = array(
          'id' => 'openlayers_views_zoom_to_layer',
          'type' => 'openlayers_behaviors_zoom_to_layer',
        );

        if ($grouped) {
          $map['behaviors']['openlayers_views_zoom_to_layer']['layer'] = array();
          foreach ($sets as $group_title => $records) {
            $group_name = _openlayers_views_clean($group_title);
            $layer_id = 'openlayers_views_layer_' . $this->view->name;
            $layer_id .= $grouped ? '_' . $group_name : '';
            $map['behaviors']['openlayers_views_zoom_to_layer']['layer'][] = $layer_id;
          }
        }
        else {
          $map['behaviors']['openlayers_views_zoom_to_layer']['layer'] = 'openlayers_views_layer_'. $this->view->name;
        }
      }

      // Full screen
      if ($this->options['behaviors']['fullscreen']) {
        $map['behaviors']['openlayers_views_fullscreen'] = array(
          'id' => 'openlayers_views_fullscreen',
          'type' => 'openlayers_behaviors_fullscreen'
        );
      }
    }

    // Render map
    $map = openlayers_render_map($map);

    // Return map array
    $output .= theme($this->theme_functions(), $this->view, $this->options, $map, $group_name);
    return $output;
  }
}

/**
 * Format names so that they can be more easily used by
 * Views.
 *
 * @param $dirty_name
 *   String to manipulate
 * @return
 *   Manipulated string
 */
function _openlayers_views_clean($dirty_name = '') {
  $cleaning = strip_tags($dirty_name);
  $cleaning = drupal_strtolower(str_replace(' ', '_', $cleaning));
  $cleaning = preg_replace("/[^a-z0-9_.]+/i", "", $cleaning);
  return $cleaning;
}
