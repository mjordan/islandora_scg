<?php

/**
 * @file
 * An implementation of a newspaper content type sample content generator.
 *
 * Newspaper issues are handled by a separate class, IslandoraSampleNewspaperIssueGenerator.
 */

/**
 * A class that generates sample newspaper (not newspaper issue) content.
 */
class IslandoraSampleNewspaperGenerator extends IslandoraSampleContentGenerator {
  public function generateContent() {
    for ($i = 1; $i <= $this->quantity; $i++) {
      $metadata_ds = $this->generateMetadataDs('text');
      $metadata_ds_file_path = $this->data_dir . DIRECTORY_SEPARATOR . $i . '.xml';
      file_put_contents($metadata_ds_file_path, $metadata_ds);

      // Get the title to add to the image.
      $mods = simplexml_load_string($metadata_ds);
      $title = $mods->titleInfo[0]->title;
      $text = htmlspecialchars_decode($title);

      // Generate the OBJ datastream file. Newapapers don't need this according to
      // their content model but we add it so keep Islandora Batch happy. Plus this
      // image is used to create the thumbnail.
      $cmd = "convert -size 1000x1000 xc:white ";
      $cmd .= $this->data_dir .  DIRECTORY_SEPARATOR . $i . '.jpg; ';
      $cmd .= "convert -size 1000x1000 xc:white -pointsize 100 -fill black ";
      $cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($text, 15));
      $cmd .=  " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . ".jpg";
      drush_print("Generating sample content for $title");
      exec($cmd);
    }
  }

  public function loadContent() {
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_scan_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace;
    $batch_cmd .= ' --content_models=islandora:newspaperCModel --parent=' . $this->parent;
    $batch_cmd .= ' --parent_relationship_pred=isMemberOfCollection --type=directory --target=' . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    drush_shell_exec($ingest_cmd);
    $result = drush_shell_exec_output();
    print_r($result);
  }

}
