
// NewSearchDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NewSearch.h"
#include "NewSearchDlg.h"
#include "CStockName.h"
#include "afxdialogex.h"
#include "ta_libc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CNewSearchDlg 对话框



CNewSearchDlg::CNewSearchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEWSEARCH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNewSearchDlg::~CNewSearchDlg()
{
	CMap<CString, LPCTSTR, CStockName*, CStockName*>::CPair* p = m_map_stockname.PGetFirstAssoc();
	while (NULL != p) {
		delete p->value;
		p = m_map_stockname.PGetNextAssoc(p);
	}
}

void CNewSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewSearchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CNewSearchDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CNewSearchDlg::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// CNewSearchDlg 消息处理程序

BOOL CNewSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CListCtrl* p_list = (CListCtrl*)GetDlgItem(IDC_LIST_SEARCH_RESULT);
	p_list->SetExtendedStyle(p_list->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加三列
	RECT rect;
	p_list->GetClientRect(&rect);
	p_list->InsertColumn(0, _T("序号"), LVCFMT_CENTER, rect.right * 7 / 100);
	p_list->InsertColumn(1, _T("股票代码"), LVCFMT_CENTER, rect.right * 14 / 100);
	p_list->InsertColumn(2, _T("股票名称"), LVCFMT_CENTER, rect.right / 5);
	p_list->InsertColumn(3, _T("最新价格"), LVCFMT_RIGHT, rect.right * 14 / 100);
	p_list->InsertColumn(4, _T(""), LVCFMT_CENTER, rect.right / 5);
	p_list->InsertColumn(5, _T(""), LVCFMT_CENTER, rect.right / 5);

	BOOL result = FALSE;
	if (TRUE == LoadIniFile()) {
		if (TRUE == LoadStockNames()) {
			result = TRUE;
		}
	}
	return result;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNewSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNewSearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNewSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CNewSearchDlg::LoadStockNames()
{
	BOOL result = FALSE;
	if (TRUE == LoadTnfFile(_T("sh"))) {
		if (TRUE == LoadTnfFile(_T("sz"))) {
			result = TRUE;
		}
	}
	return result;
}

struct TdxSymbolMap {
	char code[23];
	char name[49];
	char type;
	char x1[200];
	char type2;
	char x2[11];
	char sname[29];
};

BOOL CNewSearchDlg::LoadTnfFile(LPCTSTR market)
{
	BOOL result = TRUE;
	TdxSymbolMap buff[32];
	UINT read_count = 0;
	//
	CString filename;
	filename.AppendFormat(_T("%s\\T0002\\hq_cache\\%sm.tnf"), m_str_tdxHome.GetString(), market);
	TRACE(_T("filename:%s\n"), filename.GetString());
	//
	try {
		CFile tnffile(filename.GetString(), CFile::modeRead | CFile::typeBinary);
		tnffile.Seek(50, CFile::begin);
		CString fullcode;
		do {
			LPTSTR code, name, sname;
			int code_len, name_len, sname_len;
			read_count = tnffile.Read(buff, sizeof(TdxSymbolMap) * 32);
			read_count = read_count / sizeof(TdxSymbolMap);
			for (UINT idx = 0; idx < read_count; idx++) {
				if ((2 == (int)buff[idx].type) && (35 == (int)buff[idx].type2)) {
					fullcode.Empty();
					CStockName* p = new CStockName;
					code_len = _mbstowcsz(0, buff[idx].code, 0);
					code = new TCHAR[code_len];
					code_len = _mbstowcsz(code, buff[idx].code, code_len);
					name_len = _mbstowcsz(0, buff[idx].name, 0);
					name = new TCHAR[name_len];
					name_len = _mbstowcsz(name, buff[idx].name, name_len);
					sname_len = _mbstowcsz(0, buff[idx].sname, 0);
					sname = new TCHAR[sname_len];
					sname_len = _mbstowcsz(sname, buff[idx].sname, sname_len);
					fullcode.AppendFormat(_T("%s%s"), market, code);
					p->fullcode.SetString(fullcode.GetString());
					p->code.SetString(code);
					p->name.SetString(name);
					p->sname.SetString(sname);
					m_map_stockname.SetAt(fullcode.GetString(), p);
					TRACE(_T("size:%d, code:%s, type:%d, name:%s\n"), sizeof(TdxSymbolMap), code, buff[idx].type2, name);
					delete[] code;
					delete[] name;
					delete[] sname;
				}
			}
		} while (32 == read_count);
		tnffile.Close();
	}
	catch (CFileException* e) {
		result = FALSE;
		AfxMessageBox(_T("没有找到股票名称数据库文件，请将本程序放至到通达信目录，再执行！"));
	}
	return result;
}

LPCTSTR SV_5MIN_FILE_TEMPLATE = _T("%s\\vipdoc\\%s\\fzline\\%s%s.lc5");
LPCTSTR SV_LDAY_FILE_TEMPLATE = _T("%s\\vipdoc\\%s\\lday\\%s%s.day");
LPCTSTR iniFile = _T("\\Stock.ini");
LPCTSTR appFile = _T("TDX_APP_HOME_DIR");
LPCTSTR keyFile = _T("tdx_home");

BOOL CNewSearchDlg::LoadIniFile() {
	TCHAR str_tmp_define[1024] = { 0 };
	memset(str_tmp_define, 0, sizeof(TCHAR) * 1024);
	::GetCurrentDirectory(sizeof(str_tmp_define), str_tmp_define);
	this->m_str_tdxHome.SetString(str_tmp_define);
	return TRUE;
}

//OnBnClickedButtonSearch
void CNewSearchDlg::OnBnClickedButtonSearch() {
	CButton* p_btn_search = (CButton*)GetDlgItem(IDC_BUTTON_SEARCH);
	CButton* p_btn_cancel = (CButton*)GetDlgItem(IDCANCEL);
	CButton* p_btn_export = (CButton*)GetDlgItem(IDC_BUTTON_EXPORT);
	p_btn_search->EnableWindow(FALSE);
	p_btn_cancel->EnableWindow(FALSE);
	UINT searched_count = Search();
	p_btn_cancel->EnableWindow(TRUE);
	if (searched_count < 0) {
		p_btn_search->EnableWindow(TRUE);
	}
	// p_btn_export->ShowWindow(SW_SHOW);
}

//OnBnClickedButtonExport
void CNewSearchDlg::OnBnClickedButtonExport() {
	const int UNICODE_TXT_FLG = 0xFEFF;//UNICODE文本标示
	// TODO: 在此添加控件通知处理程序代码
	CListCtrl* p_list = (CListCtrl*)GetDlgItem(IDC_LIST_SEARCH_RESULT);
	CButton* p_btn_export = (CButton*)GetDlgItem(IDC_BUTTON_EXPORT);
	p_btn_export->EnableWindow(FALSE);
	int item_count = p_list->GetItemCount();
	CFile file(_T("NewSearch.csv"), CFile::modeCreate | CFile::modeWrite);
	file.Write(&UNICODE_TXT_FLG, 2);
	for (int i = 0; i < item_count; i++) {
		for (int j = 1; j < 4; j++) {
			LPCTSTR pstr = p_list->GetItemText(i, j).GetString();
			TRACE1("pstr:%s\n", pstr);
			if (j > 1) {
				file.Write(_T(","), sizeof(TCHAR));
			}
			file.Write(pstr, wcslen(pstr) * sizeof(TCHAR));
			TRACE1("pstr:%s\n", pstr);
		}
		file.Write(_T("\n"), sizeof(TCHAR));
	}
	file.Close();
	AfxMessageBox(_T("导出完毕"), MB_OK);
}

UINT CNewSearchDlg::Search() {
	UINT result = 0;
	CObList all_files;
	CString fullpath;
	LPCTSTR sh = _T("sh");
	LPCTSTR sz = _T("sz");
	LPCTSTR start = _T("*");
	LPCTSTR str_result_0 = _T("☆·妖股初现");
	LPCTSTR str_result_1 = _T("★·擒龙抓妖");
	TCHAR str_seq[32] = { 0 };
	::TA_Initialize();
	//
	const int UNICODE_TXT_FLG = 0xFEFF;//UNICODE文本标示
	try {
		CFile file(_T("NewSearch.csv"), CFile::modeCreate | CFile::modeWrite);
		file.Write(&UNICODE_TXT_FLG, 2);
		//
		fullpath.AppendFormat(SV_LDAY_FILE_TEMPLATE, m_str_tdxHome, sh, sh, start);
		FindFiles(fullpath.GetString(), all_files);
		fullpath.Empty();
		fullpath.AppendFormat(SV_LDAY_FILE_TEMPLATE, m_str_tdxHome, sz, sz, start);
		FindFiles(fullpath.GetString(), all_files);
		//
		INT_PTR file_count = all_files.GetSize();
		CProgressCtrl* pProg = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SEARCHED);
		CListCtrl* p_list = (CListCtrl*)GetDlgItem(IDC_LIST_SEARCH_RESULT);
		if (file_count > 0) {
			pProg->SetRange(0, file_count);
			short searched_count = 0, result_count = 0;
			POSITION position = all_files.GetHeadPosition();
			for (; NULL != position; searched_count++) {
				CStockFile* p_cstockfile = (CStockFile*)all_files.GetNext(position);
				CMap<CString, LPCTSTR, CStockName*, CStockName*>::CPair* p = m_map_stockname.PLookup(p_cstockfile->fullcode.GetString());
				if (NULL != p) {
					int calculate_result = Calculate(p_cstockfile);
					if (calculate_result > 0) {
						memset(str_seq, 0, sizeof(str_seq));
						_stprintf_s(str_seq, 32, _T("%d"), result_count + 1);
						CString filebuf;
						filebuf.AppendFormat(_T("'%s\t%s\t%s"), p->value->code.GetString(), p->value->name.GetString(), p_cstockfile->price.GetString());
						int list_row_idx = p_list->InsertItem(result_count, str_seq);
						p_list->SetItemText(result_count, 1, p->value->code.GetString());
						p_list->SetItemText(result_count, 2, p->value->name.GetString());
						p_list->SetItemText(result_count, 3, p_cstockfile->price.GetString());
						if (calculate_result & 0x0001) {
							p_list->SetItemText(result_count, 4, str_result_0);
							filebuf.AppendFormat(_T("\t%s"), str_result_0);
						}
						else {
							filebuf.Append(_T("\t"));
						}
						if (calculate_result & 0x0002) {
							p_list->SetItemText(result_count, 5, str_result_1);
							filebuf.AppendFormat(_T("\t%s"), str_result_1);
						}
						filebuf.Append(_T("\n"));
						// TRACE(_T("filebuf:%s, length:%d\n"), filebuf.GetString(), filebuf.GetLength());
						file.Write(filebuf.GetString(), filebuf.GetLength() * sizeof(TCHAR));
						result_count++;
						result = result_count;
					}
				}
				delete p_cstockfile;
				pProg->SetPos(searched_count);
			}
		}
		else {
			pProg->SetRange(0,  1);
			pProg->SetPos(1);
		}
		file.Close();
	}
	catch (CFileException* e) {
		AfxMessageBox(_T("请先关毕Excel文件！"), MB_OK);
		result = -1;
	}
	::TA_Shutdown();
	return result;
}

