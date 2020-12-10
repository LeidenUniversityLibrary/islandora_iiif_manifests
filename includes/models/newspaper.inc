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
  module_load_include('inc', 'islandora', 'includes/datastream');
  
  if (empty($selected_collection)) {
    $manifest = islandora_iiif_manifest_get_newspaper_manifest($object);
  }
  else {
    $manifest = islandora_iiif_manifest_get_newspaper_collection($object, $selected_collection);
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
    
    $newspaper_member = islandora_iiif_manifests_get_newspaper_member_collection($member);
    if (!empty($newspaper_member)) {
      $manifest_uri = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url']) . "/iiif_manifest/{$object->id}/collection/{$collection}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )));
      if (is_numeric($collection)) {
        $label = strval($collection);
      }
      else {
        $label = $collection;
      }
      $newspaper_collection = array(
        "@id" => $manifest_uri,
        "@type" => 'sc:Collection',
        "label" => $label,
      );
      $newspaper_collection['members'] = $newspaper_member;
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
function islandora_iiif_manifest_get_newspaper_collection($object, $selected_collection) {
  
  echo "Selected collection:$selected_collection <br>";
  
  // Members array
  $manifest['members'] = array();
  $manifest_ds = json_decode($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]->content);
  foreach ($manifest_ds->collections as $col) {
    $collection_id = str_replace("'", "", $col->label);
    echo "$selected_collection = $collection_id  <br>";
    if ($selected_collection === $collection_id) {
      $manifest['members'] = $col->members;
    }
  }
  
  return $manifest;
  
}


