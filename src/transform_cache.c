/**
 *    Copyright (c) 2002 WebThing Ltd
 *    Copyright (c) 2004 Edward Rudd
 *    Copyright (c) 2004 Paul Querna
 *    Authors:    Nick Kew <nick webthing.com>
 *                Edward Rudd <urkle at outoforder dot com>
 *                Paul Querna <chip at outoforder dot com>
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

#include "mod_transform_private.h"

void *transform_cache_get(svr_cfg * sconf, const char *descriptor)
{
    transform_xslt_cache *p;
    if (!descriptor)
        return 0;

    for (p = sconf->data; p; p = p->next) {
        if (!strcmp(descriptor, p->id)) {
            return p->transform;
        }
    }

    return 0;
}

const char *transform_cache_add(cmd_parms * cmd, void *cfg,
                                            const char *url, const char *path)
{
    svr_cfg *conf = ap_get_module_config(cmd->server->module_config,
                                         &transform_module);
    xsltStylesheetPtr xslt = xsltParseStylesheetFile(path);
    if (url && path && xslt) {
        transform_xslt_cache *me =
            apr_palloc(cmd->pool, sizeof(transform_xslt_cache));
        me->id = apr_pstrdup(cmd->pool, url);
        me->transform = xslt;
        me->next = conf->data;
        conf->data = me;
        ap_log_perror(APLOG_MARK, APLOG_NOTICE, 0, cmd->pool,
                      "mod_transform: Cached Precompiled XSL: %s", url);
        return NULL;
    }
    else {
        ap_log_perror(APLOG_MARK, APLOG_ERR, 0, cmd->pool,
                      "mod_transform: Error fetching or compiling XSL from: %s",
                      path);
        return "Error trying to precompile XSLT";
    }
}

apr_status_t transform_cache_free(void *conf)
{
    transform_xslt_cache *p;
    svr_cfg *cfg = conf;
    for (p = cfg->data; p; p = p->next) {
        xsltFreeStylesheet(p->transform);
    }
    return APR_SUCCESS;
}

