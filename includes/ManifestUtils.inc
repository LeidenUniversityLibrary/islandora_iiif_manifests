<?php
class ManifestUtils {
  protected $object = '';
  protected $canvases = array ();

  /**
   * Initiate the manifest
   *
   * @param unknown $metadata_uri
   * @param string $object_label
   */
  function __construct($object) {
    $this->object = $object;
  }

  /**
   * Add a $canvas to $sequences
   *
   * @param string $canvas
   */
  function addCanvas($canvas = null) {
    if ($canvas != null) {
      $this->canvases [] = $canvas->toArray();
    }
  }

  /**
   * Build the manifest container
   */

  function getContainer() {

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
            'label' => $this->object->label,
            "logo" => "https://kerkuil-a.leidenuniv.nl/iiif/ubl_logo.png",
            "attribution" => "This manifest was created from Islandora. Contact r.k.s.mwinyimbegu@library.leidenuniv.nl",
            "description" => $this->object->label,
            'sequences' => array ( // an array but will always be a single object in our application
                array (
                    '@type' => 'sc:Sequence',
                    'label' => $this->object->label,
                    'canvases' => array()
                )
            )
        );
        $metadata = $this->getMetadata();
        if (isset($metadata['manifest'])) {
          $scManifest = array_merge_recursive($scManifest, $metadata['manifest']);
        }
        //return json_encode ( $scManifest );
        return $scManifest;
    }


  function getMetadata() {
    $metadata = array();
    if (isset($this->object['MODS'])) {
      $modsxml = $this->object['MODS']->content;
      $xslpath = drupal_get_path('module', 'islandora_iiif_manifests') . '/xsl/mods2manifest.xsl';
      $xsl = new DOMDocument();
      $xsl->load($xslpath);
      $mods = new DOMDocument();
      $mods->loadXML($modsxml);
      $processor = new XSLTProcessor();
      $processor->importStylesheet($xsl);
      $manifest = $processor->transformToDoc($mods);
      $metadata = $this->xml_to_array($manifest);
    }
    return $metadata;
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

    return $result;
  }
}
