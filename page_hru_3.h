
class page_hru_3 : public wxPanel {
	public:
		page_hru_3
			(wxWindow *parent,
			const int id,
			class HRU_record *,
			class SubBasin *,
			class SWATControlRecord *,
			class swat_controller *);

		// implement wxWizardPage functions
		virtual wxNotebookPage *GetPrev() const { return m_prev; }
		virtual wxNotebookPage *GetNext() const
		{
			// return m_checkbox->GetValue() ? m_next->GetNext() : m_next;
			return m_next;
		}
		void SetPrev(wxNotebookPage *prev) { m_prev = prev; };
		void SetNext(wxNotebookPage *next) { m_next = next; };

		virtual bool TransferDataToWindow ();
		virtual bool TransferDataFromWindow ();

		float_edit *edit_depth_to_subsurface_drain;
		float_edit *edit_timetodrainsoiltofieldcapacity;
		float_edit *edit_draintilelagtime;
		float_edit *edit_re;
		float_edit *edit_sdrain;
		float_edit *edit_dc;
		float_edit *edit_latksatf;
		float_edit *edit_pc;
		float_edit *edit_uslepfactor;
		float_edit *edit_sftmp;
		float_edit *edit_smtmp;
		float_edit *edit_smfmx;
		float_edit *edit_smfmn;
		float_edit *edit_imperviouslayerdepth;
		float_edit *edit_fld_fr;
		float_edit *edit_rip_fr;
		wxTextCtrl *edit_help;

	protected:
		class HRU_record *hru;
		class SubBasin *subbasin;
		class SWATControlRecord *control;
		class swat_controller *controller;

		void show_irrigation_source ();

	private:
		DECLARE_EVENT_TABLE()
		wxNotebookPage *m_prev, *m_next;
};

enum {
	EDIT_DEPTH_TO_SUBSURFACE_DRAIN = 101,
	EDIT_TIMETODRAINSOILTOFIELDCAPACITY,
	EDIT_DRAINTILELAGTIME,
	EDIT_RE,
	EDIT_SDRAIN,
	EDIT_DC,
	EDIT_LATKSATF,
	EDIT_PC,
	EDIT_USLEPFACTOR,
	EDIT_SFTMP,
	EDIT_SMTMP,
	EDIT_SMFMX,
	EDIT_SMFMN,
	EDIT_IMPERVIOUSLAYERDEPTH,
	EDIT_FLD_FR,
	EDIT_RIP_FR,
	EDIT_HELP_3
};
