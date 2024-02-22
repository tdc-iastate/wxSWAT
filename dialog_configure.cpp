#include "../util/utility_afx.h"
#include <vector>
#include <set>
#include <wx/stattext.h>
#include <wx/window.h>
#include <wx/dialog.h>
#include <wx/filedlg.h>
#include <wx/clipbrd.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/app.h>
#include <wx/button.h>
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/listbox_files.h"
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "dialog_configure.h"
#include "../i_swat_database/SWATDeclarations.h"
#include "wxSWAT.h"


BEGIN_EVENT_TABLE(dialog_configure, wxDialog)
	EVT_BUTTON(ID_BUTTON_SWAT_PATH, dialog_configure::OnEditPath)
	EVT_BUTTON(BUTTON_ADD_FOLDER, dialog_configure::OnAddFolder)
	EVT_BUTTON(BUTTON_DELETE_FOLDER, dialog_configure::OnDeleteFolder)
	EVT_COMBOBOX (ID_LIST_SWAT_VERSION, dialog_configure::OnSelChangeVersion)
END_EVENT_TABLE()

void add_edit_path
    (const wxString &static_label,
    const int edit_id,
    wxTextCtrl **edit_control,
    const wxSize &edit_size,
	wxButton *edit_path_button,
    wxWindow *parent,
    wxBoxSizer *stack)

{

	wxBoxSizer *row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (parent, -1, static_label), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
    *edit_control = new wxTextCtrl (parent, edit_id, wxEmptyString, wxDefaultPosition, edit_size); //, wxPoint(15, 30), regular_size);
	row->Add (*edit_control, 0, wxALL, 5);
	row->Add (edit_path_button, 0, wxALL, 5);
	stack->Add (row, 0, wxGROW | wxLEFT | wxRIGHT, 5);
}

void configure_filename_set
	(wxWindow* parent,
	const int list_index,
	filename_placeholder* holder)

// Used for listbox_files.callback_filename_set
// Called when a path has been browsed

// For SWAT executables, keep path and filename separate

{
	dialog_configure *dc = (dialog_configure *) parent;

	// "Output Variables", "SWAT help", "Historical Weather Folder", run folders, swat_filenames
	if (list_index >= 3 + dc->app->run_folders.size()) {
		// filename_placeholder.location contains path and filename
		// Strip the path and call filename_placeholder.set_file
		holder->location.set_path ("");
		holder->set_file ();
	}
}

