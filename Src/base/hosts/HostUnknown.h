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

#ifndef HOSTUNKNOWN_H_
#define HOSTUNKNOWN_H_

#include <QObject>
#include "HostBase.h"
#include "NyxInputControl.h"

class HostUnknown : public HostBase
{
	Q_OBJECT
public:
	HostUnknown() : m_keys(0), m_keysTried(false) {}
	virtual ~HostUnknown() { delete m_keys; }

	virtual void init(int w, int h) { }
	virtual const char* hardwareName() const { return "unknown"; }

	// An emulator or VM has no HostArm, but it can still have the keys a
	// device has - a VM's ACPI power button is a KEY_POWER input device -
	// and LunaDisplayManager only learns about the power key through this
	// control. Hand it out when nyx has a keys device to open; where there is
	// none (a desktop) keep returning 0, as before.
	virtual InputControl* getInputControlKeys()
	{
		if (!m_keysTried) {
			m_keysTried = true;
			NyxInputControl* keys = new NyxInputControl(NYX_DEVICE_KEYS, "Main");
			if (keys->getHandle())
				m_keys = keys;
			else
				delete keys;
		}
		return m_keys;
	}

private:
	NyxInputControl* m_keys;
	bool m_keysTried;
};

#endif
