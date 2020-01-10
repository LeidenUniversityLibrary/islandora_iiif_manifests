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
  jQuery('.ubl-detail-tools LI A .fa-eye').parent().click(function (e) {
    var $li = jQuery(this).parent();
    var $additionalblock = jQuery('DIV.iiif-additional');
    if ($additionalblock.size() > 0) {
      e.preventDefault();
      if ($additionalblock.is(':visible')) {
        return; // is already visible
      }

      var x = $li.offset().left - $additionalblock.outerWidth() + $li.outerWidth();
      if (x > 0) {
        $additionalblock.css('left', x + 'px');
      }
      var y = $li.offset().top + $li.outerHeight();
      $additionalblock.css('top', y + 'px');

      e.stopPropagation();
      jQuery('BODY').one('click', function() {
        $additionalblock.slideUp();
      });
      $additionalblock.slideDown();
    }
  });
})
