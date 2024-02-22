
#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/Timestamp.h"
#include "../util/device_coordinate.h"
#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include "../util/interface_tree.h"
#include "../util/interface_tree_wx.h"
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_field_set.h"
#include <wx/artprov.h>
#include "swat_tree_control.h"
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
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATPotentialEvapotranspiration.h"
#include "../i_swat_database/SWATControlRecord.h"

const int NUM_CHILDREN_PER_LEVEL = 5;
const int NUM_LEVELS = 2;

const bool TREE_SHOW_IMAGES = true;
const bool TREE_SHOW_STATES = true;

static const char * unchecked_xpm[] = {
"13 13 14 1",
" 	c None",
".	c #CCCED3",
"+	c #838793",
"@	c #C4C7CF",
"#	c #F5F5F5",
"$	c #F6F6F6",
"%	c #F7F7F7",
"&	c #F9F9F9",
"*	c #FAFAFA",
"=	c #FBFBFB",
"-	c #FCFCFC",
";	c #FDFDFD",
">	c #FEFEFE",
",	c #FFFFFF",
".+++++++++++.",
"+@@@@@@@@@@@+",
"+@##########+",
"+@$$$$$$$$$$+",
"+@%%%%%%%%%%+",
"+@&&&&&&&&&&+",
"+@**********+",
"+@==========+",
"+@----------+",
"+@;;;;;;;;;;+",
"+@>>>>>>>>>>+",
"+@,,,,,,,,,,+",
".+++++++++++."};

static const char * checked_xpm[] = {
"13 13 38 1",
" 	c None",
".	c #CCCED3",
"+	c #838793",
"@	c #C4C7CF",
"#	c #F5F5F5",
"$	c #BBBBBB",
"%	c #131313",
"&	c #0C0C0C",
"*	c #F6F6F6",
"=	c #BEBEBE",
"-	c #030303",
";	c #060606",
">	c #B4B4B4",
",	c #F7F7F7",
"'	c #C6C6C6",
")	c #080808",
"!	c #000000",
"~	c #C0C0C0",
"{	c #020202",
"]	c #363636",
"^	c #F9F9F9",
"/	c #505050",
"(	c #2C2C2C",
"_	c #545454",
":	c #606060",
"<	c #010101",
"[	c #FAFAFA",
"}	c #FBFBFB",
"|	c #333333",
"1	c #1D1D1D",
"2	c #FCFCFC",
"3	c #C4C4C4",
"4	c #FDFDFD",
"5	c #B9B9B9",
"6	c #FEFEFE",
"7	c #5E5E5E",
"8	c #777777",
"9	c #FFFFFF",
".+++++++++++.",
"+@@@@@@@@@@@+",
"+@######$%&#+",
"+@*****=-;**+",
"+@$>,,')!~,,+",
"+@{!]^/!(^^^+",
"+@_!!:<![[[[+",
"+@}|!!!1}}}}+",
"+@22)!!32222+",
"+@445!)44444+",
"+@6667866666+",
"+@9999999999+",
".+++++++++++."};

BEGIN_EVENT_TABLE(swat_tree_control, wxTreeCtrl)
    EVT_TREE_BEGIN_DRAG(TreeTest_Ctrl, swat_tree_control::OnBeginDrag)
    EVT_TREE_BEGIN_RDRAG(TreeTest_Ctrl, swat_tree_control::OnBeginRDrag)
    EVT_TREE_END_DRAG(TreeTest_Ctrl, swat_tree_control::OnEndDrag)
    EVT_TREE_BEGIN_LABEL_EDIT(TreeTest_Ctrl, swat_tree_control::OnBeginLabelEdit)
    EVT_TREE_END_LABEL_EDIT(TreeTest_Ctrl, swat_tree_control::OnEndLabelEdit)
    EVT_TREE_DELETE_ITEM(TreeTest_Ctrl, swat_tree_control::OnDeleteItem)
#if 0       // there are so many of those that logging them causes flicker
    EVT_TREE_GET_INFO(TreeTest_Ctrl, swat_tree_control::OnGetInfo)
