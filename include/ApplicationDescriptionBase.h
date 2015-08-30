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

#ifndef APPLICATIONDESCRIPTIONBASE_H
#define APPLICATIONDESCRIPTIONBASE_H

#include <string>
#include <QObject>

#include "Common.h"

struct json_object;

class ApplicationDescriptionBase: public QObject
{
public:
    enum Type {
        Type_Web = 0,
        Type_Native,
        Type_PDK,
        Type_SysmgrBuiltin,
        Type_Qt,
        Type_QML
    };

    ApplicationDescriptionBase();
    ApplicationDescriptionBase(const ApplicationDescriptionBase& other);
    virtual ~ApplicationDescriptionBase() {}
    const std::string& id()         const { return m_id; }
    const std::string& title()         const { return m_title; }
    const std::string& icon()       const { return m_icon; }
    const std::string& entryPoint() const { return m_entryPoint; }
    bool               isHeadLess() const { return m_headLess; }
    const std::string& requestedWindowOrientation() { return m_requestedWindowOrientation; }
    const std::string& pluginName() const { return m_pluginName; }
    bool               isFlickable() const { return m_flickable; }
    bool               isInternetConnectivityRequired() const { return m_internetConnectivityRequired; }
    const std::list<std::string>& urlsAllowed() const { return m_urlsAllowed; }
    const std::string& userAgent() const { return m_userAgent; }
    bool               isLoadingAnimationDisabled() const { return m_loadingAnimationDisabled; }
    bool               allowCrossDomainAccess() const { return m_allowCrossDomainAccess; }


    virtual void getAppDescriptionString(std::string &descString) const;
protected:
    // Gives an json_object filled by information within this class
    virtual json_object* getAppDescription() const;
    // populates the base class with data from the json object
    bool fromJsonObject(const struct json_object* json);

    std::string             m_id;
    std::string             m_title;    //copy of default launchpoint's title
    std::string             m_icon;
    std::string             m_entryPoint;
    bool                    m_headLess;
    std::string             m_requestedWindowOrientation;
    std::string             m_pluginName;
    bool                    m_flickable;
    bool                    m_internetConnectivityRequired;
    std::list<std::string>  m_urlsAllowed;
    std::string             m_userAgent;
    bool                    m_loadingAnimationDisabled;
    bool                    m_allowCrossDomainAccess;
};

#endif // APPLICATIONDESCRIPTIONBASE_H
