/*
 *  Copyright (c) 2006 Giuseppe Torelli <colossus73@gmail.com>
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
 *
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

static GtkTargetEntry src_targets[] =
{
  { "XdndDirectSave0", 0, 1 },
};

static GtkTargetEntry dst_targets[] =
{
  { "text/uri-list", 0, 2 },
};

GtkWidget *create_MainWindow (void)
{
  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();

  MainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (MainWindow, "MainWindow");
  gtk_window_set_title (GTK_WINDOW (MainWindow), "Xarchiver " VERSION);
  //By using GDK_ACTION_MOVE GDK_ACTION_MOVE GDK_ACTION_LINK GDK_ACTION_ASK we have KDE DnD compatibility.
  gtk_drag_dest_set (MainWindow,GTK_DEST_DEFAULT_ALL,dst_targets,1,GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK | GDK_ACTION_ASK);
  gtk_window_set_default_icon_from_file ("xarchiver.png", NULL );
  g_signal_connect (GTK_WINDOW (MainWindow), "drag_data_received", G_CALLBACK (on_drag_data_received), NULL);
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox1, "vbox1");
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (MainWindow), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_set_name (menubar1, "menubar1");
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  menuitem1 = gtk_menu_item_new_with_mnemonic (_("_Archive"));
  gtk_widget_set_name (menuitem1, "menuitem1");
  gtk_widget_show (menuitem1);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem1);

  menuitem1_menu = gtk_menu_new ();
  gtk_widget_set_name (menuitem1_menu, "menuitem1_menu");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menuitem1_menu);

  new1 = gtk_image_menu_item_new_from_stock ("gtk-new", accel_group);
  gtk_widget_set_name (new1, "new1");
  gtk_widget_show (new1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), new1);

  open1 = gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
  gtk_widget_set_name (open1, "open1");
  gtk_widget_show (open1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), open1);

  separatormenuitem1 = gtk_separator_menu_item_new ();
  gtk_widget_set_name (separatormenuitem1, "separatormenuitem1");
  gtk_widget_show (separatormenuitem1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), separatormenuitem1);
  gtk_widget_set_sensitive (separatormenuitem1, FALSE);

  check_menu = gtk_image_menu_item_new_with_mnemonic (_("Test"));
  gtk_widget_show (check_menu);
  gtk_widget_set_sensitive ( check_menu , FALSE);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), check_menu);
  gtk_widget_add_accelerator (check_menu, "activate",accel_group,GDK_t, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  tmp_image = gtk_image_new_from_stock ("gtk-index", GTK_ICON_SIZE_MENU);
  gtk_widget_show (tmp_image);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (check_menu), tmp_image);

  properties = gtk_image_menu_item_new_with_mnemonic (_("Properties"));
  gtk_widget_show (properties);
  gtk_widget_set_sensitive ( properties , FALSE);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), properties);
  gtk_widget_add_accelerator (properties, "activate",accel_group,GDK_p, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  
  tmp_image = gtk_image_new_from_stock ("gtk-properties", GTK_ICON_SIZE_MENU);
  gtk_widget_show (tmp_image);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (properties), tmp_image);

  quit1 = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_set_name (quit1, "quit1");
  gtk_widget_show (quit1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), quit1);

  menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Action"));
  gtk_widget_set_name (menuitem2, "menuitem2");
  gtk_widget_show (menuitem2);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem2);

  menuitem2_menu = gtk_menu_new ();
  gtk_widget_set_name (menuitem2_menu, "menuitem2_menu");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), menuitem2_menu);

  addfile = gtk_menu_item_new_with_mnemonic (_("Add File"));
  gtk_widget_show (addfile);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), addfile);
 
  addfolder = gtk_menu_item_new_with_mnemonic (_("Add Folder"));
  gtk_widget_show (addfolder);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), addfolder);

  separatormenuitem2 = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenuitem2);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), separatormenuitem2);
  gtk_widget_set_sensitive (separatormenuitem2, FALSE);
   
  extract_menu = gtk_menu_item_new_with_mnemonic (_("Extract"));
  gtk_widget_show (extract_menu);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), extract_menu);
  gtk_widget_add_accelerator (extract_menu, "activate",accel_group,GDK_e, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  delete_menu = gtk_image_menu_item_new_from_stock ("gtk-delete", accel_group);
  gtk_widget_set_name (delete_menu, "delete_menu");
  gtk_widget_set_sensitive (delete_menu,FALSE);
  gtk_widget_show (delete_menu);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), delete_menu);
  gtk_widget_add_accelerator (delete_menu, "activate",accel_group,GDK_d, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  view_menu = gtk_image_menu_item_new_with_mnemonic (_("View"));
  gtk_widget_show (view_menu);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), view_menu);
  gtk_widget_set_sensitive (view_menu, FALSE);
  gtk_widget_add_accelerator (view_menu, "activate",accel_group,GDK_v, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  image2 = gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image2);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (view_menu), image2);

  separatormenuitem3 = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenuitem3);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), separatormenuitem3);
  gtk_widget_set_sensitive (separatormenuitem3, FALSE);
  
  /*options1 = gtk_image_menu_item_new_with_mnemonic ("Options");
  gtk_widget_set_name (options1, "options1");
  gtk_widget_show (options1);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), options1);

  image1 = gtk_image_new_from_stock ("gtk-preferences", GTK_ICON_SIZE_MENU);
  gtk_widget_set_name (image1, "image1");
  gtk_widget_show (image1);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (options1), image1);
*/

  add_pwd = gtk_menu_item_new_with_mnemonic (_("Set Password"));
  gtk_widget_show (add_pwd);
  gtk_widget_set_sensitive ( add_pwd , FALSE );
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), add_pwd);
  gtk_widget_add_accelerator (add_pwd, "activate",accel_group,GDK_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  view_shell_output1 = gtk_image_menu_item_new_with_mnemonic (_("View Error Messages"));
  gtk_widget_show (view_shell_output1);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), view_shell_output1);
  gtk_widget_add_accelerator (view_shell_output1, "activate",accel_group,GDK_m, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  image2 = gtk_image_new_from_stock ("gtk-find-and-replace", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image2);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (view_shell_output1), image2);
  
  iso_info = gtk_image_menu_item_new_with_mnemonic (_("Show ISO info"));
  gtk_widget_show (iso_info);
  gtk_widget_set_sensitive ( iso_info , FALSE );
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), iso_info);

  tmp_image = gtk_image_new_from_stock ("gtk-cdrom", GTK_ICON_SIZE_MENU);
  gtk_widget_show (tmp_image);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (iso_info), tmp_image);

  menuitem4 = gtk_menu_item_new_with_mnemonic ("_Help");
  gtk_widget_set_name (menuitem4, "menuitem4");
  gtk_widget_show (menuitem4);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem4);

  menuitem4_menu = gtk_menu_new ();
  gtk_widget_set_name (menuitem4_menu, "menuitem4_menu");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem4), menuitem4_menu);

  about1 = gtk_menu_item_new_with_mnemonic (_("_About"));
  gtk_widget_set_name (about1, "about1");
  gtk_widget_show (about1);
  gtk_container_add (GTK_CONTAINER (menuitem4_menu), about1);

  /*Create the toolbar */
  toolbar1 = gtk_toolbar_new ();
  gtk_widget_set_name (toolbar1, "toolbar1");
  gtk_widget_show (toolbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar1, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar1), GTK_TOOLBAR_BOTH);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1));

  tmp_image = gtk_image_new_from_stock ("gtk-new", tmp_toolbar_icon_size);
  gtk_widget_show (tmp_image);
  New_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("New"));
  gtk_widget_set_name (New_button, "New_button");
  gtk_widget_show (New_button);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (New_button), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), New_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (New_button), tooltips, _("Create a new archive"), NULL);

  tmp_image = gtk_image_new_from_stock ("gtk-open", tmp_toolbar_icon_size);
  gtk_widget_show (tmp_image);
  Open_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("Open"));
  gtk_widget_set_name (Open_button, "Open_button");
  gtk_widget_show (Open_button);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (Open_button), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), Open_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (Open_button), tooltips, _("Open an archive"), NULL);

  separatortoolitem1 = (GtkWidget*) gtk_separator_tool_item_new ();
  gtk_widget_set_name (separatortoolitem1, "separatortoolitem1");
  gtk_widget_show (separatortoolitem1);
  gtk_container_add (GTK_CONTAINER (toolbar1), separatortoolitem1);

  tmp_image = xa_main_window_find_image("add.png", GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_widget_show (tmp_image);
  AddFile_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("Add File"));
  gtk_widget_set_name (AddFile_button, "AddFile_button");
  gtk_widget_show (AddFile_button);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (AddFile_button), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), AddFile_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (AddFile_button), tooltips, _("Add a file to the current archive"), NULL);

  tmp_image = xa_main_window_find_image("add_folder.png", GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_widget_show (tmp_image);
  AddFolder_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("Add Folder"));
  gtk_widget_set_name (AddFolder_button, "AddFolder_button");
  gtk_widget_show (AddFolder_button);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (AddFolder_button), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), AddFolder_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (AddFolder_button), tooltips, _("Add an entire folder to the current archive"), NULL);

  separatortoolitem2 = (GtkWidget*) gtk_separator_tool_item_new ();
  gtk_widget_set_name (separatortoolitem2, "separatortoolitem2");
  gtk_widget_show (separatortoolitem2);
  gtk_container_add (GTK_CONTAINER (toolbar1), separatortoolitem2);

  tmp_image = xa_main_window_find_image("extract.png", GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_widget_show (tmp_image);
  Extract_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("Extract"));
  gtk_widget_set_name (Extract_button, "Extract_button");
  gtk_widget_show (Extract_button);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (Extract_button), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), Extract_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (Extract_button), tooltips, _("Extract files from the current archive; use the mouse to select files individually"), NULL);

  tmp_image = gtk_image_new_from_stock ("gtk-delete", tmp_toolbar_icon_size);
  gtk_widget_show (tmp_image);
  Delete_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("Delete"));
  gtk_widget_set_name (Delete_button, "Delete_button");
  gtk_widget_show (Delete_button);
  gtk_widget_set_sensitive (Delete_button,FALSE);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (Delete_button), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), Delete_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (Delete_button), tooltips, _("Delete files from the current archive"), NULL);

  tmp_image = gtk_image_new_from_stock ("gtk-find", tmp_toolbar_icon_size);
  gtk_widget_show (tmp_image);
  View_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("View"));
  gtk_widget_set_name (View_button, "View_button");
  gtk_widget_show (View_button);
  gtk_widget_set_sensitive (View_button,FALSE);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (View_button), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), View_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (View_button), tooltips, _("View file content in the current archive"), NULL);

  separatortoolitem3 = (GtkWidget*) gtk_separator_tool_item_new ();
  gtk_widget_set_name (separatortoolitem3, "separatortoolitem3");
  gtk_widget_show (separatortoolitem3);
  gtk_container_add (GTK_CONTAINER (toolbar1), separatortoolitem3);

  tmp_image = gtk_image_new_from_stock ("gtk-stop", tmp_toolbar_icon_size);
  gtk_widget_show (tmp_image);
  Stop_button = (GtkWidget*) gtk_tool_button_new (tmp_image, _("Stop"));
  gtk_widget_set_sensitive (Stop_button,FALSE);
  gtk_widget_set_name (Stop_button, "Stop_button");
  gtk_widget_show (Stop_button);
  gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM ( Stop_button ), FALSE);
  gtk_container_add (GTK_CONTAINER (toolbar1), Stop_button);
  gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (Stop_button), tooltips, _("Cancel current operation"), NULL);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_name ( scrolledwindow1, "scrolledwindow1");
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW (scrolledwindow1) , GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
  gtk_widget_show (scrolledwindow1);
  
  treeview1 = gtk_tree_view_new ();
  gtk_widget_set_name (treeview1, "treeview1");
  gtk_widget_show (treeview1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview1);
  gtk_drag_source_set (treeview1, GDK_BUTTON1_MASK, src_targets, 1,GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK | GDK_ACTION_ASK);

  g_signal_connect (G_OBJECT(treeview1), "drag_data_get" , G_CALLBACK (drag_data_get), NULL );
  g_signal_connect (G_OBJECT (treeview1), "drag_begin", G_CALLBACK (drag_begin), NULL);
  g_signal_connect (G_OBJECT (treeview1), "drag_end", G_CALLBACK (drag_end), NULL);

  vbox_body = gtk_vbox_new (FALSE, 2);
  gtk_widget_show (vbox_body);
  gtk_container_set_border_width (GTK_CONTAINER(vbox_body), 2);
  gtk_box_pack_start(GTK_BOX(vbox1), vbox_body, TRUE, TRUE, 0);
 
  gtk_box_pack_start (GTK_BOX (vbox_body), scrolledwindow1, TRUE, TRUE, 0);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

  hbox_sb = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_sb);
  gtk_box_pack_start (GTK_BOX (vbox_body), hbox_sb, FALSE, TRUE, 0);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_box_pack_start (GTK_BOX (hbox_sb), viewport1, TRUE, TRUE, 0);

  info_label = gtk_label_new (NULL);
  gtk_misc_set_alignment (GTK_MISC(info_label), 0.0, 0.5);
  gtk_widget_show (info_label);
  gtk_container_add (GTK_CONTAINER (viewport1), info_label);
  
  viewport2 = gtk_viewport_new (NULL, NULL);
  //gtk_widget_show (viewport2);
  gtk_box_pack_start (GTK_BOX (hbox_sb), viewport2, TRUE, TRUE, 0);

  progressbar = gtk_progress_bar_new ();
  gtk_widget_show (progressbar);
  gtk_widget_set_size_request(progressbar, 80, 1);
  gtk_container_add (GTK_CONTAINER (viewport2), progressbar);

  viewport3 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport3);
  gtk_box_pack_start (GTK_BOX (hbox_sb), viewport3, FALSE, TRUE, 0);

  ebox = gtk_event_box_new();
  pad_image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_AUTHENTICATION, GTK_ICON_SIZE_MENU);
  gtk_container_add (GTK_CONTAINER(ebox), pad_image);
  gtk_widget_show (ebox);
  gtk_container_add (GTK_CONTAINER (viewport3), ebox);
  gtk_widget_set_size_request(ebox, 15, -1);
  pad_tooltip = gtk_tooltips_new ();
  gtk_tooltips_set_tip (pad_tooltip , ebox , _("This archive contains password protected files"), NULL );
  gtk_tooltips_disable ( pad_tooltip );

  g_signal_connect ((gpointer) new1, "activate",
                    G_CALLBACK (xa_new_archive),
                    NULL);
  g_signal_connect ((gpointer) open1, "activate",
                    G_CALLBACK (xa_open_archive),
                    NULL);
  g_signal_connect ((gpointer) check_menu, "activate",
                    G_CALLBACK (xa_test_archive),
                    NULL);
  g_signal_connect ((gpointer) properties, "activate",
                    G_CALLBACK (xa_archive_properties),
                    NULL);
  g_signal_connect ((gpointer) extract_menu, "activate",
                    G_CALLBACK (xa_extract_archive),
                    NULL);
  g_signal_connect ((gpointer) addfile, "activate",
                    G_CALLBACK (xa_add_files_archive),
                    "file");
  g_signal_connect ((gpointer) addfolder, "activate",
                    G_CALLBACK (xa_add_files_archive),
                    "folder");
  g_signal_connect ((gpointer) view_shell_output1, "activate", G_CALLBACK (ShowShellOutput), (gpointer) FALSE);
  g_signal_connect ((gpointer) iso_info, "activate", G_CALLBACK (ShowShellOutput), (gpointer) TRUE);
  g_signal_connect ((gpointer) add_pwd, "activate", G_CALLBACK (Show_pwd_Window), NULL);
  g_signal_connect ((gpointer) quit1, "activate", G_CALLBACK (on_quit1_activate), NULL);
  g_signal_connect ((gpointer) delete_menu, "activate",
                    G_CALLBACK (xa_delete_archive),
                    NULL);
  g_signal_connect ((gpointer) view_menu, "activate",
                    G_CALLBACK (View_File_Window),
                    NULL);
  g_signal_connect ((gpointer) about1, "activate",
                    G_CALLBACK (xa_about),
                    NULL);
  g_signal_connect ((gpointer) New_button, "clicked",
                    G_CALLBACK (xa_new_archive),
                    NULL);
  g_signal_connect ((gpointer) Open_button, "clicked",
                    G_CALLBACK (xa_open_archive),
                    NULL);
  g_signal_connect ((gpointer) AddFile_button, "clicked",
                    G_CALLBACK (xa_add_files_archive),
                    "file");
  g_signal_connect ((gpointer) AddFolder_button, "clicked",
                  G_CALLBACK (xa_add_files_archive), "folder" );
		  
  g_signal_connect ((gpointer) Extract_button, "clicked",
                    G_CALLBACK (xa_extract_archive),
                    NULL);
  g_signal_connect ((gpointer) Delete_button, "clicked",
                    G_CALLBACK (xa_delete_archive),
                    NULL);
  g_signal_connect ((gpointer) View_button, "clicked",
                    G_CALLBACK (View_File_Window),
                    NULL);
  g_signal_connect ((gpointer) Stop_button, "clicked",
                    G_CALLBACK (xa_cancel_archive),
                    NULL);
 g_signal_connect (MainWindow, "key_press_event",
                    G_CALLBACK (key_press_function),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). 
  GLADE_HOOKUP_OBJECT_NO_REF (MainWindow, MainWindow, "MainWindow");
  GLADE_HOOKUP_OBJECT (MainWindow, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (MainWindow, menubar1, "menubar1");
  GLADE_HOOKUP_OBJECT (MainWindow, menuitem1, "menuitem1");
  GLADE_HOOKUP_OBJECT (MainWindow, menuitem1_menu, "menuitem1_menu");
  GLADE_HOOKUP_OBJECT (MainWindow, new1, "new1");
  GLADE_HOOKUP_OBJECT (MainWindow, open1, "open1");
  GLADE_HOOKUP_OBJECT (MainWindow, separatormenuitem1, "separatormenuitem1");
  GLADE_HOOKUP_OBJECT (MainWindow, quit1, "quit1");
  GLADE_HOOKUP_OBJECT (MainWindow, menuitem2, "menuitem2");
  GLADE_HOOKUP_OBJECT (MainWindow, menuitem2_menu, "menuitem2_menu");
  GLADE_HOOKUP_OBJECT (MainWindow, delete_menu, "delete_menu");
  GLADE_HOOKUP_OBJECT (MainWindow, view_shell_output1, "view_shell_output1");
  GLADE_HOOKUP_OBJECT (MainWindow, image2, "image2");
  GLADE_HOOKUP_OBJECT (MainWindow, menuitem4, "menuitem4");
  GLADE_HOOKUP_OBJECT (MainWindow, menuitem4_menu, "menuitem4_menu");
  GLADE_HOOKUP_OBJECT (MainWindow, about1, "about1");
  GLADE_HOOKUP_OBJECT (MainWindow, toolbar1, "toolbar1");
  GLADE_HOOKUP_OBJECT (MainWindow, New_button, "New_button");
  GLADE_HOOKUP_OBJECT (MainWindow, Open_button, "Open_button");
  GLADE_HOOKUP_OBJECT (MainWindow, separatortoolitem1, "separatortoolitem1");
  GLADE_HOOKUP_OBJECT (MainWindow, AddFile_button, "AddFile_button");
  GLADE_HOOKUP_OBJECT (MainWindow, Extract_button, "Extract_button");
  GLADE_HOOKUP_OBJECT (MainWindow, Delete_button, "Delete_button");
  GLADE_HOOKUP_OBJECT (MainWindow, separatortoolitem3, "separatortoolitem3");
  GLADE_HOOKUP_OBJECT (MainWindow, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (MainWindow, treeview1, "treeview1");
  GLADE_HOOKUP_OBJECT_NO_REF (MainWindow, tooltips, "tooltips");*/
  gtk_window_add_accel_group (GTK_WINDOW (MainWindow), accel_group);
  return MainWindow;
}

GtkWidget *prefs (gint selected)
{
  GtkWidget *prefs;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *fixed1;
  GtkWidget *button1;
  GtkWidget *alignment1;
  GtkWidget *hbox1;
  GtkWidget *image1;
  GtkWidget *label1;
  GtkWidget *frame1;
  GtkWidget *alignment2;
  GtkWidget *vbox2;
  GtkWidget *radiobutton1;
  GSList *radiobutton1_group = NULL;
  GtkWidget *radiobutton2;
  GtkWidget *fixed2;
  GtkWidget *radiobutton3;
  GSList *radiobutton3_group = NULL;
  GtkWidget *label3;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  prefs = gtk_dialog_new ();
  if (archive->type != XARCHIVETYPE_BZIP2 && archive->type != XARCHIVETYPE_GZIP)
  {
	if (selected)
		gtk_window_set_title (GTK_WINDOW (prefs), _("Extract ONLY selected files"));
	else
		gtk_window_set_title (GTK_WINDOW (prefs), _("Extract ALL files"));
  }
  else if (archive->type == XARCHIVETYPE_BZIP2)
			gtk_window_set_title (GTK_WINDOW (prefs), _("Extract bzip2 compressed file"));
		else gtk_window_set_title (GTK_WINDOW (prefs), _("Extract gzip compressed file"));
  gtk_window_set_type_hint (GTK_WINDOW (prefs), GDK_WINDOW_TYPE_HINT_DIALOG);
  gtk_window_set_transient_for ( GTK_WINDOW (prefs) , GTK_WINDOW (MainWindow) );
  dialog_vbox1 = GTK_DIALOG (prefs)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  fixed1 = gtk_fixed_new ();
  gtk_widget_show (fixed1);
  gtk_box_pack_start (GTK_BOX (vbox1), fixed1, FALSE, TRUE, 0);
  gtk_widget_set_size_request (fixed1, -1, 38);

  entry1 = gtk_entry_new ();
  gtk_widget_show (entry1);
  gtk_entry_set_activates_default (GTK_ENTRY(entry1), TRUE);
  gtk_fixed_put (GTK_FIXED (fixed1), entry1, 88, 8);
  gtk_widget_set_size_request (entry1, 287, 24);
  if (es_path != NULL) gtk_entry_set_text (GTK_ENTRY (entry1) , es_path);
  
  button1 = gtk_button_new ();
  gtk_widget_show (button1);
  gtk_fixed_put (GTK_FIXED (fixed1), button1, 384, 7);
  gtk_widget_set_size_request (button1, 33, 27);

  GtkTooltips *button_tooltip = gtk_tooltips_new ();
  gtk_tooltips_set_tip (button_tooltip,button1 , _("Choose a folder where to extract files"), NULL );
  
  alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (button1), alignment1);
  g_signal_connect ( (gpointer) button1, "clicked", G_CALLBACK (Show_File_Dialog) ,  "extract" );
  
  hbox1 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (alignment1), hbox1);
  
  image1 = gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (hbox1), image1, FALSE, FALSE, 0);

  label1 = gtk_label_new (_("Extract to:"));
  gtk_widget_show (label1);
  gtk_fixed_put (GTK_FIXED (fixed1), label1, 16, 12);
  gtk_widget_set_size_request (label1, 72, 16);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame1), 4);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_OUT);

  alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (frame1), alignment2);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);

  vbox2 = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (alignment2), vbox2);

  dialog_action_area1 = GTK_DIALOG (prefs)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (prefs), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (prefs), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
  return prefs;
}

