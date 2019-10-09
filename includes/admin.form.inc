<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Creates the administration form
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the islandora_mirador_bookreader administration form.
 */
function islandora_iiif_admin_settings_form($form, &$form_state) {
    $get_default_value = function($name, $default) use($form_state) {
        return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
    };
    //$solr_enabled = module_exists('islandora_solr');
    $form = array(
        'islandora_sc_manifest_image_uri' => array(
            '#type' => 'textfield',
            '#title' => t('SC Manifest Image URI'),
            '#default_value' => $get_default_value('islandora_sc_manifest_image_uri', 'https://iiif-a.issc.leidenuniv.nl/iiif/2'),
        ),
        'islandora_sc_manifest_presentation_uri' => array(
            '#type' => 'textfield',
            '#title' => t('SC Manifest Presentation URI'),
            '#default_value' => $get_default_value('islandora_sc_manifest_presentation_uri', 'https://iiif.library.utoronto.ca/presentation/v2/'),
        )
    );
    return system_settings_form($form);
}

/**
 * Ajax callback for the djatoka url textfield.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The element to render as part the ajax callback.
 */
function islandora_iiif_admin_settings_form_image_server_ajax_callback(array $form, array &$form_state) {
  return $form['islandora_iiif_images_server_url'];
}

/**
 * Gets a message to display if the djatoka is accessible or not.
 *
 * @param string $image_server_url
 *   The url to an djatoka instance.
 *
 * @return string
 *   A message in html detailing if the djatoka is accessible.
 */
function islandora_iiif_admin_settings_form_image_server_available_message($image_server_url) {
  $image_server_url = trim($image_server_url, '/');

  $file_headers = @get_headers($image_server_url);
  $image_server_availible = strstr($file_headers[0], '200') !== FALSE;
  if ($image_server_availible) {
    $image = theme_image(array('path' => 'misc/watchdog-ok.png', 'attributes' => array()));
    $message = t('IIIF image server url is valid.');
  }
  else {
    $image = theme_image(array('path' => 'misc/watchdog-error.png', 'attributes' => array()));
    $message = t('Unable to locate iiif image server endpoint at @path', array('@path' => $image_server_url));
  }
  return $image . $message;
}

