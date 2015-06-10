# Islandora Sample Content Generator

## Introduction

This module uses ImageMagick's `convert` command to generate sample images containing brief text, which are then ingested along with an accompanying MODS datastream using Islandora Batch's drush interface. It can generate objects of the following content models:

* Basic image
* Large image
* PDF
* Book (in development)
* Newspaper (in development)

The images are very simple, just a colored background with the title of the object in white text as illustrated by this thumbnail datastream:

![Islandora Sample Content Generator thumbnail for images](https://dl.dropboxusercontent.com/u/1015702/linked_to/islandora_scg/islandora_scg_sample_tn.jpg)

Paged content (including PDFs) has a white background with black text:

![Islandora Sample Content Generator thumbnail for paged content](https://dl.dropboxusercontent.com/u/1015702/linked_to/islandora_scg/islandora_scg_sample_tn_paged.jpg)

## Requiredments

* [Islandora Batch](https://github.com/Islandora/islandora_batch)
* To ingest books, you will need [Islandora Book Batch](https://github.com/Islandora/islandora_book_batch)
* To ingest newspaper issues, you will need [Islandora Newspaper Batch](https://github.com/mjordan/islandora_newspaper_batch)

ImageMagick must be installed on the server, which is the case on most, if not all, Islandora servers.

## Usage

The Islandora Sample Content Generator only has a drush interface. To use it, issue the following command:

`drush iscgl --user=someuser --content_model=foo:contentModel --parent=bar:collection`

Some sample commands are:

* `sudo drush iscgl --user=admin --quantity=20 --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection --namespace=testing`
* `sudo drush iscgl --user=admin  --content_model=islandora:sp_pdf --parent=islandora:sp_pdf_collection --pages=10`

There is no need to run Islandora Batch's drush commands separately - the Content Generator does that for you. Since the Content Generator's drush command creates a directory in your Drupal site's public files folder, you will need to run it as sudo, or make your files directory writable by the user running the command. The directory is deleted after the content is loaded.

Optional parameters include `--quantity` (how many sample objects to create; defaults to 5), `--namespace` (the namespace to use for the sample objects; defaults to 'islandora'), `--pages` (how many pages to add to newspapers, books, and PDFs; defaults to 4), and `-bgcolor` (name of the background color for basic and large image content, from the ImageMagick's "[list of color names](http://www.imagemagick.org/script/color.php)").

## Sample metadata

The metadata used for the sample objects is taken, at random, from `includes/sample_metadata.tsv` ([view it here](https://github.com/mjordan/islandora_scg/blob/7.x/includes/sample_metadata.tsv)). This metadata is derived from [a collection of early-20th century postcards](http://content.lib.sfu.ca/cdm/landingpage/collection/bcp)* depicting various landscapes and landmarks in British Columbia. Each tab-delimited record contains a title, a date of publication, one or more place names, one or more subject keywords, and a description.

If you want to use other metadata for your sample objects, you can replace this file with our own, as long as you follow the povided file's structure: five tab-separated columns: title, date, place name(s), subject keyword(s), and description. Repeated place names and subject keywords are separated by semicolons.

If you want additional control of the metadata for your sample objects, you can use Drupal's theming layer to completely override the way that the MODS datastream is populated. Just override the islandora_scg_preprocess_islandora_scg_metadata_ds() function and the islandora_scg_metadata_ds.tpl.php template file.

## Maintainers/Sponsors

Current maintainers:

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
* The sample metadata in includes/sample_metadata.tsv is distriubuted under the [Creative Commons attribution, non-commercial license](http://creativecommons.org/licenses/by-nc/3.0/legalcode).

*The collection is currently hosted in CONTENTdm. It will be hosted in Islandora soon.
