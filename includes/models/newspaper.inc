<?php


/**
 * @file
 */

/**
 * This is the actual manifest building function.
 */
function islandora_iiif_manifest_create_newspaper_manifest(AbstractObject $object) {
    module_load_include('inc', 'islandora', 'includes/utilities');
    module_load_include('inc', 'islandora_iiif_manifests', 'includes/utilities');
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    module_load_include('inc', 'islandora_newspaper', 'includes/utilities');

    // Manifest array.
    $manifest = array();

    // Get issues.
    $issues = islandora_newspaper_get_issues($object);

    // Get issues.
    $group_issues = islandora_newspaper_group_issues($issues);


    $collection_members_members = array();

    $i = 0;

    foreach($group_issues as $group_issue){

        foreach ($group_issue as $key0 => $val0) {

            $manifest_uri = urldecode(url("iiif_manifest/{$object->id}/collection/{$key0}", array(
                'absolute' => TRUE,
                'language' => (object)array('language' => FALSE),
            )));


            $collection_item['collections'][][$key0] = array(
                "@id" =>  $manifest_uri,
                "@type" => 'sc:Collection',
                "label" => "Volume ".$key0,
            );

            // Level 0
            if (is_array($val0)) {

                foreach ($val0 as $key1 => $val1) {

                    //Level 1
                    if (is_array($val1)) {

                        foreach ($val1 as $key2 => $val2) {

                            // Level 2
                            if (is_array($val2)) {

                                foreach ($val2 as $key3 => $val3) {

                                    // Level 3
                                    echo " $i Level 3:" . $key3 . ' = ' . $val3 . '<br />';
                                    echo "<pre>";
                                    print_r($val3);
                                    $issue_object = islandora_object_load($val3['pid']);
                                    $manifest_uri = urldecode(url("iiif_manifest/{$val3['pid']}/manifest/", array(
                                        'absolute' => TRUE,
                                        'language' => (object)array('language' => FALSE),
                                    )));
                                    $label = islandora_iiif_manifests_convert_label($issue_object);
                                    $collection_members_members['members'][$i] = array(
                                        "@id" =>  $manifest_uri,
                                        "@type" => 'sc:Manifest',
                                        "label" => $label,
                                    );


                                    $i++;





                                }
                            }
                            else {
                                echo "Level 2:" . $key2 . ' = ' . $val2 . '<br />';
                            }
                        }
                    }
                    else {
                        echo "Level 1:" . $key1 . ' = ' . $val1 . '<br />';
                    }
                }
            }
            else {
                echo "Level 0:" . $key0 . ' = ' . $val0 . '<br />';
            }

        }

    }



    echo "<pre>";
    print_r($collection_members_members);
    print_r($collection_item);
    print_r($group_issues);
    die;


    // Load issue.
    $group_issue = islandora_object_load($issue['pid']);


    foreach ($issues as $issue) {

        // Load issue.
        $issue_object = islandora_object_load($issue['pid']);

        if (islandora_iiif_manifests_allowed_manifest($issue_object)) {

            $collection_item = islandora_iiif_manifests_get_object_collection($issue_object);

            $collection_members_members[] = $collection_item['object'];
        }

    }

    if (islandora_iiif_manifests_allowed_manifest($issue_object)) {

        $collection_item = islandora_iiif_manifests_get_object_collection($issue_object);

        $collection_members_members[] = $collection_item['object'];
    }


    if (!empty($collection_members_members)) {
        $manifest['members'] = $collection_members_members;
    }

    return $manifest;

}
