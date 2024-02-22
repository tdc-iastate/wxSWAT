
class propsheet_control_record : public wxPropertySheetDialog {
	public:
		propsheet_control_record
			(wxWindow *,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *help_list,
			class odbc_database *db,
			const wxSize &size = wxDefaultSize);

		wxNotebookPage *GetFirstPage() const;
		void OnPageChanged (wxNotebookEvent &);

	protected:

		DECLARE_EVENT_TABLE()
		class page_control_record *page1;
		class page_control_weather *page2;
		class page_control_details *page3;

};
