/*
 * VentanaSenal.cpp
 *
 *  Created on: 12 ago. 2019
 *      Author: iso9660
 */

#include <string.h>
#include <VentanaSelectNode.h>
#include <VentanaSenal.h>


namespace ui {

VentanaSenal::VentanaSenal(GtkBuilder *builder, ldf *db, const char *signal_name)
{
	GtkTreeIter it;

	// Store input info
	this->builder = builder;
	this->db = db;
	this->signal_name = signal_name;
	this->handle = gtk_builder_get_object(builder, "VentanaSenal");

	// Pin widgets
	G_PIN(VentanaSenalName);
	G_PIN(VentanaSenalBitSize);
	G_PIN(VentanaSenalDefaultValue);
	G_PIN(VentanaSenalPublisher);
	G_PIN(VentanaSenalSubscriberList);
	G_PIN(VentanaSenalSubscriberSelection);
	G_PIN(VentanaSenalSubscriberNew);
	G_PIN(VentanaSenalSubscriberEdit);
	G_PIN(VentanaSenalSubscriberDelete);
	G_PIN(VentanaSenalAccept);
	G_PIN(VentanaSenalCancel);

	// Fill publisher field with static data, selecting the publisher by default
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaSenalPublisher));
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaSenalPublisher), (gchar *)db->GetMasterNode()->GetName(), (gchar *)db->GetMasterNode()->GetName());
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaSenalPublisher), (gchar *)db->GetMasterNode()->GetName());
	for (uint32_t ix = 0; ix < db->GetSlaveNodesCount(); ix++)
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaSenalPublisher), (gchar *)db->GetSlaveNodeByIndex(ix)->GetName(), (gchar *)db->GetSlaveNodeByIndex(ix)->GetName());

	// Prepare subscribers list
	PrepareListSubscribers();

	// Fill fields with data
	ldfsignal *s = db->GetSignalByName((uint8_t *)signal_name);
	if (s != NULL)
	{
		// Name
		EntrySet(g_VentanaSenalName, s->GetName());

		// Bit size
		EntrySet(g_VentanaSenalBitSize, "%d", s->GetBitSize());

		// Default value
		EntrySet(g_VentanaSenalDefaultValue, "%d", s->GetDefaultValue());

		// Publisher
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaSenalPublisher), (gchar *)s->GetPublisher());

		// Subscribers
		GtkListStore *ls = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaSenalSubscriberList)));
		gtk_list_store_clear(ls);
		for (uint32_t ix = 0; ix < s->GetSubscribersCount(); ix++)
		{
			gtk_list_store_append(ls, &it);
			gtk_list_store_set(ls, &it, 0, (gchar *)s->GetSubscriber(ix), -1);
		}
	}
	else
	{
		// Name
		EntrySet(g_VentanaSenalName, "signal");

		// Bit size
		EntrySet(g_VentanaSenalBitSize, "1");

		// Default value
		EntrySet(g_VentanaSenalDefaultValue, "0");

		// Publisher should be a default one

		// No subscribers
		GtkListStore *ls = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaSenalSubscriberList)));
		gtk_list_store_clear(ls);
	}

	// Connect text fields
	G_CONNECT_INSTXT(VentanaSenalName, NAME_EXPR);
	G_CONNECT_INSTXT(VentanaSenalBitSize, INT3_EXPR);
	G_CONNECT_INSTXT(VentanaSenalDefaultValue, INT5_EXPR);

	// Connect lists
	G_CONNECT(VentanaSenalPublisher, changed);
	G_CONNECT(VentanaSenalSubscriberSelection, changed);
	G_CONNECT(VentanaSenalSubscriberList, row_activated);

	// Connect buttons
	G_CONNECT(VentanaSenalSubscriberNew, clicked);
	G_CONNECT(VentanaSenalSubscriberEdit, clicked);
	G_CONNECT(VentanaSenalSubscriberDelete, clicked);
	G_CONNECT(VentanaSenalAccept, clicked);
	G_CONNECT(VentanaSenalCancel, clicked);
}