GtkWidget *passwd_win ()
{
  GtkWidget *passwd;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox2;
  GtkWidget *fixed1;
  GtkWidget *label2;
  GtkWidget *fixed2;
  GtkWidget *label3;
  GtkWidget *label1;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  passwd = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (passwd),_("Enter Archive Password"));
  gtk_window_set_type_hint (GTK_WINDOW (passwd), GDK_WINDOW_TYPE_HINT_DIALOG);
  gtk_window_set_transient_for ( GTK_WINDOW (passwd) , GTK_WINDOW (MainWindow) );
  dialog_vbox1 = GTK_DIALOG (passwd)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox2, TRUE, TRUE, 0);

  fixed1 = gtk_fixed_new ();
  gtk_widget_show (fixed1);
  gtk_box_pack_start (GTK_BOX (vbox2), fixed1, TRUE, TRUE, 0);

  password_entry = gtk_entry_new ();
  gtk_entry_set_visibility (GTK_ENTRY (password_entry), FALSE);
  gtk_entry_set_activates_default(GTK_ENTRY(password_entry), TRUE);
  gtk_widget_show (password_entry);
  gtk_fixed_put (GTK_FIXED (fixed1), password_entry, 77, 16);
  gtk_widget_set_size_request (password_entry, 304, 24);

  label2 = gtk_label_new (_("Password:"));
  gtk_widget_show (label2);
  gtk_fixed_put (GTK_FIXED (fixed1), label2, 5, 16);
  gtk_widget_set_size_request (label2, 72, 24);

  fixed2 = gtk_fixed_new ();
  gtk_widget_show (fixed2);
  gtk_box_pack_start (GTK_BOX (vbox2), fixed2, TRUE, TRUE, 0);

  repeat_password = gtk_entry_new ();
  gtk_entry_set_visibility (GTK_ENTRY (repeat_password), FALSE);
  gtk_entry_set_activates_default(GTK_ENTRY(repeat_password), TRUE);
  gtk_widget_show (repeat_password);
  gtk_fixed_put (GTK_FIXED (fixed2), repeat_password, 77, 4);
  gtk_widget_set_size_request (repeat_password, 304, 24);

  label3 = gtk_label_new (_("Retype it:"));
  gtk_widget_show (label3);
  gtk_fixed_put (GTK_FIXED (fixed2), label3, 5, 4);
  gtk_widget_set_size_request (label3, 72, 24);
  
  label1 = gtk_label_new (_("Please note that the password is automatically resetted each time you open or create a new archive."));
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox2), label1, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_CENTER);
  gtk_label_set_line_wrap (GTK_LABEL (label1), TRUE);
  gtk_misc_set_padding (GTK_MISC (label1), 0, 3);

  dialog_action_area1 = GTK_DIALOG (passwd)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (passwd), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (passwd), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). 
  GLADE_HOOKUP_OBJECT_NO_REF (passwd, passwd, "passwd");
  GLADE_HOOKUP_OBJECT_NO_REF (passwd, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (passwd, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (passwd, fixed1, "fixed1");
  GLADE_HOOKUP_OBJECT (passwd, password_entry, "password_entry");
  GLADE_HOOKUP_OBJECT (passwd, label2, "label2");
  GLADE_HOOKUP_OBJECT (passwd, fixed2, "fixed2");
  GLADE_HOOKUP_OBJECT (passwd, repeat_password, "repeat_password");
  GLADE_HOOKUP_OBJECT (passwd, label3, "label3");
  GLADE_HOOKUP_OBJECT (passwd, label1, "label1");
  GLADE_HOOKUP_OBJECT_NO_REF (passwd, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (passwd, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (passwd, okbutton1, "okbutton1");*/
  return passwd;
}

GtkWidget *view_win ()
{
  GtkWidget *view_window;
  GtkWidget *scrolledwindow2;

  view_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (view_window), _("View File Window"));
  gtk_window_set_destroy_with_parent (GTK_WINDOW (view_window), TRUE);
  gtk_window_set_type_hint (GTK_WINDOW (view_window), GDK_WINDOW_TYPE_HINT_UTILITY);
  gtk_window_set_position (GTK_WINDOW (view_window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW (view_window), 450, 300);
  gtk_window_set_modal ( GTK_WINDOW (view_window),TRUE);
  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow2);
  gtk_container_add (GTK_CONTAINER (view_window), scrolledwindow2);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_SHADOW_IN);
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW (scrolledwindow2) , GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

  textview1 = gtk_text_view_new ();
  gtk_widget_show (textview1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow2), textview1);
  gtk_container_set_border_width (GTK_CONTAINER (textview1), 5);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (textview1), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview1), FALSE);
  viewtextbuf = gtk_text_view_get_buffer ( GTK_TEXT_VIEW (textview1) );
  gtk_text_buffer_get_start_iter (viewtextbuf, &viewenditer);
  return view_window;
}

