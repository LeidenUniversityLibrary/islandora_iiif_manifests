<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_basic_image_manifest(AbstractObject $object)
{
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/createcanvas');

    // Retrieve image width and height using the islandora_handled_access module
    $full_image_size_url = url("islandora/handled_access/1887.1/{$object->id}/image/view", array('absolute' => TRUE));
    list($width, $height, $mimetype, $attr) = getimagesize($full_image_size_url);

    $islandora_iiif_manifest_image_uri = url(variable_get('islandora_sc_manifest_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2/hdl:1887.1%2F'),
        array(
            'absolute' => TRUE,
            'language' => (object) array('language' => FALSE),
        )
    );

    $canvas_uri = urldecode(url("islandora_iiif_manifests/{$object->id}/canvas/default", array(
        'absolute' => TRUE,
        'language' => (object) array('language' => FALSE),
    )));

    $thumbnail_uri = $islandora_iiif_manifest_image_uri."hdl:1887.1%2F$object->id/full/80,/0/default.jpg";
    $resource_uri = $islandora_iiif_manifest_image_uri."hdl:1887.1%2F$object->id/full/full/0/default.jpg";
    $service_uri = $islandora_iiif_manifest_image_uri."hdl:1887.1%2F$object->id";

    // Create the basic image canvas
    $canvas = new createcanvas($canvas_uri, $object->label, $thumbnail_uri);
    $canvas->setResource($resource_uri, $service_uri, $mimetype, $width, $height);


    return $canvas->toArray();

}
