# Islandora Sample Content Generator

## Design notes

https://github.com/mjordan/islandora_groovy demonstrates a method for using ImageMagick's convert command to generate images with text and then loading those images using Islandora Batch's drush interface. Given these abilities, we can in theory generate and load sample content conforming to the following content models:

* Basic image
* Large image
* PDF
* Paged content
* Book
* Newspaper

Some design considerations for the Islandora Sample Content Generator:

* The module should provide a Drush and UI interface for creating content and loading it into and Islandora instance.
* The module should provide configuration options for the text to be inserted into images, and should also provide a sample image that can be used as the 'template' for sample content.
* The module should provide a template MODS file that is themeable so that it can be overridden if desired.
* The module should provide a way of adding additional content models and batch loaders, perhaps via a plugin system (e.g. implemented as one content model per class file).
