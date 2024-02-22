#include "../util/utility_afx.h"
#include <chrono>
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
#include <wx/bookctrl.h>
#include <wx/propdlg.h>
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/listbox_files.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../util/custom.h"
#include "../util/device_coordinate.h"
#include "../i_swat_database/SWATDeclarations.h"
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
#include "../odbc/odbc_field_set.h"

#include "propsheet_routing.h"
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
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
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
#include "page_routing_node.h"
#include "page_routing_output.h"

BEGIN_EVENT_TABLE(propsheet_routing, wxPropertySheetDialog)
EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, propsheet_routing::OnPageChanged)
END_EVENT_TABLE()

propsheet_routing::propsheet_routing
	(wxWindow *pParent,
	swat_routing_node *node,
	SWATControlRecord *control,
	swat_controller *controller)
	: wxPropertySheetDialog()
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP | wxWS_EX_VALIDATE_RECURSIVELY);


	Create(pParent, wxID_ANY, _("Routing"), wxDefaultPosition,
	wxDefaultSize, // wxSize (850, 650),
	wxDEFAULT_DIALOG_STYLE); // | (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxRESIZE_BORDER));

	CreateButtons(wxOK | wxCANCEL | (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP));

	wxBookCtrlBase* notebook = GetBookCtrl();

	page1 = new page_routing_node(notebook, wxID_ANY, node, control, controller);
	page2 = new page_routing_output(notebook, wxID_ANY, node, control);

	notebook->AddPage(page1, wxT("Node"));
	notebook->AddPage(page2, wxT("Output"));

	LayoutDialog();
}

void propsheet_routing::OnPageChanged(wxNotebookEvent& eb)

{
	size_t old_page_number = (size_t)eb.GetOldSelection();
	if ((old_page_number >= 0)
	&& (old_page_number < GetBookCtrl()->GetPageCount())) {
		// First call occurs as dialog opens, old_page_number is a very large integer
		wxWindow* old_page = GetBookCtrl()->GetPage(old_page_number);
		old_page->TransferDataFromWindow();
	}

	wxWindow* new_page = GetBookCtrl()->GetPage(eb.GetSelection());
	new_page->TransferDataToWindow();
}
