<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_large_image_canvas(AbstractObject $object)
{
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/createcanvas');

    // Get the Handle
    $handle = variable_get('islandora_iiif_manifests_object_handle', 'hdl:1887.a');

    // Get Image info from TECHMD
    $mods_datastream = $object['TECHMD']->content;
    $xml = simplexml_load_string($mods_datastream);
    $json  = json_encode($xml);
    $tech_md = json_decode($json, true);

    echo "<pre>";print_r($tech_md);die;
    // Set width and height
    $imageWidth = $tech_md['metadata']['image']['imageWidth'];
    $imageHeight = $tech_md['metadata']['image']['imageHeight'];


    // Retrieve image width and height using the islandora_handled_access module
    //$full_image_size_url = url("islandora/handled_access/{$handle}/{$object->id}/image/view", array('absolute' => TRUE));
    //list($width, $height, $mimetype, $attr) = getimagesize($full_image_size_url);

    // Set the handle ready for URL
    $object_handle = "/".$handle."%2F";

    $islandora_iiif_manifest_image_uri = url(variable_get('islandora_iiif_manifests_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2').$object_handle,
        array(
            'absolute' => TRUE,
            'language' => (object) array('language' => FALSE),
        )
    );
    //echo $islandora_iiif_manifest_image_uri;die;
    $canvas_uri = urldecode(url("islandora_iiif_manifests/{$object->id}/canvas/default", array(
        'absolute' => TRUE,
        'language' => (object) array('language' => FALSE),
    )));

    $thumbnail_uri = $islandora_iiif_manifest_image_uri."$object->id/full/80,/0/default.jpg";
    $resource_uri = $islandora_iiif_manifest_image_uri."$object->id/full/full/0/default.jpg";
    $service_uri = $islandora_iiif_manifest_image_uri."$object->id";

    // Create the basic image canvas
    $canvas = new createcanvas($canvas_uri, $object->label, $thumbnail_uri);
    $canvas->setResource($resource_uri, $service_uri, $imageWidth, $imageHeight);


    return $canvas->toArray();

}
