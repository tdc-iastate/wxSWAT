#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <mutex>
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../util/interface_window.h"
#include "../util/interface_window_wx.h"
#include "../util/device_coordinate.h" // for map_point and map_rectangle
#include "../util/bounding_cube.h"
#include "../map/color_set.h"
#include "../map/dynamic_map.h"
#include "../map/dynamic_map_wx.h"
// #include "../util/interface_tree.h"
// for all others, include the necessary headers
#include "wx/app.h"
// #include "wx/log.h"
#include "wx/frame.h"
#include <wx/panel.h>
#include <wx/stattext.h>
#include "wx/menu.h"
#include "wx/layout.h"
#include "wx/msgdlg.h"
#include "wx/icon.h"
#include "wx/button.h"
#include "wx/sizer.h"
#include "wx/textctrl.h"
#include "wx/settings.h"
#include <wx/grid.h>
#include <wx/listbox.h>
#include <wx/calctrl.h>
#include <wx/splitter.h>

#ifdef wxHAS_NATIVE_CALENDARCTRL
    #include "wx/generic/calctrlg.h" // wxmsw29ud_adv.lib
#endif

#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"

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
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATOutputHRUAnnual.h"
#include "../i_swat_database/SWATOutputHRUMonthly.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
#include "../i_swat_database/SWATOutputSubBasinDaily.h"
#include "../i_swat_database/SWATOutputHRUDaily.h"
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
#include "../i_swat_database/SWATSepticWaterQuality.h"
#include "../i_swat_database/SWATSeptic.h"
#include "frame_swat.h"
#include "panel_routing_tree.h"

panel_node::panel_node ()
{
	id = 0;
	HSL = 0;
	command = 0;
	destination = 0;
	center_x = center_y = width = height = 0;
	HSL1 = 0;
	HSL2 = 0;
}

panel_node::panel_node
	(const panel_node &other)

{
	copy (other);
}

panel_node panel_node::operator =
	(const panel_node &other)

{
	copy (other);
	return *this;
}

void panel_node::copy
	(const panel_node &other)

{
	id = other.id;
	HSL = other.HSL;
	command = other.command;
	destination = other.destination;
	center_x = other.center_x;
	center_y = other.center_y;
	width = other.width;
	height = other.height;
	label = other.label;
	HSL1 = other.HSL1;
	HSL2 = other.HSL2;
}

int panel_node::distance_squared
	(const panel_node *other) const

{
	return (center_x - other->center_x) * (center_x - other->center_x) + (center_y - other->center_y) * (center_y - other->center_y);
}

void panel_node::move_to_distance_squared
	(const panel_node *other,
	const int d2)

// Along the line from this to other, move to the point whose distance_squared is d2

{
	if (center_x == other->center_x) {
		// move straight up or down
		if (other->center_y > center_y)
			center_y += sqrt (d2);
		else
			center_y -= sqrt (d2);
	}
	else
		if (center_y == other->center_y) {
			// move straight left or right
			if (other->center_x > center_x)
				center_x += sqrt (d2);
			else
				center_x -= sqrt (d2);
		}
		else {
			double slope = (double) (other->center_y - (double) center_y) / (double) (other->center_x - (double) center_x);
			double y_intercept = (double) center_y - (slope * (double) center_x);
			double new_x, new_y;

			// http://math.stackexchange.com/questions/409689/how-do-i-find-a-point-a-given-distance-from-another-point-along-a-line
			if (center_x > other->center_x)
				new_x = center_x - sqrt (d2 / (1 + (slope * slope)));
			else
				new_x = center_x + sqrt (d2 / (1 + (slope * slope)));
			new_y = (slope * new_x) + y_intercept;

			// double new_d_2 = (new_x - x) * (new_x - x) + (new_y - y) * (new_y - y);

			center_x = (int) new_x;
			center_y = (int) new_y;
		}
}

bool panel_node::overlaps
	(const wxPoint &click) const
{
	if ((center_x - (width / 2) > click.x)
	|| (center_x + (width / 2) < click.x))
		// most of the time we can skip out early
		return false;
	else {
		if ((center_y - (height / 2) > click.y)
		|| (center_y + (height / 2) < click.y))
			return false;
		else {
			if ((center_x - (width / 2) <= click.x)
			&& (center_x + (width / 2) >= click.x)) {
				// Check for y overlap
				if ((center_y - (height / 2) <= click.y)
				&& (center_y + (height / 2) >= click.y))
					return true;
			}
		}
	}
	return false;
}

