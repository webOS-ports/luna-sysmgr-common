/**
 *******************************************************************************
 * @file NyxLedControl.cpp
 *
 * Copyright (c) 2010 Hewlett-Packard Development Company, L.P.
 * All rights reserved.
 *
 *
 *******************************************************************************
 */

#include "NyxLedControl.h"
#include <glib.h>
#include <string.h>


// nyx offers no way to cancel an in-flight effect, so track live instances and
// ignore callbacks whose context has already been destroyed
static GMutex sLiveInstancesLock;
static GSList* sLiveInstances = 0;

static void registerInstance(NyxLedControl* instance)
{
    g_mutex_lock(&sLiveInstancesLock);
    sLiveInstances = g_slist_prepend(sLiveInstances, instance);
    g_mutex_unlock(&sLiveInstancesLock);
}

static void unregisterInstance(NyxLedControl* instance)
{
    g_mutex_lock(&sLiveInstancesLock);
    sLiveInstances = g_slist_remove(sLiveInstances, instance);
    g_mutex_unlock(&sLiveInstancesLock);
}

static bool isLiveInstance(NyxLedControl* instance)
{
    g_mutex_lock(&sLiveInstancesLock);
    bool live = g_slist_find(sLiveInstances, instance) != 0;
    g_mutex_unlock(&sLiveInstancesLock);
    return live;
}



NyxLedControl::NyxLedControl(nyx_device_id_t id) : m_callback(0), m_ctx(0), m_handle(0)
{
    nyx_error_t error = nyx_device_open(NYX_DEVICE_LED_CONTROLLER, id, &m_handle);

    if ((error != NYX_ERROR_NONE) || (m_handle == NULL))
    {
            g_critical("Unable to obtain m_handle for NyxLedControl");
    }

    registerInstance(this);
}

NyxLedControl::~NyxLedControl()
{
    unregisterInstance(this);

    if (m_handle)
    {
        nyx_error_t error = nyx_device_close(m_handle);

        if (error != NYX_ERROR_NONE)
            g_critical("Unable to release m_handle");

        m_handle = NULL;
    }
}

void led_controller_callback(nyx_device_handle_t device,
        nyx_callback_status_t status, void* context)
{
    NyxLedControl* hlc = (NyxLedControl*) context;
    if (!isLiveInstance(hlc))
        return;
    if (hlc->m_callback)
        hlc->m_callback(hlc->m_ctx);
}

bool NyxLedControl::setBrightness(int keypadBrightness, int displayBrightness, LedControlCallback callback, void* ctx)
{
    if (m_handle)
    {
        m_callback = callback;
        m_ctx = ctx;

        nyx_error_t error = NYX_ERROR_NONE;
	
        nyx_led_controller_effect_t effect;
	    memset(&effect, 0, sizeof(effect));
	    effect.required.effect = NYX_LED_CONTROLLER_EFFECT_LED_SET;
	    effect.backlight.callback = led_controller_callback;
	    effect.backlight.callback_context = this;
	    effect.required.led = NYX_LED_CONTROLLER_BACKLIGHT_LEDS;
	    effect.backlight.brightness_lcd = displayBrightness;
	    effect.backlight.brightness_keypad = keypadBrightness;

	    error = nyx_led_controller_execute_effect(m_handle, effect);

        return (error == NYX_ERROR_NONE || error == NYX_ERROR_NOT_IMPLEMENTED);
    }

    return true;
}


