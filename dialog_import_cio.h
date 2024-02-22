
class dialog_import_cio : public wxDialog {
	public:
		enum {
			BUTTON_BROWSE_INPUT_FILE = 101
		};
		dialog_import_cio
			(wxWindow *,
			dynamic_string *p_filename_cio,
			long *new_record_id,
			// long *new_outlet_hsl,
			BYTE *p_read_weather,
			short *p_swat_version,
			int *p_swat_revision,
			bool *p_save_exact_fraction);

		wxStaticText *static_cio_filename;
		class long_integer_edit *edit_record_id;
		wxComboBox *combo_swat_version, *combo_swat_revision;
		wxComboBox *combo_read_weather;
		wxCheckBox *check_save_fraction;

		void OnBrowseInputFile(wxCommandEvent &);
		void EndModal (int retCode);

	protected:
		dynamic_string *filename_cio;
		long *record_id;
		short *swat_version;
		int *swat_revision;
		BYTE *read_all_weather;
		bool *save_exact_fraction;

	DECLARE_EVENT_TABLE()
};

