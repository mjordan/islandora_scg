<?php
/**
 * @file
 * Template file for the Islandora Sample Content Generator metadata
 * MODS datastream. 
 * 
 * Available variables:
 *   $title
 *     The title of the sample object.
 */
?>
<?xml version="1.0" encoding="UTF-8"?>
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
    <titleInfo>
      <title><?php print $title; ?></title>
    </titleInfo>
    <originInfo>
      <place>
        <placeTerm></placeTerm>
      </place>
      <publisher></publisher>
      <dateIssued encoding="iso8601">1928-06-01</dateIssued>
    </originInfo>
    <language>
      <languageTerm>eng</languageTerm>
    </language>
    <subject>
      <topic></topic>
      <topic></topic>
    </subject>
    <identifier></identifier>
</mods>
