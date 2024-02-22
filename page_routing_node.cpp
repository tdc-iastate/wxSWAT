#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/custom.h"
#include "../util/Timestamp.h"
#include "../util/device_coordinate.h"
#ifdef _ODBC_TDC
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_field_set.h"
#else
#include "../util/db.h"
#endif
#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATReservoir.h"
#include "resource.h"
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include "../i_swat_database/SWATOutputHRUAnnual.h"
#include "../i_swat_database/SWATOutputHRUMonthly.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
#include "../i_swat_database/SWATOutputSubBasinDaily.h"
#include "../i_swat_database/SWATOutputHRUDaily.h"
#include "../i_swat_database/output_column_position.h"
#include "../i_swat_database/swat_output.h"
#include "../i_swat_database/SWATManagement.h"
#include "../i_swat_database/SWATTemporalConservationPractices.h"
#include "../i_swat_database/rowset_hru.h"
#include "../i_swat_database/rowset_subbasin.h"
#include "../i_swat_database/SWATPotentialEvapotranspiration.h"
#include "../i_swat_database/SWATControlRecord.h"
#include "page_routing_node.h"

BEGIN_EVENT_TABLE(page_routing_node, wxPanel)
EVT_BUTTON(BUTTON_RESERVOIR, page_routing_node::OnReservoir)
EVT_BUTTON(BUTTON_POINT_SOURCE, page_routing_node::OnPointSource)
EVT_COMBOBOX(COMBO_COMMAND, page_routing_node::OnSelChangeCommand)
END_EVENT_TABLE()


