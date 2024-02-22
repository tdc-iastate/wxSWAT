

class notebook_routing : public wxNotebook {
	public:
		notebook_routing
			(wxWindow *,
			class swat_routing_node *,
			class SWATControlRecord *,
			class swat_controller *controller,
			const wxSize &size = wxDefaultSize);

		wxNotebookPage *GetFirstPage() const;
		void OnPageChanged (wxNotebookEvent &);

	protected:
		void add_extra_buttons ();

	private:
		DECLARE_EVENT_TABLE()
		class page_routing_node *page1;
		class page_routing_output *page2;
};
