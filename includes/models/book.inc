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
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');


    $viewingHint = islandora_paged_content_get_viewing_hint($object) !="" ?  islandora_paged_content_get_viewing_hint($object):"paged";
    $viewingDirection = getViewingDirection($object);

    // Manifest array
    $manifest = array();

    // Canvases
    $canvases = array();

    // Get pages
    $pages = islandora_paged_content_get_pages($object);

    foreach ($pages as $page){

        // Get page object
        $page_object = islandora_object_load($page['pid']);

        // Add page to canvases
        $canvases[] = getObjectCanvas($page_object);
    }

    //Set Presentation information
    $manifest['viewingDirection'] = $viewingDirection;
    $manifest['viewingHint'] =  $viewingHint;

    $label = convertLabel($object);

    // Set the canvases
    $manifest['sequences'][] = array (
        '@type' => 'sc:Sequence',
        'label' => $label,
        'canvases' => $canvases
    );

    return $manifest;

}

