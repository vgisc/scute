/* error-mapping.c - Scute error mapping interface.
   Copyright (C) 2006 g10 Code GmbH

   This file is part of Scute.
 
   Scute is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Scute is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Scute; if not, write to the Free Software Foundation,
   Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

   In addition, as a special exception, g10 Code GmbH gives permission
   to link this library: with the Mozilla Foundation's code for
   Mozilla (or with modified versions of it that use the same license
   as the "Mozilla" code), and distribute the linked executables.  You
   must obey the GNU General Public License in all respects for all of
   the code used other than "Mozilla".  If you modify this file, you
   may extend this exception to your version of the file, but you are
   not obligated to do so.  If you do not wish to do so, delete this
   exception statement from your version.  */

#ifndef ERROR_MAPPING_H
#define ERROR_MAPPING_H	1

#include <errno.h>

#include <gpg-error.h>

#include "cryptoki.h"

/* Map a system error code to a cryptoki return value.  */
CK_RV scute_sys_to_ck (int err);

/* Map a GnuPG error code to a cryptoki return value.  */
CK_RV scute_gpg_err_to_ck (gpg_error_t err);

#endif /* !ERROR_MAPPING_H */
