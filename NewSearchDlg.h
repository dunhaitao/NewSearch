
// NewSearchDlg.h: 头文件
//

#pragma once
#include "CStockName.h"

typedef struct tagBar {
	int date;
	int open;
	int high;
	int low;
	int close;
	int amount;
	int vol;
	int reserved;
} BAR;

class CRunBar : public CObject {
public:
	int tm_mday = 0;
	int tm_mon = 0;
	int tm_year = 0;
	int tm_yweek = 0;
	int tm_wday = 0;
	int tm_yday = 0;
	int open = 0;
	int high = 0;
	int low = 0;
	int close = 0;
	int chigh = 0;
	int clow = 0;
	size_t idx = 0;
	size_t cidx = 0;
	double dif = 0;
	double dea = 0;
	int dea_val = 0;
};

typedef CMap<int, int, int, int> YEARS_MAP;

// CNewSearchDlg 对话框
class CNewSearchDlg : public CDialogEx
{
// 构造
public:
	CNewSearchDlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CNewSearchDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSEARCH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	CMap<CString, LPCTSTR, CStockName*, CStockName*> m_map_stockname;
	CString m_str_tdxHome;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL LoadIniFile();
	BOOL LoadStockNames();
	BOOL LoadTnfFile(LPCTSTR market);
	UINT Search();
	void FindFiles(LPCTSTR findstr, CObList&);
	int Calculate(CStockFile*);
	void TranslateBar2Runbar(BAR* bar_array, UINT bar_count, CObList& day_list);
	int calculate_yweek(int year, int yday);
	void translate_day_week_month(CObList& day_list, CObList& mon_list, CObList& week_list_1, CObList& week_list_2, CObList& week_list_3);
	void process_macd(const CObList& bar_list);
	int process_cross(const CObList& bar_list);
	int process_result_0(const CObList& bar_list, int* cross_array);
	int process_result_1(const CObList& day_list, const CObList& mon_list, const CObList& week_list, int* crossess);
	int more_than(const CObList& bar_list);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonExport();
};

