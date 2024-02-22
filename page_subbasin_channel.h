
class page_subbasin_channel : public wxPanel {
	public:
		page_subbasin_channel
			(wxWindow *parent,
			const int id,
			class SubBasin *);

		void set_controls ();
		bool read_controls ();

		float_edit *edit_channelaveragewidth;
		float_edit *edit_channeldepth;
		float_edit *edit_channellength;
		float_edit *edit_channelaverageslope;
		float_edit *edit_channelmanningsn;
		float_edit *edit_channelhydraulicconductivity;
		float_edit *edit_channelerodibilityfactor;
		float_edit *edit_channelcoverfactor;
		float_edit *edit_channelwidthdepthratio;
		float_edit *edit_baseflowalphafactor;
		float_edit *edit_sedimentreentrainparameter;
		float_edit *edit_sedimentreentrainexponent;
		wxCheckBox *check_irrigation_canal;
		float_edit *edit_channelorganicn_rte;
		float_edit *edit_channelorganicp_rte;
		wxTextCtrl *edit_help;

	protected:
		class SubBasin *basin;

		DECLARE_EVENT_TABLE()
};

enum {
	EDIT_CHANNELAVERAGEWIDTH = 101,
	EDIT_CHANNELDEPTH,
	EDIT_CHANNELLENGTH,
	EDIT_CHANNELAVERAGESLOPE,
	EDIT_CHANNELMANNINGSN,
	EDIT_CHANNELHYDRAULICCONDUCTIVITY,
	EDIT_CHANNELERODIBILITYFACTOR,
	EDIT_CHANNELCOVERFACTOR,
	EDIT_CHANNELWIDTHDEPTHRATIO,
	EDIT_BASEFLOWALPHAFACTOR,
	EDIT_SEDIMENTREENTRAINPARAMETER,
	EDIT_SEDIMENTREENTRAINEXPONENT,
	CHECK_IRRIGATION_CANAL,
	EDIT_CHANNELORGANICN_RTE,
	EDIT_CHANNELORGANICP_RTE,
	EDIT_HELP
};
