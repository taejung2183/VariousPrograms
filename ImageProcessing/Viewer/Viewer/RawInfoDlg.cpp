// RawInfoDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "RawInfoDlg.h"
#include "afxdialogex.h"


// CRawInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(CRawInfoDlg, CDialogEx)

CRawInfoDlg::CRawInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RAWINFODIALOG, pParent)
	, m_radio_rawFileInfo(0)
{
	nFormat = 3;
}

CRawInfoDlg::~CRawInfoDlg()
{
}

void CRawInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_GRAYFORMAT, m_radio_rawFileInfo);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_EditHeight);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_EditWidth);
}


BEGIN_MESSAGE_MAP(CRawInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRawInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRawInfoDlg::OnBnClickedCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_GRAYFORMAT, IDC_RADIO_YUV420FORMAT, OnClickedRadio)
END_MESSAGE_MAP()



// CRawInfoDlg 메시지 처리기


void CRawInfoDlg::OnBnClickedOk()
{
	CString cTemp1, cTemp2;

	m_EditHeight.GetWindowTextA(cTemp1);
	m_EditWidth.GetWindowTextA(cTemp2);

	nHeight = atoi(cTemp1);
	nWidth = atoi(cTemp2);

	CDialogEx::OnOK();
}


void CRawInfoDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CRawInfoDlg::OnClickedRadio(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_rawFileInfo) {
	case 0://Gray
		nFormat = 3;
		break;
	case 1://YUV444
		nFormat = 1;
		break;
	case 2://YUV420
		nFormat = 2;
		break;
	}
}

int CRawInfoDlg::GetRawHeight() {
	return nHeight;
}

int CRawInfoDlg::GetRawWidth() {
	return nWidth;
}

int CRawInfoDlg::GetRawFormat() {
	return nFormat;
}
