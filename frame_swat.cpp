
#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <thread>
#include <future>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/propdlg.h>
#include <wx/filename.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/dialog.h>
#include <wx/log.h>
#include <wx/app.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/stopwatch.h>
#include <wx/sizer.h>
#include <sys/timeb.h>
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/Timestamp.h"
#include "../util/interface_window.h"
#include "../util/listbox_files.h"
#include "../util/interface_window_wx.h"
#include "../util/dialog_error_list.h"
#include "../util/dialog_prompt.h"
#include "../util/device_coordinate.h"
#include "../util/list_field_help.h"
#include "../util/custom.h"
#include "../util/bounding_cube.h"
#include "../map/color_set.h"
#include "../map/dynamic_map.h"
// wx setup.h must appear before odbc_inst.h or error with HAVE_UNISTD_H
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
#include "../odbc/odbc_field_set.h"
#include "../util/ARMS_Declarations.h"
#include "../util/nri_point.h"
#include "../i_epic_database/EPICDeclarations.h"
#include "../subbasin_effect/thread_manager.h"
#include "../subbasin_effect/thread_work_swat.h"
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
#include "../i_swat_database/SWATOutputHRUDaily.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
#include "../i_swat_database/SWATOutputSubBasinDaily.h"
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
#include "../i_swat_database/SWATReservoir.h"
#include "../i_swat_database/SWATWeirStage.h"
#include "../i_swat_database/SWATDetentionPond.h"
#include "../i_swat_database/SWATWetPond.h"
#include "../i_swat_database/SWATRetentionIrrigationBasin.h"
#include "../i_swat_database/SWATSedimentationFiltrationBasin.h"
#include "../i_swat_database/SWATPond.h"
#include "../i_swat_database/SWATWaterUse.h"
#include "../i_swat_database/SWATStreamWaterQuality.h"
#include "../i_swat_database/SWATLakeWaterQuality.h"
#include "../i_swat_database/SWATWatershedWaterQuality.h"
#include "../i_swat_database/SWATOutputHSL.h"
#include "../i_swat_database/swat_controller.h"
#include "../i_swat_database/SWATSepticWaterQuality.h"
#include "../i_swat_database/SWATSeptic.h"
#include "../i_epic_database/output_variable_description.h"
#include "../i_swat_database/output_variable_description_swat.h"
#include "../i_swat_database/output_yield_tracker.h"
#include "../i_swat_database/FortranCommonData.h"
#include "list_control_records.h"
#include "list_field_help_swat.h"
#include "propsheet_control_record.h"
#include "wxSWAT.h"
#include "frame_swat.h"
#include "swat_tree_control.h"
#include "../map/map_scale.h"
#include "../map/dbase.h"
#include "../map/shapefile.h"
#include "../map/dialog_import_shapefile.h"
#include "../util/RecentlyOpened.h"
#include "../util/RecentlyOpenedDatabases.h"
#include "../util/RecentlyOpenedShapefiles.h"
#include "dialog_configure.h"
#include "dialog_open_database.h"
#include "panel_routing_tree.h"
#include "dialog_import_cio.h"
#include "bitmaps/edit_control.xpm"
#include "bitmaps/run_swat.xpm"
#ifdef _LESUEUR_RUN
#include "../wxGeneticiSWAT/NitrateNetworkModel.h"
#endif

#ifdef _MRB_DOUBLE_RUN
#include "../wxGeneticiSWAT/PointSourceLookup.h"
#include "../wxGeneticiSWAT/RavineAreas.h"
#include "MRB_Double_Run.h"
#include "dialog_paste_genome.h"
#endif

BEGIN_EVENT_TABLE(frame_swat, wxFrame)
    EVT_MENU(wxID_EXIT, frame_swat::OnExit)
    EVT_MENU(ID_About, frame_swat::OnAbout)
    EVT_MENU(wxID_OPEN, frame_swat::OnFileOpen)
    EVT_MENU(wxID_NEW, frame_swat::OnFileNew)
	EVT_MENU_RANGE( ID_FILE_RECENT_DB, ID_FILE_RECENT_DB + 25, frame_swat::OnOpenRecent )
	EVT_MENU(wxID_REPORT_POLLUTANTS, frame_swat::OnReportPollutants)
	EVT_MENU(wxID_REPORT_TILLAGE, frame_swat::OnReportTillage)
	EVT_MENU(wxID_REPORT_FERTILIZER, frame_swat::OnReportFertilizer)
	EVT_MENU(wxID_REPORT_FLOW, frame_swat::OnReportFlow)
	EVT_MENU(wxID_IMPORT_CIO, frame_swat::OnImportCIO)
	EVT_MENU(wxID_INSERT_POINT_SOURCE, frame_swat::OnInsertPointSource)
	EVT_MENU(wxID_READ_OUTPUT_HRU, frame_swat::OnReadOutputHRU)
#ifdef _MRB_DOUBLE_RUN
	EVT_MENU(wxID_MRB_DOUBLE_RUN, frame_swat::OnMRBDoubleRun)
#endif
END_EVENT_TABLE()

frame_swat::frame_swat
	(const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	app_wxswat *p_app)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	app = p_app;
	create_menus ();
	create_toolbar ();

    list_control = NULL;
	help_list = NULL;
    // m_numListItems = 10;

	recent_shapefiles = new RecentlyOpenedShapefiles;
	recent_shapefiles->load_configuration (app->AppName);

	stack = new wxBoxSizer( wxHORIZONTAL);

	m_panel = new wxPanel(this, wxID_ANY);

	{
		// Log window on panel
		// m_logWindow = new wxTextCtrl(m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxSUNKEN_BORDER);
		// m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_logWindow));

	   list_control = new list_control_records (m_panel, &control_records, wxID_ANY, wxDefaultPosition, wxSize (300, 300), wxLC_LIST);

		// Tree control also on panel
		// tree = new swat_tree_control (m_panel, TreeTest_Ctrl, wxDefaultPosition, wxSize (300, 300), wxTR_DEFAULT_STYLE | wxTR_HAS_VARIABLE_ROW_HEIGHT | wxTR_EDIT_LABELS | wxSUNKEN_BORDER, &control);

		// list_subbasins = new wxListBox (m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, wxTR_DEFAULT_STYLE);
	}
	stack->Add (m_panel, 0, wxEXPAND);

	// panel_tree = new panel_routing_tree (this, wxID_ANY);
	// stack->Add (panel_tree);
	panel_tree = NULL;

	/*
	panel_control_record = new wxPanel (this, wxID_ANY);
	// notebook = new notebook_control_record (panel_control_record, &control, wxSize (700, 500));
	// panel_control_record->AddChild (notebook);
	stack->Add (panel_control_record, 0, wxEXPAND);
	*/

    // GetMenuBar()->Enable(TreeTest_SelectRoot, !(style & wxTR_HIDE_ROOT));

#ifdef __WXMSW__
    // this is useful to know specially when debugging :)
    // wxLogMessage("Your version of comctl32.dll is: %d", wxApp::GetComCtl32Version());
#endif

    CreateStatusBar();
    // SetStatusText( _("Welcome to wxWidgets!") );

	// RecreateList(wxLC_LIST | wxLC_SINGLE_SEL);
	// DoSize ();
    SetSizerAndFit (stack);
    stack->SetSizeHints( this );

	dynamic_string log;
	output_variables = new list_output_variable_descriptions_swat;
	output_variables->read_csv (app->filename_swat_output_variables, log);

	help_list = new list_field_help_swat;
	help_list->read (app->filename_swat_help.get_text_ascii (), log);

#ifdef _DEBUG
	test_make_crop_list (log);
#endif

	if (log.get_length () > 0) {
		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"frame constructor", L"", log);
		d.ShowWindowModal ();
	}

	db_odbc = NULL;
}

frame_swat::~frame_swat ()

{
    // delete wxLog::SetActiveTarget(m_logOld);

	if (help_list)
		delete help_list;

	if (output_variables) {
		delete output_variables;
		output_variables = NULL;
	}

	if (db_odbc) {
		db_odbc->close ();
		delete db_odbc;
	}
	// control.clear ();

	if (files_recently_opened) {
		files_recently_opened->save_configuration (app->AppName);
		delete files_recently_opened;
	}

	if (recent_shapefiles) {
		recent_shapefiles->save_configuration (app->AppName);
		delete recent_shapefiles;
	}

	std::vector <SWATControlRecord *>::iterator c;
	for (c = control_records.begin ();
	c != control_records.end ();
	++c)
		delete *c;
}

void frame_swat::create_menus ()

{
    wxMenu *fileMenu = new wxMenu;

	fileMenu->Append( ID_About, _("&About...") );
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_CONFIGURE, wxT("&Configure"), wxT("Set SWAT executable location"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_NEW);
    fileMenu->Append(wxID_OPEN);
    fileMenu->Append(wxID_CONNECT, wxT("&Connect to DB"), wxT("Open connection to ODBC database"));
    fileMenu->Append(wxID_CREATE_TABLES, wxT("Create &Tables"), wxT("Create empty tables in ODBC database"));
	files_recently_opened = new RecentlyOpenedDatabases;
	files_recently_opened->load_configuration (app->AppName);
	files_recently_opened->setup_menu (fileMenu, ID_FILE_RECENT_DB);
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXPORTINPUT, wxT("&Export Input"), wxT("Write input table data to files"));
	fileMenu->Append(wxID_IMPORT_CIO, wxT("&Import file.cio"), wxT("Read input table data from run folder"));
	fileMenu->Append(wxID_IMPORT, wxT("&Import"), wxT("Read input table data from csv or binary files"));
	fileMenu->Append(wxID_READ_OUTPUT_HRU, wxT("&Read Output.HRU"), wxT("Read Output.HRU"));
	fileMenu->Append(wxID_IMPORT_HISTORICAL, wxT("&Import Historical Weather"), wxT("Read historical PCP and TMP files"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit"), wxT("Exit program"));

    wxMenu *editMenu = new wxMenu;
    editMenu->Append(wxID_COPY);
    editMenu->Append(wxID_PASTE);
    editMenu->Append(wxID_EDIT_CONTROL_RECORD, wxT("&Control Record"), wxT("Edit control record"));

    editMenu->Append(wxID_REPORT_POLLUTANTS, wxT("&Pollutant Report"), wxT("Summarize N && P outputs"));
    editMenu->Append(wxID_REPORT_TILLAGE, wxT("&Tillage Report"), wxT("Show tillage effects by HRU"));
    editMenu->Append(wxID_REPORT_FERTILIZER, wxT("&Fertilizer Report"), wxT("Show fertilizer by HRU"));
    editMenu->Append(wxID_REPORT_FLOW, wxT("&Flow Report"), wxT("Show flow by reach"));

    wxMenu * const menuRouting = new wxMenu;
	menuRouting->Append(wxID_FIND_OUTLET, wxT("&Find Outlet Subbasin"), wxT("Determine which subbasin is closest to routing outlet."));
	menuRouting->Append(wxID_TRACE_HSL, wxT("T&race HSL"), wxT("Create a list of all HSL numbers from target to outlet."));
    menuRouting->Append(wxID_VIEW_ROUTING, wxT("&Tree"), wxT("View routing Tree"));
	menuRouting->Append(wxID_INSERT_POINT_SOURCE, wxT("Insert Point &Source"), wxT("Add a point source to routing"));
    menuRouting->Append(wxID_MAP_ROUTING, wxT("&Map"), wxT("View routing Map"));
	menuRouting->Append(wxID_ADD_UPSTREAM, wxT("&Sum upstream area"), wxT("Compute area upstream from a subbasin"));
	menuRouting->Append(wxID_ROUTING_LIST, wxT("&List"), wxT("Create text list of routing"));
	menuRouting->Append(wxID_SUBBASIN_DISTANCE_TO_OUTLET, wxT("&Subbasin Outlet Distance"), wxT("Create list of subbasins and distances to outlet"));
    editMenu->AppendSubMenu(menuRouting, "R&outing");

    wxMenu * const menuRecord = new wxMenu;
	menuRecord->Append(wxID_RECORD_PREVIOUS, wxT("&Previous"), wxT("Move to previous control record."));
	menuRecord->Append(wxID_RECORD_NEXT, wxT("&Next"), wxT("Move to next control record."));

	wxMenu *execMenu = new wxMenu;
    execMenu->Append(wxID_RUN_SWAT, wxT("&SWAT"), wxT("Run SWAT"));
    execMenu->Append(wxID_RUN_SWAT_ALL, wxT("&All"), wxT("Run All SWAT"));
#ifdef _MRB_DOUBLE_RUN
	execMenu->Append(wxID_MRB_DOUBLE_RUN, wxT("&MRB Double Run"), wxT("Run SWAT, compute new point sources, rerun SWAT"));
#endif

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(editMenu, wxT("&Edit"));
    menuBar->Append(menuRecord, wxT("&Record"));
    menuBar->Append(execMenu, wxT("E&xecute"));

	SetMenuBar(menuBar);

	Connect(wxID_EDIT_CONTROL_RECORD, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnEditControl);
	Connect(wxID_VIEW_ROUTING, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnViewRouting);
	Connect(wxID_MAP_ROUTING, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnMapRouting);
	Connect(wxID_TRACE_HSL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnTraceHSL);
	Connect(wxID_SUBBASIN_DISTANCE_TO_OUTLET, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&frame_swat::OnSubbasinDistanceToOutlet);
	Connect(wxID_ADD_UPSTREAM, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnAddUpstreamArea);
	Connect(wxID_FIND_OUTLET, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnFindOutlet);
	Connect(wxID_RUN_SWAT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnRunSWAT);
	Connect(wxID_RUN_SWAT_ALL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnRunSWATAll);
	Connect(wxID_CONFIGURE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnConfigure);
	Connect(wxID_CONNECT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnConnect);
	Connect(wxID_EXPORTINPUT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnExportInput);
	Connect(wxID_IMPORT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnImport);
	Connect(wxID_CREATE_TABLES, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnCreateTables);
	Connect(wxID_IMPORT_HISTORICAL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnImportHistoricalWeather);
	Connect (wxID_ROUTING_LIST, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnRoutingList);
	Connect (wxID_RECORD_PREVIOUS, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnRecordPrevious);
	Connect (wxID_RECORD_NEXT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &frame_swat::OnRecordNext);
}

void frame_swat::create_toolbar ()
{
    // delete and recreate the toolbar

    wxToolBarBase *toolBar = GetToolBar();
    long style = toolBar ? toolBar->GetWindowStyle() : wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;

    delete toolBar;

    SetToolBar(NULL);

    style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL | wxTB_BOTTOM | wxTB_RIGHT | wxTB_HORZ_LAYOUT);
	style |= wxTB_TOP;
    style &= ~wxTB_NO_TOOLTIPS;

    toolBar = CreateToolBar (style, ID_TOOLBAR);

	toolBar->SetToolBitmapSize(wxSize(24, 24));

	/*
    toolBar->AddTool(wxID_OPEN, wxT(""), wxBitmap (open_xpm), wxT("Open Configuration"));
	toolBar->AddTool(wxID_NEW, wxT(""), wxBitmap (new_xpm), wxT("New Configuration"));
    toolBar->AddTool(wxID_SAVE, wxT(""), wxBitmap (save_xpm), wxT("Save Configuration"), wxITEM_NORMAL);

    toolBar->AddSeparator();
    toolBar->AddTool(wxID_COPY, wxT(""), wxBitmap (copy_xpm), wxT("Copy"));
    toolBar->AddTool(wxID_CUT, wxT(""), wxBitmap (cut_xpm), wxT("Cut"));
    toolBar->AddTool(wxID_PASTE, wxT(""), wxBitmap (paste_xpm), wxT("Paste"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_PRINT, wxT(""), wxBitmap (print_xpm), wxT("Print"));
    toolBar->AddTool(wxID_HELP, wxT(""), wxBitmap (help_xpm), wxT("Help"));
    toolBar->AddSeparator();
	*/
    toolBar->AddTool(wxID_EDIT_CONTROL_RECORD, wxT(""), edit_control_xpm, wxT("Edit control record"));
    toolBar->AddTool(wxID_RUN_SWAT, wxT(""), run_swat_xpm, wxT("Run SWAT"));
    toolBar->Realize();

}

