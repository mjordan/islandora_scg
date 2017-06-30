<?php

/**
 * @file
 * An implementation of a PDF content type sample content generator.
 */

/**
 * A class that generates sample PDF content.
 */
class IslandoraSamplePdfGenerator extends IslandoraSampleContentGenerator {
  /**
   * Generates the the sample PDF content.
   */
  public function generateContent() {
   for ($i = 1; $i <= $this->quantity; $i++) {
      $metadata_ds = $this->generateMetadataDs('text');
      $metadata_ds_file_path = $this->data_dir . DIRECTORY_SEPARATOR . $i . '.xml';
      file_put_contents($metadata_ds_file_path, $metadata_ds);

      // Get the title to add to the image. Clunky.
      $mods = simplexml_load_string($metadata_ds);
      $title = $mods->titleInfo[0]->title;
      $title = htmlspecialchars_decode($title);

      // Generate the OBJ datastream file.
      if (function_exists('drush_print')) {
        drush_print("Generating sample content for $title");
      }
      if ($this->pages == 1) {
	$text = $title . "\n\nPage 1 / 1";
        $cmd = "convert -size 612x792 xc:white ";
        $cmd .= $this->data_dir . DIRECTORY_SEPARATOR . $i . '.pdf; ';
        $cmd .= "convert -size 612x792 xc:white " . " -pointsize 50 -fill black ";
        $cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($text, 15));
        $cmd .=  " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '.pdf';
        exec($cmd);
      }
      else {
	// Generate the designated number of pages.
	for ($p = 1; $p <= $this->pages; $p++) {
	  $text = $title . "\n\nPage $p / " . $this->pages;
	  $page_cmd = "convert -size 612x792 xc:white ";
	  $page_cmd .= $this->data_dir .  DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf; ';
	  $page_cmd .= "convert -size 612x792 xc:white " . " -pointsize 50 -fill black ";
	  $page_cmd .= "-gravity center -annotate +0+0 " .  escapeshellarg(wordwrap($text, 15));
	  $page_cmd .=  " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf';
	  exec($page_cmd);
	}

	// Join the individual files into a single multi-page PDF.
	$join_cmd = "convert -density 72x72 ";
	for ($p = 1; $p <= $this->pages; $p++) {
	  $join_cmd .= " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf';
	}
	$join_cmd .= " " . $this->data_dir . DIRECTORY_SEPARATOR . $i . '.pdf';
	exec($join_cmd);

	// Remove the individual page files so they won't get ingested.
	for ($p = 1; $p <= $this->pages; $p++) {
	  $file_to_delete = $this->data_dir . DIRECTORY_SEPARATOR . $i . '-' . $p . '.pdf';
	  if (file_exists($file_to_delete)) {
	    drupal_unlink($file_to_delete);
	  }
	}
      }
    }
  }

  /**
   * Runs the Islandora Batch drush commands to ingest the sample content.
   */
  public function loadContent() {
    $start_time = $this->startTimer();
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_scan_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace;
    $batch_cmd .= ' --content_models=islandora:sp_pdf --parent=' . $this->parent;
    $batch_cmd .= ' --parent_relationship_pred=isMemberOfCollection --type=directory ' . $this->source_dir_option . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    exec($ingest_cmd);
    $this->stopTimer($start_time, $this->quantity, 'PDFs with ' . $this->pages . 'pages per PDF');
  }

}
