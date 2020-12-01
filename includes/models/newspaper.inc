<?php


/**
 * @file
 */

/**
 * Create the requested newspaper manifest. When $selected_collection is set, the manifest of that collection is returned otherwise
 * the full news paper manifest collection is returned
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
 * Create the newspaper manifest with collections and its member within
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
  
  foreach ($group_issues as $collection => $member) {
    
    if (!empty(islandora_iiif_manifests_get_newspaper_member_collection($member))) {
      
      $manifest_uri = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url'] ) ."/iiif_manifest/{$object->id}/collection/{$collection}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )));
      
      $newspaper_collection = array(
        "@id" => $manifest_uri,
        "@type" => 'sc:Collection',
        "label" => strval($collection),
      );
      
      $newspaper_collection['members'] = islandora_iiif_manifests_get_newspaper_member_collection($member);
      array_push($manifest['collections'], $newspaper_collection);
      
    }
    
  }
  
  return $manifest;
}

/**
 * Create the newspaper manifest collection
 * @param $object
 * @param $selected_collection
 * @return array
 */
function islandora_iiif_manifest_get_newspaper_collection($object,$selected_collection) {
  // Collections array
  $manifest['members'] = array();
  
  // Get issues.
  $issues = islandora_newspaper_get_issues($object);
  
  // Get issues.
  $group_issues = islandora_newspaper_group_issues($issues);
  
  echo "JSON <pre>";print_r($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]);
  
  
  foreach ($group_issues as $collection => $member) {
    
    if (intval($selected_collection) === $collection) {
      
      if (!empty(islandora_iiif_manifests_get_newspaper_member_collection($member))) {
  
        $manifest['members'] = islandora_iiif_manifests_get_newspaper_member_collection($member);
        
      }
    }
    
  }

  return $manifest;
}