void frame_swat::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close (true /*force closing*/);
}

void frame_swat::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("i_SWAT"), _("About i_SWAT"), wxOK | wxICON_INFORMATION, this );
}

bool frame_swat::read_record_list
	(interface_window *,
	dynamic_string &error_message)

{
	bool error = false;
	{
		// peek at control record table just to see first ID#
		SWATControlRecord set_control, *new_record;

		set_control.initialize_field_set ();

		if (set_control.field_set->open (db_odbc, error_message)) {
			if (set_control.field_set->move_first (error_message)) {
				do {
					new_record = new SWATControlRecord;
					new_record->copy_rowset (set_control);
					control_records.push_back (new_record);
				} while (set_control.field_set->move_next (error_message));
			}
			set_control.field_set->close ();
		}
	}

	/*
	if (id != -1) {

		control.clear ();
		if (!control.read (db_odbc, id, true, update_display, error_message))
			error = true;
	}
	else
		error = true;
	*/

	return !error;
}

void frame_swat::OnOpenRecent
	(wxCommandEvent &ev)

{
	dynamic_string log;
	odbc_database_credentials creds;

	if (files_recently_opened->get (ev.GetId () - ID_FILE_RECENT_DB, &creds)) {
		interface_window_wx view;
		view.setup_wx (this);
		view.update_status_bar ("Opening database");
		SetCursor (*wxHOURGLASS_CURSOR);

		if (creds.type == ODBC_ACCESS)
			open_file (creds.database_name, &view, log);
		else
			if (connect_to_odbc (&creds, &view, log)) {
				if (read_db_start (&view, log)) {
					// tree->Update ();
					// fill_subbasin_list ();
					list_control->Update ();
					// if (read_first_record (log))
						// panel_tree->set_watershed (log);
				}
			}

		SetCursor (wxNullCursor);
		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"File Open", L"", log);
		d.ShowWindowModal ();
	}
}

bool frame_swat::connect_to_odbc
	(odbc_database_credentials *creds,
	interface_window *view,
	dynamic_string &log)

// DB can't be reached solely through it's filename

{
	dynamic_string connection_string;
	bool cancel = false, error = false;

	// if (!((app_i_epic *) AfxGetApp ())->get_credentials (database_filename.get_text_ascii (), type, &creds))

	dialog_open_database ddb (this, &app->db_credentials, creds);
	if (ddb.ShowModal () == wxID_OK) {
		app->add_credentials (*creds);
	}
	else
		cancel = true;

	if (!cancel) {
		switch (creds->type) {
			case ODBC_MYSQL:
				db_odbc = new odbc_database_mysql;
				break;
			case ODBC_SQLSERVER:
				db_odbc = new odbc_database_sql_server;
				break;
			case ODBC_ACCESS:
				db_odbc = new odbc_database_access;
				break;
		}
		if (db_odbc) {
			if (db_odbc->open (creds, view, log)) {
				files_recently_opened->retain (creds);
			}
			else {
				error = true;
				delete db_odbc;
				db_odbc = NULL;
			}
		}
	}

	if (error)
		return false;
	else
		return !cancel;
}

void frame_swat::OnFileNew
	(wxCommandEvent&)

{
    filename_struct f;
	dynamic_string log;
	BYTE database_type= ODBC_MYSQL;
	bool cancel = false;

	if (database_type == ODBC_MYSQL) {
		interface_window_wx view;
		view.setup_wx (this);
		view.update_status_bar ("Creating database");
		new_mysql_database (log);
	}
	else {
		dynamic_string path;
		// ODBC_ACCESS:
		wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "Access files (*.mdb)|*.mdb|Firebird files (*.fdb)|*.fdb", wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 
		if ( openFileDialog->ShowModal() == wxID_OK ) {
			SetCursor (*wxHOURGLASS_CURSOR);
			interface_window_wx view;
			view.setup_wx (this);
			view.update_status_bar ("Creating database");

			path = openFileDialog->GetDirectory().ToAscii ();
			path += PATH_SEPARATOR;
			path += openFileDialog->GetFilename().ToAscii ();

			/*
			switch (database_type) {
				case ODBC_SQLSERVER:
					if (!new_sqlserver_database (error_message))
						cancel = true;
					break;
				case ODBC_ACCESS:
					if (prompt_remove_database (database_filename)) {
						database_maker maker;
						db_odbc = new odbc_database_access;

						if (db_odbc
						&& maker.create_database (database_filename.get_text_ascii (), error_message)) {

							if (((odbc_database_access *) db_odbc)->open_by_name (database_filename, ODBC_ACCESS_DEFAULT_DRIVER, "", "", "", error_message))
								boss.create_tables (db_odbc, error_message);

							maker.close ();
						}
					}
					else
						cancel = true;
					break;
				case ODBC_FIREBIRD:
					if (prompt_remove_database (database_filename)) {
						if (is_firebird (database_filename.get_text ())) {
							odbc_database_credentials creds;

							creds.set_database_filename (database_filename);
							creds.type = ODBC_FIREBIRD;
							creds.set_hostname ("localhost");
							creds.set_driver_name (ODBC_FIREBIRD_DEFAULT_DRIVER);
							((app_i_swat *) AfxGetApp ())->db_credentials.push_back (creds);
							dialog_open_database ddb (NULL, &((app_i_swat *) AfxGetApp ())->db_credentials, &creds, ((app_i_swat *) AfxGetApp ())->db_encryption_key);
							if (ddb.DoModal () == IDOK) {
								((app_i_swat *) AfxGetApp ())->add_credentials (creds);

								db_odbc = new odbc_database_firebird;
								if (((odbc_database_firebird *) db_odbc)->open (&creds, ((app_i_swat *) AfxGetApp ())->db_encryption_key, error_message)) {

									((app_i_swat *) AfxGetApp ())->add_credentials (creds);

									boss.create_tables (db_odbc, error_message);
								}
							}
						}
						else {
							error_message += "ERROR, filename suffix incorrect.\n";
							cancel = true;
						}
					}
					else
						cancel = true;
			}
			*/
		}
		else
			cancel = true;
	}

	if (!cancel) {

		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"File Open", L"", log);
		d.ShowWindowModal ();

		// wxString wxlog = log.get_text_wide ();
		// m_logWindow->WriteText (wxlog);

		SetCursor (wxNullCursor);
	}
}

void frame_swat::OnCreateTables
	(wxCommandEvent &)

{
	swat_controller boss;
	dynamic_string log;
	interface_window_wx views;
	views.start_progress (this);

	views.update_progress ("DB is open");
	setup_controller (&boss);
	views.update_progress ("creating tables", 1);
	boss.create_tables (db_odbc, log);

	log.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"File Open", L"", log);
	d.ShowWindowModal ();
}

bool frame_swat::new_mysql_database
	(dynamic_string &log)
{
	bool error = false;

	dynamic_string sql, dbname;
	odbc_database_credentials creds;
	interface_window_wx views;
	views.setup_wx (this);

	// first connect without database name, it doesn't exist yet
	// creds.set_database_filename (database_filename);
	creds.set_database_filename ("SWAT");
	creds.type = ODBC_MYSQL;
	creds.set_driver_name (ODBC_MYSQL_DEFAULT_DRIVER);
	creds.set_hostname ("localhost"); // "." for named pipe; "localhost" for mysql server allowing TCP/IP connections
	creds.set_socket ("/var/run/mysqld/mysqld.sock");
	app->db_credentials.push_back (creds);

	dialog_open_database ddb (this, &app->db_credentials, &creds);
	if (ddb.ShowModal () == wxID_OK) {

		// pull db name out of creds for open (it doesn't exist yet)
		dbname = creds.database_name;
		creds.database_name.clear ();

		views.update_status_bar ("Opening DB");

		db_odbc = new odbc_database_mysql;
		if (((odbc_database_mysql *) db_odbc)->open (&creds, &views, log)) {

			swat_controller boss;

			views.update_status_bar ("DB is open");
			setup_controller (&boss);

			sql = "CREATE DATABASE ";
			sql += dbname;
			sql += ";";
			if (db_odbc->execute (sql, log)) {
				log += "Database created.\n";
				// Must use or else "[MySQL][ODBC 5.2(w) Driver][mysqld-5.5.28]No database selected"
				sql = "USE ";
				sql += dbname;
				sql += ";";
				if (db_odbc->execute (sql, log))
					log += "Database selected.\n";

				creds.database_name = dbname;
				app->add_credentials (creds);
				boss.create_tables (db_odbc, log);
			}
		}
		else
			error = true;
		return !error;
	}
	else
		return false;
}

void frame_swat::OnFileOpen
	(wxCommandEvent&)

