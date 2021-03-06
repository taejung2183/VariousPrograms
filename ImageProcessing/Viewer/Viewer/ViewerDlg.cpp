
// ViewerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "RawInfoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CViewerDlg 대화 상자

CViewerDlg::CViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_IN, m_PicOri);
	DDX_Control(pDX, IDC_PICTURE_OUT, m_PicProc);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_FILEOPEN, &CViewerDlg::OnFileOpenBtnClick)
	ON_COMMAND(ID_FILEOPEN_FILECLOSE, &CViewerDlg::OnFileCloseBtnClick)
	ON_COMMAND(ID_EDIT_CONVERT, &CViewerDlg::OnConvertGrayBtnClick)
	ON_COMMAND(ID_HISTOGRAM_GETHISTOGRAM, &CViewerDlg::OnHistogramGetClick)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMEQUALIZATION, &CViewerDlg::OnHistogramEqualizationClicked)
	ON_COMMAND(ID_EDIT_SCALING, &CViewerDlg::OnEditScalingClick)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMMATCHING, &CViewerDlg::OnHistogramMatchingClicked)
	ON_STN_CLICKED(IDC_PICTURE_OUT, &CViewerDlg::OnStnClickedPictureOut)
	ON_COMMAND(ID_EDIT_ROTATION, &CViewerDlg::OnEditRotationClick)
	ON_COMMAND(ID_EDIT_FILTER, &CViewerDlg::OnEditFilterClicked)
	ON_COMMAND(ID_NOISEFILTERING_MEANFILTER, &CViewerDlg::OnNoisefilteringMeanfilter)
	ON_COMMAND(ID_NOISEFILTERING_ORDER, &CViewerDlg::OnNoisefilteringOrder)
	ON_COMMAND(ID_NOISEREDUCTION_GETPSNR, &CViewerDlg::OnGetpsnr)
	ON_COMMAND(ID_NOISEREDUCTION_ADDNOISE, &CViewerDlg::OnNoisereductionAddnoiseClicked)
END_MESSAGE_MAP()

// CViewerDlg 메시지 처리기

BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CViewerDlg::DisplayImage(Mat DispMat, bool bInOut) {
	if (bInOut) {
		GetDlgItem(IDC_PICTURE_IN)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicOri.GetDC();
		m_PicOri.GetClientRect(&rect);
	}
	else {
		GetDlgItem(IDC_PICTURE_OUT)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicProc.GetDC();
		m_PicProc.GetClientRect(&rect);
	}

	IplImage* Ori = &IplImage(DispMat);
	IplImage* Resize = NULL;

	ResizeImage(Ori, &Resize, &rect);
	DisplayBitmap(m_pDC, rect, Resize);
}

void CViewerDlg::DisplayBitmap(CDC* pDC, CRect rect, IplImage * DispIplImage) {
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = DispIplImage->width;
	bitmapInfo.bmiHeader.biHeight = DispIplImage->height;

	if (DispIplImage->nChannels == 3) {
		IplImage* ReverseImg = cvCreateImage(CvSize(DispIplImage->width, DispIplImage->height), 8, 3);

		for (int h = 0; h < DispIplImage->height; h++) {
			for (int w = 0; w < DispIplImage->width; w++) {
				ReverseImg->imageData[h * DispIplImage->widthStep + w * 3] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w * 3];
				ReverseImg->imageData[h * DispIplImage->widthStep + w * 3 + 1] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w * 3 + 1];
				ReverseImg->imageData[h * DispIplImage->widthStep + w * 3 + 2] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w * 3 + 2];

			}
		}

		bitmapInfo.bmiHeader.biBitCount = DispIplImage->depth * DispIplImage->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, DispIplImage->width, DispIplImage->height, ReverseImg->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
	}
	else {
		IplImage* ReverseImg = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 1);

		for (int h = 0; h < DispIplImage->height; h++) {
			for (int w = 0; w < DispIplImage->width; w++) {
				ReverseImg->imageData[h * DispIplImage->widthStep + w] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w];
			}
		}

		IplImage * tmp = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 3);
		cvCvtColor(ReverseImg, tmp, CV_GRAY2BGR);

		bitmapInfo.bmiHeader.biBitCount = tmp->depth * tmp->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
		cvReleaseImage(&tmp);

	}
}

