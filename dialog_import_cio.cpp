// dialog_import_cio.cpp : implementation file
//

#include "../util/utility_afx.h"
#include <vector>
#include "resource.h"
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/filedlg.h>
#include "../util/custom.h"
#include "../util/dynamic_string.h"
#include "../util/utility.h"
#include "../util/filename_struct.h"
#include "dialog_import_cio.h"
#include "../i_swat_database/SWATDeclarations.h"

BEGIN_EVENT_TABLE(dialog_import_cio, wxDialog)
	EVT_BUTTON(BUTTON_BROWSE_INPUT_FILE, dialog_import_cio::OnBrowseInputFile)
END_EVENT_TABLE()

dialog_import_cio::dialog_import_cio
	(wxWindow* pParent,
	dynamic_string *p_filename_cio,
	long *new_record_id,
	BYTE *p_read_weather,
	short *p_swat_version,
	int *p_swat_revision,
	bool *p_save_exact_fraction)
	: wxDialog (pParent, wxID_ANY, L"Import CIO File")
{
	record_id = new_record_id;
	filename_cio = p_filename_cio;
	swat_version = p_swat_version;
	swat_revision = p_swat_revision;
	read_all_weather = p_read_weather;
	save_exact_fraction = p_save_exact_fraction;

    wxBoxSizer *stack = new wxBoxSizer (wxVERTICAL);

	wxStaticBoxSizer *outline_box = new wxStaticBoxSizer (wxHORIZONTAL, this, wxT("CIO Filename"));
	static_cio_filename = new wxStaticText (this, wxID_ANY, L"", wxDefaultPosition, wxSize (300, 24));
	outline_box->Add (static_cio_filename, 0, wxALL, 5);
	outline_box->Add (new wxButton (this, BUTTON_BROWSE_INPUT_FILE, L"Browse"), 0, wxALL, 5);
	stack->Add (outline_box);

    wxBoxSizer *row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, L"Record ID"), 0, wxALL, 5);
	edit_record_id = new long_integer_edit (this, wxID_ANY);
	row->Add (edit_record_id, 0, wxALL, 5);
	stack->Add (row);

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, L"Weather"), 0, wxALL, 5);

		wxArrayString choices;
		choices.Add ("Read recorded weather as needed");
		choices.Add ("Read all recorded weather first");
		choices.Add ("Skip recorded weather");

		combo_read_weather = new wxComboBox (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_READONLY);
		row->Add (combo_read_weather, 0, wxALL, 5);
		stack->Add (row);
	}

    row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, L"SWAT version"), 0, wxALL, 5);
	combo_swat_version = new wxComboBox (this, wxID_ANY, L"", wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY);
	row->Add (combo_swat_version, 0, wxALL, 5);
	stack->Add (row);

    row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, L"SWAT revision"), 0, wxALL, 5);
	combo_swat_revision = new wxComboBox (this, wxID_ANY, L"", wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY);
	row->Add (combo_swat_revision, 0, wxALL, 5);
	stack->Add (row);

	check_save_fraction = new wxCheckBox (this, wxID_ANY, L"Save exact HRU fraction in description");
	stack->Add (check_save_fraction, 0, wxALL, 5);

	stack->Add (CreateButtonSizer (wxOK | wxCANCEL));

	SetSizerAndFit(stack);


	int index, item;

	static_cio_filename->SetLabel (filename_cio->get_text ());
	edit_record_id->Set (*record_id);
	for (index = 0; index < NUMBER_OF_SWAT_VERSIONS; ++index)
		combo_swat_version->Append (SWAT_VERSION_NAME [index]);
	combo_swat_version->SetSelection (*swat_version - 1);

	for (index = 0; index < NUMBER_OF_SWAT_REVISIONS; ++index) {
		item = combo_swat_revision->Append (SWAT_REVISION_NAME [index]);
		combo_swat_revision->SetClientData (item, (void *) &SWAT_REVISION [index]);
		if (SWAT_REVISION [index] == *swat_revision)
			combo_swat_revision->SetSelection (item);
	}
	combo_read_weather->SetSelection (*read_all_weather);
	check_save_fraction->SetValue (*save_exact_fraction);
}


void dialog_import_cio::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {

		*record_id = edit_record_id->Get ();
		*read_all_weather = (BYTE) combo_read_weather->GetSelection ();
		*swat_version = combo_swat_version->GetSelection () + 1;
		*swat_revision = *((int *) combo_swat_revision->GetClientData (combo_swat_revision->GetSelection ()));
		*save_exact_fraction = check_save_fraction->GetValue ();
	}
	wxDialog::EndModal (retCode);
}

void dialog_import_cio::OnBrowseInputFile
	(wxCommandEvent &) 

{
	wxFileDialog* openFileDialog = new wxFileDialog( this, _("Open file"), "", "", "SWAT CIO Files (*.cio)|*.cio", wxFD_OPEN, wxDefaultPosition);

	openFileDialog->SetPath (filename_cio->get_text ());
	if ( openFileDialog->ShowModal() == wxID_OK ) {

		*filename_cio = openFileDialog->GetDirectory().ToAscii ();
		*filename_cio += PATH_SEPARATOR;
		*filename_cio += openFileDialog->GetFilename().ToAscii ();

		static_cio_filename->SetLabel (filename_cio->get_text ());
	}
}