{
    filename_struct f;
	dynamic_string path;
	dynamic_string log;
	bool cancel = false;

	// ODBC_ACCESS:
	wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "Access files (*.mdb;*.accdb)|*.mdb;*.accdb|Firebird files (*.fdb)|*.fdb", wxFD_OPEN, wxDefaultPosition);
 
	if ( openFileDialog->ShowModal() == wxID_OK ) {
		SetCursor (*wxHOURGLASS_CURSOR);
		interface_window_wx view;
		view.setup_wx (this);
		view.update_status_bar ("Opening database");

		path = openFileDialog->GetDirectory().ToAscii ();
		path += PATH_SEPARATOR;
		path += openFileDialog->GetFilename().ToAscii ();

		if (open_file (path, &view, log)) {
			odbc_database_credentials creds;
			creds.type = ODBC_ACCESS;
			creds.set_database_filename (path);
			files_recently_opened->retain (&creds);
			view.update_status_bar ("Database open");
			read_db_start (&view, log);
		}
		SetCursor (wxNullCursor);
	}
	else
		cancel = true;

	if (!cancel) {

		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"File Open", L"", log);
		d.ShowWindowModal ();

		// wxString wxlog = log.get_text_wide ();
		// m_logWindow->WriteText (wxlog);

	}
}

bool frame_swat::read_db_start
	(interface_window *view,
	dynamic_string &log)

{
	dynamic_string record_description;
	wxString wxrecord;
	bool error = false;

	view->update_progress ("Reading soil", 1);
	if (soils.read (db_odbc, log)) {
		view->update_progress ("Reading weather", 1);
		swat_weather.read (db_odbc, view, log);
		view->update_progress ("Reading [Tillage]", 1);
		tillages.read (db_odbc, log);
		view->update_progress ("Reading [Crop]", 1);
		crops.read (db_odbc, log);
		view->update_progress ("Reading [Urban]", 1);
		urbans.read (db_odbc, log);
		view->update_progress ("Reading [Fertilizer]", 1);
		fertilizers.read (db_odbc, log);
		view->update_progress ("Reading [Pesticide]", 1);
		pesticides.read (db_odbc, log);
		view->update_progress ("Reading [Septic Water Quality]", 1);
		septic_wqs.read (db_odbc, log);

		if (read_record_list (view, log)) {
			/*
			record_description.unicode = false;

			record_description.format ("%ld %s", control.ID, control.Description);
			wxrecord = record_description.get_text_wide ();
			list_control->InsertItem (i++, wxrecord);
			*/
		}
		else
			error = true;
	}
	else
		// Emtpy database?
		error = true;

	return !error;
}

/*
bool frame_swat::read_first_record
	(dynamic_string &log)

{
	long id;
	if ((id = list_control->GetSelectedID ()) != 0) {
		if (current_control_record)
			delete current_control_record;
		current_control_record = new SWATControlRecord;
		if (current_control_record->read (db_odbc, id, false, NULL, log)) {
			return true;
		}
	}
	return false;
}
*/

bool frame_swat::open_file
	(const dynamic_string &filename,
	interface_window *view,
	dynamic_string &log)

{
	bool error = false;

	if (db_odbc == NULL) {
		db_odbc = new odbc_database_access;
		odbc_database_credentials creds;
		creds.set_database_filename (filename);
#ifdef _WIN64
		creds.set_driver_name (ODBC_ACCESS_DEFAULT_DRIVER_64);
#else
		creds.set_driver_name (ODBC_ACCESS_DEFAULT_DRIVER);
#endif
		creds.type = ODBC_ACCESS;
		creds.set_hostname (".");

		// if (((odbc_database_access *) db_odbc)->open (filename, ODBC_ACCESS_DEFAULT_DRIVER, "", "", L"", log))
		if (((odbc_database_access *) db_odbc)->open (&creds, view, log)) {
			if (read_db_start (view, log)) {
				// tree->Update ();
				list_control->Update ();

				// panel_tree->set_watershed (log);
			}
			else
				error = true;
		}
		else {
			delete db_odbc;
			db_odbc = NULL;
			error = true;
		}
	}
	else {
		error = true;
		log += "DB already open.\n";
	}

	return !error;
}

void frame_swat::DoSize()
{
    // if ( !m_logWindow )
    //  return;


	/*
    wxSize size = GetClientSize();
    wxCoord segment_height = size.y / 5;
    // list_control->SetSize(0, 0, size.x, y);
    if (tree) {
	    tree->SetSize (0, 0, size.x, segment_height * 2);
		// list_subbasins->SetSize (0, segment_height * 2 + 1, size.x, segment_height * 4);
		m_logWindow->SetSize(0, segment_height * 4 + 1, size.x, size.y);
	}
	else
		m_logWindow->SetSize(0, 0, size.x, size.y);
	*/
}

void frame_swat::OnReportPollutants
	(wxCommandEvent &)

// Summarize [Output Reach] for outlet as done by GA runs

{
	dynamic_string report, outlet_search_report;
	std::map <long, swat_output_reach>::iterator output_sub;
	std::vector <SWATOutputReachAnnual>::const_iterator reach_year;
	std::vector <double> annual_n [4], annual_p [2], sediment;
	std::vector <double>::const_iterator pollutant_year;
	double N [4], P [2], total_sediment;
	SWATControlRecord control;
	int id, pollutant_index;
	SetCursor (*wxHOURGLASS_CURSOR);
	long outlet_override = -1; // MRB=501

	report += db_odbc->database_filename ();
	report += "\n";

	if ((id = list_control->GetSelectedID ()) != 0) {
		if (control.read (db_odbc, id, true, NULL, report)) {

			long outlet_subbasin;

			if (outlet_override == -1)
				outlet_subbasin = control.routing.find_outlet_subbasin (report);
			else
				// long outlet_subbasin = 30; // Boone	
				outlet_subbasin = outlet_override;

			report.add_formatted ("ID %ld\n", id);

			total_sediment = 0.0;
			memset (N, 0, sizeof (double) * 4);
			memset (P, 0, sizeof (double) * 2);

			// N at outlet
			for (output_sub = control.output.reach_map.begin ();
			output_sub != control.output.reach_map.end ();
			++output_sub) {
				if (output_sub->second.Reach == outlet_subbasin) {
					for (reach_year = output_sub->second.year_list.begin ();
					reach_year != output_sub->second.year_list.end ();
					++reach_year) {
						if (reach_year->Year >= control.FirstYear + control.SkipYears) {
							sediment.push_back ((double) reach_year->SedimentOut);
							annual_n [0].push_back ((double) reach_year->OrganicNOut);
							annual_n [1].push_back ((double) reach_year->NO3Out);
							annual_n [2].push_back ((double) reach_year->NH4Out);
							annual_n [3].push_back ((double) reach_year->NO2Out);
							annual_p [0].push_back ((double) reach_year->OrganicPOut);
							annual_p [1].push_back ((double) reach_year->MineralPOut);
						}
					}
				}
			}


			report.add_formatted ("Outlet Reach %ld.\n", outlet_subbasin);
			report += "\n";

			for (pollutant_year = sediment.begin ();
			pollutant_year != sediment.end ();
			++pollutant_year)
				total_sediment += *pollutant_year;

			for (pollutant_index = 0; pollutant_index < 4; ++pollutant_index)
				for (pollutant_year = annual_n [pollutant_index].begin ();
				pollutant_year != annual_n [pollutant_index].end ();
				++pollutant_year)
					N [pollutant_index] += *pollutant_year;

			for (pollutant_index = 0; pollutant_index < 2; ++pollutant_index)
				for (pollutant_year = annual_p [pollutant_index].begin ();
				pollutant_year != annual_p [pollutant_index].end ();
				++pollutant_year)
					P [pollutant_index] += *pollutant_year;

			report += "\tMean";
			for (int year_index = 0; year_index < (control.RunLength - control.SkipYears); ++year_index)
				report.add_formatted ("\t%d", year_index + control.FirstYear + control.SkipYears);
			report += "\n";

			report.add_formatted ("Sediment Out\t%.2lf", total_sediment / (double) (control.RunLength - control.SkipYears));
			for (pollutant_year = sediment.begin ();
			pollutant_year != sediment.end ();
			++pollutant_year)
				report.add_formatted ("\t%.2lf", *pollutant_year);
			report += "\n";

			report += "\n";
			report.add_formatted ("Organic N\t%.2lf", N [0] / (double) (control.RunLength - control.SkipYears));
			for (pollutant_year = annual_n [0].begin ();
			pollutant_year != annual_n [0].end ();
			++pollutant_year)
				report.add_formatted ("\t%.2lf", *pollutant_year);
			report += "\n";

			report.add_formatted ("NO3\t%.2lf", N [1] / (double) (control.RunLength - control.SkipYears));
			for (pollutant_year = annual_n [1].begin ();
			pollutant_year != annual_n [1].end ();
			++pollutant_year)
				report.add_formatted ("\t%.2lf", *pollutant_year);
			report += "\n";

			report.add_formatted ("NH4\t%.2lf", N [2] / (double) (control.RunLength - control.SkipYears));
			for (pollutant_year = annual_n [2].begin ();
			pollutant_year != annual_n [2].end ();
			++pollutant_year)
				report.add_formatted ("\t%.2lf", *pollutant_year);
			report += "\n";

			report.add_formatted ("NO2\t%.2lf", N [3] / (double) (control.RunLength - control.SkipYears));
			for (pollutant_year = annual_n [3].begin ();
			pollutant_year != annual_n [3].end ();
			++pollutant_year)
				report.add_formatted ("\t%.2lf", *pollutant_year);
			report += "\n";

			report += "\n";
			report.add_formatted ("Organic P\t%.2lf", P [0] / (double) (control.RunLength - control.SkipYears));
			for (pollutant_year = annual_p [0].begin ();
			pollutant_year != annual_p [0].end ();
			++pollutant_year)
				report.add_formatted ("\t%.2lf", *pollutant_year);
			report += "\n";

			report.add_formatted ("Mineral P\t%.2lf", P [1] / (double) (control.RunLength - control.SkipYears));
			for (pollutant_year = annual_p [1].begin ();
			pollutant_year != annual_p [1].end ();
			++pollutant_year)
				report.add_formatted ("\t%.2lf", *pollutant_year);
			report += "\n";

			report += "\n";
			report.add_formatted ("Mean N\t%.2lf\n", (N [0] + N [1] + N [2] + N [3]) / (double) (control.RunLength - control.SkipYears));
			report.add_formatted ("Mean P\t%.2lf\n", (P [0] + P [1]) / (double) (control.RunLength - control.SkipYears));
		}
	}
	SetCursor (wxNullCursor);

	report.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Pollutants Report", L"", report);
	d.ShowWindowModal ();

}

void frame_swat::OnReportTillage
	(wxCommandEvent &)

// 2015-01-12 Report tillage effect by HRU

{
	dynamic_string report;
	double *residue;
	std::map <int, long> planted_crops;
	std::map <int, long>::iterator crop_year;
	std::vector <SubBasin>::const_iterator basin;
	std::vector <HRU_record>::const_iterator hru;
	int id;
	SWATControlRecord control;
	int effective_tillage, year_index;
	int hru_error_count = 0;
	bool hru_error;

	if ((id = list_control->GetSelectedID ()) != 0) {
		if (control.read (db_odbc, id, false, NULL, report)) {
			report += "Subbasin\tHRU\tRotation\tYear\tTillage\tResidue\tEffective Tillage\n";
			for (basin = control.subbasins.basins.begin ();
			basin != control.subbasins.basins.end ();
			++basin) {
				for (hru = basin->hru_list.hrus.begin ();
				hru != basin->hru_list.hrus.end ();
				++hru) {
					hru_error = false;

					hru->tillage_residue (&tillages, &residue, report);

					for (year_index = 0; year_index < hru->RotationLength; ++year_index) {

						// 2015-01-12 Tillage percentages are from view_procost_budget.render
						if (residue [year_index] < 0.15)
							effective_tillage = TILLAGE_EPIC_CONVENTIONAL;
						else
							if (residue [year_index] <= 0.30)
								effective_tillage = TILLAGE_EPIC_REDUCED;
							else
								if (residue [year_index] <= 0.70)
									effective_tillage = TILLAGE_EPIC_MULCH;
								else
									effective_tillage = TILLAGE_EPIC_NO;

						report.add_formatted ("%ld\t%ld\t%d\t%d\t%d\t%.2lf\t%d", basin->Subbasin, hru->HRU,
						hru->Rotation, year_index + 1,
						hru->Tillage, residue [year_index], effective_tillage);

						if ((hru->Tillage != effective_tillage)
						&& !hru_error) {
							hru->make_crop_list (&planted_crops, &crops);
							crop_year = planted_crops.find (year_index + 1);

							if (hru->Rotation == ROTATION_ARMS_C_A) {
								if ((crop_year->second != CROP_NRI_CORN)
								&& (effective_tillage == TILLAGE_EPIC_NO)) {
									// Alfalfa years of C-A rotation are always no till
								}
								else {
									report += "\t*";
									hru_error = true;
									++hru_error_count;
								}
							}
						}
						report += "\n";
					}
					delete [] residue;
				}
			}
		}
		report.add_formatted ("\n#HRUs with errors\t%d\n", hru_error_count);

		report.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"Tillage Report", L"", report);
		d.ShowWindowModal ();
	}
}

