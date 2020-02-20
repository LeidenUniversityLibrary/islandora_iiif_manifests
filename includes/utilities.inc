<?php


/**
 * Build the manifest container
 */
function createManifest($object) {

  $id = urldecode(url(current_path(),
      array(
        'absolute' => TRUE,
        'language' => (object)array('language' => FALSE),
      )
    )
  );
  if ((count(array_intersect($object->models, array('islandora:compoundCModel'))) > 0)) {

    $manifest_type = compoundManifestType($object);

  } else {
    // Set manifest type
    $manifest_type = (count(array_intersect($object->models, array('islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
  }


  $label = convertLabel($object);

  $scManifest = array(
    '@context' => 'http://iiif.io/api/presentation/2/context.json',
    '@id' => $id,//$this->id,
    '@type' => $manifest_type,
    'label' => $label,
    "logo" => "https://kerkuil-a.leidenuniv.nl/iiif/ubl_logo.png",
    'metadata' => array()
  );

  $metadata = getMetadata($object);

  if (isset($metadata['manifest'])) {

    $scManifest = array_merge_recursive($scManifest, $metadata['manifest']);
  }

  return $scManifest;
}

/**
 * Returns the appropriate manifest type for the compound.
 * If the compound only consists out of basic and large image, display the compound as a manifest.
 */
function compoundManifestType($object) {

  // Get compound parts
  $parts = islandora_compound_object_get_parts($object->id);

  $arrayPartsCModel = array();

  foreach ($parts as $part) {

    // Load compound part
    $part_object = islandora_object_load($part);
    $arrayPartsCModel = array_merge($arrayPartsCModel, $part_object->models);
  }

  $manifest_type = (count(array_intersect($arrayPartsCModel, array('islandora:bookCModel', 'islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:newspaperIssueCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
  return $manifest_type;
}

/**
 * Build the default canvas object
 */
function createDefaultCanvas($canvas_object) {

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
    'images' => array( // always contain only one
      array(
        '@id' => $canvas_object['service_uri'],
        '@type' => 'oa:Annotation',
        'motivation' => 'sc:Painting',
        'resource' => array(
          '@id' => $canvas_object['resource_uri'],
          '@type' => 'dctypes:Image',
          //'format' => $this->image_format,
          'height' => $canvas_object['imageHeight'],
          'width' => $canvas_object['imageWidth'],
          'service' => array(
            '@context' => 'http://iiif.io/api/image/2/context.json',
            '@id' => $canvas_object['service_uri'],
            'profile' => 'http://iiif.io/api/image/2/level2.json'
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
function getObjectCanvas($object) {

  $image_info = getImageInfo($object);

  $id = urldecode(url("islandora_iiif_manifests/{$object->id}/canvas/default", array(
    'absolute' => TRUE,
    'language' => (object)array('language' => FALSE),
  )));

  $label = convertLabel($object);

  $canvas_object = array();
  $canvas_object['id'] = $id;
  $canvas_object['label'] = $label;
  $canvas_object['thumbnail_uri'] = $image_info['thumbnail_uri'];
  $canvas_object['resource_uri'] = $image_info['resource_uri'];
  $canvas_object['service_uri'] = $image_info['service_uri'];
  $canvas_object['imageWidth'] = $image_info['imageWidth'];
  $canvas_object['imageHeight'] = $image_info['imageHeight'];

  $canvas = createDefaultCanvas($canvas_object);

  return $canvas;
}

function getObjectCollection($object) {

  $collection_item = array();

  $models = $object->models;

  $manifest_uri = urldecode(url("islandora_iiif_manifests/{$object->id}/manifest", array(
    'absolute' => TRUE,
    'language' => (object)array('language' => FALSE),
  )));

  if ((count(array_intersect($object->models, array('islandora:compoundCModel'))) > 0)) {

    $manifest_type = compoundManifestType($object);

  } else {
    // Set manifest type
    $manifest_type = (count(array_intersect($object->models, array('islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
  }

  $collection_item['type'] = $manifest_type;
  $label = convertLabel($object);

  $collection_item['object'] = array(
    "@id" => $manifest_uri,
    "@type" => $manifest_type,
    "label" => $label,
  );

  return $collection_item;
}

function allowedCModel($object) {

  $cmodels = array('islandora:sp_basic_image', 'islandora:sp_large_image_cmodel', 'islandora:bookCModel', 'islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:newspaperIssueCModel', 'islandora:compoundCModel');

  if (count(array_intersect($object->models, $cmodels)) > 0) {
    return true;
  }
  return false;
}

function allowedCanvasCModel($object) {

  $cmodels = array('islandora:sp_basic_image', 'islandora:sp_large_image_cmodel', 'islandora:pageCModel', 'islandora:newspaperIssueCModel');

  if (count(array_intersect($object->models, $cmodels)) > 0) {
    return true;
  }
  return false;
}

/**
 * Convert numeric label to a string
 */
function convertLabel($object) {

  if (is_numeric($object->label)) {
    $label = "'" . $object->label . "'";
  } else {
    $label = $object->label;
  }

  return $label;
}

/**
 * Retrieve object metadata
 */
function getMetadata($object) {

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
    $metadata = xml_to_array($manifest);

  }
  return $metadata;
}

/**
 * Retrieve the image width and height
 */
function getImageInfo($object) {

  module_load_include('inc', 'islandora_handle', 'includes/handle');

  $handler_class = islandora_handle_retrieve_selected_handler();
  $handler = new $handler_class($object);
  $handle = $handler->getFullHandle($object);
// Get the Handle
  if (TRUE) {
    $handle_data = explode("/", $handle);
    $handle_admin = variable_get('islandora_iiif_manifests_object_handle', 'hdl:1887.a');
    $handle = $handle_admin . "/" . $handle_data[1];
  }
  $encoded_handle = urlencode($handle);

  $imageArray = array();

  $islandora_iiif_manifest_image_uri = url(variable_get('islandora_iiif_manifests_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2') . "/" . $encoded_handle,
    array(
      'absolute' => TRUE,
      'language' => (object)array('language' => FALSE),
    )
  );

  $imageArray['thumbnail_uri'] = $islandora_iiif_manifest_image_uri . "/full/80,/0/default.jpg";
  $imageArray['resource_uri'] = $islandora_iiif_manifest_image_uri . "/full/pct:50/0/default.jpg";
  $imageArray['service_uri'] = $islandora_iiif_manifest_image_uri;


  if ($object['TECHMD']) {
    // Get Image info from TECHMD
    $mods_datastream = $object['TECHMD']->content;
    $xml = simplexml_load_string($mods_datastream);
    $json = json_encode($xml);
    $tech_md = json_decode($json, true);

    if (isset($tech_md['metadata']['image']['imageWidth'])) {
      // Set width and height
      $imageArray['imageWidth'] = $tech_md['metadata']['image']['imageWidth'];
      $imageArray['imageHeight'] = $tech_md['metadata']['image']['imageHeight'];
    } else {
      // Retrieve image width and height using the islandora_handled_access module
      $full_image_size_url = url("islandora/handled_access/{$encoded_handle}/image/view", array('absolute' => TRUE));
      list($width, $height, $mimetype, $attr) = getimagesize($full_image_size_url);

      $imageArray['imageWidth'] = $width;
      $imageArray['imageHeight'] = $height;
    }

  } else {

    // Retrieve image width and height using the islandora_handled_access module
    $full_image_size_url = url("islandora/handled_access/{$encoded_handle}/image/view", array('absolute' => TRUE));
    list($width, $height, $mimetype, $attr) = getimagesize($full_image_size_url);

    $imageArray['imageWidth'] = $width;
    $imageArray['imageHeight'] = $height;
  }
  return $imageArray;
}

/**
 * Retrieve the view direction from the book
 */
function getViewingDirection($object) {

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

function xml_to_array($root) {
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
        $result[$child->nodeName] = xml_to_array($child);
      } else {
        if (!isset($groups[$child->nodeName])) {
          $result[$child->nodeName] = array($result[$child->nodeName]);
          $groups[$child->nodeName] = 1;
        }
        $result[$child->nodeName][] = xml_to_array($child);
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
function islandora_iiif_manifests_add_manifest_datastream($object,$manifest){
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



