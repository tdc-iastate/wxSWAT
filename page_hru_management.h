
class page_hru_management : public wxPanel {
	public:
		page_hru_management
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

		long_integer_edit *edit_management_code;
		wxCheckBox *check_landcover;
		long_integer_edit *edit_land_cover_id;
		float_edit *edit_initial_leaf_area_index;
		float_edit *edit_initial_dry_weight_biomass;
		float_edit *edit_plant_heat_units;
		float_edit *edit_grazingbiomassminimum;
		float_edit *edit_biologicalmixingefficiency;
		float_edit *edit_runoffcurvenumber;
		float_edit *edit_cf;
		float_edit *edit_cfh;
		float_edit *edit_cfdec;

		float_edit *edit_cmn, *edit_cdn, *edit_nperco, *edit_phoskd, *edit_psp, *edit_sdnco;
		wxTextCtrl *edit_help;

	protected:
		class HRU_record *hru;
		class SubBasin *subbasin;
		class SWATControlRecord *control;
		class swat_controller *controller;

	private:
		DECLARE_EVENT_TABLE()
		wxNotebookPage *m_prev, *m_next;
};

enum {
	EDIT_MANAGEMENT_CODE = 101,
	CHECK_LANDCOVER,
	EDIT_LAND_COVER_ID,
	EDIT_INITIAL_LEAF_AREA_INDEX,
	EDIT_INITIAL_DRY_WEIGHT_BIOMASS,
	EDIT_PLANT_HEAT_UNITS,
	EDIT_GRAZINGBIOMASSMINIMUM,
	EDIT_BIOLOGICALMIXINGEFFICIENCY,
	EDIT_RUNOFFCURVENUMBER,
	EDIT_CF,
	EDIT_CFH,
	EDIT_CFDEC,
	EDIT_CMN,
	EDIT_CDN,
	EDIT_NPERCO,
	EDIT_PHOSKD,
	EDIT_PSP,
	EDIT_SDNCO,
	EDIT_HELP_MANAGEMENT
};
