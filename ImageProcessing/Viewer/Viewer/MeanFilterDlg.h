#pragma once


// CMeanFilterDlg 대화 상자

class CMeanFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeanFilterDlg)
private:
	int m_radio_mf;
	
	int nFilterSize;
	int nMeanFilterType;
	int Q;

	CEdit Edit_FilterSize;
	CEdit Edit_Q;
public:
	CMeanFilterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMeanFilterDlg();


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEANFILTERDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioClicked(UINT msg);

	int GetFilterSize();
	int GetMeanFilterType();
	int GetQ();
};
