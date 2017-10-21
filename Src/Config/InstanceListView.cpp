/*****************************************************************************
 InstanceListView.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "smconfig.h"

#include "InstanceListDoc.h"

#include "InstanceListView.h"

#include "GeneralPage.h"
#include "TimeoutPage.h"
#include "EmailPage.h"
#include "OdbcPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstanceListView

IMPLEMENT_DYNCREATE(CInstanceListView, CListView)

CInstanceListView::CInstanceListView()
	{
	}

CInstanceListView::~CInstanceListView()
	{
	}

BEGIN_MESSAGE_MAP(CInstanceListView, CListView)
	//{{AFX_MSG_MAP(CInstanceListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblClk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_STARTAUTOMATICALLY, OnUpdateServiceStartAutomatically)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_SERVICE_START, OnUpdateServiceStart)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_STOP, OnUpdateServiceStop)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_PAUSE, OnUpdateServicePause)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_CONTINUE, OnUpdateServiceContinue)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_RESTART, OnUpdateServiceRestart)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTIES, OnUpdateEditProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DUPLICATE, OnUpdateEditDuplicate)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SAVE, OnSave)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_COMMAND(ID_EDIT_DUPLICATE, OnEditDuplicate)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_NEW, OnEditNew)
	ON_COMMAND(ID_SERVICE_START, OnServiceStart)
	ON_COMMAND(ID_SERVICE_STOP, OnServiceStop)
	ON_COMMAND(ID_SERVICE_CONTINUE, OnServiceContinue)
	ON_COMMAND(ID_SERVICE_PAUSE, OnServicePause)
	ON_COMMAND(ID_SERVICE_RESTART, OnServiceRestart)
	ON_COMMAND(ID_SERVICE_STARTAUTOMATICALLY, OnServiceStartAutomatically)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstanceListView diagnostics

#ifdef _DEBUG
void CInstanceListView::AssertValid() const
	{
	CListView::AssertValid();
	}

void CInstanceListView::Dump(CDumpContext& dc) const
	{
	CListView::Dump(dc);
	}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInstanceListView message handlers

BOOL CInstanceListView::PreCreateWindow(CREATESTRUCT& cs) 
	{
	// apply standard styles
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT | LVS_NOSORTHEADER;

	return CListView::PreCreateWindow(cs);
	}

void CInstanceListView::OnInitialUpdate() 
	{
	TRACE("CInstanceListView::OnInitialUpdate:\n");

	CListCtrl& ListCtrl = GetListCtrl();

	// apply extended styles
	ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES |
		LVS_EX_FLATSB);

	// insert columns
	int col = 0;
	ListCtrl.InsertColumn(col++, "Instance", LVCFMT_LEFT, 60);
	ListCtrl.InsertColumn(col++, "Port", LVCFMT_LEFT, 60);
	ListCtrl.InsertColumn(col++, "Status", LVCFMT_LEFT, 75);
	ListCtrl.InsertColumn(col++, "Auto Start", LVCFMT_LEFT, 65);
	ListCtrl.InsertColumn(col++, "Log File", LVCFMT_LEFT, 300);

	//UpdateItems();

	// base class calls OnUpdate()
	CListView::OnInitialUpdate();

	GetDocument()->GetServiceStatus();

	unsigned dwRc = SetTimer(1, 500, NULL);
	ASSERT(dwRc);
	}

void CInstanceListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
	{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CInstanceListView::OnUpdate:\n");
	UpdateItems();
	}

void CInstanceListView::OnTimer(UINT nIDEvent) 
	{
	static unsigned TimerCount = 0;
	static BOOL flGetServiceStatusActive = FALSE;

	//TRACE("CInstanceListView::OnTimer: nIDEvent=%u, TimerCount=%u\n", nIDEvent, TimerCount);
	switch (nIDEvent)
		{
		case 1:
			TimerCount++;
			if (!flGetServiceStatusActive)
				{
				flGetServiceStatusActive = TRUE;
				GetDocument()->GetServiceStatus();
				flGetServiceStatusActive = FALSE;
				}
			else
				{
				//TRACE("CInstanceListView::OnTimer: GetServiceStatus already active\n");
				}
			break;
		default:	
			CListView::OnTimer(nIDEvent);
			break;
		}
	}

void CInstanceListView::UpdateItems()
	{
	BOOL flRc;

	CInstanceListDoc &doc = *GetDocument();
	std::map<unsigned, CInstanceParameters> &mapIp = GetDocument()->mapIp;

	TRACE("CInstanceListView::UpdateItems: mapIp.size=%d\n", mapIp.size());
	
	CListCtrl& list = GetListCtrl();

	//flRc = list.DeleteAllItems();
	//ASSERT(flRc);

	// insert or update items

	std::map<unsigned, CInstanceParameters>::const_iterator i;
	unsigned nItem = 0;
	char szInstance[33] = {0};
	int col = 0;
	for (i = mapIp.begin() ; i != mapIp.end() ; i++, nItem++)
		{

		LVFINDINFO fi = {0};
		fi.flags = LVFI_PARAM;
		fi.lParam = i->first;
		int nItemNew = list.FindItem(&fi);
		if (nItemNew == -1)
			{
			// insert item; LPARAM is used to hold the instance number
			ultoa(i->first, szInstance, 10);
			nItemNew = list.InsertItem(LVIF_PARAM, nItem, 0, 0, 0, 0, i->first);
			ASSERT(nItemNew != -1);

			col = 0;
			flRc = list.SetItemText(nItemNew, col++, szInstance);
			ASSERT(flRc);
			}

		col = 1;
		flRc = list.SetItemText(nItemNew, col++, i->second.listPp[0].strPortName.c_str());
		TRACE("SetItemText rc=%d, s=[%s]\n", flRc, i->second.listPp[0].strPortName.c_str());
		ASSERT(flRc);

		string strStatus("");
		switch (i->second.ssStatus.dwCurrentState)
			{
			case 0:
				strStatus = "";
				break;
			case SERVICE_STOPPED:
				strStatus = "Stopped";
				break;
			case SERVICE_RUNNING:
				strStatus = "Running";
				break;
			case SERVICE_PAUSED:
				strStatus = "Paused";
				break;
			case SERVICE_START_PENDING:
				strStatus = "Starting...";
				break;
			case SERVICE_STOP_PENDING:
				strStatus = "Stopping...";
				break;
			case SERVICE_PAUSE_PENDING:
				strStatus = "Pausing...";
				break;
			case SERVICE_CONTINUE_PENDING:
				strStatus = "Continuing...";
				break;
			default:
				strStatus = "Unknown";
				break;
			}		
		flRc = list.SetItemText(nItemNew, col++, strStatus.c_str());
		ASSERT(flRc);

		flRc = list.SetItemText(nItemNew, col++, i->second.flStartAutomatically ? "Yes" : "No");
		ASSERT(flRc);

		flRc = list.SetItemText(nItemNew, col++, i->second.strLogFilename.c_str());
		ASSERT(flRc);

		//TRACE("UpdateItems[%u,%d]: LogFilename=%s\n", i->first, nItemNew, i->second.strLogFilename.c_str());
		//TRACE("UpdateItems[%u]:    PortName=%s\n", i->first, i->second.listPp[0].strPortName.c_str());
		}
	}

void CInstanceListView::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	OnEditProperties(TRUE);		// don't display message if nothing is selected
	}

void CInstanceListView::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	CListCtrl& list = GetListCtrl();

	CPoint point;
	if (GetCursorPos(&point))
		// open instance context menu or "New" menu
		DoInstanceListContextMenu(GetSingleSelection() == -1 ? 1 : 0, point);

	*pResult = 0;
	}

void CInstanceListView::DoInstanceListContextMenu(const int iMenuType, const CPoint& point)
	{
	CListCtrl& list = GetListCtrl();

	// load the menus....
	CMenu menuPopups;
	if (menuPopups.LoadMenu(IDM_POPUPS))
		{
		// get the popup....
		CMenu* pMenu = menuPopups.GetSubMenu(iMenuType);
		if (pMenu)
			{
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
			//TRACE("CInstanceListView::DoInstanceListContextMenu: TrackPopupMenu returned\n");
			}
		}
	}

int CInstanceListView::GetSingleSelection(BOOL *flMultipleSelection)
	{
	CListCtrl& list = GetListCtrl();

	// this is the item in focus; not necessary selected
	int nItemFocus = list.GetSelectionMark();
	// determine if item in focus is selected
	BOOL flItemFocusSelected = list.GetItemState(nItemFocus, LVIS_SELECTED) ? TRUE : FALSE;
	// determine how many items are selected
	unsigned cSelected = list.GetSelectedCount();

	TRACE("CInstanceListView::GetSingleSelection: nItemFocus=%d, flItemFocusSelected=%u\n"
		  "cSelected=%d\n", nItemFocus, flItemFocusSelected, cSelected);

	// set flMultipleSelection output parameters
	if (flMultipleSelection)
		*flMultipleSelection = (cSelected > 1) ? TRUE : FALSE;
	
	if (nItemFocus == -1)
		{
		// no item is in focus; determine if at least one item is selected
		if (cSelected > 0)
			{
			// at least one item is selected; return first selected item
			POSITION pos = list.GetFirstSelectedItemPosition();
			if (pos)
				return list.GetNextSelectedItem(pos);
			else
				return -1;		// shouldn't happen because there's more than one selected item
			}
		else
			// no items are selected
			return -1;
		}
	else
		{
		// an item is in focus; determine if it is selected
		if (flItemFocusSelected)
			// in-focus item is also selected; return it
			return nItemFocus;
		else
			return -1;
		}
	}

int CInstanceListView::ForceSingleSelection(const BOOL flQuiet)
	{
	TRACE("CInstanceListView::ForceSingleSelection:\n");

	CListCtrl& list = GetListCtrl();

	int nItemSel = GetSingleSelection();
	if (nItemSel == -1)
		{
		// there is not an obvious single selected item; show error message
		if (!flQuiet)
			MessageBox("You must select an instance.", "Invalid Selection", MB_ICONEXCLAMATION);
		return -1;
		}
	else
		{
		// Select single "selected" item and deselect all other items
		list.SetItemState(nItemSel, LVIS_SELECTED, LVIS_SELECTED);

		POSITION pos = list.GetFirstSelectedItemPosition();
		if (pos)
			{
			while (pos)
				{
				int nItem = list.GetNextSelectedItem(pos);
				TRACE1("Item %d was selected!\n", nItem);
				// deselect any other selected item
				if (nItem != nItemSel)
					list.SetItemState(nItem, 0, LVIS_SELECTED);
				}
			}
		return nItemSel;
		}
	}

unsigned CInstanceListView::GetInstanceFromItem(int nItem)
	{
	CListCtrl& list = GetListCtrl();
	return list.GetItemData(nItem);
	}

void CInstanceListView::OnEditProperties()
	{
	OnEditProperties(FALSE);
	}

void CInstanceListView::OnEditProperties(const BOOL flQuiet)
	{
	DWORD dw = 0;

	TRACE("CInstanceListView::OnEditProperties:\n");

	CListCtrl& list = GetListCtrl();

	// Determine which item is selected and has focus.
	// Unselect any other items.

	BOOL flMultipleSelection = 0;
	int nItemSel = GetSingleSelection(&flMultipleSelection);
	if (nItemSel == -1)
		return;		// shouldn't happen

	unsigned nInstanceSel = GetInstanceFromItem(nItemSel);

	TRACE("CInstanceListView::OnEditProperties: nItemSel=%d, nInstance=%u\n", nItemSel, nInstanceSel);
	
	//
	// Create property sheet and pages
	//

	stringstream ssInstanceList("");

	POSITION pos = list.GetFirstSelectedItemPosition();
	while (pos)
		{
		int nItem = list.GetNextSelectedItem(pos);
		int nInstance = GetInstanceFromItem(nItem);
		if (!ssInstanceList.str().empty())
			ssInstanceList << ", ";
		ssInstanceList << nInstance;
		}

	string strTitle = string("Properties for Instance") + 
		(list.GetSelectedCount() == 1 ? "" : "s") + " " + ssInstanceList.str();

	CPropertySheet ps(strTitle.c_str(), this);
	
	CGeneralPage GeneralPage;
	CTimeoutPage TimeoutPage;
	CEmailPage EmailPage;
	COdbcPage OdbcPage;

	{
	//
	// Determine instance to edit
	//

	CInstanceParameters &ip = GetInstanceParameters(nInstanceSel);
	CPortParameters &pp = ip.listPp[0];

	//
	// Populate page member variables from CInstanceParameters structure
	//

	GeneralPage.ip = &ip;
	GeneralPage.flMultipleSelection = flMultipleSelection;

	GeneralPage.m_strPortName = pp.strPortName.c_str();
	GeneralPage.m_strLogFilename = ip.strLogFilename.c_str();
	GeneralPage.m_strBaudRate.Format("%d", pp.BaudRate);
	GeneralPage.m_nByteSize = pp.ByteSize - 4;
	GeneralPage.m_nParity = pp.Parity;
	GeneralPage.m_nStopBits = pp.StopBits;
	GeneralPage.m_nTimestampFormat = ip.dwTimestampFormat ? 1 : 0;
	GeneralPage.m_flLogStartStop = (ip.dwVerboseLevel & VERBOSE_STARTSTOP) ? 1 : 0;
	GeneralPage.m_flLogConfig = (ip.dwVerboseLevel & VERBOSE_CONFIGURATION) ? 1 : 0;
	GeneralPage.m_flLogTimeouts = (ip.dwVerboseLevel & VERBOSE_TIMEOUT) ? 1 : 0;
	GeneralPage.m_flLogEmail = (ip.dwVerboseLevel & VERBOSE_SMTPSUMMARY) ? 1 : 0;
	GeneralPage.m_nLogRotation = ip.dwLogRotation;
	GeneralPage.m_flStartAutomatically = ip.flStartAutomatically;

	TimeoutPage.m_iTimeoutSecGrp = (pp.dwTimeout ? 1 : 0);
	TimeoutPage.m_strTimeoutSec.Format("%d", pp.dwTimeout);
	TimeoutPage.m_iWarningIntervalGrp = (pp.dwWarningInterval ? 1 : 0);
	TimeoutPage.m_strWarningIntervalSec.Format("%d", pp.dwWarningInterval);			
	TimeoutPage.m_flSunday =	(pp.dwDayOfWeekMask & 0x40) ? 1 : 0;
	TimeoutPage.m_flMonday =	(pp.dwDayOfWeekMask & 0x20) ? 1 : 0;
	TimeoutPage.m_flTuesday =	(pp.dwDayOfWeekMask & 0x10) ? 1 : 0;
	TimeoutPage.m_flWednesday = (pp.dwDayOfWeekMask & 0x08) ? 1 : 0;
	TimeoutPage.m_flThursday =	(pp.dwDayOfWeekMask & 0x04) ? 1 : 0;
	TimeoutPage.m_flFriday =	(pp.dwDayOfWeekMask & 0x02) ? 1 : 0;
	TimeoutPage.m_flSaturday =	(pp.dwDayOfWeekMask & 0x01) ? 1 : 0;
	TimeoutPage.m_flEntireDay = (pp.dwStartHour == 0 && pp.dwStartMin == 0 && 
		pp.dwEndHour == 24 && pp.dwEndMin == 0);
	CTime time1(2000, 1, 1, pp.dwStartHour, pp.dwStartMin, 0);
	TimeoutPage.m_timeBegin = time1;
	CTime time2(2000, 1, 1, pp.dwEndHour, pp.dwEndMin, 0);
	TimeoutPage.m_timeEnd = time2;
	TimeoutPage.m_strTimeoutCmd = pp.strTimeoutCmd.c_str();
	TimeoutPage.m_strNetSendList = pp.strNetSendUsers.c_str();

	EmailPage.ip = &ip;
	EmailPage.pGeneralPage = &GeneralPage;
	EmailPage.m_strSmtpServer = ip.strSmtpServer.c_str();
	EmailPage.m_strSmtpFromAddress = ip.strSmtpFromAddress.c_str();
	EmailPage.m_strSmtpToAddress = ip.strSmtpToAddress.c_str();

	OdbcPage.m_strSQLConnectionString = ip.strSQLConnectionString.c_str();
	OdbcPage.m_strSQLSafeConnectionString = GetSafeConnectionString(ip.strSQLConnectionString).c_str();
	OdbcPage.m_strSQLWriteLineStatement = ip.strSQLWriteLineStatement.c_str();
	OdbcPage.m_dwMaxLineLength = ip.dwMaxProcessedLineLength;
	}

	//
	// Add pages to property sheet and display it; wait for OK or Cancel button
	//

	ps.AddPage(&GeneralPage);
	ps.AddPage(&TimeoutPage);
	ps.AddPage(&EmailPage);
	ps.AddPage(&OdbcPage);

	ps.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	int nResponse = ps.DoModal();

	if (nResponse == IDOK)
		{
		//
		// Copy page member variables to CInstanceParameters structure
		//

		POSITION pos = list.GetFirstSelectedItemPosition();
		while (pos)
			{
			int nItem = list.GetNextSelectedItem(pos);
			int nInstance = GetInstanceFromItem(nItem);
			ASSERT(nInstance != -1);
			CInstanceParameters &ip = GetInstanceParameters(nInstance);
			CPortParameters &pp = ip.listPp[0];

			TRACE("CInstanceListView::OnEditProperties: saved nItem=%d, nInstance=%u\n", nItem, nInstance);

			if (!flMultipleSelection)
				{
				pp.strPortName = GeneralPage.m_strPortName;
				ip.strLogFilename = GeneralPage.m_strLogFilename;
				}

			pp.BaudRate = atoi(GeneralPage.m_strBaudRate);
			pp.ByteSize = GeneralPage.m_nByteSize + 4;
			pp.Parity = GeneralPage.m_nParity;
			pp.StopBits = GeneralPage.m_nStopBits;
			ip.dwTimestampFormat = GeneralPage.m_nTimestampFormat;

			dw = ip.dwVerboseLevel & ~VERBOSE_CONFIGMASK;		// don't change bits not controlled by this app
			if (GeneralPage.m_flLogStartStop) dw |= VERBOSE_STARTSTOP;
			if (GeneralPage.m_flLogConfig) dw |= VERBOSE_CONFIGURATION;
			if (GeneralPage.m_flLogTimeouts) dw |= VERBOSE_TIMEOUT;
			if (GeneralPage.m_flLogEmail) dw |= VERBOSE_SMTPSUMMARY;
			ip.dwVerboseLevel = dw;
			//pp.dwVerboseLevel = dw;

			ip.dwLogRotation = GeneralPage.m_nLogRotation;

			ip.flStartAutomatically = GeneralPage.m_flStartAutomatically;

			pp.dwTimeout = (TimeoutPage.m_iTimeoutSecGrp == 1) ? 
					atoi(TimeoutPage.m_strTimeoutSec) : 0;
			pp.dwWarningInterval = (TimeoutPage.m_iWarningIntervalGrp == 1) ? 
					atoi(TimeoutPage.m_strWarningIntervalSec) : 0;

			dw = 0;
			if (TimeoutPage.m_flSunday) dw |= 0x40;
			if (TimeoutPage.m_flMonday) dw |= 0x20;
			if (TimeoutPage.m_flTuesday) dw |= 0x10;
			if (TimeoutPage.m_flWednesday) dw |= 0x08;
			if (TimeoutPage.m_flThursday) dw |= 0x04;
			if (TimeoutPage.m_flFriday) dw |= 0x02;
			if (TimeoutPage.m_flSaturday) dw |= 0x01;		
			pp.dwDayOfWeekMask = dw;

			pp.dwStartHour = TimeoutPage.m_flEntireDay ? 0 : TimeoutPage.m_timeBegin.GetHour();
			pp.dwStartMin = TimeoutPage.m_flEntireDay ? 0 : TimeoutPage.m_timeBegin.GetMinute();
			pp.dwEndHour = TimeoutPage.m_flEntireDay ? 24 : TimeoutPage.m_timeEnd.GetHour();
			pp.dwEndMin = TimeoutPage.m_flEntireDay ? 0 : TimeoutPage.m_timeEnd.GetMinute();

			pp.strNetSendUsers = TimeoutPage.m_strNetSendList;
			pp.strTimeoutCmd = TimeoutPage.m_strTimeoutCmd;
			
			ip.strSmtpServer = EmailPage.m_strSmtpServer;
			ip.strSmtpFromAddress = EmailPage.m_strSmtpFromAddress;
			ip.strSmtpToAddress = EmailPage.m_strSmtpToAddress;

			ip.strSQLConnectionString = OdbcPage.m_strSQLConnectionString;
			ip.strSQLWriteLineStatement = OdbcPage.m_strSQLWriteLineStatement;
			ip.dwMaxProcessedLineLength = OdbcPage.m_dwMaxLineLength;
			TRACE("CInstanceListView::OnEditProperties: connect=%s\n", ip.strSQLConnectionString.c_str());

			ip.CalcParameters();
			}

		GetDocument()->SetModifiedFlag();
		UpdateItems();
		}
	else if (nResponse == IDCANCEL)
		{
		}

	TRACE("CInstanceListView::OnEditProperties: Done.\n");
	}

void CInstanceListView::OnEditDuplicate()
	{
	TRACE("CInstanceListView::OnEditDuplicate:\n");

	CListCtrl& list = GetListCtrl();

	int nItemSel = ForceSingleSelection();
	if (nItemSel == -1)
		return;

	// deselect current item
	list.SetItemState(nItemSel, 0, LVIS_SELECTED);

	// map nItem to nInstance using LPARAM
	unsigned nInstanceSel = list.GetItemData(nItemSel);

	//CServiceParameters &sp = GetServiceParameters();
	unsigned nInstanceNew = GetDocument()->GetNextAvailableInstanceNumber();
	TRACE("OnEditDuplicate: nInstanceNew=%u\n", nInstanceNew);

	GetDocument()->mapIp[nInstanceNew] = GetDocument()->mapIp[nInstanceSel];
	GetDocument()->mapIp[nInstanceNew].SetServiceInstance(nInstanceNew);
	GetDocument()->mapIp[nInstanceNew].ssStatus.dwCurrentState = 0;
	GetDocument()->mapIp[nInstanceNew].flRestart = 0;
	UpdateItems();

	// find and select new instance
	LVFINDINFO fi = {0};
	fi.flags = LVFI_PARAM;
	fi.lParam = nInstanceNew;
	int nItemNew = list.FindItem(&fi);
	ASSERT(nItemNew != -1);
	list.SetSelectionMark(nItemNew);
	list.SetItemState(nItemNew, LVIS_SELECTED, LVIS_SELECTED);

	GetDocument()->SetModifiedFlag();
	}

void CInstanceListView::OnEditDelete()
	{
	TRACE("CInstanceListView::OnEditDelete:\n");

	CListCtrl& list = GetListCtrl();
	//CServiceParameters &sp = GetServiceParameters();

	for (;;)
		{	
		POSITION pos = list.GetFirstSelectedItemPosition();
		if (pos == NULL)
			break;

		int nItem = list.GetNextSelectedItem(pos);
		int nInstance = list.GetItemData(nItem);
		ASSERT(nInstance != -1);

		GetDocument()->mapIp.erase(nInstance);

		BOOL flRc = list.DeleteItem(nItem);
		ASSERT(flRc);
		}

	GetDocument()->SetModifiedFlag();
	}

void CInstanceListView::OnEditNew()
	{
	CListCtrl& list = GetListCtrl();

	//CServiceParameters &sp = GetServiceParameters();
	CInstanceParameters ip;
	CPortParameters pp;

	ip.listPp.push_back(pp);

	unsigned nInstanceNew = GetDocument()->GetNextAvailableInstanceNumber();
	TRACE("OnEditNew: nInstanceNew=%u\n", nInstanceNew);

	GetDocument()->mapIp[nInstanceNew] = ip;
	GetDocument()->mapIp[nInstanceNew].SetServiceInstance(nInstanceNew);
	GetDocument()->SetDefaults(nInstanceNew);
	UpdateItems();

	// deselect all items
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos)
		{
		while (pos)
			{
			int nItem = list.GetNextSelectedItem(pos);
			list.SetItemState(nItem, 0, LVIS_SELECTED);
			}
		}

	// find and select new instance
	LVFINDINFO fi = {0};
	fi.flags = LVFI_PARAM;
	fi.lParam = nInstanceNew;
	int nItemNew = list.FindItem(&fi);
	ASSERT(nItemNew != -1);
	list.SetSelectionMark(nItemNew);
	list.SetItemState(nItemNew, LVIS_SELECTED, LVIS_SELECTED);

	GetDocument()->SetModifiedFlag();
	}

void CInstanceListView::OnSave()
	{
	TRACE("CInstanceListView::OnSave: modified=%u\n", GetDocument()->IsModified());

	GetDocument()->Save();

	//GetDocument()->SetModifiedFlag(FALSE);
	TRACE("CInstanceListView::OnSave: after modified=%u\n", GetDocument()->IsModified());
	}

void CInstanceListView::OnEditSelectAll()
	{
	TRACE("CInstanceListView::OnEditSelectAll\n");

	CListCtrl& list = GetListCtrl();
	int cItems = list.GetItemCount();
	for (int nItem = 0 ; nItem < cItems ; nItem++)
		{
		list.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

void CInstanceListView::OnServiceStartAutomatically()
	{
	TRACE("CInstanceListView::OnServiceStartAutomatically: m_flLastStartAutomatically=%u\n",
		m_flLastStartAutomatically);

	CListCtrl& list = GetListCtrl();
	POSITION pos = list.GetFirstSelectedItemPosition();
	while (pos)
		{
		int nItem = list.GetNextSelectedItem(pos);
		CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItem));
		ip.flStartAutomatically = !m_flLastStartAutomatically;
		}
	UpdateItems();
	GetDocument()->SetModifiedFlag();
	}

void CInstanceListView::OnServiceStart()
	{
	ControlServices(0x80000000);
	}

void CInstanceListView::OnServiceStop()
	{
	ControlServices(SERVICE_CONTROL_STOP);
	}

void CInstanceListView::OnServiceContinue()
	{
	ControlServices(SERVICE_CONTROL_CONTINUE);
	}

void CInstanceListView::OnServicePause()
	{
	ControlServices(SERVICE_CONTROL_PAUSE);
	}

void CInstanceListView::OnServiceRestart()
	{
	ControlServices(0x80000001);
	}

/*
  dwControl = SERVICE_CONTROL_STOP, PAUSE, CONTINUE.
  Use dwControl = 0x80000000 to start.
  Use dwControl = 0x80000001 to restart;
*/
void CInstanceListView::ControlServices(DWORD dwControl)
	{
	TRACE("CInstanceListView::ControlServices(%u):\n", dwControl);

	// If starting or restarting, save config
	if ((dwControl == 0x80000000 || dwControl == 0x80000001) && GetDocument()->IsModified())
		{
		TRACE("CInstanceListView::ControlServices: saving config\n");
		GetDocument()->Save();
		}

	CListCtrl& list = GetListCtrl();

    SC_HANDLE schSCManager = 0;
    SC_HANDLE schService = 0;
	SERVICE_STATUS ssStatus = {0};

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager)
		{
		CString s;
		s.Format("OpenSCManager failed: %s", GetLastErrorText());
		DisplayError(s);
		}
	else
		{
		POSITION pos = list.GetFirstSelectedItemPosition();
		while (pos)
			{
			int nItem = list.GetNextSelectedItem(pos);
			CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItem));
			//string strServiceName = GetServiceName(GetInstanceFromItem(nItem));

			schService = ::OpenService(schSCManager, ip.strServiceName.c_str(),
				SERVICE_ALL_ACCESS);
			if (!schService)
				{
				CString s;
				s.Format("OpenService failed on service \"%s\": %s", 
					ip.strServiceName.c_str(), GetLastErrorText());
				DisplayError(s);
				}
			else
				{
				DWORD dwInstanceControl = dwControl;	// control for current instance

				if (dwControl == 0x80000001)			// restart
					{
					if (ip.ssStatus.dwCurrentState == SERVICE_STOPPED ||
						ip.ssStatus.dwCurrentState == 0)
						dwInstanceControl = 0x80000000;				// already stopped, start now
					else
						{
						dwInstanceControl = SERVICE_CONTROL_STOP;	// stop service, restart in GetServiceStatus()
						ip.flRestart = TRUE;
						}
					}

				if (dwInstanceControl == 0x80000000)		// start
					{
					if (!::StartService(schService, 0, NULL))
						{
						if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING)
							{
							CString s;				
							s.Format("StartService failed on service \"%s\": %s", 
								ip.strServiceName.c_str(), GetLastErrorText());
							DisplayError(s);
							}
						}
					else
						{
						ip.flJustStarted = TRUE;
						}
					}
				else
					{
					BOOL flAccepted = FALSE;	// true if control is accepted by service
					switch (dwInstanceControl)
						{
						case SERVICE_CONTROL_STOP:
							flAccepted = ip.ssStatus.dwControlsAccepted & SERVICE_ACCEPT_STOP;
							break;
						case SERVICE_CONTROL_PAUSE:
						case SERVICE_CONTROL_CONTINUE:
							flAccepted = ip.ssStatus.dwControlsAccepted & SERVICE_ACCEPT_PAUSE_CONTINUE;
							break;
						}

					TRACE("CInstanceListView::ControlServices: flAccepted=%u, cmd=%u\n", flAccepted,
						dwInstanceControl);

					if (flAccepted && !::ControlService(schService, dwInstanceControl, &ssStatus))
						{
						CString s;				
						s.Format("ControlService failed on service \"%s\": %s", 
							ip.strServiceName.c_str(), GetLastErrorText());
						DisplayError(s);
						}
					}

				CloseServiceHandle(schService);
				schService = 0;
				}
			}
		CloseServiceHandle(schSCManager);
		schSCManager = 0;
		}
	}


