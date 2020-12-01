<?php


/**
 * Build the manifest container.
 */
function islandora_iiif_manifests_create_manifest($object) {

    $id = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url'] ) . "/iiif_manifest/{$object->id}/manifest",
            array(
                'absolute' => TRUE,
                'language' => (object)array('language' => FALSE),
            )
        )
    );
  
    $manifest_type = islandora_iiif_manifests_get_manifest_type($object);

    $label = islandora_iiif_manifests_convert_label($object);
    $iiifmanifestlogourl = variable_get('islandora_iiif_manifests_logo_url');
    $scManifest = array(
        '@context' => 'http://iiif.io/api/presentation/2/context.json',
        '@id' => $id,
        '@type' => $manifest_type,
        'label' => $label,
        "logo" => $iiifmanifestlogourl,
        'metadata' => array(),
    );

    $metadata = islandora_iiif_manifests_get_metadata($object);

    if (isset($metadata['manifest'])) {

        $scManifest = array_merge_recursive($scManifest, $metadata['manifest']);
    }

    return $scManifest;
}

/**
 * Returns the appropriate manifest type for the compound.
 * If the compound only consists out of basic and large image, display the compound as a manifest.
 */
function islandora_iiif_manifests_compound_nanifest_type($object) {

    // Get compound parts.
    $parts = islandora_compound_object_get_parts($object->id);

    $arrayPartsCModel = array();

    foreach ($parts as $part) {

        // Load compound part.
        $part_object = islandora_object_load($part);
        $arrayPartsCModel = array_merge($arrayPartsCModel, $part_object->models);
    }

    $manifest_type = (count(array_intersect($arrayPartsCModel, array('islandora:bookCModel', 'islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:newspaperIssueCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
    return $manifest_type;
}

/**
 * Build the default canvas object
 */
function islandora_iiif_manifests_createDefaultCanvas($canvas_object) {

    $CanvasArray = array(
        '@type' => 'sc:Canvas',
        '@id' => $canvas_object['id'],
        'label' => $canvas_object['label'],
        'height' => $canvas_object['imageHeight'],
        'width' => $canvas_object['imageWidth'],
        'thumbnail' => array(
            '@id' => $canvas_object['thumbnail_uri'],
            'service' => array(
                '@context' => 'http://iiif.io/api/image/2/context.json',
                '@id' => $canvas_object['service_uri'],
                'profile' => 'http://iiif.io/api/image/2/level2.json'
            )
        ),
        'images' => array(
            array(
                '@id' => $canvas_object['service_uri'],
                '@type' => 'oa:Annotation',
                'motivation' => 'sc:painting',
                'resource' => array(
                    '@id' => $canvas_object['resource_uri'],
                    '@type' => 'dctypes:Image',
                    'height' => $canvas_object['imageHeight'],
                    'width' => $canvas_object['imageWidth'],
                    'service' => array(
                        '@context' => 'http://iiif.io/api/image/2/context.json',
                        '@id' => $canvas_object['service_uri'],
                        'profile' => 'http://iiif.io/api/image/2/level2.json',
                    )
                ),
                'on' => $canvas_object['id'],
            )
        )
    );

    return $CanvasArray;
}

/**
 * Build the page specific canvas object
 */
function islandora_iiif_manifests_get_object_canvas($object) {

    $image_info = islandora_iiif_manifests_get_image_info($object);

    $id = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url'] ) . "/iiif_manifest/{$object->id}/canvas/default", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
    )));

    $label = islandora_iiif_manifests_convert_label($object);

    $canvas_object = array();
    $canvas_object['id'] = $id;
    $canvas_object['label'] = $label;
    $canvas_object['thumbnail_uri'] = $image_info['thumbnail_uri'];
    $canvas_object['resource_uri'] = $image_info['resource_uri'];
    $canvas_object['service_uri'] = $image_info['service_uri'];
    $canvas_object['imageWidth'] = $image_info['imageWidth'];
    $canvas_object['imageHeight'] = $image_info['imageHeight'];

    $canvas = islandora_iiif_manifests_createDefaultCanvas($canvas_object);

    return $canvas;
}


function islandora_iiif_manifests_get_object_collection($object) {

    $collection_item = array();

    $manifest_uri = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url'] ) ."/iiif_manifest/{$object->id}/manifest", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
    )));
    
    $manifest_type = islandora_iiif_manifests_get_manifest_type($object);

    $collection_item['type'] = $manifest_type;
    $label = islandora_iiif_manifests_convert_label($object);

    $collection_item['object'] = array(
        "@id" => $manifest_uri,
        "@type" => $manifest_type,
        "label" => $label,
    );

    return $collection_item;
}

