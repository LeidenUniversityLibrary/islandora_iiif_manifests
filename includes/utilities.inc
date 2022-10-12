<?php

/**
 * @file
 */

/**
 * Build the manifest container.
 * @return array Manifest as an array
 */
function islandora_iiif_manifests_create_manifest($object) {
  
  $id = url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url']) . "/iiif_manifest/{$object->id}/manifest",
    array(
      'absolute' => TRUE,
      'language' => (object)array('language' => FALSE),
    )
  );
  if (islandora_iiif_manifests_in_multiarray('islandora:compoundCModel', $object->models)) {
    $manifest_type = islandora_iiif_manifests_compound_manifest_type($object);
  }
  else {
    // Set manifest type.
    $manifest_type = (count(array_intersect($object->models, array('islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
  }
  
  
  $label = islandora_iiif_manifests_convert_label($object);
  $iiifmanifestlogourl = variable_get('islandora_iiif_manifests_logo_url');
  $sc_manifest = array(
    '@context' => 'http://iiif.io/api/presentation/2/context.json',
    '@id' => $id,
    '@type' => $manifest_type,
    'label' => $label,
    "logo" => $iiifmanifestlogourl,
    'metadata' => array(),
  );
  
  $metadata = islandora_iiif_manifests_get_metadata($object);
  if (isset($metadata['manifest'])) {
    
    // Add the issued date to the label if newspaperIssueCModel
    $metadata = islandora_iiif_manifests_add_issued_date($object, $metadata);
    
    $sc_manifest = array_merge_recursive($sc_manifest, $metadata['manifest']);
  }
  else {
    unset($sc_manifest['metadata']);
  }
  
  return $sc_manifest;
}

/**
 * Returns the appropriate manifest type for the compound.
 * If the compound only consists out of basic and large image, display the compound as a manifest.
 * @return string
 */
function islandora_iiif_manifests_compound_manifest_type($object) {
  
  // Get compound parts.
  $parts = islandora_compound_object_get_parts($object->models);
  
  $array_parts_cmodel = array();
  
  foreach ($parts as $part) {
    
    // Load compound part.
    $part_object = islandora_object_load($part);
    $array_parts_cmodel = array_merge($array_parts_cmodel, $part_object->models);
  }
  
  $manifest_type = (count(array_intersect($array_parts_cmodel, array('islandora:bookCModel', 'islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:newspaperIssueCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
  
  return $manifest_type;
}

/**
 * Build the default canvas object
 * @return array Array representation of a Canvas
 */
function islandora_iiif_manifests_createDefaultCanvas($canvas_object) {
  
  $canvas_array = array(
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
        '@id' => $canvas_object['id_uri'],
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
  
  return $canvas_array;
}

/**
 * Build the page specific canvas object
 * @return array Array representation of a Canvas
 */
function islandora_iiif_manifests_get_object_canvas($object) {
  
  $image_info = islandora_iiif_manifests_get_image_info($object);
  
 
  $canvas = array();
  
  // If imageWidth is not set, then the image file is incorrect. Return an empty canvas array
  if(!isset($image_info['imageWidth'])) {
  
    $id = url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url']) . "/iiif_manifest/{$object->id}/canvas/default", array(
      'absolute' => TRUE,
      'language' => (object)array('language' => FALSE),
    ));

    $label = islandora_iiif_manifests_convert_label($object);
  
    $canvas_object = array();
    $canvas_object['id'] = $id;
    $canvas_object['label'] = $label;
    $canvas_object['thumbnail_uri'] = $image_info['thumbnail_uri'];
    $canvas_object['resource_uri'] = $image_info['resource_uri'];
    $canvas_object['service_uri'] = $image_info['id_uri'];
    $canvas_object['imageWidth'] = intval($image_info['imageWidth']);
    $canvas_object['imageHeight'] = intval($image_info['imageHeight']);
  
    $canvas = islandora_iiif_manifests_createDefaultCanvas($canvas_object);
  }
  
  return $canvas;
}

function islandora_iiif_manifests_get_object_collection($solrobj) {
  
  $manifest_uri = urldecode(url("iiif_manifest/{$solrobj['PID']}/manifest", array(
    'absolute' => TRUE,
    'language' => (object)array('language' => FALSE),
  )));
  
  $manifest_type = $solrobj['solr_doc']['json_MANIFEST_json_type_ms'][0];
  
  $label = $solrobj['object_label'];
  
  $collection_member = array(
    "@id" => $manifest_uri,
    "@type" => $manifest_type,
    "label" => $label,
  );
  
  return $collection_member;
}

/**
 * Loop through newspaper volumes. Get issue data for each day with the volumes.
 */
function islandora_iiif_manifests_get_newspaper_member_collection($issue) {
  
  $i = 0;
  $volume_issues = array();
  
  foreach ($issue as $key_month => $value_month) {
    
    // Check if there are issues in this month
    if (is_array($value_month)) {
      
      // Loop through available days
      foreach ($value_month as $key_day => $value_day) {
        
        // Check if there are issues.
        if (is_array($value_day)) {
          
          // Get values for each value on this day
          foreach ($value_day as $issue_data) {
            
            $issue_object = islandora_object_load($issue_data['pid']);
            
            if (isset($issue_object[ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID])) {
              
              $manifest_type = (count(array_intersect($issue_object->models, array('islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:compoundCModel'))) > 0) ? "sc:Collection" : "sc:Manifest";
              
              $manifest_uri = urldecode(url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url']) . "/iiif_manifest/{$issue_data['pid']}/manifest/", array(
                'absolute' => TRUE,
                'language' => (object)array('language' => FALSE),
              )));
              
              // Set issue manifest data
              $volume_issues[$i] = array(
                "@id" => $manifest_uri,
                "@type" => $manifest_type,
                "label" => islandora_iiif_manifests_convert_label($issue_object),
                "navDate" => $issue_data['issued']->format('Y-m-d\TH:i:s\Z\''),
              );
              $i++;
            }
            
          }
        }
      }
    }
    
  }
  
  return $volume_issues;
}

/**
 * Determines if a Manifest can be created for the object's CModel(s)
 * @return bool
 */
function islandora_iiif_manifests_allowed_cmodel($object) {
  
  $cmodels = array('islandora:sp_basic_image', 'islandora:sp_large_image_cmodel', 'islandora:bookCModel', 'islandora:newspaperCModel', 'islandora:collectionCModel', 'islandora:newspaperIssueCModel', 'islandora:compoundCModel');
  
  if (count(array_intersect($object->models, $cmodels)) > 0) {
    return TRUE;
  }
  
  return FALSE;
}

/**
 * Determines if a Canvas can be created for the object's CModel(s)
 * @return bool
 */
function islandora_iiif_manifests_allowed_canvas_cmodel($object) {
  
  $cmodels = array('islandora:sp_basic_image', 'islandora:sp_large_image_cmodel', 'islandora:pageCModel', 'islandora:newspaperIssueCModel');
  
  if (count(array_intersect($object->models, $cmodels)) > 0) {
    return TRUE;
  }
  
  return FALSE;
}

/**
 * Convert numeric label to a string
 * @return string
 */
function islandora_iiif_manifests_convert_label($object) {
  
  $label = $object->label;
  
  // Check if cmodels is a newspaper issue.
  if (count(array_intersect($object->models, array('islandora:newspaperIssueCModel'))) > 0) {
    $date_issued = islandora_newspaper_get_date_issued_from_mods($object['MODS']);
    $label = $label . " - " . $date_issued->format('D j F Y');
  }
  
  
  return $label;
}

/**
 * Add the issued date to the metadata label if object models contains newspaperIssueCModel
 * @param $object
 * @param $metadata
 * @return mixed
 */
function islandora_iiif_manifests_add_issued_date($object, $metadata) {
  
  // Check if cmodels is a newspaper issue.
  if (count(array_intersect($object->models, array('islandora:newspaperIssueCModel'))) > 0) {
    $date_issued = islandora_newspaper_get_date_issued_from_mods($object['MODS']);
    
    $i = 0;
    foreach ($metadata['manifest']['metadata'] as $metadata_part) {
      
      if ($metadata_part['label'] === 'Title') {
        $metadata['manifest']['metadata'][$i]['value'] = $metadata_part['value'] . " - " . $date_issued->format('D j F Y');
      }
      $i++;
    }
  }
  
  return $metadata;
}

/**
 * Retrieve object metadata from MODS.
 * @return array
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
 * Retrieve the image width and height, and URIs
 * @return array Array of (technical) image information
 */
function islandora_iiif_manifests_get_image_info($object) {
  
  module_load_include('inc', 'islandora_handle', 'includes/handle');
  
  $handler_class = islandora_handle_retrieve_selected_handler();
  $handler = new $handler_class($object);
  $handle = $handler->getFullHandle($object);
  
  $handle_pieces = explode("/", $handle);
  
  $new_handle = "hdl:" . $handle_pieces[0] . "%252F" . $handle_pieces[1];
  
  $image_array = array();
  
  $islandora_iiif_manifest_image_uri = url(variable_get('islandora_iiif_manifests_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2') . "/" . $new_handle,
    array(
      'absolute' => TRUE,
      'language' => (object)array('language' => FALSE),
    )
  );
  
  $id_uri = url(variable_get('islandora_iiif_manifests_domain_uri', $GLOBALS['base_url']) . "/iiif_manifest/{$object->id}/annotation/image", array(
    'absolute' => TRUE,
    'language' => (object)array('language' => FALSE),
  ));
  
  $image_array['thumbnail_uri'] = $islandora_iiif_manifest_image_uri . "/full/80,/0/default.jpg";
  $image_array['resource_uri'] = $islandora_iiif_manifest_image_uri . "/full/full/0/default.jpg";
  $image_array['service_uri'] = $islandora_iiif_manifest_image_uri;
  $image_array['id_uri'] = $id_uri;
  
  
  if ($object['TECHMD']) {
    // Get Image info from TECHMD.
    $mods_datastream = $object['TECHMD']->content;
    $xml = simplexml_load_string($mods_datastream);
    $json = json_encode($xml);
    $tech_md = json_decode($json, TRUE);
    if (isset($tech_md['metadata']['image']['imageWidth']) && !is_array($tech_md['metadata']['image']['imageWidth'])) {
      
      // Set width and height
      $image_array['imageWidth'] = is_array($tech_md['metadata']['image']['imageWidth']) ? $tech_md['metadata']['image']['imageWidth'][0] : $tech_md['metadata']['image']['imageWidth'];
      $image_array['imageHeight'] = is_array($tech_md['metadata']['image']['imageHeight']) ? $tech_md['metadata']['image']['imageHeight'][0] : $tech_md['metadata']['image']['imageHeight'];
    }
    
  }
  if (!isset($image_array['imageWidth']) && isset($object['OBJ'])) {
    
    $temp_file = drupal_tempnam('temporary://', 'dimensions');
    $object['OBJ']->getContent($temp_file);
    list($width, $height) = getimagesize($temp_file);
    
    $image_array['imageWidth'] = $width;
    $image_array['imageHeight'] = $height;
    
    @unlink($temp_file);
  }
  
  return $image_array;
}

/**
 * Retrieve the view direction from the book
 */
function islandora_iiif_manifests_get_viewing_direction($object) {
  
  $page_progression = islandora_paged_content_get_page_progression($object);
  $viewing_direction = "";
  switch ($page_progression) {
    
    case 'lr':
      $viewing_direction = "left-to-right";
      break;
    
    case 'rl':
      $viewing_direction = "right-to-left";
      break;
  }
  
  return $viewing_direction;
  
}

function islandora_iiif_manifests_in_multiarray($elem, $array) {
  $top = sizeof($array) - 1;
  $bottom = 0;
  while ($bottom <= $top) {
    if ($array[$bottom] == $elem)
      return true;
    else
      if (is_array($array[$bottom]))
        if (in_multiarray($elem, ($array[$bottom])))
          return true;
    
    $bottom++;
  }
  
  return FALSE;
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
 * @return void
 */
function islandora_iiif_manifests_create_json_response($status, $data = NULL) {
  
  drupal_add_http_header('Status', $status);
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  drupal_add_http_header('access-control-allow-origin', '*');
  drupal_add_http_header('Set-Cookie: same-site-cookie=foo; SameSite=Lax', FALSE);
  drupal_add_http_header('Set-Cookie: cross-site-cookie=bar; SameSite=None; Secure', FALSE);
  
  if ($data !== NULL) {
    echo json_encode($data, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE);
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


