

class propsheet_basin : public wxPropertySheetDialog {
	public:
		propsheet_basin
			(wxWindow *,
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
		class page_basin_1 *page1;
		class page_basin_2 *page2;
		class page_basin_3 *page3;
		class page_basin_4 *page4;
		class page_basin_5 *page5;
		class page_basin_6 *page6;
		class page_basin_7 *page7;

};
