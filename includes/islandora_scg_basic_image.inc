<?php

/**
 * @file
 * An implementation of a Basic Image content type sample content generator.
 */

/**
 * A class that generates sample Basic Image content.
 */
class IslandoraSampleBasicImageGenerator extends IslandoraSampleContentGenerator {

  public function printMessage() {
    print "Hi from basic image. You asked for " . $this->quantity . " objects.\n";
  }

}
