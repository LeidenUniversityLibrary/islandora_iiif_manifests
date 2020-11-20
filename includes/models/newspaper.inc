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

    foreach ($group_issues as $group_issue) {

        $keys = array_keys($group_issue);
        echo "<pre>";print_r($group_issue[$keys[0]]);
        echo "<br><pre>";print_r($group_issue[$keys[1]]);
        echo "<br><pre>";print_r($group_issue[$keys[2]]);
        echo "<br><pre>";print_r($group_issue[$keys[3]]);
        echo "<br><pre>";
        print_r($group_issue );
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