#endif
    EVT_TREE_SET_INFO(TreeTest_Ctrl, swat_tree_control::OnSetInfo)
    EVT_TREE_ITEM_EXPANDED(TreeTest_Ctrl, swat_tree_control::OnItemExpanded)
    EVT_TREE_ITEM_EXPANDING(TreeTest_Ctrl, swat_tree_control::OnItemExpanding)
    EVT_TREE_ITEM_COLLAPSED(TreeTest_Ctrl, swat_tree_control::OnItemCollapsed)
    EVT_TREE_ITEM_COLLAPSING(TreeTest_Ctrl, swat_tree_control::OnItemCollapsing)

    EVT_TREE_SEL_CHANGED(TreeTest_Ctrl, swat_tree_control::OnSelChanged)
    EVT_TREE_SEL_CHANGING(TreeTest_Ctrl, swat_tree_control::OnSelChanging)
    EVT_TREE_KEY_DOWN(TreeTest_Ctrl, swat_tree_control::OnTreeKeyDown)
    EVT_TREE_ITEM_ACTIVATED(TreeTest_Ctrl, swat_tree_control::OnItemActivated)
    EVT_TREE_STATE_IMAGE_CLICK(TreeTest_Ctrl, swat_tree_control::OnItemStateClick)

    // so many different ways to handle right mouse button clicks...
    EVT_CONTEXT_MENU(swat_tree_control::OnContextMenu)
    // EVT_TREE_ITEM_MENU is the preferred event for creating context menus
    // on a tree control, because it includes the point of the click or item,
    // meaning that no additional placement calculations are required.
    EVT_TREE_ITEM_MENU(TreeTest_Ctrl, swat_tree_control::OnItemMenu)
    EVT_TREE_ITEM_RIGHT_CLICK(TreeTest_Ctrl, swat_tree_control::OnItemRClick)

    EVT_RIGHT_DOWN(swat_tree_control::OnRMouseDown)
    EVT_RIGHT_UP(swat_tree_control::OnRMouseUp)
    EVT_RIGHT_DCLICK(swat_tree_control::OnRMouseDClick)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(swat_tree_control, wxTreeCtrl)

swat_tree_control::swat_tree_control
	(wxWindow *parent,
	const wxWindowID id,
    const wxPoint& pos,
	const wxSize& size,
    long style,
	SWATControlRecord *p_control)
	: wxTreeCtrl(parent, id, pos, size, style)
{
    m_reverseSort = false;

	control = p_control;

    CreateImageList();
    CreateStateImageList();

    // Add some items to the tree
    Update ();
}

void swat_tree_control::CreateImageList(int size)
{
    if ( size == -1 )
    {
        SetImageList(NULL);
        return;
    }
    if ( size == 0 )
        size = m_imageSize;
    else
        m_imageSize = size;

    // Make an image list containing small icons
    wxImageList *images = new wxImageList(size, size, true);

    // should correspond to TreeCtrlIcon_xxx enum
    wxBusyCursor wait;
    wxIcon icons[5];

    wxSize iconSize(size, size);

    icons[TreeCtrlIcon_File] =
    icons[TreeCtrlIcon_FileSelected] = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_LIST, iconSize);
    icons[TreeCtrlIcon_Folder] =
    icons[TreeCtrlIcon_FolderSelected] =
    icons[TreeCtrlIcon_FolderOpened] = wxArtProvider::GetIcon(wxART_FOLDER, wxART_LIST, iconSize);

    for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
    {
        int sizeOrig = icons[0].GetWidth();
        if ( size == sizeOrig )
        {
            images->Add(icons[i]);
        }
        else
        {
            images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
        }
    }

    AssignImageList(images);
}