void CInstanceListView::OnUpdateEditProperties(CCmdUI* pCmdUI) 
	{
	pCmdUI->Enable(GetSingleSelection() != -1);	
	}

void CInstanceListView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
	{
	pCmdUI->Enable(GetSingleSelection() != -1);	
	}

void CInstanceListView::OnUpdateEditDuplicate(CCmdUI* pCmdUI) 
	{
	pCmdUI->Enable(GetSingleSelection() != -1);	
	}

void CInstanceListView::OnUpdateServiceStartAutomatically(CCmdUI* pCmdUI) 
	{
	TRACE("CInstanceListView::OnUpdateServiceStartautomatically\n");
	
	int nItemSel = GetSingleSelection();
	if (nItemSel == -1)
		pCmdUI->Enable(FALSE);
	else
		{
		CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItemSel));
		m_flLastStartAutomatically = ip.flStartAutomatically;
		pCmdUI->SetCheck(m_flLastStartAutomatically ? 1 : 0);
		}	
	}

void CInstanceListView::OnUpdateServiceStart(CCmdUI* pCmdUI) 
	{
	BOOL flMultipleSelection = 0;
	int nItemSel = GetSingleSelection(&flMultipleSelection);
	if (nItemSel == -1)
		pCmdUI->Enable(FALSE);
	else if (!flMultipleSelection)
		{
		CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItemSel));
		pCmdUI->Enable(
			ip.ssStatus.dwCurrentState == 0 ||
			ip.ssStatus.dwCurrentState == SERVICE_STOPPED);
		}	
	}

