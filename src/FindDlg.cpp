// FindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mboxview.h"
#include "FindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindDlg dialog


CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
	, m_startDate(COleDateTime::GetCurrentTime())
	, m_endDate(COleDateTime::GetCurrentTime())
	, m_scope(FALSE)
	, m_filterDates(FALSE)
{
	//{{AFX_DATA_INIT(CFindDlg)
	m_string = _T("");
	m_bWholeWord = FALSE;
	m_bCaseSensitive = TRUE;
	//}}AFX_DATA_INIT
}


void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDlg)
	DDX_Text(pDX, IDC_STRING, m_string);
	DDX_Check(pDX, IDC_WHOLE, m_bWholeWord);
	DDX_Check(pDX, IDC_CASE, m_bCaseSensitive);
	//}}AFX_DATA_MAP
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_startDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_endDate);
	DDX_Radio(pDX, IDC_SCOPE, m_scope);
	DDX_Check(pDX, IDC_CHECK1, m_filterDates);
	if (pDX->m_bSaveAndValidate) {
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(m_filterDates);
		GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(m_filterDates);
	}
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	//{{AFX_MSG_MAP(CFindDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, &CFindDlg::OnBnClickedFilterDates)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDlg message handlers

void CFindDlg::OnOK()
{
	UpdateData();
	m_endDate.SetDate(m_endDate.GetYear(), m_endDate.GetMonth(), m_endDate.GetDay());
	m_startDate.SetDate(m_startDate.GetYear(), m_startDate.GetMonth(), m_startDate.GetDay());
	m_string.TrimRight();
	g_tu.MakeLower(m_string);
	if( m_string.IsEmpty() ) {
		AfxMessageBox("Cannot search for an empty string!", MB_OK | MB_ICONHAND);
		return;
	}
/*
	g_lastCase = m_bCaseSensitive;
	g_lastScope = m_scope;
	g_lastStartDate = m_startDate;
	g_lastEndDate = m_endDate;
*/	
	CDialog::OnOK();
}


BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CFindDlg::OnBnClickedFilterDates()
{
	UpdateData(TRUE);
}