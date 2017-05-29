<?php

/**
 * @file
 * An implementation of a newspaper issue content type sample content generator.
 */

/**
 * A class that generates sample newspaper issue content.
 */
class IslandoraSampleNewspaperIssueGenerator extends IslandoraSampleContentGenerator {
  public function generateContent() {
    for ($i = 1; $i <= $this->quantity; $i++) {
      $issue_dir = $this->data_dir . DIRECTORY_SEPARATOR . $i;
      drupal_mkdir($issue_dir);
      $this->generateIssue($issue_dir, $i);
    }
  }

  /**
   * Generates a single newspaper issue.
   *
   * @param string $issue_dir
   *  The directory where the newspaper issue content is written.
   */
  public function generateIssue($issue_dir, $issue_number) {
    $metadata_ds = $this->generateMetadataDs('text');
    $metadata_ds_file_path = $issue_dir . DIRECTORY_SEPARATOR . 'MODS.xml';
    $mods = simplexml_load_string($metadata_ds);

    // Modify mods->originInfo[0]->dateIssued to be in yyyy-mm-dd.
    // Pick a random month and day for each issue.
    $year = $mods->originInfo[0]->dateIssued;
    $rand_month = (string) rand(1, 12);
    $rand_month = str_pad($rand_month, 2, '0', STR_PAD_LEFT);
    $rand_day = (string) rand(1, 31);
    $rand_day = str_pad($rand_day, 2, '0', STR_PAD_LEFT);
    $date_issued = $year . '-' . $rand_month . '-' . $rand_day;
    $mods->originInfo[0]->dateIssued = $date_issued;

    // Get the parent issue's title and add it to the MODS XML.
    $parent_object = islandora_object_load($this->parent);
    $parent_title = $parent_object->label;
    $issue_title = htmlspecialchars_decode($parent_title) . " ($date_issued)";
    $mods->titleInfo[0]->title = $issue_title;

    // Delete some of the MODS elements that don't apply to newspaper issues.
    unset($mods->abstract);
    unset($mods->subject);

    $metadata_ds = $mods->asXML();
    file_put_contents($metadata_ds_file_path, $metadata_ds);

    // Generate the page-level OBJ datastream files (one per directory).
    drush_print("Generating sample content for $issue_title");
    // Generate the designated number of pages.
    for ($p = 1; $p <= $this->pages; $p++) {
      $page_dir = $issue_dir . DIRECTORY_SEPARATOR . $p;
      drupal_mkdir($page_dir);

      if ($p == 1) {
        $text = $issue_title;
      }
      else {
        $text = $issue_title . "\n\nPage $p / " . $this->pages;
      }
      $page_cmd = "convert -density 300x300 -size 2550x3300 xc:white ";
      $page_cmd .= $page_dir . DIRECTORY_SEPARATOR  . 'OBJ.tiff; ';
      $page_cmd .= "convert -size 2550x3300 xc:white " . " -pointsize 300 -fill black ";
      $page_cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($text, 15));
      $page_cmd .=  " -flatten +matte -density 300x300 " . $page_dir . DIRECTORY_SEPARATOR . 'OBJ.tiff';
      exec($page_cmd);
    }
  }

  public function loadContent() {
    if ($this->child_uses_timer) {
      $start_time = $this->startTimer();
    }
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_newspaper_batch_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace . ' --parent=' . $this->parent;
    $batch_cmd .= ' --type=directory ' . $this->source_dir_option . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    exec($ingest_cmd);
    if ($this->child_uses_timer) {
      $this->stopTimer($start_time, $this->quantity, 'newspaper issues');
    }
  }

}
