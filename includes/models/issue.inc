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
  
  $viewing_hint = islandora_paged_content_get_viewing_hint($object) != "" ? islandora_paged_content_get_viewing_hint($object) : "paged";
  $viewing_direction = islandora_iiif_manifests_get_viewing_direction($object);
  
  foreach ($pages as $page) {
    
    // Get page object.
    $page_object = islandora_object_load($page['pid']);
    
    // Add page to canvases.
    $canvases[] = islandora_iiif_manifests_get_object_canvas($page_object);
  }
  
  //Set Presentation information.
  $manifest['viewingDirection'] = $viewing_direction;
  $manifest['viewingHint'] = $viewing_hint;
  
  $date_issued = islandora_newspaper_get_date_issued_from_mods($object['MODS']);
  
  $label = islandora_iiif_manifests_convert_label($object);
  
  // Set the canvases.
  $manifest['sequences'][] = array(
    '@type' => 'sc:Sequence',
    "label" => $label,
    "navDate" => $date_issued->format('Y-m-d\TH:i:s\Z\''),
    'canvases' => $canvases
  );
  
  return $manifest;
}
