/**
 *    Copyright (c) 2002, WebThing Ltd
 *    Copyright (c) 2004 Edward Rudd
 *    Copyright (c) 2004 Paul Querna
 *    Authors:    Nick Kew <nick webthing.com>
 *                Edward Rudd <eddie at omegaware dot com>
 *                Paul Querna <chip at force-elite.com>
 *                Christian Parpart <trapni at gentoo.org>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef _MOD_TRANSFORM_PRIVATE_H
#define _MOD_TRANSFORM_PRIVATE_H

#include "mod_transform.h"

#define XSLT_FILTER_NAME "XSLT"

#define APACHEFS_FILTER_NAME "transform_store_brigade"

#include "http_config.h"
#include "http_protocol.h"
#include "http_core.h"
#include "http_log.h"
#include "http_request.h"
#include "apr_buckets.h"
#include "apr_strings.h"
#include "apr_uri.h"
#include "apr_tables.h"
#include "apr_dso.h"

#include <libxml/globals.h>
#include <libxml/threads.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>
#include <libxslt/xsltutils.h>
#include <libxslt/transform.h>
#include <libexslt/exslt.h>

/* Did I mention auto*foo sucks? */
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#include "mod_transform_config.h"

extern module AP_MODULE_DECLARE_DATA transform_module;

/* TransformOptions */
#define NO_OPTIONS          (1 <<  0)
#define USE_APACHE_FS       (1 <<  1)
#define XINCLUDES           (1 <<  2)

/* Extension Namespace */
#define TRANSFORM_APACHE_NAMESPACE ((const xmlChar *) "http://outoforder.cc/apache")

/* Static Style Sheet Caching */
typedef struct transform_xslt_cache
{
    const char *id;
    xsltStylesheetPtr transform;
    struct transform_xslt_cache *next;
}
transform_xslt_cache;

typedef struct transform_plugin_info {
    const char *name;
    int argc;
    const char **argv;

    apr_dso_handle_t *handle;
    mod_transform_plugin_t *plugin;

    struct transform_plugin_info *next;
}
transform_plugin_info_t;

typedef struct svr_cfg
{
    transform_xslt_cache *data;
    int announce;
    transform_plugin_info_t *plugins;
}
svr_cfg;

typedef struct dir_cfg
{
    const char *xslt;
    const char *default_xslt;
    apr_int32_t opts;
    apr_int32_t incremented_opts;
    apr_int32_t decremented_opts;
}
dir_cfg;

typedef struct
{
    const char *xslt;
    xmlDocPtr document;
}
transform_notes;


typedef struct
{
    ap_filter_t *next;
    apr_bucket_brigade *bb;
}
transform_xmlio_output_ctx;

int transform_xmlio_output_write(void *context, const char *buffer, int len);
int transform_xmlio_output_close(void *context);

typedef struct
{
    ap_filter_t *f;
    apr_pool_t *p;
    request_rec *rr;
    apr_bucket_brigade *bb;
}
transform_xmlio_input_ctx;

apr_status_t transform_apachefs_filter(ap_filter_t * f,
                                       apr_bucket_brigade * bb);

xmlParserInputBufferPtr transform_get_input(const char *URI,
                                            xmlCharEncoding enc);



void *transform_cache_get(svr_cfg * sconf, const char *descriptor);
apr_status_t transform_cache_free(void *conf);
const char *transform_cache_add(cmd_parms * cmd, void *cfg, const char *url,
                                const char *path);

#endif /* _MOD_TRANSFORM_PRIVATE_H */
/* vim:ai:et:ts=4:nowrap
 */
