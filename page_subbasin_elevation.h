
class page_subbasin_elevation : public wxPanel {
	public:
		page_subbasin_elevation
			(wxWindow *parent,
			const int id,
			class SubBasin *);

		void set_controls ();
		bool read_controls ();

		float_edit *edit_elevation [10];
		float_edit *edit_elevation_band_area [10];
		float_edit *edit_elevation_band_snow [10];

	protected:
		class SubBasin *basin;

		DECLARE_EVENT_TABLE()
};

enum {
	EDIT_ELEVATION_1 = 101,
	EDIT_ELEVATION_BAND_AREA_1,
	EDIT_ELEVATION_BAND_SNOW_1,
};
