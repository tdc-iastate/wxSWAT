

class gridwidget : public wxGrid {
	public:
		gridwidget
			(class wxWindow *parent,
			const int rows,
			const int columns,
			const int id);
		~gridwidget();

		void SetCellFgColour(wxColour &);
		void SetCellBgColour(wxColour &);

		void InsertRow( wxCommandEvent& );
		void InsertCol( wxCommandEvent& );
		void DeleteSelectedRows( wxCommandEvent& );
		void DeleteSelectedCols( wxCommandEvent& );
		void SelectCells( wxCommandEvent& );
		void SelectRows( wxCommandEvent& );
		void SelectCols( wxCommandEvent& );
		void SelectRowsOrCols( wxCommandEvent& );

		void DeselectAll(wxCommandEvent& event);
		void SelectCell(wxCommandEvent& event);
		void OnAddToSelectToggle(wxCommandEvent& event);

		void AutoSizeRowLabel(wxCommandEvent& event);
		void AutoSizeColLabel(wxCommandEvent& event);
		void AutoSizeLabelsCol(wxCommandEvent& event);
		void AutoSizeLabelsRow(wxCommandEvent& event);
		void AutoSizeTable(wxCommandEvent& event);

		void OnLabelLeftClick( wxGridEvent& );
		void OnCellLeftClick( wxGridEvent& );
		void OnRowSize( wxGridSizeEvent& );
		void OnColSize( wxGridSizeEvent& );
		void OnSelectCell( wxGridEvent& );
		void OnRangeSelected( wxGridRangeSelectEvent& );
		void OnCellValueChanging( wxGridEvent& );
		void OnCellValueChanged( wxGridEvent& );
		void OnCellBeginDrag( wxGridEvent& );

		void OnEditorShown(wxGridEvent&);
		void OnEditorHidden(wxGridEvent&);

		void OnSetHighlightWidth(wxCommandEvent&);
		void OnSetROHighlightWidth(wxCommandEvent&);

	/*
    enum
    {
        ID_TOGGLEROWLABELS = 100,
        ID_TOGGLECOLLABELS,
        ID_TOGGLEEDIT,
        ID_TOGGLEROWSIZING,
        ID_TOGGLECOLSIZING,
        ID_TOGGLECOLMOVING,
        ID_TOGGLEGRIDSIZING,
        ID_TOGGLEGRIDDRAGCELL,
        ID_TOGGLEGRIDLINES,
        ID_AUTOSIZECOLS,
        ID_CELLOVERFLOW,
        ID_RESIZECELL,
        ID_SETLABELCOLOUR,
        ID_SETLABELTEXTCOLOUR,
        ID_SETLABEL_FONT,
        ID_ROWLABELALIGN,
        ID_ROWLABELHORIZALIGN,
        ID_ROWLABELVERTALIGN,
        ID_COLLABELALIGN,
        ID_COLLABELHORIZALIGN,
        ID_COLLABELVERTALIGN,
        ID_COLDEFAULTHEADER,
        ID_COLNATIVEHEADER,
        ID_COLCUSTOMHEADER,
        ID_GRIDLINECOLOUR,
        ID_INSERTROW,
        ID_INSERTCOL,
        ID_DELETEROW,
        ID_DELETECOL,
        ID_CLEARGRID,
        ID_CHANGESEL,
        ID_SELCELLS,
        ID_SELROWS,
        ID_SELCOLS,
        ID_SELROWSORCOLS,
        ID_SET_CELL_FG_COLOUR,
        ID_SET_CELL_BG_COLOUR,
        ID_VTABLE,
        ID_BUGS_TABLE,
        ID_TABULAR_TABLE,
        ID_SELECT_UNSELECT,
        ID_SELECT_ALL,
        ID_SELECT_ROW,
        ID_SELECT_COL,
        ID_SELECT_CELL,
        ID_DESELECT_ALL,
        ID_DESELECT_ROW,
        ID_DESELECT_COL,
        ID_DESELECT_CELL,
        ID_SIZE_ROW,
        ID_SIZE_COL,
        ID_SIZE_ROW_LABEL,
        ID_SIZE_COL_LABEL,
        ID_SIZE_LABELS_COL,
        ID_SIZE_LABELS_ROW,
        ID_SIZE_GRID,

        ID_SET_HIGHLIGHT_WIDTH,
        ID_SET_RO_HIGHLIGHT_WIDTH,

        ID_TESTFUNC
    };
	*/

    // add the cells to selection when using commands from select menu?
    bool m_addToSel;

    DECLARE_EVENT_TABLE()
};