
class dialog_hru : public wxDialog {
	public:
		dialog_hru
			(class wxWindow *,
			class HRU_record *,
			class SubBasin *,
			class SWATControlRecord *,
			class swat_controller *);

		class notebook_hru *notebook;

	protected:
		virtual void EndModal
			(int retCode);
	DECLARE_EVENT_TABLE()
};
