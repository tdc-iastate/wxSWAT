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
#include "page_basin_6.h"
#include "../weather/averager_weighted.h"
#include "propsheet_hru.h"
#include "list_field_help_swat.h"


page_basin_6::page_basin_6
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
	row->Add (new wxStaticText (this, wxID_ANY, "Anion Porosity Exclusion:"));
	edit_anionporosityexclusion = new float_edit (this, EDIT_ANIONPOROSITYEXCLUSION, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_anionporosityexclusion);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Channel Organic N:"));
	edit_channelorganicn = new float_edit (this, EDIT_CHANNELORGANICN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelorganicn);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Organic P:"));
	edit_channelorganicp = new float_edit (this, EDIT_CHANNELORGANICP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelorganicp);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Nitrogen Groundwater Half Life:"));
	edit_nitrogengroundwaterhalflife = new float_edit (this, EDIT_NITROGENGROUNDWATERHALFLIFE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_nitrogengroundwaterhalflife);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Rainfall Nitrate Concentration, ppm:"));
	edit_rainfallnitrateconcentration = new float_edit (this, EDIT_RAINFALLNITRATECONCENTRATION, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_rainfallnitrateconcentration);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Biological Oxidation Rate Constant, NH3-NO2:"));
	edit_nh3no2biologicaloxidationrateconstant = new float_edit (this, EDIT_NH3NO2BIOLOGICALOXIDATIONRATECONSTANT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_nh3no2biologicaloxidationrateconstant);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "NO2-NO3:"));
	edit_no2no3biologicaloxidationrateconstant = new float_edit (this, EDIT_NO2NO3BIOLOGICALOXIDATIONRATECONSTANT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_no2no3biologicaloxidationrateconstant);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "N-NH4 Hydrolysis Rate Constant:"));
	edit_nnh4hydrolysisrateconstant = new float_edit (this, EDIT_NNH4HYDROLYSISRATECONSTANT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_nnh4hydrolysisrateconstant);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Phosphorus Mineralization Rate Constant:"));
	edit_phosphorusmineralizationrateconstant = new float_edit (this, EDIT_PHOSPHORUSMINERALIZATIONRATECONSTANT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_phosphorusmineralizationrateconstant);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Daily Residue Decay Minimum:"));
	edit_dailyresiduedecayminimum = new float_edit (this, EDIT_DAILYRESIDUEDECAYMINIMUM, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_dailyresiduedecayminimum);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "C Factor Calculation:"));
		wxArrayString items;
		items.Add ("0 - Use CMIN");
		items.Add ("1 - Use RUSLE");
		combo_cfactorcalculation = new wxComboBox (this, COMBO_CFACTORCALCULATION, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
		row->Add (combo_cfactorcalculation);
		stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	}

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Residue Cover Factor:"));
	edit_residuecoverfactor = new float_edit (this, EDIT_RESIDUECOVERFACTOR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_residuecoverfactor);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Flow Critical Velocity:"));
	edit_flowcriticalvelocity = new float_edit (this, EDIT_FLOWCRITICALVELOCITY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_flowcriticalvelocity);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "Carbon Function:"));
		wxArrayString items;
		items.Add ("0 - Original");
		items.Add ("1 - Armen's");
		items.Add ("2 - Zhang's");
		combo_carbonfunction = new wxComboBox (this, COMBO_CARBONFUNCTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN);
		row->Add (combo_carbonfunction);
		stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	}

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Reservoir Sediment Settling Coefficient:"));
	edit_reservoirsedimentsettlingcoefficient = new float_edit (this, EDIT_RESERVOIRSEDIMENTSETTLINGCOEFFICIENT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_reservoirsedimentsettlingcoefficient);
	stack->Add (row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	stack->Add (edit_help);
	SetSizerAndFit(stack);
}

BEGIN_EVENT_TABLE(page_basin_6, wxPanel)
END_EVENT_TABLE()

bool page_basin_6::TransferDataToWindow ()

{
	edit_anionporosityexclusion->Set (control->AnionPorosityExclusion);
	edit_channelorganicn->Set (control->ChannelOrganicN);
	edit_channelorganicp->Set (control->ChannelOrganicP);
	edit_nitrogengroundwaterhalflife->Set (control->NitrogenGroundwaterHalfLife);
	edit_rainfallnitrateconcentration->Set (control->RainfallNitrateConcentration);
	edit_nh3no2biologicaloxidationrateconstant->Set (control->NH3NO2BiologicalOxidation);
	edit_no2no3biologicaloxidationrateconstant->Set (control->NO2NO3BiologicalOxidation);
	edit_nnh4hydrolysisrateconstant->Set (control->NNH4HydrolysisRateConstant);
	edit_phosphorusmineralizationrateconstant->Set (control->PhosphorusMineralizationRate);
	edit_dailyresiduedecayminimum->Set (control->DailyResidueDecayMinimum);
	combo_cfactorcalculation->SetSelection (control->CFactorCalculation);
	edit_residuecoverfactor->Set (control->ResidueCoverFactor);
	edit_flowcriticalvelocity->Set (control->FlowCriticalVelocity);
	combo_carbonfunction->SetSelection (control->CarbonFunction);
	edit_reservoirsedimentsettlingcoefficient->Set (control->ReservoirSedimentSettling);

	return true;
}

bool page_basin_6::TransferDataFromWindow ()

{

	control->AnionPorosityExclusion = edit_anionporosityexclusion->Get ();
	control->ChannelOrganicN = edit_channelorganicn->Get ();
	control->ChannelOrganicP = edit_channelorganicp->Get ();
	control->NitrogenGroundwaterHalfLife = edit_nitrogengroundwaterhalflife->Get ();
	control->RainfallNitrateConcentration = edit_rainfallnitrateconcentration->Get ();
	control->NH3NO2BiologicalOxidation = edit_nh3no2biologicaloxidationrateconstant->Get ();
	control->NO2NO3BiologicalOxidation = edit_no2no3biologicaloxidationrateconstant->Get ();
	control->NNH4HydrolysisRateConstant = edit_nnh4hydrolysisrateconstant->Get ();
	control->PhosphorusMineralizationRate = edit_phosphorusmineralizationrateconstant->Get ();
	control->DailyResidueDecayMinimum = edit_dailyresiduedecayminimum->Get ();
	control->CFactorCalculation = combo_cfactorcalculation->GetSelection ();
	control->ResidueCoverFactor = edit_residuecoverfactor->Get ();
	control->FlowCriticalVelocity = edit_flowcriticalvelocity->Get ();
	control->CarbonFunction = combo_carbonfunction->GetSelection ();
	control->ReservoirSedimentSettling = edit_reservoirsedimentsettlingcoefficient->Get ();
	return true;
}

