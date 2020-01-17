<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_compound_manifest(AbstractObject $object)
{
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');

  // Manifest array
  $manifest = array();

  // Canvases
  $canvases = array();

  // Get compound parts
  $parts = islandora_compound_object_get_parts($object->id);

  $manifest_type = compoundManifestType($object);

  // collection of the manifests
  $collection_members_members = array();

  foreach ($parts as $part) {

    // Load compound part
    $part_object = islandora_object_load($part);

    if (allowedCModel($part_object)) {

      if ($manifest_type == "sc:Collection") {
        $collection_item = getObjectCollection($part_object);
        $collection_members_members[] = $collection_item['object'];
      } elseif ($manifest_type == "sc:Manifest") {
        // Add page to canvases
        $canvases[] = getObjectCanvas($part_object);

      }

    }

  }

  if ($manifest_type == "sc:Collection") {
    if (!empty($collection_members_members)) {
      $manifest['members'] = $collection_members_members;
    }
  } elseif ($manifest_type == "sc:Manifest") {
    //Set Presentation information
    $manifest['viewingDirection'] =   "left-to-right";
    $manifest['viewingHint'] = "paged";

    $label = convertLabel($object);

    // Set the canvases
    $manifest['sequences'][] = array(
      '@type' => 'sc:Sequence',
      'label' => $label,
      'canvases' => $canvases
    );

    return $manifest;
  }




  return $manifest;

}

