/**
 * Copyright (c) 2006 LxDE Developers, see the file AUTHORS for details.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef PRIVATE_H
#define PRIVATE_H

#include "plugin.h"
#include "conf.h"

#include <gmodule.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include "panel.h"

#include <gdk/gdk.h>

#include "bg.h"
#include "ev.h"

/* -----------------------------------------------------------------------------
 *   Definitions used by lxpanel main code internally */

/* Extracted from panel.h */
enum { ALLIGN_NONE, ALLIGN_LEFT, ALLIGN_CENTER, ALLIGN_RIGHT  };
enum { EDGE_NONE=0, EDGE_LEFT, EDGE_RIGHT, EDGE_TOP, EDGE_BOTTOM };
enum { WIDTH_NONE, WIDTH_REQUEST, WIDTH_PIXEL, WIDTH_PERCENT };
enum { HEIGHT_NONE, HEIGHT_PIXEL, HEIGHT_REQUEST };
enum {
    ORIENT_NONE = -1,
    ORIENT_VERT = GTK_ORIENTATION_VERTICAL,
    ORIENT_HORIZ = GTK_ORIENTATION_HORIZONTAL
};

#define PANEL_ICON_SIZE               24	/* Default size of panel icons */
#define PANEL_HEIGHT_DEFAULT          26	/* Default height of horizontal panel */
#define PANEL_WIDTH_DEFAULT           150	/* Default "height" of vertical panel */
#define PANEL_HEIGHT_MAX              200	/* Maximum height of panel */
#define PANEL_HEIGHT_MIN              16	/* Minimum height of panel */
#define PANEL_ICON_HIGHLIGHT          0x202020	/* Constant to pass to icon loader */

/* to check if we are in LXDE */
extern gboolean is_in_lxde;

/* Context of a panel on a given edge. */
struct _Panel {
    char* name;
    GtkWidget * topgwin;		/* Main panel window */
    Window topxwin;			/* Main panel's X window   */
    GdkDisplay * display;		/* Main panel's GdkDisplay */
    GtkStyle * defstyle;
    GtkIconTheme* icon_theme; /*Default icon theme*/

    GtkWidget * box;			/* Top level widget */

    GtkRequisition requisition;
    GtkWidget *(*my_box_new) (gboolean, gint);
    GtkWidget *(*my_separator_new) ();

    FbBg *bg;
    int alpha;
    guint32 tintcolor;
    guint32 fontcolor;
    GdkColor gtintcolor;
    GdkColor gfontcolor;

    int ax, ay, aw, ah;  /* prefferd allocation of a panel */
    int cx, cy, cw, ch;  /* current allocation (as reported by configure event) allocation */
    int allign, edge, margin;
    int orientation;
    int widthtype, width;
    int heighttype, height;
    gint monitor;
    gulong strut_size;			/* Values for WM_STRUT_PARTIAL */
    gulong strut_lower;
    gulong strut_upper;
    int strut_edge;

    guint config_changed : 1;
    guint self_destroy : 1;
    guint setdocktype : 1;
    guint setstrut : 1;
    guint round_corners : 1;
    guint usefontcolor : 1;
    guint usefontsize : 1;
    guint fontsize;    
    guint transparent : 1;
    guint background : 1;
    guint spacing;

    guint autohide : 1;
    guint visible : 1;
    int height_when_hidden;
    guint hide_timeout;
    int icon_size;			/* Icon size */

    int desknum;
    int curdesk;
    guint32 *workarea;
    int wa_len;

    char* background_file;

    GSList * system_menus;		/* List of plugins having menus */
    PanelConf * config;                 /* Panel configuration data */

    GtkWidget* plugin_pref_dialog;	/* Plugin preference dialog */
    GtkWidget* pref_dialog;		/* preference dialog */
    GtkWidget* margin_control;		/* Margin control in preference dialog */
    GtkWidget* height_label;		/* Label of height control */
    GtkWidget* width_label;		/* Label of width control */
    GtkWidget* alignment_left_label;	/* Label of alignment: left control */
    GtkWidget* alignment_right_label;	/* Label of alignment: right control */
    GtkWidget* height_control;		/* Height control in preference dialog */
    GtkWidget* width_control;		/* Width control in preference dialog */
};

