# Makefile.am - Doc directory Makefile for scute.
# Copyright (C) 2006, 2007 g10 Code GmbH
# 
# This file is part of scute.
#
# Scute is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Scute is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Scute; if not, write to the Free Software Foundation,
# Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
#
# In addition, as a special exception, g10 Code GmbH gives permission
# to link this library: with the Mozilla Foundation's code for
# Mozilla (or with modified versions of it that use the same license
# as the "Mozilla" code), and distribute the linked executables.  You
# must obey the GNU General Public License in all respects for all of
# the code used other than "Mozilla".  If you modify this file, you
# may extend this exception to your version of the file, but you are
# not obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.

## Process this file with automake to produce Makefile.in

DISTCLEANFILES = scute.tmp

images = firefox-cm.png firefox-cm-view-detail.png firefox-cm-view.png \
	firefox-dm-load-after.png firefox-dm-load-before.png \
	firefox-dm-load.png firefox-dm-token-present.png firefox-pref.png \
	firefox-pref-view.png firefox-bad-pin.png \
	thunderbird-account-settings.png thunderbird-smime-button.png \
	libreoffice-certificate-selection.png \
	libreoffice-digital-signatures.png libreoffice-pdf-signature.png

images_eps = $(images:.png=.eps)

EXTRA_DIST = $(images)
CLEANFILES = $(images_eps)

info_TEXINFOS = scute.texi
scute_TEXINFOS = gpl.texi

scute.html: scute.texi $(foreach i,$(images),scute.html/$(i))
	$(MAKEINFO) --html --output "$@" "$<"
scute.html/%.png: %.png
	@mkdir -p scute.html
	cp -v "$<" "$@"

scute.dvi: $(images_eps)
scute.pdf: $(images)

%.eps : %.png
	$(CONVERT) `test -f '$<' || echo '$(srcdir)/'`$< $@
