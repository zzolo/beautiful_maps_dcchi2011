// $Id: openlayers_cck.js,v 1.20.2.6 2009/10/06 03:10:12 zzolo Exp $

/**
 * @file
 * Main JS file for open_layers_cck
 *
 * @ingroup openlayers
 */

/**
 * Global Object for Namespace
 */
var OL = OL || {};
OL.CCK = OL.CCK || {};

/**
 * Main Setup Event for CCK Processing
 *
 * Evnet callback for map.
 *
 * @param event
 *   Event object
 */
OL.EventHandlers.CCKProcess = function(event) {
  OL.CCK.maps = OL.CCK.maps || Drupal.settings.openlayers_cck.maps || [];
  var mapid = event.mapDef.id;
  var fieldContainer = OL.CCK.maps[mapid].field_container;
  var $map = $('#' + mapid);
  var $textareas = $('textarea[rel=' + mapid + ']');
  var $wktSwitcher = $('#' + mapid + '-wkt-switcher');
  var $field = $('#' + OL.CCK.maps[mapid].field_id);

  // Define a space for data from the map
  OL.CCK.maps[mapid].features = OL.CCK.maps[mapid].features || {};

  // Define a count for features
  OL.CCK.maps[mapid].featureID = 1;

  // WKT Switcher event
  $wktSwitcher.click(function() {
    $('#' + fieldContainer).toggle('normal');
    return false;
  });

  // Hide textareas by default
  $('#' + fieldContainer).hide();

  // Populate any existing fields
  OL.CCK.populateMap(mapid);
}

/**
 * OpenLayers CCK Populate Map
 *
 * Get Values From CCK and populate the map with shapes
 *
 * @param mapid
 *   String ID of map
 */
OL.CCK.populateMap = function(mapid) {
  var featuresToAdd = [];
  var $hiddenField = $('#' + OL.CCK.maps[mapid].hidden_field_id);
  var collectionString = $hiddenField.val();
  var collection = [];
	var wktFormat = new OpenLayers.Format.WKT();

  // Check if collection
  if (collectionString) {
    // Get array collection and go through
    collection = collectionString.split('||');
    for (var wkt in collection) {
      // Check to make sure the wkt is not empty
      if (collection[wkt] == ''){
        delete collection[wkt];
        continue;
      }
      // Update id for collection item
      OL.CCK.maps[mapid].featureID = OL.CCK.maps[mapid].featureID + 1;
      var featureID = 'feature_id_' + OL.CCK.maps[mapid].featureID.toString();

      // Create feature
      var newFeature = wktFormat.read(collection[wkt]);

    	// Check if feature is defined
      if (typeof(newFeature) == "undefined") {
        // TODO: Notification a little less harsh
        alert(Drupal.t('WKT is not valid'));
        return false;
      }
      else {
      	// Project the geometry if our map has a different geospatial
      	// projection as our CCK geo data.
        if (OL.maps[mapid].projection != OL.mapDefs[mapid].options.displayProjection) {
          newFeature.geometry.transform(OL.maps[mapid].displayProjection, OL.maps[mapid].projection);
        }

        // Link the feature to the field.
        newFeature.cckID = featureID;
    		featuresToAdd.push(newFeature);

        // Update collection
        OL.CCK.updateCollection(mapid, 'add', featureID, collection[wkt]);
      }
    }

    // Add new features
    if (featuresToAdd.length != 0) {
      OL.maps[mapid].layers['openlayers_cck_vector'].addFeatures(featuresToAdd);
    }
  }
}

/**
 * OpenLayers CCK Feature Added Handler
 *
 * This function is triggered when a feature is added by the user;
 * it adds the feature and new ID to cck feature collection
 *
 * @param event
 *   Event object
 */
OL.CCK.featureAdded = function(event) {
  var feature = event.feature;
  var mapid = feature.layer.map.mapid;
  var geometry = feature.geometry.clone();

  // Get new feature ID
  OL.CCK.maps[mapid].featureID = OL.CCK.maps[mapid].featureID + 1;

  // Assign field to feature
  feature.cckID = 'feature_id_' + OL.CCK.maps[mapid].featureID.toString();

  // Project the geometry if our map has a different geospatial projection as our CCK geo data.
  if (OL.maps[mapid].projection != OL.mapDefs[mapid].options.displayProjection){
    geometry.transform(OL.maps[mapid].projection, OL.maps[mapid].displayProjection);
  }

  // Add new field
  OL.CCK.updateCollection(mapid, 'add', feature.cckID, geometry.toString());
}

/**
 * OpenLayers CCK Feaure Modified Handler
 *
 * When a feature on the layer is modified, update
 * the corresponding feature in the collection
 *
 * @param event
 *   Event object
 */
OL.CCK.featureModified = function(event) {
  var feature = event.feature;
  var mapid = feature.layer.map.mapid;

  // Clone the geometry so we may safetly work on it without hurting the feature
  var geometry = feature.geometry.clone();

  // Project the geometry if our map has a different geospatial projection as our CCK geo data.
  if (OL.maps[mapid].projection != OL.maps[mapid].displayProjection) {
    geometry.transform(OL.maps[mapid].projection, OL.maps[mapid].displayProjection);
  }

  // Update collection
  OL.CCK.updateCollection(mapid, 'update', feature.cckID, geometry.toString());
}

