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
    
      // Cmodels that are allowed in the collection manifest
      $cmodels = array(
        'info:fedora/islandora:sp_basic_image',
        'info:fedora/islandora:sp_large_image_cmodel',
        'info:fedora/islandora:bookCModel',
        'info:fedora/islandora:newspaperCModel',
        'info:fedora/islandora:collectionCModel',
        'info:fedora/islandora:newspaperIssueCModel',
        'info:fedora/islandora:compoundCModel');
      
      if (count(array_intersect($solrobj['content_models'], $cmodels)) > 0) {
        $collection_item = islandora_iiif_manifests_get_object_collection($solrobj);
        $collection_members_members[] = $collection_item['object'];
      }
      
    }
    
    if (!empty($collection_members_members)) {
      $manifest['members'] = $collection_members_members;
    }
  }
  
  return $manifest;
}