void CNewSearchDlg::FindFiles(LPCTSTR findstr, CObList& p_all_files) {
	CFileFind filefinder;
	BOOL find_result = filefinder.FindFile(findstr);
	size_t find_count = 0;
	TCHAR fullcode[16];
	while (find_result) {
		find_result = filefinder.FindNextFile();
		memset(fullcode, 0, sizeof(fullcode));
		_tcsncpy_s(fullcode, _countof(fullcode), filefinder.GetFileName().GetString(), 8);
		CStockFile* p = new CStockFile;
		p->fullcode.SetString(fullcode);
		p->fullpath.SetString(filefinder.GetFilePath().GetString());
		p_all_files.AddTail(p);
		find_count++;
	}
	TRACE(_T("find_count:%d\n"), find_count);
}

int CNewSearchDlg::Calculate(CStockFile* pfilepath) {
	int result = 0;
	POSITION position = NULL;
	BAR* bar_array = NULL;
	int cross_array[5] = { 0 };
	CObList day_list;
	CObList month_list;
	CObList week_list_1;
	CObList week_list_2;
	CObList week_list_3;
	//
	CFile stockfile(pfilepath->fullpath.GetString(), CFile::modeRead | CFile::modeNoTruncate | CFile::typeBinary);
	ULONGLONG bar_count = stockfile.GetLength() / sizeof(BAR);
	bar_array = new BAR[bar_count];
	stockfile.Read(bar_array, stockfile.GetLength());
	stockfile.Close();
	//
	TranslateBar2Runbar(bar_array, bar_count, day_list);
	translate_day_week_month(day_list, month_list, week_list_1, week_list_2, week_list_3);
	process_macd(day_list);
	process_macd(month_list);
	process_macd(week_list_1);
	process_macd(week_list_2);
	process_macd(week_list_3);
	cross_array[0]=process_cross(day_list);
	cross_array[1]=process_cross(month_list);
	cross_array[2]=process_cross(week_list_1);
	cross_array[3]=process_cross(week_list_2);
	cross_array[4]=process_cross(week_list_3);
	int result_0 = process_result_0(day_list, cross_array);
	int result_1 = process_result_1(day_list, month_list, week_list_3, cross_array);
	float close = ((float)(((CRunBar*)day_list.GetTail()))->close) / 100;
	pfilepath->price.Empty();
	pfilepath->price.AppendFormat(_T("%.2f"), close);
	delete[] bar_array;
	// TRACE(_T("%s: result_0:%d, result_1:%d\n"), pfilepath->fullcode.GetString(), result_0, result_1);
	for (position = day_list.GetHeadPosition(); NULL != position;) {
		CRunBar* pRunBar = (CRunBar*)day_list.GetNext(position);
		delete pRunBar;
	}
	for (position = month_list.GetHeadPosition(); NULL != position;) {
		CRunBar* pRunBar = (CRunBar*)month_list.GetNext(position);
		delete pRunBar;
	}
	for (position = week_list_1.GetHeadPosition(); NULL != position;) {
		CRunBar* pRunBar = (CRunBar*)week_list_1.GetNext(position);
		delete pRunBar;
	}
	for (position = week_list_2.GetHeadPosition(); NULL != position;) {
		CRunBar* pRunBar = (CRunBar*)week_list_2.GetNext(position);
		delete pRunBar;
	}
	for (position = week_list_3.GetHeadPosition(); NULL != position;) {
		CRunBar* pRunBar = (CRunBar*)week_list_3.GetNext(position);
		delete pRunBar;
	}
	result = result_0 + result_1;
	return result;
}

