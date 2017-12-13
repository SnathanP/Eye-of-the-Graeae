# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <gtk/gtk.h>

# include "property.h"
# include "LayerStruct.h"
# include "savesystem.h"
# include "matrix.h"
# include "Picture_Treatment/picture_treatment.h"

# include "ocr.h"


static void cb_quit (GtkWidget * p_wid, gpointer p_data)
{
  (void)p_wid;
  (void)p_data;
   gtk_main_quit ();
}

static void cb_about_quit (GtkWidget * p_wid, gpointer p_data)
{  
  gtk_widget_destroy (p_wid);
  (void)p_data;
}

static void cb_about (GtkWidget * p_wid, gpointer p_data)
{
  (void)p_wid;
  (void)p_data;
  GtkWidget *about_dialog;

  about_dialog = gtk_about_dialog_new ();

  const gchar *authors[] = {"Thomas LUPIN, Louis HOLLEVILLE, Arthur BUSUTIL", NULL};
  gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about_dialog), "A propos de Eye of the Graeae");
  gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (about_dialog), "Copyright \xc2\xa9 2017 Bible White Corp.");
  gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about_dialog), authors);
  gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (about_dialog), "Eye of the Graeae website");
  gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (about_dialog), "https://eyeofthegraeae.tk");
  GdkPixbuf *img = gdk_pixbuf_new_from_file("icon.png", NULL);
  gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG (about_dialog), img);
  gtk_window_set_title (GTK_WINDOW (about_dialog), "A propos");

  g_signal_connect (GTK_DIALOG (about_dialog), "response",
                    G_CALLBACK (cb_about_quit), NULL);

  /* Show the about dialog */

  gtk_widget_show (about_dialog);
}

static void cb_save (GtkWidget * p_wid, gpointer p_data)
{  
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Enregistrer le texte",
                                        GTK_WINDOW(gtk_widget_get_toplevel(p_wid)),
                                        action,
                                        "_Annuler",
                                        GTK_RESPONSE_CANCEL,
                                        "_Engegistrer",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);
  chooser = GTK_FILE_CHOOSER (dialog);

  gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
  gtk_file_chooser_set_current_name (chooser, "Save_EotG.txt");

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      char *filename;

      filename = gtk_file_chooser_get_filename (chooser);

      GtkTextView *tmp = GTK_TEXT_VIEW(gtk_builder_get_object(GTK_BUILDER(p_data), "txt"));
      GtkTextIter start, end;
      GtkTextBuffer *buffer = gtk_text_view_get_buffer (tmp);
      gchar *text;
      gtk_text_buffer_get_bounds (buffer, &start, &end);
      text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
      // SAVE ME - text
      FILE* fichier = NULL;

      fichier = fopen(filename, "w");
      if (fichier != NULL)
      {
          fputs(text, fichier);
          fclose(fichier);
      }
      g_free(text);

      g_free (filename);
    }

  gtk_widget_destroy (dialog);


}

static void cb_main (GtkWidget * p_wid, gpointer p_data)
{
  (void)p_wid;
  GtkTextView *text = GTK_TEXT_VIEW(gtk_builder_get_object(GTK_BUILDER(p_data), "txt"));
  // CALL OCR
  char *final = execOcr("tmp");
  // Delete text and insert result.
  GtkTextBuffer *buffer;
  buffer = gtk_text_view_get_buffer (text);

  GtkTextIter start, end;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  gtk_text_buffer_delete(buffer, &start, &end);

  GtkTextMark *mark;
  GtkTextIter iter;
  mark = gtk_text_buffer_get_insert (buffer);
  gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
  gtk_text_buffer_insert (buffer, &iter, final, -1);
  
  free(final);
}

static void cb_load (GtkWidget * p_wid, gpointer p_data)
{
  GtkImage *img = GTK_IMAGE(gtk_builder_get_object(GTK_BUILDER(p_data), "img"));

  GtkWidget *dialog;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Ouvrir une image",
            GTK_WINDOW(gtk_widget_get_toplevel(p_wid)), GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Annuler", GTK_RESPONSE_CANCEL,
            "_Ouvrir", GTK_RESPONSE_ACCEPT,
            NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));

  if (res == GTK_RESPONSE_ACCEPT) {
    gchar *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    printf("File : %s\n",filename);

    SDL_Surface *sdl = IMG_Load(filename);
    SDL_SaveBMP(sdl, "tmp");
    SDL_FreeSurface(sdl);

    gtk_image_set_from_file(img, "tmp");
  }
  gtk_widget_destroy (dialog);

}