void panel_node::write
	(const panel_node *dest,
	dynamic_string &log) const

{
	log.add_formatted ("\t%ld\t%ld\t%ld\t%d\t%d", HSL, id, destination, center_x, center_y);
	log += "\t";
	log += label;
	if (dest)
		log.add_formatted ("\t%d", distance_squared (dest));
}

BEGIN_EVENT_TABLE(panel_routing_tree, wxPanel)
	EVT_KEY_UP(panel_routing_tree::OnKeyUp)
	EVT_PAINT(panel_routing_tree::OnPaint)
	EVT_LEFT_UP(panel_routing_tree::OnMouseLeftUp)
	EVT_RIGHT_UP(panel_routing_tree::OnMouseRightUp)
	EVT_LEFT_DCLICK(panel_routing_tree::OnCalDblClick)
END_EVENT_TABLE()

panel_routing_tree::panel_routing_tree
	(frame_swat *parent,
	const int id)
	: wxScrolledWindow (parent, id, wxDefaultPosition, wxSize (1600, 1200))
{
	frame = parent;
	modification_count = 0;
	subbasin_width = 30;
	subbasin_height = 20;
	route_width = route_height = 12;
	add_width = add_height = 16;
	selection = NULL;
	node_size_proportional = false;
}

void panel_routing_tree::recompute_sizes ()

{
	Refresh ();
}


void panel_routing_tree::OnKeyUp
	(wxKeyEvent &key_event)

{
	switch (key_event.m_keyCode) {
		case WXK_RIGHT:
			break;
		case 'd':
		case 'D':
			arrange_distances ();
			break;
		case 'o':
		case 'O':
			arrange_overlaps ();
			break;
		case 'N':
			// lower case 'p' appears as 'P'
			node_size_proportional = true;
			arrange_proportion ();
			break;
		case 'P':
			node_size_proportional = false;
			arrange_proportion ();
			break;
		case 'r':
		case 'R':
			arrange_recursive ();
			break;
		case 's':
		case 'S':
			arrange_swing ();
			break;
		case WXK_LEFT:
			Refresh ();
	}
}

/*
panel_tier *panel_routing_tree::match_hsl_tier
	(const long hsl)

{
	std::vector <panel_tier *>::iterator tier;
	panel_tier *found = NULL;

	for (tier = tiers.begin ();
	!found
	&& (tier != tiers.end ());
	++tier) {
		if ((*tier)->match_hsl (hsl))
			found = *tier;
	}

	return found;
}
*/

/*
panel_node *panel_routing_tree::match_hsl
	(const long hsl) const

{
	std::map <long, panel_node>::const_iterator node;

	if ((node = nodes.find (hsl)) != nodes.end ())
		return (panel_node *) &node->second;
	else
		return NULL;
}
*/

bool panel_routing_tree::descends_through
	(const class panel_node *ancestor,
	const class panel_node *descendent) const

// True if anscestor routes through descendent sometime

{
	std::map <long, panel_node>::const_iterator child, grandchild;
	bool found = false;
	
	child = nodes.find (ancestor->HSL);

	while (!found
	&& ((grandchild = nodes.find (child->second.destination)) != nodes.end ())) {
		if (grandchild->second.HSL == descendent->HSL)
			found = true;
		child = grandchild;
	}

	return found;
}

void panel_routing_tree::apply_offset_upstream
	(const class panel_node *start,
	const wxSize offset)

// offset position of start & everything upstream

{
	std::map <long, panel_node>::iterator node;

	for (node = nodes.begin ();
	node != nodes.end ();
	++node) {
		if ((node->second.HSL == start->HSL)
		|| descends_through (&node->second, start)) {
			node->second.center_x += offset.x;
			node->second.center_y += offset.y;
		}
	}
}

bool panel_routing_tree::find_add_sources
	(const class panel_node *destination,
	class panel_node * sources []) const

// Find the two nodes whose destination is this

{
	std::map <long, panel_node>::const_iterator node;
	int found = 0;

	for (node = nodes.begin ();
	(found < 2) && (node != nodes.end ());
	++node) {
		if (node->second.destination == destination->HSL) {
			sources [found] = (panel_node *) &node->second;
			++found;
		}
	}

	return found == 2;
}

panel_node *panel_routing_tree::find_route_source
	(const class panel_node *destination) const

// Find the two nodes whose destination is this

