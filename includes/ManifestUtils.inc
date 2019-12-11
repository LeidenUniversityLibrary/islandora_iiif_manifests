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
        //return json_encode ( $scManifest );
        return $scManifest;
    }
}
