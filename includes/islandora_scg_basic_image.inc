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
        " -pointsize 100 -fill white -gravity center -annotate +0+0 '" .
        addslashes($title) ."' " . $this->data_dir . DIRECTORY_SEPARATOR . $i. ".jpg";
      drush_print("Generating OBJ file for " . addslashes($title));
      exec($cmd);

      $metadata_ds_file_path = $this->data_dir . DIRECTORY_SEPARATOR . $i . '.xml';
      $metadata_ds = $this->generateMetadataDs();
      file_put_contents($metadata_ds_file_path, $metadata_ds);
    }
  }

  public function loadContent() {
    //
  }
}