void frame_swat::setup_controller
	(swat_controller *boss)

{
	app->setup_controller (boss);
	
	if (db_odbc)
		boss->filename_database = db_odbc->database_filename ();

	boss->soils = &soils;
	boss->swat_weather = &swat_weather;
	boss->tillages = &tillages;
	boss->crops = &crops;
	boss->urbans = &urbans;
	boss->fertilizers = &fertilizers;
	boss->pesticides = &pesticides;
	boss->septic_wqs = &septic_wqs;
	boss->output_variables = output_variables;
}

void frame_swat::OnRunSWAT
	(wxCommandEvent&)

{
	dynamic_string log;

	swat_controller boss;
	interface_window_wx views;
	dynamic_string status_text;
	filename_struct path_struct;
	bool run_success = false, error = false;
	SWATControlRecord control;
	FortranCommonData *common;
	long id, outlet_subbasin;
	swat_routing_node *node_459 = NULL, *node_outlet = NULL, *start_nodes [10];

	// boss.write_input_std = false;
	// boss.write_output_std = false;

	// _ftime_s (&start);

	/*
	{
		NitrateNetworkModel nnm;
		nnm.write_binary_input = false;
		// nnm.run (&control, "E:\\NNM\\Application\\", &views, log);
		nnm.run (&control, "g:\\swat2012\\", &views, log);

		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"SWAT run", L"", log);
		d.ShowWindowModal ();
	}
	*/

	if ((id = list_control->GetSelectedID ()) != 0) {
		SetCursor (*wxHOURGLASS_CURSOR);

		control.read (db_odbc, id, false, NULL, log);

		// NHDNetworkModel nnm;
		// nnm.read_binary ("g:\\swat2012\\NNM_Data0001.bin", &control, log);
		// nnm.read_results ("E:\\NNM\\Application\\NNM_Results0001.bin", log);

		if (control.check (app->SWATVersion, app->historical_weather_method, &soils, &swat_weather, &tillages, &fertilizers, &pesticides, log)) {

			views.setup_wx (this);

			setup_controller (&boss);

			if (boss.write_output_hsl) {
				outlet_subbasin = control.routing.find_outlet_subbasin (log);
				for (int index = 0; index < 10; ++index)
					start_nodes[index] = control.routing.find_node_below_subbasin (index, SWAT_ROUTING_COMMAND_ADD);
				node_459 = control.routing.find_node_below_subbasin (459, SWAT_ROUTING_COMMAND_ADD);
				node_outlet = control.routing.find_node_below_subbasin (outlet_subbasin, SWAT_ROUTING_COMMAND_ADD);
			}

			if ((boss.run_method == SWAT_RUN_METHOD_DLL)
			|| (boss.run_method == SWAT_RUN_METHOD_HRU)) {
				views.update_status_bar ("Running SWAT DLL");
				log += "Weather Path \"";
				log += app->path_historical_weather;
				log += "\"\n";

				if (boss.historical_weather_method == SWAT_HISTORICAL_WEATHER_DIRECT_FILE) {
					// 2022-05-26 Second SWAT run must either not reread historical weather, or clear before reading
					swat_weather.historical_data.clear();

					// read once from files for DLL array, don't write to database
					if (!boss.swat_weather->preread_historical_weather (boss.path_historical_weather_and_atmos, &control, &views, log))
						error = true;
				}
				if ((common = boss.create_common_data (&control, log)) == NULL)
					error = true;
			}
			else {
				common = NULL;
				status_text = "Writing SWAT input files to \"";
				status_text += boss.run_folder;
				status_text.add_formatted ("\" for ID %ld\n", boss.run_folder.get_text (), control.ID);
				views.update_status_bar (&status_text);
			}

			if (!error) {
				log += boss.filename_database;
				log += "\n";
				boss.write_run_settings (log);

				run_success = boss.run_swat (&control, common, &views, log);

				if (run_success) {
#ifdef _ODBC_TDC
					control.output.write (db_odbc,
#else
					control.output.write (database,
#endif
					&boss, &control, &views, log);

					control.output.write_std_output (log, false);

					if (boss.write_output_hsl) {
						routing_environment_data environment;

						environment.mission = ROUTING_MISSION_WRITE_OUTPUT;
						environment.log = &log;

						log += "\nHSL\tCommand\tDestination\tFlow m^3\tSediment t\tOrganic Nitrogen kg\tOrganic Phosphorus kg\tNitrate kg\tSoluble Phosphorus kg\tSoluble Pesticide mg\n";
						control.routing.outlet->traverse (NULL, &environment, 0);

						// log += "\nHSL\tFlow m^3\tSediment t\tOrganic Nitrogen kg\tOrganic Phosphorus kg\tNitrate kg\tSoluble Phosphorus kg\tSoluble Pesticide mg\n";
						// node_459->write_hydrology_output (log);
						// node_outlet->write_hydrology_output (log);
						// log += "\n";
					}

					output_yield_tracker yield_tracker;

					yield_tracker.show_crop = true;
					yield_tracker.show_year = false;
					yield_tracker.show_subbasin_crop = false;
					yield_tracker.show_year_crop = false;
					yield_tracker.show_subbasin_year_crop = false;

					if (yield_tracker.accumulate (&control, log))
						yield_tracker.summarize (&crops, log);

					// write daily output to NNM
					// NitrateNetworkModel nnm;
					// nnm.run (&control, boss.path_swat, &views, log);
				}
			}
			if ((boss.run_method == SWAT_RUN_METHOD_DLL)
			|| (boss.run_method == SWAT_RUN_METHOD_HRU))
				delete common;
		}
		SetCursor (wxNullCursor);

		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"SWAT run", L"", log);
		d.ShowWindowModal ();
	}
}

void frame_swat::OnConfigure
	(wxCommandEvent&)

{
	dialog_configure *d = new dialog_configure (this, app);
	if (d->ShowModal () == wxID_OK) {
	}
}

void frame_swat::OnConnect
	(wxCommandEvent&)

{
	// ODBC_ACCESS:
	SetCursor (*wxHOURGLASS_CURSOR);
	dynamic_string log;
	odbc_database_credentials creds;
	bool cancel = false;
	interface_window_wx view;
	view.setup_wx (this);
	view.update_status_bar ("Opening database");

#ifdef _WINDOWS
	creds.set_database_filename ("wxswat");
	creds.type = ODBC_ACCESS;
	creds.set_hostname (".");
	creds.set_driver_name (ODBC_ACCESS_DEFAULT_DRIVER);
#else
	creds.set_database_filename ("wxswat");
	creds.type = ODBC_MYSQL;
	creds.set_hostname ("localhost");
	creds.set_socket ("/var/run/mysqld/mysqld.sock");
	// creds.set_driver_name (ODBC_MYSQL_DEFAULT_DRIVER);
	creds.set_driver_name (ODBC_MYSQL_DEFAULT_DRIVER); // result of "odbcinst -q -d"
#endif

	if (connect_to_odbc (&creds, &view, log)) {
		// If new database, this will fail and swat_controller.create_tables must be called
		if (read_db_start (&view, log)) {
			// tree->Update ();
			// fill_subbasin_list ();
			list_control->Update ();
		}
	}
	else
		cancel = true;

	if (!cancel) {

		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"File Open", L"", log);
		d.ShowWindowModal ();

		SetCursor (wxNullCursor);
	}
}

/*
void frame_swat::fill_subbasin_list ()

{
	dynamic_string item;
	wxString *list_array;
	int index, count;
	SubBasin *basin;

	list_subbasins->Clear ();

	count = control.subbasins.size ();

	list_array = new wxString [count];

	index = 0;
	basin = (SubBasin *) control.subbasins.first_member ();
	while (basin) {
		item.format ("%ld %ld ", basin->ID, basin->Subbasin);
		item += basin->Description;
		list_array [index++] = item.get_text ();
		basin = (SubBasin *) basin->next;
	}
	list_subbasins->InsertItems (count, list_array, 0);

	delete [] list_array;
}
*/

void frame_swat::OnEditControl (wxCommandEvent &)

{
	dynamic_string log;
	swat_controller boss;
	long id;
	bool error = false;
	SWATControlRecord control_record;

	if (db_odbc) {
		setup_controller (&boss);

		log = "Edit control\n";
		if ((id = list_control->GetSelectedID ()) != 0) {
			SetStatusText (_ ("Reading control"));
			SetCursor (*wxHOURGLASS_CURSOR);
			if (control_record.read (db_odbc, id, false, NULL, log)) {
				SetCursor (wxNullCursor);
				SetStatusText (_ ("Starting propsheet"));
				propsheet_control_record dc (this, &control_record, &boss, help_list, db_odbc);
				if (dc.ShowModal () == wxID_OK) {
					SetCursor (*wxHOURGLASS_CURSOR);
					interface_window_wx update_display;
					update_display.setup_wx (this);
					if (!write_control_record (&control_record, &update_display, log))
						error = true;
					SetCursor (wxNullCursor);
				}
			}
			else {
				SetCursor (wxNullCursor);
				error = true;
			}

			if (error) {
				log.convert_linefeeds_for_CEdit ();
				dialog_error_list d (this, L"File Open", L"", log);
				d.ShowWindowModal ();
			}
		}
	}

}

bool save_shapefile
	(map_layer *layer,
	const char *filename,
	const dynamic_string &id_field_name,
	const dynamic_string &name_field_name,
	interface_window *update_display,
	dynamic_string &log)

{
	importer_shapefile s;
	std::vector <dynamic_string>::iterator column_name;

	// Write to new shapefile
	std::map <long long, shapefile_feature> map_features;
	std::map <int, shapefile_dbase_field> dbf_fields;
	shapefile_dbase_field field;
	int column_number;

	s.create_features (layer, &map_features, layer->enclosure == MAP_POLYGON_LAST_OVERLAPS_FIRST, log);
	layer->check_extent (&s.bounding_box);
	s.bounding_box.x [0] /= 1.0e6;
	s.bounding_box.x [1] /= 1.0e6;
	s.bounding_box.y [0] /= 1.0e6;
	s.bounding_box.y [1] /= 1.0e6;

	column_number = 1;

	if (id_field_name.get_length () > 0) {
		s.id_field_name = id_field_name;

		strcpy_s (field.Name, DB_FIELD_NAME_LENGTH, s.id_field_name.left (DB_FIELD_NAME_LENGTH - 1).get_text_ascii ());
		field.Type = DBASE_FIELD_LONG_INTEGER;
		field.map_layer_index_numeric = column_number;
		field.is_id = true;
		dbf_fields.insert (std::pair <int, shapefile_dbase_field> (column_number++, field));
	}
	if (name_field_name.get_length () > 0) {
		s.name_field_name = name_field_name;

		strcpy_s (field.Name, DB_FIELD_NAME_LENGTH, s.name_field_name.left (DB_FIELD_NAME_LENGTH - 1).get_text_ascii ());
		field.Type = DBASE_FIELD_TEXT;
		field.Length = DBASE_TEXT_FIELD_LENGTH;
		field.map_layer_index_text = column_number;
		field.is_name = true;
		dbf_fields.insert (std::pair <int, shapefile_dbase_field> (column_number++, field));
	}
	s.projection = SHAPEFILE_PROJECTED_LAT_LONG;
	s.filename_source = filename;

	// Add columns for attributes_numeric
	for (column_name = layer->column_names_numeric.begin ();
	column_name != layer->column_names_numeric.end ();
	++column_name) {
		strcpy_s (field.Name, DB_FIELD_NAME_LENGTH, column_name->left (DB_FIELD_NAME_LENGTH - 1).get_text_ascii ());
		field.Type = DBASE_FIELD_DOUBLE;
		field.map_layer_index_numeric = column_number;
		dbf_fields.insert (std::pair <int, shapefile_dbase_field> (column_number++, field));
	}

	// Add columns for attributes_text
	for (column_name = layer->column_names_text.begin ();
	column_name != layer->column_names_text.end ();
	++column_name) {
		strcpy_s (field.Name, DB_FIELD_NAME_LENGTH, column_name->left (DB_FIELD_NAME_LENGTH - 1).get_text_ascii ());
		field.Type = DBASE_FIELD_TEXT;
		field.map_layer_index_text = column_number;
		dbf_fields.insert (std::pair <int, shapefile_dbase_field> (column_number++, field));
	}

	return s.write (&map_features, &dbf_fields, layer, update_display, log);

}

