
// priceManagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "priceManager.h"
#include "priceManagerDlg.h"
#include "afxdialogex.h"

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


// CpriceManagerDlg 대화 상자



CpriceManagerDlg::CpriceManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRICEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CpriceManagerDlg::~CpriceManagerDlg()
{
	if (isData) t_data.Close();
	for (unsigned int i = 0; i < nTab; i++)
	{
		free(bufferUni_data[i]);
		free(bufferUni_title[i]);
	}
	if (bufferUni_data) free(bufferUni_data);
	if (bufferUni_title) free(bufferUni_title);
}

void CpriceManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpriceManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(comboBox_ID1, &CpriceManagerDlg::OnCbnSelChange_t)
	ON_CBN_SELCHANGE(comboBox_ID2, &CpriceManagerDlg::OnCbnSelChange_s)
END_MESSAGE_MAP()


// CpriceManagerDlg 메시지 처리기

BOOL CpriceManagerDlg::OnInitDialog()
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
	m_title.Create(WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST, { 30,10,130,30 }, this, comboBox_ID1);
	m_sumBox.Create(WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST, { 30,320,130,340 }, this, comboBox_ID2);

	bufferUni_title = (LPTSTR*)malloc(tTabMax * sizeof(LPTSTR));
	if (!bufferUni_title) return 0;
	bufferUni_data = (LPTSTR*)malloc(tTabMax * sizeof(LPTSTR));
	if (!bufferUni_data) return 0;



	if (isData = t_data.Open(_T("priceData.txt"), CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate, NULL))
	{
		int nBytesRead = t_data.Read(buffer, sizeof(buffer) - 1);
		if (nBytesRead != t_data.GetLength())
		{
			CString msgA;
			msgA.Format(_T("파일 데이터가 너무 큽니다."));
			MessageBox(msgA);
			t_data.Close();
			isData = false;
			return 0;
		}
		unsigned char* pBuffer = buffer;
		unsigned char* pBuffer2 = pBuffer;
		buffer[nBytesRead] = '\0';



		while (*pBuffer2)
		{
			while (*(++pBuffer2) != '@') { if (!*pBuffer2) break; }
			char* aa = (char*)malloc((pBuffer2 - pBuffer) + 1);
			if (!aa) break;
			char* tmp = aa;
			bufferUni_title[nTab] = (LPTSTR)malloc(tTitleMax * sizeof(LPTSTR));
			if (!bufferUni_title) break;
			for (; pBuffer < pBuffer2; )
			{
				*(tmp++) = *(pBuffer++);
			}
			*tmp = '\0';
			++pBuffer; ++pBuffer; ++pBuffer;
			++pBuffer2; ++pBuffer2; ++pBuffer2;
			MultiByteToWideChar(CP_UTF8, 0, aa, (int)strlen(aa) + 1, bufferUni_title[nTab], MultiByteToWideChar(CP_UTF8, 0, aa, (int)strlen(aa), 0, 0) + 1);
			m_title.AddString(bufferUni_title[nTab]);
			free(aa);



			while (*(++pBuffer2) != '!') { if (!*pBuffer2) break; }
			aa = (char*)malloc((pBuffer2 - pBuffer) + 1);
			if (!aa)
			{
				free(bufferUni_title[nTab]);
				break;
			}
			tmp = aa;
			bufferUni_data[nTab] = (LPTSTR)malloc(tDataMax * sizeof(LPTSTR));
			if (!bufferUni_data[nTab])
			{
				free(bufferUni_title[nTab]);
				break;
			}
			for (; pBuffer < pBuffer2; )
			{
				*(tmp++) = *(pBuffer++);
			}
			*tmp = '\0'; ++pBuffer; ++pBuffer2;
			MultiByteToWideChar(CP_UTF8, 0, aa, (int)strlen(aa) + 1, bufferUni_data[nTab], MultiByteToWideChar(CP_UTF8, 0, aa, (int)strlen(aa), 0, 0) + 1);
			free(aa);
			++nTab;
			if (*pBuffer2 != '\0')
			{
				++pBuffer; ++pBuffer;
				++pBuffer2; ++pBuffer2;
			}
		}
	}
	int tmp001 = 0;
	if (item_data = (CEdit**)malloc(tItemMax * sizeof(CEdit**)))
	{
		++tmp001;
		if (price_data = (CEdit**)malloc(tItemMax * sizeof(CEdit**)))
		{
			++tmp001;
			if (class_data = (CEdit**)malloc(tItemMax * sizeof(CEdit**)))
			{
				++tmp001;
				if (itemsum_data = (CEdit**)malloc(tItemMax * sizeof(CEdit**))) ++tmp001;
			}
		}
	}
	if (tmp001 != 4)
	{
		if (tmp001 != 0)
		{
			free(item_data);
			if (tmp001 != 1)
			{
				free(price_data);
				if (tmp001 != 2) free(class_data);
			}
		}
		return 0;
	}
	for (int i = 0; i < tItemMax; i++)item_data[i] = new CEdit();
	for (int i = 0; i < tItemMax; i++)price_data[i] = new CEdit();
	for (int i = 0; i < tItemMax; i++)class_data[i] = new CEdit();
	for (int i = 0; i < tItemMax; i++)itemsum_data[i] = new CEdit();

	m_title.SetCurSel(0);
	tFunc_selchange(0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CpriceManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CpriceManagerDlg::OnPaint()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CpriceManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CpriceManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return false;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CpriceManagerDlg::OnCbnSelChange_t()
{
	tFunc_selchange(m_title.GetCurSel());
}

void CpriceManagerDlg::tFunc_selchange(int nSel)
{
	scrollIndex = 0;
	for (unsigned int i = 0; i < nData; i++)
	{
		::DestroyWindow(item_data[i]->m_hWnd);
		::DestroyWindow(price_data[i]->m_hWnd);
		::DestroyWindow(class_data[i]->m_hWnd);
		::DestroyWindow(itemsum_data[i]->m_hWnd);
	}
	nData = 0;
	LPTSTR aa = (LPTSTR)malloc(tTitleMax * sizeof(LPTSTR));
	if (!aa) return;

	LPTSTR pBuffer = bufferUni_data[nSel];
	LPTSTR pBuffer2 = pBuffer;
	while (*pBuffer2)
	{
		item_data[nData]->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, { 10,40 + (long)(nData * 25),110,60 + (long)(nData * 25) }, this, 3 * nData + 1);
		price_data[nData]->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, { 115,40 + (long)(nData * 25),345,60 + (long)(nData * 25) }, this, 3 * nData + 1);
		class_data[nData]->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, { 350,40 + (long)(nData * 25),450,60 + (long)(nData * 25) }, this, 3 * nData + 1);
		itemsum_data[nData]->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, { 455,40 + (long)(nData * 25),550,60 + (long)(nData * 25) }, this, 3 * nData + 1);
		LPTSTR tmp = aa;
		while (*(++pBuffer2) != 0x0020/*_T(" ")*/) { if (!*pBuffer2) break; }
		for (; pBuffer < pBuffer2; )
		{
			*(tmp++) = *(pBuffer++);
		}
		*tmp = 0x0000;
		item_data[nData]->SetWindowTextW(aa);

		++pBuffer;
		++pBuffer2;
		tmp = aa;
		while (*(++pBuffer2) != 0x0020/*_T(" ")*/) { if (!*pBuffer2) break; }
		for (; pBuffer < pBuffer2; )
		{
			*(tmp++) = *(pBuffer++);
		}
		*tmp = 0x0000;
		price_data[nData]->SetWindowTextW(aa);

		++pBuffer;
		++pBuffer2;
		tmp = aa;
		while (*(++pBuffer2) != 0x000D/*'\r'*/) { if (!*pBuffer2) break; }
		for (; pBuffer < pBuffer2; )
		{
			*(tmp++) = *(pBuffer++);
		}
		*tmp = 0x0000;
		class_data[nData]->SetWindowTextW(aa);

		++nData;
		if (*pBuffer2 != 0x0000)
		{
			++pBuffer; ++pBuffer;
			++pBuffer2; ++pBuffer2;
		}
	}
	free(aa);

	m_sumBox.ResetContent();
	m_sumBox.AddString(_T("전체"));
	m_sumBox.SetCurSel(0);
	sFunc_selchange(0);
}

void CpriceManagerDlg::OnCbnSelChange_s()
{
	sFunc_selchange(m_sumBox.GetCurSel());
}

void CpriceManagerDlg::sFunc_selchange(int nSel)
{
	CString sumselText;
	m_sumBox.GetLBText(nSel, sumselText);
	MessageBox((LPCTSTR)sumselText);

}