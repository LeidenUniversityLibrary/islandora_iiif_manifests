<?php
class ManifestUtils {
  protected $object_label = '';
  protected $canvases = array ();

  /**
   * Initiate the manifest
   *
   * @param unknown $metadata_uri
   * @param string $object_label
   */
  function __construct($label) {
    $this->object_label = $label;
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
   * Build and return a json string based on what we have in the class
   */
  function getJson() {

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
        'label' => $this->object_label,
        //'attribution' => 'For rights and reproduction information please contact collections@library.utoronto.ca',
        'sequences' => array ( // an array but will always be a single object in our application
            array (
                '@type' => 'sc:Sequence',
                'label' => $this->object_label . ', in order',
                'canvases' => $this->canvases
            )
        )
    );
    //return json_encode ( $scManifest );
    return $scManifest;
  }

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
            'label' => $this->object_label,
            //'attribution' => 'For rights and reproduction information please contact collections@library.utoronto.ca',
            'sequences' => array ( // an array but will always be a single object in our application
                array (
                    '@type' => 'sc:Sequence',
                    'label' => $this->object_label . ', in order',
                    'canvases' => array()
                )
            )
        );
        //return json_encode ( $scManifest );
        return $scManifest;
    }
}