void frame_swat::OnAddUpstreamArea (wxCommandEvent &)

// 2017-06-01 
{
	dynamic_string log;
	SWATControlRecord control_record;
	long target_subbasin;
	double area_km2, total_area_km2;

	dialog_prompt dp (this, "Enter Subbasin", &target_subbasin, "20");
	if (dp.ShowModal () == wxID_OK) {

		interface_window_wx update_display;
		update_display.start_progress (this);
		update_display.update_progress ("Loading");
		if (db_odbc
		&& control_record.read (db_odbc, 1, false, NULL, log)) {

			// Path to 421
			std::set <long> upstream_hsls;
			std::set <long>::iterator hsl;

			// 2017-06-01 LeSueur fig file built by AVSWAT
			// Each subbasin connects to an ADD, which is where the search has to start
			swat_routing_node *add_node;
			add_node = control_record.routing.find_node_below_subbasin (target_subbasin, SWAT_ROUTING_COMMAND_ADD);
			control_record.routing.list_upstream_nodes (add_node->HydrographStorageLocation, &upstream_hsls);

			total_area_km2 = 0.0;

			SubBasin *basin;
			for (hsl = upstream_hsls.begin ();
			hsl != upstream_hsls.end ();
			++hsl)
				if ((basin = control_record.match_subbasin (*hsl)) != NULL) {
					area_km2 = basin->total_area_km2 ();
					log.add_formatted ("%ld\t%.4lf\n", *hsl, area_km2);
					total_area_km2 += area_km2;
				}

			log.add_formatted ("All\t%.4lf\n", total_area_km2);
		}
	}

	if (log.get_length () > 0) {
		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"", L"", log);
		d.ShowModal ();
	}
}

void frame_swat::OnRoutingList (wxCommandEvent &)

// 2017-06-01 
{
	dynamic_string log;
	SWATControlRecord control_record;

	interface_window_wx update_display;
	update_display.start_progress (this);
	update_display.update_progress ("Loading");
	if (db_odbc
	&& control_record.read (db_odbc, 1, false, NULL, log)) {

		// Path to 421
		std::vector <long> route_hsls;
		std::vector <long>::const_iterator hsl;
		// if (control_record.routing.outlet->accumulate_path (421, &route_hsls))
		control_record.routing.describe (log);
	}

	if (log.get_length () > 0) {
		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"", L"", log);
		d.ShowModal ();
	}
}

void frame_swat::OnTraceHSL (wxCommandEvent &)

// List all HSLs from outlet up to target

{
	dynamic_string log;
	SWATControlRecord control_record;
	long target_hsl;

	dialog_prompt dp (this, "Enter HSL", &target_hsl, "20229");
	if (dp.ShowModal () == wxID_OK) {

		interface_window_wx update_display;
		update_display.start_progress (this);
		update_display.update_progress ("Loading");
		if (db_odbc
		&& control_record.read (db_odbc, 1, false, NULL, log)) {

			// Path to 421
			std::vector <long> route_hsls;
			std::vector <long>::const_iterator hsl;

			routing_environment_data environment;
			swat_routing_node *node;
			std::map <long, int>::iterator basin;
			int lowest_height = 999, second_lowest_height = 999;
			long lowest_HSL = -1, second_lowest_HSL = -1;

			log += "HSL";
			log += "\tLowest HSL\tHeight";
			log += "\t2nd Lowest HSL\tHeight";
			log += "\n";

			// if (control_record.routing.outlet->accumulate_path (421, &route_hsls))
			if (control_record.routing.outlet->accumulate_path (target_hsl, &route_hsls)) {

				environment.mission = ROUTING_ENVIRONMENT_OUTLET_SUBBASIN;
				for (hsl = route_hsls.begin ();
				hsl != route_hsls.end ();
				++hsl) {
					log.add_formatted ("%ld", *hsl);
					// Find nearest subbasin upstream from HSL
					if ((node = control_record.routing.match_hsl (*hsl)) != NULL) {
						environment.basin_distances.clear ();
						node->traverse (NULL, &environment, 0);

						if ((basin = environment.basin_distances.begin ()) != environment.basin_distances.end ()) {
							lowest_height = basin->second;
							lowest_HSL = basin->first;

							while (++basin != environment.basin_distances.end ()) {
								if (basin->second < lowest_height) {
									second_lowest_height = lowest_height;
									second_lowest_HSL = lowest_HSL;

									lowest_height = basin->second;
									lowest_HSL = basin->first;
								}
								else
									if (basin->second < second_lowest_height) {
										second_lowest_height = basin->second;
										second_lowest_HSL = basin->first;
									}
							}
							log.add_formatted ("\t%ld\t%d\t%ld\t%d", lowest_HSL, lowest_height, second_lowest_HSL, second_lowest_height);
						}
					}
					log += "\n";
				}
			}
		}
	}

	if (log.get_length () > 0) {
		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"", L"", log);
		d.ShowModal ();
	}
}

void frame_swat::OnFindOutlet (wxCommandEvent &)

{
	dynamic_string log;
	SWATControlRecord control_record;

	interface_window_wx update_display;
	update_display.start_progress (this);
	update_display.update_progress ("Loading");

	if (db_odbc
	&& control_record.read (db_odbc, 1, false, NULL, log))
		control_record.routing.find_outlet_subbasin (log);

	if (log.get_length () > 0) {
		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"", L"", log);
		d.ShowModal ();
	}
}

void frame_swat::OnMapRouting (wxCommandEvent &)

{
	map_layer *basin_layer, *vector_layer;
	map_layer *shapefile_layer = NULL;
	device_coordinate outlet_position;
	dynamic_string log, path_vectors, path_basins;
	dynamic_map map;
	SWATControlRecord control_record;
	importer_shapefile importer;
	long outlet_subbasin_id;
	map_polygon *sub;
	filename_struct shapefile_path;
	bool clip_to_map;

	/*
	// UMRB 12-digit Subbasins
	shapefile_layer = new map_layer;
	shapefile_layer->name = "12digits_UMRB_UTM";
	importer.longitude_central_meridian = -93; // 75;
	importer.filename_source = "r:\\NSF project\\GIS_Data\\UTM Projection\\12digits_UMRB_UTM.shp";
	importer.has_id = true;
	importer.id_field_name = "Subbasin";
	importer.projection = SHAPEFILE_PROJECTED_UTM;

	// OTRB 12-digit
	shapefile_layer = new map_layer;
	shapefile_layer->name = "12digits_OTRB";
	importer.longitude_central_meridian = -96;
	importer.filename_source = "r:\\NSF project\\GIS_Data\\UTM Projection\\12digits_OTRB.shp";
	importer.has_id = true;
	importer.id_field_name = "Subbasin";
	importer.projection = SHAPEFILE_PROJECTED_UTM;
	*/

	// LeSueur Subbasins
	// Albers; converted to WGS1984
	shapefile_layer = new map_layer;
	importer.longitude_central_meridian = -93; // 75;
	importer.filename_source = "E:\\swat runs\\Minnesota\\Shapefiles 2017-02-17\\GCS_WGS_1984\\subs1.shp";
	importer.id_field_name = "Subbasin";
	importer.projection = SHAPEFILE_PROJECTED_LAT_LONG;
	shapefile_layer->name = "LeSueur Subbasin";

	/*
	// Boone subbasins
	shapefile_layer = new map_layer;
	importer.longitude_central_meridian = -93;
	importer.filename_source = "R:\\SWAT\\Boone\\GIS\\watsub2.shp";
	importer.has_id = true;
	importer.id_field_name = "SUBBASIN";
	importer.projection = SHAPEFILE_PROJECTED_UTM;
	shapefile_layer->name = "Boone";
	*/

	dialog_import_shapefile dis (this, recent_shapefiles, &importer, &clip_to_map);
 
	if (dis.ShowModal() == wxID_OK ) {
		interface_window_wx update_display;
		update_display.start_progress (this);
		update_display.update_progress ("Loading");

		if (db_odbc
		&& control_record.read (db_odbc, 1, false, NULL, log)) {

			map.layers.clear ();

			update_display.update_progress ("Loading Shapefile");

			shapefile_path.parse (&db_odbc->database_filename ());
			shapefile_path.write_path (&path_vectors);
			path_vectors += "Routing vectors.shp";
			shapefile_path.write_path (&path_basins);
			path_basins += "Routing basins.shp";

			if (importer.import (shapefile_layer, NULL, &map, &update_display, log)) {
				map.layers.push_back (shapefile_layer);

				update_display.update_progress ("Loading vectors");
				// vectors first so they'll be drawn first
				// and so they won't be selected when clicked
				vector_layer = new map_layer;
				vector_layer->type = MAP_OBJECT_VECTOR;
				vector_layer->name = "Vectors";
				vector_layer->color = RGB (0, 0, 160);
				vector_layer->arrow_width = 25;
				vector_layer->attribute_count_numeric = 1;
				vector_layer->initialize_attributes = true;
				vector_layer->column_names_numeric.push_back ("Destination");
				map.layers.push_back(vector_layer);

				update_display.update_progress ("Loading Subbasins");
				basin_layer = new map_layer;
				basin_layer->type = MAP_OBJECT_POLYGON;
				basin_layer->attribute_count_numeric = 1;
				basin_layer->initialize_attributes = true;
				basin_layer->column_names_numeric.push_back ("Type"); // 1=basin, 2=routeres, 3=add
				basin_layer->name = "Sub-Basin Centroids";
				basin_layer->color = RGB (255, 0, 0);
				map.layers.push_back(basin_layer);

				// destination of get_position and build_map needs to be a real position
				// Use coordinates of outlet subbasin
				// outlet node is a route command or other
				outlet_subbasin_id = control_record.routing.find_outlet_subbasin (log);

				if ((shapefile_layer->type == MAP_OBJECT_POLYGON)
				&& ((sub = (map_polygon *) shapefile_layer->match_id (outlet_subbasin_id)) != NULL)) {

					sub->centroid (&outlet_position.y, &outlet_position.x);

					control_record.routing.outlet->build_map (&control_record.subbasins,
					shapefile_layer,
					vector_layer,
					basin_layer,
					outlet_position,
					control_record.routing.outlet->HydrographStorageLocation);

					map.set_extent ();

					// vector_layer->dump (log);
					// basin_layer->dump (log);

					save_shapefile (vector_layer, path_vectors.get_text_ascii (), "ID", "Name", &update_display, log);
					save_shapefile (basin_layer, path_basins.get_text_ascii (), "ID", "Name", &update_display, log);
				}
				else
					log.add_formatted ("ERROR, can't find subbasin id %ld in shapefile.\n", outlet_subbasin_id);
			}
		}
	}

	if (log.get_length () > 0) {
		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"", L"", log);
		d.ShowModal ();
	}
}


void frame_swat::OnViewRouting (wxCommandEvent &)

{
	dynamic_string log;
	swat_controller boss;
	long id;
	bool error = false;
	SWATControlRecord control_record;

	setup_controller (&boss);

	log = "Edit control\n";
	if ((id = list_control->GetSelectedID ()) != 0) {
		if (control_record.read (db_odbc, id, false, NULL, log)) {
			SetSize (1000, 1000);
			// add panel_routing_tree;
			panel_tree = new panel_routing_tree (this, wxID_ANY);
			stack->Add (panel_tree);
			SetSizerAndFit (stack);
			stack->SetSizeHints( this );
			panel_tree->set_watershed (&control_record, log);
		}
		else
			error = true;

		if (error) {
			log.convert_linefeeds_for_CEdit ();
			dialog_error_list d (this, L"File Open", L"", log);
			d.ShowWindowModal ();
		}
	}

}

bool frame_swat::write_control_record
	(SWATControlRecord *control,
	interface_window *update_display,
	dynamic_string &log)

{
	bool error = false;
	dynamic_string progress;
	_timeb start, end;
	float elapsed_milliseconds;
	SWATControlRecord set_control;
	swat_controller boss;

	_ftime_s (&start);
	update_display->update_progress ("Saving ControlRecord");

	setup_controller (&boss);

	control->remove (db_odbc, control->ID, log);

	set_control.initialize_field_set ();

	if (set_control.field_set->open (db_odbc, false, log)) {
		if (!control->write (db_odbc, &boss, &set_control, update_display, log))
			error = true;

		set_control.field_set->close ();
	}
	else {
		error = true;
		log += "ERROR, unable to open table [Control Records].\n";
	}


	_ftime_s (&end);

	elapsed_milliseconds = ((float) end.time * 1000.0f + (float) end.millitm) - ((float) start.time * 1000.0f + (float) start.millitm);
	progress.format ("Elapsed: %.3f seconds\n", elapsed_milliseconds / 1000.0f);
	log += progress;

	return !error;
}

