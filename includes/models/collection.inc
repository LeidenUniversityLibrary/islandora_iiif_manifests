<?php


/**
 * @file
 */
function islandora_iiif_manifest_create_collection_manifest(AbstractObject $object) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  
  $limit = 500;
  $num_found = 1;
  $current = 0;
  
  while ($current < $num_found) {
    
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
    
    $num_found = isset($qp->islandoraSolrResult['response']['numFound']) ? $qp->islandoraSolrResult['response']['numFound'] : 0;
    
    if ($num_found > 0) {
      
      foreach ($qp->islandoraSolrResult['response']['objects'] as $solrobj) {
        
        $current++;
        $manifest['members'][] = islandora_iiif_manifests_get_object_collection($solrobj);
        
      }
 
    }
    
  }
  
  return $manifest;
}