void CViewerDlg::ResizeImage(IplImage * src, IplImage ** dst, CRect * rect) {
	int ndst_width = 0, ndst_height = 0;

	//먼저, 영상의 가로와 세로 중 장축을 구한다.
	if (src->width > src->height) {
		ndst_width = rect->Width();
		ndst_height = (src->height * rect->Width()) / src->width;
	}
	else {
		ndst_width = (src->width * rect->Height()) / src->height;
		ndst_height = rect->Height();
	}

	(*dst) = cvCreateImage(cvSize(ndst_width, ndst_height), IPL_DEPTH_8U, src->nChannels);

	cvResize(src, (*dst));
	rect->right = rect->left + ndst_width;
	rect->bottom = rect->top + ndst_height;
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal) {
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++) {
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

void MemFree2D(unsigned char** Mem, int nHeight) {
	for (int n = 0; n < nHeight; n++) {
		delete[] Mem[n];

	}
	delete[] Mem;
}

void CViewerDlg::OnFileOpenBtnClick()
{
	CFileDialog dlg(TRUE);

	if (isImageOpened) {
		if (ch_in_1 != NULL) {
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL) {
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}

	if (dlg.DoModal() == IDOK) {
		if (dlg.GetFileExt() != "jpg" && dlg.GetFileExt() != "JPG" && dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "yuv") {
			MessageBox("JPG 또는 YUV, Gray 파일이 아닙니다.");
			return;
		}

		InpFileName = dlg.GetFileTitle();

		if (dlg.GetFileExt() == "jpg" || dlg.GetFileExt() == "JPG") {
			Ori_Img = imread((const char*)dlg.GetPathName());
			nHeight_in = Ori_Img.rows;
			nWidth_in = Ori_Img.cols;
			if (Ori_Img.channels() == 3) {
				ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);

				for (int h = 0; h < nHeight_in; h++) {
					for (int w = 0; w < nWidth_in; w++) {
						Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
						ch_in_1[h][w] = RGB[2];
						ch_in_2[h][w] = RGB[1];
						ch_in_3[h][w] = RGB[0];

					}
				}
				nFormat = FORMAT_RGB;
			}
			else {//Gray Image(JPG)
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
				for (int h = 0; h < nHeight_in; h++) {
					for (int w = 0; w < nWidth_in; w++) {
						ch_in_gray[h][w] = Ori_Img.at<unsigned char>(h, w);
					}
				}
				nFormat = FORMAT_GRAY;
			}
			isImageOpened = true;
			DisplayImage(Ori_Img, true);
		}
		else {//Raw File & YUV File
			//RawOpenDlg를 통하여 Raw 파일의 정보를 입력받는다.
			CRawInfoDlg RawInfoDlg;
			if (RawInfoDlg.DoModal() == IDOK) {
				nHeight_in = RawInfoDlg.GetRawHeight();
				nWidth_in = RawInfoDlg.GetRawWidth();
				nFormat = RawInfoDlg.GetRawFormat();

				if (nHeight_in == 0 || nWidth_in == 0) {
					MessageBox("값을 잘못 입력하였습니다.");
					return;
				}

				FILE *fp_in;

				fopen_s(&fp_in, (const char*)dlg.GetPathName(), "rb");

				if (nFormat == FORMAT_GRAY) {
					ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_gray[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
							;
						}
					}
				}
				else if (nFormat == FORMAT_YUV444) {
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);

					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_1, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_2, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_3, sizeof(unsigned char), nWidth_in, fp_in);
					}
				}
				else {//nFormat == FORMAT_YUV420
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					ch_in_3 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);

					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_1, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in/2; h++) {
						fread(ch_in_2, sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
					for (int h = 0; h < nHeight_in/2; h++) {
						fread(ch_in_3, sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
				}
				fclose(fp_in);

				isImageOpened = true;
				DisplayImage(Ori_Img, true);
			}
		}
	}
}

void CViewerDlg::OnFileCloseBtnClick()
{
	if (isImageOpened) {
		if (ch_in_1 != NULL) {
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL) {
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

void CViewerDlg::OnConvertGrayBtnClick()
{
	if (isImageOpened) {
		if (nFormat == FORMAT_GRAY) {
			MessageBox("입력 영상이 회색조 영상입니다.");
			return;
		}
		if (nFormat == FORMAT_YUV444 || nFormat == FORMAT_YUV420) {
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = ch_in_1[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			return;
		}
		if (nFormat == FORMAT_RGB) {
			if (ch_in_gray == NULL) {
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
			}
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					ch_in_gray[h][w] = saturate_cast<unsigned char>(0.299 * ch_in_1[h][w] + 0.587 * ch_in_2[h][w] + 0.114 * ch_in_3[h][w]);
				}
			}

			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}

			DisplayImage(Proc_Img, false);
			return;
		}
	}
	else {
		MessageBox("불러온 영상이 없습니다.");
	}
}

void DisplayHistogram(int *Hist) {
	int nMax = 0;

	for (int n = 0; n < 256; n++) {
		if (nMax < Hist[n]) {
			nMax = Hist[n];
		}
	}

	double dNormalizeFactor = 255.0 / nMax;

	Mat HistDisp = Mat(256, 256, CV_8UC1);
	HistDisp = Scalar::all(0);

	for (int w = 0; w < 256; w++) {
		int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
		for (int h = 255; h > 255 - nNormalizedValue; h--) {
			HistDisp.at<unsigned char>(h, w) = 255;
		}
	}

	imshow("Histogram", HistDisp);
	waitKey(0);
}

void CViewerDlg::OnHistogramGetClick()
{
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (Hist == NULL) {
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		//Get Histogram
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}

		//Normalize Histogram & Display
		int nMax = 0;

		for (int n = 0; n < 256; n++) {
			if (nMax < Hist[n]) {
				nMax = Hist[n];
			}
		}

		double dNormalizeFactor = 255.0 / nMax;

		Mat HistDisp = Mat(256, 256, CV_8UC1);
		HistDisp = Scalar::all(0);

		for (int w = 0; w < 256; w++) {
			int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
			for (int h = 255; h > 255 - nNormalizedValue; h--) {
				HistDisp.at<unsigned char>(h, w) = 255;
			}
		}

		imshow("Histogram of Input Image", HistDisp);
		waitKey(0);
	}
}

