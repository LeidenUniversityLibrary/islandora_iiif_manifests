<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_basic_image_canvas(AbstractObject $object) {
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');

  $canvas[] = getObjectCanvas($object);

  $label = convertLabel($object);

  // Set the canvases.
  $manifest['sequences'][] = array(
    '@type' => 'sc:Sequence',
    'label' => $label,
    'canvases' => $canvas
  );

  return $manifest;

}
