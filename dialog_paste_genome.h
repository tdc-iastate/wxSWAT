
class dialog_paste_genome : public wxDialog {
	public:
		enum {
			ID_FILENAME_OUTPUT_VARIABLES = 103,
			BUTTON_CHECK_TYPES,
			BUTTON_PASTE,
			STATIC_PREVIEW
		};
		dialog_paste_genome
			(class wxWindow *,
			int *p_genome,
			long *p_id,
			class SWATControlRecord *p_control);

		void OnCompress
			(wxCommandEvent& e);
		void OnCheckTypes
			(wxCommandEvent& e);
		void OnPaste
			(wxCommandEvent& e);

		wxButton *button_ok, *button_paste;
		class wxStaticText *static_preview;
		class wxListBox *list_counts;
		class integer_static *static_id;

	protected:
		int *genome;
		long *id;
		class SWATControlRecord *control;

		virtual void EndModal
			(int retCode);
		void parse_genome
			(const dynamic_string &genome_text);
		void show_counts ();
		void show_genome ();
	DECLARE_EVENT_TABLE()
};
