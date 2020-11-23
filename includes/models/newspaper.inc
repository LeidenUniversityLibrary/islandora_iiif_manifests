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



    foreach ($group_issues as $volume => $issue) {

        echo "Volume $volume issue: <pre>";
        print_r($issue);

        $manifest_uri = urldecode(url("iiif_manifest/{$object->id}/collection/{$volume}", array(
            'absolute' => TRUE,
            'language' => (object)array('language' => FALSE),
        )));


        $collection_item['collections'][][$volume] = array(
            "@id" => $manifest_uri,
            "@type" => 'sc:Collection',
            "label" => "Volume " . $volume,
        );

        $i = 0;
        foreach ($issue as $key_month => $value_month) {

            // Level 0
            if (is_array($value_month)) {

                foreach ($value_month as $key_day => $value_day) {

                    //Level 1
                    if (is_array($value_day)) {

                        echo "Value Day<pre>";print_r($value_day);

                        foreach ($value_day as $issue_data) {

                            // Level 3

                            $issue_object = islandora_object_load($issue_data['pid']);
                            $manifest_uri = urldecode(url("iiif_manifest/{$issue_data['pid']}/manifest/", array(
                                'absolute' => TRUE,
                                'language' => (object)array('language' => FALSE),
                            )));
                            $label = islandora_iiif_manifests_convert_label($issue_object);
                            $collection_members_members[$i] = array(
                                "@id" => $manifest_uri,
                                "@type" => 'sc:Manifest',
                                "label" => $label,
                            );
                            echo "<pre>";print_r($issue_data);
                            echo " Volume $volume Issue $i $label  <br />";
                            $collection_item['collections'][][$volume]['members'] = $collection_members_members;
                            $i++;

                        }
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