typedef struct {
    char *name;
    char *disp_name;
    void (*cmd)(void);
} Command;

extern Command commands[];

extern FbEv *fbev;

#define FBPANEL_WIN(win)  gdk_window_lookup(win)

/* Extracted from misc.h */
typedef struct {
    int num;
    gchar *str;
} pair;

extern pair allign_pair[];
extern pair edge_pair[];
extern pair width_pair[];
extern pair height_pair[];
extern pair bool_pair[];

int str2num(pair *p, const gchar *str, int defval);
const gchar *num2str(pair *p, int num, const gchar *defval);

char* get_config_file( const char* profile, const char* file_name, gboolean is_global );


/* -----------------------------------------------------------------------------
 *   Deprecated declarations. Kept for compatibility with old code plugins.
 *   Should be removed and appropriate code cleaned on some of next releases. */

/* Extracted from panel.h */
extern const char* lxpanel_get_file_manager();


/* Extracted from misc.h */
typedef struct _Plugin Plugin;

enum { LINE_NONE, LINE_BLOCK_START, LINE_BLOCK_END, LINE_VAR };

typedef struct {
    int num, len, type;
    gchar str[256];
    gchar *t[3];
} line;


extern int lxpanel_get_line(char **fp, line *s);
extern int lxpanel_put_line(FILE* fp, const char* format, ...);
#define lxpanel_put_str(fp, name, val) (G_UNLIKELY( !(val) || !*(val) )) ? 0 : lxpanel_put_line(fp, "%s=%s", name, val)
#define lxpanel_put_bool(fp, name, val) lxpanel_put_line(fp, "%s=%c", name, (val) ? '1' : '0')
#define lxpanel_put_int(fp, name, val) lxpanel_put_line(fp, "%s=%d", name, val)

GtkWidget *_gtk_image_new_from_file_scaled(const gchar *file, gint width,
                                           gint height, gboolean keep_ratio);

GtkWidget * fb_button_new_from_file(
    const gchar * image_file, int width, int height, gulong highlight_color, gboolean keep_ratio);
GtkWidget * fb_button_new_from_file_with_label(
    const gchar * image_file, int width, int height, gulong highlight_color, gboolean keep_ratio, Panel * panel, gchar * label);

void fb_button_set_from_file(GtkWidget* btn, const char* img_file, gint width, gint height, gboolean keep_ratio);

char* translate_exec_to_cmd( const char* exec, const char* icon,
                             const char* title, const char* fpath );

void show_error( GtkWindow* parent_win, const char* msg );

gboolean spawn_command_async(GtkWindow *parent_window, gchar const* workdir,
        gchar const* cmd);

/* Parameters: const char* name, gpointer ret_value, GType type, ....NULL */
GtkWidget* create_generic_config_dlg( const char* title, GtkWidget* parent,
                              GSourceFunc apply_func, Plugin * plugin,
                      const char* name, ... );

extern GtkMenu* lxpanel_get_panel_menu( Panel* panel, Plugin* plugin, gboolean use_sub_menu );

gboolean lxpanel_launch_app(const char* exec, GList* files, gboolean in_terminal, char const* in_workdir);

extern GdkPixbuf* lxpanel_load_icon( const char* name, int width, int height, gboolean use_fallback );


/* Extracted from plugin.h */
struct _Plugin;

/* Support for external plugin versioning.
 * Plugins must invoke PLUGINCLASS_VERSIONING when they instantiate PluginClass. */
#define PLUGINCLASS_VERSION 1
#define PLUGINCLASS_VERSIONING \
    .structure_size = sizeof(PluginClass), \
    .structure_version = PLUGINCLASS_VERSION

