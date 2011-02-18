// $Id: openlayers.js,v 1.44.2.24 2010/05/11 18:35:13 zzolo Exp $

/*jslint white: false */
/*global OpenLayers Drupal $ */

/**
 * @file
 * This file holds the main javascript API for OpenLayers. It is
 * responsable for loading and displaying the map.
 *
 * @ingroup openlayers
 */

/**
 * This is a workaround for a bug involving IE and VML support.
 * See the Drupal Book page describing this problem:
 * http://drupal.org/node/613002
 */
document.namespaces;

/**
 * Global Object for Namespace
 */
var OL = OL || {'Layers': {}, 'EventHandlers': {} ,'Behaviors': {}, 'maps': []};

/**
 * OpenLayers Base Drupal Behavoirs
 */
Drupal.behaviors.openlayers = function() {
  // Check for openlayers
  if ((typeof(Drupal.settings.openlayers) === 'object') && (OL.isSet(Drupal.settings.openlayers.maps))) {
    OL.loadMaps();
  }
};

/**
 * Load Maps from OpenLayers Data
 *
 * Main function to sart loading maps by parsing
 * data from Drupal.
 */
OL.loadMaps = function() {
  // Store rendered maps and other OpenLayer objects in OL object
  OL.mapDefs = Drupal.settings.openlayers.maps;

  // Go through array and make maps
  $.each(OL.mapDefs, function(i, value) {
    var map = OL.mapDefs[i];
    var $map = $('#' + map.id);

    // Define proxy host, if available.
    if (map.proxy_host) {
      OpenLayers.ProxyHost = map.proxy_host;
    }

    // Check if map is already rendered
    if (OL.isSet(OL.maps[map.id]) && OL.isSet(OL.maps[map.id].rendered) &&
      (OL.maps[map.id].rendered === true)
    ) {
      // Do somthing for rendered maps
    }
    else {
      // Trigger beforeEverything event
      var event = {'mapDef': map};
      OL.triggerCustom(map, 'beforeEverything', event);

      // Check to see if there is a div on the page ready for the map.
      // If there is then proceed.
      if ($map.length > 0 && OL.isSet(map.width) && OL.isSet(map.height)) {
        // Add any custom controls
        $map.after(Drupal.theme('mapControls', map.id, map.height));

        // Set-up our registry of active OpenLayers javascript objects
        // for this particular map.
        OL.maps[map.id] = {};
        // Set up places for us to store layers, controls, etc.
        OL.maps[map.id].controls = [];
        OL.maps[map.id].layers = [];
        OL.maps[map.id].active = false;

        // Render Map
        OL.renderMap(map);

        // Hack for IE so points show up
        if ($.browser.msie) {
          $(window).load(function() {
            OL.redrawVectors(OL.maps[map.id].map);
          });
        }
      }
    }
  });
};

/**
 * Render OpenLayers Map
 *
 * The main function to go through all the steps nessisary for rendering a map.
 *
 * @param map
 *   The map definition array.
 */
