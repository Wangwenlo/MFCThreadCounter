#pragma once


class CMFCThreadDlg;

class Worker
{
public:
	Worker(CMFCThreadDlg* pParent);
	virtual ~Worker();

	BOOL StartThread();
	BOOL StopThread();
	BOOL PauseThread();
	BOOL ContinueThread();
	BOOL IsRunning() const;

	static UINT function(LPVOID pParam);
private:
	CMFCThreadDlg* m_pParent; //父窗口指针
	HANDLE m_hThread;   //线程句柄
	DWORD m_dwThreadId;        // 线程ID
	volatile BOOL m_bRunning;  // 线程运行标志
	volatile BOOL m_bStop;     // 线程停止请求标志
	volatile BOOL m_bPause;

};