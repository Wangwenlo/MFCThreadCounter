
// MFCThreadDlg.h: 头文件
//

#pragma once

#include "NumThread.h"
// CMFCThreadDlg 对话框
class CMFCThreadDlg : public CDialogEx
{
// 构造
public:
	CMFCThreadDlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CMFCThreadDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTHREAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	afx_msg LRESULT OnNMCustomdrawProgress1(WPARAM wParam, LPARAM lParam);
public:
	CProgressCtrl m_progress;
protected:
	Worker* m_wThreadWorker;
private:
	BOOL m_butAntry;
	BOOL m_bPause;

};
