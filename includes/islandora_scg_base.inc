<?php

/**
 * @file
 * Abstract base class for Islandora Sample Content Generators.
 */

/**
 * Abstract sample content generator base class.
 */
abstract class IslandoraSampleContentGenerator {

  abstract protected function printMessage();

  public function __construct($options) {
    $this->quantity = $options['quantity'];
    $this->parent = $options['parent'];
    $this->content_model = $options['content_model'];
    $this->namespace = $options['namespace'];
  }

  public function printCommon() {
    print "Hi from a common function\n";
  }

}
