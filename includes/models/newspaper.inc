<?php


/**
 * @file
 */

/**
 * @param AbstractObject $object
 * @param null $volume
 * @return mixed
 */
function islandora_iiif_manifest_create_newspaper_manifest(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_newspaper', 'includes/utilities');
  
  // Collections array
  $manifest['collections'] = array();
  
  // Get issues.
  $issues = islandora_newspaper_get_issues($object);
  
  // Get issues.
  $group_issues = islandora_newspaper_group_issues($issues);

  foreach ($group_issues as $volume => $issue) {
    
    
    
    if (!empty(islandora_iiif_manifests_get_newspaper_member_collection($issue))) {
      
      $manifest_uri = urldecode(url("iiif_manifest/{$object->id}/collection/{$volume}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )));
      
      $newspaper_collection =  array(
        "@id" => $manifest_uri,
        "@type" => 'sc:Collection',
        "label" => $volume,
      );
      
      $newspaper_collection['members'] = islandora_iiif_manifests_get_newspaper_member_collection($issue);
      array_push($manifest['collections'],$newspaper_collection);
  
    }
  }
  return $manifest;
  
}

function islandora_iiif_manifest_create_newspaper_manifest_volume(AbstractObject $object, $volume) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_newspaper', 'includes/utilities');
  
  // Collections array
  $manifest['collections'] = array();
  
  // Get issues.
  $issues = islandora_newspaper_get_issues($object);
  
  // Get issues.
  $group_issues = islandora_newspaper_group_issues($issues);
  
  echo $volume;
  echo "group issues:<pre>";print_r($group_issues);die;
  
  foreach ($group_issues as $volume => $issue) {
    
    
    
    if (!empty(islandora_iiif_manifests_get_newspaper_member_collection($issue))) {
      
      $manifest_uri = urldecode(url("iiif_manifest/{$object->id}/collection/{$volume}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )));
      
      $newspaper_collection =  array(
        "@id" => $manifest_uri,
        "@type" => 'sc:Collection',
        "label" => $volume,
      );
      
      $newspaper_collection['members'] = islandora_iiif_manifests_get_newspaper_member_collection($issue);
      array_push($manifest['collections'],$newspaper_collection);
      
    }
  }
  return $manifest;
  
}
