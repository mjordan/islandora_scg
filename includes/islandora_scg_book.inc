<?php

/**
 * @file
 * An implementation of a Book content type sample content generator.
 */

/**
 * A class that generates sample book content.
 */
class IslandoraSampleBookGenerator extends IslandoraSampleContentGenerator {
  public function generateContent() {
    for ($i = 1; $i <= $this->quantity; $i++) {
      $book_dir = $this->data_dir . DIRECTORY_SEPARATOR . $i;
      drupal_mkdir($book_dir);
      $this->generateBook($book_dir);
    }
  }

  /**
   * Generates a single book.
   *
   * @param string $book_dir
   *  The directory where the book content is written.
   */
  public function generateBook($book_dir) {
    $metadata_ds = $this->generateMetadataDs('text');
    $metadata_ds_file_path = $book_dir . DIRECTORY_SEPARATOR . 'MODS.xml';
    file_put_contents($metadata_ds_file_path, $metadata_ds);

    // Get the title to add to the image. Clunky.
    $mods = simplexml_load_string($metadata_ds);
    $title = $mods->titleInfo[0]->title;
    $title = htmlspecialchars_decode($title);

    // Generate the page-level OBJ datastream files (one per directory).
    if (function_exists('drush_print')) {
      drush_print("Generating sample content for $title");
    }
    // Generate the designated number of pages.
    for ($p = 1; $p <= $this->pages; $p++) {
      $page_dir = $book_dir . DIRECTORY_SEPARATOR . $p;
      drupal_mkdir($page_dir);

      if ($p == 1) {
        $text = $title;
      }
      else {
        $text = $title . "\n\nPage $p / " . $this->pages;
      }
      $page_cmd = "convert -density 300x300 -size 2550x3300 xc:white ";
      $page_cmd .= $page_dir . DIRECTORY_SEPARATOR  . 'OBJ.tiff; ';
      $page_cmd .= "convert -size 2550x3300 xc:white " . " -pointsize 300 -fill black ";
      $page_cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($text, 15));
      $page_cmd .=  " -flatten +matte -density 300x300 " . $page_dir . DIRECTORY_SEPARATOR . 'OBJ.tiff';
      exec($page_cmd);
    }
  }

  /**
   * Runs the Islandora Book Batch drush commands to ingest the sample content.
   */
  public function loadContent() {
    $start_time = $this->startTimer();
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_book_batch_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace . ' --parent=' . $this->parent;
    $batch_cmd .= ' --parent_relationship_pred=isMemberOfCollection --type=directory ' . $this->source_dir_option . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    exec($ingest_cmd);
    $this->stopTimer($start_time, $this->quantity, 'books', 'with ' . $this->pages . ' pages per book');
  }

}
