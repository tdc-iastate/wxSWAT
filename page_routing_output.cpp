#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/custom.h"
#include "../util/Timestamp.h"
#include "../util/device_coordinate.h"
#ifdef _ODBC_TDC
#include <odbcinst.h>
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"
#else
#include "../util/db.h"
#endif
#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATReservoir.h"
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include "resource.h"
#include "page_routing_output.h"
// #include "property_page_reservoir_size.h"
// #include "property_page_reservoir_flow.h"
// #include "propsheet_reservoir.h"
#include "../i_swat_database/SWATOutputHRUAnnual.h"
#include "../i_swat_database/SWATOutputHRUMonthly.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
#include "../i_swat_database/SWATOutputSubBasinDaily.h"
#include "../i_swat_database/SWATOutputHRUDaily.h"
#include "../i_swat_database/SWATOutputHSL.h"
#include "../i_swat_database/output_column_position.h"
#include "../i_swat_database/swat_output.h"

BEGIN_EVENT_TABLE(page_routing_output, wxPanel)
END_EVENT_TABLE()

page_routing_output::page_routing_output
	(wxWindow *parent,
	const int id,
	swat_routing_node *p_node,
	SWATControlRecord *p_control_record)
	: wxPanel(parent, id)
{
	node = p_node;
	control_record = p_control_record;

	wxFlexGridSizer* grid = new wxFlexGridSizer (2, wxSize(4, 4));

	static_command_number = new integer_static (this, STATIC_COMMAND_NUMBER, wxT("Static"));
	grid->Add (static_command_number);
	grid->AddSpacer(0);

	grid->Add (new wxStaticText (this, -1, wxT("Output flow (m^3):")));
	edit_flow = new double_edit (this, EDIT_FLOW, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add(edit_flow, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, -1, wxT("Output sediment (t):")));
	edit_sediment = new double_edit (this, EDIT_SEDIMENT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add(edit_sediment, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, -1, wxT("Organic Nitrogen (kg):")));
	edit_organic_n = new double_edit (this, EDIT_ORGANIC_N, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add(edit_organic_n, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, -1, wxT("Organic Phosphorus (kg):")));
	edit_organic_p = new double_edit (this, EDIT_ORGANIC_P, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add(edit_organic_p, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, -1, wxT("Nitrate (kg):")));
	edit_nitrate = new double_edit (this, EDIT_NITRATE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add(edit_nitrate, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, -1, wxT("Soluble Phosphorus (kg):")));
	edit_soluble_p = new double_edit (this, EDIT_SOLUBLE_P, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add(edit_soluble_p, 0, wxALIGN_RIGHT);

	grid->Add (new wxStaticText (this, -1, wxT("Soluble Pesticide (mg):")));
	edit_soluble_pst = new double_edit (this, EDIT_SOLUBLE_PST, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	grid->Add(edit_soluble_pst, 0, wxALIGN_RIGHT);

	SetSizerAndFit (grid);

	static_command_number->Set (node->CommandNumber);
	if (node->output) {
		edit_flow->Set (node->output->Flow);
		edit_sediment->Set (node->output->Sediment);
		edit_organic_n->Set (node->output->OrganicNitrogen);
		edit_organic_p->Set (node->output->OrganicPhosphorus);
		edit_nitrate->Set (node->output->Nitrate);
		edit_soluble_p->Set (node->output->SolublePhosphorus);
		edit_soluble_pst->Set (node->output->SolublePesticide);
	}
}

page_routing_output::~page_routing_output()
{
}

