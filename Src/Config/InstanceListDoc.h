/*****************************************************************************
 InstanceListDoc.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_INSTANCEDOC_H__048D5AE9_C5E7_4167_8C87_0B68465457A2__INCLUDED_)
#define AFX_INSTANCEDOC_H__048D5AE9_C5E7_4167_8C87_0B68465457A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstanceListDoc.h : header file
//

struct CEnumServiceStatus
	{
	std::string strServiceName;
	std::string strDisplayName;
	SERVICE_STATUS ServiceStatus;
	CEnumServiceStatus() {};
	CEnumServiceStatus(LPTSTR _strServiceName, LPTSTR _strDisplayName, SERVICE_STATUS _ServiceStatus) :
		strServiceName(_strServiceName),
		strDisplayName(_strDisplayName),
		ServiceStatus(_ServiceStatus) {};
	};

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc document

class CInstanceListDoc : public CDocument
{
protected:
	CInstanceListDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInstanceListDoc)

// Attributes
public:
	CCommonParameters cp;
	std::map<unsigned, CInstanceParameters> mapIp;

	// Operations
public:
	BOOL Save();
	inline int DisplayError(LPCTSTR lpszText)
		{return ((CSMConfigApp*)AfxGetApp())->DisplayError(lpszText);};
	inline void SetStatusText(LPCTSTR lpszText)
		{((CSMConfigApp*)AfxGetApp())->SetStatusText(lpszText);};
	BOOL RegSetString(HKEY hkey, LPCTSTR szValueName, const string &s);
	BOOL RegSetInt(HKEY hkey, LPCTSTR szValueName, DWORD dw);
	void GetServiceConfig();
	void GetServiceStatus();
	unsigned GetNextAvailableInstanceNumber();
	BOOL EnumServices(std::map<unsigned, CEnumServiceStatus> &mapEss);
	BOOL RemoveService(const CEnumServiceStatus &Ess, SC_HANDLE schSCManager);
	void SetDefaults(unsigned nInstance);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstanceListDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
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
	afx_msg void OnAppAbout();
	afx_msg void OnRegister();
	afx_msg void OnEventViewer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTANCEDOC_H__048D5AE9_C5E7_4167_8C87_0B68465457A2__INCLUDED_)
