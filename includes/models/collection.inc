<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_collection_manifest(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
  module_load_include('module', 'islandora_basic_collection');

  // Manifest array.
  $manifest = array();

  // Get collection members.
  $collection_members = islandora_basic_collection_get_member_objects($object,0,1);
  $collection_objects_total = $collection_members[0];
  $limit = 20;
  $pages = ceil($collection_objects_total / $limit);
  
  echo "Pages:".$pages."<br>";
  echo "Total members:".$collection_objects_total."<br>";
  
  // collection of the manifests.
  $collection_members_members = array();
  
 for($i = 0;$i <= $pages;$i++){
   
   list($count,$members) = islandora_basic_collection_get_member_objects($object,$i,$limit);

   foreach ($members as $member) {
  
     echo "Page:".$i." Limit:".$limit." Count:".$count."<br>";
    
     // Load collection object.
     $fedora_collection_object = islandora_object_load($member['object']['value']);
    
     if (islandora_iiif_manifests_allowed_cmodel($fedora_collection_object)) {
       $collection_item = islandora_iiif_manifests_get_object_collection($fedora_collection_object);
       $collection_members_members[] = $collection_item['object'];
     }
     else{
       
       echo "Not allowed:<pre>";print_($fedora_collection_object->models);
     }
   }
 }

echo "Collection members:<pre>";print_r($collection_members_members);die;
 

  if (!empty($collection_members_members)) {
    $manifest['members'] = $collection_members_members;
  }

  return $manifest;

}
