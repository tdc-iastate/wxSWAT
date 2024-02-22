// dialog_select.cpp : implementation file
//

#include "../util/utility_afx.h"
#include <vector>
#include <map>
#include "resource.h"
#include "../util/custom.h"
#include "../util/list.h"
#include "../util/dynamic_string.h"
// #include "../util/edit_help_text.h"
#include "../util/list_field_help.h"
#include "../util/dialog_error_list.h"
#include <vector>
#ifdef _ODBC_TDC
#include <odbcinst.h>
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"
#endif
#include "list_field_help_swat.h"
#include "dialog_select.h"
#include "../i_swat_database/SWATManagement.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "dialog_pesticide.h"
#include "dialog_fertilizer.h"
#include "../i_swat_database/SWATDeclarations.h"


BEGIN_EVENT_TABLE(dialog_select, wxDialog)
	EVT_LISTBOX_DCLICK (LIST_CHOICES, dialog_select::OnDblclkChoices)
	EVT_BUTTON(BUTTON_EDIT, dialog_select::OnEdit)
END_EVENT_TABLE()

dialog_select::dialog_select
	(wxWindow* pParent,
	void **pick,
	std::vector <class T> *p_list,
	int code,
	bool edit,
	list_field_help_swat *p_help,
	odbc_database *db)
	: wxDialog(pParent, wxID_ANY, "Select")
{
	//{{AFX_DATA_INIT(dialog_select)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	item_list = p_list;
	selection = pick;
	op_code = code;
	allow_edit = edit;
	help_list = p_help;
	database = db;

    wxBoxSizer *stack = new wxBoxSizer (wxVERTICAL);

	wxBoxSizer *row = new wxBoxSizer (wxHORIZONTAL);
	list_choices = new wxListBox (this, LIST_CHOICES);
	row->Add (list_choices);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	button_edit = new wxButton (this, BUTTON_EDIT, L"Edit");
	row->Add (button_edit);
	stack->Add (row);

	stack->Add (CreateButtonSizer (wxOK | wxCANCEL));

	SetSizerAndFit(stack);

	show_list ();
}

void dialog_select::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {

		if (selection
		&& (*selection != NULL)) {
			int count;
			if ((count = list_choices->GetSelection ()) != -1) {
				switch (op_code) {
					case SWAT_OP_CODE_PLANT:
						{
							// std::vector <T>::const_iterator crop;
							std::vector <SWATCrop> *croplist;
							croplist = (std::vector <SWATCrop> *) item_list;
							*selection = (SWATCrop *) &(*croplist) [count];
						}
						break;
					case SWAT_OP_CODE_TILLAGE:
					case SWAT_OP_CODE_HARVEST:
					case SWAT_OP_CODE_KILL:
						{
							std::vector <SWATTillage> *production_list;
							production_list = (std::vector <SWATTillage> *) item_list;
							*selection = (SWATTillage *) &(*production_list) [count];
						}
						break;
					case SWAT_OP_CODE_FERTILIZE:
					case SWAT_OP_CODE_INIT_AUTO_FERT:
					case SWAT_OP_CODE_GRAZE:
						{
							std::vector <SWATFertilizer> *production_list;
							production_list = (std::vector <SWATFertilizer> *) item_list;
							*selection = (SWATFertilizer *) &(*production_list) [count];
						}
						break;
					case SWAT_OP_CODE_PESTICIDE:
						{
							std::vector <SWATPesticide> *production_list;
							production_list = (std::vector <SWATPesticide> *) item_list;
							*selection = (SWATPesticide *) &(*production_list) [count];
						}

				}
			}
			else
				*selection = NULL;
		}
	}
	wxDialog::EndModal (retCode);
}

void dialog_select::show_list ()

