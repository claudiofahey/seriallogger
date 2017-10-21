/*****************************************************************************
 MainFrm.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "smconfig.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
	{
	cs.style &= ~FWS_ADDTOTITLE;		// don't prefix title with "Untitled - "
	
	return CFrameWnd::PreCreateWindow(cs);
	}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	UINT indicators[] = {ID_SEPARATOR};
	
	// TODO: Add your specialized creation code here
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
		{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
		}
	
	return 0;
	}