void swat_tree_control::CreateStateImageList(bool del)
{
    if ( del )
    {
        SetStateImageList(NULL);
        return;
    }

    wxImageList *states;
    wxBusyCursor wait;

    wxIcon icons[2];
    icons[0] = wxIcon(unchecked_xpm);
    icons[1] = wxIcon(checked_xpm);

    int width  = icons[0].GetWidth(),
        height = icons[0].GetHeight();

    // Make an state image list containing small icons
    states = new wxImageList(width, height, true);

    for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
        states->Add(icons[i]);

    AssignStateImageList(states);
}

#if USE_GENERIC_TREECTRL || !defined(__WXMSW__)
void swat_tree_control::CreateButtonsImageList(int size)
{
    if ( size == -1 )
    {
        SetButtonsImageList(NULL);
        return;
    }

    // Make an image list containing small icons
    wxImageList *images = new wxImageList(size, size, true);

    // should correspond to TreeCtrlIcon_xxx enum
    wxBusyCursor wait;
    wxIcon icons[4];

    wxSize iconSize(size, size);

    icons[0] =                                                                 // closed
    icons[1] = wxArtProvider::GetIcon(wxART_FOLDER, wxART_LIST, iconSize);     // closed, selected
    icons[2] =                                                                 // open
    icons[3] = wxArtProvider::GetIcon(wxART_FOLDER_OPEN, wxART_LIST, iconSize);// open, selected

   for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
    {
        int sizeOrig = icons[i].GetWidth();
        if ( size == sizeOrig )
        {
            images->Add(icons[i]);
        }
        else
        {
            images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
        }
    }

    AssignButtonsImageList(images);
#else
void swat_tree_control::CreateButtonsImageList(int WXUNUSED(size))
{
#endif
}

int swat_tree_control::OnCompareItems(const wxTreeItemId& item1,
                               const wxTreeItemId& item2)
{
    if ( m_reverseSort )
    {
        // just exchange 1st and 2nd items
        return wxTreeCtrl::OnCompareItems(item2, item1);
    }
    else
    {
        return wxTreeCtrl::OnCompareItems(item1, item2);
    }
}

void swat_tree_control::AddItemsRecursively(const wxTreeItemId& idParent,
                                     size_t numChildren,
                                     size_t depth,
                                     size_t folder)
{
    if ( depth > 0 )
    {
        bool hasChildren = depth > 1;

        wxString str;
        for ( size_t n = 0; n < numChildren; n++ )
        {
            // at depth 1 elements won't have any more children
            if ( hasChildren )
                str.Printf(wxT("%s child %u"), wxT("Folder"), unsigned(n + 1));
            else
                str.Printf(wxT("%s child %u.%u"), wxT("File"), unsigned(folder), unsigned(n + 1));

            // here we pass to AppendItem() normal and selected item images (we
            // suppose that selected image follows the normal one in the enum)
            int image, imageSel;
            if (TREE_SHOW_IMAGES)
            {
                image = depth == 1 ? TreeCtrlIcon_File : TreeCtrlIcon_Folder;
                imageSel = image + 1;
            }
            else
            {
                image = imageSel = -1;
            }
            wxTreeItemId id = AppendItem(idParent, str, image, imageSel,
                                         new swat_tree_item_data(str));

            if (TREE_SHOW_STATES)
                SetItemState(id, 0);

            // and now we also set the expanded one (only for the folders)
            if ( hasChildren && TREE_SHOW_IMAGES ) {
                SetItemImage(id, TreeCtrlIcon_FolderOpened,
                             wxTreeItemIcon_Expanded);
            }

            // remember the last child for OnEnsureVisible()
            if ( !hasChildren && n == numChildren - 1 )
            {
                m_lastItem = id;
            }

            AddItemsRecursively(id, numChildren, depth - 1, n + 1);
        }
    }
    //else: done!
}

void swat_tree_control::Update ()
{
	/*
    int image = TREE_SHOW_IMAGES ? swat_tree_control::TreeCtrlIcon_Folder : -1;

    wxTreeItemId rootId = AddRoot(wxT("Root"), image, image, new swat_tree_item_data(wxT("Root item")));

    if ( !HasFlag(wxTR_HIDE_ROOT) && image != -1 ) {
        SetItemImage(rootId, TreeCtrlIcon_FolderOpened, wxTreeItemIcon_Expanded);
    }
	*/

	// AddItemsRecursively(rootId, NUM_CHILDREN_PER_LEVEL, NUM_LEVELS, 0);

	interface_tree_wx tree_wx;
	interface_tree_leaf_wx *root;
	tree_wx.tree = this;

	if (control->routing.outlet)
		root = (interface_tree_leaf_wx *) control->routing.outlet->write_tree (&tree_wx, NULL, control);
	else
		root = NULL;

	if (root) {
		wxTreeItemIdValue cookie;
		wxTreeItemId id = GetFirstChild (root->id, cookie);
		if (id) {
			// set some colours/fonts for testing
			if ( !HasFlag(wxTR_HIDE_ROOT) )
				SetItemFont(id, *wxITALIC_FONT);

			SetItemTextColour(id, *wxBLUE);

			id = GetNextChild(root->id, cookie);
			if ( id )
				id = GetNextChild(root->id, cookie);
			if ( id )
			{
				SetItemTextColour(id, *wxRED);
				SetItemBackgroundColour(id, *wxLIGHT_GREY);
			}
		}
	}
}

void swat_tree_control::GetItemsRecursively(const wxTreeItemId& idParent,
                                     wxTreeItemIdValue cookie)
{
    wxTreeItemId id;

    if ( !cookie )
        id = GetFirstChild(idParent, cookie);
    else
        id = GetNextChild(idParent, cookie);

    if ( !id.IsOk() )
        return;

    wxString text = GetItemText(id);
    // wxLogMessage(text);

    if (ItemHasChildren(id))
        GetItemsRecursively(id);

    GetItemsRecursively(idParent, cookie);
}

void swat_tree_control::DoToggleIcon(const wxTreeItemId& item)
{
    int image = GetItemImage(item) == TreeCtrlIcon_Folder
                    ? TreeCtrlIcon_File
                    : TreeCtrlIcon_Folder;
    SetItemImage(item, image, wxTreeItemIcon_Normal);

    image = GetItemImage(item, wxTreeItemIcon_Selected) == TreeCtrlIcon_FolderSelected
                    ? TreeCtrlIcon_FileSelected
                    : TreeCtrlIcon_FolderSelected;
    SetItemImage(item, image, wxTreeItemIcon_Selected);
}

void swat_tree_control::DoToggleState(const wxTreeItemId& item)
{
    // we have only 2 checkbox states, so next state will be reversed
    SetItemState(item, wxTREE_ITEMSTATE_NEXT);
}

void swat_tree_control::DoResetBrokenStateImages(const wxTreeItemId& idParent,
                                          wxTreeItemIdValue cookie, int state)
{
    wxTreeItemId id;

    if ( !cookie )
        id = GetFirstChild(idParent, cookie);
    else
        id = GetNextChild(idParent, cookie);

    if ( !id.IsOk() )
        return;

    int curState = GetItemState(id);
    if ( curState != wxTREE_ITEMSTATE_NONE && curState > state )
        SetItemState(id, state);

    if (ItemHasChildren(id))
        DoResetBrokenStateImages(id, 0, state);

    DoResetBrokenStateImages(idParent, cookie, state);
}

void swat_tree_control::LogEvent(const wxChar *name, const wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    wxString text;
    if ( item.IsOk() )
        text << wxT('"') << GetItemText(item).c_str() << wxT('"');
    else
        text = wxT("invalid item");
    // wxLogMessage(wxT("%s(%s)"), name, text.c_str());
}

// avoid repetition
#define TREE_EVENT_HANDLER(name)                                 \
void swat_tree_control::name(wxTreeEvent& event)                        \
{                                                                \
    LogEvent(wxT(#name), event);                                  \
    SetLastItem(wxTreeItemId());                                 \
    event.Skip();                                                \
}

TREE_EVENT_HANDLER(OnBeginRDrag)
TREE_EVENT_HANDLER(OnDeleteItem)
TREE_EVENT_HANDLER(OnGetInfo)
TREE_EVENT_HANDLER(OnSetInfo)
TREE_EVENT_HANDLER(OnItemExpanded)
TREE_EVENT_HANDLER(OnItemExpanding)
TREE_EVENT_HANDLER(OnItemCollapsed)
TREE_EVENT_HANDLER(OnSelChanged)
TREE_EVENT_HANDLER(OnSelChanging)

#undef TREE_EVENT_HANDLER

void LogKeyEvent(const wxChar *name, const wxKeyEvent& event)
{
    wxString key;
    long keycode = event.GetKeyCode();
    {
        switch ( keycode )
        {
            case WXK_BACK: key = wxT("BACK"); break;
            case WXK_TAB: key = wxT("TAB"); break;
            case WXK_RETURN: key = wxT("RETURN"); break;
            case WXK_ESCAPE: key = wxT("ESCAPE"); break;
            case WXK_SPACE: key = wxT("SPACE"); break;
            case WXK_DELETE: key = wxT("DELETE"); break;
            case WXK_START: key = wxT("START"); break;
            case WXK_LBUTTON: key = wxT("LBUTTON"); break;
            case WXK_RBUTTON: key = wxT("RBUTTON"); break;
            case WXK_CANCEL: key = wxT("CANCEL"); break;
            case WXK_MBUTTON: key = wxT("MBUTTON"); break;
            case WXK_CLEAR: key = wxT("CLEAR"); break;
            case WXK_SHIFT: key = wxT("SHIFT"); break;
            case WXK_ALT: key = wxT("ALT"); break;
            case WXK_CONTROL: key = wxT("CONTROL"); break;
            case WXK_MENU: key = wxT("MENU"); break;
            case WXK_PAUSE: key = wxT("PAUSE"); break;
            case WXK_CAPITAL: key = wxT("CAPITAL"); break;
            case WXK_END: key = wxT("END"); break;
            case WXK_HOME: key = wxT("HOME"); break;
            case WXK_LEFT: key = wxT("LEFT"); break;
            case WXK_UP: key = wxT("UP"); break;
            case WXK_RIGHT: key = wxT("RIGHT"); break;
            case WXK_DOWN: key = wxT("DOWN"); break;
            case WXK_SELECT: key = wxT("SELECT"); break;
            case WXK_PRINT: key = wxT("PRINT"); break;
            case WXK_EXECUTE: key = wxT("EXECUTE"); break;
            case WXK_SNAPSHOT: key = wxT("SNAPSHOT"); break;
            case WXK_INSERT: key = wxT("INSERT"); break;
            case WXK_HELP: key = wxT("HELP"); break;
            case WXK_NUMPAD0: key = wxT("NUMPAD0"); break;
            case WXK_NUMPAD1: key = wxT("NUMPAD1"); break;
            case WXK_NUMPAD2: key = wxT("NUMPAD2"); break;
            case WXK_NUMPAD3: key = wxT("NUMPAD3"); break;
            case WXK_NUMPAD4: key = wxT("NUMPAD4"); break;
            case WXK_NUMPAD5: key = wxT("NUMPAD5"); break;
            case WXK_NUMPAD6: key = wxT("NUMPAD6"); break;
            case WXK_NUMPAD7: key = wxT("NUMPAD7"); break;
            case WXK_NUMPAD8: key = wxT("NUMPAD8"); break;
            case WXK_NUMPAD9: key = wxT("NUMPAD9"); break;
            case WXK_MULTIPLY: key = wxT("MULTIPLY"); break;
            case WXK_ADD: key = wxT("ADD"); break;
            case WXK_SEPARATOR: key = wxT("SEPARATOR"); break;
            case WXK_SUBTRACT: key = wxT("SUBTRACT"); break;
            case WXK_DECIMAL: key = wxT("DECIMAL"); break;
            case WXK_DIVIDE: key = wxT("DIVIDE"); break;
            case WXK_F1: key = wxT("F1"); break;
            case WXK_F2: key = wxT("F2"); break;
            case WXK_F3: key = wxT("F3"); break;
            case WXK_F4: key = wxT("F4"); break;
            case WXK_F5: key = wxT("F5"); break;
            case WXK_F6: key = wxT("F6"); break;
            case WXK_F7: key = wxT("F7"); break;
            case WXK_F8: key = wxT("F8"); break;
            case WXK_F9: key = wxT("F9"); break;
            case WXK_F10: key = wxT("F10"); break;
            case WXK_F11: key = wxT("F11"); break;
            case WXK_F12: key = wxT("F12"); break;
            case WXK_F13: key = wxT("F13"); break;
            case WXK_F14: key = wxT("F14"); break;
            case WXK_F15: key = wxT("F15"); break;
            case WXK_F16: key = wxT("F16"); break;
            case WXK_F17: key = wxT("F17"); break;
            case WXK_F18: key = wxT("F18"); break;
            case WXK_F19: key = wxT("F19"); break;
            case WXK_F20: key = wxT("F20"); break;
            case WXK_F21: key = wxT("F21"); break;
            case WXK_F22: key = wxT("F22"); break;
            case WXK_F23: key = wxT("F23"); break;
            case WXK_F24: key = wxT("F24"); break;
            case WXK_NUMLOCK: key = wxT("NUMLOCK"); break;
            case WXK_SCROLL: key = wxT("SCROLL"); break;
            case WXK_PAGEUP: key = wxT("PAGEUP"); break;
            case WXK_PAGEDOWN: key = wxT("PAGEDOWN"); break;
            case WXK_NUMPAD_SPACE: key = wxT("NUMPAD_SPACE"); break;
            case WXK_NUMPAD_TAB: key = wxT("NUMPAD_TAB"); break;
            case WXK_NUMPAD_ENTER: key = wxT("NUMPAD_ENTER"); break;
            case WXK_NUMPAD_F1: key = wxT("NUMPAD_F1"); break;
            case WXK_NUMPAD_F2: key = wxT("NUMPAD_F2"); break;
            case WXK_NUMPAD_F3: key = wxT("NUMPAD_F3"); break;
            case WXK_NUMPAD_F4: key = wxT("NUMPAD_F4"); break;
            case WXK_NUMPAD_HOME: key = wxT("NUMPAD_HOME"); break;
            case WXK_NUMPAD_LEFT: key = wxT("NUMPAD_LEFT"); break;
            case WXK_NUMPAD_UP: key = wxT("NUMPAD_UP"); break;
            case WXK_NUMPAD_RIGHT: key = wxT("NUMPAD_RIGHT"); break;
            case WXK_NUMPAD_DOWN: key = wxT("NUMPAD_DOWN"); break;
            case WXK_NUMPAD_PAGEUP: key = wxT("NUMPAD_PAGEUP"); break;
            case WXK_NUMPAD_PAGEDOWN: key = wxT("NUMPAD_PAGEDOWN"); break;
            case WXK_NUMPAD_END: key = wxT("NUMPAD_END"); break;
            case WXK_NUMPAD_BEGIN: key = wxT("NUMPAD_BEGIN"); break;
            case WXK_NUMPAD_INSERT: key = wxT("NUMPAD_INSERT"); break;
            case WXK_NUMPAD_DELETE: key = wxT("NUMPAD_DELETE"); break;
            case WXK_NUMPAD_EQUAL: key = wxT("NUMPAD_EQUAL"); break;
            case WXK_NUMPAD_MULTIPLY: key = wxT("NUMPAD_MULTIPLY"); break;
            case WXK_NUMPAD_ADD: key = wxT("NUMPAD_ADD"); break;
            case WXK_NUMPAD_SEPARATOR: key = wxT("NUMPAD_SEPARATOR"); break;
            case WXK_NUMPAD_SUBTRACT: key = wxT("NUMPAD_SUBTRACT"); break;
            case WXK_NUMPAD_DECIMAL: key = wxT("NUMPAD_DECIMAL"); break;

            default:
            {
               if ( keycode < 128 && wxIsprint((int)keycode) )
                   key.Printf(wxT("'%c'"), (char)keycode);
               else if ( keycode > 0 && keycode < 27 )
                   key.Printf(_("Ctrl-%c"), wxT('A') + keycode - 1);
               else
                   key.Printf(wxT("unknown (%ld)"), keycode);
            }
        }
    }

    // wxLogMessage( wxT("%s event: %s (flags = %c%c%c%c)"), name, key.c_str(), event.ControlDown() ? wxT('C') : wxT('-'), event.AltDown() ? wxT('A') : wxT('-'), event.ShiftDown() ? wxT('S') : wxT('-'), event.MetaDown() ? wxT('M') : wxT('-'));
}

void swat_tree_control::OnTreeKeyDown(wxTreeEvent& event)
{
    LogKeyEvent(wxT("Tree key down "), event.GetKeyEvent());

    event.Skip();
}

void swat_tree_control::OnBeginDrag(wxTreeEvent& event)
{
    // need to explicitly allow drag
    if ( event.GetItem() != GetRootItem() )
    {
        m_draggedItem = event.GetItem();

        wxPoint clientpt = event.GetPoint();
        wxPoint screenpt = ClientToScreen(clientpt);

        wxLogMessage(wxT("OnBeginDrag: started dragging %s at screen coords (%i,%i)"),
                     GetItemText(m_draggedItem).c_str(),
                     screenpt.x, screenpt.y);

        event.Allow();
    }
    else
    {
        wxLogMessage(wxT("OnBeginDrag: this item can't be dragged."));
    }
}

void swat_tree_control::OnEndDrag(wxTreeEvent& event)
{
    wxTreeItemId itemSrc = m_draggedItem,
                 itemDst = event.GetItem();
    m_draggedItem = (wxTreeItemId)0l;

    // where to copy the item?
    if ( itemDst.IsOk() && !ItemHasChildren(itemDst) )
    {
        // copy to the parent then
        itemDst = GetItemParent(itemDst);
    }

    if ( !itemDst.IsOk() )
    {
        wxLogMessage(wxT("OnEndDrag: can't drop here."));

        return;
    }

    wxString text = GetItemText(itemSrc);
    wxLogMessage(wxT("OnEndDrag: '%s' copied to '%s'."),
                 text.c_str(), GetItemText(itemDst).c_str());

    // just do append here - we could also insert it just before/after the item
    // on which it was dropped, but this requires slightly more work... we also
    // completely ignore the client data and icon of the old item but could
    // copy them as well.
    //
    // Finally, we only copy one item here but we might copy the entire tree if
    // we were dragging a folder.
    int image = TREE_SHOW_IMAGES ? TreeCtrlIcon_File : -1;
    wxTreeItemId id = AppendItem(itemDst, text, image);

    if ( TREE_SHOW_STATES )
        SetItemState(id, GetItemState(itemSrc));
}

void swat_tree_control::OnBeginLabelEdit(wxTreeEvent& event)
{
    // wxLogMessage(wxT("OnBeginLabelEdit"));

    // for testing, prevent this item's label editing
    wxTreeItemId itemId = event.GetItem();
    if ( IsTestItem(itemId) )
    {
        wxMessageBox(wxT("You can't edit this item."));

        event.Veto();
    }
    else if ( itemId == GetRootItem() )
    {
        // test that it is possible to change the text of the item being edited
        SetItemText(itemId, wxT("Editing root item"));
    }
}

void swat_tree_control::OnEndLabelEdit(wxTreeEvent& event)
{
    // wxLogMessage(wxT("OnEndLabelEdit"));

    // don't allow anything except letters in the labels
    if ( !event.GetLabel().IsWord() )
    {
        wxMessageBox(wxT("The new label should be a single word."));

        event.Veto();
    }
}

void swat_tree_control::OnItemCollapsing(wxTreeEvent& event)
{
    // wxLogMessage(wxT("OnItemCollapsing"));

    // for testing, prevent the user from collapsing the first child folder
    wxTreeItemId itemId = event.GetItem();
    if ( IsTestItem(itemId) )
    {
        wxMessageBox(wxT("You can't collapse this item."));

        event.Veto();
    }
}

void swat_tree_control::OnItemActivated(wxTreeEvent& event)
{
    // show some info about this item
    wxTreeItemId itemId = event.GetItem();
    swat_tree_item_data *item = (swat_tree_item_data *)GetItemData(itemId);

    if ( item != NULL )
    {
        item->ShowInfo(this);
    }

    // wxLogMessage(wxT("OnItemActivated"));
}

void swat_tree_control::OnItemStateClick(wxTreeEvent& event)
{
    // toggle item state
    wxTreeItemId itemId = event.GetItem();
    DoToggleState(itemId);

    // wxLogMessage(wxT("Item \"%s\" state changed to %d"), GetItemText(itemId), GetItemState(itemId));
}

void swat_tree_control::OnItemMenu(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxCHECK_RET( itemId.IsOk(), "should have a valid item" );

    // swat_tree_item_data *item = (swat_tree_item_data *)GetItemData(itemId);
    wxPoint clientpt = event.GetPoint();
    wxPoint screenpt = ClientToScreen(clientpt);

    // wxLogMessage(wxT("OnItemMenu for item \"%s\" at screen coords (%i, %i)"), item->GetDesc(), screenpt.x, screenpt.y);

    ShowMenu(itemId, clientpt);
    event.Skip();
}

void swat_tree_control::OnContextMenu(wxContextMenuEvent& event)
{
    wxPoint pt = event.GetPosition();

    // wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);

    event.Skip();
}

void swat_tree_control::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
    wxString title;
    if ( id.IsOk() )
    {
        title << wxT("Menu for ") << GetItemText(id);
    }
    else
    {
        title = wxT("Menu for no particular item");
    }

#if wxUSE_MENUS
    wxMenu menu(title);
    menu.Append(TreeTest_About, wxT("&About"));
    menu.AppendSeparator();
    menu.Append(TreeTest_Highlight, wxT("&Highlight item"));
    menu.Append(TreeTest_Dump, wxT("&Dump"));

    PopupMenu(&menu, pt);
#endif // wxUSE_MENUS
}

void swat_tree_control::OnItemRClick(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxCHECK_RET( itemId.IsOk(), "should have a valid item" );

    // swat_tree_item_data *item = (swat_tree_item_data *)GetItemData(itemId);
	// if (item)
	    // wxLogMessage(wxT("Item \"%s\" right clicked"), item->GetDesc());

    event.Skip();
}

void swat_tree_control::OnRMouseDown(wxMouseEvent& event)
{
    // wxLogMessage(wxT("Right mouse button down"));

    event.Skip();
}

void swat_tree_control::OnRMouseUp(wxMouseEvent& event)
{
    // wxLogMessage(wxT("Right mouse button up"));

    event.Skip();
}

void swat_tree_control::OnRMouseDClick(wxMouseEvent& event)
{
    wxTreeItemId id = HitTest(event.GetPosition());
    if ( !id )
    {
        // wxLogMessage(wxT("No item under mouse"));
    }
    else
    {
        swat_tree_item_data *item = (swat_tree_item_data *)GetItemData(id);
        if ( item )
        {
            // wxLogMessage(wxT("Item '%s' under mouse"), item->GetDesc());
        }
    }

    event.Skip();
}

static inline const wxChar *Bool2String(bool b)
{
    return b ? wxT("") : wxT("not ");
}

void swat_tree_item_data::ShowInfo(wxTreeCtrl *tree)
{
    wxLogMessage(wxT("Item '%s': %sselected, %sexpanded, %sbold,\n")
                 wxT("%u children (%u immediately under this item)."),
                 m_desc.c_str(),
                 Bool2String(tree->IsSelected(GetId())),
                 Bool2String(tree->IsExpanded(GetId())),
                 Bool2String(tree->IsBold(GetId())),
                 unsigned(tree->GetChildrenCount(GetId())),
                 unsigned(tree->GetChildrenCount(GetId(), false)));
}
