<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_page_canvas(AbstractObject $object)
{
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/createcanvas');

    //TODO: Handle should be retrieved from the object. Currenty 1887.1 is the hard coded default

    // Get the Handle
    $handle = variable_get('islandora_iiif_manifests_object_handle', '1887.1');

    $image_info = getImageInfo($object);

    // Set width and height
    $imageWidth = $image_info['imageWidth'];
    $imageHeight = $image_info['imageHeight'];

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
    $canvas->setResource($resource_uri, $service_uri, $imageWidth, $imageHeight);


    return $canvas->toArray();

}
