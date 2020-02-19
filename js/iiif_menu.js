/**
 * @file
 * js/iiif_menu.js
 *
 *
 *  Copyright 2019 Leiden University Library
 *
 *  This file is part of iiif_menu.
 *
 *  iiif_menu is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

jQuery(document).ready(function() {
  var $iiifbutton = jQuery('DIV.iiifbutton');
  var $metadataTable = jQuery('.dc-box .dc-metadata .islandora-metadata-fields');
  if ($metadataTable.size() > 0 && $iiifbutton.size() > 0) {
    var iiifManifest = $iiifbutton.data('manifest');
    var iiifUrl = $iiifbutton.find('IMG').attr('src');

    if (typeof iiifUrl === 'string' && iiifUrl.length > 0 && typeof iiifManifest === 'string' && iiifManifest.length > 0) {
      $metadataTable.find('TBODY').append('<TR><TH>IIIF manifest</TH><TD class="iiifmanifest"><A href="' + iiifManifest + '" target="_blank"><IMG src="' + iiifUrl + '"/> manifest</A></TD></TR>');
    }
  }
})