{
	std::map <long, panel_node>::const_iterator node;
	panel_node *source = NULL;

	for (node = nodes.begin ();
	!source && (node != nodes.end ());
	++node) {
		if (node->second.destination == destination->HSL)
			source = (panel_node *) &node->second;
	}

	return source;
}

void panel_routing_tree::write
	(dynamic_string &log) const

{
	std::map <long, panel_node>::const_iterator node, dest;

	for (node = nodes.begin ();
	node != nodes.end ();
	++node) {
		dest = nodes.find (node->second.destination);
		node->second.write (&dest->second, log);
		log += "\n";
	}
}

int panel_routing_tree::distance_to_outlet
	(std::vector <SWATRouting> *raw_list,
	SWATRouting *basin)

{
	std::vector <SWATRouting>::const_iterator raw_route;
	SWATRouting *cursor;
	int count = 0;
	bool match = true;

	cursor = basin;

	while (match) {
		match = false;

		for (raw_route = raw_list->begin ();
		!match
		&& (raw_route != raw_list->end ());
		++raw_route) {
			switch (raw_route->Command) {
				case SWAT_ROUTING_COMMAND_ADD:
					if ((raw_route->Parameter1 == cursor->HydrographStorageLocation)
					|| (raw_route->Parameter2 == cursor->HydrographStorageLocation)) {
						cursor = (SWATRouting *) &*raw_route;
						++count;
						match = true;
					}
					break;
				case SWAT_ROUTING_COMMAND_ROUTE:
				case SWAT_ROUTING_COMMAND_ROUTRES:
					if (raw_route->Parameter2 == cursor->HydrographStorageLocation) {
						cursor = (SWATRouting *) &*raw_route;
						++count;
						match = true;
					}
			}
		}
	}
	return count;
}

int panel_routing_tree::resolve_overlaps
	(const bounding_cube &field)

// panel_node x & y are top left corner

{
	std::map <long, panel_node>::iterator cursor, obscured_cursor;
	int shift_count = 0;
	bounding_cube node_cube, obscured_cube;

	bool shifted;
	double shift_direction_x, shift_direction_y;

	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		node_cube.x [0] = cursor->second.center_x;
		node_cube.x [1] = node_cube.x [0] + cursor->second.width;
		node_cube.y [0] = cursor->second.center_y;
		node_cube.y [1] = node_cube.y [0] + cursor->second.height;

		if (cursor->second.center_x < (int) field.x [0])
			shift_direction_x = 1.0;
		else
			if (cursor->second.center_x > (int) field.x [1])
				shift_direction_x = -1.0;
			else
				shift_direction_x = rand () % 2 ? 1.0 : -1.0;

		if (cursor->second.center_y < (int) field.y [0])
			shift_direction_y = 1.0;
		else
			if (cursor->second.center_y > (int) field.y [1])
				shift_direction_y = -1.0;
			else
				shift_direction_y = rand () % 2 ? 1.0 : -1.0;

		for (obscured_cursor = nodes.begin ();
		obscured_cursor != nodes.end ();
		++obscured_cursor) {
			if (cursor != obscured_cursor) {
				shifted = false;
				obscured_cube.x [0] = obscured_cursor->second.center_x;
				obscured_cube.x [1] = obscured_cube.x [0] + obscured_cursor->second.width;
				obscured_cube.y [0] = obscured_cursor->second.center_y;
				obscured_cube.y [1] = obscured_cube.y [0] + obscured_cursor->second.height;

				while (node_cube.overlaps (obscured_cube)) {
					node_cube.offset (shift_direction_x, shift_direction_y, 0.0);
					cursor->second.center_x += (int) shift_direction_x;
					cursor->second.center_y += (int) shift_direction_y;
					shifted = true;
				}
				if (shifted)
					++shift_count;
			}
		}

	}
	return shift_count;
}


int panel_routing_tree::moderate_distances ()

