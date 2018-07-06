#pragma once
#include "Header.h"
class Writers_Readers
{
public:
	Writers_Readers(void);
public:
	~Writers_Readers(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static VOID CALLBACK Writers_Readers::TimerProc2(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static Writers_Readers* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnTimer1(HWND hwnd, UINT id);
	HWND hButton1, hButton2, hButton3;
};

