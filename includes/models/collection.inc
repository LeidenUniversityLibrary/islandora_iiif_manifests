<?php


/**
 * @file
 */
function islandora_iiif_manifest_create_collection_manifest(AbstractObject $object) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  
  $limit = 500;
  $numfound = 1;
  $current = 0;
  $iteration = 0;
  
  while ($current < $numfound) {
    
    $qp = new IslandoraSolrQueryProcessor();
    $usedsolrfields = array('PID', 'RELS_EXT_hasModel_uri_ms', 'fgs_label_s', 'json_MANIFEST_json_type_ms');
    $parent_id = islandora_solr_lesser_escape($object->id);
    $relcomp = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');
    $query = "($relcomp:($parent_id) OR $relcomp:(" . islandora_solr_lesser_escape('info:fedora/') . "$parent_id))";
    $query .= ' AND (fedora_datastreams_ms:' . islandora_solr_lesser_escape(ISLANDORA_IIIF_MANIFESTS_DATASTREAM_ID) . ')';
    $qp->buildQuery("*:*");
    $qp->solrStart = $current;
    $qp->solrLimit = $limit;
    $qp->solrParams['facet'] = 'false';
    $qp->solrParams['fq'][] = $query;
    $qp->solrParams['fl'] = implode(',', $usedsolrfields);
    
    $qp->executeQuery(FALSE);
    
    $numfound = isset($qp->islandoraSolrResult['response']['numFound']) ? $qp->islandoraSolrResult['response']['numFound'] : 0;
    
    if ($numfound > 0) {
      
      foreach ($qp->islandoraSolrResult['response']['objects'] as $solrobj) {
        
        $current++;
        $collection_item = islandora_iiif_manifests_get_object_collection($solrobj);
        $collection_members_members[] = $collection_item['object'];
      }
      
      if (!empty($collection_members_members)) {
        $manifest['members'] = $collection_members_members;
      }
      
    }
    
    $iteration++;
  }
  
  return $manifest;
}
