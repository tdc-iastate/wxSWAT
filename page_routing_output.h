
class page_routing_output : public wxPanel {
	friend class propsheet_routing;

	public:
		page_routing_output
			(class wxWindow *,
			const int id,
			class swat_routing_node *,
			class SWATControlRecord *);
		virtual ~page_routing_output();

		enum {
			STATIC_COMMAND_NUMBER = 101,
			EDIT_FLOW,
			EDIT_SEDIMENT,
			EDIT_ORGANIC_N,
			EDIT_ORGANIC_P,
			EDIT_NITRATE,
			EDIT_SOLUBLE_P,
			EDIT_SOLUBLE_PST
		};
		integer_static *static_command_number;
		double_edit *edit_flow, *edit_sediment, *edit_organic_n;
		double_edit *edit_organic_p, *edit_nitrate, *edit_soluble_p, *edit_soluble_pst;

		virtual bool read_controls ()
		{ return true; };
		void SetPrev(wxNotebookPage *prev) { m_prev = prev; };
		void SetNext(wxNotebookPage *next) { m_next = next; };
		DECLARE_EVENT_TABLE()

	protected:
		class swat_routing_node *node;
		class SWATControlRecord *control_record;
		wxNotebookPage *m_prev, *m_next;
};
