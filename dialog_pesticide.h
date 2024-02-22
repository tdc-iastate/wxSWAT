
class dialog_pesticide : public wxDialog {
	public:
		dialog_pesticide
			(wxWindow* pParent,
			class SWATPesticide *pest,
			class list_field_help_swat *);

		float_edit *edit_applicationefficiency;
		float_edit *edit_water_solubility;
		float_edit *edit_washofffraction;
		float_edit *edit_soilabsorptioncoefficient;
		float_edit *edit_henrysconstant;
		float_edit *edit_halflifeonfoliage;
		float_edit *edit_halflifeinsoil;
		wxTextCtrl *edit_description,  *edit_name;
		virtual void EndModal
			(int retCode);

	protected:
		class SWATPesticide *pesticide;
		class list_field_help_swat *help_list;

	DECLARE_EVENT_TABLE()
};

