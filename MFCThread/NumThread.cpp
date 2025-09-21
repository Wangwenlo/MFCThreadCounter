
#include "pch.h"
#include "NumThread.h"
#include "MFCThreadDlg.h"

// 自定义消息
#define WM_THREAD_UPDATE (WM_USER + 100)


Worker::Worker(CMFCThreadDlg* pParent)
	:
	m_pParent(pParent),
	m_hThread(nullptr),
	m_dwThreadId(0),
	m_bRunning(FALSE),
	m_bStop(FALSE),
	m_bPause(FALSE)
{

}
Worker::~Worker()
{
	StopThread();
}

BOOL Worker::StartThread()
{
	if (m_bRunning)
		return FALSE; // 线程已在运行
	m_bStop = FALSE;
	m_bRunning = TRUE;
	m_hThread = ::CreateThread(
		nullptr,
		0,
		(LPTHREAD_START_ROUTINE)function,
		this,
		0,
		&m_dwThreadId);


	return (m_hThread != nullptr);
}

BOOL Worker::StopThread()
{
	if (!m_bRunning)
		return FALSE; // 线程未运行

	// 设置停止标志
	m_bStop = TRUE;

	// 等待线程结束（最多等待2秒）
	if (m_hThread)
	{
		DWORD dwWaitResult = WaitForSingleObject(m_hThread, 2000);
		if (dwWaitResult == WAIT_TIMEOUT)
		{
			// 线程超时未结束，强制终止
			TerminateThread(m_hThread, 0);
		}

		CloseHandle(m_hThread);
		m_hThread = nullptr;
	}

	m_bRunning = FALSE;
	return TRUE;
}

BOOL Worker::PauseThread()
{
	if (!m_bRunning) return FALSE;
	m_bPause = TRUE;
	return TRUE;
}

BOOL Worker::ContinueThread()
{
	if (!m_bRunning) return FALSE;
	m_bPause = FALSE;
	return TRUE;
}

BOOL Worker::IsRunning() const
{
	return m_bRunning;
}

UINT Worker::function(LPVOID pParam)
{
	Worker* pThis = static_cast<Worker*>(pParam);
	if (!pThis || !pThis->m_pParent)
		return 1;
	for (int i = 0;i < 100;i++) {
		if (pThis->m_bStop)
		{
			break;
		}
		while (pThis->m_bPause)
		{
			Sleep(50);
		}
		if (::IsWindow(pThis->m_pParent->GetSafeHwnd()))
		{
			pThis->m_pParent->PostMessage(WM_THREAD_UPDATE, (WPARAM)i, 0);
		}
		// 模拟工作耗时
		Sleep(50);
	}
	if (::IsWindow(pThis->m_pParent->GetSafeHwnd())) {
		pThis->m_pParent->PostMessage(WM_THREAD_UPDATE, (WPARAM)100, 1);
	}
	pThis->m_bRunning = FALSE;
	return 0;
}
