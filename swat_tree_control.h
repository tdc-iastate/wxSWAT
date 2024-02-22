class swat_tree_item_data : public wxTreeItemData
{
public:
    swat_tree_item_data(const wxString& desc) : m_desc(desc) { }

    void ShowInfo(wxTreeCtrl *tree);
    const wxChar *GetDesc() const { return m_desc.c_str(); }

private:
    wxString m_desc;
};

class swat_tree_control : public wxTreeCtrl
{
public:
    enum
    {
        TreeCtrlIcon_File,
        TreeCtrlIcon_FileSelected,
        TreeCtrlIcon_Folder,
        TreeCtrlIcon_FolderSelected,
        TreeCtrlIcon_FolderOpened
    };

    swat_tree_control()
	{
	};
    swat_tree_control
		(wxWindow *parent,
		const wxWindowID id,
        const wxPoint& pos,
		const wxSize& size,
        long style,
		class SWATControlRecord *);
    virtual ~swat_tree_control(){};

    void OnBeginDrag(wxTreeEvent& event);
    void OnBeginRDrag(wxTreeEvent& event);
    void OnEndDrag(wxTreeEvent& event);
    void OnBeginLabelEdit(wxTreeEvent& event);
    void OnEndLabelEdit(wxTreeEvent& event);
    void OnDeleteItem(wxTreeEvent& event);
    void OnContextMenu(wxContextMenuEvent& event);
    void OnItemMenu(wxTreeEvent& event);
    void OnGetInfo(wxTreeEvent& event);
    void OnSetInfo(wxTreeEvent& event);
    void OnItemExpanded(wxTreeEvent& event);
    void OnItemExpanding(wxTreeEvent& event);
    void OnItemCollapsed(wxTreeEvent& event);
    void OnItemCollapsing(wxTreeEvent& event);
    void OnSelChanged(wxTreeEvent& event);
    void OnSelChanging(wxTreeEvent& event);
    void OnTreeKeyDown(wxTreeEvent& event);
    void OnItemActivated(wxTreeEvent& event);
    void OnItemStateClick(wxTreeEvent& event);
    void OnItemRClick(wxTreeEvent& event);

    void OnRMouseDown(wxMouseEvent& event);
    void OnRMouseUp(wxMouseEvent& event);
    void OnRMouseDClick(wxMouseEvent& event);

    void GetItemsRecursively(const wxTreeItemId& idParent,
                             wxTreeItemIdValue cookie = 0);

    void CreateImageList(int size = 16);
    void CreateButtonsImageList(int size = 11);
    void CreateStateImageList(bool del = false);

    void Update ();

    void DoSortChildren(const wxTreeItemId& item, bool reverse = false)
        { m_reverseSort = reverse; wxTreeCtrl::SortChildren(item); }
    void DoEnsureVisible() { if (m_lastItem.IsOk()) EnsureVisible(m_lastItem); }

    void DoToggleIcon(const wxTreeItemId& item);
    void DoToggleState(const wxTreeItemId& item);

    void ShowMenu(wxTreeItemId id, const wxPoint& pt);

    int ImageSize(void) const { return m_imageSize; }

    void SetLastItem(wxTreeItemId id) { m_lastItem = id; }

    void ResetBrokenStateImages()
    {
        const size_t count = GetStateImageList()->GetImageCount();
        int state = count > 0 ? count - 1 : wxTREE_ITEMSTATE_NONE;
        DoResetBrokenStateImages(GetRootItem(), 0, state);
    }

		protected:
		class SWATControlRecord *control;
		virtual int OnCompareItems(const wxTreeItemId& i1, const wxTreeItemId& i2);

    // is this the test item which we use in several event handlers?
    bool IsTestItem(const wxTreeItemId& item)
    {
        // the test item is the first child folder
        return GetItemParent(item) == GetRootItem() && !GetPrevSibling(item);
    }

private:
    void AddItemsRecursively(const wxTreeItemId& idParent,
                             size_t nChildren,
                             size_t depth,
                             size_t folder);

    void DoResetBrokenStateImages(const wxTreeItemId& idParent,
                                  wxTreeItemIdValue cookie, int state);

    void LogEvent(const wxChar *name, const wxTreeEvent& event);

    int          m_imageSize;               // current size of images
    bool         m_reverseSort;             // flag for OnCompareItems
    wxTreeItemId m_lastItem,                // for OnEnsureVisible()
                 m_draggedItem;             // item being dragged right now

    // NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
    //     if you want your overloaded OnCompareItems() to be called.
    //     OTOH, if you don't want it you may omit the next line - this will
    //     make default (alphabetical) sorting much faster under wxMSW.
    DECLARE_DYNAMIC_CLASS(MyTreeCtrl)
    DECLARE_EVENT_TABLE()
};

enum
{
    TreeTest_Quit = wxID_EXIT,
    TreeTest_About = wxID_ABOUT,
    TreeTest_ClearLog = wxID_CLEAR,
    TreeTest_TogButtons = wxID_HIGHEST,
    TreeTest_TogTwist,
    TreeTest_TogLines,
    TreeTest_TogEdit,
    TreeTest_TogHideRoot,
    TreeTest_TogRootLines,
    TreeTest_TogBorder,
    TreeTest_TogFullHighlight,
    TreeTest_SetFgColour,
    TreeTest_SetBgColour,
    TreeTest_ResetStyle,
    TreeTest_Highlight,
    TreeTest_Dump,
    TreeTest_DumpSelected,
    TreeTest_Count,
    TreeTest_CountRec,
    TreeTest_Sort,
    TreeTest_SortRev,
    TreeTest_SetBold,
    TreeTest_ClearBold,
    TreeTest_Rename,
    TreeTest_Delete,
    TreeTest_DeleteChildren,
    TreeTest_DeleteAll,
    TreeTest_Recreate,
    TreeTest_ToggleImages,
    TreeTest_ToggleStates,
    TreeTest_ToggleBell,
    TreeTest_ToggleAlternateImages,
    TreeTest_ToggleAlternateStates,
    TreeTest_ToggleButtons,
    TreeTest_SetImageSize,
    TreeTest_ToggleSel,
    TreeTest_CollapseAndReset,
    TreeTest_EnsureVisible,
    TreeTest_SetFocus,
    TreeTest_AddItem,
    TreeTest_AddManyItems,
    TreeTest_InsertItem,
    TreeTest_IncIndent,
    TreeTest_DecIndent,
    TreeTest_IncSpacing,
    TreeTest_DecSpacing,
    TreeTest_ToggleIcon,
    TreeTest_ToggleState,
    TreeTest_Select,
    TreeTest_Unselect,
    TreeTest_SelectRoot,
    TreeTest_ClearFocused,
    TreeTest_SetFocusedRoot,
    TreeTest_SelectChildren,
    TreeTest_ShowFirstVisible,
    TreeTest_ShowLastVisible,
    TreeTest_ShowNextVisible,
    TreeTest_ShowPrevVisible,
    TreeTest_ShowParent,
    TreeTest_ShowPrevSibling,
    TreeTest_ShowNextSibling,
    TreeTest_ScrollTo,
    TreeTest_SelectLast,
    TreeTest_Ctrl = 1000
};
