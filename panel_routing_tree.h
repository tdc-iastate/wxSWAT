
class panel_node {
	public:
		long HSL, id;
		BYTE command;
		long destination;
		dynamic_string label;
		int center_x, center_y, width, height;
		long HSL1, HSL2; // sources for ADD
		panel_node ();
		panel_node
			(const panel_node &);
		panel_node operator =
			(const panel_node &);
		void copy
			(const panel_node &);

		int distance_squared
			(const class panel_node *other) const;

		void move_to_distance_squared
			(const panel_node *other,
			const int d2);

		bool overlaps
			(const wxPoint &other) const;

		void write
			(const class panel_node *dest,
			dynamic_string &) const;
};

/*
class panel_tier {
	public:
		int level;
		std::vector <panel_node> basins;

		panel_tier ();
		panel_tier
			(const panel_tier &);
		panel_tier operator =
			(const panel_tier &);
		void clear ();
		void copy
			(const panel_tier &);
		panel_node *match_hsl
			(const long);
		void write
			(dynamic_string &) const;
};
*/

class panel_routing_tree : public wxScrolledWindow {
	public:
		enum {
			ID_LIST_EVENTS = 103
		};

		panel_routing_tree
			(frame_swat *parent,
			const int id);
		void OnKeyUp (wxKeyEvent &);

		void recompute_sizes ();
	    void OnPaint(wxPaintEvent &);
		void OnMouseLeftUp (wxMouseEvent &);
		void OnMouseRightUp (wxMouseEvent &);
		void OnCalDblClick (wxMouseEvent &);
		void set_watershed
			(const class SWATControlRecord *,
			dynamic_string &log);

		void write
			(dynamic_string &) const;

	protected:
		int modification_count;
		int subbasin_width, subbasin_height, route_width, route_height, add_width, add_height;
		bool node_size_proportional;
		class panel_node *selection;
		wxSize map_size;

		class wxListBox *list_events;
		class frame_swat *frame;
		std::map <long, panel_node> nodes;

		void apply_offset_upstream
			(const class panel_node *,
			const wxSize offset);
		bool build_tree
			(std::vector <SWATRouting> *raw_list,
			dynamic_string &log);
		bool descends_through
			(const class panel_node *ancestor,
			const class panel_node *descendent) const;
		int distance_to_outlet
			(std::vector <SWATRouting> *,
			class SWATRouting *basin);
		void draw_arrow
			(wxDC &dc,
			const class panel_node *source,
			const class panel_node *destination,
			const COLORREF color);
		bool find_add_sources
			(const class panel_node *,
			class panel_node * []) const;
		class panel_node *find_route_source
			(const class panel_node *) const;
		class panel_node *match_position
			(const wxPoint &where);

		void arrange_distances ();
		void arrange_overlaps ();
		void arrange_proportion ();
		void arrange_swing ();
		void arrange_recursive ();

		int moderate_distances ();
		wxSize recursively_position
			(class panel_node *node,
			const int level,
			const int x,
			const int y);
		void reproportion
			(const bounding_cube &field);
		int resolve_overlaps
			(const bounding_cube &field);

		DECLARE_EVENT_TABLE()
};

