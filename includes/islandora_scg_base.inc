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
   * @var bool $child_uses_timer - Indicates whether the child object
   *   (pages, for example) should use a timer or let the parent use it.
   */
  public $child_uses_timer = TRUE;

  /**
   * Constructor for the IslandoraSampleContentGenerator abstract class.
   */
  public function __construct($options) {
    // Options from the drush script.
    $this->quantity = $options['quantity'];
    $this->parent = $options['parent'];
    $this->content_model = $options['content_model'];
    $this->namespace = $options['namespace'];
    $this->pages = isset($options['pages']) ? $options['pages'] : 4;
    $this->bgcolor = $options['bgcolor'];
    $this->metadata_file = $options['metadata_file'];
    $this->quantity_newspaper_issues = isset($options['quantity_newspaper_issues']) ? $options['quantity_newspaper_issues'] : 0;
    if ($options['timer']) {
      $this->timer = TRUE;
    }
    else {
      $this->timer = FALSE;
    }
    $this->data_dir = $options['data_dir'];

    // Serialized XML content that will be used for the MODS, DC,
    // etc. datastream.
    $this->metadata_ds = '';

    if (DRUSH_VERSION >= 7) {
      $this->source_dir_option = '--scan_target=';
    }
    else {
      $this->source_dir_option = '--target=';
    }
  }

  /**
   * Calls the theming function for the metadata XML.
   *
   * @param string $genre
   *  The value to use in the MODS <genre> element.
   */
  public function generateMetadataDs($genre = '') {
    return theme('islandora_scg_metadata_ds', array(
      'genre' => $genre,
      'metadata_file' => $this->metadata_file)
    );
  }

  /**
   * Blasts, then creates the output directory.
   */
  public function createDataDir() {
    // We remove the data directory here in case of aborted ingest.
    $this->removeDataDir();
    drupal_mkdir($this->data_dir);
  }

  /**
   * Deletes the output directory.
   */
  public function removeDataDir() {
    if (file_exists($this->data_dir)) {
      file_unmanaged_delete_recursive($this->data_dir);
    }
  }

  /**
   * Starts the benchmark timer.
   */
  public function startTimer() {
    $time_start = microtime(true);
    return $time_start;
  }

  /**
   * Stops the benchmark timer and reports the results.
   *
   * @param float $start_time
   *   The current time in seconds since the Unix epoch accurate
   *   to the nearest microsecond.
   * @param int $quantity
   *   The value of the --quantity option as passed in from the command line.
   * @param string $label
   *   A string indicating what is being ingested.
   * @param string $extra
   *   A string indicating any additional information you want printed,
   *   like how many pages were in each  book.
   */
  public function stopTimer($time_start, $quantity, $label, $extra = '') {
    if ($this->timer) {
      $time_end = microtime(true);
      $time = $time_end - $time_start;
      $average_time = $time / $quantity;
      print "Took $time seconds to ingest $quantity $label $extra (for an average of $average_time seconds each)\n";
    }
  }

}
