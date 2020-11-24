<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
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
  
  
  $j = 0;
  
  foreach ($group_issues as $volume => $issue) {
    
    $newspaper_collection = array();
    
    if (!empty(islandora_iiif_manifests_get_newspaper_member_collection($issue))) {
      
      $manifest_uri = urldecode(url("iiif_manifest/{$object->id}/collection/{$volume}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )));
      
      
      $newspaper_collection =  array(
        "@id" => $manifest_uri,
        "@type" => 'sc:Collection',
        "label" => "Volume " . $volume,
      );
      
      $newspaper_collection['members'] = islandora_iiif_manifests_get_newspaper_member_collection($issue);
      echo "<pre>";print_r($newspaper_collection);
      array_push($manifest['collections'],$newspaper_collection['members']);
  
      echo "Regular: <pre>";print_r($manifest);
      echo "Json <pre>";print_r(json_encode($manifest)); die;
      $j++;
    }
   
    
  }
  
  echo "Regular: <pre>";print_r($manifest);
  echo "Json <pre>";print_r(json_encode($manifest)); die;
  
  return $manifest;
  
}
