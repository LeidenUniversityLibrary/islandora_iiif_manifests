<?php

/**
 * @file
 */


/**
 * Response in JSON for the manifest.
 * Creates collection manifests dynamically if no statict manifest is present
 * Creates newspaper volumes manifest dynamically based on the selected part in the URL
 */
function islandora_iiif_manifests_view_manifest_json(AbstractObject $object, $part = NULL) {
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/datastream');
  
  if (isset($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]) && is_null($part)) {
    header('Access-Control-Allow-Origin: *');
    
    islandora_view_datastream($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]);
  }
  // Dynamically create collection manifest
  elseif (in_array('islandora:collectionCModel', $object->models) || !is_null($part)) {
    islandora_iiif_manifests_create_manifest_json($object, $part);
  }
  else {
    if (!isset($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID])) {
      $response = array(
        "data" => '{"error": "No manifest available"}',
        "status" => "404 Not Found"
      );
    }
    elseif (islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID])) {
      $response = array(
        "data" => "Forbidden",
        "status" => "403 Forbidden"
      );
    }
    islandora_iiif_manifests_create_json_response($response['status'], $response['data']);
  }
  
}

/**
 * Response in JSON for the manifest.
 */
function islandora_iiif_manifests_create_manifest_json(AbstractObject $object, $part = NULL) {
  
  module_load_include('inc', 'islandora', 'includes/utilities');
  
  $output = islandora_iiif_manifests_build_manifest($object, $part);
  
  islandora_iiif_manifests_create_json_response('200 OK', $output);
}

/**
 * Response in JSON for the canvas.
 */
function islandora_iiif_manifests_create_canvas_json(AbstractObject $object) {
  
  module_load_include('inc', 'islandora', 'includes/utilities');
  
  $output = islandora_iiif_manifests_build_canvas($object);
  
  islandora_iiif_manifests_create_json_response('200 OK', $output);
}

/**
 * Build manifest array.
 */
function islandora_iiif_manifests_build_manifest($object, $part = NULL) {
  
  $output = array();
  
  $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFESTS_ISLANDORA_IIIF_MANIFEST_HOOK, $object->models);
  
  foreach ($hooks as $hook) {
    $temp = module_invoke_all($hook, $object, $part);
    
    if (!empty($temp)) {
      $output = array_merge_recursive($output, $temp);
    }
  }
  
  return json_encode($output, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
}

/**
 * Build canvas array.
 */
function islandora_iiif_manifests_build_canvas($object) {
  
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  
  $output = array();
  
  if (islandora_iiif_manifests_allowed_canvas_cmodel($object)) {
    
    $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFESTS_ISLANDORA_IIIF_MANIFEST_CANVAS_HOOK, $object->models);
    
    foreach ($hooks as $hook) {
      $temp = module_invoke_all($hook, $object);
      
      if (!empty($temp)) {
        $output = array_merge_recursive($output, $temp);
      }
    }
  }
  
  return json_encode($output, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
}
