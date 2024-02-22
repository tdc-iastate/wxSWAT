
class dialog_edit_management : public wxDialog {
public:
	dialog_edit_management
		(wxWindow *pParent,
		class SWATManagement *manage,
		class swat_controller *p_boss,
		class list_field_help_swat *,
		class odbc_database *);
		enum {
		    BUTTON_PARAMETER_2 = 103,
			BUTTON_PARAMETER_6,
			COMBO_TYPE
		};

	wxStaticText *static_mgt_1, *static_mgt_2, *static_mgt_3, *static_mgt_4, *static_mgt_5, *static_mgt_6, *static_mgt_7, *static_mgt_8, *static_mgt_9;
	wxButton *button_parameter_2, *button_parameter_6;
	float_edit	*edit_parameter_9;
	float_edit	*edit_parameter_8;
	float_edit	*edit_parameter_7;
	integer_edit	*edit_parameter_6;
	float_edit	*edit_parameter_5;
	float_edit	*edit_parameter_4;
	float_edit	*edit_parameter_3;
	integer_edit *edit_parameter_2;
	float_edit	*edit_parameter_1;
	float_edit	*edit_heat_units;
	integer_edit	*edit_day;
	integer_edit	*edit_year;
	integer_edit	*edit_month;
	wxComboBox	*combo_type;

		void EndModal (int retCode);
		void OnParameter2(wxCommandEvent &);
		void OnParameter6(wxCommandEvent &);
		void OnSelchangeType(wxCommandEvent &);

	protected:
		class SWATManagement *op;
		class swat_controller *boss;
		class list_field_help_swat *help_list;
		class odbc_database *database;

		void describe_parameters ();

	DECLARE_EVENT_TABLE()
};

