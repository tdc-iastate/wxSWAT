class page_basin_7 : public wxPanel {
	// 7
	public:
		enum {
			COMBO_SOL_P_MODEL = 101,
			EDIT_IABSTR,
			EDIT_BFLO_DIST,
			COMBO_IUH,
			EDIT_UHALPHA,
			EDIT_EROS_SPL,
			EDIT_RILL_MULT,
			EDIT_EROS_EXPO,
			COMBO_SUB_CHSED,
			EDIT_C_FACTOR,
			EDIT_CH_D50,
			EDIT_SIG_G,
			COMBO_IATMODEP,
			EDIT_HELP
		};

		page_basin_7
			(wxWindow *pParent,
			const int id,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		wxComboBox *combo_sol_p_model;
		float_edit *edit_iabstr;
		float_edit *edit_bflo_dist;
		wxComboBox *combo_iuh;
		float_edit *edit_uhalpha;
		float_edit *edit_eros_spl;
		float_edit *edit_rill_mult;
		float_edit *edit_eros_expo;
		wxComboBox *combo_sub_chsed;
		float_edit *edit_c_factor;
		float_edit *edit_ch_d50;
		float_edit *edit_sig_g;
		wxComboBox *combo_iatmodep;
		wxTextCtrl *edit_help;

		virtual bool TransferDataToWindow ();
		virtual bool TransferDataFromWindow ();

	protected:
		class SWATControlRecord *control;
		class swat_controller *controller;
		class list_field_help_swat *help;
		class odbc_database *db;

		DECLARE_EVENT_TABLE()
};