{
	dynamic_string entry;
	int count;

	list_choices->Clear ();

	count = 0;
	switch (op_code) {
		case SWAT_OP_CODE_PLANT:
			{
				SWATCrop *crop;
				std::vector <SWATCrop> *croplist;
				std::vector <SWATCrop>::const_iterator item;

				croplist = (std::vector <SWATCrop> *) item_list;

				for (item = croplist->begin (), count = 0;
				item != croplist->end ();
				++item, ++count) {

					crop = (SWATCrop *) &*item;
					entry.format ("%d ", crop->ID);
					entry += crop->Name;
					list_choices->Append (entry.get_text ());
					list_choices->SetClientData (count, crop);
					if (selection
					&& (*selection == crop))
						list_choices->SetSelection (count);
				}
			}
			break;
		case SWAT_OP_CODE_TILLAGE:
		case SWAT_OP_CODE_HARVEST:
		case SWAT_OP_CODE_KILL:
			{
				SWATTillage *till;
				std::vector <SWATTillage> *croplist;
				std::vector <SWATTillage>::const_iterator item;

				croplist = (std::vector <SWATTillage> *) item_list;

				for (item = croplist->begin (), count = 0;
				item != croplist->end ();
				++item, ++count) {

					till = (SWATTillage *) &*item;
					entry.format ("%d ", till->ID);
					entry += till->Name;
					list_choices->Append (entry.get_text ());
					list_choices->SetClientData (count, till);
					if (selection
					&& (*selection == till))
						list_choices->SetSelection (count);
				}
			}
			break;
		case SWAT_OP_CODE_FERTILIZE:
			{
				SWATFertilizer *till;
				std::vector <SWATFertilizer> *croplist;
				std::vector <SWATFertilizer>::const_iterator item;

				croplist = (std::vector <SWATFertilizer> *) item_list;

				for (item = croplist->begin (), count = 0;
				item != croplist->end ();
				++item, ++count) {

					till = (SWATFertilizer *) &*item;
					entry.format ("%d ", till->ID);
					entry += till->Name;
					list_choices->Append (entry.get_text ());
					list_choices->SetClientData (count, till);
					if (selection
					&& (*selection == till))
						list_choices->SetSelection (count);
				}
			}
			break;
		case SWAT_OP_CODE_PESTICIDE:
			{
				SWATPesticide *till;
				std::vector <SWATPesticide> *croplist;
				std::vector <SWATPesticide>::const_iterator item;

				croplist = (std::vector <SWATPesticide> *) item_list;

				for (item = croplist->begin (), count = 0;
				item != croplist->end ();
				++item, ++count) {
					till = (SWATPesticide *) &*item;
					entry.format ("%d ", till->ID);
					entry += till->Name;
					entry += " ";
					entry += till->Description;
					list_choices->Append (entry.get_text ());
					list_choices->SetClientData (count, till);
					if (selection
					&& (*selection == till))
						list_choices->SetSelection (count);
				}
			}
	}
}

/////////////////////////////////////////////////////////////////////////////
// dialog_select message handlers

void dialog_select::OnDblclkChoices
	(wxCommandEvent &ev) 
{
	if (allow_edit) {
		OnEdit (ev);
	}
	else
		EndModal (wxID_OK);
}

void dialog_select::OnEdit
	(wxCommandEvent &) 
{
	// long id;
	// if ((id = (long) list_choices->GetClientData (list_choices.GetCurSel ())) != -1) {
	switch (op_code) {
		case SWAT_OP_CODE_PESTICIDE:
			{
				SWATPesticide *item;
				if ((item = (SWATPesticide *) list_choices->GetClientData (list_choices->GetSelection ())) != NULL) {

					dialog_pesticide d (this, item, help_list);
					if (d.ShowModal () == wxID_OK) {
						dynamic_string error_message;
						item->update (database, error_message);
						if (error_message.get_length () > 0) {
							dialog_error_list de (this, L"Write Crop", L"", error_message);
							de.ShowWindowModal ();
						}
						show_list ();
					}
				}
			}
			break;
			case SWAT_OP_CODE_FERTILIZE:
				{
					SWATFertilizer *item;
					if ((item = (SWATFertilizer *) list_choices->GetClientData (list_choices->GetSelection ())) != NULL) {

						dialog_fertilizer d (this, item, help_list);
						if (d.ShowModal () == wxID_OK) {
							dynamic_string error_message;
							item->update (database, error_message);
							if (error_message.get_length () > 0) {
								dialog_error_list de (this, L"Write Crop", L"", error_message);
								de.ShowWindowModal ();
							}
							show_list ();
						}
					}
				}
		}
}

