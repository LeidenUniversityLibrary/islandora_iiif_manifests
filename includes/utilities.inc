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


    $imageWidth = 0;
    $imageHeight = 0;



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

