#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/Timestamp.h"
#include "../util/device_coordinate.h"
#include <wx/frame.h>
#include <wx/wizard.h>
#include <wx/filename.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_field_set.h"
#include "../i_swat_database/SWATDeclarations.h"
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
#include "../i_swat_database/rowset_subbasin.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATPotentialEvapotranspiration.h"
#include "../i_swat_database/SWATControlRecord.h"
#include "../i_swat_database/SWATSoilLayers.h"
#include "../i_swat_database/SWATSoils.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATUrban.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATSepticWaterQuality.h"
#include "../i_swat_database/SWATWeatherHistorical.h"
#include "../i_swat_database/SWATWeatherByMonth.h"
#include "../i_swat_database/SWATWeatherStations.h"
#include "../i_swat_database/swat_weather_list.h"
#include "list_control_records.h"
#include "frame_swat.h"
// #include "wizard_control_record.h"

BEGIN_EVENT_TABLE(list_control_records, wxListCtrl)
    EVT_LIST_BEGIN_DRAG(LIST_CTRL, list_control_records::OnBeginDrag)
    EVT_LIST_BEGIN_RDRAG(LIST_CTRL, list_control_records::OnBeginRDrag)
    EVT_LIST_BEGIN_LABEL_EDIT(LIST_CTRL, list_control_records::OnBeginLabelEdit)
    EVT_LIST_END_LABEL_EDIT(LIST_CTRL, list_control_records::OnEndLabelEdit)
    EVT_LIST_DELETE_ITEM(LIST_CTRL, list_control_records::OnDeleteItem)
    EVT_LIST_DELETE_ALL_ITEMS(LIST_CTRL, list_control_records::OnDeleteAllItems)
    EVT_LIST_ITEM_SELECTED(LIST_CTRL, list_control_records::OnSelected)
    EVT_LIST_ITEM_DESELECTED(LIST_CTRL, list_control_records::OnDeselected)
    EVT_LIST_KEY_DOWN(LIST_CTRL, list_control_records::OnListKeyDown)
    EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, list_control_records::OnActivated)
    EVT_LIST_ITEM_FOCUSED(LIST_CTRL, list_control_records::OnFocused)

    EVT_LIST_COL_CLICK(LIST_CTRL, list_control_records::OnColClick)
    EVT_LIST_COL_RIGHT_CLICK(LIST_CTRL, list_control_records::OnColRightClick)
    EVT_LIST_COL_BEGIN_DRAG(LIST_CTRL, list_control_records::OnColBeginDrag)
    EVT_LIST_COL_DRAGGING(LIST_CTRL, list_control_records::OnColDragging)
    EVT_LIST_COL_END_DRAG(LIST_CTRL, list_control_records::OnColEndDrag)

    EVT_LIST_CACHE_HINT(LIST_CTRL, list_control_records::OnCacheHint)
    EVT_CONTEXT_MENU(list_control_records::OnContextMenu)
    EVT_CHAR(list_control_records::OnChar)

    EVT_RIGHT_DOWN(list_control_records::OnRightClick)
END_EVENT_TABLE()

list_control_records::list_control_records
	(wxWindow *parent,
	std::vector <SWATControlRecord *> *p_control_records,
    const wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style)
    : wxListCtrl(parent, id, pos, size, style), m_attr(*wxBLUE, *wxLIGHT_GREY, wxNullFont)

