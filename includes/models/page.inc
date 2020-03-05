<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_page_canvas(AbstractObject $object) {
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');

  $canvas[] = islandora_iiif_manifests_get_object_canvas($object);

  $label = islandora_iiif_manifests_convert_label($object);

  // Set the canvases.
  $manifest['sequences'][] = array(
    '@type' => 'sc:Sequence',
    'label' => $label,
    'canvases' => $canvas,
  );

  return $manifest;

}