void CViewerDlg::OnHistogramEqualizationClicked()
{
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (Hist == NULL) {
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		//Get Histogram
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}

		//Histogram Equalization
		double Hist_CDF[256] = { 0.0 };
		Mat Eq_Img = Mat(256, 256, CV_8UC1);

		//누적 분포 함수
		Hist_CDF[0] = (double)Hist[0] / (nHeight_in * nWidth_in);
		for (int n = 1; n < 256; n++) {
			Hist_CDF[n] = (double)Hist[n] / (nHeight_in * nWidth_in) + Hist_CDF[n - 1];
		
		}

		for (int n = 0; n < 256; n++) 
			for (int h = 0; h < nHeight_in; h++) 
				for (int w = 0; w < nWidth_in; w++) 
					if (ch_in_gray[h][w] == n) 
						Eq_Img.at<uchar>(h, w) = round(Hist_CDF[n] * 255);

		//Display Result
		int* Hist_Eq = new int[256];
		memset(Hist_Eq, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist_Eq[Eq_Img.at<unsigned char>(h, w)]++;
			}
		}

		DisplayImage(Eq_Img, false);
		DisplayHistogram(Hist);
		DisplayHistogram(Hist_Eq);

		delete[] Hist_Eq;
	}
}

void CViewerDlg::OnHistogramMatchingClicked()
{
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (Hist == NULL) {
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		//Get Histogram
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}

		//Histogram Matching

		//선언 & 초기화
		Mat MatchImg(256, 256, CV_8UC1), RefImg(nHeight_in, nWidth_in, CV_8UC1);
		int Hist_Ref[256] = { 0 }; int Hist_Mat[256] = { 0 };
		float Hist_CDF[256], Hist_CDF_Ref[256];

		MatchImg = Scalar::all(0);
		RefImg = Scalar::all(0);

		//Reference 영상 생성 및 히스토그램 계산
		srand(GetTickCount());
		for (int h = 0; h < RefImg.rows; h++) {
			for (int w = 0; w < RefImg.cols; w++) {
				RefImg.at<unsigned char>(h, w) = (unsigned char)rand() % 255;
				Hist_Ref[RefImg.at<unsigned char>(h, w)]++;
			}
		}

		//CDF 구하기
		Hist_CDF[0] = (double)Hist[0] / (nHeight_in * nWidth_in);
		Hist_CDF_Ref[0] = (double)Hist_Ref[0] / (nHeight_in * nWidth_in);
		for (int n = 1; n < 256; n++) {
			Hist_CDF[n] = (double)Hist[n] / (nHeight_in * nWidth_in) + Hist_CDF[n - 1];
			Hist_CDF_Ref[n] = (double)Hist_Ref[n] / (nHeight_in * nWidth_in) + Hist_CDF_Ref[n - 1];
		}
		/*
		//Histogram Matching 과정을 통하여 Matching 결과를 Mat에 저장
		for (int h = 0; h < MatchImg.rows; h++) {
			for (int w = 0; w < MatchImg.cols; w++) {
				MatchImg.at<unsigned char>(h,w) = round()
			}
		}
		*/

		DisplayImage(MatchImg, false);
		DisplayHistogram(Hist_Ref);
		DisplayHistogram(Hist_Mat);
	}
}