{
	control_records = p_control_records;

    m_updated = -1;

#ifdef __POCKETPC__
    EnableContextMenu();
#endif

	wxFont list_font (12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	SetFont (list_font);
}

void list_control_records::Update ()

{
	std::vector <SWATControlRecord *>::iterator c;
	int i;
	long item;
	dynamic_string data;

	for (c = control_records->begin (), i = 0;
	c != control_records->end ();
	++c, ++i) {
		data.format ("%ld ", (*c)->ID);
		data += (*c)->Description;

		item = InsertItem (i, data.get_text ());
		SetItemData (item, (*c)->ID);
	}

	if (control_records->size () >= 1) {
	    SetItemState(0, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
		SetItemState (0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
}

long list_control_records::GetSelectedID ()

{
    int item = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if ( item != -1 )
		return GetItemData (item);
	else
		return 0;
}

void list_control_records::OnContextMenu
	(wxContextMenuEvent &e)

{
	wxMenu menu;
    menu.Append (wxID_EDIT_CONTROL_RECORD, wxT("&Edit"));
	menu.Append (wxID_REPORT_POLLUTANTS, wxT("Report Pollutants"));
    PopupMenu(&menu, ScreenToClient (e.GetPosition ()));
}

void list_control_records::OnCacheHint(wxListEvent& event)
{
    wxLogMessage( wxT("OnCacheHint: cache items %ld..%ld"), event.GetCacheFrom(), event.GetCacheTo() );
}

void list_control_records::SetColumnImage(int col, int image)
{
    wxListItem item;
    item.SetMask(wxLIST_MASK_IMAGE);
    item.SetImage(image);
    SetColumn(col, item);
}

void list_control_records::OnColClick(wxListEvent& event)
{
    int col = event.GetColumn();

    // set or unset image
    static bool x = false;
    x = !x;
    SetColumnImage(col, x ? 0 : -1);

    wxLogMessage( wxT("OnColumnClick at %d."), col );
}

void list_control_records::OnColRightClick(wxListEvent& event)
{
    int col = event.GetColumn();
    if ( col != -1 )
    {
        SetColumnImage(col, -1);
    }

    // Show popupmenu at position
    wxMenu menu(wxT("Test"));
    menu.Append(LIST_ABOUT, wxT("&About"));
    PopupMenu(&menu, event.GetPoint());

    wxLogMessage( wxT("OnColumnRightClick at %d."), event.GetColumn() );
}

void list_control_records::LogColEvent(const wxListEvent& event, const wxChar *name)
{
    const int col = event.GetColumn();

    wxLogMessage(wxT("%s: column %d (width = %d or %d)."),
                 name,
                 col,
                 event.GetItem().GetWidth(),
                 GetColumnWidth(col));
}

void list_control_records::OnColBeginDrag(wxListEvent& event)
{
    LogColEvent( event, wxT("OnColBeginDrag") );

    if ( event.GetColumn() == 0 )
    {
        wxLogMessage(wxT("Resizing this column shouldn't work."));

        event.Veto();
    }
}

void list_control_records::OnColDragging(wxListEvent& event)
{
    LogColEvent( event, wxT("OnColDragging") );
}

void list_control_records::OnColEndDrag(wxListEvent& event)
{
    LogColEvent( event, wxT("OnColEndDrag") );
}

void list_control_records::OnBeginDrag(wxListEvent& event)
{
    const wxPoint& pt = event.m_pointDrag;

    int flags;
    wxLogMessage( wxT("OnBeginDrag at (%d, %d), item %ld."),
                  pt.x, pt.y, HitTest(pt, flags) );
}

void list_control_records::OnBeginRDrag(wxListEvent& event)
{
    wxLogMessage( wxT("OnBeginRDrag at %d,%d."),
                  event.m_pointDrag.x, event.m_pointDrag.y );
}

void list_control_records::OnBeginLabelEdit(wxListEvent& event)
{
    wxLogMessage( wxT("OnBeginLabelEdit: %s"), event.m_item.m_text.c_str());

    wxTextCtrl * const text = GetEditControl();
    if ( !text )
    {
        wxLogMessage("BUG: started to edit but no edit control");
    }
    else
    {
        wxLogMessage("Edit control value: \"%s\"", text->GetValue());
    }
}

void list_control_records::OnEndLabelEdit(wxListEvent& event)
{
    wxLogMessage( wxT("OnEndLabelEdit: %s"),
        (
            event.IsEditCancelled() ?
            wxString("[cancelled]") :
            event.m_item.m_text
        ).c_str()
    );
}

void list_control_records::OnDeleteItem(wxListEvent& event)
{
    LogEvent(event, wxT("OnDeleteItem"));
    wxLogMessage( wxT("Number of items when delete event is sent: %d"), GetItemCount() );
}

void list_control_records::OnDeleteAllItems(wxListEvent& event)
{
    LogEvent(event, wxT("OnDeleteAllItems"));
}

void list_control_records::OnSelected(wxListEvent& event)
{
    LogEvent(event, wxT("OnSelected"));

    if ( GetWindowStyle() & wxLC_REPORT )
    {
        wxListItem info;
        info.m_itemId = event.m_itemIndex;
        info.m_col = 1;
        info.m_mask = wxLIST_MASK_TEXT;
        if ( GetItem(info) )
        {
            wxLogMessage(wxT("Value of the 2nd field of the selected item: %s"),
                         info.m_text.c_str());
        }
        else
        {
            wxFAIL_MSG(wxT("wxListCtrl::GetItem() failed"));
        }
    }
}

void list_control_records::OnDeselected(wxListEvent& event)
{
    LogEvent(event, wxT("OnDeselected"));
}

void list_control_records::OnActivated(wxListEvent& event)

{
	// doubleclick
    LogEvent(event, wxT("OnActivated"));

	// Edit control record
	// wizard_control_record wizard ((wxFrame *) GetParent ());
    // wizard.RunWizard(wizard.GetFirstPage());
}

void list_control_records::OnFocused(wxListEvent& event)
{
    LogEvent(event, wxT("OnFocused"));

    event.Skip();
}

void list_control_records::next_record ()

{
	long item_index, previous_item;

    previous_item = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED);
	item_index = previous_item;
    if ( item_index++ == GetItemCount() - 1 )
    {
        item_index = GetItemCount() - 1;
    }

    // wxLogMessage(wxT("Focusing item %ld"), item);

	SetItemState (previous_item, 0, wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED);
    SetItemState(item_index, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
	SetItemState (item_index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    EnsureVisible(item_index);
}

void list_control_records::previous_record ()

{
	long item_index, previously_selected_item;

    previously_selected_item = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED);
	item_index = previously_selected_item;
    if ( item_index-- < 0)
    {
        item_index = 0;
    }

    // wxLogMessage(wxT("Focusing item %ld"), item);

	SetItemState (previously_selected_item, 0, wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED);
    SetItemState(item_index, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
	SetItemState (item_index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    EnsureVisible(item_index);
}

void list_control_records::OnListKeyDown(wxListEvent& event)
{
    long item;

    switch ( event.GetKeyCode() )
    {
        case 'C': // colorize
            {
                wxListItem info;
                info.m_itemId = event.GetIndex();
                if ( info.m_itemId == -1 )
                {
                    // no item
                    break;
                }

                GetItem(info);

                wxListItemAttr *attr = info.GetAttributes();
                if ( !attr || !attr->HasTextColour() )
                {
                    info.SetTextColour(*wxCYAN);

                    SetItem(info);

                    RefreshItem(info.m_itemId);
                }
            }
            break;

        case 'N': // next
			next_record ();
            break;

       case 'P': // previous
			previous_record ();
            break;

        case 'R': // show bounding rectangle
            {
                item = event.GetIndex();
                wxRect r;
                if ( !GetItemRect(item, r) )
                {
                    wxLogError(wxT("Failed to retrieve rect of item %ld"), item);
                    break;
                }

                wxLogMessage(wxT("Bounding rect of item %ld is (%d, %d)-(%d, %d)"),
                             item, r.x, r.y, r.x + r.width, r.y + r.height);
            }
            break;

        case '1': // show sub item bounding rectangle for the given column
        case '2': // (and icon/label rectangle if Shift/Ctrl is pressed)
        case '3':
        case '4': // this column is invalid but we want to test it too
            if ( InReportView() )
            {
                int subItem = event.GetKeyCode() - '1';
                item = event.GetIndex();
                wxRect r;

                int code = wxLIST_RECT_BOUNDS;
                if ( wxGetKeyState(WXK_SHIFT) )
                    code = wxLIST_RECT_ICON;
                else if ( wxGetKeyState(WXK_CONTROL) )
                    code = wxLIST_RECT_LABEL;

                if ( !GetSubItemRect(item, subItem, r, code) )
                {
                    wxLogError(wxT("Failed to retrieve rect of item %ld column %d"), item, subItem + 1);
                    break;
                }

                wxLogMessage(wxT("Bounding rect of item %ld column %d is (%d, %d)-(%d, %d)"),
                             item, subItem + 1,
                             r.x, r.y, r.x + r.width, r.y + r.height);
            }
            break;

        case 'U': // update
            if ( !IsVirtual() )
                break;

            if ( m_updated != -1 )
                RefreshItem(m_updated);

            m_updated = event.GetIndex();
            if ( m_updated != -1 )
            {
                // we won't see changes to this item as it's selected, update
                // the next one (or the first one if we're on the last item)
                if ( ++m_updated == GetItemCount() )
                    m_updated = 0;

                wxLogMessage("Updating colour of the item %ld", m_updated);
                RefreshItem(m_updated);
            }
            break;

        case 'D': // delete
            item = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            while ( item != -1 )
            {
                DeleteItem(item);

                wxLogMessage(wxT("Item %ld deleted"), item);

                // -1 because the indices were shifted by DeleteItem()
                item = GetNextItem(item - 1,
                                   wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            }
            break;

        case 'I': // insert
            if ( GetWindowStyle() & wxLC_REPORT )
            {
                if ( GetWindowStyle() & wxLC_VIRTUAL )
                {
                    SetItemCount(GetItemCount() + 1);
                }
                else // !virtual
                {
                    InsertItemInReportView(event.GetIndex());
                }
            }
            //else: fall through

        default:
            LogEvent(event, wxT("OnListKeyDown"));

            event.Skip();
    }
}

void list_control_records::OnChar(wxKeyEvent& event)
{
    // wxLogMessage(wxT("Got char event."));

    switch ( event.GetKeyCode() )
    {
        case 'n':
        case 'N':
        case 'c':
        case 'C':
        case 'r':
        case 'R':
        case 'u':
        case 'U':
        case 'd':
        case 'D':
        case 'i':
        case 'I':
            // these are the keys we process ourselves
            break;

        default:
            event.Skip();
    }
}

void list_control_records::OnRightClick(wxMouseEvent& event)
{
    if ( !event.ControlDown() )
    {
        event.Skip();
        return;
    }

    int flags;
    long subitem;
    long item = HitTest(event.GetPosition(), flags, &subitem);

    wxString where;
    switch ( flags )
    {
        case wxLIST_HITTEST_ABOVE: where = wxT("above"); break;
        case wxLIST_HITTEST_BELOW: where = wxT("below"); break;
        case wxLIST_HITTEST_NOWHERE: where = wxT("nowhere near"); break;
        case wxLIST_HITTEST_ONITEMICON: where = wxT("on icon of"); break;
        case wxLIST_HITTEST_ONITEMLABEL: where = wxT("on label of"); break;
        case wxLIST_HITTEST_ONITEMRIGHT: where = wxT("right on"); break;
        case wxLIST_HITTEST_TOLEFT: where = wxT("to the left of"); break;
        case wxLIST_HITTEST_TORIGHT: where = wxT("to the right of"); break;
        default: where = wxT("not clear exactly where on"); break;
    }

    wxLogMessage(wxT("Right double click %s item %ld, subitem %ld"),
                 where.c_str(), item, subitem);
}

void list_control_records::LogEvent(const wxListEvent& event, const wxChar *eventName)
{
    wxLogMessage(wxT("Item %ld: %s (item text = %s, data = %ld)"),
                 event.GetIndex(), eventName,
                 event.GetText().c_str(), event.GetData());
}

wxString list_control_records::OnGetItemText(long item, long column) const
{
	// return list_data [item].get_text ();
	return wxT("");
}

int list_control_records::OnGetItemColumnImage(long item, long column) const
{
    if (!column)
        return 0;

    if (!(item % 3) && column == 1)
        return 0;

    return -1;
}

wxListItemAttr *list_control_records::OnGetItemAttr(long item) const
{
    // test to check that RefreshItem() works correctly: when m_updated is
    // set to some item and it is refreshed, we highlight the item
    if ( item == m_updated )
    {
        static wxListItemAttr s_attrHighlight(*wxRED, wxNullColour, wxNullFont);
        return &s_attrHighlight;
    }

    return item % 2 ? NULL : (wxListItemAttr *)&m_attr;
}

void list_control_records::InsertItemInReportView(int i)
{
    wxString buf;
    buf.Printf(wxT("This is item %d"), i);
    long tmp = InsertItem(i, buf, 0);
    SetItemData(tmp, i);

	// dynamic_string d = buf.ToStdString ().c_str ();
	// list_data.push_back (d);

    buf.Printf(wxT("Col 1, item %d"), i);
    SetItem(tmp, 1, buf);

    buf.Printf(wxT("Item %d in column 2"), i);
    SetItem(tmp, 2, buf);
}

