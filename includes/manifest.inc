<?php

/**
 * @file
 */

/**
 * Implements hook_islandora_iiif_manifest_query_backends().
 */
function islandora_iiif_islandora_iiif_manifest_query_backends() {
  $module_path = drupal_get_path('module', 'islandora_solr');
  return array(
    'islandora_solr_query_backend' => array(
      'title' => t('Manifest via Solr'),
      'callable' => 'islandora_iiif_islandora_manifest_solr_backend_callable',
      'file' => "$module_path/includes/manifest.inc",
    ),
  );
}

/**
 * Implements callback_islandora_iiif_manifest_query_backends()
 */
function islandora_iiif_islandora_manifest_solr_backend_callable($object, $page, $limit) {
  module_load_include('inc', 'islandora_solr', 'includes/db');
  $qp = new IslandoraSolrQueryProcessor();

  $qp->buildQuery(format_string('!member_field:("info:fedora/!pid" OR "!pid") OR !collection_member_field:("info:fedora/!pid" OR "!pid")', array(
    '!member_field' => variable_get('islandora_solr_member_of_field', 'RELS_EXT_isMemberOf_uri_ms'),
    '!pid' => $object->id,
    '!collection_member_field' => variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms'),
  )), drupal_get_query_parameters());

  $limit = variable_get('islandora_solr_num_of_results', '20');

  $qp->solrStart = $page * $limit;
  $qp->solrLimit = $limit;

  $qp->solrParams['sort'] = $qp->internalSolrParams['sort'];

  $qp->executeQuery();

  $map_to_pids = function ($result) {
    return $result['PID'];
  };
  return array(
    $qp->islandoraSolrResult['response']['numFound'],
    array_map($map_to_pids, $qp->islandoraSolrResult['response']['objects']),
  );
};

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_callback(AbstractObject $object) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  module_load_include('inc', 'islandora_solr_metadata', 'theme/theme');
  module_load_include('inc', 'islandora_iiif', 'includes/sequence');

  require_once ("utilities.inc");


  $objectModels = $object->models;
  /*$output = array();
  $hooks = islandora_build_hook_list(ISLANDORA_VIEW_HOOK, $objectModels);

  foreach ($hooks as $hook) {
        $temp = module_invoke_all($hook, $object);
        islandora_as_renderable_array($temp);
        if (!empty($temp)) {
            $output = array_merge_recursive($output, $temp);
        }
  }

  echo "<pre> out put "; print_r($output);die;*/
  $models = explode(":", $objectModels[0]);


  $model =  $models[1];
  switch($model){

      /*case 'collectionCModel':
          // generate manifest
      break;

      case 'sp_basic_image':
          // generate manifest
          break;

      case 'sp_large_image_cmodel':
          // generate manifest
          break;

      case 'sp_pdf':
          // generate manifest
          break;

      case 'sp-audioCModel':
          // generate manifest
          break;

      case 'sp_videoCModel':
          // generate manifest
          break;

      case 'sp_large_image_cmodel':
          // generate manifest
          echo "<pre> test"; print_r($object->models). "\n";
          break;*/

      case 'bookCModel':
          __generate_book_manifest($object->id, False,true);
          break;

      /*case 'pageCModel':
          // generate manifest
          break;

      case 'newspaperCModel':
          // generate manifest
          break;

      case 'newspaperIssueCModel':
          // generate manifest
          break;

      case 'compoundCModel':
          // generate manifest
          break;

      case 'entityCModel':
          // generate manifest
          break;

      case 'eventCModel':
          // generate manifest
          break;

      case 'placeCModel':
          // generate manifest
          break;

      case 'personCModel':
          // generate manifest
          break;

      case 'organizationCModel':
          // generate manifest
          break;

      case 'sp_disk_image':
          // generate manifest
          break;

      case 'sp_web_archive':
          // generate manifest
          break;

      case 'citationCModel':
          // generate manifest
          break;

      case 'thesisCModel':
          // generate manifest
          break;*/

      default:
          echo "No manifest code found for: ".$model. "\n";
          echo "<pre> Object"; print_r($object->models). "\n";
          break;
  }


}
