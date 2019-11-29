<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_collection_manifest(AbstractObject $object)
{
    module_load_include('inc', 'islandora', 'includes/utilities');
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
    module_load_include('module', 'islandora_basic_collection');

    // Manifest array
    $manifest = array();

    $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFEST_HOOK, $object->models);

    $manifest_set = false;
    foreach ($hooks as $hook) {
        $temp = module_invoke_all($hook, $object);

        if (!empty($temp) ) {

            // Check if the manifest container is set
            if(!$manifest_set){
                $manifest = array_merge_recursive($manifest, $temp);
                break;
            }

        }
    }

    //echo "Manifest <pre>:";print_r($manifest);die;

    // Canvases
    $canvases = array();

    // Get collection members
    $collection_members = islandora_basic_collection_get_member_objects($object);
    //$collection_count = $collection_members[0];
    $collection_objects = $collection_members[1];

    // Don't create a manifest container for the compound parts
    $child_manifest_set = false;

    foreach ($collection_objects as $collection_object){

        // Load collection object
        $fedora_collection_object = islandora_object_load($collection_object['object']['value']);

        // Get the hooks to create the canvases
        $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFEST_HOOK, $fedora_collection_object->models);

        foreach ($hooks as $part_hook) {

            $temp = module_invoke_all($part_hook, $fedora_collection_object);


            if (!empty($temp)) {
                // Check if the manifest container is set
                if(!$child_manifest_set){
                    $child_manifest_set = true;
                }
                else{
                    if(canvasHook($part_hook)){
                        // Add a canvas
                        $canvases[] = $temp;//array_merge_recursive($children_output, $temp);
                    }
                }
            }
        }
    }

    //Set Presentation information
    $manifest['viewingDirection'] ="left-to-right";
    $manifest['viewingHint'] ="paged";

    // Set the canvases
    $manifest['sequences'][0]['canvases'] = $canvases;

    _islandora_iiif_create_manifest_response('200 OK', $manifest);

}

