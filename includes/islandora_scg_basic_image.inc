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
      // Generate the OBJ datastream file.
      $cmd = "convert -size 1000x1000 xc:blue" . " " . $this->data_dir .
        DIRECTORY_SEPARATOR . $i . ".jpg;  convert -size 1000x1000 xc:blue" .
        " -pointsize 200 -fill white -gravity center -annotate +0+0 '" .
        "Object " . $i . "' " . $this->data_dir . DIRECTORY_SEPARATOR . $i. ".jpg";
      drush_print("Generating OBJ file for $i...");
      exec($cmd);

      // Generate metadata datastream file.
      $metadata_ds = theme('metadata_ds', array(
          'title' => "Item " . $i,
      ));
      $metadata_ds_file_path = $this->data_dir . DIRECTORY_SEPARATOR . $i . '.xml';
      file_put_contents($metadata_ds_file_path, $metadata_ds);
    }
  }

  public function loadContent() {
    //
  }
}
