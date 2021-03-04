<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_collection_manifest(AbstractObject $object) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');

  $qp = new IslandoraSolrQueryProcessor();
  $usedsolrfields = array('PID', 'RELS_EXT_hasModel_uri_ms', 'fgs_label_s');
  $parent_id = islandora_solr_lesser_escape($object->id);
  $relcomp = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');
  $query = "($relcomp:($parent_id) OR $relcomp:(" . islandora_solr_lesser_escape('info:fedora/') . "$parent_id))";
  $query .= ' AND (fedora_datastreams_ms:' . islandora_solr_lesser_escape(ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID) . ')';
  $qp->buildQuery("*:*");
  $qp->solrStart = 0;
  $qp->solrLimit = 10000;
  $qp->solrParams['facet'] = 'false';
  $qp->solrParams['fq'][] = $query;
  $qp->solrParams['fl'] = implode(',', $usedsolrfields);

  $qp->executeQuery();
  if (isset($qp->islandoraSolrResult['response']['numFound']) && $qp->islandoraSolrResult['response']['numFound'] > 0) {
    foreach ($qp->islandoraSolrResult['response']['objects'] as $solrobj) {
      
      echo "<pre>";print_r($solrobj);die;
      // Hier moet nog iets gebeuren, maar laat hiervoor NIET het hele islandora object in met islandora_object_load, omdat dit zorgt voor slechte
      // performance. Gebruik de waardes uit $solrobj om de juiste members te genereren: het bevat PID (item id), content_models en object_label.
      $manifest['members'][] = $solrobj;
    }
  }

  return $manifest;
}
