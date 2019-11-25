<?php
/**
 * @file
 * Defines functions used when constructing derivatives.
 */

/**
 * Decides which derivative function to call and runs it.
 *
 * @param AbstractObject $object
 *   The object to run the derivative function for.
 * @param string $dsid
 *   The DSID to run the derivative function for.
 */
function islandora_iiif_create_manifest(AbstractObject $object = NULL, $force = TRUE) {
    dpm($object,"Going to create manifest!");
    $output = array();
    $hooks = islandora_build_hook_list(ISLANDORA_IIIF_MANIFEST_HOOK, $object->models);
    dpm($hooks,"hooks");
    foreach ($hooks as $hook) {
        $temp = module_invoke_all($hook, $object);
        if (!empty($temp)) {
            arsort($temp);
            $output = array_merge_recursive($output, $temp);
        }
    }
    dpm($output, "output");
}