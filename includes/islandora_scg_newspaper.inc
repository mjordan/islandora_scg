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
    if (!$this->quantity_newspaper_issues) {
      $start_time = $this->startTimer();
    }
    $batch_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_scan_preprocess';
    $batch_cmd .= ' --namespace=' . $this->namespace;
    $batch_cmd .= ' --content_models=islandora:newspaperCModel --parent=' . $this->parent;
    $batch_cmd .= ' --parent_relationship_pred=isMemberOfCollection --type=directory ' . $this->source_dir_option . $this->data_dir;
    exec($batch_cmd);
    $ingest_cmd = 'drush -u 1 --uri=http://localhost islandora_batch_ingest';
    drush_shell_exec($ingest_cmd);
    $newspaper_load_result = drush_shell_exec_output();

    $newspaper_pids = array();
    foreach ($newspaper_load_result as $ingested_status) {
      // Parsing Islandora Batch's output to get the PIDs
      // of the loaded newspapers objects.
      if (preg_match('/^Ingested\s/', $ingested_status)) {
        $ingested_status = preg_replace('/^Ingested\s/', '', $ingested_status);
        $ingested_status = preg_replace('/\..*$/', '', $ingested_status);
        $newspaper_pids[] = trim($ingested_status);
      }
    }
    $num_newspapers = count($newspaper_pids);
    if (!$this->quantity_newspaper_issues) {
      $this->stopTimer($start_time, $num_newspapers, 'newspapers');
    }

    // Load issues into each newspaper that was ingested.
    if ($this->quantity_newspaper_issues) {
      $this->child_uses_timer = FALSE;
      $start_time = $this->startTimer();
      drush_print(dt("Loading !num_issues issues into each newspaper.",
        array('!num_issues' => $this->quantity_newspaper_issues)));
      foreach ($newspaper_pids as $newspaper_pid) {
        drush_print("Loading issues into $newspaper_pid.");
        exec("drush iscgl --user=admin --quantity=$this->quantity_newspaper_issues --content_model=islandora:newspaperPageCModel --parent=$newspaper_pid --namespace=$this->namespace");
      }
      $this->stopTimer($start_time, $this->quantity_newspaper_issues, 'newspaper issues', 'with ' . $this->pages . ' pages per issue');
    }
    else {
      drush_print("Loading newspapers with no issues.");
    }
  }

}