// Shift nodes until destination distance is reasonable
{
	std::map <long, panel_node>::iterator cursor, destination;
	int shift_count = 0, distance_2;
	bounding_cube node_cube;
	int limit_close, limit_far, limit_best;

	bool shifted;
	// double shift_direction_x, shift_direction_y;

	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		// compute vector to destination, & move in that direction if possible
		if ((destination = nodes.find (cursor->second.destination)) != nodes.end ()) {

			node_cube.x [0] = cursor->second.center_x;
			node_cube.x [1] = node_cube.x [0] + cursor->second.width;
			node_cube.y [0] = cursor->second.center_y;
			node_cube.y [1] = node_cube.y [0] + cursor->second.height;

			distance_2 = cursor->second.distance_squared (&destination->second);
			shifted = false;

			/*
			if (cursor->second.command == SWAT_ROUTING_COMMAND_SUBBASIN) {
				// keep subbasin close to its route command
				limit_close = 4000;
				limit_far = 5000;
				limit_best = 4500;
			}
			else {
				limit_close = 10000;
				limit_far = 15000;
				limit_best = 10000;
			}
			*/
			limit_close = 10000;
			limit_far = 14000;
			limit_best = 12000;

			if (distance_2 > limit_far) {
				// move to a point sqrt (limit_close) away from destination
				cursor->second.move_to_distance_squared (&destination->second, limit_best);
				shifted = true;
			}
			else
				if (distance_2 < limit_close) {
					cursor->second.move_to_distance_squared (&destination->second, limit_best);
					shifted = true;
					/*
					// Move away
					if (cursor->second.x > destination->x)
						shift_direction_x = 1.0;
					else
						if (cursor->second.x < destination->x)
							shift_direction_x = -1.0;
						else
							shift_direction_x = 0.0;

					if (cursor->second.y > destination->y)
						shift_direction_y = 1.0;
					else
						if (cursor->second.y < destination->y)
							shift_direction_y = -1.0;
						else
							shift_direction_y = 0.0;

					if ((shift_direction_x == 0)
					&& (shift_direction_y == 0)) {
						// infinite loop when node & destination coincide.  Move randomly to avoid
						shift_direction_x = rand () % 2 ? 1.0 : -1.0;
						shift_direction_y = rand () % 2 ? 1.0 : -1.0;
					}

					while ((distance_2 = cursor->second.distance_squared (destination)) < limit_close) {
						cursor->second.x += (int) (shift_direction_x * 10.0);
						cursor->second.y += (int) (shift_direction_y * 10.0);
						shifted = true;
						if (cursor->second.y > 1200) {
							int z = 99;
						}
					}
					*/
				}

			if (shifted)
				++shift_count;
		}
	}
	return shift_count;
}

void panel_routing_tree::reproportion
	(const bounding_cube &field)

// squish or expand positions in horizontal and vertical directions as necessary to fill the field

{
	std::map <long, panel_node>::iterator cursor;
	bounding_cube node_cube, footprint, margin_field;
	double x_offset, y_offset, x_factor, y_factor;

	margin_field = field;
	margin_field.deflate (0.95);

	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		node_cube.x [0] = cursor->second.center_x;
		node_cube.x [1] = node_cube.x [0] + cursor->second.width;
		node_cube.y [0] = cursor->second.center_y;
		node_cube.y [1] = node_cube.y [0] + cursor->second.height;

		footprint.check_extent (node_cube);
	}

	x_factor = (margin_field.x [1] - margin_field.x [0]) / (footprint.x [1] - footprint.x [0]);
	y_factor = (margin_field.y [1] - margin_field.y [0]) / (footprint.y [1] - footprint.y [0]);

	if (x_factor > y_factor) {
		// stretching wider because north-south is a closer fit
		x_factor = 1.0;
	}
	else
		if (y_factor > x_factor)
			y_factor = 1.0;

	x_offset = margin_field.x [0] - footprint.x [0];
	y_offset = margin_field.y [0] - footprint.y [0];

	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		cursor->second.center_x = (int) ((double) cursor->second.center_x * x_factor) + x_offset;
		cursor->second.center_y = (int) ((double) cursor->second.center_y * y_factor) + y_offset;

		if (node_size_proportional) {
			// 2014-07-30 shrinking boxes in proportion causes boxes too small to see
			cursor->second.width *= x_factor;
			cursor->second.height *= y_factor;
		}
	}
}

wxSize panel_routing_tree::recursively_position
	(panel_node *node,
	const int level,
	const int x,
	const int y)

