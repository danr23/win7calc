#include <gtk/gtk.h>
#include "ui.h"

static void activate(GtkApplication *app, gpointer user_data){
    GtkWidget *win = create_main_window(app);
    gtk_window_set_icon_name(GTK_WINDOW(win), "accesories-calculator");
    //gtk_window_set_icon_from_file(GTK_WINDOW(win), "/usr/share/icons/Windows 7 Aero/apps/16/accessories-calculator.png");
    gtk_window_present(GTK_WINDOW(win));
    //GIcon *icon = g_themed_icon_new("accessories-calculator");
    //gtk_entry_set_icon_from_gicon(GTK_WINDOW(win), icon, NULL);
    //g_clear_object(&icon);
}

int main(int argc, char **argv){
    GtkApplication *app = gtk_application_new("org.example.win7calc", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
