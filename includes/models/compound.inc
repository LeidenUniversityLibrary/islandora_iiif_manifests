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

    // Get compound parts
    $parts = islandora_compound_object_get_parts($object->id);

    // collection of the manifests
    $collection_members_members = array();

    foreach ($parts as $part){

        // Load compound part
        $part_object = islandora_object_load($part);

        if(allowedCModel($part_object)){

            $collection_item = getObjectCollection($part_object);
            $collection_members_members[] = $collection_item['object'];
        }

    }

    if (!empty( $collection_members_members)) {
        $manifest['members'] = $collection_members_members;
    }

    return $manifest;

}

