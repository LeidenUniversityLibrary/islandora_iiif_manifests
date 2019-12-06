<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_collection_manifest(AbstractObject $object)
{
    module_load_include('inc', 'islandora', 'includes/utilities');
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
    module_load_include('module', 'islandora_basic_collection');

    $islandora_iiif_manifest_image_uri = url(variable_get('islandora_iiif_manifests_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2').$object_handle,
        array(
            'absolute' => TRUE,
            'language' => (object) array('language' => FALSE),
        )
    );

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


    // Change manifest to collection
    $manifest['@type'] = "sc:Collection";

    // remove sequences from the manifests
    unset($manifest['sequences']);


    // Get collection members
    $collection_members = islandora_basic_collection_get_member_objects($object);
    $collection_objects = $collection_members[1];

    // collection of the manifests
    $collection_members_manifests = array();
    $collection_members_collectons = array();

    // Don't create a manifest container for the compound parts
    $child_manifest_set = false;

    foreach ($collection_objects as $collection_object){



        // Load collection object
        $fedora_collection_object = islandora_object_load($collection_object['object']['value']);
        $models = $fedora_collection_object->models;
        $thumbnail_uri = $islandora_iiif_manifest_image_uri."$fedora_collection_object->id/full/80,/0/default.jpg";
        $service_uri = $islandora_iiif_manifest_image_uri."$fedora_collection_object->id";

        $manifest_uri = urldecode(url("islandora_iiif_manifests/{$fedora_collection_object->id}/manifest", array(
            'absolute' => TRUE,
            'language' => (object) array('language' => FALSE),
        )));

        $label = $fedora_collection_object->label;
        $thumb = array(
        "@id" => $thumbnail_uri,
        "service" => array(
          "@context" => "http://iiif.io/api/image/2/context.json",
          "@id" => $service_uri,
          "profile" => "http://iiif.io/api/image/2/level2.json",
            ),
        );
        if(in_array('islandora:collectionCModel', $models)){
            $type = "sc:Collection";
            $collection_members_collectons[] = array(
                "@id" => $manifest_uri,
                "@type" => $type,
                "label" => $label,
                "thumbnail" => $thumb,
            );
        }
        else {

            $type = "sc:Manifest";
            $collection_members_manifests[] = array(
                "@id" => $manifest_uri,
                "@type" => $type,
                "label" => $label,
                "thumbnail" => $thumb,
            );
        }

        $collection_members_members[] = array(
            "@id" => $manifest_uri,
            "@type" => $type,
            "label" => $label,
            "thumbnail" => $thumb,
        );



        /*  // Get the hooks to create the canvases
         $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFEST_HOOK, $fedora_collection_object->models);
         //echo "Hooks <pre>:";print_r($hooks);die;
         foreach ($hooks as $part_hook) {

             $temp = module_invoke_all($part_hook, $fedora_collection_object);


             if (!empty($temp)) {
                 // Check if the manifest container is set
                 if(!$child_manifest_set){
                     $child_manifest_set = true;
                 }
                 else{
                     if(canvasHook($part_hook)){
                         // Add a canvas
                         $canvases[] = $temp;//array_merge_recursive($children_output, $temp);
                     }
                 }
             }
         }*/
    }
    if (!empty($collection_members_manifests)) {
        $manifest['manifests'] = $collection_members_manifests;
    }
    if (!empty( $collection_members_collectons)) {
        $manifest['collections'] = $collection_members_collectons;
    }
    if (!empty( $collection_members_members)) {
        $manifest['members'] = $collection_members_members;
    }

    _islandora_iiif_create_manifest_response('200 OK', $manifest);

}
/*
function getCollectionMembers(){

    $label = $children_info['object_label'];
    $thumb = array(
        "@id" => urldecode(url("islandora_iiif/{$object->id}/full/50,/0/default.jp2", array(
            'absolute' => TRUE,
            'language' => (object) array('language' => FALSE),
        ))),
        "service" => array(
            "@context" => "http://iiif.io/api/image/2/context.json",
            "@id" => urldecode(url("islandora_iiif/{$object->id}/info.json", array(
                'absolute' => TRUE,
                'language' => (object) array('language' => FALSE),
            ))),
            "profile" => "http://iiif.io/api/image/2/level2.json",
        ),
    );
}*/

