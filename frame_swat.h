
class frame_swat: public wxFrame {
	public:
		frame_swat
			(const wxString& title,
			const wxPoint& pos,
			const wxSize& size,
			class app_wxswat *);
		virtual ~frame_swat ();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnFileNew(wxCommandEvent& event);
	void OnFileOpen(wxCommandEvent& event);
	void OnOpenRecent (wxCommandEvent& event);
	void OnRunSWAT (wxCommandEvent &);
	void OnRunSWATAll (wxCommandEvent &);
	void OnConfigure (wxCommandEvent &);
	void OnConnect (wxCommandEvent &);
	void OnReportPollutants (wxCommandEvent& e);
	void OnReportFertilizer (wxCommandEvent& e);
	void OnReportFlow (wxCommandEvent& e);
	void OnEditControl (wxCommandEvent &);
	void OnViewRouting (wxCommandEvent &);
	void OnExportInput (wxCommandEvent &);
	void OnCreateTables (wxCommandEvent &);
	void OnImport (wxCommandEvent &);
	void OnReportTillage (wxCommandEvent &);
	void OnImportCIO (wxCommandEvent &);
	void OnMapRouting (wxCommandEvent &);
	void OnTraceHSL (wxCommandEvent &);
	void OnSubbasinDistanceToOutlet(wxCommandEvent&);
	void OnFindOutlet (wxCommandEvent &);
	void OnImportHistoricalWeather(wxCommandEvent &);
	void OnInsertPointSource (wxCommandEvent &);
	void OnAddUpstreamArea (wxCommandEvent &);
	void OnRoutingList (wxCommandEvent &);
	void OnRecordPrevious (wxCommandEvent &);
	void OnRecordNext (wxCommandEvent &);
	void OnReadOutputHRU (wxCommandEvent &);
	void OnMRBDoubleRun(wxCommandEvent &);
	DECLARE_EVENT_TABLE()

	protected:
		class app_wxswat *app;
		wxPanel *m_panel, *panel_control_record;
		class panel_routing_tree *panel_tree;
		class list_control_records *list_control;
	    // class swat_tree_control *tree;
		// class wxListBox *list_subbasins;
		// class notebook_control_record *notebook;
		// int m_numListItems;
		// wxTextCtrl *m_logWindow;
		// wxLog *m_logOld;
		std::vector <SWATControlRecord *> control_records;
		class RecentlyOpenedDatabases *files_recently_opened;
		class RecentlyOpenedShapefiles *recent_shapefiles;

		odbc_database *db_odbc;
		// SWATControlRecord control;
		swat_soil_list soils;
		swat_weather_list swat_weather;
		swat_tillage_list tillages;
		swat_urban_list urbans;
		swat_fertilizer_list fertilizers;
		swat_pesticide_list pesticides;
		swat_crop_list crops;
		swat_septic_water_quality_list septic_wqs;

		class list_output_variable_descriptions_swat *output_variables;

		class list_field_help_swat *help_list;

		class wxBoxSizer *stack;

		void DoSize();

		bool connect_to_odbc
			(class odbc_database_credentials *creds,
			class interface_window *view,
			dynamic_string &log);

		void create_menus ();
		void create_toolbar ();

		// void fill_subbasin_list ();
		bool new_mysql_database
			(dynamic_string &log);
		bool create_tables
			(dynamic_string &log);

		bool read_record_list
			(class interface_window *update_display,
			dynamic_string &error_message);
		bool open_file
			(const dynamic_string &filename,
			class interface_window *view,
			dynamic_string &log);

		bool read_db_start
			(class interface_window *view,
			dynamic_string &log);

		// recreate the list control with the new flags
		void setup_controller
			(class swat_controller *boss);

		bool write_control_record
			(class SWATControlRecord *control,
			class interface_window *update_display,
			dynamic_string &log);
};

enum
{
    ID_Quit = 1,
    ID_About,
	ID_TOOLBAR,
	wxID_REPORT_POLLUTANTS = 110,
	wxID_EDIT_CONTROL_RECORD,
	wxID_VIEW_ROUTING,
	wxID_MAP_ROUTING,
	wxID_FIND_OUTLET,
	wxID_TRACE_HSL,
	wxID_SUBBASIN_DISTANCE_TO_OUTLET,
	wxID_RUN_SWAT,
	wxID_RUN_SWAT_ALL,
	wxID_CONFIGURE,
	wxID_CONNECT,
	wxID_EXPORTINPUT,
	wxID_IMPORT,
	wxID_CREATE_TABLES,
	wxID_REPORT_TILLAGE,
	wxID_REPORT_FERTILIZER,
	wxID_REPORT_FLOW,
	wxID_IMPORT_CIO,
	wxID_IMPORT_HISTORICAL,
	wxID_INSERT_POINT_SOURCE,
	wxID_ADD_UPSTREAM,
	wxID_ROUTING_LIST,
	wxID_RECORD_PREVIOUS,
	wxID_RECORD_NEXT,
	wxID_READ_OUTPUT_HRU,
	wxID_MRB_DOUBLE_RUN
};
