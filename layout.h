/*
 * layout.h - layout management header
 *
 * Copyright © 2007 Julien Danjou <julien@danjou.info>
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

#ifndef AWESOME_LAYOUT_H
#define AWESOME_LAYOUT_H

#include "config.h"
#include "uicb.h"

#define AWESOMEPROPS_ATOM(disp)                    XInternAtom(disp, "_AWESOME_PROPERTIES", False)

typedef void (LayoutArrange)(int);

void arrange(int);
void restack(int);
void loadawesomeprops(int);
void saveawesomeprops(int);
void client_maximize(Client *c, int, int, int, int);

Uicb uicb_client_focusnext;
Uicb uicb_client_focusprev;
Uicb uicb_tag_setlayout;
Uicb uicb_client_togglemax;
Uicb uicb_client_toggleverticalmax;
Uicb uicb_client_togglehorizontalmax;
Uicb uicb_client_zoom;

#endif
// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
