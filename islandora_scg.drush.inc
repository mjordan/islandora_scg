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
      'drush iscgl --user=admin --quantity=20 --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection --namespace=foo',
      'drush iscgl --user=admin --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection',
    ),
    'options' => array(
      'quantity' => array(
        'description' => 'The number of objects to load. Defaults to 5.',
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
      'pages' => array(
        'description' => 'For paged content (including PDFs), the number of ' .
        'pages in each sample object. Defaults to 4.',
      ),
      'bgcolor' => array(
        'description' => 'For basic and large image content, the background color.' .
        'Defaults to blue. Available colors are the "color name" ' .
        'at http://www.imagemagick.org/script/color.php.',
      ),
      'metadata_file' => array(
        'description' => 'Name of the tab-separated metadata file, ' .
        "which must be in the module's includes directory. " .
        "Defaults to sample_metadata.tsv.",
      ),
      'quantity_newspaper_issues' => array(
        'description' => 'Number of newspaper issues to load into each newspaper ' .
        "Defaults to 0.",
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['islandora_scg_dump'] = array(
    'aliases' => array('iscgd'),
    'description' => 'Dump (purge) sample Islandora content',
    'examples' => array(
      'drush iscgd --user=admin --quantity=all',
      'drush iscgd --user=admin --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection',
    ),
    'options' => array(
      'quantity' => array(
        'description' => 'Use this option is you want to purge all of the sample ' .
        'objects from your repository. Only one value is allowed: "all". Subsets ' .
        'must be defined by combinations of --content_model and --parent.'
      ),
      'content_model' => array(
        'description' => 'The content models of the objects to purge.',
      ),
      'parent' => array(
        'description' => 'The collection from which to purge the objects. ' .
        'Note: objects are purged from the repository, not just removed ' .
        'from the collection.',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Generates and loads sample content.
 */
function drush_islandora_scg_load() {
  $path_to_metadata_file = drupal_get_path('module', 'islandora_scg') .
    DIRECTORY_SEPARATOR . 'includes' . DIRECTORY_SEPARATOR . 'sample_metadata.tsv';
  $options = array(
    'quantity' => drush_get_option('quantity', 5),
    'content_model' => drush_get_option('content_model'),
    'parent' => drush_get_option('parent'),
    'namespace' => drush_get_option('namespace', 'islandora'),
    'pages' => drush_get_option('pages', 4),
    'bgcolor' => drush_get_option('bgcolor', 'blue'),
    'metadata_file' => drush_get_option('metadata_file', $path_to_metadata_file),
    'quantity_newspaper_issues' => drush_get_option('quantity_newspaper_issues', 0),
  );

  // Map content models to classes, batch loader modules, etc.
  // If you add new content models, register them here.
  $registry = array(
    'islandora:sp_basic_image' => array(
       'class' => 'IslandoraSampleBasicImageGenerator',
       'cmodel_module' => 'islandora_basic_image',
       'batch_loader_module' => 'islandora_batch',
    ),
    'islandora:sp_large_image_cmodel' => array(
      'class' => 'IslandoraSampleLargeImageGenerator',
      'cmodel_module' => 'islandora_large_image',
      'batch_loader_module' => 'islandora_batch',
    ),
    'islandora:sp_pdf' => array(
      'class' => 'IslandoraSamplePdfGenerator',
      'cmodel_module' => 'islandora_pdf',
      'batch_loader_module' => 'islandora_batch',
    ),
    'islandora:bookCModel' => array(
      'class' => 'IslandoraSampleBookGenerator',
      'cmodel_module' => 'islandora_book',
      'batch_loader_module' => 'islandora_book_batch',
    ),
    'islandora:newspaperCModel' => array(
      'class' => 'IslandoraSampleNewspaperGenerator',
      'cmodel_module' => 'islandora_newspaper',
      'batch_loader_module' => 'islandora_batch',
    ),
    'islandora:newspaperPageCModel' => array(
      'class' => 'IslandoraSampleNewspaperIssueGenerator',
      'cmodel_module' => 'islandora_newspaper',
      'batch_loader_module' => 'islandora_newspaper_batch',
    ),
  );

  // Perform some validation on the options.
  $cmodel = $options['content_model'];
  if (!module_exists($registry[$cmodel]['batch_loader_module'])) {
    drush_set_error('Loader not available', dt('Sorry, the batch loader module !loader is not enabled.',
      array('!loader' => $registry[$cmodel]['batch_loader_module'])));
    exit;
  }
  if (!module_exists($registry[$cmodel]['cmodel_module'])) {
    drush_set_error('Content model module not available', dt('Sorry, the module that defines the content model !cmodel is not enabled.',
      array('!cmodel' => $registry[$cmodel]['cmodel_module'])));
    exit;
  }
  if ($options['bgcolor'] != 'blue') {
     module_load_include('inc', 'islandora_scg', 'includes/image_magic_colors');
     $image_magick_color_names = islandora_scg_get_color_names();
     if (!in_array($options['bgcolor'], $image_magick_color_names)) {
       drush_set_error('Bad color name',
         dt('Sorry, the value for --bgcolor (!color) is not a valid ImageMagick color name. Using default value of "blue".',
           array('!color' => $options['bgcolor']))
       );
       $options['bgcolor'] = 'blue';
     }
  }

  drush_print("Starting to generate sample content...");
  $generator = new $registry[$cmodel]['class']($options);
  $generator->createDataDir();
  $generator->generateContent();

  drush_print("Starting to load sample content...");
  $generator->loadContent();
  $generator->removeDataDir();
}

/**
 * Purges objects generated by this module.
 *
 * @todo: For book and newspaper objects, get all children and
 * purge them as well.
 */
function drush_islandora_scg_dump() {
  module_load_include('inc', 'islandora_scg', 'includes/utilities');
  $query = 'mods_recordInfo_recordOrigin_t:%22Islandora%20Sample%20Content%20Generator%22';
  // Build up query so that content model and parent are appended to the
  // basic query, if they are present.
  if (drush_get_option('quantity') == 'all') {
    $query = $query;
  }
  else {
    // Check that at least one of content model or parent are provided as options.
    if (!drush_get_option('content_model') && !drush_get_option('parent')) {
      drush_set_error('iscd options required',
        dt('Sorry, you need to specify at least one of --parent or --content_model.'));
      exit;
      
    }
    if (drush_get_option('content_model')) {
      $query .= '%20AND%20RELS_EXT_hasModel_uri_t:%22' . drush_get_option('content_model') . '%22';
    }
    if (drush_get_option('parent')) {
      $query .= '%20AND%20RELS_EXT_isMemberOfCollection_uri_t:%22' . drush_get_option('parent'). '%22';
    }
  }

  $pids = islandora_scg_query_solr($query);
  foreach ($pids as $pid) {
    $object = islandora_object_load($pid);
    if (islandora_delete_object($object)) {
      drush_print(t('Purging object !pid', array('!pid' => $pid)));
    }
    else {
      drush_print(t('Failed to purge object !pid', array('!pid' => $pid)));
    }
  }
}
