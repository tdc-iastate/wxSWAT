#include "../util/utility_afx.h"
#include <vector>
#include <set>
#include <map>
#include <chrono>
#include <wx/stattext.h>
#include <wx/window.h>
#include <wx/dialog.h>
#include <wx/filedlg.h>
#include <wx/clipbrd.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/app.h>
#include <wx/button.h>
#include <wx/grid.h>
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/dialog_error_list.h"
#include "../util/custom.h"
#include "../util/list.h"
#include "../util/Timestamp.h"
#include "../util/device_coordinate.h"
#include "dialog_paste_genome.h"
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"

#include "../i_epic_database/EPICDeclarations.h"
#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATSoilLayers.h"
#include "../i_swat_database/SWATSoils.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATUrban.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATOutputHRUAnnual.h"
#include "../i_swat_database/SWATOutputHRUMonthly.h"
#include "../i_swat_database/SWATOutputHRUDaily.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
#include "../i_swat_database/SWATOutputSubBasinDaily.h"
#include "../i_swat_database/output_column_position.h"
#include "../i_swat_database/swat_output.h"
#include "../i_swat_database/SWATManagement.h"
#include "../i_swat_database/SWATTemporalConservationPractices.h"
#include "../i_swat_database/rowset_hru.h"
#include "../i_swat_database/SWATWeatherHistorical.h"
#include "../i_swat_database/SWATWeatherByMonth.h"
#include "../i_swat_database/SWATWeatherStations.h"
#include "../i_swat_database/swat_weather_list.h"
#include "../i_swat_database/rowset_subbasin.h"
#include "../i_swat_database/SWATPotentialEvapotranspiration.h"
#include "../i_swat_database/SWATControlRecord.h"
#ifdef _MRB_DOUBLE_RUN
#include "MRB_Double_Run.h"

BEGIN_EVENT_TABLE(dialog_paste_genome, wxDialog)
	EVT_BUTTON (BUTTON_PASTE, dialog_paste_genome::OnPaste)
    // EVT_GRID_RANGE_SELECT (dialog_paste_genome::OnSelectRowColumn) // clicked on row or column label
END_EVENT_TABLE()


dialog_paste_genome::dialog_paste_genome
	(wxWindow *pParent,
	int *p_genome,
	long *p_id,
	SWATControlRecord *p_control)
	: wxDialog (pParent, -1, "Paste Genome")
{
	wxBoxSizer *stack, *row;

	genome = p_genome;
	control = p_control;
	id = p_id;

	stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, -1, L"Paste genome from SAVE file:"), 0, wxALL, 5);
	row->Add (new wxButton (this, BUTTON_PASTE, "Paste"), 0, wxALL, 5);
	row->AddSpacer (25);
	list_counts = new wxListBox (this, wxID_ANY, wxDefaultPosition, wxSize (200, 200));
	row->Add (list_counts, 0, wxALL, 5);

	row->AddSpacer (25);
	row->Add (new wxStaticText (this, -1, L"ID:"), 0, wxALL, 5);
	row->AddSpacer (10);
	static_id = new integer_static (this, wxID_ANY, wxT("ID"), wxDefaultPosition, wxDefaultSize);
	row->Add (static_id, 0, wxALL, 5);
	stack->Add (row);

	stack->AddSpacer (5);
	wxStaticBoxSizer *preview_outline = new wxStaticBoxSizer (wxVERTICAL, this, "Preview");
	static_preview = new wxStaticText (this, STATIC_PREVIEW, wxT("..."), wxDefaultPosition, wxSize (800, 550));
	preview_outline->Add (static_preview, wxALL, 5);
	stack->Add (preview_outline, wxALL, 5);

	stack->AddSpacer (5);
	stack->Add (CreateButtonSizer (wxOK | wxCANCEL));

	SetSizerAndFit(stack);

	static_id->Set (*id);
}

void dialog_paste_genome::show_counts ()

{
	int index;
	dynamic_string label;
	double evaldata_other[MRB_EVALDATA_OTHER_COUNT];

	memset (evaldata_other, 0, sizeof (double) * MRB_EVALDATA_OTHER_COUNT);
	count_wetlands (control, genome, evaldata_other);

	list_counts->Clear ();
	for (index = 0; index < 18; ++index) {
		label = MRB_EVALDATA_OTHER_NAME[index];
		label.add_formatted (": %d", (int) evaldata_other [index]);
		list_counts->Append (label.get_text ());
	}

	static_id->Set (*id);
}

void dialog_paste_genome::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {
	}
	wxDialog::EndModal (retCode);
}

void dialog_paste_genome::parse_genome
	(const dynamic_string &genome_text)

{
	std::vector <dynamic_string> tokens;
	int index;

	genome_text.tokenize (" \n", &tokens);

	if (tokens.size () > MRB_GENOME_LENGTH) {

		*id = atol (tokens[0].get_text_ascii ());

		for (index = 0; index < MRB_GENOME_LENGTH; ++index)
			genome[index] = atoi (tokens[1 + index].get_text_ascii ());
	}
}

void dialog_paste_genome::OnPaste
	(wxCommandEvent& e)
{
	wxClipboard board;
	wxTextDataObject data_object;
	wxDataFormatId paste_type;
	dynamic_string genome_text;

	paste_type = wxDF_TEXT;

	if (wxTheClipboard->Open ())
	{
		if (wxTheClipboard->IsSupported (paste_type)) {
			// This data objects are held by the clipboard,
			// so do not delete them in the app.
			if (wxTheClipboard->GetData (data_object)) {
				genome_text = data_object.GetText ().ToAscii ();
				parse_genome (genome_text);

				show_genome ();
				show_counts ();
			}
		}
		else
			wxMessageBox ("Unsupported clipboard type", "", wxICON_ERROR | wxOK);
        wxTheClipboard->Close();
	}
}

void dialog_paste_genome::show_genome ()


{
	dynamic_string label;
	int index;

	for (index = 0; index < MRB_GENOME_LENGTH; ++index) {
		if (index == 0)
			label.format ("%d", genome [index]);
		else
			label.add_formatted (" %d", genome [index]);
	}
	static_preview->SetLabel (label.get_text ());
	static_preview->Wrap (800);
}


#endif