<?php

/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_issue_manifest(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_newspaper', 'includes/utilities');

  // Manifest array.
  $manifest = array();

  // Canvases.
  $canvases = array();

  // Get pages.
  $pages = islandora_paged_content_get_pages($object);

  $viewingHint = islandora_paged_content_get_viewing_hint($object) != "" ? islandora_paged_content_get_viewing_hint($object) : "paged";
  $viewingDirection = islandora_iiif_manifests_get_viewing_direction($object);

  foreach ($pages as $page) {

    // Get page object.
    $page_object = islandora_object_load($page['pid']);
  
    echo "page:<pre>";print_r($page_object);die;

    // Add page to canvases.
    $canvases[] = islandora_iiif_manifests_get_object_canvas($page_object);
  }

  //Set Presentation information.
  $manifest['viewingDirection'] = $viewingDirection;
  $manifest['viewingHint'] = $viewingHint;

  $label = islandora_iiif_manifests_convert_label($object);
  
  
  // Set the canvases.
  $manifest['sequences'][] = array(
    '@type' => 'sc:Sequence',
    'label' => $label,
    'canvases' => $canvases
  );

  return $manifest;
}