YEARS_MAP years_map;

void CNewSearchDlg::TranslateBar2Runbar(BAR* bar_array, UINT bar_count, CObList& day_list) {
	for (UINT idx = 0; idx < bar_count; idx++) {
		int tmp_i_date = (bar_array + idx)->date;
		CRunBar* p_run_stock = new CRunBar;
		tm tmp_tm;
		memset(&tmp_tm, 0, sizeof(tmp_tm));
		p_run_stock->tm_mday = tmp_i_date % 100;
		p_run_stock->tm_mon = (tmp_i_date - p_run_stock->tm_mday) / 100 % 100 - 1;
		p_run_stock->tm_year = (tmp_i_date - p_run_stock->tm_mday - p_run_stock->tm_mon * 100) / 10000 - 1900;
		tmp_tm.tm_mday = p_run_stock->tm_mday;
		tmp_tm.tm_mon = p_run_stock->tm_mon;
		tmp_tm.tm_year = p_run_stock->tm_year;
		time_t rtime = (mktime(&tmp_tm));
		::localtime_s(&tmp_tm, &rtime);
		p_run_stock->tm_year = tmp_tm.tm_year + 1900;
		p_run_stock->tm_mon += 1;
		p_run_stock->tm_wday = tmp_tm.tm_wday;
		p_run_stock->tm_yday = tmp_tm.tm_yday;
		p_run_stock->high = (bar_array + idx)->high;
		p_run_stock->low = (bar_array + idx)->low;
		p_run_stock->open = (bar_array + idx)->open;
		p_run_stock->close = (bar_array + idx)->close;
		p_run_stock->idx = idx;
		p_run_stock->tm_yweek = calculate_yweek(p_run_stock->tm_year, p_run_stock->tm_yday);
		day_list.AddTail(p_run_stock);
	}
}

