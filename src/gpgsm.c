/* gpgsm.c - Talking to gpgsm.
   Copyright (C) 2006, 2008 g10 Code GmbH

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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <locale.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <assuan.h>
#include <gpg-error.h>

#include "cryptoki.h"
#include "support.h"
#include "cert.h"
#include "agent.h"
#include "gpgsm.h"
#include "debug.h"


struct search
{
  bool found;
  cert_get_cb_t cert_get_cb;
  void *hook;
  bool with_chain;
};


static gpg_error_t
search_cb (void *hook, struct cert *cert)
{
  struct search *ctx = hook;
  gpg_error_t err = 0;

  CK_ATTRIBUTE_PTR attrp;
  CK_ULONG attr_countp;

  /* Add the private key object only once.  */
  if (!ctx->found)
    {
      err = scute_attr_prv (cert, &attrp, &attr_countp);
      if (err)
	return err;

      err = (*ctx->cert_get_cb) (ctx->hook, attrp, attr_countp);
      if (err)
	{
	  scute_attr_free (attrp, attr_countp);
	  return err;
	}

      ctx->found = true;
    }

  /* Add the certificate chain recursively before adding the
     certificate.  But ignore errors.  If the chain is incomplete, we
     might still be able to proceed, for example with client
     authentication.  */
  if (ctx->with_chain && strcmp (cert->chain_id, cert->fpr))
    scute_gpgsm_search_certs_by_fpr (cert->chain_id, search_cb, ctx);

  /* Turn this certificate into a certificate object.  */
  err = scute_attr_cert (cert, &attrp, &attr_countp);
  if (err)
    return err;
  
  err = (*ctx->cert_get_cb) (ctx->hook, attrp, attr_countp);
  if (err)
    {
      scute_attr_free (attrp, attr_countp);
      return err;
    }

  return err;
}


/* Create the attributes required for a new certificate object.
   Returns allocated attributes for the certificate object in ATTRP
   and ATTR_COUNTP, and for the private key object in PRV_ATTRP
   and PRV_ATTR_COUNTP.  */
gpg_error_t
scute_gpgsm_get_cert (char *grip, int no, cert_get_cb_t cert_get_cb, void *hook)
{
  gpg_error_t err;
  struct search search;

  search.found = false;
  search.cert_get_cb = cert_get_cb;
  search.hook = hook;
  search.with_chain = false;

  /* If the key is from the card, we might get the certificate from
     the card as well.  */
  if (no >= 0)
    {
      struct cert cert;

      memset (&cert, '\0', sizeof (cert));
      err = scute_agent_get_cert (no, &cert);
      if (! err)
	{
#if 0
	  /* For now, we don't need no stinking chain.  */

	  /* As we only have the DER certificate from the card, we need to
	     parse that and fill out the missing info and try to get the
	     certificate chain from gpgsm.  */
	  err = scute_cert_from_der (&cert);
#endif
	  if (! err)
	    err = search_cb (&search, &cert);
	  return err;
	}
    }
  
  search.with_chain = true;
  err = scute_gpgsm_search_certs_by_grip (grip, search_cb, &search);
  return err;
}
