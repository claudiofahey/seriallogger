#if !defined(AFX_INSTANCELISTDOC_H__60B1B3A9_2A9C_4E8C_BEF4_E4275F70E0EC__INCLUDED_)
#define AFX_INSTANCELISTDOC_H__60B1B3A9_2A9C_4E8C_BEF4_E4275F70E0EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstanceListDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc document

class CInstanceListDoc : public CDocument
{
protected:
	CInstanceListDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInstanceListDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstanceListDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInstanceListDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CInstanceListDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTANCELISTDOC_H__60B1B3A9_2A9C_4E8C_BEF4_E4275F70E0EC__INCLUDED_)
