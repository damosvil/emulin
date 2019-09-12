/*
 * VentanaFrame.cpp
 *
 *  Created on: 21 ago. 2019
 *      Author: iso9660
 */

#include <VentanaFrame.h>
#include <VentanaFrameSignal.h>

namespace ui {

VentanaFrame::VentanaFrame(GtkBuilder *builder, ldf *db, const char *frame_name)
{
	GtkTreeIter it;

	// Store input info
	this->builder = builder;
	this->db = db;
	this->frame_name = frame_name;
	this->handle = gtk_builder_get_object(builder, "VentanaFrame");

	// Pin widgets
	G_PIN(VentanaFrameName);
	G_PIN(VentanaFrameID);
	G_PIN(VentanaFrameSize);
	G_PIN(VentanaFramePublisher);
	G_PIN(VentanaFrameSignalsList);
	G_PIN(VentanaFrameSignalsSelection);
	G_PIN(VentanaFrameSignalsNew);
	G_PIN(VentanaFrameSignalsEdit);
	G_PIN(VentanaFrameSignalsDelete);
	G_PIN(VentanaFrameAccept);
	G_PIN(VentanaFrameCancel);

	// Fill publisher field with static data, selecting the publisher by default
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(g_VentanaFramePublisher));
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaFramePublisher), (gchar *)db->GetMasterNode()->GetName(), (gchar *)db->GetMasterNode()->GetName());
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaFramePublisher), (gchar *)db->GetMasterNode()->GetName());
	for (uint32_t ix = 0; ix < db->GetSlaveNodesCount(); ix++)
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g_VentanaFramePublisher), (gchar *)db->GetSlaveNodeByIndex(ix)->GetName(), (gchar *)db->GetSlaveNodeByIndex(ix)->GetName());

	// Prepare signals list
	PrepareListSignals();

	// Fill fields with data
	ldfframe *f = db->GetFrameByName(Str(frame_name));
	if (f != NULL)
	{
		// Name
		EntrySet(g_VentanaFrameName, f->GetName());

		// ID
		EntrySet(g_VentanaFrameID, "%d", f->GetId());

		// Size
		EntrySet(g_VentanaFrameSize, "%d", f->GetSize());

		// Publisher
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(g_VentanaFramePublisher), (gchar *)f->GetPublisher());

		// Signals
		GtkListStore *ls = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaFrameSignalsList)));
		gtk_list_store_clear(ls);
		for (uint32_t ix = 0; ix < f->GetSignalsCount(); ix++)
		{
			ldfframesignal *fs = f->GetSignal(ix);
			ldfsignal *ss = db->GetSignalByName(fs->GetName());

			// Add Offset, name and bit size
			gtk_list_store_append(ls, &it);
			gtk_list_store_set(ls, &it, 0, (gchar *)GetStrPrintf("%d", fs->GetOffset()), -1);
			gtk_list_store_set(ls, &it, 1, (gchar *)fs->GetName(), -1);
			gtk_list_store_set(ls, &it, 2, (gchar *)GetStrPrintf("%d", ss->GetBitSize()), -1);
		}
	}
	else
	{
		// Name
		EntrySet(g_VentanaFrameName, "frame");

		// ID
		EntrySet(g_VentanaFrameID, "0");

		// Size
		EntrySet(g_VentanaFrameSize, "8");

		// Publisher should be a default one

		// No subscribers
		GtkListStore *ls = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaFrameSignalsList)));
		gtk_list_store_clear(ls);
	}

	// Connect text fields
	G_CONNECT_INSTXT(VentanaFrameName, NAME_EXPR);
	G_CONNECT_INSTXT(VentanaFrameID, INT_0_63_EXPR);
	G_CONNECT_INSTXT(VentanaFrameSize, INT_1_8_EXPR);

	// Connect lists
	G_CONNECT(VentanaFramePublisher, changed);
	G_CONNECT(VentanaFrameSignalsSelection, changed);
	G_CONNECT(VentanaFrameSignalsList, row_activated);

	// Connect buttons
	G_CONNECT(VentanaFrameSignalsNew, clicked);
	G_CONNECT(VentanaFrameSignalsEdit, clicked);
	G_CONNECT(VentanaFrameSignalsDelete, clicked);
	G_CONNECT(VentanaFrameAccept, clicked);
	G_CONNECT(VentanaFrameCancel, clicked);
}

