#include "DrugStore.h"
#include <gtk/gtk.h>

GtkWidget* window;
GtkWidget* appTree;
GtkTreeViewColumn* appTreeCol;
GtkCellRenderer* appTreeRenderer;
GtkCellRenderer* appIconRenderer;
GtkTreeStore *drugStore;

void on_notification();

void drugstore_panel_init();

int main(int argc, char* argv[]){
	gtk_init(&argc,&argv);

	drugstore_panel_init();

	gtk_main();
	return 0;
}


void drugstore_panel_init() {
	// create the window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(
		GTK_WINDOW(window),
		WIDTH,
		gdk_screen_get_height(GDK_SCREEN(gtk_window_get_screen(GTK_WINDOW(window))))
	);
	gtk_container_set_border_width(GTK_CONTAINER(window), MARGIN);
	gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

	// create tree view
	appTree = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(appTree), FALSE);
	gtk_tree_view_set_level_indentation(
		GTK_TREE_VIEW(appTree),
		INDENT - gtk_tree_view_get_level_indentation(GTK_TREE_VIEW(appTree))
	);
	g_object_set(G_OBJECT(appTree), "tooltip-column", COLUMN_DETAILS, NULL);

	appTreeCol = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(appTree), appTreeCol);
	gtk_tree_view_column_set_sizing(appTreeCol, GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_fixed_width(appTreeCol, WIDTH-2*MARGIN);

	appIconRenderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(appTreeCol, appIconRenderer, FALSE);
	gtk_tree_view_column_add_attribute(appTreeCol, appIconRenderer, "pixbuf", COLUMN_ICON);
	appTreeRenderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(appTreeCol, appTreeRenderer, TRUE);
	gtk_tree_view_column_add_attribute(appTreeCol, appTreeRenderer, "text", COLUMN_APPNAME);

	// create tree model
	drugStore = gtk_tree_store_new(COLUMN_COUNT, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING);

	// final initialization
	gtk_container_add(GTK_CONTAINER(window), appTree);
	//#test
	g_signal_connect(G_OBJECT(appTree), "button_press_event", G_CALLBACK(on_notification), NULL);
	gtk_tree_view_set_model(GTK_TREE_VIEW(appTree), GTK_TREE_MODEL(drugStore));

	gtk_widget_show_all(window);
}

void on_notification() {
	GValue curGvalue = G_VALUE_INIT;
	GtkTreeIter app, notification;
	GdkPixbuf* icon;
	gboolean isNewApp = TRUE;

	g_value_init(&curGvalue, G_TYPE_STRING);

	// look if a tree section for this app already exists
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(drugStore), &app)) {
		do {
			g_value_unset(&curGvalue);
			gtk_tree_model_get_value(GTK_TREE_MODEL(drugStore), &app, COLUMN_APPNAME, &curGvalue);
			if (!g_strcmp0(g_value_get_string(&curGvalue), "tist")) {
				isNewApp = FALSE;
				break;
			}
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(drugStore), &app));
	}

	// create a new section
	if (isNewApp) {
		gtk_tree_store_append(drugStore, &app, NULL);
		icon = gtk_icon_theme_load_icon(
			gtk_icon_theme_get_default(),
			"starred",
			16,
			0,
			NULL
		);
		gtk_tree_store_set(
			drugStore, &app,
			COLUMN_APPNAME, "tist2",
			COLUMN_ICON, GDK_PIXBUF(icon),
			-1
		);
		g_object_unref(icon);
	}

	// add new notification to the section
	gtk_tree_store_append(drugStore, &notification, &app);
	gtk_tree_store_set(
		drugStore, &notification,
		COLUMN_APPNAME, "kasjdfl jskld;afskldfj lskdjfkl;asdkf; sldkfjaskldjfk",
		COLUMN_DETAILS, "Lorem ipsum sit amet dolor",
		COLUMN_ICON, icon,
		-1
	);
}
