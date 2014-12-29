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

#include "panel-handle.h"
#include "panel-typebuiltins.h"

#define HANDLE_SIZE_MIN 1
#define HANDLE_SIZE_MAX 50
#define HANDLE_SIZE_DEFAULT 10

struct _PanelHandlePrivate
{
	PanelHandleType type;
	GtkOrientation  orientation;

	gint            size;
};

G_DEFINE_TYPE_WITH_PRIVATE (PanelHandle,
                            panel_handle,
                            GTK_TYPE_EVENT_BOX)

enum
{
	PROP_0,
	PROP_TYPE,
	PROP_ORIENTATION,
	N_PROPERTIES
};

static GParamSpec *object_properties[N_PROPERTIES] = { NULL, };

static void
panel_handle_update_size_request (PanelHandle *handle)
{
	GtkWidget *widget;

	widget = GTK_WIDGET (handle);

	if (handle->priv->orientation == GTK_ORIENTATION_HORIZONTAL) {
		gtk_widget_set_size_request (widget,
		                             handle->priv->size,
		                             -1);
	} else {
		gtk_widget_set_size_request (widget,
		                             -1,
		                             handle->priv->size);
	}
}

static void
panel_handle_set_type (PanelHandle     *handle,
                       PanelHandleType  type)
{
	GtkWidget       *widget;
	GtkStyleContext *context;
	const gchar     *class;

	widget = GTK_WIDGET (handle);
	context = gtk_widget_get_style_context (widget);

	switch (type)
	{
		case PANEL_HANDLE_PANEL_START:
			class = "panel-start";
			break;
		case PANEL_HANDLE_PANEL_END:
			class = "panel-end";
			break;
		case PANEL_HANDLE_APPLET:
			class = "applet";
			break;
		default:
			g_assert_not_reached ();
			break;
	}

	gtk_style_context_add_class (context,
	                             class);
}

static void
panel_handle_set_property (GObject      *object,
                           guint         property_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
	PanelHandle *handle;

	handle = PANEL_HANDLE (object);

	switch (property_id)
	{
		case PROP_TYPE:
			panel_handle_set_type (handle,
			                       g_value_get_enum (value));
			break;
		case PROP_ORIENTATION:
			panel_handle_set_orientation (handle,
			                              g_value_get_enum (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
			                                   property_id,
			                                   pspec);
			break;
	}
}

static void
panel_handle_get_property (GObject    *object,
                           guint       property_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
	PanelHandle *handle;

	handle = PANEL_HANDLE (object);

	switch (property_id)
	{
		case PROP_TYPE:
			g_value_set_enum (value, handle->priv->type);
			break;
		case PROP_ORIENTATION:
			g_value_set_enum (value, handle->priv->orientation);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
			                                   property_id,
			                                   pspec);
			break;
	}
}

static void
panel_handle_get_preferred_height (GtkWidget *widget,
                                   gint      *minimum_height,
                                   gint      *natural_height)
{
	PanelHandle *handle;

	GTK_WIDGET_CLASS (panel_handle_parent_class)->get_preferred_height (widget,
	                                                                    minimum_height,
	                                                                    natural_height);

	handle = PANEL_HANDLE (widget);

	if (handle->priv->orientation == GTK_ORIENTATION_HORIZONTAL) {
		*minimum_height = handle->priv->size;
		*natural_height = handle->priv->size;
	}
}

static void
panel_handle_get_preferred_width (GtkWidget *widget,
                                  gint      *minimum_width,
                                  gint      *natural_width)
{
	PanelHandle *handle;

	GTK_WIDGET_CLASS (panel_handle_parent_class)->get_preferred_width (widget,
	                                                                   minimum_width,
	                                                                   natural_width);
	handle = PANEL_HANDLE (widget);

	if (handle->priv->orientation == GTK_ORIENTATION_VERTICAL) {
		*minimum_width = handle->priv->size;
		*natural_width = handle->priv->size;
	}
}

