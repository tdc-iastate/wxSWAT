#include "../util/utility_afx.h"
#include <wx/grid.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/generic/choicdgg.h>
#include "gridwidget.h"

BEGIN_EVENT_TABLE( gridwidget, wxGrid )
    EVT_GRID_LABEL_LEFT_CLICK( gridwidget::OnLabelLeftClick )
    EVT_GRID_CELL_LEFT_CLICK( gridwidget::OnCellLeftClick )
    EVT_GRID_ROW_SIZE( gridwidget::OnRowSize )
    EVT_GRID_COL_SIZE( gridwidget::OnColSize )
    EVT_GRID_SELECT_CELL( gridwidget::OnSelectCell )
    EVT_GRID_RANGE_SELECT( gridwidget::OnRangeSelected )
    EVT_GRID_CELL_CHANGING( gridwidget::OnCellValueChanging )
    EVT_GRID_CELL_CHANGED( gridwidget::OnCellValueChanged )
    EVT_GRID_CELL_BEGIN_DRAG( gridwidget::OnCellBeginDrag )

    EVT_GRID_EDITOR_SHOWN( gridwidget::OnEditorShown )
    EVT_GRID_EDITOR_HIDDEN( gridwidget::OnEditorHidden )
END_EVENT_TABLE()


gridwidget::gridwidget
	(wxWindow *parent,
	const int rows,
	const int columns,
	const int id)
	: wxGrid (parent, id, wxDefaultPosition, wxDefaultSize)

{

    m_addToSel = false;

    // this will create a grid and, by default, an associated grid
    // table for strings
    // CreateGrid (rows, columns);
}

