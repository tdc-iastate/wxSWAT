
class page_basin_3 : public wxPanel {
	public:
		enum {
			EDIT_HELP = 101,
			BUTTON_WEATHER_SUMMARY
		};
		page_basin_3
			(wxWindow *parent,
			const int id,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		virtual bool TransferDataToWindow ();
		virtual bool TransferDataFromWindow ();

		wxComboBox	*combo_sol_p_model, *combo_iuh, *combo_sub_chsed, *combo_iatmodep;
		float_edit	*edit_ch_d50, *edit_sig_g;

		wxTextCtrl *edit_help;

	protected:
		class SWATControlRecord *control;
		class swat_controller *controller;
		class list_field_help_swat *help;
		class odbc_database *db;

		DECLARE_EVENT_TABLE()
};

