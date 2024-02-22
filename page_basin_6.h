class page_basin_6 : public wxPanel {
	// 6
	public:

		enum {
			EDIT_ANIONPOROSITYEXCLUSION = 101,
			EDIT_CHANNELORGANICN,
			EDIT_CHANNELORGANICP,
			EDIT_NITROGENGROUNDWATERHALFLIFE,
			EDIT_RAINFALLNITRATECONCENTRATION,
			EDIT_NH3NO2BIOLOGICALOXIDATIONRATECONSTANT,
			EDIT_NO2NO3BIOLOGICALOXIDATIONRATECONSTANT,
			EDIT_NNH4HYDROLYSISRATECONSTANT,
			EDIT_PHOSPHORUSMINERALIZATIONRATECONSTANT,
			EDIT_DAILYRESIDUEDECAYMINIMUM,
			COMBO_CFACTORCALCULATION,
			EDIT_RESIDUECOVERFACTOR,
			EDIT_FLOWCRITICALVELOCITY,
			COMBO_CARBONFUNCTION,
			EDIT_RESERVOIRSEDIMENTSETTLINGCOEFFICIENT,
			EDIT_HELP
		};
		page_basin_6
			(wxWindow *pParent,
			const int id,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		float_edit *edit_anionporosityexclusion;
		float_edit *edit_channelorganicn;
		float_edit *edit_channelorganicp;
		float_edit *edit_nitrogengroundwaterhalflife;
		float_edit *edit_rainfallnitrateconcentration;
		float_edit *edit_nh3no2biologicaloxidationrateconstant;
		float_edit *edit_no2no3biologicaloxidationrateconstant;
		float_edit *edit_nnh4hydrolysisrateconstant;
		float_edit *edit_phosphorusmineralizationrateconstant;
		float_edit *edit_dailyresiduedecayminimum;
		wxComboBox *combo_cfactorcalculation;
		float_edit *edit_residuecoverfactor;
		float_edit *edit_flowcriticalvelocity;
		wxComboBox *combo_carbonfunction;
		float_edit *edit_reservoirsedimentsettlingcoefficient;
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