{
	wxSize size_above;
	if (node->command == SWAT_ROUTING_COMMAND_ADD) {
		panel_node *sources [2];
		wxSize size_left, size_right;
		find_add_sources (node, sources);
		if (sources [0]->center_x > sources [1]->center_x) {
			int temp = sources [0]->center_x;
			sources [0]->center_x = sources [1]->center_x;
			sources [1]->center_x = temp;
		}
		// need results: descendent's width before setting x for child
		// set sources' x position after call?
		size_left = recursively_position (sources [0], level + 1, x, y - 100);
		size_right = recursively_position (sources [1], level + 1, x, y - 100);

		size_above.x = size_left.x + 10 + size_right.x;
		size_above.y = size_left.y > size_right.y ? size_left.y : size_right.y;
		apply_offset_upstream (sources [0], wxSize (-size_above.x / 2, 0));
		apply_offset_upstream (sources [1], wxSize (size_above.x / 2, 0));

		node->center_x = x;
		node->center_y = y;
		// *width_above = total_width;
	}
	else
		if ((node->command == SWAT_ROUTING_COMMAND_ROUTE)
		|| (node->command == SWAT_ROUTING_COMMAND_ROUTRES)) {
			panel_node *source;
			source = find_route_source (node);
			size_above = recursively_position (source, level + 1, x, y - 100);

			/*
			if (*width_above > subbasin_width)
				// more than just a single subbasin
				apply_offset_upstream (source, wxSize (-(*width_above - node->width) / 2, 0));
			*/

			node->center_x = x;
			node->center_y = y;
		}
		else {
			node->center_x = x;
			node->center_y = y;
			size_above.x = subbasin_width;
			size_above.y = subbasin_height;
		}
	return size_above;
}

void panel_routing_tree::arrange_recursive ()

{
	std::map <long, panel_node>::iterator cursor;
	panel_node *outlet = NULL;
	int level = 0, x, y;
    wxClientDC dc(this);
	wxSize boundary;

	boundary = dc.GetSize ();

	y = boundary.y - 25;
	x = boundary.x / 2;

	for (cursor = nodes.begin ();
	!outlet && (cursor != nodes.end ());
	++cursor) {
		if (cursor->second.destination == 0)
			outlet = &cursor->second;
	}

	map_size = recursively_position (outlet, level, x, y);
	++modification_count;

	Refresh ();
}

void panel_routing_tree::arrange_overlaps ()

{
	bounding_cube field;
	std::map <long, panel_node>::iterator cursor;

	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		field.check_extent (cursor->second.center_x - (cursor->second.width / 2), cursor->second.center_y - (cursor->second.height / 2), 0.0);
		field.check_extent (cursor->second.center_x + (cursor->second.width / 2), cursor->second.center_y + (cursor->second.height / 2), 0.0);
	}
	while (resolve_overlaps (field) > 0) {
		++modification_count;
		Refresh ();
	}
}

void panel_routing_tree::arrange_swing ()

// if Add, move nodes & ancestors to NW & NE positions
// if route, move it & ancestors straight up

{
	bounding_cube field;
	std::map <long, panel_node>::iterator cursor;
	wxSize offset;

	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		if (cursor->second.command == SWAT_ROUTING_COMMAND_ADD) {
			// move nodes & ancestors to NW & NE positions
			wxSize nw, ne;
			panel_node *sources [2];

			nw.Set (cursor->second.center_x - 40, cursor->second.center_y + 50);
			ne.Set (cursor->second.center_x + 40, cursor->second.center_y + 50);

			find_add_sources (&cursor->second, sources);

			offset.x = nw.x - sources [0]->center_x;
			offset.y = nw.y - sources [0]->center_y;

			apply_offset_upstream (sources [0], offset);

			offset.x = ne.x - sources [1]->center_x;
			offset.y = ne.y - sources [1]->center_y;
			apply_offset_upstream (sources [1], offset);

			++modification_count;
		}
		else
			if ((cursor->second.command == SWAT_ROUTING_COMMAND_ROUTE)
			|| (cursor->second.command == SWAT_ROUTING_COMMAND_ROUTRES)) {
				panel_node *source;
				wxSize north;

				north.Set (cursor->second.center_x, cursor->second.center_y + 50);
				source = find_route_source (&cursor->second);

				offset.x = north.x - source->center_x;
				offset.y = north.y - source->center_y;

				apply_offset_upstream (source, offset);
				++modification_count;
			}
	}
	Refresh ();
}

void panel_routing_tree::arrange_proportion ()

{
	bounding_cube field;
    wxClientDC dc(this);
	wxSize boundary;

	boundary = dc.GetSize ();

	field.x [0] = 0;
	field.x [1] = boundary.x;
	field.y [0] = 0;
	field.y [1] = boundary.y;

	/*
	std::map <long, panel_node>::iterator cursor;
	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		field.check_extent (cursor->second.x, cursor->second.y, 0.0);
		field.check_extent (cursor->second.x + cursor->second.width, cursor->second.y + cursor->second.height, 0.0);
	}
	*/
	reproportion (field);
	++modification_count;
	Refresh ();
}

