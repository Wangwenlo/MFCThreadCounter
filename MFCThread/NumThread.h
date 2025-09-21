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
	CMFCThreadDlg* m_pParent; //������ָ��
	HANDLE m_hThread;   //�߳̾��
	DWORD m_dwThreadId;        // �߳�ID
	volatile BOOL m_bRunning;  // �߳����б�־
	volatile BOOL m_bStop;     // �߳�ֹͣ�����־
	volatile BOOL m_bPause;

};