<?php


/**
 * @file
 */

/**
 * @param AbstractObject $object
 * @param null $selected_collection - Return the selected sub collection
 * @return mixed
 */
function islandora_iiif_manifest_create_newspaper_manifest(AbstractObject $object, $selected_collection = NULL) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_newspaper', 'includes/utilities');
  
  if(empty($selected_collection)){
    
    $manifest =  islandora_iiif_manifest_get_newspaper_manifest($object);
    
  }else{
      $manifest =  islandora_iiif_manifest_get_newspaper_collection($object,$selected_collection);
  }
  
  return $manifest;
  
}

/**
 * Create the newspaper manifest
 * @param $object
 * @return mixed
 */
function islandora_iiif_manifest_get_newspaper_manifest($object) {
  
  // Collections array
  $manifest['collections'] = array();
  
  // Get issues.
  $issues = islandora_newspaper_get_issues($object);
  
  // Get issues.
  $group_issues = islandora_newspaper_group_issues($issues);
  // Collections array
  $manifest['collections'] = array();
  
  foreach ($group_issues as $volume => $issue) {
    
    
    if (!empty(islandora_iiif_manifests_get_newspaper_member_collection($issue))) {
      
      $manifest_uri = urldecode(url("iiif_manifest/{$object->id}/collection/{$volume}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )));
      
      $newspaper_collection = array(
        "@id" => $manifest_uri,
        "@type" => 'sc:Collection',
        "label" => $volume,
      );
      
      $newspaper_collection['members'] = islandora_iiif_manifests_get_newspaper_member_collection($issue);
      array_push($manifest['collections'], $newspaper_collection);
      
    }
    
  }
  
  return $manifest;
}

/**
 * Create the newspaper sub collection manifest
 * @param $object
 * @param $selected_collection
 * @return array
 */
function islandora_iiif_manifest_get_newspaper_collection($object,$selected_collection) {
  // Collections array
  $manifest = array();
  
  // Get issues.
  $issues = islandora_newspaper_get_issues($object);
  
  // Get issues.
  $group_issues = islandora_newspaper_group_issues($issues);
  
  
  foreach ($group_issues as $collection => $issue) {
    
    if (intval($selected_collection) === $collection) {
      
      if (!empty(islandora_iiif_manifests_get_newspaper_member_collection($issue))) {
        
        $collection_members_members[] = islandora_iiif_manifests_get_newspaper_member_collection($issue);
        
      }
    }
    
  }
  
  if (!empty($collection_members_members)) {
    $manifest['members'] = $collection_members_members;
  }
  
  return $manifest;
}


