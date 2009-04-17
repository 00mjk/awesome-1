/*
 * structs.h - basic structs header
 *
 * Copyright © 2007-2008 Julien Danjou <julien@danjou.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef AWESOME_STRUCTS_H
#define AWESOME_STRUCTS_H

#define SN_API_NOT_YET_FROZEN
#include <libsn/sn.h>

#include <xcb/xcb_icccm.h>
#include <xcb/xcb_property.h>

#include "config.h"
#include "luaa.h"
#include "swindow.h"
#include "key.h"
#include "common/xutil.h"
#include "common/xembed.h"
#include "common/tokenize.h"

typedef struct wibox_t wibox_t;
typedef struct a_screen screen_t;
typedef struct button_t button_t;
typedef struct widget_t widget_t;
typedef struct widget_node_t widget_node_t;
typedef struct client_t client_t;
typedef struct client_node client_node_t;
typedef struct tag tag_t;
typedef struct tag_client_node_t tag_client_node_t;
typedef struct awesome_t awesome_t;

ARRAY_TYPE(widget_node_t, widget_node)
ARRAY_TYPE(button_t *, button)
ARRAY_TYPE(tag_t *, tag)
ARRAY_TYPE(screen_t, screen)
ARRAY_TYPE(client_t *, client)

/** Main configuration structure */
struct awesome_t
{
    /** Connection ref */
    xcb_connection_t *connection;
    /** Event and error handlers */
    xcb_event_handlers_t evenths;
    /** Property change handler */
    xcb_property_handlers_t prophs;
    /** Default screen number */
    int default_screen;
    /** Keys symbol table */
    xcb_key_symbols_t *keysyms;
    /** Logical screens */
    screen_array_t screens;
    /** True if xinerama is active */
    bool xinerama_is_active;
    /** Root window key bindings */
    keybindings_t keys;
    /** Root window mouse bindings */
    button_array_t buttons;
    /** Check for XRandR extension */
    bool have_randr;
    /** Check for XTest extension */
    bool have_xtest;
    /** Clients list */
    client_array_t clients;
    /** Embedded windows */
    xembed_window_array_t embedded;
    /** Path to config file */
    char *conffile;
    /** Stack client history */
    client_node_t *stack;
    /** Command line passed to awesome */
    char *argv;
    /** Lua VM state */
    lua_State *L;
    /** Default colors */
    struct
    {
        xcolor_t fg, bg;
    } colors;
    /** Default font */
    font_t *font;
    struct
    {
        /** Command to execute when spawning a new client */
        luaA_ref manage;
        /** Command to execute when unmanaging client */
        luaA_ref unmanage;
        /** Command to execute when giving focus to a client */
        luaA_ref focus;
        /** Command to execute when removing focus to a client */
        luaA_ref unfocus;
        /** Command to run when mouse enter a client */
        luaA_ref mouse_enter;
        /** Command to run when mouse leave a client */
        luaA_ref mouse_leave;
        /** Command to run on arrange */
        luaA_ref arrange;
        /** Command to run when client list changes */
        luaA_ref clients;
        /** Command to run on numbers of tag changes */
        luaA_ref tags;
        /** Command to run when client gets (un)tagged */
        luaA_ref tagged;
        /** Command to run on property change */
        luaA_ref property;
        /** Command to run on time */
        luaA_ref timer;
        /** Startup notification hooks */
        luaA_ref startup_notification;
#ifdef WITH_DBUS
        /** Command to run on dbus events */
        luaA_ref dbus;
#endif
    } hooks;
    /** The event loop */
    struct ev_loop *loop;
    /** The timeout after which we need to stop select() */
    struct ev_timer timer;
    /** The key grabber function */
    luaA_ref keygrabber;
    /** The mouse pointer grabber function */
    luaA_ref mousegrabber;
    /** Focused screen */
    screen_t *screen_focus;
    /** Need to call client_stack_refresh() */
    bool client_need_stack_refresh;
    /** The startup notification display struct */
    SnDisplay *sndisplay;
};

extern awesome_t globalconf;

#endif
// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