int CNewSearchDlg::calculate_yweek(int year, int yday)
{
	int tm_wday = -1;
	int result = -1;

	int tmp_tm_wday = 0;
	YEARS_MAP::CPair* pPair = years_map.PLookup(year);
	if (NULL != pPair) {
		tm_wday = pPair->value;
	}
	else {
		struct tm tm_tmp;
		tm_tmp.tm_year = (year - 1900);
		tm_tmp.tm_mon = 0;
		tm_tmp.tm_mday = 1;
		tm_tmp.tm_hour = 12;
		tm_tmp.tm_min = 0;
		tm_tmp.tm_sec = 0;
		time_t time_tmp = mktime(&tm_tmp);
		localtime_s(&tm_tmp, &time_tmp);
		tm_wday = tm_tmp.tm_wday;
		years_map.SetAt(year, tm_wday);
	}
	result = (yday + tm_wday) / 7;
	return result;
}

void CNewSearchDlg::translate_day_week_month(CObList& day_list, CObList& mon_list, CObList& week_list_1, CObList& week_list_2, CObList& week_list_3)
{
	POSITION position = day_list.GetHeadPosition();
	CRunBar* p_week_bar_3 = NULL;
	CRunBar* p_week_bar_2 = NULL;
	CRunBar* p_week_bar_1 = NULL;
	CRunBar* p_mont_bar = NULL;
	int i_month = -1;
	int i_week = -1;
	INT_PTR max_day = day_list.GetSize();
	INT_PTR idx = 0;
	for (; NULL != position; idx++) {
		CRunBar* p_stock = (CRunBar*)day_list.GetNext(position);
		int i_tmp_month = 100 * p_stock->tm_year + p_stock->tm_mon;
		int i_tmp_week = 100 * p_stock->tm_year + p_stock->tm_yweek;
		if (i_tmp_month != i_month) {
			p_mont_bar = new CRunBar;
			p_mont_bar->open = p_stock->open;
			i_month = i_tmp_month;
			p_mont_bar->idx = i_month;
			mon_list.AddTail(p_mont_bar);
		}
		if (NULL != p_mont_bar) {
			p_mont_bar->close = p_stock->close;
		}
		if (i_tmp_week != i_week) {
			i_week = i_tmp_week;
			p_week_bar_3 = new CRunBar;
			p_week_bar_3->idx = i_week;
			week_list_3.AddTail(p_week_bar_3);
			if (idx < (max_day - 1)) {
				p_week_bar_2 = new CRunBar;
				p_week_bar_2->idx = i_week;
				week_list_2.AddTail(p_week_bar_2);
			}
			if (idx < (max_day - 2)) {
				p_week_bar_1 = new CRunBar;
				p_week_bar_1->idx = i_week;
				week_list_1.AddTail(p_week_bar_1);
			}
		}
		if (NULL != p_week_bar_3) {
			p_week_bar_3->close = p_stock->close;
		}
		if (idx < (max_day - 1)) {
			if (NULL != p_week_bar_2) {
				p_week_bar_2->close = p_stock->close;
			}
		}
		if (idx < (max_day - 2)) {
			if (NULL != p_week_bar_1) {
				p_week_bar_1->close = p_stock->close;
			}
		}
	}
}

