#ifndef _CODEEDITOR_H
#define _CODEEDITOR_H

#include <afxwin.h>
#include <iostream>

using namespace std;

#define WM_THREADNOTIFY (WM_APP + 2000)

class CodeEditingForm;
class Document;
class PrintJobManager;
class PrintStateDialog;
class OutputForm;
class CodeNumberingForm;

class CodeEditor : public CFrameWnd {
public:
	CodeEditor();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	CodeEditingForm* codeEditingForm;
	CMenu menu;
	Document* document;
	PrintJobManager* printJobManager;
	PrintStateDialog* printStateDialog;
	OutputForm* outputForm;
	CodeNumberingForm* codeNumberingForm;

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCommandRange(UINT uID);
	afx_msg void OnSimpleCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnThreadNotify(WPARAM wParaml, LPARAM lParam);
	//afx_msg void OnUpdateCommandUIRange(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	BOOL GetIsCompiled() const;
	void SetIsCompiled(BOOL isCompiled);
	BOOL GetIsLinked() const;
	void SetIsLinked(BOOL isLinked);
private:
	BOOL isCompiled;
	BOOL isLinked;
};

inline BOOL CodeEditor::GetIsCompiled() const {
	return this->isCompiled;
}

inline void CodeEditor::SetIsCompiled(BOOL isCompiled) {
	this->isCompiled = isCompiled;
}

inline BOOL CodeEditor::GetIsLinked() const {
	return this->isLinked;
}

inline void CodeEditor::SetIsLinked(BOOL isLinked) {
	this->isLinked = isLinked;
}

#endif //_CODEEDITOR_H