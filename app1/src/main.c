/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"

int
main (int argc, char *argv[])
{
  GtkWidget *w4espacecitoyen;
  GtkWidget *dialogSuppCpt;
  GtkWidget *authentification;
  GtkWidget *w1espaceadmin;
  GtkWidget *w5;
  GtkWidget *w3;
  GtkWidget *w6;
  GtkWidget *w4;
  GtkWidget *w2;
  GtkWidget *w1;
  GtkWidget *inscription;
  GtkWidget *w4listeCit;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  w4espacecitoyen = create_w4espacecitoyen ();
  gtk_widget_show (w4espacecitoyen);
  dialogSuppCpt = create_dialogSuppCpt ();
  gtk_widget_show (dialogSuppCpt);
  authentification = create_authentification ();
  gtk_widget_show (authentification);
  w1espaceadmin = create_w1espaceadmin ();
  gtk_widget_show (w1espaceadmin);
  w5 = create_w5 ();
  gtk_widget_show (w5);
  w3 = create_w3 ();
  gtk_widget_show (w3);
  w6 = create_w6 ();
  gtk_widget_show (w6);
  w4 = create_w4 ();
  gtk_widget_show (w4);
  w2 = create_w2 ();
  gtk_widget_show (w2);
  w1 = create_w1 ();
  gtk_widget_show (w1);
  inscription = create_inscription ();
  gtk_widget_show (inscription);
  w4listeCit = create_w4listeCit ();
  gtk_widget_show (w4listeCit);

  gtk_main ();
  return 0;
}
