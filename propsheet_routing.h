
class propsheet_routing : public wxPropertySheetDialog {
	public:
		propsheet_routing
			(class wxWindow *,
			class swat_routing_node *,
			class SWATControlRecord *,
			class swat_controller *);

		void OnPageChanged(wxNotebookEvent&);

	protected:
		class page_routing_node* page1;
		class page_routing_output* page2;
		DECLARE_EVENT_TABLE()
};
