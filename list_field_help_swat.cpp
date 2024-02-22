#include "../util/utility_afx.h"
#include <vector>
// #include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/list_field_help.h"
#include "list_field_help_swat.h"
// #include "../i_swat_database/rowset_help_text.h"

list_field_help_swat::list_field_help_swat ()
	: list_field_help ()

{
}

/*
void list_field_help_swat::use_database
	(CDataSource *db)

{
	database = db;
}

BOOL list_field_help_swat::get_help
	(const char *field_name,
	const short version,
	CString &field_description)

{
	char found = FALSE;
	CString field_name_uppercase, filter;
	rowset_help_text set_help;
	HelpText help_record;

	// Open site info Table
	field_name_uppercase = field_name;
	field_name_uppercase.MakeUpper ();
	filter.Format ("[Name] = '%s'", field_name_uppercase);
	if (set_help.Open (database, filter) == S_OK) {
		if (set_help.MoveFirst () == S_OK) {
			help_record.read (&set_help);
			if (help_record.Units [0] != '\0')
				field_description.Format ("%s (%s) %s", help_record.Name, help_record.Units, help_record.Description);
			else
				field_description.Format ("%s %s", help_record.Name, help_record.Description);
			found = TRUE;
		}
		set_help.Close ();
	}

	return (found);
}
*/