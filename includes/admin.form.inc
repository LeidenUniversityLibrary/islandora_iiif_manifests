<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_iiif_admin_settings_form(array $form, array &$form_state) {
  form_load_include($form_state, 'inc', 'islandora_iiif', 'includes/admin.form');
  $get_default_value = function ($name, $default) use (&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  $solr_enabled = module_exists('islandora_solr');
  $image_server_url = $get_default_value('islandora_iiif_image_server', 'https://iiif-a.issc.leidenuniv.nl/iiif/2/');
  $image_server_available_message = islandora_iiif_admin_settings_form_image_server_available_message($image_server_url);
  $form = array(
    'islandora_iiif_images_server_url' => array(
      '#type' => 'textfield',
      '#prefix' => '<div id="imgs-path-wrapper">',
      '#suffix' => '</div>',
      '#title' => t('Adore-djatoka URL'),
      '#description' => filter_xss(t('<strong>Server accessible</strong>  <br/>!msg', array('!msg' => $image_server_available_message))),
      '#default_value' => $image_server_url,
      '#ajax' => array(
        'callback' => 'islandora_iiif_admin_settings_form_image_server_ajax_callback',
        'wrapper' => 'imgs-path-wrapper',
        'effect' => 'fade',
        'event' => 'change',
      ),
    ),
    'islandora_iiif_resizeondrupal' => array(
      '#type' => 'checkbox',
      '#title' => t('Resize images on Drupal via imagemagick'),
      '#description' => t('Enabling this can have some perfomance costs in favour of a much better image quality. Djatoka\'s resizing function suffers from quality degradation.'),
      '#default_value' => (int) $get_default_value('islandora_iiif_resizeondrupal', 0),
    ),
  // @TODO need to define how we are going to fetch web annotations. Probably add an API entry and a hookable callback selector

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