function islandora_iiif_manifests_get_newspaper_member_collection($issue) {

  $i = 0;
  $volume_issues = array();
  
  foreach ($issue as $key_month => $value_month) {
    
    // Level 0
    if (is_array($value_month)) {

      foreach ($value_month as $key_day => $value_day) {

        if (is_array($value_day)) {
          
          foreach ($value_day as $issue_data) {
            
            $issue_object = islandora_object_load($issue_data['pid']);
  
            if (islandora_iiif_manifests_allowed_manifest($issue_object)) {
  
              $manifest_type = islandora_iiif_manifests_get_manifest_type($issue_object);
              
              $manifest_uri = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url'] ) ."/iiif_manifest/{$issue_data['pid']}/manifest/", array(
                'absolute' => TRUE,
                'language' => (object)array('language' => FALSE),
              )));
              
              $d = new DateTime($issue_data['issued']->date);
              
              $volume_issues [$i] = array(
                "@id" => $manifest_uri,
                "@type" => $manifest_type,
                "label" => islandora_iiif_manifests_convert_label($issue_object)." ".$d->format('D j F Y'),
                "navDate" => $d->format('c'),
              );
              $i++;
            }
            
            
          }
        }
      }
    }
    
  }

  return $volume_issues ;
}



function islandora_iiif_manifests_allowed_cmodel($object) {

    $cmodels = array('islandora:sp_basic_image', 'islandora:sp_large_image_cmodel', 'islandora:bookCModel', 'islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:newspaperIssueCModel', 'islandora:compoundCModel');

    if (count(array_intersect($object->models, $cmodels)) > 0) {
        return TRUE;
    }
    return FALSE;
}

