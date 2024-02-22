const int LIST_CTRL = 1000;

class list_control_records : public wxListCtrl {
public:
    list_control_records
		(wxWindow *parent,
		std::vector <SWATControlRecord *> *,
        const wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style);

	long GetSelectedID ();

    // add one item to the listctrl in report mode
    void InsertItemInReportView(int i);

    void OnColClick(wxListEvent& event);
    void OnColRightClick(wxListEvent& event);
    void OnColBeginDrag(wxListEvent& event);
    void OnColDragging(wxListEvent& event);
    void OnColEndDrag(wxListEvent& event);
    void OnBeginDrag(wxListEvent& event);
    void OnBeginRDrag(wxListEvent& event);
    void OnBeginLabelEdit(wxListEvent& event);
    void OnEndLabelEdit(wxListEvent& event);
    void OnDeleteItem(wxListEvent& event);
    void OnDeleteAllItems(wxListEvent& event);
    void OnSelected(wxListEvent& event);
    void OnDeselected(wxListEvent& event);
    void OnListKeyDown(wxListEvent& event);
    void OnActivated(wxListEvent& event);
    void OnFocused(wxListEvent& event);
    void OnCacheHint(wxListEvent& event);

    void OnChar(wxKeyEvent& event);

    void OnContextMenu(wxContextMenuEvent& event);
    void OnRightClick(wxMouseEvent& event);
	void next_record ();
	void previous_record ();
	virtual void Update ();

protected:
	std::vector <SWATControlRecord *> *control_records;
    void ShowContextMenu(const wxPoint& pos);
    wxLog *m_logOld;
    void SetColumnImage(int col, int image);

    void LogEvent(const wxListEvent& event, const wxChar *eventName);
    void LogColEvent(const wxListEvent& event, const wxChar *eventName);

    virtual wxString OnGetItemText(long item, long column) const;
    virtual int OnGetItemColumnImage(long item, long column) const;
    virtual wxListItemAttr *OnGetItemAttr(long item) const;

    wxListItemAttr m_attr;

	// std::vector <dynamic_string> list_data;

    long m_updated;


    // wxDECLARE_NO_COPY_CLASS(list_control_records);
    DECLARE_EVENT_TABLE()
};

enum
{
    LIST_ABOUT = wxID_ABOUT,
    LIST_QUIT = wxID_EXIT,

    LIST_LIST_VIEW = wxID_HIGHEST,
    LIST_ICON_VIEW,
    LIST_ICON_TEXT_VIEW,
    LIST_SMALL_ICON_VIEW,
    LIST_SMALL_ICON_TEXT_VIEW,
    LIST_REPORT_VIEW,
    LIST_VIRTUAL_VIEW,
    LIST_SMALL_VIRTUAL_VIEW,
    LIST_SET_ITEMS_COUNT,

    LIST_DESELECT_ALL,
    LIST_SELECT_ALL,
    LIST_DELETE_ALL,
    LIST_DELETE,
    LIST_ADD,
    LIST_EDIT,
    LIST_SORT,
    LIST_FIND,
    LIST_SET_FG_COL,
    LIST_SET_BG_COL,
    LIST_TOGGLE_MULTI_SEL,
    LIST_TOGGLE_HEADER,
    LIST_TOGGLE_FIRST,
    LIST_SHOW_COL_INFO,
    LIST_SHOW_SEL_INFO,
    LIST_SHOW_VIEW_RECT,
#ifdef wxHAS_LISTCTRL_COLUMN_ORDER
    LIST_SET_COL_ORDER,
    LIST_GET_COL_ORDER,
#endif // wxHAS_LISTCTRL_COLUMN_ORDER
    LIST_GOTO,
    LIST_FOCUS_LAST,
    LIST_FREEZE,
    LIST_THAW,
    LIST_TOGGLE_LINES,
#ifdef __WXOSX__
    LIST_MAC_USE_GENERIC,
#endif
};