void panel_routing_tree::arrange_distances ()

{
	moderate_distances ();
	++modification_count;
	Refresh ();
}

bool panel_routing_tree::build_tree
	(std::vector <SWATRouting> *raw_list,
	dynamic_string &log)

{
	std::vector <SWATRouting>::const_iterator raw_route;
	std::map <long, panel_node>::iterator basin, basin1, basin2;
	panel_node new_basin;
	bool error = false;

	for (raw_route = raw_list->begin ();
	!error
	&& (raw_route != raw_list->end ());
	++raw_route) {
		// Find the next appearence of current_hsl in some other node
		switch (raw_route->Command) {
			case SWAT_ROUTING_COMMAND_ADD:
				/*
				// if tier is not already higher than source tiers, increment it
				source1 = match_hsl_tier (raw_route->Parameter1);
				source2 = match_hsl_tier (raw_route->Parameter2);
				if (tier->level <= source1->level) {
					old_level = tier->level;
					tier = new panel_tier;
					tier->level = ++old_level;
					tiers.push_back (tier);
				}

				if (tier->level <= source2->level) {
					old_level = tier->level;
					tier = new panel_tier;
					tier->level = ++old_level;
					tiers.push_back (tier);
				}
				*/
				if (((basin1 = nodes.find (raw_route->Parameter1)) != nodes.end ())
				&& ((basin2 = nodes.find (raw_route->Parameter2)) != nodes.end ())) {

					new_basin.HSL = raw_route->HydrographStorageLocation;
					new_basin.command = raw_route->Command;
					new_basin.HSL1 = raw_route->Parameter1;
					new_basin.HSL2 = raw_route->Parameter2;
					new_basin.width = add_width;
					new_basin.height = add_height;
					new_basin.center_x = (basin1->second.center_x + basin2->second.center_x) / 2 + basin1->second.center_x;
					// new_basin.center_y = distance_to_outlet (raw_list, (SWATRouting *) &*raw_route) * new_basin.height;
					new_basin.center_y = (basin1->second.center_y + basin2->second.center_y) / 2 + basin1->second.center_y;
					// if (basin1->y > basin2->y)
					//	new_basin.y = basin1->y + node_size_y;
					//else
					//	new_basin.y = basin2->y + node_size_y;
					new_basin.label = "+";
					nodes.insert (std::pair <long, panel_node> (new_basin.HSL, new_basin));
				}

				break;
			case SWAT_ROUTING_COMMAND_ROUTE:
			case SWAT_ROUTING_COMMAND_ROUTRES:
				/*
				// discard by replacing HSL of source with this HSL
				basin = match_hsl (raw_route->Parameter2);
				basin->HSL = raw_route->HydrographStorageLocation;
				*/
				if ((basin = nodes.find (raw_route->Parameter2)) != nodes.end ()) {
					new_basin.HSL = raw_route->HydrographStorageLocation;
					new_basin.id = raw_route->Parameter5;
					new_basin.command = raw_route->Command;
					new_basin.HSL1 = raw_route->Parameter2;
					new_basin.width = route_width;
					new_basin.height = route_height;
					new_basin.center_x = basin->second.center_x;
					new_basin.center_y = distance_to_outlet (raw_list, (SWATRouting *) &*raw_route) * new_basin.height;
					new_basin.label = "v";
					nodes.insert (std::pair <long, panel_node> (new_basin.HSL, new_basin));
				}
				break;
			case SWAT_ROUTING_COMMAND_SUBBASIN:
				new_basin.HSL = raw_route->HydrographStorageLocation;
				new_basin.id = raw_route->Parameter5;
				new_basin.command = raw_route->Command;

				new_basin.width = subbasin_width;
				new_basin.height = subbasin_height;
				new_basin.center_x = (nodes.size () % 30) * new_basin.width;
				new_basin.center_y = distance_to_outlet (raw_list, (SWATRouting *) &*raw_route) * new_basin.height;
				new_basin.label.format ("%ld", new_basin.HSL);
				nodes.insert (std::pair <long, panel_node> (new_basin.HSL, new_basin));
			//	break;
			// default:
				// log.add_formatted ("build_tree: error unknown command %d \"", (int) raw_route->Command);
				// log += route_command_name (raw_route->Command);
				// log += "\".\n";
				// error = true;
		}
	}

	// 2nd pass to set destination of nodes for ADD & ROUTE
	for (basin = nodes.begin ();
	basin != nodes.end ();
	++basin) {
		switch (basin->second.command) {
			case SWAT_ROUTING_COMMAND_ADD:
				if (((basin1 = nodes.find (basin->second.HSL1)) != nodes.end ())
				&& ((basin2 = nodes.find (basin->second.HSL2)) != nodes.end ())) {
					basin1->second.destination = basin->second.HSL;
					basin2->second.destination = basin->second.HSL;
				}
				else {
					log.add_formatted ("ERROR, ADD command source(s) not found: %ld and %ld", basin->second.HSL1, basin->second.HSL2);
					error = true;
				}

				break;
			case SWAT_ROUTING_COMMAND_ROUTE:
			case SWAT_ROUTING_COMMAND_ROUTRES:
				if ((basin1 = nodes.find (basin->second.HSL1)) != nodes.end ())
					basin1->second.destination = basin->second.HSL;
		}

	}


	return !error;
}

