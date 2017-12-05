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

static void cb_save (GtkWidget * p_wid, gpointer p_data)
{  
   (void)p_wid;
   GtkTextView *tmp = GTK_TEXT_VIEW(gtk_builder_get_object(GTK_BUILDER(p_data), "txt"));
   GtkTextIter start, end;
   GtkTextBuffer *buffer = gtk_text_view_get_buffer (tmp);
   gchar *text;
   gtk_text_buffer_get_bounds (buffer, &start, &end);
   text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
   // SAVE ME - text
   printf("%s\n",text);
   g_free(text);
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

  /* Variables */
   GtkBuilder  *  p_builder   = NULL;
   GError      *  p_err       = NULL;

  /* Initialisation de GTK+ */
  gtk_init(&argc, &argv);

  /* Création de la fenêtre */
  p_builder = gtk_builder_new ();
  gtk_builder_add_from_file (p_builder, "gtkbuild.glade", & p_err);
  GtkWidget * win = (GtkWidget *) gtk_builder_get_object (
    p_builder, "win"
  );


g_signal_connect (
   gtk_builder_get_object (p_builder, "load"),
   "clicked", G_CALLBACK (cb_load), p_builder
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "launch"),
   "clicked", G_CALLBACK (cb_main), p_builder
);
 
/* Signal du bouton Annuler */
g_signal_connect (
   gtk_builder_get_object (p_builder, "save"),
   "clicked", G_CALLBACK (cb_save), p_builder
);

g_signal_connect (
   gtk_builder_get_object (p_builder, "quit"),
   "activate", G_CALLBACK (cb_quit), NULL
);


  /* Paramètres */
  gtk_window_set_title(GTK_WINDOW(win), "Eye of the Graeae");
  gtk_window_set_default_size(GTK_WINDOW(win),100,100);

  gtk_widget_show_all(win);
  gtk_main();

  return EXIT_SUCCESS;
}