void CViewerDlg::OnEditScalingClick()
{
	if (isImageOpened && nFormat == FORMAT_GRAY) {//영상이 열려 있을 때 & 영상 포맷이 Gray (1 Channel)일 때만 실행
		if (ImgScalingDlg.DoModal() == IDOK) { //스케일링 정보를 받아옴 
			int nInterpolationType = ImgScalingDlg.GetInterpType();//보간타입
			double dScaleRate = ImgScalingDlg.GetScaleRate(); // 스케일링 비율

			if (dScaleRate < 1 || dScaleRate > 2) { // 스케일링 비율이 잘못되었을 경우, 범위를 벗어났을 경우 실행하지 않음
				MessageBox("스케일링 범위가 잘못되었습니다.");
				return;
			}

			//Scaling 후 영상의 크기 계산
			int nHeight_Scale = nHeight_in * dScaleRate;
			int nWidth_Scale = nWidth_in * dScaleRate;

			//스케일링 결과를 담을 2차원 unsigned char형 배열
			unsigned char **Out = MemAlloc2D(nHeight_Scale, nWidth_Scale, 0);

			for (int h = 0; h < nHeight_Scale; h++) {
				for (int w = 0; w < nWidth_Scale; w++) {
					double h_Ori = h / dScaleRate;
					double w_Ori = w / dScaleRate;
					if (nInterpolationType == BILINEAR) {
						Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
					}
					else {
						Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
					}
				}
			}

			Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			Proc_Img = Mat(nHeight_Scale, nWidth_Scale, CV_8UC1);

			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}

			for (int h = 0; h < nHeight_Scale; h++) {
				for (int w = 0; w < nWidth_Scale; w++) {
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}

			imshow("Original Image", Ori_Img);
			imshow("Scaled Image", Proc_Img);
			waitKey(0);
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

unsigned char CViewerDlg::BilinearInterpolation(unsigned char **In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt) {
	
	//  IN : 원본 영상의 픽셀 데이터
	//  nHeight_ORi, nWidth_Ori : 원본 영상의 높이, 너비
	//  h_Cvt, w_Cvt : 스케일링 한 영상의 특정 픽셀이
	//			   원본 영상에서의 비율상의 위치.
	unsigned char bilinearInterpolationResult = 0; //양선형 보간의 결과를 반환할 변수
	//좌 상단의 기준 픽셀 위치
	int h = (int)h_Cvt;
	int w = (int)w_Cvt;

	//보간의 비율을 저장할 변수
	double h_upper_length = h_Cvt - h;//위쪽 세로
	double h_lower_length = (h + 1) - h_Cvt;//아래쪽 세로
	double w_left_length = w_Cvt - w;//왼쪽 가로
	double w_right_length = (w + 1) - w_Cvt;//오른쪽 가로

	//가장자리 처리
	if (h == nHeight_Ori - 1 && w == nWidth_Ori - 1) {//오른쪽 아래 귀퉁이 
		return In[h][w];
	}
	else if (h == nHeight_Ori - 1 && w != nWidth_Ori - 1) {//우측 하단 귀퉁이를 뺀 아래쪽 가장자리
		return In[h][w];
	}
	else if (h != nHeight_Ori - 1 && w == nWidth_Ori - 1) {//우측 하단 귀퉁이를 뺀 오른쪽 가장자리
		return In[h][w];
	}
	else {//양선형 보간한 픽셀값을 반환
		bilinearInterpolationResult = h_lower_length * ((w_right_length * In[h][w]) + (w_left_length * In[h][w + 1]))
			+ h_upper_length * ((w_right_length * In[h + 1][w]) + (w_left_length * In[h + 1][w + 1]));
	}
	return bilinearInterpolationResult;
}

/*
void CViewerDlg::invertMatrix(const double m[16],double *invOut)
{
	double inv[16], det;
	int i, j;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0) {
		cout << "역행렬이 존재하지 않음" << endl;
		return;
	}

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

}
*/
/*
unsigned char CViewerDlg::CubicInterpolation(int w[], int h, double target, unsigned char **originalPixel,bool isVertical) {
	//	*************************변수 설명*****************************
	//	f(w_Cvt) = (target세제곱 target제곱 target 1) (       )-1  (f(w1))
	//					↓								↓	   	(f(w2))
	//				targetSet						cubicMatrix	(f(w3))
	//															(f(w4))
	//															   ↓
	//															pixelValue
	unsigned char pixelValue[4];//보간할 4개의 픽셀값을 저장할 변수
	unsigned char cubicInterpolationResult = 0;//cubicInterpolation의 결과를 반환할 변수
	double invMatrix[16];//cubicMatrix의 역행렬을 저장하는 변수.
	double targetSet[4] = {
		target * target * target,
		target * target,
		target,
		1
	};
	double cubicMatrix[16] = {
		w[0] * w[0] * w[0],w[0] * w[0],w[0],1,w[1] * w[1] * w[1],w[1] * w[1],w[1],1,
		w[2] * w[2] * w[2],w[2] * w[2],w[2],1,w[3] * w[3] * w[3],w[3] * w[3],w[3],1
	};
	
	if (isVertical) {//마지막 수직방향 보간인 경우
		for (int i = 0; i < 4; i++)
			pixelValue[i] = originalPixel[0][i];
	}
	else {//4번의 가로방향 보간 
		for (int i = 0; i < 4; i++)
			//pixelValue[i] = originalPixel[w[i]][h];
			pixelValue[i] = originalPixel[h][w[i]];
	}

	
	//cubicMatrix의 역행렬을 구해서 f(w_Cvt)구한 뒤 반환
	invertMatrix(cubicMatrix, invMatrix);
	cubicInterpolationResult = (targetSet[0] * invMatrix[0] + targetSet[1] * invMatrix[4] + targetSet[2] * invMatrix[8] + targetSet[3] * invMatrix[12])*pixelValue[0] +
		(targetSet[0] * invMatrix[1] + targetSet[1] * invMatrix[5] + targetSet[2] * invMatrix[9] + targetSet[3] * invMatrix[13])*pixelValue[1] +
		(targetSet[0] * invMatrix[2] + targetSet[1] * invMatrix[6] + targetSet[2] * invMatrix[10] + targetSet[3] * invMatrix[14])*pixelValue[2] +
		(targetSet[0] * invMatrix[3] + targetSet[1] * invMatrix[7] + targetSet[2] * invMatrix[11] + targetSet[3] * invMatrix[15])*pixelValue[3];
	return cubicInterpolationResult;
}
*/
/*
unsigned char CViewerDlg::BicubicInterpolation(unsigned char **In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt) {
	unsigned char **Pad;//padding된 원본 픽셀을 저장
	unsigned char bicubicInterpolationResult;//보간의 결과 저장
	unsigned char **cubicInterpolationPixel;//cubic interpolation을 통해 구한 4개의 픽셀값을 저장

	//가장자리 처리를 위한 임시 변수
	
	int tmp_h = (int)h_Cvt;
	int tmp_w = (int)w_Cvt;
	//오른쪽, 아래쪽 가장자리 => 그냥 복사하는 값
	if (h_Cvt > nHeight_Ori - 1 && w_Cvt > nWidth_Ori - 1) {//우 하단 귀퉁이
		return In[tmp_h][tmp_w];
	}
	else if (h_Cvt > nHeight_Ori - 1 && tmp_w <= nWidth_Ori - 1) {//아래쪽 가장자리
		return In[tmp_h][tmp_w];
	}
	else if (tmp_h <= nHeight_Ori - 1 && w_Cvt > nWidth_Ori - 1) {//오른쪽 가장자리
		return In[tmp_h][tmp_w];
	}
	else {
		;
	}

	//bicubic보간할 4x4픽셀의 기준이 되는 좌 상단 픽셀 좌표
	int h = (int)h_Cvt-1;
	int w = (int)w_Cvt-1;
	if (h < 0) h = 0;
	if (w < 0) w = 0;

	//가장 자리 픽셀을 보간할 때는 원본 가장자리의 값을 복사해준 뒤 보간한다.
	int width[4];// cubic interpolation을 위한 4개 픽셀의 w 좌표
	int height[4];// 마지막 수직방향 cubic interpolation을 위한 4개 픽셀의 h좌표

	for (int i = 0; i < 4; i++) {
		width[i] = w + i;
		height[i] = h + i;
	}
	cubicInterpolationPixel = new unsigned char*[4];
	cubicInterpolationPixel[0] = new unsigned char[4];//4개의 배열만 사용하므로 더 이상 할당할 필요 없음 

	Pad = Padding(In, nHeight_Ori, nWidth_Ori, 4);

	for (int i = 0; i < 4; i++) {//가로 방향 4번의 cubic interpolation
		cubicInterpolationPixel[0][i] = CubicInterpolation(width, height[i], w_Cvt, Pad, false);
	}
	
	CString msg;
	msg.Format("cubic result : %d %d %d %d\n height : %d %d %d %d\n h_Cvt : %lf", cubicInterpolationPixel[0][0], cubicInterpolationPixel[0][1], cubicInterpolationPixel[0][2], cubicInterpolationPixel[0][3],
		height[0], height[1], height[2], height[3],w_Cvt);
	MessageBox(msg);
	
	//마지막 세로방향 cubic interpolation
	bicubicInterpolationResult = CubicInterpolation(height, -1, h_Cvt,	cubicInterpolationPixel, true);
	
	msg.Format("bicubic result : %d",bicubicInterpolationResult);
	MessageBox(msg);
	
	return bicubicInterpolationResult;
}

*/


unsigned char CViewerDlg::BicubicInterpolation(unsigned char ** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt)
{

	/*
	0.5      -1 0 1 2 OutOfRange
	1.5       0 1 2 3
	253.5   252 253 254 255
	254.5   253 254 255 256 OutOfRange
	*/
	if (h_Cvt < 1 || h_Cvt >= nHeight_Ori - 2 || w_Cvt < 1 || w_Cvt >= nWidth_Ori - 2)
		return BilinearInterpolation(In, nHeight_Ori, nWidth_Ori, h_Cvt, w_Cvt);
	else {
		/*
			 n_x
		[ ] [ ] [ ] [ ]
		x1   x2   x3   x4

		f = M_a * M_b-1 * M_c
		f(n_x_1) = (pow(n_x, 3), pow(n_x, 2), n_x, 1) ((pow(x1, 3), pow(x1, 2), x1, 1)-1 (f(x_1))
										   ((pow(x2, 3), pow(x2, 2), x2, 1)   (f(x_2))
										   ((pow(x3, 3), pow(x3, 2), x3, 1)   (f(x_3))
										   ((pow(x4, 3), pow(x4, 2), x4, 1)   (f(x_4))


		*/
		/* x1, x2, x3, x4
		y1, y2, y3, y4*/
		int x[4] = { (int)w_Cvt - 1, (int)w_Cvt, (int)w_Cvt + 1, (int)w_Cvt + 2 };
		int y[4] = { (int)h_Cvt - 1, (int)h_Cvt, (int)h_Cvt + 1, (int)h_Cvt + 2 };

		Mat m_a_x = (Mat_<double>(1, 4) << pow(w_Cvt, 3), pow(w_Cvt, 2), w_Cvt, 1);
		Mat m_a_y = (Mat_<double>(1, 4) << pow(h_Cvt, 3), pow(h_Cvt, 2), h_Cvt, 1);

		Mat m_b_x = Mat_<double>(4, 4);
		Mat m_b_y = Mat_<double>(4, 4);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m_b_x.at<double>(i, j) = pow(x[i], 3 - j);
				m_b_y.at<double>(i, j) = pow(y[i], 3 - j);
			}
		}
		m_b_x = m_b_x.inv();
		m_b_y = m_b_y.inv();

		Mat m_c_y = Mat_<double>(4, 1);
		//x 4번
		for (int i = 0; i < 4; i++) {
			Mat m_c_x = (Mat_<double>(4, 1) << In[y[i]][x[0]], In[y[i]][x[1]], In[y[i]][x[2]], In[y[i]][x[3]]);
			Mat temp = (m_a_x * m_b_x * m_c_x);
			m_c_y.at<double>(i, 0) = temp.at<double>(0, 0);
		}
		//y 
		Mat temp = (m_a_y * m_b_y * m_c_y);
		return temp.at<double>(0, 0);


	}
}
void CViewerDlg::OnStnClickedPictureOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CViewerDlg::OnEditRotationClick()
{
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (RotationDlg.DoModal() == IDOK) {//회전 정보를 받아옴
			int nInterpolationType = RotationDlg.GetInterpolationType();//보간타입
			int nAngle = RotationDlg.GetAngle();//회전 각도

			if (nAngle < 0 || nAngle > 360) {//회전 각도 범위가 잘못되면 실행하지 않음
				MessageBox("회전 범위가 잘못되었습니다.");
				return;
			}

			//회전 결과를 담을 2차원 unsigned char 형 배열 
			unsigned char **Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

			int nHeight_2 = nHeight_in / 2;
			int nWidth_2 = nWidth_in / 2;

			double h_Rotate = 0;
			double w_Rotate = 0;

			//입력받은 각도를 호도법으로 계산
			double radian = PI / 180.0f;
			radian = radian * nAngle;			

			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {

					//회전한 좌표 계산
					h_Rotate = (w - nWidth_2) * sin(radian) + (h - nHeight_2) * cos(radian) + nHeight_2;
					w_Rotate = (w - nWidth_2) * cos(radian) - (h - nHeight_2) * sin(radian) + nWidth_2;

					if (isInsideBoundary(nHeight_in, nWidth_in, h_Rotate, w_Rotate)) {
						if (nInterpolationType == BILINEAR) {
							Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
						else {//Bicubic interpolation
							Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
						
					}
				}
			}
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);

			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			MemFree2D(Out, nHeight_in);
		}
	}
	else
	{
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

//해당 좌표가 영상 내부에 있으면 true, 영상 외부에 있으면 false return 
bool CViewerDlg::isInsideBoundary(int nHeight, int nWidth, double h, double w) {
	if (h >= 0 && w >= 0 && h < nHeight&&w < nWidth) {
		return true;
	}
	else return false;
}
Mat GrayToMat(unsigned char** img, int nHeight, int nWidth) {
	Mat rtn = Mat(nHeight, nWidth, CV_8UC1);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			rtn.at<unsigned char>(h, w) = img[h][w];
		}
	}
	return rtn;
}