int main(int argc, char **argv)
{
  if (argc > 1) {

    if (argc > 2 && argv[1][0] == '1') {
      apprentissage(250, 10000, (argv[2][0] - '0'));
      return 0;
    }
    else {
      printf("You must specify if you want to load a existing save of the neural network. \n./main 1 1 : Load network\n./main 1 0 : Use existing network\n");
      return 1;
    }
  }
  /* Variables */
   GtkBuilder  *  p_builder   = NULL;
   //GError      *  p_err       = NULL;

  /* Initialisation de GTK+ */
  gtk_init(&argc, &argv);

  /* Création de la fenêtre */
  char test[] = "<?xml version='1.0'encoding='UTF-8'?><!--Generated with glade 3.20.2--><interface><requires lib='gtk+'version='3.20'/><object class='GtkWindow'id='win'><property name='can_focus'>False</property><child><object class='GtkBox'><property name='visible'>True</property><property name='can_focus'>False</property><property name='orientation'>vertical</property><child><object class='GtkMenuBar'><property name='visible'>True</property><property name='can_focus'>False</property><child><object class='GtkMenuItem'><property name='visible'>True</property><property name='can_focus'>False</property><property name='label'translatable='yes'>_Fichier</property><property name='use_underline'>True</property><child type='submenu'><object class='GtkMenu'><property name='visible'>True</property><property name='can_focus'>False</property><child><object class='GtkImageMenuItem'id='open'><property name='label'>gtk-open</property><property name='visible'>True</property><property name='can_focus'>False</property><property name='use_underline'>True</property><property name='use_stock'>True</property></object></child><child><object class='GtkImageMenuItem'id='exec'><property name='label'>gtk-execute</property><property name='visible'>True</property><property name='can_focus'>False</property><property name='use_underline'>True</property><property name='use_stock'>True</property></object></child><child><object class='GtkImageMenuItem'id='sav'><property name='label'>gtk-save</property><property name='visible'>True</property><property name='can_focus'>False</property><property name='use_underline'>True</property><property name='use_stock'>True</property></object></child><child><object class='GtkSeparatorMenuItem'><property name='visible'>True</property><property name='can_focus'>False</property></object></child><child><object class='GtkImageMenuItem'id='quit'><property name='label'>gtk-quit</property><property name='visible'>True</property><property name='can_focus'>False</property><property name='use_underline'>True</property><property name='use_stock'>True</property></object></child></object></child></object></child><child><object class='GtkMenuItem'><property name='visible'>True</property><property name='can_focus'>False</property><property name='label'translatable='yes'>Aid_e</property><property name='use_underline'>True</property><child type='submenu'><object class='GtkMenu'><property name='visible'>True</property><property name='can_focus'>False</property><child><object class='GtkImageMenuItem'id='propos'><property name='label'>gtk-about</property><property name='visible'>True</property><property name='can_focus'>False</property><property name='use_underline'>True</property><property name='use_stock'>True</property></object></child></object></child></object></child></object><packing><property name='expand'>False</property><property name='fill'>True</property><property name='position'>0</property></packing></child><child><object class='GtkImage'id='img'><property name='visible'>True</property><property name='can_focus'>False</property><property name='pixbuf'>icon.png</property></object><packing><property name='expand'>False</property><property name='fill'>True</property><property name='position'>1</property></packing></child><child><object class='GtkTextView'id='txt'><property name='visible'>True</property><property name='can_focus'>True</property></object><packing><property name='expand'>True</property><property name='fill'>True</property><property name='position'>2</property></packing></child><child><object class='GtkBox'><property name='visible'>True</property><property name='can_focus'>False</property><child><object class='GtkButton'id='load'><property name='label'translatable='yes'>Charger une image</property><property name='visible'>True</property><property name='can_focus'>True</property><property name='receives_default'>True</property></object><packing><property name='expand'>True</property><property name='fill'>True</property><property name='position'>0</property></packing></child><child><object class='GtkButton'id='launch'><property name='label'translatable='yes'>Détecter le texte</property><property name='visible'>True</property><property name='can_focus'>True</property><property name='receives_default'>True</property></object><packing><property name='expand'>True</property><property name='fill'>True</property><property name='position'>1</property></packing></child><child><object class='GtkButton'id='save'><property name='label'translatable='yes'>Enregistrer le texte</property><property name='visible'>True</property><property name='can_focus'>True</property><property name='receives_default'>True</property></object><packing><property name='expand'>True</property><property name='fill'>True</property><property name='position'>2</property></packing></child></object><packing><property name='expand'>False</property><property name='fill'>True</property><property name='position'>3</property></packing></child></object></child><child type='titlebar'><placeholder/></child></object></interface>";
  p_builder = gtk_builder_new_from_string (test, -1);
  //gtk_builder_add_from_file (p_builder, "gtkbuild.glade", & p_err);
  GtkWidget * win = (GtkWidget *) gtk_builder_get_object (
    p_builder, "win"
  );


g_signal_connect (
   gtk_builder_get_object (p_builder, "load"),
   "clicked", G_CALLBACK (cb_load), p_builder
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "open"),
   "activate", G_CALLBACK (cb_load), p_builder
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "launch"),
   "clicked", G_CALLBACK (cb_main), p_builder
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "exec"),
   "activate", G_CALLBACK (cb_main), p_builder
);
 
g_signal_connect (
   gtk_builder_get_object (p_builder, "save"),
   "clicked", G_CALLBACK (cb_save), p_builder
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "sav"),
   "activate", G_CALLBACK (cb_save), p_builder
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "propos"),
   "activate", G_CALLBACK (cb_about), NULL
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "quit"),
   "activate", G_CALLBACK (cb_quit), NULL
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "win"),
   "delete-event", G_CALLBACK (cb_quit), NULL
);


  /* Paramètres */
  gtk_window_set_title(GTK_WINDOW(win), "Eye of the Graeae");
  gtk_window_set_default_size(GTK_WINDOW(win),100,100);

  gtk_widget_show_all(win);
  gtk_main();

  return EXIT_SUCCESS;
}