dialog_configure::dialog_configure
	(wxWindow *pParent,
	app_wxswat *p_app)
	: wxDialog (pParent, -1, "Configuration")
{
	int version_index;
	app = p_app;
	wxBoxSizer *stack = new wxBoxSizer (wxVERTICAL);

	// wxCheckBox *check_test = new wxCheckBox (this, wxID_ANY, "Background Color Test"); // , wxDefaultPosition, wxDefaultSize, wxBG_STYLE_TRANSPARENT);
	// stack->Add (check_test, 0, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);

	// 3 across: Static, listbox_files, button_edit_swat_path
	wxBoxSizer *row = new wxBoxSizer (wxHORIZONTAL);
	{
		row->Add (new wxStaticText (this, -1, L"Files:"), 0, wxALIGN_BOTTOM);
		row->AddSpacer (10);
		list_files = new listbox_files (this, ID_LIST_FILES, wxDefaultPosition, wxSize (425, 200));
		row->Add (list_files);
		list_files->callback_filename_set = configure_filename_set;
		row->AddSpacer (10);

		wxBoxSizer* column = new wxBoxSizer(wxVERTICAL);

		button_edit_path = new wxButton (this, ID_BUTTON_SWAT_PATH, _ ("Edit"), wxDefaultPosition, wxDefaultSize);
		column->Add (button_edit_path, 0, wxALL, 5);
		button_add_folder = new wxButton (this, BUTTON_ADD_FOLDER, _ ("Add Folder"), wxDefaultPosition, wxDefaultSize);
		column->Add (button_add_folder, 0, wxALL, 5);
		button_delete_folder = new wxButton (this, BUTTON_DELETE_FOLDER, _ ("Delete Folder"), wxDefaultPosition, wxDefaultSize);
		column->Add (button_delete_folder, 0, wxALL, 5);
		row->Add(column);
	}
	stack->Add(row, 0, wxALL, 5);

	wxString version_choices[NUMBER_OF_SWAT_VERSIONS];
	for (version_index = 0; version_index < NUMBER_OF_SWAT_VERSIONS; ++version_index)
		version_choices[version_index] = SWAT_VERSION_NAME[version_index];
	combo_swat_version = new wxComboBox (this, ID_LIST_SWAT_VERSION, wxEmptyString, wxDefaultPosition, wxDefaultSize, NUMBER_OF_SWAT_VERSIONS, version_choices);

	// 3 across: static, listbox, edit
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Version"), 0, wxALL, 5);
	row->AddStretchSpacer ();
	row->Add (combo_swat_version, 0, wxALL, 5);
	row->AddStretchSpacer ();
	row->Add (new wxStaticText (this, wxID_ANY, "Revision"), 0, wxALL, 5);
	row->AddStretchSpacer ();
	edit_swat_revision = new wxTextCtrl (this, ID_EDIT_SWAT_REVISION, wxEmptyString, wxDefaultPosition, wxSize (350, 24));
	row->Add (edit_swat_revision, 0, wxALL, 5);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer(wxHORIZONTAL);
	{
		row->AddSpacer(20);

		wxBoxSizer* column = new wxBoxSizer(wxVERTICAL);

		check_output_hru_annual = new wxCheckBox (this, wxID_ANY, "Output HRU Annual");
		column->Add (check_output_hru_annual);

		check_output_hru_monthly = new wxCheckBox (this, wxID_ANY, "Output HRU Monthly");
		column->Add (check_output_hru_monthly);

		check_output_hru_daily = new wxCheckBox (this, wxID_ANY, "Output HRU Daily");
		column->Add (check_output_hru_daily);

		column->AddSpacer (5);

		check_output_subbasin_annual = new wxCheckBox (this, wxID_ANY, "Output Subbasin Annual");
		column->Add (check_output_subbasin_annual);

		check_output_subbasin_monthly = new wxCheckBox (this, wxID_ANY, "Output Subbasin Monthly");
		column->Add (check_output_subbasin_monthly);

		check_output_subbasin_daily = new wxCheckBox (this, wxID_ANY, "Output Subbasin Daily");
		column->Add (check_output_subbasin_daily);
		row->Add (column);

		row->AddSpacer (5);

		column = new wxBoxSizer(wxVERTICAL);
		check_output_reach_annual = new wxCheckBox (this, wxID_ANY, "Output Reach Annual");
		column->Add (check_output_reach_annual);

		check_output_reach_monthly = new wxCheckBox (this, wxID_ANY, "Output Reach Monthly");
		column->Add (check_output_reach_monthly);

		check_output_reach_daily = new wxCheckBox (this, wxID_ANY, "Output Reach Daily");
		column->Add (check_output_reach_daily);

		column->AddSpacer (5);

		check_output_mgt = new wxCheckBox (this, wxID_ANY, "Output MGT");
		column->Add (check_output_mgt);
		check_output_hsl = new wxCheckBox(this, wxID_ANY, "Output HSL");
		column->Add(check_output_hsl);
		check_input_std = new wxCheckBox(this, wxID_ANY, "Input STD");
		column->Add(check_input_std);
		check_output_std = new wxCheckBox(this, wxID_ANY, "Output STD");
		column->Add(check_output_std);
		check_use_id = new wxCheckBox(this, wxID_ANY, "Use ID");
		column->Add(check_use_id);
		check_match_file_precision = new wxCheckBox(this, wxID_ANY, "Match File Precision");
		column->Add(check_match_file_precision);

		row->Add (column);
	}
	stack->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	{
		row->Add (new wxStaticText (this, wxID_ANY, "Run method:"), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

		wxArrayString choices;
		choices.Add ("Hidden");
		choices.Add ("Visible console");
		choices.Add("DLL");
		choices.Add("HRU Only");
		combo_run_method = new wxComboBox (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_READONLY);
		row->Add (combo_run_method);

		row->AddSpacer (25);
		row->Add (new wxStaticText (this, wxID_ANY, "Historical Weather:"), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
		choices.clear ();
		choices.Add ("Write as Needed");
		choices.Add ("Do Not Write");
		choices.Add ("Write All");
		choices.Add ("DLL array from file");
		combo_historical_weather = new wxComboBox (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_READONLY);
		row->Add (combo_historical_weather);
	}
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Save daily output for reaches:"), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_save_daily_reach = new wxTextCtrl (this, wxID_ANY, "");
	row->Add (edit_save_daily_reach, 0, wxALL, 5);
	// row->AddStretchSpacer ();
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Save daily output for HRUs (\"1-1 1-2 ...\"):"), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_save_daily_hru = new wxTextCtrl (this, wxID_ANY, "", wxDefaultPosition, wxSize (300, 24));
	row->Add (edit_save_daily_hru, 0, wxALL, 5);
	// row->AddStretchSpacer ();
	stack->Add (row, 0, wxALL, 5);

	stack->Add (CreateButtonSizer (wxOK | wxCANCEL));

	SetSizerAndFit(stack);

	check_output_hru_annual->SetValue (app->write_output_hru_annual);
	check_output_hru_monthly->SetValue (app->write_output_hru_monthly);
	check_output_hru_daily->SetValue (app->write_output_hru_daily);
	check_output_subbasin_annual->SetValue (app->write_output_subbasin_annual);
	check_output_subbasin_monthly->SetValue (app->write_output_subbasin_monthly);
	check_output_subbasin_daily->SetValue (app->write_output_subbasin_daily);
	check_output_reach_annual->SetValue (app->write_output_reach_annual);
	check_output_reach_monthly->SetValue (app->write_output_reach_monthly);
	check_output_reach_daily->SetValue (app->write_output_reach_daily);
	check_output_mgt->SetValue (app->read_output_mgt);
	check_output_hsl->SetValue(app->write_output_hsl);
	check_input_std->SetValue(app->write_input_std);
	check_output_std->SetValue(app->write_output_std);
	check_use_id->SetValue (app->use_id);
	check_match_file_precision->SetValue (app->match_file_precision);
	combo_run_method->SetSelection (app->swat_run_method - 1);

	{
		// Reach #s to save daily output for
		std::set <long>::const_iterator reach_id;
		dynamic_string reaches;
		for (reach_id = app->save_daily_reach.begin ();
		reach_id != app->save_daily_reach.end ();
		++reach_id)
			reaches.add_formatted (" %ld", *reach_id);
		edit_save_daily_reach->SetValue (reaches.get_text ());
	}

	{
		// HRU #s to save daily output for
		std::set <std::pair <long, long>>::const_iterator hru_id;
		dynamic_string reaches;
		for (hru_id = app->save_daily_hru.begin ();
		hru_id != app->save_daily_hru.end ();
		++hru_id)
			reaches.add_formatted (" %ld-%ld", hru_id->first, hru_id->second);
		edit_save_daily_hru->SetValue (reaches.get_text ());
	}

	combo_swat_version->SetSelection (app->SWATVersion - 1);
	combo_historical_weather->SetSelection (app->historical_weather_method - 1);

	show_version_files ();

	dynamic_string text_value;
	text_value.format ("%d", app->SWATRevision);
	edit_swat_revision->SetValue (text_value.get_text_wide ());
}

void dialog_configure::OnSelChangeVersion
	(wxCommandEvent &)

{
	int version;

	version = combo_swat_version->GetSelection () + 1;

	if (version >= SWAT_VERSION_2009)
		edit_swat_revision->Enable (true);
	else
		edit_swat_revision->Enable (false);

	// list_files->set_highlight (version);
	// list_files->Invalidate ();
	show_version_files ();
}

void dialog_configure::OnEditPath
	(wxCommandEvent &)

// Click of Edit button intended for listbox_filenames

{
	wxCommandEvent e (wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, ID_LIST_FILES);
	list_files->ProcessWindowEvent (e);
}

void dialog_configure::OnAddFolder
	(wxCommandEvent &)

{
	app->run_folders.push_back ("New Folder");

	show_version_files ();
}

void dialog_configure::OnDeleteFolder
	(wxCommandEvent &)

{
	app->run_folders.resize (app->run_folders.size () - 1);
	show_version_files ();
}

void dialog_configure::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {
		// app->swat_locations [SWAT_VERSION_2009 - 1] [0] = edit_swat_path->GetValue ().ToAscii ();

		app->write_output_hru_annual = check_output_hru_annual->GetValue ();
		app->write_output_hru_monthly = check_output_hru_monthly->GetValue ();
		app->write_output_hru_daily = check_output_hru_daily->GetValue ();
		app->write_output_subbasin_annual = check_output_subbasin_annual->GetValue ();
		app->write_output_subbasin_monthly = check_output_subbasin_monthly->GetValue ();
		app->write_output_subbasin_daily = check_output_subbasin_daily->GetValue ();
		app->write_output_reach_annual = check_output_reach_annual->GetValue ();
		app->write_output_reach_monthly = check_output_reach_monthly->GetValue ();
		app->write_output_reach_daily = check_output_reach_daily->GetValue ();
		app->read_output_mgt = check_output_mgt->GetValue ();
		app->write_output_hsl = check_output_hsl->GetValue ();
		app->write_input_std = check_input_std->GetValue();
		app->write_output_std = check_output_std->GetValue();
		app->use_id = check_use_id->GetValue ();
		app->match_file_precision = check_match_file_precision->GetValue ();
		app->swat_run_method = combo_run_method->GetSelection () + 1;

		// Reach #s to save daily output for
		dynamic_string reaches;
		int position;
		long reach, subbasin, hru;

		app->save_daily_reach.clear ();
		reaches = edit_save_daily_reach->GetValue ().ToAscii ();
		position = 0;
		while (reaches.parse (" ", &position, &reach))
			app->save_daily_reach.insert (reach);

		// HRU #s to save daily output for
		// Subbasin-HRU
		// 109-2
		app->save_daily_hru.clear ();
		reaches = edit_save_daily_hru->GetValue ().ToAscii ();

		position = 0;
		while (reaches.parse (" -", &position, &subbasin, true, true)
		&& reaches.parse (" -", &position, &hru, true, true)) {
			app->save_daily_hru.insert (std::pair <long, long> (subbasin, hru));
		}

		dynamic_string text_value;
		text_value = edit_swat_revision->GetValue ().ToAscii ();
		app->SWATRevision = atoi (text_value.get_text_ascii ());

		app->SWATVersion = combo_swat_version->GetSelection () + 1;
		app->historical_weather_method = combo_historical_weather->GetSelection () + 1;

		{
			int version_index;
			filename_struct path_parser;

			// Any executable file set will get the path too.  Remove them
			for (version_index = 0; version_index < NUMBER_OF_SWAT_VERSIONS; ++version_index) {
				// strip path from app->filename_swat_executable because path is kept separate
				path_parser.parse (&app->filename_swat_executable[version_index]);
				path_parser.write_filename (&app->filename_swat_executable[version_index]);
			}
		}
	}
	wxDialog::EndModal (retCode);
}

void dialog_configure::show_version_files ()

{
	dynamic_string description;
	int version_index, thread_index;
	std::vector <dynamic_string>::iterator instance;
	filename_struct path_parser;

	version_index = combo_swat_version->GetSelection ();
	list_files->clear ();
	list_files->add_filename ("Output Variables", &app->filename_swat_output_variables);
	list_files->add_filename ("SWAT help", &app->filename_swat_help);
	list_files->add_path ("Historical Weather Folder", &app->path_historical_weather);

	for (instance = app->run_folders.begin (), thread_index = 0;
	instance != app->run_folders.end ();
	++instance, ++thread_index) {
		description.add_formatted ("Run Folder %d", thread_index + 1);
		list_files->add_path (description.get_text_ascii (), &*instance, true);
	}

	for (version_index = 0; version_index < NUMBER_OF_SWAT_VERSIONS; ++version_index) {
		description = SWAT_VERSION_NAME[version_index];
		description += " executable";

		// strip path from app->filename_swat_executable because path is kept separate
		path_parser.parse (&app->filename_swat_executable [version_index]);
		path_parser.write_filename (&app->filename_swat_executable[version_index]);
		
		list_files->add_filename (description.get_text_ascii (), &app->filename_swat_executable [version_index], false);
	}

	list_files->show_filenames ();


	/*
	if (app->swat_locations [SWAT_VERSION_2009 - 1].size () == 0)
		app->swat_locations [SWAT_VERSION_2009 - 1].push_back ("Default swat path");
	edit_swat_path->SetValue (app->swat_locations [SWAT_VERSION_2009 - 1] [0].get_text ());
	*/
}