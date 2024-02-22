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
#include "page_basin_4.h"
#include "../weather/averager_weighted.h"
#include "propsheet_hru.h"
#include "list_field_help_swat.h"

page_basin_4::page_basin_4
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
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Daily maximum half hour rainfall code:"));
	edit_ised_det = new integer_edit (this, EDIT_ISED_DET, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_ised_det);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "Channel water routing method:"));
		wxArrayString items;
		items.Add ("0 - Variable Storage Method");
		items.Add ("1 - Muskingum method");
		combo_irte = new wxComboBox (this, COMBO_IRTE, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
		row->Add (combo_irte);
		stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	}

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Storage time calibration coefficient, normal flow:"));
	edit_msk_co1 = new float_edit (this, EDIT_MSK_CO1, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_msk_co1);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Coefficient, low flow:"));
	edit_msk_co2 = new float_edit (this, EDIT_MSK_CO2, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_msk_co2);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Inflow / outflow weighting factor:"));
	edit_msk_x = new float_edit (this, EDIT_MSK_X, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_msk_x);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	check_channel_degradation_updating = new wxCheckBox (this, CHECK_CHANNEL_DEGRADATION_UPDATING, wxT("Degradation updates channel dimensions"));
	stack->Add (check_channel_degradation_updating);
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Main channel transmission losses to deep aquifer:"));
	edit_trnsrch = new float_edit (this, EDIT_TRNSRCH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_trnsrch);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Reach evaporation adjustment factor:"));
	edit_evrch = new float_edit (this, EDIT_EVRCH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_evrch);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "Pesticide routed through watershed:"));
		wxArrayString items;
		std::vector <SWATPesticide>::const_iterator pesticide;
		for (pesticide = controller->pesticides->pesticides.begin ();
		pesticide != controller->pesticides->pesticides.end ();
		++pesticide)
			items.Add (pesticide->Name);
		combo_pesticide = new wxComboBox (this, COMBO_PESTICIDE, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
		row->Add (combo_pesticide);
		stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	}

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "Daily curve number calculation:"));
		wxArrayString items;
		items.Add ("0 - soil moisture");
		items.Add ("1 - plant evapotranspiration");
		combo_icn = new wxComboBox (this, COMBO_ICN, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
		row->Add (combo_icn);
		stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	}

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Evapotranspiration curve number coefficient:"));
	edit_cncoef = new float_edit (this, EDIT_CNCOEF, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cncoef);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Denitrification exponential rate coefficient:"));
	edit_cdn = new float_edit (this, EDIT_CDN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cdn);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Denitrification threshold water content:"));
	edit_sdnco = new float_edit (this, EDIT_SDNCO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_sdnco);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	stack->Add (edit_help);
	SetSizerAndFit(stack);

}

BEGIN_EVENT_TABLE(page_basin_4, wxPanel)
END_EVENT_TABLE()

bool page_basin_4::TransferDataToWindow ()

{
	edit_ised_det->Set ((int) control->DailyMaxHalfhourRainfall);
	combo_irte->SetSelection (control->ChannelWaterRoutingMethod);
	edit_msk_co1->Set (control->StorageTimeNormalCalibration);
	edit_msk_co2->Set (control->StorageTimeLowCalibration);
	edit_msk_x->Set (control->InflowOutflowWeighting);
	check_channel_degradation_updating->SetValue (control->ChannelDegradationUpdating == VARIANT_TRUE);
	edit_trnsrch->Set (control->TransmissionLossDeepAquifer);
	edit_evrch->Set (control->ReachEvaporationFactor);
	combo_pesticide->SetSelection (control->PesticideIDRouted - 1);
	combo_icn->SetSelection (control->DailyCurveNumberMethod);
	edit_cncoef->Set (control->ETCurveNumberFactor);
	edit_cdn->Set (control->DenitrificationRateFactor);
	edit_sdnco->Set (control->DenitrificationThresholdWater);

	return true;
}

bool page_basin_4::TransferDataFromWindow ()

{
	control->DailyMaxHalfhourRainfall = (BYTE) edit_ised_det->Get ();
	control->ChannelWaterRoutingMethod = combo_irte->GetSelection ();
	control->StorageTimeNormalCalibration = edit_msk_co1->Get ();
	control->StorageTimeLowCalibration = edit_msk_co2->Get ();
	control->InflowOutflowWeighting = edit_msk_x->Get ();
	control->ChannelDegradationUpdating = check_channel_degradation_updating->GetValue () ? VARIANT_TRUE : VARIANT_FALSE;
	control->TransmissionLossDeepAquifer = edit_trnsrch->Get ();
	control->ReachEvaporationFactor = edit_evrch->Get ();
	control->PesticideIDRouted = combo_pesticide->GetSelection () + 1;
	control->DailyCurveNumberMethod = combo_icn->GetSelection ();
	control->ETCurveNumberFactor = edit_cncoef->Get ();
	control->DenitrificationRateFactor = edit_cdn->Get ();
	control->DenitrificationThresholdWater = edit_sdnco->Get ();
	return true;
}
