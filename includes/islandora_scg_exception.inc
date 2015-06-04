<?php

/**
 * @file
 * Custom exception class for the Islandora Sample Content Generator.
 */

/**
 * A class that defines a custom exception for the Islandora Sample
 * Content Generator.
 */
class IslandoraSampleContentGeneratorException extends Exception {

  public function __construct($message, $code = 0, Exception $previous = null) {
    
        parent::__construct($message, $code, $previous);
  }

  public function __toString() {
    return __CLASS__ . ": [{$this->code}]: {$this->message}\n";
  }

}