VentanaSenal::~VentanaSenal()
{
	// Disconnect text fields
	G_DISCONNECT_FUNC(VentanaSenalName, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaSenalBitSize, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaSenalDefaultValue, EditableInsertValidator);

	// Disconnect lists
	G_DISCONNECT_DATA(VentanaSenalPublisher, this);
	G_DISCONNECT_DATA(VentanaSenalSubscriberSelection, this);
	G_DISCONNECT_DATA(VentanaSenalSubscriberList, this);

	// Disconnect buttons
	G_DISCONNECT_DATA(VentanaSenalSubscriberNew, this);
	G_DISCONNECT_DATA(VentanaSenalSubscriberEdit, this);
	G_DISCONNECT_DATA(VentanaSenalSubscriberDelete, this);
	G_DISCONNECT_DATA(VentanaSenalAccept, this);
	G_DISCONNECT_DATA(VentanaSenalCancel, this);
}

ldfsignal *VentanaSenal::ShowModal(GObject *parent)
{
	ldfsignal *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Name
		const char *signal_name = EntryGetStr(g_VentanaSenalName);

		// Bit size
		uint16_t bit_size = EntryGetInt(g_VentanaSenalBitSize);

		// Default value
		uint32_t default_value = EntryGetInt(g_VentanaSenalDefaultValue);

		// Publisher
		const gchar *publisher = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaSenalPublisher));

		// Subscribers
		char *subscribers[1000];
		uint32_t subscribers_count = 0;
		GtkTreeIter iter;
		GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaSenalSubscriberList));
		if (gtk_tree_model_get_iter_first(model, &iter))
		{
			do
			{
				gtk_tree_model_get(model, &iter, 0, &subscribers[subscribers_count++], -1);
			}
			while (gtk_tree_model_iter_next(model, &iter));
		}

		// Compose the signal
		res = new ldfsignal((uint8_t *)signal_name, bit_size, default_value, (uint8_t *)publisher, (uint8_t **)subscribers, subscribers_count);
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaSenal::PrepareListSubscribers()
{
	const char *columns[] = { "Subscriber", NULL };

	// Prepare tree view
	TreeViewPrepare(g_VentanaSenalSubscriberList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_VentanaSenalSubscriberEdit, false);
	WidgetEnable(g_VentanaSenalSubscriberDelete, false);
}

int VentanaSenal::GetAllFreeSubscriberNodes(const char **nodes)
{
	const char *nodes_used[1000];
	int nodes_used_count = 0;
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaSenalSubscriberList));
	int count = 0;

	// Initialize count

	// Get all nodes
	nodes[count] = (char *)db->GetMasterNode()->GetName();
	count += 1;
	for (uint32_t ix = 0; ix < db->GetSlaveNodesCount(); ix++)
	{
		nodes[count] = (char *)db->GetSlaveNodeByIndex(ix)->GetName();
		count += 1;
	}

	// Get all nodes in use
	nodes_used[nodes_used_count++] = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaSenalPublisher));
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			gtk_tree_model_get(model, &iter, 0, &nodes_used[nodes_used_count++], -1);
		}
		while (gtk_tree_model_iter_next(model, &iter));
	}

	// Remove nodes in use from all nodes list
	for (int ix = 0; ix < count; )
	{
		// Check whether node in use
		bool in_use = false;
		for (int jx = 0; !in_use && jx < nodes_used_count; jx++)
			in_use = Same(nodes[ix], nodes_used[jx]);
		if (!in_use)
		{
			ix++;
			continue;
		}

		// Remove node
		count -= 1;
		for (int jx = ix; jx < count; jx++)
			nodes[jx] = nodes[jx + 1];
	}

	return count;
}

void VentanaSenal::OnVentanaSenalPublisher_changed(GtkComboBoxText *widget, gpointer user_data)
{
	VentanaSenal *v = (VentanaSenal *)user_data;
	const gchar *publisher_name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(widget));
	GtkTreeModel *model = GTK_TREE_MODEL(gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaSenalSubscriberList)));
	GtkTreeIter iter;
	const gchar *model_publisher_name;

	// Look for publisher in publishers list and delete it
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			// Get publisher in list
			gtk_tree_model_get(model, &iter, 0, &model_publisher_name, -1);
			if (!Same(publisher_name, model_publisher_name))
				continue;

			// Remove publisher from model
			gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
			break;
		} while (gtk_tree_model_iter_next(model, &iter));
	}
}