VentanaFrame::~VentanaFrame()
{
	// Disconnect text fields
	G_DISCONNECT_FUNC(VentanaFrameName, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaFrameID, EditableInsertValidator);
	G_DISCONNECT_FUNC(VentanaFrameSize, EditableInsertValidator);

	// Disconnect lists
	G_DISCONNECT_DATA(VentanaFramePublisher, this);
	G_DISCONNECT_DATA(VentanaFrameSignalsSelection, this);
	G_DISCONNECT_DATA(VentanaFrameSignalsList, this);

	// Disconnect buttons
	G_DISCONNECT_DATA(VentanaFrameSignalsNew, this);
	G_DISCONNECT_DATA(VentanaFrameSignalsEdit, this);
	G_DISCONNECT_DATA(VentanaFrameSignalsDelete, this);
	G_DISCONNECT_DATA(VentanaFrameAccept, this);
	G_DISCONNECT_DATA(VentanaFrameCancel, this);
}

ldfframe *VentanaFrame::ShowModal(GObject *parent)
{
	ldfframe *res = NULL;

	// Put this window always on top of parent
	gtk_window_set_transient_for(GTK_WINDOW(handle), GTK_WINDOW(parent));

	// Show dialog
	if (gtk_dialog_run(GTK_DIALOG(handle)))
	{
		// Name
		const char *name = EntryGetStr(g_VentanaFrameName);

		// ID
		uint16_t id = EntryGetInt(g_VentanaFrameID);

		// Size
		uint32_t size = EntryGetInt(g_VentanaFrameSize);

		// Publisher
		const gchar *publisher = gtk_combo_box_get_active_id(GTK_COMBO_BOX(g_VentanaFramePublisher));

		// Compose the signal
		res = new ldfframe(Str(name), id, Str(publisher), size);

		// Signals
		char *strOffset;
		char *signal;
		GtkTreeIter iter;
		GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaFrameSignalsList));
		if (gtk_tree_model_get_iter_first(model, &iter))
		{
			do
			{
				gtk_tree_model_get(model, &iter, 0, &strOffset, -1);
				gtk_tree_model_get(model, &iter, 1, &signal, -1);
				res->AddSignal(new ldfframesignal(Str(signal), MultiParseInt(strOffset)));
			}
			while (gtk_tree_model_iter_next(model, &iter));
		}
	}
	gtk_widget_hide(GTK_WIDGET(handle));

	return res;
}

void VentanaFrame::PrepareListSignals()
{
	const char *columns[] = { "Offset", "Signal", "Bit size", NULL };

	TreeViewPrepare(g_VentanaFrameSignalsList, columns);

	// Disable edit and delete buttons
	WidgetEnable(g_VentanaFrameSignalsEdit, false);
	WidgetEnable(g_VentanaFrameSignalsDelete, false);
}

uint32_t VentanaFrame::CalculateMaxSignalOffset()
{
	uint32_t max_signal_pos = 0;
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(g_VentanaFrameSignalsList));
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			char *p;
			uint32_t q;
			gtk_tree_model_get(model, &iter, 0, &p, -1);	// Offset
			q = MultiParseInt(p);
			gtk_tree_model_get(model, &iter, 2, &p, -1);	// Bit size
			q += MultiParseInt(p);
			max_signal_pos = (q > max_signal_pos) ? q : max_signal_pos;
		}
		while (gtk_tree_model_iter_next(model, &iter));
	}

	return max_signal_pos;
}

void VentanaFrame::OnVentanaFramePublisher_changed(GtkComboBoxText *widget, gpointer user_data)
{
	VentanaFrame *v = (VentanaFrame *)user_data;
	GtkListStore *ls = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaFrameSignalsList)));

	// When publisher changes all signals will be removed from list
	gtk_list_store_clear(ls);
}

void VentanaFrame::OnVentanaFrameSignalsList_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	VentanaFrame *v = (VentanaFrame *)user_data;

	gtk_widget_activate(GTK_WIDGET(v->g_VentanaFrameSignalsEdit));
}

