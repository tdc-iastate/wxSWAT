
class dialog_subbasin : public wxDialog {
	public:
		dialog_subbasin
			(class wxWindow *,
			class SubBasin *,
			class SWATControlRecord *,
			class swat_controller *);

		class notebook_subbasin *notebook;

	protected:
		virtual void EndModal
			(int retCode);
	DECLARE_EVENT_TABLE()
};
