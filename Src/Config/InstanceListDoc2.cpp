// InstanceListDoc.cpp : implementation file
//

#include "stdafx.h"
#include "smconfig.h"
#include "InstanceListDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc

IMPLEMENT_DYNCREATE(CInstanceListDoc, CDocument)

CInstanceListDoc::CInstanceListDoc()
{
}

BOOL CInstanceListDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CInstanceListDoc::~CInstanceListDoc()
{
}


BEGIN_MESSAGE_MAP(CInstanceListDoc, CDocument)
	//{{AFX_MSG_MAP(CInstanceListDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc diagnostics

#ifdef _DEBUG
void CInstanceListDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInstanceListDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc serialization

void CInstanceListDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc commands
