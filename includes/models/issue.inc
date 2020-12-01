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

    // Add page to canvases.
    $canvases[] = islandora_iiif_manifests_get_object_canvas($page_object);
  }

  //Set Presentation information.
  $manifest['viewingDirection'] = $viewingDirection;
  $manifest['viewingHint'] = $viewingHint;

  $label = islandora_iiif_manifests_convert_label($object);
  
  if ($object['MODS']) {
    // Get Image info from TECHMD.
    $mods_datastream = $object['MODS']->content;
    $xml = simplexml_load_string($mods_datastream);
    $json = json_encode($xml);
    $tech_md = json_decode($json, TRUE);
    
    echo "1<pre>";print_r($tech_md);
    echo "2<pre>";print_r(islandora_newspaper_get_issue($object));die;
  }
  
  // Set the canvases.
  $manifest['sequences'][] = array(
    '@type' => 'sc:Sequence',
    'label' => $label." - ".$page['issued']->format('D j F Y'),
    "navDate" => $page['issued']->format('Y-m-d\TH:i:s\Z\''),
    'canvases' => $canvases
  );

  return $manifest;
}