void VentanaSenal::OnVentanaSenalSubscriberList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaSenal *v = (VentanaSenal *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_VentanaSenalSubscriberEdit));
}

void VentanaSenal::OnVentanaSenalSubscriberSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaSenal *v = (VentanaSenal *)user_data;

	// Update subscriber list buttons
	gboolean enable = gtk_tree_selection_count_selected_rows(widget);
	WidgetEnable(v->g_VentanaSenalSubscriberEdit, enable);
	WidgetEnable(v->g_VentanaSenalSubscriberDelete, enable);
}

void VentanaSenal::OnVentanaSenalSubscriberNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSenal *v = (VentanaSenal *)user_data;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaSenalSubscriberList));
	const char *nodes[1000];
	int nodes_count = 0;
	GtkTreeIter iter;

	// Get all nodes
	nodes_count = v->GetAllFreeSubscriberNodes(nodes);
	if (nodes_count == 0)
	{
		ShowErrorMessageBox(v->handle, "There are no more nodes for subscription");
		return;
	}

	VentanaSelectNode w(v->builder, nodes, nodes_count);
	const char *new_node_name = w.ShowModal(v->handle);

	// Add node to list
	if (new_node_name != NULL)
	{
		gtk_list_store_append(GTK_LIST_STORE(model), &iter);
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, new_node_name, -1);
	}
}

void VentanaSenal::OnVentanaSenalSubscriberEdit_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSenal *v = (VentanaSenal *)user_data;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaSenalSubscriberList));
	const char *nodes[1000];
	int nodes_count = 0;
	GtkTreeIter iter;

	// Get current selected node
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaSenalSubscriberSelection), &model, &iter);
	gtk_tree_model_get(model, &iter, 0, &nodes[nodes_count++], -1);

	// Get other nodes
	nodes_count = v->GetAllFreeSubscriberNodes(nodes + 1);

	VentanaSelectNode w(v->builder, nodes, nodes_count + 1);
	const char *new_node_name = w.ShowModal(v->handle);

	// Update node in list
	if (new_node_name != NULL)
	{
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, new_node_name, -1);
	}
}

void VentanaSenal::OnVentanaSenalSubscriberDelete_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSenal *v = (VentanaSenal *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;

	// Remove subscriber from list model
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaSenalSubscriberSelection), &model, &iter);
	gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
}

void VentanaSenal::OnVentanaSenalAccept_clicked(GtkButton *button, gpointer user_data)
{
	GtkTreeIter iter;
	VentanaSenal *v = (VentanaSenal *)user_data;
	const char *new_signal_name = EntryGetStr(v->g_VentanaSenalName);
	int new_signal_size = EntryGetInt(v->g_VentanaSenalBitSize);

	if (strlen(new_signal_name) == 0)
	{
		ShowErrorMessageBox(v->handle, "Signal name length shall not be 0.");
		return;
	}
	else if (v->signal_name == NULL && v->db->GetSignalByName((uint8_t *)new_signal_name) != NULL)
	{
		ShowErrorMessageBox(v->handle, "Signal name '%s' is already in use.", new_signal_name);
		return;
	}
	else if (v->signal_name != NULL && !Same((char *)v->signal_name, new_signal_name) && v->db->GetSignalByName((uint8_t *)new_signal_name) != NULL)
	{
		ShowErrorMessageBox(v->handle, "Signal name changed to '%s' that is already in use.", new_signal_name);
		return;
	}
	else if (!gtk_tree_model_get_iter_first(gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaSenalSubscriberList)), &iter))
	{
		ShowErrorMessageBox(v->handle, "Signals shall have subscribers.");
		return;
	}
	else if (new_signal_size == 0)
	{
		ShowErrorMessageBox(v->handle, "Signal bit size shall be bigger than 0.");
		return;
	}

	// Return true
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaSenal::OnVentanaSenalCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaSenal *v = (VentanaSenal *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}


} /* namespace ui */
