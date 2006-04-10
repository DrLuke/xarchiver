/*
 *  Copyright (c) 2006 Stephan Arts <stephan.arts@hva.nl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <glib.h>
#include <glib-object.h>
#include <libxarchiver/libxarchiver.h>
#include <gtk/gtk.h>
#include <getopt.h>

#include "main.h"
#include "property-dialog.h"
#include "main-window.h"


GtkWidget *main_window;
XArchive *archive = NULL;
XArchive *sub_archive = NULL;
XArchiveSupport *support = NULL;
XArchiveSupport *sub_support = NULL;

void
xa_show_about(GtkWidget *widget, gpointer data)
{
	static GtkWidget *about = NULL;
	const char *authors[] = {"\nMain Developer:\nGiuseppe Torelli - Colossus <colossus73@gmail.com>\n\nDevelopers:\nStephan Arts <psyBSD@gmail.com>\nSalvatore Santagati <salvatore.santagati@gmail.com>\n",NULL};
	const char *documenters[] = {"\nThanks to:\nBenedikt Meurer for helping me with DnD.\n\nFabian Pedrosa, Szervac Attila and Iuri Stanchev\nfor testing this release.\n\nUracile for the stunning logo.\n\nEugene Ostapets and Enrico Troeger\nfor russian and german translation.\n\nThe people of gtk-app-devel-list\nwho kindly answered my questions.", NULL};
	if (about != NULL)
	{
		gtk_window_present (GTK_WINDOW (about));
		return;
	}
	about = gtk_about_dialog_new ();
	g_object_set (about,
"name",  "Xarchiver",
"version", PACKAGE_VERSION,
"copyright", "Copyright 2005-2006 Giuseppe Torelli",
"comments", "A lightweight GTK2 archive manager",
"authors", authors,
"documenters",documenters,
"translator_credits", NULL,
"logo_icon_name", "xarchiver",
"website", "http://xarchiver.sourceforge.net",
"website_label", NULL,
"license",    "Copyright @2005 Giuseppe Torelli - Colossus <gt67@users.sourceforge.net>\n\n"
"This library is free software; you can redistribute it and/or\n"
"modify it under the terms of the GNU Library General Public License as\n"
	"published by the Free Software Foundation; either version 2 of the\n"
	"License, or (at your option) any later version.\n"
	"\n"
	"This library is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
	"Library General Public License for more details.\n"
	"\n"
	"You should have received a copy of the GNU Library General Public\n"
	"License along with the Gnome Library; see the file COPYING.LIB.  If not,\n"
	"write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,\n"
	"Boston, MA 02111-1307, USA.\n",
	NULL);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (about), TRUE);
	g_signal_connect (G_OBJECT (about), "destroy",  G_CALLBACK (gtk_widget_destroyed), &about);
	gtk_window_set_position (GTK_WINDOW (about), GTK_WIN_POS_CENTER);
	gtk_widget_show (about);
}

void
xa_close_archive(GtkWidget *widget, gpointer data)
{
	xa_main_window_clear_list(XA_MAIN_WINDOW(main_window), TRUE);
	xarchiver_archive_destroy(archive);
	if(sub_archive)
		xarchiver_archive_destroy(sub_archive);
	archive = NULL;
	sub_archive = NULL;
	support = NULL;
	sub_support = NULL;

	xa_main_window_set_property_window(XA_MAIN_WINDOW(main_window), NULL);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-test", FALSE);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-extract", FALSE);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-add-folder", FALSE);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-add-file", FALSE);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-cancel", FALSE);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-close", FALSE);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-remove", FALSE);
	xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-properties", FALSE);
}

void
xa_add_folder(GtkWidget *widget, gpointer data)
{
	GSList *list = g_slist_alloc();
	list->data = (gchar *)data;
	if((archive) && (support))
		support->add(archive, list);
}

void
xa_add_files(GtkWidget *widget, gpointer data)
{
	if((archive) && (support))
	{
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), 
			"xa-button-cancel", 
			TRUE);
		support->add(archive, data);
		while (archive->child_pid != 0)
		{
			while (gtk_events_pending())
				gtk_main_iteration();
		}
		if(sub_archive)
		{
			GSList *files = g_slist_alloc();
			files->data = archive->path;
			sub_support->add(sub_archive, files);
			g_slist_free(files);
		}
		support->open(archive);
		while (archive->child_pid != 0)
		{
			while (gtk_events_pending())
				gtk_main_iteration();
		}
		xa_main_window_clear_list(XA_MAIN_WINDOW(main_window), FALSE);
		archive->row = g_list_reverse ( archive->row ); // why this? a hack?!
		xa_main_window_append_list(XA_MAIN_WINDOW(main_window), archive->row);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), 
			"xa-button-cancel", 
			FALSE);
	}
}

void
xa_remove_files(GtkWidget *widget, gpointer data)
{
	if((archive) && (support))
	{
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), 
			"xa-button-cancel", 
			TRUE);
		support->remove(archive, data);
		while (archive->child_pid != 0)
		{
			while (gtk_events_pending())
				gtk_main_iteration();
		}
		if(sub_archive)
		{
			GSList *files = g_slist_alloc();
			files->data = archive->path;
			sub_support->add(sub_archive, files);
			g_slist_free(files);
		}
		support->open(archive);
		while (archive->child_pid != 0)
		{
			while (gtk_events_pending())
				gtk_main_iteration();
		}
		xa_main_window_clear_list(XA_MAIN_WINDOW(main_window), FALSE);
		archive->row = g_list_reverse ( archive->row ); // why this? a hack?!
		xa_main_window_append_list(XA_MAIN_WINDOW(main_window), archive->row);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), 
			"xa-button-cancel", 
			FALSE);
	}
}

void
xa_open_archive(GtkWidget *widget, gpointer data)
{
	gchar *filename = data;
	if(archive)
	{
		xa_main_window_clear_list(XA_MAIN_WINDOW(main_window), TRUE);
		xarchiver_archive_destroy(archive);
		archive = NULL;
	}
	archive = xarchiver_archive_new(filename, XARCHIVETYPE_UNKNOWN);
	if(archive == NULL)
	{
		//TODO: notify the user with a gtk_dialog error message
		g_warning("Archive %s is not supported\n", filename);
		return;
	}
	else
	{
		xa_main_window_set_statusbar_value(XA_MAIN_WINDOW(main_window), "Opening archive...");
		if((archive->type == XARCHIVETYPE_BZIP2) || (archive->type == XARCHIVETYPE_GZIP))
		{
			sub_archive = archive;
			support = xarchiver_find_archive_support(sub_archive);
			support->extract(sub_archive, "/tmp/xarchiver.tmp", NULL, FALSE);
			archive = NULL;
			archive = xarchiver_archive_new("/tmp/xarchiver.tmp", XARCHIVETYPE_UNKNOWN);
			if(!archive)
			{
				archive = sub_archive;
				sub_archive = NULL;
			}
			else
				sub_support = xarchiver_find_archive_support(sub_archive);
		}
	}
	if(archive)
	{
		support = xarchiver_find_archive_support(archive);
		support->open(archive);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), 
			"xa-button-cancel", 
			TRUE);
		while (archive->child_pid != 0)
		{
			while (gtk_events_pending())
				gtk_main_iteration();
		}
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), 
			"xa-button-cancel", 
			FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), 
			"xa-button-close", 
			TRUE);
		xa_main_window_set_list_interface(XA_MAIN_WINDOW(main_window), 
				support->n_columns, 
				support->column_names, 
				support->column_types);
		archive->row = g_list_reverse ( archive->row );
		xa_main_window_append_list(XA_MAIN_WINDOW(main_window), archive->row);
		if(archive->has_passwd)
			xa_main_window_set_widget_visible(XA_MAIN_WINDOW(main_window), "xa-passwd", TRUE);
		else
			xa_main_window_set_widget_visible(XA_MAIN_WINDOW(main_window), "xa-passwd", FALSE);
		if(support->add)
		{
			xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-add-folder", TRUE);
			xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-add-file", TRUE);
		}
		if(support->extract)
			xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-extract", TRUE);
		if(support->remove)
			xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-remove", TRUE);


		xa_main_window_set_statusbar_value(XA_MAIN_WINDOW(main_window), "Done");
	}
}

int main(int argc, char **argv)
{
	int c = 0;
	gchar *filename = NULL;
	xarchiver_init();
	gtk_init(&argc, &argv);

	while(1)
	{
		c = getopt_long(argc, argv, "f:a:e:", NULL, NULL);
		if(c == -1)
			break;
		switch(c)
		{
			case('f'):
				/* archive file */
				break;
			case('a'):
				/* file (or folder) to add */
				break;
			case('e'):
				/* destination-path (extract) */
				break;
		}
	}

	if(filename)
	{
		
	}
	else
	{
		main_window = xa_main_window_new();
		gtk_widget_set_size_request(main_window, 600, 400);
		GtkWidget *prop_dialog = xa_property_dialog_new(GTK_WINDOW(main_window));
		xa_property_dialog_add_property(XA_PROPERTY_DIALOG(prop_dialog), "filename", "/etc/passwd");

		g_signal_connect(G_OBJECT(main_window), "destroy", gtk_main_quit, NULL);
		//g_signal_connect(G_OBJECT(main_window), "xa_new_archive", G_CALLBACK(open_archive), NULL);
		g_signal_connect(G_OBJECT(main_window), "xa_open_archive", G_CALLBACK(xa_open_archive), NULL);
		//g_signal_connect(G_OBJECT(main_window), "xa_test_archive", G_CALLBACK(xa_test_archive), NULL);
		g_signal_connect(G_OBJECT(main_window), "xa_close_archive", G_CALLBACK(xa_close_archive), NULL);
		//g_signal_connect(G_OBJECT(main_window), "xa_extract_archive", G_CALLBACK(xa_extract_archive), NULL);
		g_signal_connect(G_OBJECT(main_window), "xa_add_folders", G_CALLBACK(xa_add_folder), NULL);
		g_signal_connect(G_OBJECT(main_window), "xa_add_files", G_CALLBACK(xa_add_files), NULL);
		g_signal_connect(G_OBJECT(main_window), "xa_remove_files", G_CALLBACK(xa_remove_files), NULL);
		//g_signal_connect(G_OBJECT(main_window), "xa_cancel_operation", G_CALLBACK(xa_cancel_operation), NULL);
		g_signal_connect(G_OBJECT(main_window), "xa_show_about", G_CALLBACK(xa_show_about), NULL);


		gtk_widget_show_all(main_window);
	
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-test", FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-extract", FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-add-folder", FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-add-file", FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-cancel", FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-close", FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-remove", FALSE);
		xa_main_window_set_widget_sensitive(XA_MAIN_WINDOW(main_window), "xa-button-properties", FALSE);

		xa_main_window_set_statusbar_value(XA_MAIN_WINDOW(main_window), "Xarchiver 0.3.9 svn");
		//xa_main_window_set_property_window(XA_MAIN_WINDOW(main_window), XA_PROPERTY_DIALOG(prop_dialog));
	
		gtk_main();
	}
	xarchiver_destroy();
	return 0;
}
