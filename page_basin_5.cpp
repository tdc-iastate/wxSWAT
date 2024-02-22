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
#include "page_basin_5.h"
#include "../weather/averager_weighted.h"
#include "propsheet_hru.h"
#include "list_field_help_swat.h"

page_basin_5::page_basin_5
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
	row->Add (new wxStaticText (this, wxID_ANY, "Manure fraction colony forming units:"));
	edit_bact_swf = new float_edit (this, EDIT_BACT_SWF, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_bact_swf);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Subdaily unit hydrograph basetime factor:"));
	edit_tb_adj = new float_edit (this, EDIT_TB_ADJ, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_tb_adj);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Impervious layer depth, mm:"));
	edit_depimp_bsn = new float_edit (this, EDIT_DEPIMP_BSN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_depimp_bsn);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Depth to subsurface tile, mm:"));
	edit_ddrain_bsn = new integer_edit (this, EDIT_DDRAIN_BSN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_ddrain_bsn);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Time to drain soil to field capacity, hours:"));
	edit_tdrain_bsn = new integer_edit (this, EDIT_TDRAIN_BSN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_tdrain_bsn);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Lag time, hours:"));
	edit_gdrain_bsn = new integer_edit (this, EDIT_GDRAIN_BSN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_gdrain_bsn);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "Equation:"));
		wxArrayString items;
		items.Add ("0 - Old tile flow equation");
		items.Add ("1 - New tile flow equation");
		combo_itdrn = new wxComboBox (this, COMBO_ITDRN, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
		row->Add (combo_itdrn);
		stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	}

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "Water table depth algorithm:"));
		wxArrayString items;
		items.Add ("0 - Original wt_shall routine");
		items.Add ("1 - New wt_shall routine ");
		combo_iwtdn = new wxComboBox (this, COMBO_IWTDN, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
		row->Add (combo_iwtdn);
		stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	}

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Frozen soil infiltration factor:"));
	edit_cn_froz = new float_edit (this, EDIT_CN_FROZ, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cn_froz);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Maximum day length when dormancy occurs, hours:"));
	edit_dorm_hr = new float_edit (this, EDIT_DORM_HR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_dorm_hr);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Max curve number S factor:"));
	edit_smxco = new float_edit (this, EDIT_SMXCO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_smxco);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Nitrogen fixation coefficient:"));
	edit_fixco = new float_edit (this, EDIT_FIXCO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_fixco);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Nitrogen fixation daily maximum:"));
	edit_nfixmx = new float_edit (this, EDIT_NFIXMX, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_nfixmx);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	stack->Add (edit_help);

	SetSizerAndFit(stack);

}

BEGIN_EVENT_TABLE(page_basin_5, wxPanel)
END_EVENT_TABLE()

bool page_basin_5::TransferDataToWindow ()

{
	edit_bact_swf->Set (control->ManureFractionColonyForming);
	edit_tb_adj->Set (control->SubdailyHydrographBasetime);
	edit_depimp_bsn->Set (control->ImperviousLayerDepth);
	edit_ddrain_bsn->Set (control->DepthtoSubsurfaceDrain);
	edit_tdrain_bsn->Set (control->SoilFieldCapacityDrainTime);
	edit_gdrain_bsn->Set (control->DrainTileLagTime);
	combo_itdrn->SetSelection (control->TileDrainageEquation);
	combo_iwtdn->SetSelection (control->WaterTableDepthAlgorithm);
	edit_cn_froz->Set (control->FrozenSoilInfiltrationFactor);
	edit_dorm_hr->Set (control->DormancyHours);
	edit_smxco->Set (control->SMaxCoefficient);
	edit_fixco->Set (control->NitrogenFixationCoefficient);
	edit_nfixmx->Set (control->NitrogenFixationDailyMaximum);

	return true;
}

bool page_basin_5::TransferDataFromWindow ()

{
	control->ManureFractionColonyForming = edit_bact_swf->Get ();
	control->SubdailyHydrographBasetime = edit_tb_adj->Get ();
	control->ImperviousLayerDepth = edit_depimp_bsn->Get ();
	control->DepthtoSubsurfaceDrain = edit_ddrain_bsn->Get ();
	control->SoilFieldCapacityDrainTime = edit_tdrain_bsn->Get ();
	control->DrainTileLagTime = edit_gdrain_bsn->Get ();
	control->TileDrainageEquation = combo_itdrn->GetSelection ();
	control->WaterTableDepthAlgorithm = combo_iwtdn->GetSelection () + 1;
	control->FrozenSoilInfiltrationFactor = edit_cn_froz->Get ();
	control->DormancyHours = edit_dorm_hr->Get ();
	control->SMaxCoefficient = edit_smxco->Get ();
	control->NitrogenFixationCoefficient = edit_fixco->Get ();
	control->NitrogenFixationDailyMaximum = edit_nfixmx->Get ();

	return true;
}
