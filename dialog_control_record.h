
class dialog_control_record : public wxDialog {
	public:
		dialog_control_record
			(class wxWindow *,
			class SWATControlRecord *,
			class swat_controller *);

		class notebook_control_record *notebook;

	protected:
		class SWATControlRecord *control;
		virtual void EndModal
			(int retCode);
	DECLARE_EVENT_TABLE()
};
