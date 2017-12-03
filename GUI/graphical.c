#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

# include "graphical.h"

void ShowImage(gchar *filename, GtkWidget *widget) {
  GtkWidget *image;
  image = gtk_image_new_from_file (filename);
  gtk_container_add(GTK_CONTAINER(widget),image);

  gtk_widget_show_all(widget);
}

void ShowDialog(GtkWidget *widget, gpointer data) {

  GtkWidget *dialog;
  //GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Ouvrir une image",
            GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Annuler", GTK_RESPONSE_CANCEL,
            "_Ouvrir", GTK_RESPONSE_ACCEPT,
            NULL);
  (void) data;//histoire qu'il y ait pas de warning #Nat
  res = gtk_dialog_run (GTK_DIALOG (dialog));

  if (res == GTK_RESPONSE_ACCEPT) {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    printf("File : %s\n",filename);
    //open_file (filename);
    //g_free (filename);

    ShowImage(filename,widget);
  }
 gtk_widget_destroy (dialog);
}

void quit(GtkWidget *widget, gpointer data) {
  printf("Bye !\n");
  (void) data;
  (void)widget;
  gtk_main_quit();
}

/*int main(int argc, char **argv)
{
  // Variables
  GtkWidget *Main = NULL;

  // Initialisation de GTK+
  gtk_init(&argc, &argv);

  // Création de la fenêtre
  Main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(Main), "delete-event", G_CALLBACK(gtk_main_quit), NULL);

  // Paramètres
  gtk_window_set_title(GTK_WINDOW(Main), "Eye of the Graeae");
  gtk_window_set_default_size(GTK_WINDOW(Main),100,100);
  //gtk_resize(GTK_WINDOW(Main),100,00));


  GtkWidget *hbox = gtk_box_new (FALSE, 0);
  //GtkWidget *drawingArea = gtk_drawing_area_new ();
  GtkWidget *button = gtk_button_new_with_label ("Run !");

  gtk_container_add (GTK_CONTAINER (Main), hbox);

  //gtk_box_pack_start (GTK_BOX (hbox), drawingArea, TRUE, TRUE, 0);
  //gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(hbox),button);

  GtkWidget *label = NULL;
  label = gtk_label_new("Bonjour !");

  gtk_container_add(GTK_CONTAINER(hbox),label);


  g_signal_connect(G_OBJECT(hbox), "button-press-event", G_CALLBACK( ShowDialog ), NULL);
  g_signal_connect(G_OBJECT(hbox), "delete-event", G_CALLBACK( quit ), NULL);

  // Affichage et boucle évènementielle
  gtk_widget_show(Main);
  gtk_widget_show_all(Main);
  gtk_main();

  // On quitte..

//  gtk_widget_destroy(label);
//  gtk_widget_destroy(Main);

  return EXIT_SUCCESS;
}*/