bool overlap
	(const int center1,
	const int size1,
	const int center2,
	const int size2)

{
	if (center1 - (size1 / 2) > center2 + (size2 / 2))
		// 1 is right of 2
		return false;
	else
		if (center1 + (size1 / 2) < center2 - (size2 / 2))
			// 1 is left of 2
			return false;
		else
			return true;
}

void panel_routing_tree::draw_arrow
	(wxDC &dc,
	const panel_node *source,
	const panel_node *destination,
	const COLORREF color)

{
	int wingspan = 6, x1, x2, y1, y2;

	if (overlap (source->center_x, source->width, destination->center_x, destination->width)) {
		x1 = source->center_x;
		x2 = destination->center_x;
	}
	else
		if (destination->center_x > source->center_x) {
			// to the right
			x1 = source->center_x + (source->width / 2);
			x2 = destination->center_x - (destination->width / 2);
		}
		else {
			// to the left
			x1 = source->center_x - (source->width / 2);
			x2 = destination->center_x + (destination->width / 2);
		}

	if (overlap (source->center_y, source->height, destination->center_y, destination->height)) {
		y1 = source->center_y;
		y2 = destination->center_y;
	}
	else
		if (destination->center_y > source->center_y) {
			// down
			y1 = source->center_y + (source->height / 2);
			y2 = destination->center_y - (destination->height / 2);
		}
		else {
			// up
			y1 = source->center_y - (source->height / 2);
			y2 = destination->center_y + (destination->height / 2);
		}
	DrawArrow (&dc, wxPoint (x1, y1), wxPoint (x2, y2), wingspan, color);
}

void panel_routing_tree::OnPaint
	(wxPaintEvent &)

