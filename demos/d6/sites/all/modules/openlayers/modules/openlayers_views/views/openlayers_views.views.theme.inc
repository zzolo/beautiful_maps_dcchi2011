<?php
// $Id: openlayers_views.views.theme.inc,v 1.1.2.2 2009/08/06 04:42:06 binarybill Exp $

/**
 * @file
 * This file holds views theme function for style plugin.
 *
 * @ingroup openlayers
 */

/**
 * Implementation of theme function
 */
function theme_openlayers_views_map($view, $options = array(), $map = array()) {
  $output = '
    <div class="openlayers-views-map">
      ' . $map['themed'] . '
    </div>
  ';

  return $output;
}