void CInstanceListView::OnUpdateServiceStop(CCmdUI* pCmdUI) 
	{
	BOOL flMultipleSelection = 0;
	int nItemSel = GetSingleSelection(&flMultipleSelection);
	if (nItemSel == -1)
		pCmdUI->Enable(FALSE);
	else if (!flMultipleSelection)
		{
		CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItemSel));
		pCmdUI->Enable(ip.ssStatus.dwControlsAccepted & SERVICE_ACCEPT_STOP);
		}	
	}

void CInstanceListView::OnUpdateServiceRestart(CCmdUI* pCmdUI) 
	{
	BOOL flMultipleSelection = 0;
	int nItemSel = GetSingleSelection(&flMultipleSelection);
	if (nItemSel == -1)
		pCmdUI->Enable(FALSE);
	else if (!flMultipleSelection)
		{
		CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItemSel));
		pCmdUI->Enable(
			ip.ssStatus.dwCurrentState == 0 ||
			ip.ssStatus.dwCurrentState == SERVICE_STOPPED ||
			(ip.ssStatus.dwControlsAccepted & SERVICE_ACCEPT_STOP));
		}	
	}

void CInstanceListView::OnUpdateServicePause(CCmdUI* pCmdUI) 
	{
	BOOL flMultipleSelection = 0;
	int nItemSel = GetSingleSelection(&flMultipleSelection);
	if (nItemSel == -1)
		pCmdUI->Enable(FALSE);
	else if (!flMultipleSelection)
		{
		CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItemSel));
		pCmdUI->Enable((ip.ssStatus.dwControlsAccepted & SERVICE_ACCEPT_PAUSE_CONTINUE) &&
			ip.ssStatus.dwCurrentState == SERVICE_RUNNING);
		}	
	}

void CInstanceListView::OnUpdateServiceContinue(CCmdUI* pCmdUI) 
	{
	BOOL flMultipleSelection = 0;
	int nItemSel = GetSingleSelection(&flMultipleSelection);
	if (nItemSel == -1)
		pCmdUI->Enable(FALSE);
	else if (!flMultipleSelection)
		{
		CInstanceParameters &ip = GetInstanceParameters(GetInstanceFromItem(nItemSel));
		pCmdUI->Enable((ip.ssStatus.dwControlsAccepted & SERVICE_ACCEPT_PAUSE_CONTINUE) &&
			ip.ssStatus.dwCurrentState == SERVICE_PAUSED);
		}	
	}
