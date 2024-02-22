class page_basin_4 : public wxPanel {
	// 4
	public:
		page_basin_4
			(wxWindow *pParent,
			const int id,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		integer_edit *edit_ised_det;
		wxComboBox *combo_irte;
		float_edit *edit_msk_co1;
		float_edit *edit_msk_co2;
		float_edit *edit_msk_x;
		wxCheckBox *check_channel_degradation_updating;
		float_edit *edit_trnsrch;
		float_edit *edit_evrch;
		wxComboBox *combo_pesticide;
		wxComboBox *combo_icn;
		float_edit *edit_cncoef;
		float_edit *edit_cdn;
		float_edit *edit_sdnco;
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

enum {
	EDIT_ISED_DET = 101,
	COMBO_IRTE,
	EDIT_MSK_CO1,
	EDIT_MSK_CO2,
	EDIT_MSK_X,
	CHECK_CHANNEL_DEGRADATION_UPDATING,
	EDIT_TRNSRCH,
	EDIT_EVRCH,
	COMBO_PESTICIDE,
	COMBO_ICN,
	EDIT_CNCOEF,
	EDIT_CDN,
	EDIT_SDNCO,
	EDIT_HELP
};
