/*****************************************************************************
 OdbcPage.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "smconfig.h"
#include "OdbcPage.h"
#include "TableDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COdbcPage property page

IMPLEMENT_DYNCREATE(COdbcPage, CPropertyPage)

COdbcPage::COdbcPage() : CPropertyPage(COdbcPage::IDD)
	{
	//{{AFX_DATA_INIT(COdbcPage)
	m_strSQLWriteLineStatement = _T("");
	m_dwMaxLineLength = 0;
	m_strSQLSafeConnectionString = _T("");
	//}}AFX_DATA_INIT
	}

COdbcPage::~COdbcPage()
	{
	}

void COdbcPage::DoDataExchange(CDataExchange* pDX)
	{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COdbcPage)
	DDX_Control(pDX, IDC_MAXLINELENGTH, m_ctlMaxLineLength);
	DDX_Control(pDX, IDC_SQLSTATEMENT, m_ctlSQLWriteLineStatement);
	DDX_Control(pDX, IDC_CONNECTSTRING, m_ctlSQLSafeConnectionString);
	DDX_Text(pDX, IDC_SQLSTATEMENT, m_strSQLWriteLineStatement);
	DDX_Text(pDX, IDC_MAXLINELENGTH, m_dwMaxLineLength);
	DDX_Text(pDX, IDC_CONNECTSTRING, m_strSQLSafeConnectionString);
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(COdbcPage, CPropertyPage)
	//{{AFX_MSG_MAP(COdbcPage)
	ON_BN_CLICKED(IDC_BROWSEDSN, OnBrowseDsn)
	ON_EN_CHANGE(IDC_CONNECTSTRING, OnChangeConnectString)
	ON_BN_CLICKED(IDC_BUILDSQLSTATEMENT, OnBuildSqlStatement)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcPage message handlers

void COdbcPage::OnBrowseDsn() 
	{
	//BOOL flRc = 0, flSuccess = 0, iRc = 0;
	//char szSql[1024] = {0};
	SQLRETURN sr = 0;
	SQLHENV henv = 0;				// ODBC environment handle
	SQLHDBC hdbc = 0;				// ODBC connection handle

	//CloseOdbc();

	try
		{	
		// Allocate environment handle
		sr = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
		AssertSQLEnv(sr, henv);

		// Set the ODBC version environment attribute
		sr = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0); 
		AssertSQLEnv(sr, henv);

		// Allocate connection handle
		sr = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); 
		AssertSQLEnv(sr, henv);

		// Open ODBC DSN dialog box and allow user to select a DSN
		const SQLSMALLINT MaxConnectionStringBufSize = 4096;
		SQLSMALLINT ConnectionStringBufSize = 0;
		SQLCHAR ConnectionString[MaxConnectionStringBufSize] = {0};
		sr = SQLDriverConnect(hdbc, m_hWnd, (SQLCHAR*) "DSN=;", SQL_NTS, ConnectionString, MaxConnectionStringBufSize,
			&ConnectionStringBufSize, SQL_DRIVER_PROMPT);
		AssertSQLDBC(sr, hdbc);

		TRACE("COdbcPage.OnBrowseDsn: ConnectionString=%s\n", ConnectionString);

		if (ConnectionString[0])
			{
			m_strSQLConnectionString = ConnectionString;
			m_ctlSQLSafeConnectionString.SetWindowText(GetSafeConnectionString(
				string((const char*) ConnectionString)).c_str());
			}

		// Close ODBC handles
		if (hdbc)
			{
			SQLDisconnect(hdbc);
			SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			hdbc = 0;
			}
		if (henv)
			{
			SQLFreeHandle(SQL_HANDLE_ENV, henv);
			henv = 0;
			}		
		}
	catch (COdbcException e)
		{
		((CSMConfigApp*)AfxGetApp())->DisplayError(string("An ODBC error has occured:\r\n" + 
			GetSQLErrorText(e.HandleType, e.h)).c_str());
		}
	}

void COdbcPage::OnChangeConnectString() 
	{
	TRACE("COdbcPage::OnChangeConnectString\n");
	m_ctlSQLSafeConnectionString.GetWindowText(m_strSQLConnectionString);
	}

void COdbcPage::OnBuildSqlStatement() 
	{
	TRACE("COdbcPage::OnBuildSqlStatement\n");

	CTableDialog td(this);
	td.m_strSQLConnectionString = m_strSQLConnectionString;

	int iRc;
	iRc = td.DoModal();

	if (iRc == IDOK)
		{
		TRACE("COdbcPage::OnBuildSqlStatement: selected %s.%s\n", td.m_strTable, td.m_strColumn);

		// Construct write line statement
		string s = string("INSERT INTO ") + GetSQLIdentifier(string((const char*) td.m_strTable)) +
			string(" (") + GetSQLIdentifier(string((const char*)td.m_strColumn)) + string(") VALUES (?)");
		m_ctlSQLWriteLineStatement.SetWindowText(s.c_str());

		// Set default value for line length
		const DWORD dwMax = 1024;	// by default, limit line length to 1024
		DWORD dw = td.m_dwColumnSize > dwMax ? dwMax : td.m_dwColumnSize;
		stringstream ss;
		ss << dw;
		m_ctlMaxLineLength.SetWindowText(ss.str().c_str());
		}
	}

void COdbcPage::OnTest() 
	{
	//BOOL flRc = 0, flSuccess = 0, iRc = 0;
	//char szSql[1024] = {0};
	SQLRETURN sr = 0, sr1 = 0;
	SQLHENV henv = 0;				// ODBC environment handle
	SQLHDBC hdbc = 0;				// ODBC connection handle
	SQLHSTMT hstmtWriteLine = 0;	// ODBC statement handle
	long cbLine = 0;
	string s("This is a test.");		

	UpdateData();

	AfxGetApp()->BeginWaitCursor();	

	CInstanceParameters ipTemp, *ip = &ipTemp;
	ipTemp.strSQLConnectionString = m_strSQLConnectionString;
	ipTemp.strSQLWriteLineStatement = (const char*) m_strSQLWriteLineStatement;
	ipTemp.dwMaxProcessedLineLength = m_dwMaxLineLength;

	try
		{	
		/////////////////////////////////////////////////////////////////////////
		// Open ODBC connection
		/////////////////////////////////////////////////////////////////////////

		// Allocate environment handle
		sr = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
		AssertSQLEnv(sr, henv);

		// Set the ODBC version environment attribute
		sr = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0); 
		AssertSQLEnv(sr, henv);

		// Allocate connection handle
		sr = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); 
		AssertSQLEnv(sr, henv);

		// Set login timeout to 5 seconds.
		sr = SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER) 5, 0);
		AssertSQLDBC(sr, hdbc);

		// Connect to data source
		const SQLSMALLINT MaxConnectionStringBufSize = 4096;
		SQLSMALLINT ConnectionStringBufSize = 0;
		SQLCHAR ConnectionString[MaxConnectionStringBufSize] = {0};
		sr = SQLDriverConnect(hdbc, NULL, (SQLCHAR*) ip->strSQLConnectionString.c_str(), SQL_NTS, 
			ConnectionString, MaxConnectionStringBufSize,
			&ConnectionStringBufSize, SQL_DRIVER_NOPROMPT);
		AssertSQLDBC(sr, hdbc);

		// Allocate statement handle
		sr = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmtWriteLine);
		AssertSQLDBC(sr, hdbc);

		// Prepare update statements for transaction table
		sr = SQLPrepare(hstmtWriteLine, (SQLCHAR*) ip->strSQLWriteLineStatement.c_str(), SQL_NTS);
		AssertSQLStmt(sr, hstmtWriteLine);

		// Bind parameters for update statements
		sr = SQLBindParameter(hstmtWriteLine, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR,
			ip->dwMaxProcessedLineLength, 0, (SQLPOINTER) 1, 0, &cbLine);
		AssertSQLStmt(sr, hstmtWriteLine);

		/////////////////////////////////////////////////////////////////////////
		// Execute query
		/////////////////////////////////////////////////////////////////////////

		SQLPOINTER pToken = 0;
		SQLINTEGER cbData = (s.size() > ip->dwMaxProcessedLineLength) ? ip->dwMaxProcessedLineLength : s.size();
		cbLine = SQL_LEN_DATA_AT_EXEC(cbData);

		// Execute statement		
		sr1 = SQLExecute(hstmtWriteLine);
		AssertSQLStmt(sr1, hstmtWriteLine);

		// Pass parameter
		while (sr1 == SQL_NEED_DATA)
			{
			sr1 = SQLParamData(hstmtWriteLine, &pToken);
			AssertSQLStmt(sr1, hstmtWriteLine);
			if (sr1 == SQL_NEED_DATA)
				{
				sr = SQLPutData(hstmtWriteLine, (SQLPOINTER) s.data(), cbData);
				AssertSQLStmt(sr, hstmtWriteLine);
				}
			}

		// Close results cursor
		sr1 = SQLFreeStmt(hstmtWriteLine, SQL_CLOSE);
		AssertSQLStmt(sr1, hstmtWriteLine);

		/////////////////////////////////////////////////////////////////////////
		// Close ODBC handles
		/////////////////////////////////////////////////////////////////////////

		if (hstmtWriteLine)
			{
			SQLFreeHandle(SQL_HANDLE_STMT, hstmtWriteLine);
			hstmtWriteLine = 0;
			}
		if (hdbc)
			{
			SQLDisconnect(hdbc);
			SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			hdbc = 0;
			}
		if (henv)
			{
			SQLFreeHandle(SQL_HANDLE_ENV, henv);
			henv = 0;
			}
		
		}
	catch (COdbcException e)
		{
		AfxGetApp()->EndWaitCursor();
		((CSMConfigApp*)AfxGetApp())->DisplayError(string("An ODBC error has occured:\r\n" + 
			GetSQLErrorText(e.HandleType, e.h)).c_str());
		return;
		}

	AfxGetApp()->EndWaitCursor();
	AfxMessageBox(string(string("Successfully executed the SQL statement using the test string \"") +
		s + string("\".")).c_str(), MB_OK | MB_ICONINFORMATION);
	}
