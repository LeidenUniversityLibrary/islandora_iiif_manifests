<?php

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

function getImageInfo($object){


    $imageWidth = 2000;
    $imageHeight = 2000;



    if($object['TECHMD']){
        // Get Image info from TECHMD
        $mods_datastream = $object['TECHMD']->content;
        $xml = simplexml_load_string($mods_datastream);
        $json  = json_encode($xml);
        $tech_md = json_decode($json, true);


        // Set width and height
        $imageWidth = $tech_md['metadata']['image']['imageWidth'];
        $imageHeight = $tech_md['metadata']['image']['imageHeight'];
    }


    return array("width"=>$imageWidth,"height"=>$imageHeight);
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

/**
 * Helper function to write out a JSON response.
 */
function json_response($status, $data = NULL) {
    drupal_add_http_header('Status', $status);
    drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
    drupal_add_http_header('access-control-allow-origin', '*');
    if ($data !== NULL) {
        echo json_encode($data, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK). "\n";;
        print drupal_json_encode($data);
    }
    exit();
}



