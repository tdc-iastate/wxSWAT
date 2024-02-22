
class dialog_fertilizer : public wxDialog {
public:
	dialog_fertilizer
		(wxWindow* pParent,
		class SWATFertilizer *,
		class list_field_help_swat *);

	float_edit	*edit_fminn, *edit_fminp, *edit_forgn, *edit_forgp, *edit_fnh3n, *edit_bactpdb, *edit_bactlpb, *edit_bactkdb;
	wxTextCtrl	*edit_description;
	wxTextCtrl	*edit_name;
	// edit_help_text edit_help;
		virtual void EndModal
			(int retCode);


protected:
	class SWATFertilizer *fertilizer;
	class list_field_help_swat *help_list;

	// virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	DECLARE_EVENT_TABLE()
};