OL.renderMap = function (map) {
  var options = [];
  var event = {};

  // Create Projection objects
  OL.maps[map.id].projection = new OpenLayers.Projection('EPSG:' + map.projection);

  // Check map options
  if (OL.isSet(map.options)) {
    OL.maps[map.id].displayProjection = new OpenLayers.Projection('EPSG:' + map.options.displayProjection);

    // Create base map options
    options = OL.createMapOptions(map.options, map.controls, map.id);
  }
  else {
    OL.maps[map.id].displayProjection = OL.maps[map.id].projection;
  }

  // Change image path if specified
  if (OL.isSet(map.image_path) && map.image_path) {
    OpenLayers.ImgPath = map.image_path;
  }

  // Store map in our registry of active OpenLayers objects
  OL.maps[map.id].map = new OpenLayers.Map(map.id, options);

  // Add ID to map.
  OL.maps[map.id].map.mapid = map.id;

  // On MouseOver mark the map as "active".
  $('#' + map.id).mouseover(function() {
    OL.maps[map.id].active = true;
  })
  .mouseout(function () {
    OL.maps[map.id].active = false;
  });

  // Trigger beforeLayers event
  event = {'mapDef': map, 'map': OL.maps[map.id].map};
  OL.triggerCustom(map, 'beforeLayers', event);

  // We set up all our layers
  OL.processLayers(map.layers, map.id);

  // Add layers to map
  for (var l in OL.maps[map.id].layers) {
    if (OL.isSet(OL.maps[map.id].layers[l])) {
      var layer =  OL.maps[map.id].layers[l];
      OL.maps[map.id].map.addLayer(layer);
    }
  }

  // Trigger beforeCenter event
  event = {'mapDef': map, 'map': OL.maps[map.id].map};
  OL.triggerCustom(map, 'beforeCenter', event);

  // Zoom to Center
  // @@TODO: Do this in the map options -- As is,
  // this will result in a bug in the zoom map helper in the map form
  if (OL.isSet(map.center)) {
    var center = new OpenLayers.LonLat(map.center.lon, map.center.lat);
	  var zoom = parseInt(map.center.zoom, 10);

    // Check for proj and reproject if necessary
    if (OL.isSet(map.center.proj)) {
      var proj = new OpenLayers.Projection("EPSG:" + map.center.proj);
      center.transform(proj, OL.maps[mapid].map.getProjectionObject());
    }

    OL.maps[map.id].map.setCenter(center, zoom, false, false);
  }

  // Set our default base layer
  OL.maps[map.id].map.setBaseLayer(OL.maps[map.id].layers[map.default_layer]);

  // Trigger beforeControls event
  event = {'mapDef': map, 'map': OL.maps[map.id].map};
  OL.triggerCustom(map, 'beforeControls', event);

  // Add controls to map
  $.each(OL.maps[map.id].controls, function(c, control) {
    OL.maps[map.id].map.addControl(control);
    if (control.activeByDefault) {
      control.activate();
    }
  });

  // Trigger beforeEvents event
  event = {'mapDef': map, 'map': OL.maps[map.id].map};
  OL.triggerCustom(map, 'beforeEvents', event);

  // Add events to the map
  OL.processEvents(map.events, map.id);

  // Trigger beforeBehaviors event
  event = {'mapDef': map, 'map': OL.maps[map.id].map};
  OL.triggerCustom(map, 'beforeBehaviors', event);

  // Add behaviors to map
  $.each(OL.mapDefs[map.id].behaviors, function(b, behavior) {
    event = {};
    event.mapDef = map;
    event.map = OL.maps[map.id].map;
    event.behavior = OL.mapDefs[map.id].behaviors[b];
    OL.Behaviors[OL.mapDefs[map.id].behaviors[b].js_callback](event);
  });

  // Trigger mapReady event
  event = {'mapDef': map, 'map': OL.maps[map.id].map};
  OL.triggerCustom(map, 'mapReady', event);

  // Mark as Rendered
  OL.maps[map.id].rendered = true;
};

/**
 * Get OpenLayers Map Options
 *
 * @param options
 *   Object of options to include
 * @param controls
 *   Object of controls to add
 * @param mapid
 *   String Id of map
 * @return
 *   Object of processed options
 */
OL.createMapOptions = function(options, controls, mapid) {
  var returnOptions = {};

  // Projections
  if (OL.isSet(OL.maps[mapid].projection) && OL.isSet(OL.maps[mapid].displayProjection)) {
    returnOptions.projection = OL.maps[mapid].projection;
    returnOptions.displayProjection = OL.maps[mapid].displayProjection;
  }

  // Max resolution and Extent
  if (OL.isSet(options.maxResolution)) {
    returnOptions.maxResolution = options.maxResolution;
  }
  if (typeof(options.maxExtent) !== "undefined") {
    returnOptions.maxExtent =  new OpenLayers.Bounds(
      options.maxExtent.left,
      options.maxExtent.bottom,
      options.maxExtent.right,
      options.maxExtent.top
    );
  }

  // Controls
  returnOptions.controls = [];
  if (OL.isSet(controls)) {
    // @@TODO: This should be a little more dynamic
    if (controls.LayerSwitcher) {
      returnOptions.controls.push( new OpenLayers.Control.LayerSwitcher() );
    }
    if (controls.Navigation) {
      returnOptions.controls.push( new OpenLayers.Control.Navigation() );
    }
    if (controls.Attribution) {
      returnOptions.controls.push( new OpenLayers.Control.Attribution() );
    }
    if (controls.PanZoomBar) {
      returnOptions.controls.push( new OpenLayers.Control.PanZoomBar() );
    }
    if (controls.PanZoom) {
      returnOptions.controls.push( new OpenLayers.Control.PanZoom() );
    }
    if (controls.MousePosition) {
      returnOptions.controls.push( new OpenLayers.Control.MousePosition() );
    }
    if (controls.Permalink) {
      returnOptions.controls.push( new OpenLayers.Control.Permalink() );
    }
    if (controls.ScaleLine) {
      returnOptions.controls.push( new OpenLayers.Control.ScaleLine() );
    }
    if (controls.KeyboardDefaults) {
      returnOptions.controls.push( new OpenLayers.Control.KeyboardDefaults() );
    }
    if (controls.ZoomBox) {
      returnOptions.controls.push( new OpenLayers.Control.ZoomBox() );
    }
    if (controls.ZoomToMaxExtent) {
      returnOptions.controls.push( new OpenLayers.Control.ZoomToMaxExtent() );
    }
  }

  // Handle fractional zoom
  if (OL.isSet(options.fractionalZoom)) {
    returnOptions.fractionalZoom = options.fractionalZoom;
  }

  // Return processed options
  return returnOptions;
};

/**
 * Process Layers
 *
 * Process the layers part of the map definition into OpenLayers layer objects
 *
 * @param layers
 *   The layers section of the map definition array.
 * @param mapid
 *   The id of the map to which we will eventually add these layers.
 */