static void
panel_handle_style_updated (GtkWidget *widget)
{
	PanelHandle *handle;

	GTK_WIDGET_CLASS (panel_handle_parent_class)->style_updated (widget);

	handle = PANEL_HANDLE (widget);

	gtk_widget_style_get (widget,
	                      "size", &handle->priv->size,
	                      NULL);

	panel_handle_update_size_request (handle);
}

static void
panel_handle_realize (GtkWidget *widget)
{
	GTK_WIDGET_CLASS (panel_handle_parent_class)->realize (widget);

	panel_handle_style_updated (widget);
}

static void
panel_handle_install_object_properties (GObjectClass *class)
{
	object_properties[PROP_TYPE] =
		g_param_spec_enum ("type",
		                   "type",
		                   "type",
		                   PANEL_TYPE_HANDLE_TYPE,
		                   PANEL_HANDLE_PANEL_START,
		                   G_PARAM_CONSTRUCT_ONLY |
		                   G_PARAM_READWRITE);

	object_properties[PROP_ORIENTATION] =
		g_param_spec_enum ("orientation",
		                   "orientation",
		                   "orientation",
		                   GTK_TYPE_ORIENTATION,
		                   GTK_ORIENTATION_HORIZONTAL,
		                   G_PARAM_READWRITE);

	g_object_class_install_properties (class,
	                                   N_PROPERTIES,
	                                   object_properties);
}

static void
panel_handle_install_style_properties (GtkWidgetClass *class)
{
	GParamSpec *size_spec;

	size_spec = g_param_spec_uint ("size",
	                               "size",
	                               "size",
	                               HANDLE_SIZE_MIN,
	                               HANDLE_SIZE_MAX,
	                               HANDLE_SIZE_DEFAULT,
	                               G_PARAM_READABLE |
	                               G_PARAM_STATIC_STRINGS);

	gtk_widget_class_install_style_property (class,
	                                         size_spec);
}

static void
panel_handle_class_init (PanelHandleClass *class)
{
	GObjectClass   *object_class;
	GtkWidgetClass *widget_class;

	object_class = G_OBJECT_CLASS (class);
	widget_class = GTK_WIDGET_CLASS (class);

	object_class->set_property = panel_handle_set_property;
	object_class->get_property = panel_handle_get_property;

	widget_class->get_preferred_height = panel_handle_get_preferred_height;
	widget_class->get_preferred_width = panel_handle_get_preferred_width;
	widget_class->realize = panel_handle_realize;
	widget_class->style_updated = panel_handle_style_updated;

	panel_handle_install_object_properties (object_class);
	panel_handle_install_style_properties (widget_class);
}

static void
panel_handle_init (PanelHandle *handle)
{
	handle->priv = panel_handle_get_instance_private (handle);

	gtk_widget_set_events (GTK_WIDGET (handle),
	                       GDK_BUTTON_PRESS_MASK |
	                       GDK_BUTTON_RELEASE_MASK);
}

GtkWidget *
panel_handle_new (PanelHandleType type,
                  GtkOrientation  orientation)
{
	return g_object_new (PANEL_TYPE_HANDLE,
	                     "type", type,
	                     "orientation", orientation,
	                     NULL);
}

void
panel_handle_set_orientation (PanelHandle    *handle,
                              GtkOrientation  orientation)
{
	GtkWidget       *widget;
	GtkStyleContext *context;

	if (handle->priv->orientation == orientation)
		return;

	handle->priv->orientation = orientation;

	widget = GTK_WIDGET (handle);
	context = gtk_widget_get_style_context (widget);

	if (orientation == GTK_ORIENTATION_HORIZONTAL) {
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_HORIZONTAL);
		gtk_style_context_remove_class (context, GTK_STYLE_CLASS_VERTICAL);
	} else {
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_VERTICAL);
		gtk_style_context_remove_class (context, GTK_STYLE_CLASS_HORIZONTAL);
	}
}

guint
panel_handle_get_size (PanelHandle *handle)
{
	return handle->priv->size;
}
