/*****************************************************************************
 InstanceListView.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_INSTANCELISTVIEW_H__A22E78A6_94CD_465F_82AC_E9613D779F5D__INCLUDED_)
#define AFX_INSTANCELISTVIEW_H__A22E78A6_94CD_465F_82AC_E9613D779F5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstanceListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInstanceListView view

class CInstanceListView : public CListView
{
protected:
	CInstanceListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInstanceListView)

// Attributes
public:
	//int m_nInstanceCurrent;		// zero-based index to selected instance
	int m_flLastStartAutomatically;	// true if last menu showed Start Automatically checked

// Operations
public:
	void UpdateItems();
	BOOL EditInstance(unsigned nInstance);
	void DoInstanceListContextMenu(const int iMenuType, const CPoint& point);
	int ForceSingleSelection(const BOOL flQuiet = FALSE);
	int GetSingleSelection(BOOL *flMultipleSelection = NULL);
	unsigned GetInstanceFromItem(int nItem);
	void OnEditProperties(const BOOL flQuiet);
	void OnEditProperties();
	void OnEditDuplicate();
	void OnEditSelectAll();
	void OnEditDelete();
	void OnEditNew();
	void OnSave();
	void OnServiceStart();
	void OnServiceStop();
	void OnServiceContinue();
	void OnServicePause();
	void OnServiceRestart();
	void OnServiceStartAutomatically();
	void ControlServices(DWORD dwControl);

	inline CInstanceParameters &GetInstanceParameters(unsigned nInstance)
		{return GetDocument()->mapIp[nInstance];}
	inline CInstanceListDoc *GetDocument()
		{return (CInstanceListDoc*) CListView::GetDocument();}
	inline int DisplayError(LPCTSTR lpszText)
		{return ((CSMConfigApp*)AfxGetApp())->DisplayError(lpszText);};
	inline void SetStatusText(LPCTSTR lpszText)
		{((CSMConfigApp*)AfxGetApp())->SetStatusText(lpszText);};


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstanceListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInstanceListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CInstanceListView)
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateServiceStartAutomatically(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateServiceStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServiceStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServicePause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServiceContinue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServiceRestart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditProperties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDuplicate(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTANCELISTVIEW_H__A22E78A6_94CD_465F_82AC_E9613D779F5D__INCLUDED_)
