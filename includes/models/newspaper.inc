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

  // Manifest array.
  $manifest = array();

  // Get issues.
  $issues = islandora_newspaper_get_issues($object);

  $collection_members_members = array();

  foreach ($issues as $issue) {

    // Load issue.
    $issue_object = islandora_object_load($issue['pid']);

    if(islandora_iiif_manifests_allowed_manifest($object)){
        $collection_item = islandora_iiif_manifests_get_object_collection($issue_object);

        $collection_members_members[] = $collection_item['object'];
    }


  }

  if (!empty($collection_members_members)) {
    $manifest['members'] = $collection_members_members;
  }

  return $manifest;

}