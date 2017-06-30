<?php

/**
 * @file
 * An implementation of a Basic Image content type sample content generator.
 */

/**
 * A class that generates sample Basic Image content.
 */
class IslandoraSampleBasicImageGenerator extends IslandoraSampleContentGenerator {

  /**
   * Generates sample basic image content.
   */
  public function generateContent() {
    for ($i = 1; $i <= $this->quantity; $i++) {
      $metadata_ds = $this->generateMetadataDs('image');
      $metadata_ds_file_path = $this->data_dir . DIRECTORY_SEPARATOR . $i . '.xml';
      file_put_contents($metadata_ds_file_path, $metadata_ds);

      // Get the title to add to the image. Clunky.
      $mods = simplexml_load_string($metadata_ds);
      $title = $mods->titleInfo[0]->title;
      $title = htmlspecialchars_decode($title);

      // Generate the OBJ datastream file.
      $cmd = "convert -size 1000x1000 xc:" . $this->bgcolor ." ";
      $cmd .= $this->data_dir .  DIRECTORY_SEPARATOR . $i . '.jpg; ';
      $cmd .= "convert -size 1000x1000 xc:" . $this->bgcolor . " -pointsize 100 -fill white ";
      $cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($title, 15));
      $cmd .=  " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . ".jpg";
      if (function_exists('drush_print')) {
        drush_print("Generating sample content for $title");
      }
      exec($cmd);
    }
  }

  /**
   * Runs the Islandora Batch drush commands to ingest the sample content.
   */
  public function loadContent() {
    $start_time = $this->startTimer();
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_scan_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace;
    $batch_cmd .= ' --content_models=islandora:sp_basic_image --parent=' . $this->parent;
    $batch_cmd .= ' --parent_relationship_pred=isMemberOfCollection --type=directory ' . $this->source_dir_option . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    exec($ingest_cmd);
    $this->stopTimer($start_time, $this->quantity, 'basic images');
  }

}