/* Representative of an available plugin. */
typedef struct {

    /* Keep these first.  Do not make unnecessary changes in structure layout. */
    unsigned short structure_size;		/* Structure size, for versioning support */
    unsigned short structure_version;		/* Structure version, for versioning support */

    char * fname;				/* Plugin file pathname */
    int count;					/* Reference count */
    GModule * gmodule;				/* Associated GModule structure */

    int dynamic : 1;				/* True if dynamically loaded */
    int unused_invisible : 1;			/* Unused; reserved bit */
    int not_unloadable : 1;			/* Not unloadable due to GModule restriction */
    int one_per_system : 1;			/* Special: only one possible per system, such as system tray */
    int one_per_system_instantiated : 1;	/* True if one instance exists */
    int expand_available : 1;			/* True if "stretch" option is available */
    int expand_default : 1;			/* True if "stretch" option is default */

    /* These fields point within the plugin image. */
    char * type;				/* Internal name of plugin, to match external filename */
    char * name;				/* Display name of plugin for selection UI */
    char * version;				/* Version of plugin */
    char * description;				/* Brief textual description of plugin for selection UI */

    int (*constructor)(struct _Plugin * plugin, char ** fp);		/* Create an instance of the plugin */
    void (*destructor)(struct _Plugin * plugin);			/* Destroy an instance of the plugin */
    void (*config)(struct _Plugin * plugin, GtkWindow * parent);	/* Request the plugin to display its configuration dialog */
    void (*save)(struct _Plugin * plugin, FILE * fp);			/* Request the plugin to save its configuration to a file */
    void (*panel_configuration_changed)(struct _Plugin * plugin);	/* Request the plugin to do a full redraw after a panel configuration change */
} PluginClass;

/* Representative of a loaded and active plugin attached to a panel. */
struct _Plugin {
    PluginClass * class;			/* Back pointer to PluginClass */
    Panel * panel;				/* Back pointer to Panel */
    GtkWidget * pwid;				/* Top level widget; plugin allocates, but plugin mechanism, not plugin itself, destroys this */
    int expand;					/* Expand ("stretch") setting for container */
    int padding;				/* Padding setting for container */
    int border;					/* Border setting for container */
    gpointer priv;				/* Private context for plugin; plugin frees this in its destructor */
};

/* Plugins management - deprecated style, for backward compatibility */
extern gboolean plugin_button_press_event(GtkWidget *widget, GdkEventButton *event, Plugin *plugin);
                                                        /* Handler for "button_press_event" signal with Plugin as parameter */
extern void plugin_adjust_popup_position(GtkWidget * popup, Plugin * plugin);
							/* Helper to move popup windows away from the panel */
extern void plugin_popup_set_position_helper(Plugin * p, GtkWidget * near, GtkWidget * popup, GtkRequisition * popup_req, gint * px, gint * py);
							/* Helper for position-calculation callback for popup menus */


/* -------------------------------------------------------------------------- */
/* FIXME: optional definitions */
#define STATIC_SEPARATOR
#define STATIC_LAUNCHBAR
#define STATIC_DCLOCK
#define STATIC_WINCMD
#define STATIC_DIRMENU
#define STATIC_TASKBAR
#define STATIC_PAGER
#define STATIC_TRAY
#define STATIC_MENU
#define STATIC_SPACE
#define STATIC_ICONS

/* Plugins management - new style */
void _prepare_modules(void);
void _unload_modules(void);

GtkWidget *lxpanel_add_plugin(Panel *p, const char *name, config_setting_t *cfg, gint at);
GHashTable *lxpanel_get_all_types(void); /* transfer none */

GQuark lxpanel_plugin_qinit; /* access to LXPanelPluginInit data */
#define PLUGIN_CLASS(_i) ((LXPanelPluginInit*)g_object_get_qdata(G_OBJECT(_i),lxpanel_plugin_qinit))

GQuark lxpanel_plugin_qconf; /* access to congig_setting_t data */

GQuark lxpanel_plugin_qpanel; /* access to Panel */
#define PLUGIN_PANEL(_i) ((Panel*)g_object_get_qdata(G_OBJECT(_i),lxpanel_plugin_qpanel))

#endif
