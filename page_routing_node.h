
class page_routing_node : public wxPanel {
	friend class propsheet_routing;

public:
	page_routing_node
		(class wxWindow *parent,
		const int id,
		class swat_routing_node *,
		class SWATControlRecord *,
		class swat_controller *controller);

	virtual ~page_routing_node();

		enum { 
			STATIC_COMMAND_NUMBER = 101,
			COMBO_COMMAND,
			EDIT_HSL,
			STATIC_PARAMETER_1,
			EDIT_PARAMETER_1,
			STATIC_PARAMETER_2,
			EDIT_PARAMETER_2,
			STATIC_PARAMETER_3,
			EDIT_PARAMETER_3,
			STATIC_PARAMETER_4,
			EDIT_PARAMETER_4,
			STATIC_PARAMETER_5,
			EDIT_PARAMETER_5,
			EDIT_FILENAME_1,
			EDIT_FILENAME_2,
			BUTTON_RESERVOIR,
			BUTTON_POINT_SOURCE
		};
		wxStaticText *static_command_number;
		wxComboBox *combo_command;
		long_integer_edit *edit_hsl;
		wxStaticText *static_parameter_1;
		long_integer_edit *edit_parameter_1;
		wxStaticText *static_parameter_2;
		long_integer_edit *edit_parameter_2;
		wxStaticText *static_parameter_3;
		long_integer_edit *edit_parameter_3;
		wxStaticText *static_parameter_4;
		float_edit *edit_parameter_4;
		wxStaticText *static_parameter_5;
		long_integer_edit *edit_parameter_5;
		wxTextCtrl *edit_filename_1;
		wxTextCtrl *edit_filename_2;
		wxButton *button_reservoir, *button_point_source;
		void OnReservoir (wxCommandEvent &);
		void OnPointSource (wxCommandEvent &);
		void OnSelChangeCommand (wxCommandEvent &);
		virtual bool read_controls ();
		void SetPrev(wxNotebookPage *prev) { m_prev = prev; };
		void SetNext(wxNotebookPage *next) { m_next = next; };

		DECLARE_EVENT_TABLE()

	protected:
		class swat_routing_node *node;
		class SWATControlRecord *control_record;
		// class list_field_help *help_list;
		short SWATVersion;
		wxNotebookPage *m_prev, *m_next;


};