page_routing_node::page_routing_node
	(wxWindow *parent,
	const int id,
	swat_routing_node *p_node,
	SWATControlRecord *p_control_record,
	swat_controller *controller)
	// list_field_help *p_help_list)
	: wxPanel(parent, id)
{
	dynamic_string label;
	node = p_node;
	control_record = p_control_record;
	// help_list = p_help_list;

	wxFlexGridSizer *grid = new wxFlexGridSizer (2, wxSize (4, 4));

	static_command_number = new wxStaticText (this, STATIC_COMMAND_NUMBER, wxT("Command #"));
	grid->Add (static_command_number);

	{
		wxArrayString items;
		for (int command = 0;
		command < SWAT_ROUTING_HIGHEST_COMMAND_NUMBER;
		++command) {
			label.format ("%d ", command);
			label += route_command_name (command);
			items.Add (label.get_text ());
		}

		combo_command = new wxComboBox (this, COMBO_COMMAND, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
	}
	grid->Add (combo_command, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, wxID_ANY, "Hydrographic Storage Location:"));
	edit_hsl = new long_integer_edit (this, EDIT_HSL, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add (edit_hsl, 0, wxALIGN_RIGHT);

	static_parameter_1 = new wxStaticText (this, STATIC_PARAMETER_1, wxT("Parameter 1:"));
	grid->Add (static_parameter_1);
	edit_parameter_1 = new long_integer_edit (this, EDIT_PARAMETER_1, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add (edit_parameter_1, 0, wxALIGN_RIGHT);

	static_parameter_2 = new wxStaticText (this, STATIC_PARAMETER_2, wxT("Parameter 2:"));
	grid->Add (static_parameter_2);
	edit_parameter_2 = new long_integer_edit (this, EDIT_PARAMETER_2, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add (edit_parameter_2, 0, wxALIGN_RIGHT);

	static_parameter_3 = new wxStaticText (this, STATIC_PARAMETER_3, wxT("Parameter 3:"));
	grid->Add (static_parameter_3);
	edit_parameter_3 = new long_integer_edit (this, EDIT_PARAMETER_3, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add (edit_parameter_3, 0, wxALIGN_RIGHT);

	static_parameter_4 = new wxStaticText (this, STATIC_PARAMETER_4, wxT("Parameter 4:"));
	grid->Add (static_parameter_4);
	edit_parameter_4 = new float_edit (this, EDIT_PARAMETER_4, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add (edit_parameter_4, 0, wxALIGN_RIGHT);

	static_parameter_5 = new wxStaticText (this, STATIC_PARAMETER_5, wxT("Parameter 5:"));
	grid->Add (static_parameter_5);
	edit_parameter_5 = new long_integer_edit (this, EDIT_PARAMETER_5, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add (edit_parameter_5, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, wxID_ANY, "Filename 1:"));
	edit_filename_1 = new wxTextCtrl (this, EDIT_FILENAME_1, wxEmptyString, wxDefaultPosition, wxSize (200, 24));
	grid->Add (edit_filename_1);

	grid->Add (new wxStaticText (this, wxID_ANY, "Filename 2:"));
	edit_filename_2 = new wxTextCtrl (this, EDIT_FILENAME_2, wxEmptyString, wxDefaultPosition, wxSize (200, 24));
	grid->Add (edit_filename_2);

	grid->AddSpacer (0);
	button_reservoir = new wxButton (this, BUTTON_RESERVOIR, wxT("Reservoir"));
	grid->Add (button_reservoir, 0, wxALIGN_RIGHT);

	grid->AddSpacer (0);
	button_point_source = new wxButton (this, BUTTON_POINT_SOURCE, wxT("Point Source"));
	grid->Add (button_point_source, 0, wxALIGN_RIGHT);

	SetSizerAndFit(grid);

	label.format ("%d", (int) node->CommandNumber);
	static_command_number->SetLabel (label.get_text ());
	combo_command->SetSelection (node->Command);
	edit_hsl->Set (node->HydrographStorageLocation);
	edit_parameter_1->Set (node->Parameter1);
	edit_parameter_2->Set (node->Parameter2);
	edit_parameter_3->Set (node->Parameter3);
	edit_parameter_4->Set (node->Parameter4);
	edit_parameter_5->Set (node->Parameter5);
	edit_filename_1->SetValue (node->Filename1);
	edit_filename_2->SetValue (node->Filename2);
}

page_routing_node::~page_routing_node()
{
}

bool page_routing_node::read_controls ()

{
	node->Command = combo_command->GetSelection () + 1;
	node->HydrographStorageLocation = edit_hsl->Get ();
	node->Parameter1 = edit_parameter_1->Get ();
	node->Parameter2 = edit_parameter_2->Get ();
	node->Parameter3 = edit_parameter_3->Get ();
	node->Parameter4 = edit_parameter_4->Get ();
	node->Parameter5 = edit_parameter_5->Get ();
	strcpy_s (node->Filename1, SWAT_ROUTING_FILENAME_LENGTH, edit_filename_1->GetValue ().ToAscii ());
	strcpy_s (node->Filename2, SWAT_ROUTING_FILENAME_LENGTH, edit_filename_2->GetValue ().ToAscii ());
	return true;
}


void page_routing_node::OnReservoir
	(wxCommandEvent &)

{
	ASSERT (node->Command == SWAT_ROUTING_COMMAND_ROUTRES);
	if (((swat_routing_node_routeres *) node)->reservoir == NULL) {
		// try adding a reservoir but don't keep it
		// unless OK pressed in property_sheet_reservoir
		((swat_routing_node_routeres *) node)->reservoir = new SWATReservoir;
		((swat_routing_node_routeres *) node)->reservoir->ID = control_record->ID;
		((swat_routing_node_routeres *) node)->reservoir->HydrographStorageLocation = node->HydrographStorageLocation;
		/*
		propsheet_reservoir p (NULL, ((swat_routing_node_routeres *) node)->reservoir, help_list, SWATVersion);
		if (p.DoModal () != IDOK) {
			delete ((swat_routing_node_routeres *) node)->reservoir;
			((swat_routing_node_routeres *) node)->reservoir = NULL;
		}
		*/
	}
	else {
		/*
		propsheet_reservoir p (NULL, ((swat_routing_node_routeres *) node)->reservoir, help_list, SWATVersion);
		p.DoModal ();
		*/
	}
}

void page_routing_node::OnPointSource
	(wxCommandEvent &)

{
	switch (node->Command) {
		case SWAT_ROUTING_COMMAND_RECCONST:
			// if (((swat_routing_node_reccnst *) node)->point_source == NULL) {
				// try adding a reservoir but don't keep it
				// unless OK pressed in property_sheet_reservoir
			// ((swat_routing_node_reccnst *) node)->point_source = new SWATPointSources;
			((swat_routing_node_reccnst *) node)->point_source.ID = control_record->ID;
			((swat_routing_node_reccnst *) node)->point_source.HydrographStorageLocation = node->HydrographStorageLocation;
				/*
				dialog_point_source p (NULL, ((swat_routing_node_reccnst *) node)->point_source, 0);
				if (p.DoModal () != IDOK) {
					delete ((swat_routing_node_reccnst *) node)->point_source;
					((swat_routing_node_reccnst *) node)->point_source = NULL;
				}
				*/
			break;
		case SWAT_ROUTING_COMMAND_RECALL_MONTH:
			{
				// if (((swat_routing_node_recmon *) node)->point_source_data == NULL)
				//	((swat_routing_node_recmon *) node)->point_source_data = new swat_point_source_monthly;

				/*
				dialog_monthly_point_sources pm (NULL, ((swat_routing_node_recmon *) node)->point_source_data, node);
				pm.DoModal ();
				*/
			}												
			break;
		case SWAT_ROUTING_COMMAND_RECALL_DAY:
			{
				// if (((swat_routing_node_recday *) node)->point_source_data == NULL)
				//	((swat_routing_node_recday *) node)->point_source_data = new swat_point_source_daily;

				/*
				dialog_monthly_point_sources pd (NULL, ((swat_routing_node_recday *) node)->point_source_data, node);
				pd.DoModal ();
				*/
			}
			break;
		case SWAT_ROUTING_COMMAND_RECALL_APEX:
			{

				/*
				dialog_monthly_point_sources pa (NULL, ((swat_routing_node_apex *) node)->point_source_apex, node);
				pa.DoModal ();
				*/
			}
	}
}

void page_routing_node::OnSelChangeCommand
	(wxCommandEvent &)

{
	BYTE command;
	command = (BYTE) combo_command->GetSelection ();

	switch (command) {
		case SWAT_ROUTING_COMMAND_FINISH: // 0
			static_parameter_1->SetLabelText ("");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_SUBBASIN: // 1
			static_parameter_1->SetLabelText ("Subbasin:");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("");
			static_parameter_5->SetLabelText ("GIS Code:");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_ROUTE: // 2
			static_parameter_1->SetLabelText ("Reach:");
			static_parameter_2->SetLabelText ("Inflow HSL:");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("Fraction of overland flow:");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_ROUTRES: // 3
			static_parameter_1->SetLabelText ("Reservoir:");
			static_parameter_2->SetLabelText ("Inflow HSL:");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (true);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_TRANSFER: // 4
			static_parameter_1->SetLabelText ("Water source number:");
			static_parameter_2->SetLabelText ("Destination type:");
			static_parameter_3->SetLabelText ("Destination number:");
			static_parameter_4->SetLabelText ("Transfer amount:");
			static_parameter_5->SetLabelText ("Transfer code:");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_ADD: // 5
			static_parameter_1->SetLabelText ("HSL 1:");
			static_parameter_2->SetLabelText ("HSL 2:");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_RECALL_MONTH: // 7
			static_parameter_1->SetLabelText ("File number:");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("Drainage area, km^2:");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (true);
			break;
		case SWAT_ROUTING_COMMAND_RECALL_YEAR: // 8
			static_parameter_1->SetLabelText ("File number:");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("Drainage area, km^2:");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_SAVE: // 9
			static_parameter_1->SetLabelText ("");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_RECALL_DAY: // 10
			static_parameter_1->SetLabelText ("File number:");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("Drainage area, km^2:");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (true);
			break;
		case SWAT_ROUTING_COMMAND_RECCONST: // 11
			static_parameter_1->SetLabelText ("File number:");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("Drainage area, km^2:");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (true);
			break;
		case SWAT_ROUTING_COMMAND_STRUCTURE: // 12
			static_parameter_1->SetLabelText ("Inflow HSL:");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("Aeration coefficient:");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		case SWAT_ROUTING_COMMAND_RECALL_APEX: // 13
			static_parameter_1->SetLabelText ("File number:");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("Drainage area, km^2:");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (true);
			break;
		case SWAT_ROUTING_COMMAND_SAVECONC: // 14
			static_parameter_1->SetLabelText ("File number:");
			static_parameter_2->SetLabelText ("Printing frequency:");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
			break;
		default:
			static_parameter_1->SetLabelText ("");
			static_parameter_2->SetLabelText ("");
			static_parameter_3->SetLabelText ("");
			static_parameter_4->SetLabelText ("");
			static_parameter_5->SetLabelText ("");
			button_reservoir->Enable (false);
			button_point_source->Enable (false);
	}
}