void CViewerDlg::OnEditFilterClicked()
{
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		CFilterDlg FilterDlg;
		unsigned char** Pad, **Out;
		if (FilterDlg.DoModal() == IDOK) {
			int nFilterType = FilterDlg.getFilterType();
			int nFilterSize = 0;
			switch (nFilterType) {
			case FILTER_AVERAGE:
				nFilterSize = FilterDlg.getFilterSize();
				if (nFilterSize >= 3) {
					if (nFilterSize % 2 == 0) {
						nFilterSize++;
					}
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

					AverageFilter(Pad, Out, nHeight_in, nWidth_in, nFilterSize);
					DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

					MemFree2D(Pad, nHeight_in + (int)(nFilterSize / 2) * 2);
					MemFree2D(Out, nHeight_in);
				}
				else MessageBox("Filter size가 잘못되었습니다. Filter Size는 3이상의 정수값을 가져야 합니다");
				break;
			case FILTER_SMOOTH:
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				SmoothFilter(Pad, Out, nHeight_in, nWidth_in);
				DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			case FILTER_SHARPEN:
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				SharpenFilter(Pad, Out, nHeight_in, nWidth_in);
				DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			case FILTER_GAUSSIAN:
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				GaussianFilter(Pad, Out, nHeight_in, nWidth_in);
				DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			}
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++) {//원본 데이터를 Pad의 중앙에 복사
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}

	for (int h = 0; h < nPadSize; h++) {//위 아래 가장자리 값 복사
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++) {//좌, 우 가장자리 값 복사
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}

	for (int h = 0; h < nPadSize; h++) {//4개의 귀퉁이 값 복사
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}
	return Pad;
}

void AverageFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {	
	int sum;//filter내의 픽셀값 총합

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			sum = 0;
			for (int i = 0; i < nFilterSize; i++)
				for (int j = 0; j < nFilterSize; j++)
					sum += In_Pad[h + i][w + j];//필터 내부의 픽셀값 총합
			Out[h][w] = sum / (nFilterSize*nFilterSize);//sum을 필터의 픽셀 개수로 devide 
		}
	}
}

void SmoothFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth) {
	int sum;//filter내의 픽셀값 총합
	int filter_size = 3;
	double smoothFilter[][3] = {
		{0.0625, 0.125, 0.0625},
		{0.125, 0.25, 0.125},
		{0.0625, 0.125, 0.0625}
	};

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			sum = 0;
			for (int i = 0; i < filter_size; i++)
				for (int j = 0; j < filter_size; j++)
					sum += (int)(In_Pad[h + i][w + j] * smoothFilter[i][j]);
			Out[h][w] = sum;
		}
	}
}

void SharpenFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth) {
	int filterStart_h, filterStart_w;//각 픽셀 당 필터의 좌상단 기준점
	int sumOfPixelInFilter = 0;//filter내의 픽셀값 총합
	int sharpenFilter[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};

	for (int h = 1; h < nHeight + 1; h++) {
		for (int w = 1; w < nWidth + 1; w++) {
			filterStart_h = h - 1;
			filterStart_w = w - 1;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					sumOfPixelInFilter += In_Pad[filterStart_h + i][filterStart_w + j] * sharpenFilter[i][j];
			if (sumOfPixelInFilter < 0)
				Out[h - 1][w - 1] = 0;
			else if (sumOfPixelInFilter > 255)
				Out[h - 1][w - 1] = 255;
			else {
				if (sumOfPixelInFilter < 0 || sumOfPixelInFilter > 255)
					cout << sumOfPixelInFilter << endl;
				Out[h - 1][w - 1] = sumOfPixelInFilter;
			}
			sumOfPixelInFilter = 0;
		}
	}
}

void GaussianFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth) {
	float sigma = 1.0f;
	int gaussianSum = 0;

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			gaussianSum = 0;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					gaussianSum += In_Pad[h + i][w + j] * (( (expf(-(h*h + w*w) / (2*sigma*sigma))) / sqrt(2 * PI * (sigma * sigma))));
			Out[h][w] = gaussianSum;
			
		}
	}
}

void InputGaussianNoise(unsigned char **In, unsigned char **Out, int nHeight, int nWidth, float fMean, float fStdev) {
	float fTemp = 0, fPDF[256] = { 0.0f };
	//fPDF: 확률밀도함수,  fMean: fStdev: 
	cout << "before GetGaussianPDF" << endl;
	for (int i = 0; i < 256; i++)
		cout << fPDF[i] << " ";

	GetGaussianPDF(fPDF, 256, fMean, fStdev);
	
	cout << "After GetGaussianPDF" << endl;

	for (int i = 0; i < 256; i++)
		cout << fPDF[i] << " ";
	srand(GetTickCount());
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			//원본에 잡음을 더한 형태의 영상을 저장.
			fTemp = (float)In[h][w] + GetNoise(fPDF, 256);
			Out[h][w] = saturate_cast<unsigned char>(fTemp);
		}
	}
}

void GetGaussianPDF(float* EmptyPDF, int nLength, float fMean, float fStDev) {
	int n;
	int Center = nLength / 2;
	float x; 

	for (n = 0; n < nLength; n++) {
		x = (float)(n - Center);//가우시안 확률 분포의 중심으로부터 떨어진 거리(방향값도 가짐)
		//fStDev: 시그마 fMean: u
		EmptyPDF[n] = (1 / ((float)sqrt(2 * PI)*fStDev)) * exp(-pow(x - fMean, 2) / (2 * fStDev * fStDev));
	}
}

float GetNoise(float *PDF, int nLength) {
	//확률밀도함수와 길이 256이 인자로 전달.

	int n;
	int Center = nLength / 2;
	float fRand, fComp, fTemp = 0;
	float x = 0, fDiff;
	float *CDF = new float[nLength];

	CDF[0] = 0;

	fRand = (float)rand() / (RAND_MAX + 1);

	for (n = 1; n < nLength; n++) {
		CDF[n] = (PDF[n] + PDF[n - 1]) / 2 + CDF[n - 1];
		fDiff = fRand - CDF[n];
		if (fDiff < 0) {
			x = ((float)n - Center);
			break;
		}
	}
	fComp = (fRand - CDF[n - 1]) / (CDF[n] - CDF[n - 1]);	

	delete[] CDF;

	return x + fComp;
}

void InputSaltPepperNoise(unsigned char **In, unsigned char **Out, int nHeight, int nWidth, float fSProb, float fPProb) {
	float Low = fSProb;
	float High = 1.0f - fPProb;
	float fRand;

	srand(GetTickCount());

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			fRand = ((float)rand() / RAND_MAX);

			if (fRand < Low) {
				Out[h][w] = 255;

			}
			else if (fRand > High) {
				Out[h][w] = 0;

			}
			else
				Out[h][w] = In[h][w];
		}
	}
}

void CViewerDlg::OnNoisefilteringMeanfilter()
{
	CMeanFilterDlg MFDlg;

	if (isImageOpened == true && nFormat == FORMAT_GRAY) {
		if (MFDlg.DoModal() == IDOK) {
			int nFilterSize = MFDlg.GetFilterSize();
			int nFilterType = MFDlg.GetMeanFilterType();

			if (nFilterSize < 3) {
				MessageBox("필터 크기가 잘못되었습니다. ");
				return;
			}
			if (nFilterSize % 2 == 0)
				nFilterSize++;

			CString OutFileName = InpFileName;
			unsigned char **Out = MemAlloc2D(nHeight_in, nWidth_in, 0);
			
			switch (nFilterType) {				
			case NF_ARITHMETIC:				
				ArithmeticMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_AMF.raw";
				break;
			case NF_GEOMETRIC:				
				GeometricMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_GMF.raw";
				break;
			case NF_HARMONIC:				
				HarmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_HMF.raw";
				break;
			case NF_CONTRAHARMONIC:
				ContraharmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_CHMF.raw";
				break;
			}
			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);
			FILE *fp;

			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
		}
	}
}

void CViewerDlg::ArithmeticMeanFilter(unsigned char **Img_in, unsigned char **Out, int nHeight, int nWidth, int nFilterSize) {
	int nTemp, nPadSize = (int)(nFilterSize / 2);
	unsigned char **Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			nTemp = 0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					nTemp += Img_Pad[h + m][w + n];				
				}
			}
			Out[h][w] = static_cast<unsigned char>(nTemp / (nFilterSize * nFilterSize));
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}

