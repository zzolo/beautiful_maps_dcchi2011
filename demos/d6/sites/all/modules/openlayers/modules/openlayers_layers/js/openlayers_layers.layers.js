// $Id: openlayers_layers.layers.js,v 1.2.2.2 2009/10/06 03:10:12 zzolo Exp $

/**
 * @file
 * JS functions to handle different kinds of layers fo openlayers_layers module
 *
 * @ingroup openlayers
 */

/**
 * Process Google Layers
 *
 * @param layerOptions
 *   Object of options
 * @param mapid
 *   Map ID
 * @return
 *   Valid OpenLayers layer
 */
OL.Layers.Google = function(layerOptions, mapid) {
  var mapType;
  if (layerOptions.params.type == "street") {
    mapType = G_NORMAL_MAP;
  }
  else if (layerOptions.params.type == "satellite") {
   mapType = G_SATELLITE_MAP;
  }
  else if (layerOptions.params.type == "hybrid") {
    mapType = G_HYBRID_MAP;
  }
  else if (layerOptions.params.type == "physical") {
    mapType = G_PHYSICAL_MAP;
  }

  var mapOptions = {
    type: mapType,
    sphericalMercator: true,
    maxExtent: new OpenLayers.Bounds(-20037508.34,-20037508.34,20037508.34,20037508.34)
  };

  jQuery.extend(mapOptions, layerOptions.options);

  var googleLayer = new OpenLayers.Layer.Google(
    layerOptions.name,
    mapOptions
  );
  return googleLayer;
}

/**
 * Process Yahoo Layers
 *
 * @param layerOptions
 *   Object of options
 * @param mapid
 *   Map ID
 * @return
 *   Valid OpenLayers layer
 */
OL.Layers.Yahoo = function(layerOptions, mapid) {
  var mapType;
  if (layerOptions.params.type == "street") {
    mapType = YAHOO_MAP_REG;
  }
  else if (layerOptions.params.type == "satellite") {
   mapType = YAHOO_MAP_SAT;
  }
  else if (layerOptions.params.type == "hybrid") {
    mapType = YAHOO_MAP_HYB;
  }

  var mapOptions = {
    type: mapType,
    sphericalMercator: true,
    maxExtent: new OpenLayers.Bounds(-20037508.34,-20037508.34,20037508.34,20037508.34)
  };

  jQuery.extend(mapOptions, layerOptions.options);

  var yahooLayer = new OpenLayers.Layer.Yahoo(
    layerOptions.name,
    mapOptions
  );
  return yahooLayer;
}

/**
 * Process MS Virtual Earth Layers
 *
 * @param layerOptions
 *   Object of options
 * @param mapid
 *   Map ID
 * @return
 *   Valid OpenLayers layer
 */
OL.Layers.VirtualEarth = function(layerOptions, mapid) {
  var mapType;
  if (layerOptions.params.type == "street") {
    mapType = VEMapStyle.Road;
  }
  else if (layerOptions.params.type == "satellite") {
   mapType = VEMapStyle.Aerial;
  }
  else if (layerOptions.params.type == "hybrid") {
    mapType = VEMapStyle.Hybrid;
  }

  var mapOptions = {
    type: mapType,
    sphericalMercator: true,
    maxExtent: new OpenLayers.Bounds(-20037508.34,-20037508.34,20037508.34,20037508.34)
  };

  jQuery.extend(mapOptions, layerOptions.options);

  var virtualEarthLayer = new OpenLayers.Layer.VirtualEarth(
    layerOptions.name,
    mapOptions
  );
  return virtualEarthLayer;
}

/**
 * Process KML Layers
 *
 * @param layerOptions
 *   Object of options
 * @param mapid
 *   Map ID
 * @return
 *   Valid OpenLayers layer
 */
OL.Layers.KML = function(layerOptions, mapid) {

  var mapOptions = {
    projection: new OpenLayers.Projection("EPSG:4326"),
    format: OpenLayers.Format.KML,
    formatOptions: {extractStyles: true, extractAttributes: true}
  };

  jQuery.extend(mapOptions, layerOptions.options);

  var returnKML = new OpenLayers.Layer.GML(
    layerOptions.name,
    layerOptions.url,
    mapOptions
  );

  return returnKML;
}

/**
 * Process XYZ Layers
 *
 * @param layerOptions
 *   Object of options
 * @param mapid
 *   Map ID
 * @return
 *   Valid OpenLayers layer
 */
OL.Layers.XYZ = function(layerOptions, mapid) {
  var mapOptions = {
    sphericalMercator: true,
    maxExtent: new OpenLayers.Bounds(-20037508.34, -20037508.34, 20037508.34, 20037508.34)
  };

  jQuery.extend(mapOptions, layerOptions.options);

  var returnXYZ = new OpenLayers.Layer.XYZ(layerOptions.name, layerOptions.url, mapOptions);
  return returnXYZ;
}

/**
 * Process CloudMade Layers
 *
 * @param layerOptions
 *   Object of options
 * @param mapid
 *   Map ID
 * @return
 *   Valid OpenLayers layer
 */
OL.Layers.CloudMade = function(layerOptions, mapid) {
  // @@TODO: Check for CloudMade definition since it is in another file
  // Make sure there is options for a key
  if (OL.isSet(layerOptions.options)) {
    if (OL.isSet(layerOptions.options.key)) {
      var mapOptions = {
      };
      jQuery.extend(mapOptions, layerOptions.options);

      var cloudmade = new OpenLayers.Layer.CloudMade(layerOptions.name, layerOptions.options);
      return cloudmade;
    }
  }
}