void CNewSearchDlg::process_macd(const CObList& bar_list)
{
	const INT_PTR bar_count = bar_list.GetSize();
	TA_Real* close_array = new TA_Real[bar_count];
	TA_Real* dif_0_array = new TA_Real[bar_count];
	TA_Real* dif_1_array = new TA_Real[bar_count];
	TA_Real* dea_array = new TA_Real[bar_count];
	memset(close_array, 0, sizeof(TA_Real) * bar_count);
	memset(dif_0_array, 0, sizeof(TA_Real) * bar_count);
	memset(dif_1_array, 0, sizeof(TA_Real) * bar_count);
	memset(dea_array, 0, sizeof(TA_Real) * bar_count);
	TA_RetCode retcode;
	INT_PTR tmp_bar_idx = 0;
	for (POSITION position = bar_list.GetHeadPosition(); NULL != position; tmp_bar_idx++) {
		CRunBar* p_stock = (CRunBar*)bar_list.GetNext(position);
		close_array[tmp_bar_idx] = p_stock->close;
	}
	int i_out_begin_0 = -1;
	int i_nb_element_0 = -1;
	int i_out_begin_1 = -1;
	int i_nb_element_1 = -1;
	INT_PTR i_dea_start = -1;
	INT_PTR i_dea_end = -1;
	tmp_bar_idx = 0;
	retcode = TA_EMA(0, bar_count - 1, &close_array[0], 12, &i_out_begin_0, &i_nb_element_0, &dif_0_array[0]);
	retcode = TA_EMA(0, bar_count - 1, &close_array[0], 26, &i_out_begin_1, &i_nb_element_1, &dif_1_array[0]);
	for (POSITION position = bar_list.GetHeadPosition(); NULL != position; tmp_bar_idx++) {
		CRunBar* p_stock = (CRunBar*)bar_list.GetNext(position);
		if ((tmp_bar_idx < i_out_begin_0) || (tmp_bar_idx < i_out_begin_1)) {
			continue;
		}
		if (i_dea_start < 0) {
			i_dea_start = tmp_bar_idx;
		}
		i_dea_end = tmp_bar_idx;
		dif_0_array[tmp_bar_idx - i_out_begin_0] -= dif_1_array[tmp_bar_idx - i_out_begin_1];
		p_stock->dif = dif_0_array[tmp_bar_idx - i_out_begin_0];
		dif_1_array[tmp_bar_idx - i_out_begin_1] = p_stock->dif;
	}
	i_out_begin_0 = -1;
	i_nb_element_0 = -1;
	tmp_bar_idx = 0;
	retcode = TA_EMA(0, i_dea_end - i_out_begin_1, dif_1_array, 9, &i_out_begin_0, &i_nb_element_0, dea_array);
	for (POSITION position = bar_list.GetHeadPosition(); NULL != position; tmp_bar_idx++) {
		CRunBar* p_stock = (CRunBar*)bar_list.GetNext(position);
		if (tmp_bar_idx < (i_dea_start + i_out_begin_0)) {
			continue;
		}
		p_stock->dea = dea_array[tmp_bar_idx - i_dea_start - i_out_begin_0];
		p_stock->dea_val = 1;
	}
	delete[] close_array;
	delete[] dif_0_array;
	delete[] dif_1_array;
	delete[] dea_array;
}

