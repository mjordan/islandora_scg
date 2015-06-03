<?php

/**
 * @file
 * An implementation of a PDF content type sample content generator.
 */

/**
 * A class that generates sample PDF content.
 */
class IslandoraSamplePdfGenerator extends IslandoraSampleContentGenerator {
  public function printMessage() {
    print "Hi from PDF. You asked for " . $this->quantity . " objects.\n";
  }
}
