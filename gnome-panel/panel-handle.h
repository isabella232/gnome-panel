/*
 * Copyright (C) 2014 Alberts Muktupāvels
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *     Alberts Muktupāvels <alberts.muktupavels@gmail.com>
 */

#ifndef PANEL_HANDLE_H
#define PANEL_HANDLE_H

#include <gtk/gtk.h>

#define PANEL_TYPE_HANDLE         (panel_handle_get_type ())
#define PANEL_HANDLE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), \
                                   PANEL_TYPE_HANDLE,               \
                                   PanelHandle))
#define PANEL_HANDLE_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c),    \
                                   PANEL_TYPE_HANDLE,               \
                                   PanelHandleClass))
#define PANEL_IS_HANDLE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
                                   PANEL_TYPE_HANDLE))
#define PANEL_IS_HANDLE_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c),    \
                                   PANEL_TYPE_HANDLE))
#define PANEL_HANDLE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o),   \
                                   PANEL_TYPE_HANDLE,               \
                                   PanelHandleClass))

typedef struct _PanelHandle        PanelHandle;
typedef struct _PanelHandleClass   PanelHandleClass;
typedef struct _PanelHandlePrivate PanelHandlePrivate;

struct _PanelHandle
{
	GtkEventBox         parent;
	PanelHandlePrivate *priv;
};

struct _PanelHandleClass
{
	GtkEventBoxClass parent_class;
};

typedef enum
{
	PANEL_HANDLE_PANEL_START,
	PANEL_HANDLE_PANEL_END,
	PANEL_HANDLE_APPLET
} PanelHandleType;

GType      panel_handle_get_type        (void);

GtkWidget *panel_handle_new             (PanelHandleType  type,
                                         GtkOrientation   orientation);

void       panel_handle_set_orientation (PanelHandle     *handle,
                                         GtkOrientation   orientation);

guint      panel_handle_get_size        (PanelHandle     *handle);

#endif
