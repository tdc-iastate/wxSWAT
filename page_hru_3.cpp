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
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
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
#include "page_hru_3.h"
#include "../weather/averager_weighted.h"
#include "../i_epic_database/EPICDeclarations.h"

page_hru_3::page_hru_3
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

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Depth to subsurface drain, mm:"));
	row->AddSpacer(10);
	edit_depth_to_subsurface_drain = new float_edit (this, EDIT_DEPTH_TO_SUBSURFACE_DRAIN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_depth_to_subsurface_drain);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Time to drain soil to Field Capacity:"));
	row->AddSpacer(10);
	edit_timetodrainsoiltofieldcapacity = new float_edit (this, EDIT_TIMETODRAINSOILTOFIELDCAPACITY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_timetodrainsoiltofieldcapacity);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Drain tile lag time:"));
	row->AddSpacer(10);
	edit_draintilelagtime = new float_edit (this, EDIT_DRAINTILELAGTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_draintilelagtime);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Drain tile effective radius, mm:"));
	row->AddSpacer(10);
	edit_re = new float_edit (this, EDIT_RE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_re);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Distance between, mm:"));
	row->AddSpacer(10);
	edit_sdrain = new float_edit (this, EDIT_SDRAIN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_sdrain);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Drainage coefficient, mm/day:"));
	row->AddSpacer(10);
	edit_dc = new float_edit (this, EDIT_DC, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_dc);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Potassium factor:"));
	row->AddSpacer(10);
	edit_latksatf = new float_edit (this, EDIT_LATKSATF, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_latksatf);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Pump capacity:"));
	row->AddSpacer(10);
	edit_pc = new float_edit (this, EDIT_PC, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_pc);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "USLE P factor:"));
	row->AddSpacer(10);
	edit_uslepfactor = new float_edit (this, EDIT_USLEPFACTOR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_uslepfactor);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Snowfall temperature, deg. C:"));
	row->AddSpacer(10);
	edit_sftmp = new float_edit (this, EDIT_SFTMP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_sftmp);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Snow melt temperature, deg. C:"));
	row->AddSpacer(10);
	edit_smtmp = new float_edit (this, EDIT_SMTMP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_smtmp);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Snow melt factor, June 21:"));
	row->AddSpacer(10);
	edit_smfmx = new float_edit (this, EDIT_SMFMX, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_smfmx);
	row->AddSpacer(15);
	row->Add (new wxStaticText (this, wxID_ANY, "December 21:"));
	row->AddSpacer(10);
	edit_smfmn = new float_edit (this, EDIT_SMFMN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_smfmn);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Impervious layer depth, mm:"));
	row->AddSpacer(10);
	edit_imperviouslayerdepth = new float_edit (this, EDIT_IMPERVIOUSLAYERDEPTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_imperviouslayerdepth);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Fraction of area draining into floodplain:"));
	row->AddSpacer(10);
	edit_fld_fr = new float_edit (this, EDIT_FLD_FR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_fld_fr);
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Fraction of area draining to riparian area:"));
	row->AddSpacer (10);
	edit_rip_fr = new float_edit (this, EDIT_RIP_FR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_rip_fr);
	stack->Add(row, 0, wxALL, 5);

	edit_help = new wxTextCtrl (this, EDIT_HELP_3, wxEmptyString, wxDefaultPosition, wxSize(550, 100), wxTE_MULTILINE | wxTE_WORDWRAP);
	stack->Add (edit_help);

	SetSizerAndFit(stack);
}

BEGIN_EVENT_TABLE(page_hru_3, wxNotebookPage)
END_EVENT_TABLE()


bool page_hru_3::TransferDataToWindow ()
{
	edit_depth_to_subsurface_drain->Set (hru->DepthtoSubsurfaceDrain);
	edit_timetodrainsoiltofieldcapacity->Set (hru->SoilFieldCapacityDrainTime);
	edit_draintilelagtime->Set (hru->DrainTileLagTime);
	edit_re->Set (hru->TileEffectiveRadius);
	edit_sdrain->Set (hru->TileDistance);
	edit_dc->Set (hru->TileDrainageCoefficient);
	edit_latksatf->Set (hru->TilePotassiumFactor);
	edit_pc->Set (hru->TilePumpCapacity);
	edit_uslepfactor->Set (hru->USLEPFactor);
	edit_sftmp->Set (hru->SnowfallTemperature);
	edit_smtmp->Set (hru->SnowMeltTemperature);
	edit_smfmx->Set (hru->SnowMeltRateJune21);
	edit_smfmn->Set (hru->SnowMeltRateDecember21);
	edit_imperviouslayerdepth->Set (hru->ImperviousLayerDepth);
	edit_fld_fr->Set (hru->FloodplainDrainingArea);
	edit_rip_fr->Set (hru->RiparianDrainingArea);
	return true;
}

bool page_hru_3::TransferDataFromWindow ()

{
	hru->DepthtoSubsurfaceDrain = edit_depth_to_subsurface_drain->Get ();
	hru->SoilFieldCapacityDrainTime = edit_timetodrainsoiltofieldcapacity->Get ();
	hru->DrainTileLagTime = edit_draintilelagtime->Get ();
	hru->TileEffectiveRadius = edit_re->Get ();
	hru->TileDistance = edit_sdrain->Get ();
	hru->TileDrainageCoefficient = edit_dc->Get ();
	hru->TilePotassiumFactor = edit_latksatf->Get ();
	hru->TilePumpCapacity = edit_pc->Get ();
	hru->USLEPFactor = edit_uslepfactor->Get ();
	hru->SnowfallTemperature = edit_sftmp->Get ();
	hru->SnowMeltTemperature = edit_smtmp->Get ();
	hru->SnowMeltRateJune21 = edit_smfmx->Get ();
	hru->SnowMeltRateDecember21 = edit_smfmn->Get ();
	hru->ImperviousLayerDepth = edit_imperviouslayerdepth->Get ();
	hru->FloodplainDrainingArea = edit_fld_fr->Get ();
	hru->RiparianDrainingArea = edit_rip_fr->Get ();

	return true;
}
