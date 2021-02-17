#ifndef _OUTPUTFORM_H
#define _OUTPUTFORM_H

#include <afxwin.h>

#include <string>

using namespace std;

class TextEditingForm;
class UIGraph;

class OutputForm : public CWnd {
public:
	OutputForm(CWnd* parent = NULL, string result = 0, string resultFile = 0);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void AppendResult(string result, string resultFile);

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	CWnd* parent;
	TextEditingForm* textEditingForm;
	string result;
	string resultFile;
	string resultFile2;
	UIGraph* windowCaption;
	UIGraph* windowCloseButton;
};

#endif //_OUTPUTFORM_H