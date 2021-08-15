#pragma once

// CStockName 命令目标

class CStockName : public CObject
{
public:
	CStockName();
	virtual ~CStockName();
	CString fullcode;
	CString code;
	CString name;
	CString sname;
};

class CStockFile : public CObject
{
public:
	CString fullcode;
	CString fullpath;
	CString price;
};
