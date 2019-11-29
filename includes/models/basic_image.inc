<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_basic_image_canvas(AbstractObject $object)
{
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/createcanvas');
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');

    //TODO: Handle should be retrieved from the object. Currenty 1887.1 is the hard coded default

    // Get the Handle
    $handle = variable_get('islandora_iiif_manifests_object_handle', 'hdl:1887.a');

    /*$image_info = getImageInfo($object);

    // Set width and height
    $imageWidth = $image_info['width'];
    $imageHeight = $image_info['height'];*/

    // Retrieve image width and height using the islandora_handled_access module
    $full_image_size_url = url("islandora/handled_access/{$handle}/{$object->id}/image/view", array('absolute' => TRUE));
    list($width, $height, $mimetype, $attr) = getimagesize($full_image_size_url);

    // Set the handle ready for URL
    $object_handle = "/".$handle."%2F";

    $islandora_iiif_manifest_image_uri = url(variable_get('islandora_iiif_manifests_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2').$object_handle,
        array(
            'absolute' => TRUE,
            'language' => (object) array('language' => FALSE),
        )
    );

    $canvas_uri = urldecode(url("islandora_iiif_manifests/{$object->id}/canvas/default", array(
        'absolute' => TRUE,
        'language' => (object) array('language' => FALSE),
    )));

    $thumbnail_uri = $islandora_iiif_manifest_image_uri."$object->id/full/80,/0/default.jpg";
    $resource_uri = $islandora_iiif_manifest_image_uri."$object->id/full/full/0/default.jpg";
    $service_uri = $islandora_iiif_manifest_image_uri."$object->id";

    // Create the basic image canvas
    $canvas = new createcanvas($canvas_uri, $object->label, $thumbnail_uri);
    $canvas->setResource($resource_uri, $service_uri, $width, $height);


    return $canvas->toArray();

}
