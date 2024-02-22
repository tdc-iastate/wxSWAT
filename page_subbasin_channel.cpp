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
#include "../util/device_coordinate.h"

#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
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
#include "page_subbasin_channel.h"

page_subbasin_channel::page_subbasin_channel
	(wxWindow *parent,
	const int id,
	SubBasin *pcontrol)
	: wxPanel (parent, id)

{
	wxBoxSizer *stack, *row;

	basin = pcontrol;

	stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Average width, m:")), 0, wxALL, 5);
	edit_channelaveragewidth = new float_edit (this, EDIT_CHANNELAVERAGEWIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelaveragewidth, 0, wxALL, 5);
	row->AddStretchSpacer();
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Depth, m:")), 0, wxALL, 5);
	edit_channeldepth = new float_edit (this, EDIT_CHANNELDEPTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channeldepth, 0, wxALL, 5);
	row->AddStretchSpacer ();
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Length, km:")), 0, wxALL, 5);
	edit_channellength = new float_edit (this, EDIT_CHANNELLENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channellength, 0, wxALL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Average slope:")));
	row->AddStretchSpacer ();
	edit_channelaverageslope = new float_edit (this, EDIT_CHANNELAVERAGESLOPE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelaverageslope);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Mannings n coefficient:")));
	row->AddStretchSpacer ();
	edit_channelmanningsn = new float_edit (this, EDIT_CHANNELMANNINGSN, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelmanningsn);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Hydraulic conductivity:")));
	row->AddStretchSpacer ();
	edit_channelhydraulicconductivity = new float_edit (this, EDIT_CHANNELHYDRAULICCONDUCTIVITY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelhydraulicconductivity);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Erodibility factor:")));
	row->AddStretchSpacer ();
	edit_channelerodibilityfactor = new float_edit (this, EDIT_CHANNELERODIBILITYFACTOR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelerodibilityfactor);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Cover factor:")));
	row->AddStretchSpacer ();
	edit_channelcoverfactor = new float_edit (this, EDIT_CHANNELCOVERFACTOR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelcoverfactor);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Width - depth ratio:")));
	row->AddStretchSpacer ();
	edit_channelwidthdepthratio = new float_edit (this, EDIT_CHANNELWIDTHDEPTHRATIO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelwidthdepthratio);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Baseflow alpha factor, days:")));
	row->AddStretchSpacer ();
	edit_baseflowalphafactor = new float_edit (this, EDIT_BASEFLOWALPHAFACTOR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_baseflowalphafactor);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);
	
	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Sediment reentrain parameter:")));
	row->AddStretchSpacer ();
	edit_sedimentreentrainparameter = new float_edit (this, EDIT_SEDIMENTREENTRAINPARAMETER, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_sedimentreentrainparameter);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);
	
	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Exponent:")));
	row->AddStretchSpacer ();
	edit_sedimentreentrainexponent = new float_edit (this, EDIT_SEDIMENTREENTRAINEXPONENT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_sedimentreentrainexponent);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	check_irrigation_canal = new wxCheckBox (this, CHECK_IRRIGATION_CANAL, wxT("Channel is irrigation canal"));
	stack->Add (check_irrigation_canal, 0, wxALL, 5);
	
	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Channel Organic N:")));
	row->AddStretchSpacer ();
	edit_channelorganicn_rte = new float_edit (this, EDIT_CHANNELORGANICN_RTE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelorganicn_rte);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);
	
	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Channel Organic P:")));
	row->AddStretchSpacer ();
	edit_channelorganicp_rte = new float_edit (this, EDIT_CHANNELORGANICP_RTE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_channelorganicp_rte);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);
	
	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxSize (400, 200));
	stack->Add (edit_help, 0, wxALL, 5);

	SetSizerAndFit(stack);

	edit_channelaveragewidth->Set (basin->ChannelAverageWidth);
	edit_channeldepth->Set (basin->ChannelDepth);
	edit_channellength->Set (basin->ChannelLength);
	edit_channelaverageslope->Set (basin->ChannelAverageSlope);
	edit_channelmanningsn->Set (basin->ChannelManningsN);
	edit_channelhydraulicconductivity->Set (basin->ChannelHydraulicConductivity);
	edit_channelerodibilityfactor->Set (basin->ChannelErodibilityFactor);
	edit_channelcoverfactor->Set (basin->ChannelCoverFactor);
	edit_channelwidthdepthratio->Set (basin->ChannelWidthDepthRatio);
	edit_baseflowalphafactor->Set (basin->BaseflowAlphaFactor);
	edit_sedimentreentrainparameter->Set (basin->SedimentReentrainParameter);
	edit_sedimentreentrainexponent->Set (basin->SedimentReentrainExponent);
	check_irrigation_canal->SetValue (basin->IrrigationCanal == VARIANT_TRUE);
	edit_channelorganicn_rte->Set (basin->ChannelOrganicN);
	edit_channelorganicp_rte->Set (basin->ChannelOrganicP);

	// edit_help->SetValue (basin->Help.get_text ());


	SetSizerAndFit(stack);

}

BEGIN_EVENT_TABLE(page_subbasin_channel, wxPanel)
END_EVENT_TABLE()

void page_subbasin_channel::set_controls ()

{
	/*
    if (cfg->main_file_definition.path.get_length () > 0)
        edit_data_file->WriteText (cfg->main_file_definition.path.get_text_ascii ());
    else
        edit_data_file->Clear ();

    if (cfg->cfgpath.get_length () > 0)
        static_config_file->SetLabel (cfg->cfgpath.get_text_ascii ());
    else
        static_config_file->SetLabel ("");

    if (cfg->main_file_definition.is_delimited) {
        radio_fixed->SetValue (true);
        radio_variable->SetValue (false);
    }
    else {
        radio_fixed->SetValue (false);
        radio_variable->SetValue (true);
    }
	*/

}

bool page_subbasin_channel::read_controls ()

{
	basin->ChannelAverageWidth = edit_channelaveragewidth->Get ();
	basin->ChannelDepth = edit_channeldepth->Get ();
	basin->ChannelLength = edit_channellength->Get ();
	basin->ChannelAverageSlope = edit_channelaverageslope->Get ();
	basin->ChannelManningsN = edit_channelmanningsn->Get ();
	basin->ChannelHydraulicConductivity = edit_channelhydraulicconductivity->Get ();
	basin->ChannelErodibilityFactor = edit_channelerodibilityfactor->Get ();
	basin->ChannelCoverFactor = edit_channelcoverfactor->Get ();
	basin->ChannelWidthDepthRatio = edit_channelwidthdepthratio->Get ();
	basin->BaseflowAlphaFactor = edit_baseflowalphafactor->Get ();
	basin->SedimentReentrainParameter = edit_sedimentreentrainparameter->Get ();
	basin->SedimentReentrainExponent = edit_sedimentreentrainexponent->Get ();
	basin->IrrigationCanal = check_irrigation_canal->GetValue ();
	basin->ChannelOrganicN = edit_channelorganicn_rte->Get ();
	basin->ChannelOrganicP = edit_channelorganicp_rte->Get ();

	return true;
}

