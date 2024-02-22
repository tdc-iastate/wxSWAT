
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dialog_select.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dialog_select dialog

// template <class T> class selection_list {
class selection_list {
	public:
		class T *pick;
		std::vector <class T> *item_list;
};

class dialog_select : public wxDialog {
public:
	dialog_select
		(wxWindow * pParent,
		void **pick,
		std::vector <class T> *,
		int code,
		bool edit,
		class list_field_help_swat *,
		class odbc_database *db);

		enum {
			BUTTON_EDIT = 103,
			LIST_CHOICES
		};

		wxButton *button_edit;
		wxListBox *list_choices;
		void OnDblclkChoices(wxCommandEvent &);
		void OnEdit(wxCommandEvent &);
		virtual void EndModal
			(int retCode);

protected:
	class list_field_help_swat *help_list;
	std::vector <class T> *item_list;
	void **selection;
	int op_code;
	bool allow_edit;
	odbc_database *database;

		void show_list ();

	DECLARE_EVENT_TABLE()
};

