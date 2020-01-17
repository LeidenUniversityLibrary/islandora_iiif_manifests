<?php

/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_container_manifest(AbstractObject $object)
{
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');

  return createManifest($object);

}

/**
 * Response in JSON for the manifest.
 *
 * Creates a JSON response that can have the following format:
 *  Access allowed -> Status: 200 OK, content: { 'pid' => $pid, 'dsid' => $dsid, 'access' => true }
 *  Access forbidden -> Status: 403 Forbidden, content: { 'pid' => $pid, 'dsid' => $dsid, 'access' => false, 'reason' => 'Forbidden' }
 *  Object not found -> Status: 40$ Not Found, content: { 'pid' => $pid, 'dsid' => $dsid, 'access' => false, 'reason' => 'Object not found' }
 *  Datastream not found -> Status: 40$ Not Found, content: { 'pid' => $pid, 'dsid' => $dsid, 'access' => false, 'reason' => 'Datastream not found' }
 *
 * @param string $pid
 *   A pid of an (existing?) object.
 *
 * @param string $dsid
 *   A id of a (existing?) datastream.
 */

function islandora_iiif_manifests_create_manifest_json(AbstractObject $object) {

  module_load_include('inc', 'islandora', 'includes/utilities');

  $output = array();

  $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFESTS_ISLANDORA_IIIF_MANIFEST_HOOK, $object->models);
  //echo "<pre>";print_r($hooks);die;
  foreach ($hooks as $hook) {
    $temp = module_invoke_all($hook, $object);

    if (!empty($temp)) {
      $output = array_merge_recursive($output, $temp);
    }
  }

  _islandora_iiif_create_manifest_response('200 OK', $output);
}


function islandora_iiif__manifests_create_canvas_json(AbstractObject $object) {

  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');

  $output = getObjectCanvas($object);

  _islandora_iiif_create_manifest_response('200 OK', $output);
}

/**
 * Helper function to write out a JSON response.
 */
function _islandora_iiif_create_manifest_response($status, $data = NULL) {
  drupal_add_http_header('Status', $status);
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  drupal_add_http_header('access-control-allow-origin', '*');
  if ($data !== NULL) {
    echo json_encode($data, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK) . "\n";;
    //                                                                                                                                                                             print drupal_json_encode($data);
  }
  exit();
}
