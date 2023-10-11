
// priceManagerDlg.h: 헤더 파일
//

#pragma once
#define tTabMax 0xf
#define tTitleMax 0xff
#define tDataMax 0xffff
#define tItemMax 50
#define comboBox_ID1 1001
#define comboBox_ID2 1002
#define sumEdit_ID 1003

// CpriceManagerDlg 대화 상자
class CpriceManagerDlg : public CDialogEx
{
// 생성입니다.
public:
	CpriceManagerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CpriceManagerDlg();
	CFile t_data;
	bool isData = false;
	BYTE buffer[tDataMax] = { 0, };				// 총 데이터
	LPTSTR* bufferUni_title = nullptr;			// 월별 데이터 제목
	LPTSTR* bufferUni_data = nullptr;			// 월별 데이터 내용
	unsigned int nTab = 0;						// 데이터 총 탭수
	unsigned int scrollIndex = 0;				// 몇 번째 스크롤인지
	unsigned int nData = 0;						// 데이터 항목 개수
	unsigned int nClass = 0;					// 항목 종류 개수
	// CEdit들
	CEdit** item_data = nullptr;
	CEdit** price_data = nullptr;
	CEdit** class_data = nullptr;
	CEdit** itemsum_data = nullptr;
	CEdit ifSum_data;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRICEMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnCbnSelChange_t();
	void OnCbnSelChange_s();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void tFunc_selchange(int nSel);
	void sFunc_selchange(int nSel);
public:
	CComboBox m_title, m_sumBox;
};
