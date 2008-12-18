/*
 * key.h - Keybinding helpers
 *
 * Copyright © 2008 Pierre Habouzit <madcoder@debian.org>
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

#ifndef AWESOME_KEYBINDING_H
#define AWESOME_KEYBINDING_H

#include <xcb/xcb.h>
#include "luaa.h"
#include "common/array.h"

typedef struct keyb_t
{
    /** Ref count */
    int refcount;
    /** Key modifier */
    unsigned long mod;
    /** Keysym */
    xcb_keysym_t keysym;
    /** Keycode */
    xcb_keycode_t keycode;
    /** Lua function to execute on press */
    luaA_ref press;
    /** Lua function to execute on release */
    luaA_ref release;
} keyb_t;

ARRAY_TYPE(keyb_t *, key)

keyb_t *key_find(const xcb_key_press_event_t *);
xcb_keysym_t key_getkeysym(xcb_keycode_t, uint16_t);
void window_root_grabkey(keyb_t *);

int luaA_key_new(lua_State *);

#endif
