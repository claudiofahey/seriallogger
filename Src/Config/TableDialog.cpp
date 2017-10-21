/*****************************************************************************
 TableDialog.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "smconfig.h"
#include "catsets.h"
#include "TableDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableDialog dialog

CTableDialog::CTableDialog(CWnd* pParent /*=NULL*/):
	CDialog(CTableDialog::IDD, pParent),
	m_strTable(""),
	m_strColumn(""),
	m_dwColumnSize(0),
	flSelectingColumn(0)
	{
	//{{AFX_DATA_INIT(CTableDialog)
	//}}AFX_DATA_INIT
	}

void CTableDialog::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableDialog)
	DDX_Control(pDX, IDC_LIST, m_ctlList);
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CTableDialog, CDialog)
	//{{AFX_MSG_MAP(CTableDialog)
	ON_LBN_DBLCLK(IDC_LIST, OnDblClkList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableDialog message handlers

BOOL CTableDialog::OnInitDialog() 
	{
	CDialog::OnInitDialog();

	try
		{
		AfxGetApp()->BeginWaitCursor();	
		db.OpenEx(m_strSQLConnectionString);	
		AfxGetApp()->EndWaitCursor();
		}
	catch (CDBException *e)
		{
		((CSMConfigApp*)AfxGetApp())->DisplayError(
			(const char*) (CString("An ODBC error has occured:\r\n") + e->m_strError + "" +
			e->m_strStateNativeOrigin));
		EndDialog(IDCANCEL);
		return TRUE;
		}

	PrepareDialogData();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

void CTableDialog::OnDestroy() 
	{
	CDialog::OnDestroy();
	
	try
		{
		db.Close();
		}
	catch (CDBException *e)
		{
		e;	// just to avoid compile warning
		}
	}

BOOL CTableDialog::PrepareDialogData()
	{
	SetWindowText(flSelectingColumn ? "Select a Column" : "Select a Table");

	AfxGetApp()->BeginWaitCursor();	

	m_ctlList.ResetContent();
	
	try
		{
		if (flSelectingColumn)
			{
			CColumns cols(&db);
			cols.Open(NULL, NULL, m_strTable);
			while (!cols.IsEOF())
				{
				TRACE("CTableDialog::PrepareDialogData: col=%s, size=%d\n", cols.m_strColumnName,
					cols.m_nPrecision);
				switch (cols.m_nDataType)
					{
					case SQL_CHAR:
					case SQL_VARCHAR:
					case SQL_LONGVARCHAR:
					case SQL_WCHAR:
					case SQL_WVARCHAR:
					case SQL_WLONGVARCHAR:
						int nIndex = m_ctlList.AddString(cols.m_strColumnName);
						m_ctlList.SetItemData(nIndex, cols.m_nPrecision);
						break;
					}
				cols.MoveNext();
				}
			}
		else
			{
			CTables tables(&db);
			tables.Open();
			while (!tables.IsEOF())
				{
				TRACE("CTableDialog::OnInitDialog: table=%s\n", tables.m_strTableName);
				m_ctlList.AddString(tables.m_strTableName);
				tables.MoveNext();
				}
			}
		}
	catch (CDBException *e)
		{
		AfxGetApp()->EndWaitCursor();
		((CSMConfigApp*)AfxGetApp())->DisplayError(
			(const char*) (CString("An ODBC error has occured:\r\n") + e->m_strError + "" +
			e->m_strStateNativeOrigin));
		EndDialog(IDCANCEL);
		return TRUE;
		}

	AfxGetApp()->EndWaitCursor();
	return TRUE;
	}

void CTableDialog::OnDblClkList() 
	{
	OnOK();
	}

void CTableDialog::OnOK() 
	{
	int nIndex = 0;
	TRACE("CTableDialog::OnOK: flSelectingColumn=%d\n", flSelectingColumn);

	nIndex = m_ctlList.GetCurSel();
	if (nIndex == LB_ERR)
		{
		MessageBox("No item selected.", NULL, MB_ICONERROR | MB_OK);
		return;
		}
	
	if (flSelectingColumn)
		{
		// column is being selected
		m_ctlList.GetText(nIndex, m_strColumn);
		m_dwColumnSize = m_ctlList.GetItemData(nIndex);

		CDialog::OnOK();
		}
	else
		{
		// table is being selected
		m_ctlList.GetText(nIndex, m_strTable);

		flSelectingColumn = TRUE;
		PrepareDialogData();
		}	
	}

