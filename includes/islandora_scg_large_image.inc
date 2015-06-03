<?php

/**
 * @file
 * An implementation of a Large Image content type sample content generator.
 */

/**
 * A class that generates sample Large Image content.
 */
class IslandoraSampleLargeImageGenerator extends IslandoraSampleContentGenerator {
  public function printMessage() {
    print "Hi from large image. You asked for " . $this->quantity . " objects.\n";
  }
}
