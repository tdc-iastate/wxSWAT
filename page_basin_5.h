class page_basin_5 : public wxPanel {
	// 5
	public:
		enum {
			EDIT_BACT_SWF = 101,
			EDIT_TB_ADJ,
			EDIT_DEPIMP_BSN,
			EDIT_DDRAIN_BSN,
			EDIT_TDRAIN_BSN,
			EDIT_GDRAIN_BSN,
			COMBO_ITDRN,
			COMBO_IWTDN,
			EDIT_CN_FROZ,
			EDIT_DORM_HR,
			EDIT_SMXCO,
			EDIT_FIXCO,
			EDIT_NFIXMX,
			EDIT_HELP
		};
		page_basin_5
			(wxWindow *pParent,
			const int id,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		float_edit *edit_bact_swf;
		float_edit *edit_tb_adj;
		float_edit *edit_depimp_bsn;
		integer_edit *edit_ddrain_bsn;
		integer_edit *edit_tdrain_bsn;
		integer_edit *edit_gdrain_bsn;
		wxComboBox *combo_itdrn;
		wxComboBox *combo_iwtdn;
		float_edit *edit_cn_froz;
		float_edit *edit_dorm_hr;
		float_edit *edit_smxco;
		float_edit *edit_fixco;
		float_edit *edit_nfixmx;
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


