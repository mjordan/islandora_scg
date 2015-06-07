<?php

/**
 * @file
 * An implementation of a Basic Image content type sample content generator.
 */

/**
 * A class that generates sample Basic Image content.
 */
class IslandoraSampleBasicImageGenerator extends IslandoraSampleContentGenerator {
  public function generateContent() {
    for ($i = 1; $i <= $this->quantity - 1; $i++) {
      $metadata_ds = $this->generateMetadataDs();
      // Get the title to add to the image. Clunky.
      $mods = simplexml_load_string($metadata_ds);
      $title = $mods->{titleInfo}->title;
      $title = htmlspecialchars_decode($title);

      // Generate the OBJ datastream file.
      $cmd = "convert -size 1000x1000 xc:blue" . " " . $this->data_dir .
        DIRECTORY_SEPARATOR . $i . ".jpg;  convert -size 1000x1000 xc:blue" .
        " -pointsize 100 -fill white -gravity center -annotate +0+0 " .
        escapeshellarg($title) ." " . $this->data_dir . DIRECTORY_SEPARATOR . $i. ".jpg";
      // drush_print($cmd);
      drush_print("Generating sample content for $title");
      exec($cmd);

      $metadata_ds_file_path = $this->data_dir . DIRECTORY_SEPARATOR . $i . '.xml';
      $metadata_ds = $this->generateMetadataDs();
      file_put_contents($metadata_ds_file_path, $metadata_ds);
    }
  }

  public function loadContent() {
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_scan_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace;
    $batch_cmd .= ' --content_models=islandora:sp_basic_image --parent=' . $this->parent;
    $batch_cmd .= ' --parent_relationship_pred=isMemberOfCollection --type=directory --target=' . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    exec($ingest_cmd);
  }
}
