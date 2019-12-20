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

/**
 * Build the manifest container
 */

function getContainer($object) {

    $id = urldecode(url(current_path(),
            array(
                'absolute' => TRUE,
                'language' => (object) array('language' => FALSE),
            )
        )
    );

    $scManifest = array (
        '@context' => 'http://iiif.io/api/presentation/2/context.json',
        '@id' => $id,//$this->id,
        '@type' => 'sc:Manifest',
        'label' => $object->label,
        "logo" => "https://kerkuil-a.leidenuniv.nl/iiif/ubl_logo.png",
        "attribution" => "This manifest was created from Islandora. Contact r.k.s.mwinyimbegu@library.leidenuniv.nl",
        "description" => $object->label,
        'sequences' => array ( // an array but will always be a single object in our application
            array (
                '@type' => 'sc:Sequence',
                'label' => $object->label,
                'canvases' => array()
            )
        )
    );

    $metadata = getMetadata($object);
    //echo "<pre>";print_r(getMetadata($object));die;
    if (isset($metadata['manifest'])) {
        $scManifest = array_merge_recursive($scManifest, $metadata['manifest']);
    }

    //return json_encode ( $scManifest );
    return $scManifest;
}


function getMetadata($object) {

    $metadata = array();
    if (isset($object['MODS'])) {
        $modsxml = $object['MODS']->content;
        $xslpath = drupal_get_path('module', 'islandora_iiif_manifests') . '/xsl/mods2manifest.xsl';
        $xsl = new DOMDocument();
        //echo $xslpath;die;
        $xsl->load($xslpath);
        $mods = new DOMDocument();
        $mods->loadXML($modsxml);
        $processor = new XSLTProcessor();
        $processor->importStylesheet($xsl);
        $manifest = $processor->transformToDoc($mods);
        $metadata = $this->xml_to_array($manifest);

        //echo "here";die;
    }
    return $metadata;
}

function xml_to_array($root) {
    $result = array();
    try{

        if ($root->hasAttributes()) {
            $attrs = $root->attributes;
            foreach ($attrs as $attr) {
                $result['@' . $attr->name] = $attr->value;
            }
        }

        if ($root->hasChildNodes()) {
            $children = $root->childNodes;
            if ($children->length == 1) {
                $child = $children->item(0);
                if ($child->nodeType == XML_TEXT_NODE) {
                    $result['_value'] = $child->textContent;
                    return count($result) == 1
                        ? $result['_value']
                        : $result;
                }
            }
            $groups = array();
            foreach ($children as $child) {
                if (!isset($result[$child->nodeName])) {
                    $result[$child->nodeName] = $this->xml_to_array($child);
                } else {
                    if (!isset($groups[$child->nodeName])) {
                        $result[$child->nodeName] = array($result[$child->nodeName]);
                        $groups[$child->nodeName] = 1;
                    }
                    $result[$child->nodeName][] = $this->xml_to_array($child);
                }
            }
        }

    }catch (Exception $e){
        echo $e->getMessage(); die;
    }

    return $result;
}