function islandora_iiif_manifests_allowed_canvas_cmodel($object) {

    $cmodels = array('islandora:sp_basic_image', 'islandora:sp_large_image_cmodel', 'islandora:pageCModel', 'islandora:newspaperIssueCModel');

    if (count(array_intersect($object->models, $cmodels)) > 0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Check if the object already has manifest or access is allowed
 * @param $object
 * @return bool
 */
function islandora_iiif_manifests_allowed_manifest($object){

    if (isset($object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID])) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/**
 * Return manifest type: Collection or Manifest
 * @param $object
 * @return string
 */
function islandora_iiif_manifests_get_manifest_type($object){
  
  if ((count(array_intersect($object->models, array('islandora:compoundCModel'))) > 0)) {
    $manifest_type = islandora_iiif_manifests_compound_nanifest_type($object);
  }
  else {
    // Set manifest type.
    $manifest_type = (count(array_intersect($object->models, array('islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
  }
  
  return $manifest_type;
}

/**
 * Convert numeric label to a string
 */
function islandora_iiif_manifests_convert_label($object) {

    if (is_numeric($object->label)) {
        $label = "'" . $object->label . "'";
    }
    else {
        $label = $object->label;
    }

    return $label;
}

/**
 * Retrieve object metadata
 */
function islandora_iiif_manifests_get_metadata($object) {

    $metadata = array();
    if (isset($object['MODS'])) {
        $modsxml = $object['MODS']->content;
        $xslpath = drupal_get_path('module', 'islandora_iiif_manifests') . '/xsl/mods2manifest.xsl';
        $xsl = new DOMDocument();

        $xsl->load($xslpath);
        $mods = new DOMDocument();
        $mods->loadXML($modsxml);
        $processor = new XSLTProcessor();
        $processor->importStylesheet($xsl);
        $manifest = $processor->transformToDoc($mods);
        $metadata = islandora_iiif_manifests_xml_to_array($manifest);

    }
    return $metadata;
}

/**
 * Retrieve the image width and height
 */
function islandora_iiif_manifests_get_image_info($object) {

    module_load_include('inc', 'islandora_handle', 'includes/handle');

    $handler_class = islandora_handle_retrieve_selected_handler();
    $handler = new $handler_class($object);
    $handle = $handler->getFullHandle($object);

    $encoded_handle = urlencode( 'hdl:'.$handle);

    $imageArray = array();

    $islandora_iiif_manifest_image_uri = url(variable_get('islandora_iiif_manifests_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2') . "/" . $encoded_handle,
        array(
            'absolute' => TRUE,
            'language' => (object)array('language' => FALSE),
        )
    );

    $id_uri = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url'] ) . "/iiif_manifest/{$object->id}/annotation/{$object->label}", array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
    )));

    $imageArray['thumbnail_uri'] = $islandora_iiif_manifest_image_uri . "/full/80,/0/default.jpg";
    $imageArray['resource_uri'] = $islandora_iiif_manifest_image_uri . "/full/pct:50/0/default.jpg";
    $imageArray['service_uri'] = $islandora_iiif_manifest_image_uri;
    $imageArray['id_uri'] = $id_uri;


    if ($object['TECHMD']) {
        // Get Image info from TECHMD.
        $mods_datastream = $object['TECHMD']->content;
        $xml = simplexml_load_string($mods_datastream);
        $json = json_encode($xml);
        $tech_md = json_decode($json, TRUE);

        if (isset($tech_md['metadata']['image']['imageWidth']) && !is_array($tech_md['metadata']['image']['imageWidth'])) {
            // Set width and height
            $imageArray['imageWidth'] = $tech_md['metadata']['image']['imageWidth'];
            $imageArray['imageHeight'] = $tech_md['metadata']['image']['imageHeight'];
        }

    }
    if (!isset($imageArray['imageWidth'])) {

        $temp_file = drupal_tempnam('temporary://', 'dimensions');
        $object['OBJ']->getContent($temp_file);
        list($width, $height) = getimagesize($temp_file);

        $imageArray['imageWidth'] = $width;
        $imageArray['imageHeight'] = $height;

        @unlink($temp_file);
    }
    return $imageArray;
}

/**
 * Retrieve the view direction from the book
 */
function islandora_iiif_manifests_get_viewing_direction($object) {

    $page_progression = islandora_paged_content_get_page_progression($object);
    $viewingDirection = "";
    switch ($page_progression) {

        case 'lr':
            $viewingDirection = "left-to-right";
            break;

        case 'rl':
            $viewingDirection = "right-to-left";
            break;
    }
    return $viewingDirection;

}

function islandora_iiif_manifests_xml_to_array($root) {
    $result = array();

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
                $result[$child->nodeName] = islandora_iiif_manifests_xml_to_array($child);
            }
            else {
                if (!isset($groups[$child->nodeName])) {
                    $result[$child->nodeName] = array($result[$child->nodeName]);
                    $groups[$child->nodeName] = 1;
                }
                $result[$child->nodeName][] = islandora_iiif_manifests_xml_to_array($child);
            }
        }
    }


    return $result;
}

/**
 * Helper function to write out a JSON response.
 */
function islandora_iiif_manifests_create_json_response($status, $data = NULL) {
    drupal_add_http_header('Status', $status);
    drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
    drupal_add_http_header('access-control-allow-origin', '*');
    if ($data !== NULL) {
        echo $data;
    }
    exit();
}

/**
 * Create IIIF manifest datastream.
 */
function islandora_iiif_manifests_add_manifest_datastream($object, $manifest) {
    $dsid = 'IIIF';
    // Before we do anything, check if the datastream exists. If it does, load it; otherwise construct it.
    // The easiest way to do this, as opposed to a string of cases or if/then/elses, is the ternary operator, e.g.
    $datastream = isset($object[$dsid]) ? $object[$dsid] : $object->constructDatastream($dsid);

    $datastream->label = 'IIIF Manifest';
    $datastream->mimeType = 'application/json';
    $datastream->setContentFromString($manifest);
    // Ingest the datastream if it isn't yet.
    if (!isset($object['IIIF'])) {
        $object->ingestDatastream($datastream);
    }
}


