/* @@@LICENSE
*
*      Copyright (c) 2008-2012 Hewlett-Packard Development Company, L.P.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* LICENSE@@@ */

#include <json-c/json.h>
#include <stdio.h>

#include "Common.h"
#include "ApplicationDescriptionBase.h"
#include "Utils.h"

ApplicationDescriptionBase::ApplicationDescriptionBase()
{
    m_entryPoint = "index.html";
    m_headLess = false;
    m_requestedWindowOrientation = "";
    m_flickable = false;
    m_internetConnectivityRequired = false;
    m_loadingAnimationDisabled = false;
    m_allowCrossDomainAccess = false;
}

ApplicationDescriptionBase::ApplicationDescriptionBase(const ApplicationDescriptionBase& other) :
    m_id(other.id()),
    m_title(other.title()),
    m_icon(other.icon()),
    m_miniIconName(other.miniicon()),
    m_entryPoint(other.entryPoint()),
    m_headLess(other.isHeadLess()),
    m_pluginName(other.pluginName()),
    m_flickable(other.isFlickable()),
    m_internetConnectivityRequired(other.isInternetConnectivityRequired()),
    m_urlsAllowed(other.urlsAllowed()),
    m_userAgent(other.userAgent()),
    m_loadingAnimationDisabled(other.isLoadingAnimationDisabled()),
    m_allowCrossDomainAccess(other.allowCrossDomainAccess())
{
}


bool ApplicationDescriptionBase::fromJsonObject(const struct json_object* root)
{
    bool success = true;

    success &= extractFromJson(root, "id", m_id);
    success &= extractFromJson(root, "title", m_title);

    // there is a possible fallback if there is no icon
    extractFromJson(root, "icon", m_icon);

    // the following properties are facultative, and shouldn't result in a failure
    extractFromJson(root, "miniicon", m_miniIconName);
    extractFromJson(root, "noWindow", m_headLess);
    extractFromJson(root, "requestedWindowOrientation", m_requestedWindowOrientation);
    extractFromJson(root, "main", m_entryPoint);
    extractFromJson(root, "flickable", m_flickable);
    extractFromJson(root, "internetConnectivityRequired", m_internetConnectivityRequired);
    extractFromJson(root, "urlsAllowed", m_urlsAllowed);
    extractFromJson(root, "plugin", m_pluginName);
    extractFromJson(root, "userAgent", m_userAgent);
    extractFromJson(root, "loadingAnimationDisabled", m_loadingAnimationDisabled);
    extractFromJson(root, "allowCrossDomainAccess", m_allowCrossDomainAccess);

    if(!success)
        fprintf(stderr,"ApplicationDescriptionBase::fromJsonString : error decodeing app description JSON string.\n" );

    return success;
}

void ApplicationDescriptionBase::getAppDescriptionString(std::string &descString) const
{
    json_object* json = getAppDescription();
    descString = json_object_to_json_string(json);
    json_object_put(json);
}

json_object* ApplicationDescriptionBase::getAppDescription() const
{
    // Compose json string from the app description object  -------------------------------
    // This will only include the integer and string fields of App Description

    json_object* json = json_object_new_object();

    struct json_object *json_urlAllowed = json_object_new_array ();
    std::list<std::string>::const_iterator constIterator;
    for (constIterator = urlsAllowed().begin(); constIterator != urlsAllowed().end(); ++constIterator) {
        json_object_array_add(json_urlAllowed, json_object_new_string((char*) (*constIterator).c_str()));
    }

    json_object_object_add(json, (char*) "id",   json_object_new_string((char*) m_id.c_str()));
    json_object_object_add(json, (char*) "main",   json_object_new_string((char*) m_entryPoint.c_str()));
    json_object_object_add(json, (char*) "noWindow",   json_object_new_boolean(m_headLess));
    json_object_object_add(json, (char*) "requestedWindowOrientation", json_object_new_string((char*) m_requestedWindowOrientation.c_str()));
    json_object_object_add(json, (char*) "title", json_object_new_string((char *) m_title.c_str()));
    json_object_object_add(json, (char*) "icon", json_object_new_string((char *) m_icon.c_str()));
    json_object_object_add(json, (char*) "miniicon", json_object_new_string((char *) m_miniIconName.c_str()));
    json_object_object_add(json, (char*) "flickable", json_object_new_boolean(m_flickable));
    json_object_object_add(json, (char*) "internetConnectivityRequired", json_object_new_boolean(m_internetConnectivityRequired));
    if( json_object_array_length(json_urlAllowed)>0 ) {
        json_object_object_add(json, (char*) "urlsAllowed", json_urlAllowed);
    }
    json_object_object_add(json, (char*) "plugin", json_object_new_string((char *) m_pluginName.c_str()));
    json_object_object_add(json, (char*) "userAgent", json_object_new_string((char *) m_userAgent.c_str()));
    json_object_object_add(json, (char*) "loadingAnimationDisabled", json_object_new_boolean(m_loadingAnimationDisabled));
    json_object_object_add(json, (char*) "allowCrossDomainAccess", json_object_new_boolean(m_allowCrossDomainAccess));

    return json;
}


