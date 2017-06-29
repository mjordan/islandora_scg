<?php
/**
 * @file
 * Template file for the MODS datastream.
 *
 * Available variables:
 *   $title string
 *     The title of the sample object.
 *   $date_issued string
 *     The date issued of the sample object.
 *   $abstract string
 *     The abstract of the sample object.
 *   $topics array
 *     The 'topic' subjects terms of the sample object.
 *   $geographics array
 *     The 'geographic' subjects terms of the sample object.
 *   $genre string
 *     The genre (type) of the sample object.
 */
?>
<?xml version="1.0" encoding="UTF-8"?>
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
    <recordInfo>
      <!-- If you create a custom template, be sure to include this recordOrigin element. -->
      <recordOrigin>Islandora Sample Content Generator.</recordOrigin>
    </recordInfo>
    <titleInfo>
      <title><?php print $title; ?></title>
    </titleInfo>
    <originInfo>
      <place>
        <placeTerm></placeTerm>
      </place>
      <publisher></publisher>
      <dateIssued encoding="iso8601"><?php print $date_issued; ?></dateIssued>
    </originInfo>
    <abstract><?php print $abstract; ?></abstract>
    <language>
      <languageTerm>eng</languageTerm>
    </language>
    <genre><?php print $genre; ?></genre>
    <subject>
    <?php if (count($topics)): ?>
      <?php foreach($topics as $topic): ?>
      <topic><?php print $topic; ?></topic>
      <?php endforeach; ?>
      <?php endif; ?>
      <?php if (count($geographics)): ?>
      <?php foreach($geographics as $geographic): ?>
      <geographic><?php print $geographic; ?></geographic>
      <?php endforeach; ?>
    <?php endif; ?>
    </subject>
    <identifier></identifier>
</mods>
