// $Id: openlayers.layers.js,v 1.5.2.12 2010/03/06 15:19:37 zzolo Exp $

/**
 * @file
 * JS functions to handle different types of layers for core OpenLayers modules
 *
 * @ingroup openlayers
 */

/**
 * Process WMS Layers
 *
 * Process a WMS definition into a OpenLayers WMS Object
 *
 * @param layerOptions
 *   The WMS layer part of the map definition array.
 * @param mapid
 *   Map ID.
 * @return
 *  OpenLayers WMS layer object.
 */
OL.Layers.WMS = function(layerOptions, mapid) {
  // Check if there is a defined format
  if (typeof(layerOptions.params.format) == "undefined") {
    layerOptions.params.format = "image/png";
  }

  // Return Layer object
  return new OpenLayers.Layer.WMS(layerOptions.name, layerOptions.url, layerOptions.params, layerOptions.options);
};

/**
 * Process Vector Layers
 *
 * Process a Vector layer definition into a OpenLayers Vector Object
 *
 * @param layerOptions
 *   The Vector layer part of the map definition array.
 * @param mapid
 *   Map ID.
 * @return
 *  OpenLayers Vector layer object.
 */
OL.Layers.Vector = function(layerOptions, mapid) {
  var styleMap = {};
  var stylesAdded = [];
  var newFeatures = [];
  var strategies = [];

  // Default styles
  var defaultStyles = new OpenLayers.StyleMap({
    'default': new OpenLayers.Style({
      pointRadius: 5,
      fillColor: "#ffcc66",
      strokeColor: "#ff9933",
      strokeWidth: 4,
      fillOpacity:0.5
    }),
    'select': new OpenLayers.Style({
      fillColor: "#66ccff",
      strokeColor: "#3399ff"
    })
  });

  // Go through styles if there are any
  if (OL.isSet(OL.mapDefs[mapid].styles)) {
    for (var style in OL.mapDefs[mapid].styles) {
      // Check for context callback
      if (OL.mapDefs[mapid].styleContextCallback) {
        // Create new context style
        stylesAdded[style] = new OpenLayers.Style(OL.mapDefs[mapid].styles[style], {
          context: OL.getObject(OL.mapDefs[mapid].styleContextCallback)(mapid, layerOptions.name, style)
        });
      }
      else {
        stylesAdded[style] = new OpenLayers.Style(OL.mapDefs[mapid].styles[style]);
      }
    }
    styleMap = new OpenLayers.StyleMap(stylesAdded);
  }
  else {
    styleMap = defaultStyles;
  }

  // Add features if they are defined
  if (OL.isSet(layerOptions.features)) {
    var wktFormat = new OpenLayers.Format.WKT();
    var newFeatureObject  = {};

    // Go through features
    for (var feat in layerOptions.features) {
      if (OL.isSet(layerOptions.features[feat])) {
        // Extract geometry either from wkt property or lon/lat properties
        if (typeof(layerOptions.features[feat].wkt) != "undefined") {
          var wkt;

          // Check to see if it is a string of wkt, or an array for a multipart feature.
          if (typeof(layerOptions.features[feat].wkt) == "string") {
            wkt = layerOptions.features[feat].wkt;
          }
          if (typeof(layerOptions.features[feat].wkt) == "object" && layerOptions.features[feat].wkt !== null && layerOptions.features[feat].wkt.length !== 0) {
            wkt = "GEOMETRYCOLLECTION(" + layerOptions.features[feat].wkt.join(',') + ")";
          }

          // Get new feature
          newFeatureObject = wktFormat.read(wkt);
        }
        else if (typeof(layerOptions.features[feat].lon) != "undefined") {
          newFeatureObject = wktFormat.read("POINT(" + layerOptions.features[feat].lon + " " + layerOptions.features[feat].lat + ")");
        }

        // If we have successfully extracted geometry add additional
        // properties and queue it for addition to the layer
        if (OL.isSet(newFeatureObject)) {
          var newFeatureSet = [];

          // Check to see if it is a new feature, or an array of new features.
          if (typeof(newFeatureObject[0]) == 'undefined') {
            // It's an actual OpenLayers feature object.
            newFeatureSet[0] = newFeatureObject;
          }
          else{
            // It's an array of OpenLayers objects
            newFeatureSet = newFeatureObject;
          }

          // Go through new features
          for (var i in newFeatureSet) {
            if (OL.isSet(newFeatureSet[i])) {
              var newFeature = newFeatureSet[i];

              // Transform the geometry if the 'projection' property is different from the map projection
              if (typeof(layerOptions.features[feat].projection) != 'undefined') {
                if (layerOptions.features[feat].projection != OL.mapDefs[mapid].projection) {
                  var featureProjection = new OpenLayers.Projection("EPSG:" + layerOptions.features[feat].projection);
                  var mapProjection = OL.maps[mapid].projection;
                  newFeature.geometry.transform(featureProjection,mapProjection);
                }
              }

              // Add Feature ID
              newFeature.fid = feat;

              // Add attribute data
              if (typeof(layerOptions.features[feat].attributes) != "undefined") {
                newFeature.attributes = layerOptions.features[feat].attributes;
                newFeature.data = layerOptions.features[feat].attributes;
              }

              // Add style information
              if (typeof(layerOptions.features[feat].style) != "undefined") {
                // Merge with defaults
                var featureStyle = jQuery.extend({}, OpenLayers.Feature.Vector.style['default'], layerOptions.features[feat].style);
                // Add style to feature
                newFeature.style = featureStyle;
              }

              // Push new features
              newFeatures.push(newFeature);
            }
          }
        }
      }
    }
  }

  // Strategies
  strategies = [];

  // Go through behaviors and try to find clustering (not most
  // efficient way)
  if (OL.isSet(OL.mapDefs[mapid].behaviors)) {
    for (var b in OL.mapDefs[mapid].behaviors) {
      if (OL.isSet(OL.mapDefs[mapid].behaviors[b])) {
        var behavior = OL.mapDefs[mapid].behaviors[b];
        if (behavior.type == 'openlayers_behaviors_cluster' && OL.isSet(behavior.layer) && behavior.layer == layerOptions.id) {
          var cluster = new OpenLayers.Strategy.Cluster({
            features: newFeatures,
            threshold: behavior.threshold,
            distance: behavior.distance
          });
          strategies.push(cluster);
        }
      }
    }
  }

  // Define layer object
  var returnVector = new OpenLayers.Layer.Vector(layerOptions.name, {strategies: strategies, styleMap: styleMap});

  // Add new features if there are any
  if (newFeatures.length !== 0) {
    returnVector.addFeatures(newFeatures);
  }

  // Return processed vector
  return returnVector;
};