void VentanaFrame::OnVentanaFrameSignalsSelection_changed(GtkTreeSelection *widget, gpointer user_data)
{
	VentanaFrame *v = (VentanaFrame *)user_data;

	bool enable = gtk_tree_selection_count_selected_rows(widget) == 1;
	WidgetEnable(v->g_VentanaFrameSignalsEdit, enable);
	WidgetEnable(v->g_VentanaFrameSignalsDelete, enable);
}

void VentanaFrame::OnVentanaFrameSignalsNew_clicked(GtkButton *button, gpointer user_data)
{
	VentanaFrame *v = (VentanaFrame *)user_data;

	const char *publisher = gtk_combo_box_get_active_id(GTK_COMBO_BOX(v->g_VentanaFramePublisher));
	char *signal_names[1000];
	int signal_names_count = 0;
	VentanaFrameSignal::frame_signals_raw_t frame_signals[1000];
	int frame_signals_count = 0;

	// Select all signals that have the same publisher than the frame and not in use by the frame yet
	for (uint32_t i = 0; i < v->db->GetSignalsCount(); i++)
	{
		// Skip signals
		ldfsignal *s = v->db->GetSignalByIndex(i);
		if (!s->PublisherIs(Str(publisher)))
			continue;

		signal_names[signal_names_count++] = (char *)s->GetName();
	}

	// Select all frame signals already selected
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaFrameSignalsList));
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			gtk_tree_model_get(model, &iter, 0, &frame_signals[frame_signals_count].offset, -1);	// Offset
			gtk_tree_model_get(model, &iter, 1, &frame_signals[frame_signals_count].name, -1);		// Name
			frame_signals_count++;
		}
		while (gtk_tree_model_iter_next(model, &iter));
	}

	// Remove all signals previously selected
	for (int i = 0; i < signal_names_count; )
	{
		// Skip no selected signals
		bool selected = false;
		for (int j = 0; !selected && (j < frame_signals_count); j++)
		{
			selected = Same(signal_names[i], frame_signals[j].name);
		}
		if (!selected)
		{
			i++;
			continue;
		}

		// Remove signal from list
		signal_names_count--;
		for (int j = i; j < signal_names_count; j++)
			signal_names[j] = signal_names[j + 1];
	}

	// Check whether there is a signal to add
	if (signal_names_count ==  0)
	{
		ShowErrorMessageBox(v->handle, "There are no more suitable signals to choose from.");
		return;
	}

	// Ask user to enter new signal
	VentanaFrameSignal w(v->builder, v->db, signal_names, signal_names_count, frame_signals, frame_signals_count);
	ldfframesignal *fs = w.ShowModal(v->handle);
	if (fs == NULL)
		return;

	// Look in frame_signals where to insert the new framesignal
	int ix;
	for (ix = 0; ix < frame_signals_count; ix++)
		if (MultiParseInt(frame_signals[ix].offset) > fs->GetOffset())
			break;

	// Insert item in list store
	gtk_list_store_insert(GTK_LIST_STORE(model), &iter, ix);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, (gchar *)GetStrPrintf("%d", fs->GetOffset()), -1);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 1, (gchar *)fs->GetName(), -1);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 2, (gchar *)GetStrPrintf("%d", v->db->GetSignalByName(fs->GetName())->GetBitSize()), -1);

	// Delete frame signal
	delete fs;
}

void VentanaFrame::OnVentanaFrameSignalsEdit_clicked(GtkButton *button, gpointer user_data)
{
	GtkTreeIter iter;
	VentanaFrame *v = (VentanaFrame *)user_data;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaFrameSignalsList));
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaFrameSignalsSelection), &model, &iter);

	char *signal_name;
	VentanaFrameSignal::frame_signals_raw_t frame_signals[1000];
	int frame_signals_count = 0;

	// Select current signal to choose from
	gtk_tree_model_get(model, &iter, 1, &signal_name, -1);		// Name

	// Select all frame signals already selected
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			gtk_tree_model_get(model, &iter, 0, &frame_signals[frame_signals_count].offset, -1);	// Offset
			gtk_tree_model_get(model, &iter, 1, &frame_signals[frame_signals_count].name, -1);		// Name
			frame_signals_count++;
		}
		while (gtk_tree_model_iter_next(model, &iter));
	}

	// Ask user to enter new signal
	VentanaFrameSignal w(v->builder, v->db, &signal_name, 1, frame_signals, frame_signals_count);
	ldfframesignal *fs = w.ShowModal(v->handle);
	if (fs == NULL)
		return;

	// Update frame signal
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaFrameSignalsSelection), &model, &iter);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, (gchar *)GetStrPrintf("%d", fs->GetOffset()), -1);

	// Delete frame signal
	delete fs;
}

