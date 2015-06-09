<?php

/**
 * @file
 * Abstract base class for Islandora Sample Content Generators.
 */

/**
 * Abstract sample content generator base class.
 */
abstract class IslandoraSampleContentGenerator {

  abstract protected function generateContent();
  abstract protected function loadContent();

  /**
   * Constructor for the IslandoraSampleContentGenerator abstract class.
   */
  public function __construct($options) {
    // Options from the drush script.
    $this->quantity = $options['quantity'];
    $this->parent = $options['parent'];
    $this->content_model = $options['content_model'];
    $this->namespace = $options['namespace'];

    // Path to the directory where the sample content is written.
    $this->data_dir = drupal_realpath('public://') . DIRECTORY_SEPARATOR . 'islandora_scg';
    // Serialized XML content that will be used for the MODS, DC,
    // etc. datastream.
    $this->metadata_ds = '';
  }

  /**
   * Calls the theming function for the metadata XML.
   */
  public function generateMetadataDs() {
    return theme('islandora_scg_metadata_ds', array());
  }

  /**
   * Creates the output directory.
   */
  public function createDataDir() {
    if (!file_exists($this->data_dir)) {
      drupal_mkdir($this->data_dir);
    }
  }

  /**
   * Deletes the output directory.
   */
  public function removeDataDir() {
    if (file_exists($this->data_dir)) {
      file_unmanaged_delete_recursive($this->data_dir);
    }
  }

}
