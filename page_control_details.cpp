#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/custom.h"
#include "../util/device_coordinate.h"
// #include "bitmaps/wizard_control_record.xpm"
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
#include "page_control_details.h"
#include "gridwidget.h"

BEGIN_EVENT_TABLE(page_control_details, wxPanel)
END_EVENT_TABLE()

page_control_details::page_control_details
	(wxWindow *parent,
	class SWATControlRecord *p_control)
	: wxPanel (parent, wxID_ANY)

{
	control = p_control;

	wxString items_input_code [2];
	items_input_code [0] = "1 Measured data read for each subbasin";
	items_input_code [1] = "2 Data generated for each subbasin";

	wxBoxSizer *stack, *row;
	stack = new wxBoxSizer( wxVERTICAL );

	stack->AddSpacer (10);

	// Row 1
	row = new wxBoxSizer( wxHORIZONTAL );
	check_model_instream_nutrients = new wxCheckBox (this, CHECK_MODEL_INSTREAM_NUTRIENTS, wxT("Model instream nutrients"));
	row->Add (check_model_instream_nutrients);
	stack->Add (row, 1, wxEXPAND | wxALL);

	row = new wxBoxSizer( wxHORIZONTAL );
	check_model_lake_water_quality = new wxCheckBox (this, CHECK_MODEL_LAKE_WATER_QUALITY, wxT("Model lake water quality"));
	row->Add (check_model_lake_water_quality);
	stack->Add (row, 1, wxEXPAND | wxALL);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Special project flag:")));
	wxString items [3];
	items [0] = "0 not a special project";
	items [1] = "1 HUMUS project";
	items [2] = "2 Missouri River climate change project";
	combo_special_project_flag = new wxComboBox (this, COMBO_SPECIAL_PROJECT_FLAG, wxEmptyString, wxDefaultPosition, wxSize (169, 90), 3, items, wxCB_DROPDOWN);
	row->Add (combo_special_project_flag);
	stack->Add (row, 1, wxEXPAND | wxALL);

	stack->AddSpacer (25);

	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	stack->Add (edit_help, 1, wxEXPAND | wxALL);

	SetSizerAndFit(stack);

	check_model_instream_nutrients->SetValue (control->ModelInstreamNutrients == VARIANT_TRUE);
	check_model_lake_water_quality->SetValue (control->ModelLakeWaterQuality == VARIANT_TRUE);
	combo_special_project_flag->SetSelection (control->SpecialProjectFlag - 1);
}

bool page_control_details::read_controls ()

{

	control->ModelInstreamNutrients = check_model_instream_nutrients->GetValue () ? VARIANT_TRUE : VARIANT_FALSE;
	control->ModelLakeWaterQuality = check_model_lake_water_quality->GetValue () ? VARIANT_TRUE : VARIANT_FALSE;
	control->SpecialProjectFlag = combo_special_project_flag->GetSelection () + 1;

	return true;
}

