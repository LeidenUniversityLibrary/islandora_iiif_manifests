<?php
/**
 * @file
 */
/**
 * Create the requested newspaper manifest. When $part is set, the manifest of that collection is returned otherwise
 * the full news paper manifest collection is returned
 * @param AbstractObject $object
 * @param null $part - Return the selected sub collection
 * @return mixed
 */
function islandora_iiif_manifest_create_newspaper_manifest(AbstractObject $object, $part = NULL) {
  
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_newspaper', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/datastream');
  
  if (is_null($part)) {
    $manifest = islandora_iiif_manifest_get_newspaper_manifest($object);
  }
  else {
    $manifest = islandora_iiif_manifest_get_newspaper_collection($object, $part);
  }
  
  return $manifest;
}

/**
 * Create the newspaper manifest with collections and its volumes within
 * @param $object
 * @return mixed
 */
function islandora_iiif_manifest_get_newspaper_manifest($object) {
  // Collections array
  $manifest['collections'] = array();
  
  // Gets all the issues that the given newspaper owns.
  $issues = islandora_newspaper_get_issues($object);
  
  // Get issues by their relevant dates - Year, month and day.
  $group_issues = islandora_newspaper_group_issues($issues);
  
  // Collections array
  $manifest['collections'] = array();
  
  foreach ($group_issues as $part_label => $member) {
    
    // Get newspaper volumes with its issues.
    $newspaper_member = islandora_iiif_manifests_get_newspaper_member_collection($member);
    
    if (!empty($newspaper_member)) {
      $manifest_uri = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url']) . "/iiif_manifest/{$object->id}/part/{$part_label}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )));
      
      if (is_numeric($part_label)) {
        $part_label = "'" . $part_label . "'";
      }
      
      // Create newspaper collection
      $newspaper_collection = array(
        "@id" => $manifest_uri,
        "@type" => 'sc:Collection',
        "label" => $part_label,
      );
      // Add newspaper members (volumes).
      $newspaper_collection['members'] = $newspaper_member;
      
      array_push($manifest['collections'], $newspaper_collection);
    }
  }
  
  return $manifest;
}

/**
 * Create the newspaper manifest collection
 * @param $object
 * @param $part
 * @return array
 */
function islandora_iiif_manifest_get_newspaper_collection($object, $part) {
  
  // Members array
  $manifest['members'] = array();
  
  // Get IIIF Manifest
  $manifest_ds = json_decode($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]->content);
  
  // Retrieve the collection member based on the collection label
  foreach ($manifest_ds->collections as $col) {
    
    $collection_id = str_replace("'", "", $col->label);
    
    if ($part === $collection_id) {
      $manifest['members'] = $col->members;
    }
  }
  
  return $manifest;
  
}