void frame_swat::OnExportInput
	(wxCommandEvent&)

// Save table data to binary or CSV file for import to MySQL

{
	dynamic_string path, log;
	swat_controller boss;
	bool binary = false, cancel = false;

	if (binary) {
		wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "Data Files (*.dat)|*.dat", wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 
		if ( openFileDialog->ShowModal() == wxID_OK ) {
			path = openFileDialog->GetDirectory().ToAscii ();
			path += PATH_SEPARATOR;
		}
		else
			cancel = true;
	}
	else {
		wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "Data Files (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 
		if ( openFileDialog->ShowModal() == wxID_OK ) {
			path = openFileDialog->GetDirectory().ToAscii ();
			path += PATH_SEPARATOR;
		}
		else
			cancel = true;
	}

	setup_controller (&boss);

	if (!cancel) {
		SetCursor (*wxHOURGLASS_CURSOR);

		boss.export_input (db_odbc, path, binary, log);
	}
	log.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Export Input Data", L"", log);
	d.ShowWindowModal ();
}

void frame_swat::OnImport
	(wxCommandEvent&)

// Read table data (binary or CSV file) for into MySQL

{
	dynamic_string path, log;
	swat_controller boss;
	bool binary = false, cancel = false;

	if (binary) {
		wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "Data Files (*.dat)|*.dat", wxFD_OPEN, wxDefaultPosition);
 
		if ( openFileDialog->ShowModal() == wxID_OK ) {
			path = openFileDialog->GetDirectory().ToAscii ();
			path += PATH_SEPARATOR;
		}
		else
			cancel = true;
	}
	else {
		wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "Data Files (*.csv)|*.csv", wxFD_OPEN, wxDefaultPosition);
 
		if ( openFileDialog->ShowModal() == wxID_OK ) {
			path = openFileDialog->GetDirectory().ToAscii ();
			path += PATH_SEPARATOR;
		}
		else
			cancel = true;
	}

	setup_controller (&boss);

	if (!cancel) {
		SetCursor (*wxHOURGLASS_CURSOR);

		interface_window_wx view;
		view.start_progress (this);

		log += "Importing tables from \"";
		log += path;
		log += "\".\n";

		boss.import_tables (db_odbc, path, binary, &view, log);
	}
	log.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Import Data", L"", log);
	d.ShowWindowModal ();
}

void frame_swat::OnReportFertilizer
	(wxCommandEvent &)

// Summarize [Management] fertilizer by HRU

{
	dynamic_string report;
	std::vector <SubBasin>::const_iterator sub;
	std::vector <HRU_record>::const_iterator hru;
	SWATControlRecord control;
	std::vector <SWATManagement>::const_iterator op;
	int id;
	long hru_key;
	std::map <long, double> applied_in_hru;
	std::map <long, double>::iterator applied;
	std::map <long, double> *fertilizer_in_hru;
	std::map <long, std::map <long, double>> total_applied;
	std::map <long, std::map <long, double>>::iterator hru_amount;
	std::map <long, int> years_applied;
	std::map <long, int>::iterator hru_years;
	SWATFertilizer *fert;

	if ((id = list_control->GetSelectedID ()) != 0) {
		if (control.read (db_odbc, id, false, NULL, report)) {

			// N at outlet
			for (sub = control.subbasins.basins.begin ();
			sub != control.subbasins.basins.end ();
			++sub) {
				for (hru = sub->hru_list.hrus.begin ();
				hru != sub->hru_list.hrus.end ();
				++hru) {
					hru_key = hru->Subbasin * 10000 + hru->HRU;
					applied_in_hru.clear ();

					for (op = hru->operations.ops.begin ();
					op != hru->operations.ops.end ();
					++op) {
						if (op->OperationCode == SWAT_OP_CODE_FERTILIZE) {
							if ((applied = applied_in_hru.find (op->Parameter2)) != applied_in_hru.end ())
								applied->second += (double) op->Parameter3;
							else
								applied_in_hru.insert (std::pair <long, double> (op->Parameter2, (double) op->Parameter3));
						}
					}
					if (applied_in_hru.size () > 0) {
						total_applied.insert (std::pair <long, std::map <long, double>> (hru_key, applied_in_hru));
						years_applied.insert (std::pair <long, int> (hru_key, hru->RotationLength));
					}
				}
			}

			report += "Subbasin\tHRU\tFertilizer\tApplied, kg/ha\tRotation Length\tApplied per year\tFertilizer Name\n";

			for (hru_amount = total_applied.begin (), hru_years = years_applied.begin ();
			(hru_amount != total_applied.end ()) && (hru_years != years_applied.end ());
			++hru_amount, ++hru_years) {
	
				fertilizer_in_hru = &hru_amount->second;
				for (applied = fertilizer_in_hru->begin ();
				applied != fertilizer_in_hru->end ();
				++applied) {
					report.add_formatted ("%ld\t%ld", hru_amount->first / 10000, hru_amount->first % 10000);
					
					report.add_formatted ("\t%d\t%.2lf", applied->first, applied->second);
					report.add_formatted ("\t%d\t%.2lf", hru_years->second, applied->second / hru_years->second);

					report += "\t";
					if ((fert = fertilizers.match (applied->first)) != NULL)
						report += fert->Name;

					report += "\n";
				}
			}
		}
	}

	report.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Pollutants Report", L"", report);
	d.ShowWindowModal ();

}

void frame_swat::OnReportFlow
	(wxCommandEvent &)

// requires Reach daily output 

{
	dynamic_string report;
	std::vector <SWATOutputReachAnnual>::const_iterator reach_year;
	std::vector <SWATOutputReachDaily>::const_iterator reach_day;
	std::map <long, swat_output_reach>::const_iterator output_sub;
	SWATControlRecord control;
	int count_days_above;
	long count_all_days;
	int id;
	int *days_per_year, year_index, first_year;
	double *annual_n, *annual_p, *annual_sediment;
	double FlowDays, FlowDailyAverage, N, P, Sediment;
	long outlet_subbasin; //  = 16; // Cottonwood
    
	if ((id = list_control->GetSelectedID ()) != 0) {
		if (control.read (db_odbc, id, true, NULL, report)) {

			outlet_subbasin = control.routing.find_outlet_subbasin (report);

			annual_n = new double [control.RunLength - control.SkipYears];
			annual_p = new double [control.RunLength - control.SkipYears];
			annual_sediment = new double [control.RunLength - control.SkipYears];
			days_per_year = new int [control.RunLength - control.SkipYears];
			first_year = control.FirstYear + control.SkipYears;

			memset (annual_n, 0, sizeof (double) * (control.RunLength - control.SkipYears));
			memset (annual_p, 0, sizeof (double) * (control.RunLength - control.SkipYears));
			memset (annual_sediment, 0, sizeof (double) * (control.RunLength - control.SkipYears));
			memset (days_per_year, 0, sizeof (int) * (control.RunLength - control.SkipYears));

			FlowDays = 0.0;
			FlowDailyAverage = N = P = Sediment = 0.0;

			// N at outlet
			count_days_above = count_all_days = 0;
			for (output_sub = control.output.reach_map.begin ();
			output_sub != control.output.reach_map.end ();
			++output_sub) {
				if (output_sub->second.Reach == outlet_subbasin) {
					for (reach_day = output_sub->second.day_list.begin ();
					reach_day != output_sub->second.day_list.end ();
					++reach_day) {
						if (reach_day->Year >= control.FirstYear + control.SkipYears) {
							FlowDailyAverage += reach_day->FlowOut;
							++count_all_days;
							if (reach_day->FlowOut > 33.7)
								++count_days_above;

							annual_n [reach_day->Year - first_year] += (double) reach_day->OrganicNOut + (double) reach_day->NO3Out + (double) reach_day->NH4Out + (double) reach_day->NO2Out;
							annual_p [reach_day->Year - first_year] += (double) reach_day->OrganicPOut + (double) reach_day->MineralPOut;
							annual_sediment [reach_day->Year - first_year] += (double) reach_day->SedimentOut;

							days_per_year [reach_day->Year - first_year] += 1;
						}
					}
				}
			}

			if (count_all_days > 0) {
				FlowDays = double (count_days_above);
				FlowDailyAverage /= (double) count_all_days;
			}

			for (year_index = 0; year_index < (control.RunLength - control.SkipYears); ++year_index) {
				if (days_per_year [year_index] > 0) {
					annual_n [year_index] /= (double) days_per_year [year_index];
					annual_p[year_index] /= (double)days_per_year[year_index];
					annual_sediment[year_index] /= (double)days_per_year[year_index];
				}
				N += annual_n [year_index];
				P += annual_p [year_index];
				Sediment += annual_sediment [year_index];
			}

			if (control.RunLength - control.SkipYears > 0) {
				N /= double (control.RunLength - control.SkipYears);
				P /= double (control.RunLength - control.SkipYears);
				Sediment /= double (control.RunLength - control.SkipYears);
			}

			report += "Subbasin\tFlow > 33.7 m^3/sec\tDaily Ave Flow\tN\tP\tSediment\n";

			report.add_formatted ("%ld\t%.4lf\t%.4lf\t%.4lf\t%.4lf\t%.4lf\n", outlet_subbasin, FlowDays, FlowDailyAverage, N, P, Sediment);

			delete [] days_per_year;
			delete [] annual_n;
			delete [] annual_p;
			delete [] annual_sediment;
		}
	}

	report.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Pollutants Report", L"", report);
	d.ShowWindowModal ();

}

void frame_swat::OnImportCIO
	(wxCommandEvent &)
{
	dynamic_string filename;
	dynamic_string error_message;
	SWATControlRecord *new_record;
	swat_controller boss;
	long new_id;
	BYTE read_weather = SWAT_IMPORT_HISTORICAL_WEATHER_NORMAL;
	bool save_hru_fraction = false; // Save HRU area fraction as a string in description

	setup_controller (&boss);

	if (db_odbc) {
		filename = "file.cio";
		new_id = 1;

		dialog_import_cio d (this, &filename, &new_id, &read_weather, &boss.swat_version, &boss.swat_revision, &save_hru_fraction);

		if (d.ShowModal () == wxID_OK) {

			interface_window_wx views;
			views.start_progress (this);

			new_record = new SWATControlRecord;
			// new_record->copy (&set_control);
			control_records.push_back (new_record);

			if (new_record->read_swat_cio (filename.get_text_ascii (), &boss,
			new_id, &soils, &swat_weather, &septic_wqs, read_weather, save_hru_fraction, &views,
			error_message)) {

				// control.write (error_message, &update_display);

				views.update_progress ("Writing");
				if (write_control_record (new_record, &views, error_message)) {
#ifdef _ODBC_TDC
					swat_weather.write (db_odbc, &views, error_message);
					soils.write (db_odbc, &views, error_message);
					boss.tillages->write (db_odbc, error_message);
					pesticides.write (db_odbc, error_message);
					boss.fertilizers->write (db_odbc, error_message);
					boss.crops->write (db_odbc, error_message);
					septic_wqs.write (db_odbc, error_message);
					boss.urbans->write (db_odbc, error_message);
#else
					swat_weather.write (database, &views, error_message);
					soils.write (database, &views, error_message);
					tillages.write (database, error_message);
					pesticides.write (database, error_message);
					fertilizers.write (database, error_message);
					crops.write (database, error_message);
					septic_wqs.write (database, error_message);
					urbans.write (database, error_message);
#endif
				}
			}
		}
	}
	else
		error_message += "ERROR, no database open.\n";

	error_message.convert_linefeeds_for_CEdit ();
	dialog_error_list de (this, L"i_SWAT import", L"", error_message);
	de.ShowModal ();

	list_control->Update ();
}

bool build_job_list
	(const long first_number,
	const long last_number,
	odbc_database* db_odbc,
	swat_controller* outside_controller,
	std::vector <SWATControlRecord> *control_records,
	std::map <long, thread_work*> *jobs,
	interface_window* update_display,
	dynamic_string& log)

// one job per control record
// 2022-06-01 i_SWAT uses \cpp\i_epic_database\thread_class and \cpp\i_swat_database\thread_swat_run
// GA and other newer programs use thread_manager and thread_work, so use those here

