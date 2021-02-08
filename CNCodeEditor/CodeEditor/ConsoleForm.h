#ifndef _CONSOLEFORM_H
#define _CONSOLEFORM_H

#include <afxwin.h>

#include <string>

using namespace std;

class TextEditingForm;

class ConsoleForm : public CFrameWnd {
public:
	ConsoleForm(CWnd* parent = NULL, string result = 0);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	DECLARE_MESSAGE_MAP()

public:
	CWnd* parent;
	TextEditingForm* textEditingForm;
	string result;
};

#endif //_CONSOLEFORM_H