void VentanaFrame::OnVentanaFrameSignalsDelete_clicked(GtkButton *button, gpointer user_data)
{
	GtkTreeIter iter;
	VentanaFrame *v = (VentanaFrame *)user_data;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(v->g_VentanaFrameSignalsList));
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(v->g_VentanaFrameSignalsSelection), &model, &iter);

	// Remove subscriber from list model
	gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
}

void VentanaFrame::OnVentanaFrameAccept_clicked(GtkButton *button, gpointer user_data)
{
	VentanaFrame *v = (VentanaFrame *)user_data;
	const char *new_frame_name = EntryGetStr(v->g_VentanaFrameName);
	const uint8_t new_frame_id = EntryGetInt(v->g_VentanaFrameID);
	const char *reserved_identifiers[] = { "MasterReq", "SlaveResp", "AssignNAD", "ConditionalChangeNAD", "DataDump", "SaveConfiguration", "AssignFrameIdRange", "FreeFormat", "AssignFrameId", NULL };

	ldfframe *frame_by_new_name = v->db->GetFrameByName(Str(new_frame_name));
	ldfframe *frame_by_old_name = v->db->GetFrameByName(Str(v->frame_name));
	ldfframe *frame_by_new_id = v->db->GetFrameById(new_frame_id);

	// Store maximum frame position depending on the frame size
	uint32_t max_frame_pos = EntryGetInt(v->g_VentanaFrameSize) * 8;

	// Store maximum signal position
	uint32_t max_signal_pos = v->CalculateMaxSignalOffset();

	// Check if identifier is forbidden
	if (GetStrIndexByID(reserved_identifiers, ARR_SIZE(reserved_identifiers), new_frame_name) != ARR_SIZE(reserved_identifiers))
	{
		ShowErrorMessageBox(v->handle, "Identifier '%s' is reserved for diagnostic frames.", new_frame_name);
		return;
	}
	else if (strlen(new_frame_name) == 0)
	{
		ShowErrorMessageBox(v->handle, "Frame name length shall not be 0.");
		return;
	}
	else if (v->frame_name == NULL && frame_by_new_name != NULL)
	{
		ShowErrorMessageBox(v->handle, "Frame name '%s' is already in use.", new_frame_name);
		return;
	}
	else if (v->frame_name != NULL && !Same((char *)v->frame_name, new_frame_name) && frame_by_new_name != NULL)
	{
		ShowErrorMessageBox(v->handle, "Frame name changed to '%s' that is already in use.", new_frame_name);
		return;
	}
	else if (v->frame_name == NULL && frame_by_new_id != NULL)
	{
		ShowErrorMessageBox(v->handle, "Frame ID %d is already in use.", new_frame_id);
		return;
	}
	else if (frame_by_new_id != NULL && frame_by_old_name != NULL && frame_by_new_id != frame_by_old_name)
	{
		ShowErrorMessageBox(v->handle, "Frame ID %d is in use by frame '%s'.", new_frame_id, frame_by_new_id->GetName());
		return;
	}
	else if (max_frame_pos == 0)
	{
		ShowErrorMessageBox(v->handle, "Frame size cannot be 0.");
		return;
	}
	else if (max_frame_pos < max_signal_pos)
	{
		ShowErrorMessageBox(v->handle, "Frame shall have enough bytes for all signals (%d).", (max_signal_pos + 7) / 8);
		return;
	}

	// Return true
	gtk_dialog_response(GTK_DIALOG(v->handle), true);
}

void VentanaFrame::OnVentanaFrameCancel_clicked(GtkButton *button, gpointer user_data)
{
	VentanaFrame *v = (VentanaFrame *)user_data;

	// Return false
	gtk_dialog_response(GTK_DIALOG(v->handle), false);
}


} /* namespace ui */