int CNewSearchDlg::process_cross(const CObList& bar_list)
{
	POSITION position = bar_list.GetTailPosition();
	CRunBar* p_second = NULL;
	CRunBar* p_first = NULL;
	int b_bool = 0;
	int result = 0;
	INT_PTR i_list_size = bar_list.GetSize();
	if (i_list_size > 1) {
		if (NULL != position) {
			p_second = (CRunBar*)bar_list.GetPrev(position);
			if (NULL != position) {
				p_first = (CRunBar*)bar_list.GetPrev(position);
				if (p_first->dea_val > 0) {
					b_bool = 1;
				}
			}
		}
		if (b_bool) {
			if ((p_first->dea > p_first->dif) && (p_second->dif > p_second->dea)) {
				result = 1;
			}
		}
	}
	return result;
}

int CNewSearchDlg::process_result_0(const CObList& bar_list, int* crossess) {
	POSITION position = bar_list.GetTailPosition();
	CRunBar* p_second = NULL;
	CRunBar* p_first = NULL;
	int b_bool = 0;
	int result = 0;
	INT_PTR day_size = bar_list.GetSize();
	if (day_size > 1) {
		if (NULL != position) {
			p_second = (CRunBar*)bar_list.GetPrev(position);
			if (NULL != position) {
				p_first = (CRunBar*)bar_list.GetPrev(position);
				b_bool = 1;
			}
		}
	}
	if (b_bool) {
		double diff = ((double)p_second->close) / ((double)p_first->close);
		if (diff > 1.05) {
			if ((crossess[2] > 0) || (crossess[3] > 0) || (crossess[4] > 0)) {
				result = 1;
			}
		}
	}
	return result;
}

int CNewSearchDlg::process_result_1(const CObList& day_list, const CObList& mon_list, const CObList& week_list, int* crossess)
{
	POSITION position = day_list.GetTailPosition();
	CRunBar* p_second = NULL;
	CRunBar* p_first = NULL;
	int b_bool = 0;
	int result = 0;
	int day_more = more_than(day_list);
	int week_more = more_than(week_list);
	int month_more = more_than(mon_list);
	INT_PTR day_size = day_list.GetSize();
	if (day_size > 1) {
		if (NULL != position) {
			p_second = (CRunBar*)day_list.GetPrev(position);
			if (NULL != position) {
				p_first = (CRunBar*)day_list.GetPrev(position);
				b_bool = 1;
			}
		}
	}
	if (b_bool) {
		double diff = ((double)p_second->close) / ((double)p_first->close);
		if (diff > 1.08) {
			if ((day_more > 0 && week_more > 0 && crossess[1] > 0) ||
				(week_more > 0 && month_more > 0 && crossess[0] > 0) ||
				(month_more > 0 && day_more > 0 && crossess[4] > 0) ||
				(crossess[0] > 0 && crossess[1] > 0 && crossess[4] > 0)) {
				result = 2;
			}
		}
	}
	return result;
}

int CNewSearchDlg::more_than(const CObList& bar_list)
{
	POSITION position = bar_list.GetTailPosition();
	CRunBar* p_second = NULL;
	CRunBar* p_first = NULL;
	int b_bool = 0;
	int result = 0;
	INT_PTR bar_size = bar_list.GetSize();
	if (bar_size > 0) {
		if (NULL != position) {
			p_first = (CRunBar*)bar_list.GetPrev(position);
			if (p_first->dea_val > 0) {
				b_bool = 1;
			}
		}
	}
	if (b_bool > 0) {
		if (p_first->dif > p_first->dea) {
			result = 1;
		}
	}
	return result;
}
