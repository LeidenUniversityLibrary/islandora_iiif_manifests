<?php

/**
 * @file
 */


/**
 * Response in JSON for the manifest.
 */
function islandora_iiif_manifests_view_manifest_json(AbstractObject $object) {
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/datastream');
  
  
  if (isset($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID])) {
    header('Access-Control-Allow-Origin: *');
    
    islandora_view_datastream($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]);
  }
  elseif (in_array('islandora:collectionCModel', $object->models)) {
    islandora_iiif_manifests_create_manifest_json($object);
  }
  else {
    if (!isset($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID])) {
      $response = array(
        "data" => "No manifest found",
        "status" => "400 Bad request"
      );
    }
    if (islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID])) {
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
function islandora_iiif_manifests_create_manifest_json(AbstractObject $object) {
  
  module_load_include('inc', 'islandora', 'includes/utilities');
  
  $output = islandora_iiif_manifests_build_manifest($object);
  
  if (!$output) {
    $response = array(
      "data" => "No manifest found in collection",
      "status" => "400 Bad request"
    );
    islandora_iiif_manifests_create_json_response($response['status'], $response['data']);
  }
  else
    islandora_iiif_manifests_create_json_response('200 OK', $output);
}


/**
 * Create JSON manifest.
 */
function islandora_iiif_manifests_create_manifest_datastream(AbstractObject $object) {
  
  module_load_include('inc', 'islandora', 'includes/utilities');
  
  $output = islandora_iiif_manifests_build_manifest($object);
  
  islandora_iiif_manifests_add_manifest_datastream($object, $output);
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
function islandora_iiif_manifests_build_manifest($object) {
  
  $output = array();
  
  $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFESTS_ISLANDORA_IIIF_MANIFEST_HOOK, $object->models);
  
  foreach ($hooks as $hook) {
    $temp = module_invoke_all($hook, $object);
    
    if (!empty($temp)) {
      $output = array_merge_recursive($output, $temp);
    }
  }
  
  if ($output['@type'] == "sc:Collection" && !isset($output['members'])) {
    return FALSE;
  }
  else
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