/*
void gridwidget::setup ()

{
    // GetTable()->SetAttrProvider(new CustomColumnHeadersProvider());

    AppendRows(100);
    AppendCols(100);

    int ir = GetNumberRows();
    DeleteRows(0, ir);
    AppendRows(ir);

    SetRowSize( 0, 60 );
    SetCellValue( 0, 0, wxT("Ctrl+Home\nwill go to\nthis cell") );

    SetCellValue( 0, 1, wxT("A long piece of text to demonstrate wrapping.") );
    SetCellRenderer(0 , 1, new wxGridCellAutoWrapStringRenderer);
    SetCellEditor( 0,  1 , new wxGridCellAutoWrapStringEditor);

    SetCellValue( 0, 2, wxT("Blah") );
    SetCellValue( 0, 3, wxT("Read only") );
    SetReadOnly( 0, 3 );

    SetCellValue( 0, 4, wxT("Can veto edit this cell") );

    SetCellValue( 0, 5, wxT("Press\nCtrl+arrow\nto skip over\ncells") );

    SetRowSize( 99, 60 );
    SetCellValue( 99, 99, wxT("Ctrl+End\nwill go to\nthis cell") );
    SetCellValue( 1, 0, wxT("This default cell will overflow into neighboring cells, but not if you turn overflow off."));

    SetCellTextColour(1, 2, *wxRED);
    SetCellBackgroundColour(1, 2, *wxGREEN);

    SetCellValue( 1, 4, wxT("I'm in the middle"));

    SetCellValue(2, 2, wxT("red"));

    SetCellTextColour(2, 2, *wxRED);
    SetCellValue(3, 3, wxT("green on grey"));
    SetCellTextColour(3, 3, *wxGREEN);
    SetCellBackgroundColour(3, 3, *wxLIGHT_GREY);

	SetCellValue(4, 4, wxT("a weird looking cell"));
    SetCellAlignment(4, 4, wxALIGN_CENTRE, wxALIGN_CENTRE);
    // SetCellRenderer(4, 4, new MyGridCellRenderer);

    SetCellRenderer(3, 0, new wxGridCellBoolRenderer);
    SetCellEditor(3, 0, new wxGridCellBoolEditor);

    wxGridCellAttr *attr;
    attr = new wxGridCellAttr;
    attr->SetTextColour(*wxBLUE);
    SetColAttr(5, attr);
    attr = new wxGridCellAttr;
    attr->SetBackgroundColour(*wxRED);
    SetRowAttr(5, attr);

    SetCellValue(2, 4, wxT("a wider column"));
    SetColSize(4, 120);
    SetColMinimalWidth(4, 120);

    SetCellTextColour(5, 8, *wxGREEN);
    SetCellValue(5, 8, wxT("Bg from row attr\nText col from cell attr"));
    SetCellValue(5, 5, wxT("Bg from row attr Text col from col attr and this text is so long that it covers over many many empty cells but is broken by one that isn't"));

    // Some numeric columns with different formatting.
    SetColFormatFloat(6);
    SetCellValue(0, 6, "Default\nfloat format");
    SetCellValue(1, 6, wxString::Format(wxT("%g"), 3.1415));
    SetCellValue(2, 6, wxString::Format(wxT("%g"), 1415.0));
    SetCellValue(3, 6, wxString::Format(wxT("%g"), 12345.67890));

    SetColFormatFloat(7, 6, 2);
    SetCellValue(0, 7, "Width 6\nprecision 2");
    SetCellValue(1, 7, wxString::Format(wxT("%g"), 3.1415));
    SetCellValue(2, 7, wxString::Format(wxT("%g"), 1415.0));
    SetCellValue(3, 7, wxString::Format(wxT("%g"), 12345.67890));

    SetColFormatCustom(8,
            wxString::Format("%s:%i,%i,%s", wxGRID_VALUE_FLOAT, -1, 4, "g"));
    SetCellValue(0, 8, "Compact\nformat");
    SetCellValue(1, 8, wxT("31415e-4"));
    SetCellValue(2, 8, wxT("1415"));
    SetCellValue(3, 8, wxT("123456789e-4"));

    SetColFormatNumber(9);
    SetCellValue(0, 9, "Integer\ncolumn");
    SetCellValue(1, 9, "17");
    SetCellValue(2, 9, "0");
    SetCellValue(3, 9, "-666");
    SetCellAlignment(3, 9, wxALIGN_CENTRE, wxALIGN_TOP);
    SetCellValue(3, 10, "<- This numeric cell should be centred");

    const wxString choices[] =
    {
        wxT("Please select a choice"),
        wxT("This takes two cells"),
        wxT("Another choice"),
    };
    SetCellEditor(4, 0, new wxGridCellChoiceEditor(WXSIZEOF(choices), choices));
    SetCellSize(4, 0, 1, 2);
    SetCellValue(4, 0, choices[0]);
    SetCellOverflow(4, 0, false);

    SetCellSize(7, 1, 3, 4);
    SetCellAlignment(7, 1, wxALIGN_CENTRE, wxALIGN_CENTRE);
    SetCellValue(7, 1, wxT("Big box!"));

    // create a separator-like row: it's grey and it's non-resizable
    DisableRowResize(10);
    SetRowSize(10, 30);
    attr = new wxGridCellAttr;
    attr->SetBackgroundColour(*wxLIGHT_GREY);
    SetRowAttr(10, attr);
    SetCellValue(10, 0, "You can't resize this row interactively -- try it");

    // this does exactly nothing except testing that SetAttr() handles NULL
    // attributes and does reference counting correctly
    SetAttr(11, 11, NULL);
    SetAttr(11, 11, new wxGridCellAttr);
    SetAttr(11, 11, NULL);
}
*/

gridwidget::~gridwidget()
{
}


