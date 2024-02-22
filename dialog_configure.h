
class dialog_configure : public wxDialog {
	public:
		class app_wxswat* app;
		enum {
			ID_FILENAME_OUTPUT_VARIABLES = 103,
			ID_BUTTON_FILENAME_OUTPUT_VARIABLES = 104,
			// ID_EDIT_SWAT_PATH = 105,
			ID_LIST_FILES = 105,
			ID_BUTTON_SWAT_PATH = 106,
			ID_LIST_SWAT_VERSION = 107,
			ID_EDIT_SWAT_REVISION = 108,
			BUTTON_ADD_FOLDER,
			BUTTON_DELETE_FOLDER
		};
		dialog_configure
			(class wxWindow *,
			class app_wxswat *);

		wxButton *button_ok, *button_delete_folder, *button_add_folder, *button_edit_path;
		wxCheckBox *check_output_hru_annual, *check_output_hru_monthly, *check_output_hru_daily,
		*check_output_subbasin_annual, *check_output_subbasin_monthly, *check_output_subbasin_daily,
		*check_output_reach_annual, *check_output_reach_monthly, *check_output_reach_daily, *check_output_mgt, *check_output_hsl, *check_use_id, *check_match_file_precision;
		wxCheckBox *check_input_std, *check_output_std;
		wxTextCtrl *edit_save_daily_reach, *edit_save_daily_hru;

		wxComboBox *combo_swat_version, *combo_historical_weather, *combo_run_method;
		wxTextCtrl *edit_swat_revision;
		listbox_files *list_files;

		void OnEditPath
			(wxCommandEvent &);
		void OnAddFolder
			(wxCommandEvent &);
		void OnDeleteFolder
			(wxCommandEvent &);
		void OnSelChangeVersion
			(wxCommandEvent &);

	protected:

		virtual void EndModal
			(int retCode);
		void show_version_files ();
	DECLARE_EVENT_TABLE()
};
