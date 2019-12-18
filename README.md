# Islandora IIIF Manifest generator

This module generators a IIIF manifest based on the CModel of the Islandora object. 

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Collection](https://github.com/islandora/islandora_solution_pack_collection)



## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.


## Documentation

The module uses existing Islandora functions to determine the model. For each model the appropriate manifest is
generated.  Based on the IIIF Presentation API, see http://iiif.io/api/presentation/2.1/.


It provides responses for all IIIF Presentation API 2.1 types

In the form of:

http://localhost:8000/islandora_iiif_manifest/islandora%3A1/info.json

http://localhost:8182/iiif/2/1887.a%2Fislandora:1/info.json