/**
 * OpenLayers CCK Feaure Removed Handler
 *
 * When a feature on the layer is deleted, remove it from
 * CCK feature array and update collection
 *
 * @param event
 *   Event object
 */
OL.CCK.featureRemoved = function(event) {
  var feature = event.feature;
  var mapid = feature.layer.map.mapid;

  // Update collection
  OL.CCK.updateCollection(mapid, 'delete', feature.cckID);
}

/**
 * Add Feature to CCK Feature Collection
 *
 * Given map id, add new feature to collecion.  The logic of
 * adding a feature neeeds to account for
 *
 * @param mapid
 *   ID of map being referenced
 * @param action
 *   String of action to perform ('add', 'update', 'delete')
 * @param featureID
 *   New ID of feature
 * @param wkt
 *   WKT to add
 */
OL.CCK.updateCollection = function(mapid, action, featureID, wkt) {
  var multiple = OL.CCK.maps[mapid].field_data.multiple;
  var collectionLength = 0;
  var collection = [];
  var hiddenCollection = [];
  var output = '';
  var $field = $('#' + OL.CCK.maps[mapid].field_id);
  var $hiddenField = $('#' + OL.CCK.maps[mapid].hidden_field_id);  // Assign field to feature
  var newestFeatureIDNum = OL.CCK.maps[mapid].featureID - 1;
  var newestFeatureID = 'feature_id_' + newestFeatureIDNum.toString();

  // Check action
  if (action == 'add') {
    // Check multiple to see how many features we can add
    // multiple = 0, then 1 value
    // multiple = 1, then unlimited
    // multiple > 1, then a specific number
    if (multiple == 0) {
      // Remove any features in layer
      for (var l in OL.maps[mapid].layers['openlayers_cck_vector'].features) {
        if (OL.maps[mapid].layers['openlayers_cck_vector'].features[l].cckID != featureID) {
          OL.maps[mapid].layers['openlayers_cck_vector'].features[l].destroy();
        }
      }
      // Simple remake object
      OL.CCK.maps[mapid].features = {};
      OL.CCK.maps[mapid].features[featureID] = wkt;
    }
    else if (multiple == 1) {
      // Unlimited, just add new wkt
      OL.CCK.maps[mapid].features[featureID] = wkt;
    }
    else if (multiple > 1) {
      // Since using associative array (object), have to manually get size
      for (var id in OL.CCK.maps[mapid].features) {
        collectionLength++;
      }

      // Check current length of features
      if (collectionLength < multiple) {
        OL.CCK.maps[mapid].features[featureID] = wkt;
      }
      else {
        // Remove newest features in layer
        for (var l in OL.maps[mapid].layers['openlayers_cck_vector'].features) {
          if (OL.maps[mapid].layers['openlayers_cck_vector'].features[l].cckID == newestFeatureID) {
            OL.maps[mapid].layers['openlayers_cck_vector'].features[l].destroy();
          }
        }

        // Add new feature
        OL.CCK.maps[mapid].features[featureID] = wkt;

        // Delete newest feature
        delete OL.CCK.maps[mapid].features[newestFeatureID];
      }
    }
  }
  else if (action == 'update') {
    // Update CCK features
    OL.CCK.maps[mapid].features[featureID] = wkt;
  }
  else if (action == 'delete') {
    // Remove from features object
    delete OL.CCK.maps[mapid].features[featureID];
  }

  // Make an array of wkt
  for (var wkt in OL.CCK.maps[mapid].features) {
    collection.push(OL.CCK.maps[mapid].features[wkt]);
  }

  // Make geomotry collection
  if (collection.length > 0) {
    if (collection.length > 1) {
      // There are multiple features, so output either a multipart feature or a geometrycollection.
      // Check to see if all the feature types are the same.
      var samefeaturetype = true;
      for (i in collection) {
        for (j in collection) {
          if (collection[i].substr(0, 5) != collection[j].substr(0, 5)) {
            samefeaturetype = false;
            break;
          }
        }
      }
      if (samefeaturetype == true) {
        // All the same feature types, output a multipart feature.
        output = 'test';
        if (collection[0].substr(0, 5) == 'POINT') {
          output = 'MULTIPOINT(' + collection.join(',').replace(/POINT/g,'') + ')';
        }
        if (collection[0].substr(0, 10) == 'LINESTRING') {
          output = 'MULTILINESTRING(' + collection.join(',').replace(/LINESTRING/g,'') + ')';
        }
        if (collection[0].substr(0, 7) == 'POLYGON') {
          output = 'MULTIPOLYGON(' + collection.join(',').replace(/POLYGON/g,'') + ')';
        }
      }
      else {
        // Different feature types, output a geometrycollection.
        output = 'GEOMETRYCOLLECTION(' + collection.join(',') + ')';
      }
    }
    else {
      // It's just a single feature
      output = collection[0];
    }
  }

  // Update field
  $field.val(output);

  // Update hidden field.  The hidden field is used because
  // it is a lot easier to track data with a distinct delimiter
  for (var wkt in OL.CCK.maps[mapid].features) {
    hiddenCollection.push(OL.CCK.maps[mapid].features[wkt]);
  }
  $hiddenField.val(hiddenCollection.join('||'));
}
