<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_book_manifest(AbstractObject $object)
{
    module_load_include('inc', 'islandora', 'includes/utilities');
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');


    $viewingHint = islandora_paged_content_get_viewing_hint($object) !="" ?  islandora_paged_content_get_viewing_hint($object):"paged";
    $viewingDirection = getViewingDirection($object);
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


    // Canvases
    $canvases = array();

    // Get pages
    $pages = islandora_paged_content_get_pages($object);


    // Don't create a manifest container for the compound parts
    $child_manifest_set = false;

    foreach ($pages as $page){

        // Load page
        $page_object = islandora_object_load($page['pid']);

        // Get the hooks to create the canvases
        $page_hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFEST_HOOK, $page_object->models);

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
                        $canvases[] = $temp;//array_merge_recursive($children_output, $temp);
                    }
                }
            }
        }
    }

    //Set Presentation information
    $manifest['viewingDirection'] = $viewingDirection;
    $manifest['viewingHint'] =  $viewingHint;

    //echo "Canvases:<pre>";print_r($canvases);die;
    // Set the canvases
    $manifest['sequences'][0]['canvases'] = $canvases;

    _islandora_iiif_create_manifest_response('200 OK', $manifest);

}


/**
 * Determine if the current hook is a canvas hook
 */
function canvasHook($hook){

    switch($hook){

        case 'islandora_get_iiif_manifest':
            $canvas_hook = false;
         break;

        case 'fedora_system_FedoraObject_3_0_islandora_get_iiif_manifest':
            $canvas_hook = false;
         break;

        default:
            $canvas_hook = true;
    }
     return $canvas_hook;

}

/**
 * Retrieve the view direction from the book
 */
function getViewingDirection($object){

    $page_progression = islandora_paged_content_get_page_progression($object);
    $viewingDirection = "";
    switch($page_progression){

        case 'lr':
            $viewingDirection = "left-to-right";
        break;

        case 'rl':
            $viewingDirection = "right-to-left";
        break;
    }

    return $viewingDirection;

}