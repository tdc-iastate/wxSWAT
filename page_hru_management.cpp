#include "../util/utility_afx.h"
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../util/custom.h"
#include "../util/interface_window.h"
#include "../util/ARMS_Declarations.h"
#include "../util/device_coordinate.h"

#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"

#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATSoilLayers.h"
#include "../i_swat_database/SWATSoils.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATUrban.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
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
#include "page_hru_management.h"
#include "../weather/averager_weighted.h"
#include "../i_epic_database/EPICDeclarations.h"

page_hru_management::page_hru_management
	(wxWindow *parent,
	const int id,
	HRU_record *phru,
	SubBasin *pbasin,
	SWATControlRecord *pcontrol,
	swat_controller *pcontroller)
	: wxPanel (parent, id)

{
	wxBoxSizer *stack, *row;

    m_prev = NULL;
    m_next = NULL;

	hru = phru;
	subbasin = pbasin;
	control = pcontrol;
	controller = pcontroller;

	stack = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *big_row = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer *left_stack = new wxBoxSizer( wxVERTICAL );
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Management code:"));
	row->AddStretchSpacer ();
	edit_management_code = new long_integer_edit (this, EDIT_MANAGEMENT_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_management_code);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	check_landcover = new wxCheckBox (this, CHECK_LANDCOVER, wxT("Land cover at start of run"));
	left_stack->Add (check_landcover, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Land cover ID:"));
	row->AddStretchSpacer ();
	edit_land_cover_id = new long_integer_edit (this, EDIT_LAND_COVER_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_land_cover_id);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Initial leaf area index:"));
	row->AddStretchSpacer ();
	edit_initial_leaf_area_index = new float_edit (this, EDIT_INITIAL_LEAF_AREA_INDEX, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_initial_leaf_area_index);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Initial dry weight biomass:"));
	row->AddStretchSpacer ();
	edit_initial_dry_weight_biomass = new float_edit (this, EDIT_INITIAL_DRY_WEIGHT_BIOMASS, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_initial_dry_weight_biomass);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Plant heat units:"));
	row->AddStretchSpacer ();
	edit_plant_heat_units = new float_edit (this, EDIT_PLANT_HEAT_UNITS, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_plant_heat_units);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Grazing biomass minimum:"));
	row->AddStretchSpacer ();
	edit_grazingbiomassminimum = new float_edit (this, EDIT_GRAZINGBIOMASSMINIMUM, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_grazingbiomassminimum);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Biological mixing efficiency:"));
	row->AddStretchSpacer ();
	edit_biologicalmixingefficiency = new float_edit (this, EDIT_BIOLOGICALMIXINGEFFICIENCY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_biologicalmixingefficiency);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Runoff curve number:"));
	row->AddStretchSpacer ();
	edit_runoffcurvenumber = new float_edit (this, EDIT_RUNOFFCURVENUMBER, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_runoffcurvenumber);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Soil temperature moisture factor:"));
	row->AddStretchSpacer ();
	edit_cf = new float_edit (this, EDIT_CF, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cf);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Humification factor:"));
	row->AddStretchSpacer ();
	edit_cfh = new float_edit (this, EDIT_CFH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cfh);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Carbon Decomposition factor:"));
	row->AddStretchSpacer ();
	edit_cfdec = new float_edit (this, EDIT_CFDEC, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cfdec);
	left_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	big_row->Add (left_stack);

	wxBoxSizer *right_stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Organic Humus Mineralization:"));
	row->AddStretchSpacer ();
	edit_cmn = new float_edit (this, EDIT_CMN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cmn);
	right_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Denitrification Rate Factor:"));
	row->AddStretchSpacer ();
	edit_cdn = new float_edit (this, EDIT_CDN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_cdn);
	right_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Nitrate Percolation Ratio:"));
	row->AddStretchSpacer ();
	edit_nperco = new float_edit (this, EDIT_NPERCO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_nperco);
	right_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Soluble Phosphorus Ratio:"));
	row->AddStretchSpacer ();
	edit_phoskd = new float_edit (this, EDIT_PHOSKD, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_phoskd);
	right_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Phosphorus Sorption Coefficient:"));
	row->AddStretchSpacer ();
	edit_psp = new float_edit (this, EDIT_PSP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_psp);
	right_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Denitrification Threshold Water:"));
	row->AddStretchSpacer ();
	edit_sdnco = new float_edit (this, EDIT_SDNCO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_sdnco);
	right_stack->Add (row, 0, wxEXPAND | wxALL, 5);

	big_row->Add (right_stack);
	stack->Add (big_row);

	edit_help = new wxTextCtrl(this, EDIT_HELP_MANAGEMENT, wxEmptyString, wxDefaultPosition, wxSize(550, 100), wxTE_MULTILINE | wxTE_WORDWRAP);
	stack->Add (edit_help);

	SetSizerAndFit(stack);

}

BEGIN_EVENT_TABLE(page_hru_management, wxNotebookPage)
END_EVENT_TABLE()


bool page_hru_management::TransferDataToWindow ()

{
	edit_management_code->Set (hru->ManagementCode);
	check_landcover->SetValue (hru->LandCover == VARIANT_TRUE);
	edit_land_cover_id->Set (hru->LandCoverID);
	edit_initial_leaf_area_index->Set (hru->InitialLeafAreaIndex);
	edit_initial_dry_weight_biomass->Set (hru->InitialDryWeightBiomass);
	edit_plant_heat_units->Set (hru->PlantHeatUnits);
	edit_grazingbiomassminimum->Set (hru->GrazingBiomassMinimum);
	edit_biologicalmixingefficiency->Set (hru->BiologicalMixingEfficiency);
	edit_runoffcurvenumber->Set (hru->RunoffCurveNumber);
	edit_cf->Set (hru->SoilTemperatureMoisture);
	edit_cfh->Set (hru->HumificationFactor);
	edit_cfdec->Set (hru->CarbonDecompositionFactor);
	edit_cmn->Set (hru->OrganicHumusMineralization);
	edit_cdn->Set (hru->DenitrificationRateFactor);
	edit_nperco->Set (hru->NitratePercolationRatio);
	edit_phoskd->Set (hru->SolublePhosphorusRatio);
	edit_psp->Set (hru->PhosphorusSorptionCoefficient);
	edit_sdnco->Set (hru->DenitrificationThresholdWater);
	return true;
}

bool page_hru_management::TransferDataFromWindow ()

{
	hru->ManagementCode = edit_management_code->Get ();
	hru->LandCover = check_landcover->GetValue () ? VARIANT_TRUE : VARIANT_FALSE;
	hru->LandCoverID = edit_land_cover_id->Get ();
	hru->InitialLeafAreaIndex = edit_initial_leaf_area_index->Get ();
	hru->InitialDryWeightBiomass = edit_initial_dry_weight_biomass->Get ();
	hru->PlantHeatUnits = edit_plant_heat_units->Get ();
	hru->GrazingBiomassMinimum = edit_grazingbiomassminimum->Get ();
	hru->BiologicalMixingEfficiency = edit_biologicalmixingefficiency->Get ();
	hru->RunoffCurveNumber = edit_runoffcurvenumber->Get ();
	hru->SoilTemperatureMoisture = edit_cf->Get ();
	hru->HumificationFactor = edit_cfh->Get ();
	hru->CarbonDecompositionFactor = edit_cfdec->Get ();
	hru->OrganicHumusMineralization = edit_cmn->Get ();
	hru->DenitrificationRateFactor = edit_cdn->Get ();
	hru->NitratePercolationRatio = edit_nperco->Get ();
	hru->SolublePhosphorusRatio = edit_phoskd->Get ();
	hru->PhosphorusSorptionCoefficient = edit_psp->Get ();
	hru->DenitrificationThresholdWater = edit_sdnco->Get ();

	return true;
}
