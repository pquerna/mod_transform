/**
 *    Copyright (c) 2002, WebThing Ltd
 *    Copyright (c) 2004 Edward Rudd
 *    Copyright (c) 2004 Paul Querna
 *    Copyright (c) 2007 Christian Parpart
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
 
#ifndef _MOD_TRANSFORM_H
#define _MOD_TRANSFORM_H
 
#include <httpd.h>
#include <libxml/tree.h>
#include <libxslt/xslt.h>

#ifdef __cplusplus
extern "C" {
#endif
 
void mod_transform_set_XSLT(request_rec* r, const char* name) ;
void mod_transform_XSLTDoc(request_rec* r, xmlDocPtr doc) ;

typedef struct {
    int (*plugin_init)(apr_pool_t *p, int argc, const char **argv);
    int (*post_config)(apr_pool_t *p, int argc, const char **argv);
    void (*child_init)(apr_pool_t *p, server_rec *s);
    void (*filter_init)(struct ap_filter_t *filter);
    void (*transform_run_begin)(struct ap_filter_t *filter);
    void (*transform_run_end)(struct ap_filter_t *filter);
} mod_transform_plugin_t;

#ifdef __cplusplus
}
#endif
 
#endif
/* vim:ai:et:ts=4:nowrap
 */