void CViewerDlg::GeometricMeanFilter(unsigned char **Img_in, unsigned char **Out, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char **Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	double mul_sum=0.0;

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			mul_sum = 1.0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					mul_sum *= (double)Img_Pad[h + m][w + n];//픽셀들의 곱
				}
			}
			Out[h][w] = static_cast<unsigned char>(pow(mul_sum, 1.0/(nFilterSize * nFilterSize)));
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}

void CViewerDlg::HarmonicMeanFilter(unsigned char **Img_in, unsigned char **Out, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char **Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	double inverse_sum = 0.0;

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			inverse_sum = 0.0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					inverse_sum += 1.0 / Img_Pad[h + m][w + n];//역수의 합
				}
			}
			Out[h][w] = static_cast<unsigned char>((nFilterSize * nFilterSize) / inverse_sum);//평균의 역수
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}

void CViewerDlg::ContraharmonicMeanFilter(unsigned char **Img_in, unsigned char **Out, int nHeight, int nWidth, int nFilterSize) {
	double q = 0; // q값 수정 가능 
	int nTemp_q_plus_one, nTemp_q , nPadSize = (int)(nFilterSize / 2);
	unsigned char **Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			nTemp_q_plus_one = 0;
			nTemp_q = 0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					nTemp_q_plus_one += pow((double)Img_Pad[h + m][w + n], q+1.0);
					nTemp_q += pow((double)Img_Pad[h + m][w + n], q);
				}
			}
			Out[h][w] = static_cast<unsigned char>(nTemp_q_plus_one / nTemp_q);
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);

}


void CViewerDlg::OnNoisefilteringOrder()
{
	COrderStatisticsDlg OSFDlg;

	if (isImageOpened == true && nFormat == FORMAT_GRAY) {
		if (OSFDlg.DoModal() == IDOK) {
			int nFilterSize = OSFDlg.GetFilterSize1();

			int nFilterType = OSFDlg.GetOSFType();

			if (nFilterSize < 3) {
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}
			if (nFilterSize % 2 == 0) {
				nFilterSize++;
			}

			CString OutFileName = InpFileName;
			unsigned char **Out = MemAlloc2D(nHeight_in, nWidth_in, 0);
			

			switch (nFilterType) {
			case NF_MEDIAN:						
				MedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_MEDF.raw";
				break;
			case NF_ADAPTIVEMEDIAN:				
				int MaxFilterSize = OSFDlg.GetFilterSize2();
				AdaptiveMedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, MaxFilterSize);
				OutFileName += "_AMEDF.raw";
				break;
			}

			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

			FILE *fp;

			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
		}
	}
}

void CViewerDlg::MedianFilter(unsigned char **Img_in, unsigned char **Out, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char **Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	unsigned char *pixelInMask = new unsigned char[nFilterSize*nFilterSize];//필터 크기만큼 배열 할당
	unsigned char k = 0, mid = 0;//Median filter mask 내부의 값을 순회하기 위한 변수
	
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {			
			for (int i = 0; i < nFilterSize; i++){
				for (int j = 0; j < nFilterSize; j++)
					pixelInMask[k++] = Img_Pad[h + i][w + j];//필터 내부의 값을 배열에 저장
			}	
			sort(pixelInMask, pixelInMask + (nFilterSize * nFilterSize));//저장된 픽셀 값을 오름차순으로 정렬			
			//정렬된 픽셀값들의 중앙값을 현재 픽셀값으로 대치
			mid = (nFilterSize * nFilterSize) / 2;
			Out[h][w] = pixelInMask[mid];
			k = 0;
		} 
	}

	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
	delete pixelInMask;
}

void CViewerDlg::AdaptiveMedianFilter(unsigned char **Img_in, unsigned char**Out, int nHeight, int nWidth, int nFilterSize_min, int nFilterSize_Max) {
	
}


void CViewerDlg::OnGetpsnr()
{
	CPSNRDlg PSNRDlg;

	PSNRDlg.DoModal();
}



void CViewerDlg::OnNoisereductionAddnoiseClicked()
{
	CAddNoiseDlg AddNoiseDlg;
	BYTE **NoiseImg = MemAlloc2D(nHeight_in, nWidth_in, 0);
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (AddNoiseDlg.DoModal() == IDOK) {
			CString OutFileName = InpFileName;
			float Value_1 = AddNoiseDlg.GetVal_1();
			float Value_2 = AddNoiseDlg.GetVal_2();
			int nNoiseType = AddNoiseDlg.GetNoiseType();

			if (nNoiseType == NOISE_GN) {
				if (Value_2 < 1) {
					MessageBox("표준편차 값이 잘못되었습니다. 표준편차 값은 1 이상이어야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputGaussianNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "_GN.raw";
			}
			else {
				if (Value_1 <= 0 && Value_2 <= 0) {
					MessageBox("두 개의 확률 값 중 적어도 하나는 0 보다 커야 합니다");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputSaltPepperNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "SPN.raw";
			}

			FILE *fp;
			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(NoiseImg[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
			MemFree2D(NoiseImg, nHeight_in);
		}
	}
}
