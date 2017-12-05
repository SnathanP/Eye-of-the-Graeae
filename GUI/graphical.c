#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "graphical.h"
#include "../ocr.h"

char *justforward(double **input, int lenlist);
int apprentissage(int nbmid, int ite, int load);
char* execOcr(char* filename);

void cb_quit (GtkWidget * p_wid, gpointer p_data)
{
  (void)p_wid;
  (void)p_data;
  gtk_main_quit ();
}

void cb_save (GtkWidget * p_wid, gpointer p_data)
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

void cb_main (GtkWidget * p_wid, gpointer p_data)
{
  
  (void)p_wid;
  GtkTextView *text = GTK_TEXT_VIEW(gtk_builder_get_object(GTK_BUILDER(p_data), "txt"));
  
  char* final = execOcr("tmp");

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

void cb_load (GtkWidget * p_wid, gpointer p_data)
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