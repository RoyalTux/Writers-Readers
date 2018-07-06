#include "Writers_Readers.h"

Writers_Readers* Writers_Readers::ptr = NULL;
int index = 0;

Writers_Readers::Writers_Readers(void)
{
	ptr = this;
}

Writers_Readers::~Writers_Readers(void)
{

}

void Writers_Readers::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL Writers_Readers::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hButton1 = GetDlgItem(hwnd, IDC_START);
	hButton2 = GetDlgItem(hwnd, IDC_FINISH);
	EnableWindow(hButton2, FALSE);
	return TRUE;
}

DWORD WINAPI READER(LPVOID lp)
{
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));
	if (WaitForSingleObject(hMutex, INFINITE) == WAIT_OBJECT_0)
	{
		HANDLE h = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("{41B4DBD4-F00A-4999-BFA9-1A20D12591B1}"));
		if (WaitForSingleObject(h, INFINITE) == WAIT_OBJECT_0)
		{
			TCHAR str1[100];
			wsprintf(str1, TEXT("Copy%d.txt"), index);
			wifstream r(TEXT("Original.txt"));
			if (!r.is_open())
			{
				return 1;
			}
			index++;
			while (!r.eof())
			{
				Sleep(50);
				TCHAR str[3];
				r >> str;
				wofstream wr(str1, ios_base::app);
				wr << str << ' ';
				wr.close();
			}
			r.close();
			ReleaseSemaphore(h, 1, NULL);
		}
		CloseHandle(h);
		ReleaseMutex(hMutex);
	}
	return 1;
}
DWORD WINAPI WRITER(LPVOID lp)
{
	srand(time(0));
	//HANDLE h = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("{41B4DBD4-F00A-4999-BFA9-1A20D12591B1}"));
	//HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));
	HANDLE hMutex = CreateMutex(0, FALSE, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));
	if (WaitForSingleObject(hMutex, INFINITE) == WAIT_OBJECT_0)
	{
		int a = 0;
		wofstream wr(TEXT("Original.txt"), ios_base::app);
		if (!wr.is_open())
		{
			return 1;
		}
		TCHAR str[20];
		for (int i = 0; i <= 10; i++)
		{
			a = rand() % 100;
			wsprintf(str, TEXT("%d"), a);
			wr << str << ' ';
		}
		wr.close();
		ReleaseMutex(hMutex);
	}	
	//CloseHandle(h);	
	return 1;
}
void Writers_Readers::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_START)
	{
		HANDLE hSemaphore = CreateSemaphore(NULL, 3, 3, TEXT("{41B4DBD4-F00A-4999-BFA9-1A20D12591B1}"));
		//HANDLE hEvent = CreateMutex(0, FALSE, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));
		HANDLE hThread = CreateThread(NULL, 0, WRITER, 0, 0, NULL);
		CloseHandle(hThread);
		SetTimer(hwnd, 1, 5000, NULL);
		SetTimer(hwnd, 2, 2000, TimerProc2);		
			hThread = CreateThread(NULL, 0, READER, 0, 0, NULL);
			CloseHandle(hThread);
			hThread = CreateThread(NULL, 0, READER, 0, 0, NULL);
			CloseHandle(hThread);
			hThread = CreateThread(NULL, 0, READER, 0, 0, NULL);
			CloseHandle(hThread);
		EnableWindow(hButton1, FALSE);
		EnableWindow(hButton2, TRUE);	
	}
	else if (id == IDC_FINISH)
	{
		EnableWindow(hButton1, TRUE);
		EnableWindow(hButton2, FALSE);
		KillTimer(hwnd, 1);
	}
	else if (id == IDC_STOP)
	{
		EnableWindow(hButton1, TRUE);
		EnableWindow(hButton2, FALSE);
		KillTimer(hwnd, 2);
	}
}
void Writers_Readers::Cls_OnTimer1(HWND hwnd, UINT id)
{
	//HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));
	HANDLE hThread = CreateThread(NULL, 0, WRITER, 0, 0, NULL);
	CloseHandle(hThread);	
}
VOID CALLBACK Writers_Readers::TimerProc2(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	HANDLE hThread = CreateThread(NULL, 0, READER, 0, 0, NULL);
	CloseHandle(hThread);
	hThread = CreateThread(NULL, 0, READER, 0, 0, NULL);
	CloseHandle(hThread);
	hThread = CreateThread(NULL, 0, READER, 0, 0, NULL);
	CloseHandle(hThread);
	
}
BOOL CALLBACK Writers_Readers::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer1);
	}
	return FALSE;
}
