<?php

/**
 * @file
 * Form definition functions.
 */

/**
 * Form definition function.
 */
function islandora_scg_generate_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_scg', 'includes/utilities');
  module_load_include('inc', 'islandora_scg', 'includes/image_magic_colors');
  $form['islandora_scg_overview'] = array(
    '#markup' => t('This form is intended for loading small numbers of objects (e.g., 10 or so). If you want to load larger numbers of objects, or load objects with content models not listed here, or want to use advanced options, you should use the Drush interface.'),
    '#prefix' => '<div class="messages warning">',
    '#suffix' => '</div>',
  );
  $cmodels = islandora_scg_get_cmodels();
  $form['islandora_scg_cmodel'] = array(
    '#type' => 'select',
    '#title' => t('Content model'),
    '#options' => $cmodels,
    '#default_value' => variable_get('islandora_scg_cmodel', ''),
    '#description' => t('Select the content model for the objects you want to generate.'),
  );
  $collections = islandora_scg_get_collections();
  $form['islandora_scg_collection'] = array(
    '#type' => 'select',
    '#title' => t('Collection'),
    '#options' => $collections,
    '#default_value' => variable_get('islandora_scg_collection', ''),
    '#description' => t('Select the parent collection for the objects you want to generate.'),
  );
  $form['islandora_scg_quantity'] = array(
    '#title' => t('Quantity'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => variable_get('islandora_scg_quantity', '5'),
    '#description' => t("Number of objects to generate."),
    '#required' => TRUE,
  );
  $form['islandora_scg_namespace'] = array(
    '#title' => t('Namespace'),
    '#type' => 'textfield',
    '#size' => 30,
    '#default_value' => variable_get('islandora_scg_namespace', 'islandora'),
    '#description' => t("Namespace to assign to the objects. Do not include the trailing colon."),
    '#required' => TRUE,
  );
  $color_names = islandora_scg_get_color_names();
  $colors = array();
  foreach ($color_names as $name) {
    $colors[$name] = $name;
  }
  $form['islandora_scg_color'] = array(
    '#type' => 'select',
    '#title' => t('Background color'),
    '#options' => $colors,
    '#default_value' => variable_get('islandora_scg_color', 'blue'),
    '#description' => t('Select the background color for the objects you want to generate.'),
  );
  $form['islandora_scg_generate'] = array(
    '#type' => 'submit',
    '#value' => 'Generate sample objects',
    '#ajax' => array(
      'callback' => 'islandora_scg_progress_bar',
      'progress' => array(
        'type' => 'throbber',
        'message' => t('Generating and loading sample objects. Please be patient...'),
        'url' => url('islandora_scg_load_progress'),
        'interval' => 1000,
      ),
    ),
  );
  return $form;
}

/**
 * Ajax callback.
 */
function islandora_scg_progress_bar($quantity = 5) {
  $progress = array(
    'percentage' => -1,
  );

  $num_objects_to_generate = $_SESSION['islandora_scg_num_objects_to_generate'];
  $num_objects_generated = $_SESSION['islandora_scg_num_objects_generated'];

  $progress['percentage'] = $num_objects_generated / $num_objects_to_generate * 100;
}
