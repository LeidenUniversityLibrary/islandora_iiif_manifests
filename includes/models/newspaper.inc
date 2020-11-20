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

    foreach ($group_issues as $key => $val) {
        if (is_array($val)) {
            //We can use foreach instead of printin it out directly
            //if this array will have more value
            //If not just use $val['id']
            foreach ($val as $k => $v) {
                echo $k . ' = ' . $v . '<br />';
            }
        }
        else {
            echo $key . ' = ' . $val . '<br />';
        }
    }

    foreach ($group_issues as $key0 => $val0) {

        // Level 0
        if (is_array($val0)) {

            foreach ($val0 as $key1 => $val1) {

                //Level 1
                if (is_array($val1)) {

                    foreach ($val1 as $key2 => $val2) {

                        // Level 2
                        echo "Level 2:". $key2 . ' = ' . $val2 . '<br />';
                    }
                }
                else {
                    echo "Level 1:".$key1 . ' = ' . $val1 . '<br />';
                }
            }
        }
        else{
            echo "Level 0:".$key0 . ' = ' . $val0 . '<br />';
        }

    }

    echo "<pre>";
    print_r($group_issues);die;

    $pid = array_column($value, 'pid');
    echo "Key $key <br>";
    echo "Pid $pid <br>";
    echo "<br><pre>";
    print_r($value);
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


}

foreach ($issues as $issue) {

    // Load issue.
    $issue_object = islandora_object_load($issue['pid']);

    if (islandora_iiif_manifests_allowed_manifest($issue_object)) {

        $collection_item = islandora_iiif_manifests_get_object_collection($issue_object);

        $collection_members_members[] = $collection_item['object'];
    }

}

if (!empty($collection_members_members)) {
    $manifest['members'] = $collection_members_members;
}

return $manifest;

}