OL.processLayers = function(layers, mapid) {
  OL.maps[mapid].layers = [];

  // Go through layers
  if (layers) {
    for (var layer in layers) {
      // Process layer, check for function
      if (OL.isSet(OL.Layers) && typeof(OL.Layers[layers[layer].layer_handler]) === 'function') {
        var newLayer = OL.Layers[layers[layer].layer_handler](layers[layer], mapid);
        OL.maps[mapid].layers[layer] = newLayer;

        // Add our Drupal data to the layer
        newLayer.drupalId = layer;
        newLayer.drupalData = layers[layer];

        // Add events
        for (var evtype in layers[layer].events) {
          if (OL.isSet(layers[layer].events[evtype])) {
            for (var ev in layers[layer].events[evtype]) {
              if (OL.isSet(layers[layer].events[evtype][ev])) {
                newLayer.events.register(evtype, newLayer, OL.EventHandlers[layers[layer].events[evtype][ev]]);
              }
            }
          }
        }
      }
    }
  }
};

/**
 * Redraw Vector Layers
 */
OL.redrawVectors = function(map) {
  // Get each vector layer
  $.each(map.getLayersByClass('OpenLayers.Layer.Vector'), function(i, layer) {
    layer.redraw();
  });
};

/**
 * Process Events
 *
 * Process the layers part of the map definition into OpenLayers layer objects
 *
 * @param events
 *   The events section of the map definition array.
 * @param mapid
 *   The id of the map to which we will add these events.
 */
OL.processEvents = function(events, mapid) {
  // Go through events
  for (var evtype in events) {
    if (OL.isSet(events[evtype])) {
      // Exclude One-Time map events.
      var eventTypes = ['beforeEverything', 'beforeLayers', 'beforeCenter', 'beforeControls', 'beforeEvents', 'beforeBehaviors', 'mapReady'];
      if ($.inArray(evtype, eventTypes) === -1) {
        for (var ev in events[evtype]) {
          if (OL.isSet(events[evtype][ev])) {
            OL.maps[mapid].map.events.register(evtype, OL.maps[mapid].map, OL.EventHandlers[events[evtype][ev]]);
          }
        }
      }
    }
  }
};

/**
 * Trigger Custom Event
 *
 * @param map
 *   Map object
 * @param eventName
 *   String of the name of the event
 * @param event
 *   Event object
 */
OL.triggerCustom = function(map, eventName, event) {
  if (OL.isSet(map.events) && OL.isSet(map.events[eventName])) {
    for (var ev in map.events[eventName]) {
      if (OL.isSet(map.events[eventName][ev])) {
        OL.EventHandlers[map.events[eventName][ev]](event);
      }
    }
  }
};

/**
 * Parse out key / value pairs out of a string that looks like "key:value;key2:value2"
 *
 * @param rel
 *   The string to parse. Usually the rel attribute of a html tag.
 * @return
 *   Array of key:value pairs
 */
OL.parseRel = function(rel) {
  var outputArray = [];

  // Some preprosessing
  // replace dangling whitespaces. Use regex?
  rel = rel.replace('; ',';');
  //Cut out final ; if it exists
  if (rel.charAt(rel.length-1) === ";") {
    rel = rel.substr(0,rel.length-1);
  }

  //Get all the key:value strings
  var keyValueStrings = rel.split(';');

  // Process the key:value strings into key:value pairs
  for (var i in keyValueStrings) {
    if (OL.isSet(keyValueStrings[i])) {
      var singleKeyValue = keyValueStrings[i].split(':');
      outputArray[singleKeyValue[0]] = singleKeyValue[1];
    }
  }

  return outputArray;
};

/**
 * Given a string of the form 'OL.This.that', get the object that the
 * string refers to.
 *
 * @param string
 *   The string to parse.
 * @return
 *   Object
 */
OL.getObject = function(string) {
  if (typeof(string) == 'string') {
    var parts = string.split('.');
    var i = 0;
    var object = window;
    for (var i in parts) {
      if (OL.isSet(parts[i])) {
        object = object[parts[i]];
      }
    }
    return object;
  }
  return {};
};

/**
 * Check if Variable is define
 *
 * @params variable
 *   Any variable
 * @return
 *   Boolean if the variable is definied or not
 */
OL.isSet = function(variable) {
  if (typeof(variable) == 'undefined') {
    return false;
  }
  else {
    return true;
  }
};

/**
 * Map Controls Theme Function
 *
 * @param mapid
 *   String of mapid
 * @param height
 *   String of the height of the map
 * @return
 *   Themed map control division
 */
Drupal.theme.prototype.mapControls = function(mapid, height) {
  var newcontainer = $('<div></div>');
  newcontainer.addClass('openlayers-controls').attr('id', 'openlayers-controls-' + mapid).css('position', 'relative').css('top', '-' + height);
  return newcontainer;
};