{
    wxPaintDC dc(this);

	std::map <long, panel_node>::iterator cursor, destination;
	dynamic_string label;
	panel_node *node;
	wxPen pen;
	wxBrush brush;
	wxSize canvas_size;
	COLORREF color_lines = RGB (255, 0, 0), color_subbasin = RGB (0, 0, 255), color_add = RGB (0, 255, 0), color;

	wxFont label_font (9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	dc.SetFont (label_font);
	// dc.SetTextForeground (RGB (0, 0, 0));
	// dc.SetTextBackground (RGB (255, 255, 255));
	// dc.SetBackgroundMode (wxBG_STYLE_TRANSPARENT);
	dc.SetBackgroundMode (TRANSPARENT);

	label.format ("Mod: %d", modification_count);
	dc.DrawText (label.get_text_ascii (), 10, 10);

	canvas_size = dc.GetSize ();

	SetScrollbar(wxHORIZONTAL, 0, canvas_size.x, map_size.x);
	SetScrollbar(wxVERTICAL, 0, canvas_size.y, map_size.y);

	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		node = &cursor->second;

		if (node->command == SWAT_ROUTING_COMMAND_SUBBASIN)
			color = color_subbasin;
		else
			if (node->command == SWAT_ROUTING_COMMAND_ADD)
				color = color_add;
			else
				color = color_lines;

		pen.SetColour (RGB (0, 0, 0));
		pen.SetWidth (1);
		dc.SetPen (pen);
		brush.SetColour (color);
		dc.SetBrush (brush);

		dc.DrawRectangle (node->center_x - (node->width / 2), node->center_y - (node->height / 2), node->width, node->height);

		label = node->label;
		if (node->command == SWAT_ROUTING_COMMAND_SUBBASIN)
			// dc.DrawText (label.get_text_ascii (), node->center_x - (node->width * 2 / 5), node->center_y - (node->height * 2 / 5));
			dc.DrawLabel (label.get_text_ascii (),
			wxRect (node->center_x - (node->width / 2), node->center_y - (node->height / 2), node->width, node->height),
			wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTRE_VERTICAL);
		else
			if (node->command == SWAT_ROUTING_COMMAND_ADD) {
				// Add '+'
				dc.DrawLine (node->center_x, node->center_y - (node->height / 2), node->center_x, node->center_y + (node->height / 2));
				dc.DrawLine (node->center_x - (node->width / 2), node->center_y, node->center_x + (node->width / 2), node->center_y);
			}
			else {
				// Route down arrow
				pen.SetColour (RGB (255, 255, 255));
				dc.SetPen (pen);
				dc.DrawLine (node->center_x, node->center_y - (node->height / 2), node->center_x, node->center_y + (node->height / 2));
				dc.DrawLine (node->center_x - (node->width / 2), node->center_y, node->center_x, node->center_y + (node->height / 2));
				dc.DrawLine (node->center_x + (node->width / 2), node->center_y, node->center_x, node->center_y + (node->height / 2));
			}

		if (node == selection) {
			pen.SetColour (RGB (255, 127, 39));
			pen.SetWidth (2);
			dc.SetPen (pen);
			// left
			dc.DrawLine (node->center_x - (node->width / 2) - 2, node->center_y - (node->height / 2) - 2, node->center_x - (node->width / 2) - 2, node->center_y + (node->height / 2) + 2);
			// right
			dc.DrawLine (node->center_x + (node->width / 2) + 2, node->center_y - (node->height / 2) - 2, node->center_x + (node->width / 2) + 2, node->center_y + (node->height / 2) + 2);
			// top
			dc.DrawLine (node->center_x - (node->width / 2) - 2, node->center_y - (node->height / 2) - 2, node->center_x + (node->width / 2) + 2, node->center_y - (node->height / 2) - 2);
			// bottom
			dc.DrawLine (node->center_x - (node->width / 2) - 2, node->center_y + (node->height / 2) + 2, node->center_x + (node->width / 2) + 2, node->center_y + (node->height / 2) + 2);
		}
	}

	// 2nd pass - Lines
	for (cursor = nodes.begin ();
	cursor != nodes.end ();
	++cursor) {
		node = &cursor->second;

		if (node->command == SWAT_ROUTING_COMMAND_SUBBASIN)
			color = color_subbasin;
		else
			color = color_lines;

		pen.SetColour (color);
		dc.SetPen (pen);
		brush.SetColour (color);
		dc.SetBrush (brush);

		// line from center to destination
		if (node->destination != 0) {
			destination = nodes.find (node->destination);
			draw_arrow (dc, node, &destination->second, color);
		}
	}
}

void panel_routing_tree::set_watershed
	(const class SWATControlRecord *control_record,
	dynamic_string &log)

{
 	if (control_record
	&& control_record->routing.outlet) {

		std::vector <SWATRouting> raw_list;

		interface_window_wx update_display;
		update_display.start_progress (this);

		// define new interface-independent version of interface_tree.add_leaf

		control_record->routing.build_raw_list (&raw_list);
		build_tree (&raw_list, log);
		modification_count = 0;

		arrange_recursive ();
		node_size_proportional = true;
		// arrange_proportion ();
		// arrange_proportion ();
		// write (log);
	}
}

panel_node *panel_routing_tree::match_position
	(const wxPoint &where)

{
	std::map <long, panel_node>::const_iterator cursor;
	panel_node *found = NULL;

	for (cursor = nodes.begin ();
	!found && (cursor != nodes.end ());
	++cursor) {
		if (cursor->second.overlaps (where))
			found = (panel_node *) &cursor->second;
	}

	return found;
}

void panel_routing_tree::OnMouseLeftUp (wxMouseEvent &e)

{
	panel_node *new_selection;


	if ((new_selection = match_position (e.GetPosition ())) != NULL) {
		selection = new_selection;

		dynamic_string label;
		label.format ("Selected HSL %ld", selection->HSL);
		if (selection->command == SWAT_ROUTING_COMMAND_SUBBASIN)
			label.add_formatted (" %ld", selection->id);
		interface_window_wx view;
		view.setup_wx (frame);
		view.update_status_bar (label.get_text_ascii ());
		Refresh ();
	}
}

void panel_routing_tree::OnMouseRightUp (wxMouseEvent &)

{
}

void panel_routing_tree::OnCalDblClick (wxMouseEvent &)

{
}
