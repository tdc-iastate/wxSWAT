#include "../util/utility_afx.h"
#include <vector>
#include <set>
#include <map>
#include <wx/stattext.h>
#include <wx/window.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/filedlg.h>
#include <wx/clipbrd.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/app.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/listbox_files.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../i_swat_database/SWATDeclarations.h"
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
#include "../odbc/odbc_field_set.h"

#include "dialog_control_record.h"
// #include "bitmaps/wizard_control_record.xpm"
#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATSoils.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATUrban.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATOutputHRUAnnual.h"
#include "../i_swat_database/SWATOutputHRUMonthly.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
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

dialog_control_record::dialog_control_record
	(wxWindow *pParent,
	SWATControlRecord *p_control,
	swat_controller *controller)
	: wxDialog (pParent, -1, "Control Record") // , wxDefaultPosition, wxSize (400, 600))
{
	control = p_control;

 	wxBoxSizer *stack = new wxBoxSizer( wxVERTICAL );

	wxPanel *panel = new wxPanel(this, wxID_ANY);

    notebook = new notebook_control_record (panel, control, controller, wxSize (600, 550));

	stack->Add (panel, 0, wxEXPAND);

	stack->Add (CreateButtonSizer (wxOK|wxCANCEL), 0, wxEXPAND);

    SetSizerAndFit (stack);
    stack->SetSizeHints( this );
}

BEGIN_EVENT_TABLE(dialog_control_record, wxDialog)
END_EVENT_TABLE()

void dialog_control_record::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {
	}
	wxDialog::EndModal (retCode);
}