{
	dynamic_string progress;
	int run_number;
	thread_work_swat *job;
	bool error = false;
	run_number = 0;
	std::vector <SWATControlRecord>::iterator control_record;

	// read all control records, all subbasins, all hrus
	for (control_record = control_records->begin ();
	control_record != control_records->end ();
	++control_record) {
		++run_number;
		if ((run_number >= first_number)
		&& (run_number <= last_number)) {
			job = new thread_work_swat;
			job->id = run_number;
			job->description.format("SWAT Run %ld ID %ld", run_number, control_record->ID);
			job->view = update_display;
			job->control = &*control_record;
			job->boss = new swat_controller;
			job->boss->copy(outside_controller);

			if ((outside_controller->run_method == SWAT_RUN_METHOD_DLL)
			|| (outside_controller->run_method == SWAT_RUN_METHOD_HRU))
				job->common = job->boss->create_common_data(job->control, log);
			else
				job->common = NULL;

			job->boss->stop_on_run_errors = outside_controller->stop_on_run_errors;
			job->boss->pre_swat_sleep = outside_controller->pre_swat_sleep;
			job->work_function = work_run_swat;
			job->write_input_files = SWAT_WRITE_BASE_FILES_WITH_RUN;
			// thread_work_swat shouldn't write input files if write_base_files is called below

			jobs->insert(std::pair <long, thread_work*>(job->id, job));
		}
	}

	return !error;
}

void frame_swat::OnRunSWATAll
	(wxCommandEvent&)

{
	dynamic_string log;
	bool error = false;
	swat_controller boss;
	interface_window_wx view;
	dynamic_string status_text;
	SWATControlRecord control;
	std::map <long, thread_work*> jobs;
	thread_manager threader;
	thread_manager_statistics thread_statistics;
	std::vector <SWATControlRecord> run_control_records;

	// _ftime_s (&start);

	view.start_progress(this);

	setup_controller (&boss);

	if ((boss.run_method == SWAT_RUN_METHOD_DLL)
	|| (boss.run_method == SWAT_RUN_METHOD_HRU)) {
		boss.path_historical_weather_and_atmos = app->path_historical_weather;
		log += "Weather Path \"";
		log += boss.path_historical_weather_and_atmos;
		log += "\"\n";

		if (boss.historical_weather_method == SWAT_HISTORICAL_WEATHER_DIRECT_FILE) {
			// 2022-05-26 Second SWAT run must either not reread historical weather, or clear before reading
			swat_weather.historical_data.clear();

			// read once from files for DLL array, don't write to database
			if (!boss.swat_weather->preread_historical_weather(boss.path_historical_weather_and_atmos, &control, &view, log))
				error = true;
		}
	}

	boss.read_all (&run_control_records, db_odbc, false, &view, log);
	build_job_list (1, 5, db_odbc, &boss, &run_control_records, &jobs, &view, log);

	// ((app_i_swat *) AfxGetApp ())->whereis_swat [((app_i_swat *) AfxGetApp ())->SWATVersion - 1].write_path (&working_path);
	if ((boss.run_method != SWAT_RUN_METHOD_DLL)
	&& (boss.run_method != SWAT_RUN_METHOD_HRU)) {
		status_text = "Writing SWAT input files to \"";
		status_text += boss.run_folder;
		status_text.add_formatted ("\" for ID %ld\n", boss.run_folder.get_text (), control.ID);
	}
	else
		status_text.add_formatted ("Running SWAT DLL, ID %ld", control.ID);

	// ((CMainFrame *) AfxGetMainWnd ())->m_wndStatusBar.SetWindowText (status_text.get_text ());
	view.update_status_bar (&status_text);
	
	boss.write_run_settings (log);

	view.update_progress("Running SWAT.");

	threader.statistics = &thread_statistics;
	thread_statistics.reset (app->number_of_threads); //app->run_folders.size());
	threader.thread_count = app->number_of_threads;
	// threader.maximum_restart_count = maximum_restart_count;
	view.set_data(&threader); // sends DIALOG_DATA_POINTER to message_slot in dialog_run_threads
	threader.run(&jobs, &view, log);

	if ((boss.run_method != SWAT_RUN_METHOD_DLL)
	&& (boss.run_method != SWAT_RUN_METHOD_HRU))
		thread_statistics.write_time_summary (&app->run_folders, log);

	view.destroy_dialog_progress ();

	{
		std::map <long, thread_work*>::iterator job;
		for (job = jobs.begin ();
		job != jobs.end ();
		++job) {
			if ((((thread_work_swat *) job->second)->boss->run_method == SWAT_RUN_METHOD_DLL)
			|| (((thread_work_swat *) job->second)->boss->run_method == SWAT_RUN_METHOD_HRU))
				delete ((thread_work_swat*)job->second)->common;

			delete ((thread_work_swat*)job->second)->boss;
		}
	}


	log.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"SWAT run", L"", log);
	d.ShowWindowModal ();
}

void frame_swat::OnImportHistoricalWeather
	(wxCommandEvent&)

// 2017-03-01 Read PCP & TMP files for UMRB to pass to swat2012 DLL function.
// Don't write them to database

{
	dynamic_string log, rewrite_folder;
	interface_window_wx views;
	swat_controller boss;
	dynamic_string path;
	SWATControlRecord control;
	filename_struct path_struct;
	BYTE RainfallRunoffRouting = 2; // 1=daily, 2=subdaily
	long id;

	// _ftime_s (&start);

	if ((id = list_control->GetSelectedID ()) != 0) {
		SetCursor (*wxHOURGLASS_CURSOR);
		control.read (db_odbc, id, false, NULL, log);

		wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "Precip Files (*.pcp)|*.pcp", wxFD_OPEN, wxDefaultPosition);
 
		if (openFileDialog->ShowModal () == wxID_OK) {
			views.start_progress (this);
			path += openFileDialog->GetPath ().ToAscii ();
			path_struct.parse (&path);

			setup_controller (&boss);
			boss.swat_weather->clear_weather_filenames ();
			boss.swat_weather->number_of_precipitation_files = 1;
			boss.swat_weather->precip_filenames.push_back (path);

			path_struct.set_filename ("tmp1.tmp");
			path_struct.write_file_specification (&path);
			boss.swat_weather->number_of_temperature_files = 1;
			boss.swat_weather->temperature_filenames.push_back (path);

			{
				// 2023-09-11 PCP weather reformatting
				boss.swat_weather->number_of_temperature_files = 0;
				boss.swat_weather->temperature_filenames.clear ();
			}

			boss.swat_weather->read_all_weather_files (RainfallRunoffRouting, &views, log);

			boss.historical_weather_method = SWAT_HISTORICAL_WEATHER_WRITE;
			boss.run_method = SWAT_RUN_METHOD_HIDDEN;
			path_struct.write_path(&rewrite_folder);
			rewrite_folder += "Rewrite\\";

			log += rewrite_folder;

			boss.swat_weather->write_run_files_weather(rewrite_folder, &control, boss.historical_weather_method, boss.run_method, SWAT_MAX_HISTORICAL_STATIONS_PER_FILE,
			SWAT_MAX_HISTORICAL_STATIONS_PER_FILE, RainfallRunoffRouting, 0, 0, 0, false, log);

			views.destroy_dialog_progress ();
		}
		SetCursor (wxNullCursor);

		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"SWAT run", L"", log);
		d.ShowWindowModal ();
	}
}

void frame_swat::OnInsertPointSource
	(wxCommandEvent &)
{
	dynamic_string log;
	swat_routing_node_reccnst *reccnst;
	swat_routing_node_add *new_add;
	swat_routing_node *subbasin, *subbasin_route;
	long target_subbasin;
	long selected_id, insert_hsl;
	long file_number;
	SWATControlRecord control_record;

	// 2017-05-09 If this is called more than once or on a watershed that already has a RECCNST command,
	// need to traverse routing and find highest RECCNST file number so that file_number will be unique
	file_number = 1;

	if (db_odbc) {
		interface_window_wx views;
		views.start_progress (this);
		// target_subbasin = 4; // Boone 5 doesn't have a ROUTE command
		target_subbasin = 15; // Boone 5 doesn't have a ROUTE command

		if ((selected_id = list_control->GetSelectedID ()) != 0) {
			SetStatusText( _("Reading control") );
			if (control_record.read (db_odbc, selected_id, false, NULL, log)) {

				control_record.routing.describe (log);

				// Existing: (4: Sub 4) -> (37: Route to Reach 4) -> (38: Add 5+37)
				// Insert (37: RECCNST) -> (38: Add 4 + 37) -> (39: Route to Reach 4) -> (40: Add 5+39)

				subbasin = control_record.routing.match_subbasin_gis (target_subbasin);
				subbasin_route = control_record.routing.outlet->get_descendent (subbasin);
				insert_hsl = subbasin_route->HydrographStorageLocation;
				{
					// New reccnst and add will use to HSL #s starting at subbasin_route
					// Increment every HSL >= subbasin_route->HydrographStorageLocation by 2
					routing_environment_data environment;

					log.add_formatted ("Renumber HSL >= %ld += 2\n", insert_hsl);
					environment.mission = ROUTING_MISSION_RENUMBER_HSL;
					environment.target_hsl = insert_hsl;
					environment.log = &log;

					control_record.routing.outlet->traverse (NULL, &environment, 0);
				}


				reccnst = new swat_routing_node_reccnst;

				reccnst->point_source.ID = 1;
				reccnst->point_source.HydrographStorageLocation = insert_hsl;
				reccnst->point_source.NO3 = 25.0f;
				reccnst->point_source.MineralP = 2.5f;

				reccnst->Command = SWAT_ROUTING_COMMAND_RECCONST;
				sprintf_s (reccnst->Filename1, SWAT_ROUTING_FILENAME_LENGTH, "%ldp.dat", subbasin->Parameter5);
				reccnst->HydrographStorageLocation = insert_hsl;
				reccnst->Parameter1 = file_number; // unique reccnst file number
				reccnst->CommandNumber = reccnst->HydrographStorageLocation;

				new_add = new swat_routing_node_add;
				new_add->Command = SWAT_ROUTING_COMMAND_ADD;
				new_add->HydrographStorageLocation = reccnst->HydrographStorageLocation + 1;
				new_add->CommandNumber = new_add->HydrographStorageLocation;
				new_add->Source1 = subbasin;
				new_add->Parameter1 = subbasin->HydrographStorageLocation;
				new_add->Source2 = reccnst;
				new_add->Parameter2 = reccnst->HydrographStorageLocation;

				// Change route source from subbasin to new_add
				ASSERT (subbasin_route->Parameter1 == subbasin->HydrographStorageLocation);
				((swat_routing_node_route *) subbasin_route)->Source = new_add;
				subbasin_route->Parameter2 = new_add->HydrographStorageLocation;

				log += "\n";
				control_record.routing.describe (log);

				views.update_progress ("Writing");
				if (write_control_record (&control_record, &views, log)) {
				}
			}
		}
	}
	else
		log += "ERROR, no database open.\n";

	log.convert_linefeeds_for_CEdit ();
	dialog_error_list de (this, L"i_SWAT add point source", L"", log);
	de.ShowModal ();

	list_control->Update ();
}

void frame_swat::OnRecordPrevious (wxCommandEvent &)

{
	if (db_odbc)
		list_control->previous_record ();
}

void frame_swat::OnRecordNext (wxCommandEvent &)

{
	if (db_odbc)
		list_control->next_record ();
}

void frame_swat::OnReadOutputHRU (wxCommandEvent &)

// Read an 'unattached' OUTPUT.HRU file and show results

