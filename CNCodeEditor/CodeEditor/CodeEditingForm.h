#ifndef _CODEEDITINGFORM_H
#define _CODEEDITINGFORM_H

#include <afxwin.h>
#include <iostream>

using namespace std;

#define WM_THREADNOTIFY (WM_APP + 2000)

class TextEditingForm;
class Document;
class PrintJobManager;
class PrintStateDialog;

class CodeEditingForm : public CFrameWnd {
public:
	CodeEditingForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	TextEditingForm* textEditingForm;
	CMenu menu;
	Document* document;
	PrintJobManager* printJobManager;
	PrintStateDialog* printStateDialog;

protected:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCommandRange(UINT uID);
	afx_msg void OnSimpleCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnThreadNotify(WPARAM wParaml, LPARAM lParam);
	//afx_msg void OnUpdateCommandUIRange(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

};

#endif //_CODEEDITINGFORM_H