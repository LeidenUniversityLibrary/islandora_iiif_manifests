<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_newspaper_manifest(AbstractObject $object)
{
    module_load_include('inc', 'islandora', 'includes/utilities');
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    module_load_include('inc', 'islandora_newspaper', 'includes/utilities');



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


    // Get issues
    $issues = islandora_newspaper_get_issues($object);



    // Get pages
    $pages = islandora_paged_content_get_pages($object);




    // Don't create a manifest container for the compound parts
    $child_manifest_set = false;

    foreach ($issues as $issue){

        // Load issue
        $issue_object = islandora_object_load($issue['pid']);

        // Get the hooks to create the canvases
        $issue_hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFEST_HOOK, $issue_object->models);

        foreach ($issue_hooks as $page_hook) {

            $temp = module_invoke_all($page_hook, $issue_object);

            if (!empty($temp)) {
                // Check if the manifest container is set
                if(!$child_manifest_set){
                    $child_manifest_set = true;
                }
                else{
                    if(canvasHook($page_hook)){
                        // Add a canvas
                        $canvases[] = $temp;
                    }
                }
            }
        }
    }

    //Set Presentation information
    $manifest['viewingDirection'] = $viewingDirection;
    $manifest['viewingHint'] =  $viewingHint;

    // Set the canvases
    $manifest['sequences'][0]['canvases'] = $canvases;

    _islandora_iiif_create_manifest_response('200 OK', $manifest);

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


