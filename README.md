# Islandora Sample Content Generator

## Introduction

This module uses ImageMagick's convert command to generate sample images containing brief text, which are then ingested using Islandora Batch's drush interface. It can generate objects of the following content models:

* Basic image
* Large image
* PDF
* Book (in development)
* Newspaper (in development)

## Requiredments

* [Islandora Batch](https://github.com/Islandora/islandora_batch)

ImageMagick must be installed on the server. It is by default installed on most, if not all, Islandora servers.

## Usage

The Islandora Sample Content Generator only has a drush interface. To use it, issue the following command:

```drush iscgl --user=admin --content_model=foo:contentModel --parent=bar:collection```

Some sample commands are:

* `sudo drush iscgl --user=admin --quantity=20 --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection --namespace=foo`
* `sudo drush iscgl --user=admin  --content_model=islandora:sp_pdf --parent=islandora:sp_pdf_collection --pages=10 --namespace=testing`

Since this drush command creates a directory in your Drupal site's public files folder, you will need to run it as sudo, or make your files directory writable by the user running the command. The directory is deleted after the content is loaded.

Optional parameters include `--quantity` (how many sample objects to create; defaults to 5), `--namespace` (the namespace to use for the sample objects; defaults to 'islandora'), and `--pages` (how many pages to add to newspapers, books, and PDFs; defaults to 4).

## Metadata

The metadata used for the sample objects is taken, at random, from `includes/sample_metadata.tsv`. This metadata is derived from a collection of early-20th century postcards depicting various parts of British Columbia. Each tab-delimited record contains a title, a date of publication, one or more place names, one or more subject keywords, and a description.

If you want to use other metadata for your sample objects, you can replace this file with our own, as long as you follow the povided file's structure: five tab-separated columns (title, date, place name(s), subject keyword(s), and description. Repeated place names and subject keywords are separated by semicolons.

If you want to go further than just replacing the sample metadata file, you can use Drupal's normal theming layer to completely override the way that the MODS datastream is populated. Just override the islandora_scg_preprocess_islandora_scg_metadata_ds() function and islandora_scg_metadata_ds.tpl.php template file.

## Maintainers/Sponsors

Current maintainers:

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)


