

class propsheet_hru : public wxPropertySheetDialog {
	public:
		propsheet_hru
			(wxWindow *,
			class HRU_record *,
			class SubBasin *,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *,
			class odbc_database *,
			const wxSize &size = wxDefaultSize);

	// 	wxNotebookPage *GetFirstPage() const;
		void OnPageChanged (wxBookCtrlEvent &);

	protected:
		void add_extra_buttons ();

	private:
		DECLARE_EVENT_TABLE()
		class page_hru *page1;
		class page_hru_2 *page2;
		class page_hru_3 *page3;
		class page_hru_gw *page4;
		class page_hru_management *page5;
		class page_hru_output *page6;
		class page_hru_pothole *page7;
		class page_hru_tcp *page8;
};