{
	dynamic_string log, path;
	swat_controller boss;
	bool error = false;
	SWATControlRecord file_control_record;
	std::vector <long> subbasin_routing_order;
	std::vector <swat_output_hru>::const_iterator output_hru;
	std::vector <SWATOutputHRUAnnual>::const_iterator output_year;
	long id;
	int target_hru;

	target_hru = 2229;

	wxFileDialog* openFileDialog = new wxFileDialog (this, _ ("Open file"), "", "", "SWAT HRU Output Files (*.HRU)|*.HRU", wxFD_OPEN, wxDefaultPosition);

	if (openFileDialog->ShowModal () == wxID_OK) {
		path = openFileDialog->GetDirectory ().ToAscii ();
		path += PATH_SEPARATOR;
		path += openFileDialog->GetFilename ().ToAscii ();

		interface_window_wx update_display;
		update_display.start_progress (this);

		if (db_odbc
		&& ((id = list_control->GetSelectedID ()) != 0)) {
			setup_controller (&boss);

			boss.swat_version = SWAT_VERSION_2009;
			boss.swat_revision = 670;
			boss.write_output_hru_annual = true;
			file_control_record.PrintCode = 2; // Annual

			file_control_record.read (db_odbc, id, false, NULL, log);
			file_control_record.routing.make_subbasin_list (&subbasin_routing_order);
			if (!file_control_record.output.read_swat_sbs (path.get_text_ascii (), boss.swat_version, boss.swat_revision, boss.icalen,
				boss.write_output_hru_annual, boss.write_output_hru_monthly, boss.write_output_hru_daily,
				&file_control_record, &subbasin_routing_order, &boss, &update_display, log))
				error = true;

			for (output_hru = file_control_record.output.hru_list->begin ();
			output_hru != file_control_record.output.hru_list->end ();
			++output_hru) {
				for (output_year = output_hru->year_list.begin ();
				output_year != output_hru->year_list.end ();
				++output_year) {
					log.add_formatted ("%ld\t%ld\t%d\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf", output_year->HRU, output_year->Crop,
					output_year->Precipitation, output_year->Snowfall, output_year->SnowMelt, output_year->Irrigation, output_year->PotentialEvapotranspiration);

					log += "\n";
				}
			}

		}
		else {
			// No database open, just read & echo the text with tab delimiters

			boss.output_variables = output_variables;

			boss.swat_version = SWAT_VERSION_2009;
			boss.swat_revision = 670;
			boss.write_output_hru_annual = true;

			if (!file_control_record.output.read_swat_sbs_detached (path.get_text_ascii (), target_hru, boss.swat_version, boss.swat_revision, boss.icalen,
			output_variables, &update_display, log))
				error = true;

		}

		update_display.destroy_dialog_progress ();
	}
	log.convert_linefeeds_for_CEdit ();
	dialog_error_list de (this, L"i_SWAT add point source", L"", log);
	de.ShowModal ();
}

#ifdef _MRB_DOUBLE_RUN

void frame_swat::OnMRBDoubleRun
	(wxCommandEvent&)

// 2021-02-10 Run SWAT, alter point sources, rerun SWAT

{
	dynamic_string log, filename_db_mosm_area_weights;
	swat_controller boss;
	dynamic_string status_text;
	filename_struct path_struct;
	bool run_success = false, error = false;
	SWATControlRecord control;
	FortranCommonData *common;
	long id, pasted_id;
	bool verbose = true;
	long outlet_subbasin = 501;
	int genome[MRB_GENOME_LENGTH], index;

	for (index = 0; index < MRB_GENOME_LENGTH; ++index)
		genome[index] = 1;

	filename_db_mosm_area_weights = "R:\\SWAT\\MinnesotaRiver\\2019-01-23\\MRB Wetland Costs.accdb";

	if ((id = list_control->GetSelectedID ()) != 0) {
		SetCursor (*wxHOURGLASS_CURSOR);
		control.read (db_odbc, id, false, NULL, log);
		SetCursor (wxNullCursor);

		if (control.check (app->SWATVersion, app->historical_weather_method, &soils, &swat_weather, &tillages, &fertilizers, &pesticides, log)) {
			if (control.PrintCode == SWAT_PRINTCODE_DAILY) {
				dialog_paste_genome dpg (this, genome, &pasted_id, &control);
				if (dpg.ShowModal () == wxID_OK) {
					list_point_source_lookup *point_source_lookup = new list_point_source_lookup;
					list_RavineAreas *ravine_areas = new list_RavineAreas;

					interface_window_wx views;
					views.start_progress (this);

					setup_controller (&boss);

					swat_routing_node *node_459 = control.routing.find_node_below_subbasin (459, SWAT_ROUTING_COMMAND_ADD);
					swat_routing_node *node_outlet = control.routing.find_node_below_subbasin (outlet_subbasin, SWAT_ROUTING_COMMAND_ADD);

					if (!read_point_sources_and_ravine_areas (filename_db_mosm_area_weights, point_source_lookup, ravine_areas, &views, log))
						error = true;

					if ((boss.run_method == SWAT_RUN_METHOD_DLL)
					|| (boss.run_method == SWAT_RUN_METHOD_HRU)) {
						views.update_status_bar ("Running 1st SWAT");
						log += "Weather Path \"";
						log += app->path_historical_weather;
						log += "\"\n";

						if (boss.historical_weather_method == SWAT_HISTORICAL_WEATHER_DIRECT_FILE) {
							// 2022-05-26 Second SWAT run must either not reread historical weather, or clear before reading
							swat_weather.historical_data.clear();
							// read once from files for DLL array, don't write to database
							if (!boss.swat_weather->preread_historical_weather (boss.path_historical_weather_and_atmos, &control, &views, log))
								error = true;
						}
						common = boss.create_common_data (&control, log);
					}
					else {
						status_text = "Writing SWAT input files to \"";
						status_text += boss.run_folder;
						status_text.add_formatted ("\" for ID %ld\n", boss.run_folder.get_text (), control.ID);
						views.update_status_bar (&status_text);
						common = NULL;
					}

					if (!error) {
						log += boss.filename_database;
						log += "\n";
						boss.write_run_settings (log);

						run_success = boss.run_swat (&control, common, &views, log);

						if (run_success) {
							views.update_status_bar ("After 1st run");

							control.output.write_std_output (log, false);
							log += "HSL\tFlow m^3\tSediment t\tOrganic Nitrogen kg\tOrganic Phosphorus kg\tNitrate kg\tSoluble Phosphorus kg\tSoluble Pesticide mg\n";
							node_459->write_hydrology_output (false, -1, log);
							node_outlet->write_hydrology_output (false, -1, log);
							log += "\n";

							/*
							Daily runs don't work wtih yield_tracker

							output_yield_tracker yield_tracker;

							yield_tracker.show_crop = true;
							yield_tracker.show_year = false;
							yield_tracker.show_subbasin_crop = false;
							yield_tracker.show_year_crop = false;
							yield_tracker.show_subbasin_year_crop = false;

							if (yield_tracker.accumulate (&control, log))
								yield_tracker.summarize (&crops, log);
							*/

							std::vector <SWATRouting> raw_routing_list;
							std::map <long, double> nc_sediment_by_subbasin_Mg_yr;
							std::map <long, double> RavineLoad_reduction_Mg_yr;
							double evaldata_result[3], evaldata_other[MRB_EVALDATA_OTHER_COUNT];

							memset (evaldata_result, 0, sizeof (double) * 3);
							memset (evaldata_other, 0, sizeof (double) * MRB_EVALDATA_OTHER_COUNT);

							control.routing.build_raw_list (&raw_routing_list);

							flow_to_point_sources (&control, &nc_sediment_by_subbasin_Mg_yr, &raw_routing_list, point_source_lookup, evaldata_other, verbose, log);

							set_ravine_load_reduction (&RavineLoad_reduction_Mg_yr, genome, &control, ravine_areas, point_source_lookup, evaldata_other, log);
							apply_RAMO_reduction (&control, &RavineLoad_reduction_Mg_yr, &nc_sediment_by_subbasin_Mg_yr, &raw_routing_list,
							evaldata_other, verbose, log);

							sum_first_swat (&control, outlet_subbasin, evaldata_other, verbose, log);

							if ((boss.run_method != SWAT_RUN_METHOD_DLL)
							&& (boss.run_method != SWAT_RUN_METHOD_HRU)) {
								// 2019-02-06 Can't get console handle "iSwatxxxxxxxx.con" for 2nd run.  Wait for it
								// Delete swat output
								// delete_swat_output_files (boss.swat_version, boss.use_id, control.ID, boss.imgt == 1, &run_directory, log);

								// 2019-02-07 remove previous version of the FIG file
								remove_file (boss.run_folder, "is.fig", &error, log);

								remove_files (boss.run_folder, "OUT*", &error, log);
							}

							if (write_recday (&control, &boss, boss.run_folder, log)) {
								views.update_status_bar ("Running 2nd SWAT");
								run_success = boss.run_swat (&control, common, &views, log);
								if (run_success) {
									views.update_status_bar ("After 2nd SWAT");
#ifdef _ODBC_TDC
									control.output.write (db_odbc,
#else
									control.output.write (database,
#endif
									&boss, &control, &views, log);
									control.output.write_std_output (log, false);

									// HSL output for reach 459 and outlet
									log += "HSL\tFlow m^3\tSediment t\tOrganic Nitrogen kg\tOrganic Phosphorus kg\tNitrate kg\tSoluble Phosphorus kg\tSoluble Pesticide mg\n";
									node_459->write_hydrology_output (false, -1, log);
									node_outlet->write_hydrology_output (false, -1, log);
									log += "\n";

									sum_MRB (&control, outlet_subbasin, genome, evaldata_result, evaldata_other, verbose, log);

									log += "ID";
									log += "\tLabel";
									log += "\tCost";
									log += "\tReach Outlet NO3 kg/yr";
									log += "\tReach Outlet Sediment tons / yr";

									for (index = 0; index < MRB_EVALDATA_OTHER_COUNT; ++index) {
										log += "\t";
										MRB_EVALDATA_OTHER_NAME[index];
									}
									log += "\n";

									log.add_formatted ("%ld", control.ID);
									log += "\t"; // label
									for (index = 0; index < 3; ++index)
										log.add_formatted ("\t%.6lf", evaldata_result[index]);

									for (index = 0; index < MRB_EVALDATA_OTHER_COUNT; ++index)
										log.add_formatted ("\t%.6lf", evaldata_other[index]);
									log += "\n";

								}
							}

						}
					}
					if ((boss.run_method == SWAT_RUN_METHOD_DLL)
					|| (boss.run_method == SWAT_RUN_METHOD_HRU))
						delete common;

					delete point_source_lookup;
					delete ravine_areas;
				}
			}
			else
				log += "ERROR, Printcode must be 1 (daily).\n";
		}

		log.convert_linefeeds_for_CEdit ();
		dialog_error_list d (this, L"SWAT run", L"", log);
		d.ShowWindowModal ();
	}
}

void frame_swat::OnSubbasinDistanceToOutlet(wxCommandEvent&)

// 2023-09-01 for MRB: create table of each subbasin and the # of other subbasins between it and outlet

// route_hsls, output of accumulate_path, does not include other subbasins, but only the ADD commands within/below each subbasin.
// So a map of subbasins to their nearest ADD node is built first


{
	dynamic_string log, path;
	SWATControlRecord control_record;

	interface_window_wx update_display;
	update_display.start_progress(this);
	update_display.update_progress("Loading");


	if (db_odbc
	&& control_record.read(db_odbc, 1, false, NULL, log)) {
		std::vector <SubBasin>::const_iterator sub;
		std::vector <long> route_hsls;
		std::vector <long>::const_reverse_iterator hsl;
		std::map <long, long> subbasin_by_route;
		std::map <long, long>::const_iterator nearby_subbasin;
		swat_routing_node *subbasin_route;
		int path_length = 0;

		control_record.routing.find_outlet_subbasin(log);

		// First build a map of the routing nodes within/below each subbasin
		for (sub = control_record.subbasins.basins.begin();
		sub != control_record.subbasins.basins.end();
		++sub) {
			subbasin_route = control_record.routing.find_node_below_subbasin (sub->Subbasin, SWAT_ROUTING_COMMAND_ROUTE);
			subbasin_by_route.insert (std::pair <long, long> (subbasin_route->HydrographStorageLocation, sub->Subbasin));
		}

		log += "Subbasin\tHSL";
		log += "\n";

		for (sub = control_record.subbasins.basins.begin();
		sub != control_record.subbasins.basins.end();
		++sub) {

			log.add_formatted("%ld", sub->Subbasin);
			route_hsls.clear ();
			path.clear ();

			if (control_record.routing.outlet->accumulate_path(sub->Subbasin, &route_hsls)) {

				// route_hsls does not include other subbasins, but only the route commands within/below each subbasin.
				path_length = 0;
				for (hsl = route_hsls.rbegin();
				hsl != route_hsls.rend();
				++hsl) {
					// Find nearest subbasin upstream from HSL
					if ((nearby_subbasin = subbasin_by_route.find (*hsl)) != subbasin_by_route.end ()) {
						path.add_formatted("\t%ld", nearby_subbasin->second);
						++path_length;
					}
				}
			}
			log.add_formatted ("\t%d", path_length);
			log += path;
			log += "\n";
		}
	}

	if (log.get_length() > 0) {
		log.convert_linefeeds_for_CEdit();
		dialog_error_list d(this, L"", L"", log);
		d.ShowModal();
	}
}

#endif