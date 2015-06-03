<?php

/**
 * @file
 * Drush file for the Islandora Sample Content Generator.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_scg_drush_command() {
  $items = array();
  $items['islandora_scg_load'] = array(
    'aliases' => array('iscgl'),
    'description' => 'Generate and load sample Islandora content',
    'examples' => array(
      'drush iscgl --user=admin --quantity=20 --content_model=islandora:sp_basic_image --parent=myrepo:mycollection --namespace=myrepo',
      'drush iscgl --user=admin --content_model=islandora:sp_basic_image --parent=myrepo:mycollection',
    ),
    'options' => array(
      'quantity' => array(
        'description' => 'The number of objects to load. ' .
        'Defaults to 5.',
      ),
      'content_model' => array(
        'description' => 'The content models to assign to the objects.',
        'required' => TRUE,
      ),
      'parent' => array(
        'description' => 'The collection to which the generated items should ' .
        'be added.',
        'required' => TRUE,
      ),
      'namespace' => array(
        'description' => 'Namespace of objects to create. ' .
        'Defaults to "islandora".',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

function drush_islandora_scg_load() {
  $options = array(
    'quantity' => drush_get_option('quantity', 5),
    'content_model' => drush_get_option('content_model'),
    'parent' => drush_get_option('parent'),
    'namespace' => drush_get_option('namespace', 'islandora'),
  );

  // Map content models to classes. If you add new content generator
  // classes, register them here.
  $registry = array(
    'islandora:sp_basic_image' => 'IslandoraSampleBasicImageGenerator',
    'islandora:sp_large_image_cmodel' => 'IslandoraSampleLargeImageGenerator',
    'islandora:sp_pdf' => 'IslandoraSamplePdfGenerator',
  );

  drush_print("Instantiating sample content generator...");
  $mything = new $registry[$options['content_model']]($options);
  $mything->printMessage();
  $mything->printCommon();
}
