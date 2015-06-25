# Islandora Sample Content Generator

## Introduction

This module uses ImageMagick's `convert` command to generate sample images containing brief text, which are then ingested along with an accompanying MODS datastream using Islandora Batch's drush interface. It can generate and ingest objects with the following content models:

* Basic image (islandora:sp_basic_image)
* Large image (islandora:sp_large_image_cmodel)
* PDF (islandora:sp_pdf)
* Book, including pages (islandora:bookCModel)
* Newspaper, and newspaper issues, including pages (islandora:newspaperCModel and islandora:newspaperPageCModel)
* Collection (islandora:collectionCModel)

The images are very simple (some would say downright boring), just a colored background with the title of the object in white text as illustrated by this thumbnail datastream:

![Islandora Sample Content Generator thumbnail for images](https://dl.dropboxusercontent.com/u/1015702/linked_to/islandora_scg/islandora_scg_sample_tn.jpg)

Textual content (PDFs, books, and newspaper issues) has a white background with black text:

![Islandora Sample Content Generator thumbnail for paged content](https://dl.dropboxusercontent.com/u/1015702/linked_to/islandora_scg/islandora_scg_sample_tn_paged.jpg)

The images aren't meant to look good, they're intended to act as *lorem ipsum* content so you can test your site's configuration and behavior, or quickly populate an Islandora instance for training.

As an added bonus, since all sample content is loaded using Islandora's standard batch tools, all the standard derivatives are created.

## Requirements

* [Islandora Batch](https://github.com/Islandora/islandora_batch)
* To ingest books, you will need [Islandora Book Batch](https://github.com/Islandora/islandora_book_batch)
* To ingest newspaper issues, you will need [Islandora Newspaper Batch](https://github.com/mjordan/islandora_newspaper_batch)
* To purge objects using the 'iscgd' command, you will need [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)

If you don't have Book Batch or Newspaper Batch installed and try to generate sample book or newspaper content, or don't have Islandora Solr Search enabled and try to purge objects, drush will tell you that you need to enable the relevant module and exit.

ImageMagick must be installed, which is the case on most, if not all, Islandora servers.

## Usage

The Islandora Sample Content Generator provides two drush commands, one to generate and load objects, and one to purge those objects. There is no graphical user interface.

### Loading sample objects

To load sample objects, issue a command using the following template:

`drush iscgl --user=someuser --content_model=foo:contentModel --parent=bar:collection`

Some sample commands are:

* `drush iscgl --user=admin --quantity=20 --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection --namespace=testing`
* `drush iscgl --user=admin  --content_model=islandora:sp_pdf --parent=islandora:sp_pdf_collection --pages=10 --metadata_file=/tmp/metadata.tsv`

There is no need to run Islandora Batch's drush commands separately - the Content Generator does that for you.

Required parameters are:
* `--content_model` (the PID of the content model of the objects you want to create, taken from the list at the top of thie README)
* `--parent` (the PID of the parent, usually a collection, where you want to load the objects)

Optional parameters are:
* `--quantity` (how many sample objects to create; defaults to 5)
* `--namespace` (the namespace to use for the sample objects; defaults to "islandora")
* `--pages` (how many pages to add to PDFs, books, and newspaper issues; defaults to 4)
* `--bgcolor` (name of the background color for basic and large image content, from ImageMagick's [list of color names](http://www.imagemagick.org/script/color.php); defaults to "blue")
* `--metadata_file` (the absolute path to the TSV file containing metadata, described below; defaults to `includes/sample_metadata.tsv`).
* `--quantity_newspaper_issues` (number of issues to add to each newspaper; defaults to 0)

### Purging sample objects

To purge objects created with this module, issue a command with the following template:

`drush iscgd --user=someuser --content_model=foo:contentModel --parent=bar:collection`

You need to include at least one of `--content_model` or `--parent` parameters, although you may include both. If you want to purge *all* sample objects, omit both `--content model` and `--parent` and include the `--quantity=all` parameter. "all" is the only allowed value of `--quantity`.

Some sample commands are:

* `drush iscgd --user=admin --parent=islandora:sp_pdf_collection`
* `drush iscgd --user=admin --content_model=islandora:sp_pdf`
* `drush iscgd --user=admin --quantity=all`

The 'iscgd' comand does not prompt you to confirm your choices, so choose them wisely. Also remember that you can always delete sample objects using the regular tools Islandora provides - you don't need to use `iscgd`.

## Generating and loading newspapers

Islandora Sample Content Generator provides several options for creating newspaper content.

* You can generate and load newspapers, e.g., `drush iscgl --user=admin --quantity=2 --content_model=islandora:newspaperCModel --parent=islandora:newspaper_collection --namespace=islandora` will generate and load 2 newspapers in the collection with PID "islandora:newspaper_collection"
* You can generate and load issues into an existing newspaper, e.g., `drush iscgl --user=admin --quantity=2 --content_model=islandora:newspaperPageCModel --parent=testing:62 --namespace=testing` will generate and load two issues into the existing newspaper with PID "testing:62"
* You can generate and load newspapers and populate each one with issues at the same time, e.g., `drush iscgl --user=admin --quantity=2 --content_model=islandora:newspaperCModel --parent=islandora:newspaper_collection --namespace=islandora --quantity_newspaper_issues=4` will generate and load 2 newspapers into the collection with PID "islandora_newspaper_collection" and also generate and load 4 issues into each

Be careful with the last option, since high values in the `--quantity` and `--quantity_newspaper_issues` parameters can add up to a lot of batch generating and loading.

## Sample metadata

The metadata used for the sample objects is taken, at random, from `includes/sample_metadata.tsv` ([view it here](https://github.com/mjordan/islandora_scg/blob/7.x/includes/sample_metadata.tsv)). This metadata is derived from [a collection of early-20th century postcards](http://content.lib.sfu.ca/cdm/landingpage/collection/bcp)* depicting various landscapes and landmarks in British Columbia. Each tab-delimited record contains a title, a date of publication, one or more place names, one or more subject keywords, and a description.

If you want to use other metadata for your sample objects, you can replace this file with our own, as long as you follow the povided file's structure: five tab-separated columns: title, date, place name(s), subject keyword(s), and description. All columns are required but can be empty. Repeated place names and subject keywords are separated by semicolons. You can pass in the absolute path to your metadata file using the `--metadata_file` parameter. If you are using your own metadata file, lines from it will be picked at random, just like with the default metadata file.

You can gain additional control of the metadata for your sample objects by using Drupal's theming layer to completely override the way that the MODS datastream is populated. To do so, override the islandora_scg_preprocess_islandora_scg_metadata_ds() function and the islandora_scg_metadata_ds.tpl.php template file.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
* The sample metadata in includes/sample_metadata.tsv is distriubuted under the [Creative Commons Attribution-NonCommercial 3.0 Unported](http://creativecommons.org/licenses/by-nc/3.0/legalcode) license.

*The collection is currently hosted in CONTENTdm. It will be hosted in Islandora soon.
