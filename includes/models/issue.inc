<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_issue_manifest(AbstractObject $object)
{
    module_load_include('inc', 'islandora', 'includes/utilities');
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');


    // Canvases
    $canvases = array();

    // Get pages
    $pages = islandora_paged_content_get_pages($object);


    // Don't create a manifest container for the compound parts
    $child_manifest_set = false;
    $count = 0;
    foreach ($pages as $page){

        // Load page
        $page_object = islandora_object_load($page['pid']);

        // Get the hooks to create the canvases
        $page_hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFEST_HOOK, $page_object->models);
        //echo "page hooks<pre>";print_r($page_hooks);die;
        foreach ($page_hooks as $page_hook) {

            $temp = module_invoke_all($page_hook, $page_object);

            if (!empty($temp)) {
                // Check if the manifest container is set
                if(!$child_manifest_set){
                    $child_manifest_set = true;
                }
                else{
                    if(canvasHook($page_hook)){
                        // Add a canvas
                        array_push($canvases,$temp);
                    }
                }
            }
        }
    }

    return $canvases;

}



