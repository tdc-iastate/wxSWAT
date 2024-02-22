#include "../util/utility_afx.h"
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <mutex>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/propdlg.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../util/custom.h"
#include "../util/dialog_error_list.h"
#include "../util/interface_window.h"
#include "../util/interface_window_wx.h"
#include "../util/list_field_help.h"
#include "../util/device_coordinate.h"

#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"

// #include "bitmaps/wizard_control_record.xpm"
#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATSoilLayers.h"
#include "../i_swat_database/SWATSoils.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATUrban.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/swat_routing_node.h"
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
#include "../i_swat_database/SWATWeatherHistorical.h"
#include "../i_swat_database/SWATWeatherByMonth.h"
#include "../i_swat_database/SWATWeatherStations.h"
#include "../i_swat_database/swat_weather_list.h"
#include "../i_swat_database/rowset_subbasin.h"
#include "../i_swat_database/SWATPotentialEvapotranspiration.h"
#include "../i_swat_database/SWATControlRecord.h"
#include "../i_swat_database/swat_controller.h"
#include "../i_swat_database/SWATRetentionIrrigationBasin.h"
#include "../i_swat_database/SWATSedimentationFiltrationBasin.h"
#include "../i_swat_database/SWATStreamWaterQuality.h"
#include "../i_swat_database/SWATPond.h"
#include "../i_swat_database/SWATWaterUse.h"
#include "page_basin_3.h"
#include "../weather/averager_weighted.h"
#include "propsheet_hru.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(page_basin_3, wxPanel)
END_EVENT_TABLE()

page_basin_3::page_basin_3
	(wxWindow *parent,
	const int id,
	SWATControlRecord *pcontrol,
	swat_controller *pcontroller,
	list_field_help_swat *p_help,
	odbc_database *p_db)
	: wxPanel (parent, id)

{
	wxBoxSizer *stack, *row;

	control = pcontrol;
	controller = pcontroller;
	help = p_help;
	db = p_db;

	stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Soil phosphorus method:")), 0, wxALL, 5);
	row->AddStretchSpacer ();
	{
		wxString items[2];
		items[0] = "0 new soil phosphorus routines";
		items[1] = "1 original soil phosphorus subroutines";
		combo_sol_p_model = new wxComboBox (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (169, 90), 2, items, wxCB_DROPDOWN);
	}
	row->Add (combo_sol_p_model);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Unit hydrograph method:")), 0, wxALL, 5);
	row->AddStretchSpacer ();
	{
		wxString items[2];
		items[0] = "1 triangular UH";
		items[1] = "2 gamma function UH";
		combo_iuh = new wxComboBox (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (169, 90), 2, items, wxCB_DROPDOWN);
	}
	row->Add (combo_iuh);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Unit hydrograph method:")), 0, wxALL, 5);
	row->AddStretchSpacer ();
	{
		wxString items[3];
		items[0] = "0 Bagnold model";
		items[1] = "1 Brownlie model";
		items[2] = "2 Yang model";
		combo_sub_chsed = new wxComboBox (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (169, 90), 3, items, wxCB_DROPDOWN);
	}
	row->Add (combo_sub_chsed);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Particle size median, mm:")), 0, wxALL, 5);
	row->AddStretchSpacer ();
	edit_ch_d50 = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_ch_d50, 0, wxALL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);
	
	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Particle size standard deviation:")), 0, wxALL, 5);
	row->AddStretchSpacer ();
	edit_sig_g = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_sig_g, 0, wxALL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Atmospheric Deposition Input:")), 0, wxALL, 5);
	row->AddStretchSpacer ();
	{
		wxString items[3];
		items[0] = "0 Annual Average";
		items[1] = "1 Monthly";
		items[2] = "2 Daily";
		combo_iatmodep = new wxComboBox (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (169, 90), 3, items, wxCB_DROPDOWN);
	}
	row->Add (combo_iatmodep);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	stack->Add (edit_help, 1, wxEXPAND | wxALL);

	SetSizerAndFit(stack);
}


bool page_basin_3::TransferDataToWindow ()

{
	combo_sol_p_model->SetSelection (control->SoilPhosphorusMethod);
	combo_iuh->SetSelection (control->UnitHydrographMethod - 1);
	combo_sub_chsed->SetSelection (control->InstreamSedimentModel);
	edit_ch_d50->Set (control->ParticleSizeMedian);
	edit_sig_g->Set (control->ParticleSizeSTD);
	combo_iatmodep->SetSelection (control->AtmosphericDeposition);

	return true;
}

bool page_basin_3::TransferDataFromWindow ()

{
	control->SoilPhosphorusMethod = combo_sol_p_model->GetSelection ();
	control->UnitHydrographMethod = combo_iuh->GetSelection () + 1;
	control->InstreamSedimentModel = combo_sub_chsed->GetSelection ();
	control->ParticleSizeMedian = edit_ch_d50->Get ();
	control->ParticleSizeSTD = edit_sig_g->Get ();
	control->AtmosphericDeposition = combo_iatmodep->GetSelection ();

	return true;
}