GtkWidget *create_archive_properties_window ()
{
  GtkWidget *archive_properties_window;
  GtkWidget *table1;
  GtkWidget *path_label;
  GtkWidget *modified_label;
  GtkWidget *size_label;
  GtkWidget *content_label;
  GtkWidget *compression_label;
  GtkWidget *number_of_files_label;
  GtkWidget *number_of_dirs_label;
  GtkWidget *name_label;

  archive_properties_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (archive_properties_window), _("Archive Properties Window"));
  gtk_window_set_destroy_with_parent (GTK_WINDOW (archive_properties_window), TRUE);
  gtk_window_set_transient_for ( GTK_WINDOW (archive_properties_window) , GTK_WINDOW (MainWindow) );
  gtk_window_set_position (GTK_WINDOW (archive_properties_window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable (GTK_WINDOW (archive_properties_window), FALSE);
  gtk_window_set_modal (GTK_WINDOW (archive_properties_window), TRUE);
  gtk_window_set_type_hint (GTK_WINDOW (archive_properties_window), GDK_WINDOW_TYPE_HINT_UTILITY);

  table1 = gtk_table_new (8, 2, TRUE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (archive_properties_window), table1);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 6);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 6);

  name_label = gtk_label_new ("");
  set_label ( name_label , _("Name:"));
  gtk_widget_show (name_label);
  gtk_table_attach (GTK_TABLE (table1), name_label, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (name_label), 0.99, 0.5);

  path_label = gtk_label_new ("");
  set_label ( path_label , _("Path:"));
  gtk_widget_show (path_label);
  gtk_table_attach (GTK_TABLE (table1), path_label, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (path_label), 0.99, 0.5);

  modified_label = gtk_label_new ("");
  set_label ( modified_label , _("Modified on:"));
  gtk_widget_show (modified_label);
  gtk_table_attach (GTK_TABLE (table1), modified_label, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (modified_label), 0.99, 0.5);
  
  size_label = gtk_label_new ("");
  set_label ( size_label , _("Archive size:"));
  gtk_widget_show (size_label);
  gtk_table_attach (GTK_TABLE (table1), size_label, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (size_label), 0.99, 0.5);

  content_label = gtk_label_new ("");
  set_label ( content_label , _("Content size:"));
  gtk_widget_show (content_label);
  gtk_table_attach (GTK_TABLE (table1), content_label, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (content_label), 0.99, 0.5);
  
  compression_label = gtk_label_new ("");
  set_label ( compression_label , _("Compression ratio:"));
  gtk_widget_show (compression_label);
  gtk_table_attach (GTK_TABLE (table1), compression_label, 0, 1, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (compression_label), 0.99, 0.5);
  
  number_of_files_label = gtk_label_new ("");
  set_label ( number_of_files_label , _("Number of files:"));
  gtk_widget_show (number_of_files_label);
  gtk_table_attach (GTK_TABLE (table1), number_of_files_label, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (number_of_files_label), 0.99, 0.5);

  number_of_dirs_label = gtk_label_new ("");
  set_label ( number_of_dirs_label , _("Number of dirs:"));
  gtk_widget_show (number_of_dirs_label);
  gtk_table_attach (GTK_TABLE (table1), number_of_dirs_label, 0, 1, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (number_of_dirs_label), 0.99, 0.5);

  compression_data = gtk_entry_new ();
  gtk_editable_set_editable (GTK_EDITABLE (compression_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (compression_data), FALSE);
  gtk_widget_show (compression_data);
  gtk_table_attach (GTK_TABLE (table1), compression_data, 1, 2, 7, 8,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);

  number_of_dirs_data = gtk_entry_new ();
  gtk_editable_set_editable (GTK_EDITABLE (number_of_dirs_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (number_of_dirs_data), FALSE);
  gtk_widget_show (number_of_dirs_data);
  gtk_table_attach (GTK_TABLE (table1), number_of_dirs_data, 1, 2, 6, 7,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);

  number_of_files_data = gtk_entry_new ();
  gtk_editable_set_editable (GTK_EDITABLE (number_of_files_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (number_of_files_data), FALSE);
  gtk_widget_show (number_of_files_data);
  gtk_table_attach (GTK_TABLE (table1), number_of_files_data, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);

  content_data = gtk_entry_new ();
  gtk_editable_set_editable (GTK_EDITABLE (content_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (content_data), FALSE);
  gtk_widget_show (content_data);
  gtk_table_attach (GTK_TABLE (table1), content_data, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);

  size_data = gtk_entry_new ();
  gtk_editable_set_editable (GTK_EDITABLE (size_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (size_data), FALSE);
  gtk_widget_show (size_data);
  gtk_table_attach (GTK_TABLE (table1), size_data, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);

  modified_data = gtk_entry_new ();
  gtk_widget_show (modified_data);
  gtk_table_attach (GTK_TABLE (table1), modified_data, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_editable_set_editable (GTK_EDITABLE (modified_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (modified_data), FALSE);

  path_data = gtk_entry_new ();
  gtk_editable_set_editable (GTK_EDITABLE (path_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (path_data), FALSE);
  gtk_widget_show (path_data);
  gtk_table_attach (GTK_TABLE (table1), path_data, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);

  name_data = gtk_entry_new ();
  gtk_editable_set_editable (GTK_EDITABLE (name_data), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (name_data), FALSE);
  gtk_widget_show (name_data);
  gtk_table_attach (GTK_TABLE (table1), name_data, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);
  return archive_properties_window;
}

void set_label (GtkWidget *label , gchar *text)
{
    gchar *tmp_markup = g_strdup_printf ("<b>%s</b>",text );
    gtk_label_set_markup ( GTK_LABEL (label) , tmp_markup);
    g_free (tmp_markup);
}

int xa_progressbar_pulse (gpointer data)
{
	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressbar) );
	if (GTK_WIDGET_VISIBLE(viewport2))
		return TRUE;
	else
		return FALSE;
}