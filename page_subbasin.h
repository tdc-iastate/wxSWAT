
class page_subbasin : public wxPanel {
	public:
		page_subbasin
			(wxWindow *parent,
			const int id,
			class SubBasin *,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		void set_controls ();
		bool read_controls ();

		wxTextCtrl *edit_description;
		float_edit *edit_latitude;
		float_edit *edit_longitude;
		float_edit *edit_elevation;
		float_edit *edit_precipitation_lapse_rate;
		float_edit *edit_temperature_lapse_rate;
		wxStaticText *static_area_acres;
		wxStaticText *static_area_km2;
		// wxStaticText *static_area_fraction;
		wxStaticText *static_weather_station;
		wxStaticText *static_weather_precipitation;
		wxStaticText *static_weather_temperature;
		wxStaticText *static_weather_solar_radiation;
		wxStaticText *static_weather_relative_humidity;
		wxStaticText *static_weather_wind_speed;
		wxCheckBox *check_pond;
		wxCheckBox *check_swq;
		wxCheckBox *check_water_use;
		wxListBox *list_hru;
		wxButton *button_pond, *button_swq, *button_water_use;

		void OnCheckPond (wxCommandEvent &);
		void OnCheckSWQ (wxCommandEvent &);
		void OnCheckWaterUse (wxCommandEvent &);
		void OnPond (wxCommandEvent &);
		void OnSwq (wxCommandEvent &);
		void OnWaterUse (wxCommandEvent &);
		void OnDelete (wxCommandEvent &);
		void OnEdit (wxCommandEvent &);
		void OnMove (wxCommandEvent &);
		void OnWeatherSummary (wxCommandEvent &);
		void OnDblClickHRU (wxCommandEvent &);

	protected:
		class SubBasin *basin;
		class SWATControlRecord *control;
		class swat_controller *controller;
		class list_field_help_swat *help;
		class odbc_database *db;
		void show_area_fraction ();
		void show_hru_list ();

		DECLARE_EVENT_TABLE()
};

enum {
	EDIT_DESCRIPTION = 101,
	EDIT_LATITUDE,
	EDIT_LONGITUDE,
	EDIT_ELEVATION,
	EDIT_PRECIPITATION_LAPSE_RATE,
	EDIT_TEMPERATURE_LAPSE_RATE,
	STATIC_AREA_ACRES,
	STATIC_AREA_KM2,
	// STATIC_AREA_FRACTION,
	STATIC_WEATHER_STATION,
	STATIC_RECORDED_WEATHER,
	STATIC_WEATHER_TEMPERATURE,
	STATIC_WEATHER_SOLAR_RADIATION,
	STATIC_WEATHER_RELATIVE_HUMIDITY,
	STATIC_WEATHER_WIND_SPEED,
	CHECK_POND,
	BUTTON_POND,
	CHECK_SWQ,
	BUTTON_SWQ,
	CHECK_WATER_USE,
	BUTTON_WATER_USE,
	LIST_HRU,
	BUTTON_DELETE,
	BUTTON_EDIT,
	BUTTON_MOVE,
	BUTTON_WEATHER_SUMMARY
};
