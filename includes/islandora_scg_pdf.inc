<?php

/**
 * @file
 * An implementation of a PDF content type sample content generator.
 */

/**
 * A class that generates sample PDF content.
 */
class IslandoraSamplePdfGenerator extends IslandoraSampleContentGenerator {
  public function generateContent() {
   for ($i = 1; $i <= $this->quantity; $i++) {
      $metadata_ds = $this->generateMetadataDs();
      $metadata_ds_file_path = $this->data_dir . DIRECTORY_SEPARATOR . $i . '.xml';
      file_put_contents($metadata_ds_file_path, $metadata_ds);

      // Get the title to add to the image. Clunky.
      $mods = simplexml_load_string($metadata_ds);
      $title = $mods->{titleInfo}->title;
      $title = htmlspecialchars_decode($title);

      // Generate the OBJ datastream file.
      $cmd = "convert -size 612x792 xc:white ";
      $cmd .= $this->data_dir . DIRECTORY_SEPARATOR . $i . '-0.pdf; ';
      $cmd .= "convert -size 612x792 xc:white " . " -pointsize 50 -fill black ";
      $cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($title, 15));
      $cmd .=  " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '-0.pdf';
      drush_print("Generating sample content for $title");
      exec($cmd);

      // Generate two pages to add to the first one.
      for ($p = 1; $p <= 2; $p++) {
        $page_num = $p + 1;
        $text = $title . "\n\nPage " . $page_num;
        $page_cmd = "convert -size 612x792 xc:white ";
        $page_cmd .= $this->data_dir .  DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf; ';
        $page_cmd .= "convert -size 612x792 xc:white " . " -pointsize 40 -fill black ";
        $page_cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($text, 15));
        $page_cmd .=  " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf';
        exec($page_cmd);
      }

      // Join the files into a single 3-page PDF.
      $join_cmd = "convert -density 72x72 " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '-0.pdf';
      for ($p = 1; $p <= 2; $p++) {
        $join_cmd .= " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf';
      }
      $join_cmd .= " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '.pdf';
      exec($join_cmd);

      // Remove the page files, including page 0, so they won't get loaded.
      for ($p = 0; $p <= 2; $p++) {
        $file_to_delete = $this->data_dir . DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf';
        if (file_exists($file_to_delete)) {
          drupal_unlink($file_to_delete);
        }
      }
    }
  }

  public function loadContent() {
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_scan_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace;
    $batch_cmd .= ' --content_models=islandora:sp_pdf --parent=' . $this->parent;
    $batch_cmd .= ' --parent_relationship_pred=isMemberOfCollection --type=directory --target=' . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    exec($ingest_cmd);
  }

}
