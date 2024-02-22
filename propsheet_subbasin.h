

class propsheet_subbasin : public wxPropertySheetDialog {
	public:
		propsheet_subbasin
			(wxWindow *,
			class SubBasin *basin,
			class SWATControlRecord *,
			class swat_controller *controller,
			class list_field_help_swat *p_help,
			class odbc_database *p_db,
			const wxSize &size = wxDefaultSize);

		// wxNotebookPage *GetFirstPage() const;
		void OnPageChanged (wxNotebookEvent &);

	protected:
		void add_extra_buttons ();

	private:
		DECLARE_EVENT_TABLE()
		class page_subbasin *page1;
		class page_subbasin_channel *page2;
		class page_subbasin_climate *page3;
		class page_subbasin_elevation *page4;
		class page_subbasin_tributary *page5;

};