void gridwidget::OnSetHighlightWidth( wxCommandEvent& WXUNUSED(ev) )
{
    wxString choices[] = { wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10")};

    wxSingleChoiceDialog dlg(this, wxT("Choose the thickness of the highlight pen:"),
                             wxT("Pen Width"), 11, choices);

    int current = GetCellHighlightPenWidth();
    dlg.SetSelection(current);
    if (dlg.ShowModal() == wxID_OK) {
        SetCellHighlightPenWidth(dlg.GetSelection());
    }
}

void gridwidget::OnSetROHighlightWidth( wxCommandEvent& WXUNUSED(ev) )
{
    wxString choices[] = { wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10")};

    wxSingleChoiceDialog dlg(this, wxT("Choose the thickness of the highlight pen:"),
                             wxT("Pen Width"), 11, choices);

    int current = GetCellHighlightROPenWidth();
    dlg.SetSelection(current);
    if (dlg.ShowModal() == wxID_OK) {
        SetCellHighlightROPenWidth(dlg.GetSelection());
    }
}



void gridwidget::InsertRow( wxCommandEvent& WXUNUSED(ev) )
{
    InsertRows( GetGridCursorRow(), 1 );
}


void gridwidget::InsertCol( wxCommandEvent& WXUNUSED(ev) )
{
    InsertCols( GetGridCursorCol(), 1 );
}


void gridwidget::DeleteSelectedRows( wxCommandEvent& WXUNUSED(ev) )
{
    if ( IsSelection() )
    {
        wxGridUpdateLocker locker(this);
        for ( int n = 0; n < GetNumberRows(); )
        {
            if ( IsInSelection( n , 0 ) )
                DeleteRows( n, 1 );
            else
                n++;
        }
    }
}

void gridwidget::AutoSizeRowLabel(wxCommandEvent& WXUNUSED(event))
{
    wxGridUpdateLocker locker(this);
    const wxArrayInt sels  = GetSelectedRows();
    for ( size_t n = 0, count = sels.size(); n < count; n++ )
    {
        AutoSizeRowLabelSize( sels[n] );
    }
}

void gridwidget::AutoSizeColLabel(wxCommandEvent& WXUNUSED(event))
{
    wxGridUpdateLocker locker(this);
    const wxArrayInt sels  = GetSelectedCols();
    for ( size_t n = 0, count = sels.size(); n < count; n++ )
    {
        AutoSizeColLabelSize( sels[n] );
    }
}

void gridwidget::AutoSizeLabelsCol(wxCommandEvent& WXUNUSED(event))
{
    SetColLabelSize( wxGRID_AUTOSIZE );
}

void gridwidget::AutoSizeLabelsRow(wxCommandEvent& WXUNUSED(event))
{
    SetRowLabelSize( wxGRID_AUTOSIZE );
}

void gridwidget::AutoSizeTable(wxCommandEvent& WXUNUSED(event))
{
    AutoSize();
}


void gridwidget::DeleteSelectedCols( wxCommandEvent& WXUNUSED(ev) )
{
    if ( IsSelection() )
    {
        wxGridUpdateLocker locker(this);
        for ( int n = 0; n < GetNumberCols(); )
        {
            if ( IsInSelection( 0 , n ) )
                DeleteCols( n, 1 );
            else
                n++;
        }
    }
}


void gridwidget::SelectCells( wxCommandEvent& WXUNUSED(ev) )
{
    SetSelectionMode( wxGrid::wxGridSelectCells );
}

void gridwidget::SelectRows( wxCommandEvent& WXUNUSED(ev) )
{
    SetSelectionMode( wxGrid::wxGridSelectRows );
}

void gridwidget::SelectCols( wxCommandEvent& WXUNUSED(ev) )
{
    SetSelectionMode( wxGrid::wxGridSelectColumns );
}

void gridwidget::SelectRowsOrCols( wxCommandEvent& WXUNUSED(ev) )
{
    SetSelectionMode( wxGrid::wxGridSelectRowsOrColumns );
}

void gridwidget::SetCellFgColour( wxColour &col )
{
    SetDefaultCellTextColour(col);
    Refresh();
}

void gridwidget::SetCellBgColour( wxColour &col )
{
    // Check the new Refresh function by passing it a rectangle
    // which exactly fits the grid.
    wxPoint pt(0, 0);
    wxRect r(pt, GetSize());
    SetDefaultCellBackgroundColour(col);
    Refresh(true, &r);
}

void gridwidget::DeselectAll(wxCommandEvent& WXUNUSED(event))
{
      ClearSelection();
}

void gridwidget::SelectCell(wxCommandEvent& WXUNUSED(event))
{
      SelectBlock(3, 1, 3, 1, m_addToSel);
}

void gridwidget::OnAddToSelectToggle(wxCommandEvent& event)
{
    m_addToSel = event.IsChecked();
}

void gridwidget::OnLabelLeftClick( wxGridEvent& ev )
{
	/*
    wxString logBuf;
    if ( ev.GetRow() != -1 )
    {
        logBuf << wxT("Left click on row label ") << ev.GetRow();
    }
    else if ( ev.GetCol() != -1 )
    {
        logBuf << wxT("Left click on col label ") << ev.GetCol();
    }
    else
    {
        logBuf << wxT("Left click on corner label");
    }

    if ( ev.ShiftDown() )
        logBuf << wxT(" (shift down)");
    if ( ev.ControlDown() )
        logBuf << wxT(" (control down)");
    wxLogMessage( wxT("%s"), logBuf.c_str() );
	*/

    // you must call event skip if you want default grid processing
    //
    ev.Skip();
}


void gridwidget::OnCellLeftClick( wxGridEvent& ev )
{
    // wxLogMessage(wxT("Left click at row %d, col %d"), ev.GetRow(), ev.GetCol());

    // you must call event skip if you want default grid processing
    // (cell highlighting etc.)
    //
    ev.Skip();
}


void gridwidget::OnRowSize( wxGridSizeEvent& ev )
{
    const int row = ev.GetRowOrCol();

    wxLogMessage("Resized row %d, new height = %d", row, GetRowSize(row));

    ev.Skip();
}


void gridwidget::OnColSize( wxGridSizeEvent& ev )
{
    const int col = ev.GetRowOrCol();

    wxLogMessage("Resized column %d, new width = %d", col, GetColSize(col));

    ev.Skip();
}


void gridwidget::OnSelectCell( wxGridEvent& ev )

{
	/*
    wxString logBuf;
    if ( ev.Selecting() )
        logBuf << wxT("Selected ");
    else
        logBuf << wxT("Deselected ");
    logBuf << wxT("cell at row ") << ev.GetRow()
           << wxT(" col ") << ev.GetCol()
           << wxT(" ( ControlDown: ")<< (ev.ControlDown() ? 'T':'F')
           << wxT(", ShiftDown: ")<< (ev.ShiftDown() ? 'T':'F')
           << wxT(", AltDown: ")<< (ev.AltDown() ? 'T':'F')
           << wxT(", MetaDown: ")<< (ev.MetaDown() ? 'T':'F') << wxT(" )");

    //Indicate whether this column was moved
    if ( ((wxGrid *)ev.GetEventObject())->GetColPos( ev.GetCol() ) != ev.GetCol() )
        logBuf << wxT(" *** Column moved, current position: ") << ((wxGrid *)ev.GetEventObject())->GetColPos( ev.GetCol() );

    wxLogMessage( wxT("%s"), logBuf.c_str() );
	*/

    // you must call Skip() if you want the default processing
    // to occur in wxGrid
    ev.Skip();
}

void gridwidget::OnRangeSelected( wxGridRangeSelectEvent& ev )
{
     ev.Skip();
}

void gridwidget::OnCellValueChanging( wxGridEvent& ev )
{
	/*
    int row = ev.GetRow(),
        col = ev.GetCol();

    wxLogMessage("Value of cell at (%d, %d): about to change "
                 "from \"%s\" to \"%s\"",
                 row, col,
                 GetCellValue(row, col), ev.GetString());

    // test how vetoing works
    if ( ev.GetString() == "42" )
    {
        wxLogMessage("Vetoing the change.");
        ev.Veto();
        return;
    }
	*/

    ev.Skip();
}

void gridwidget::OnCellValueChanged( wxGridEvent& ev )
{
    int row = ev.GetRow(),
        col = ev.GetCol();

    wxLogMessage("Value of cell at (%d, %d) changed and is now \"%s\" "
                 "(was \"%s\")",
                 row, col,
                 GetCellValue(row, col), ev.GetString());

    ev.Skip();
}

void gridwidget::OnCellBeginDrag( wxGridEvent& ev )
{
    wxLogMessage(wxT("Got request to drag cell at row %d, col %d"),
                 ev.GetRow(), ev.GetCol());

    ev.Skip();
}

void gridwidget::OnEditorShown( wxGridEvent& ev )
{

    if ( (ev.GetCol() == 4) &&
         (ev.GetRow() == 0) &&
     (wxMessageBox(wxT("Are you sure you wish to edit this cell"),
                   wxT("Checking"),wxYES_NO) == wxNO ) ) {

     ev.Veto();
     return;
    }

    wxLogMessage( wxT("Cell editor shown.") );

    ev.Skip();
}

void gridwidget::OnEditorHidden( wxGridEvent& ev )
{

    if ( (ev.GetCol() == 4) &&
         (ev.GetRow() == 0) &&
     (wxMessageBox(wxT("Are you sure you wish to finish editing this cell"),
                   wxT("Checking"),wxYES_NO) == wxNO ) ) {

        ev.Veto();
        return;
    }

    wxLogMessage( wxT("Cell editor hidden.") );

    ev.Skip();
}

