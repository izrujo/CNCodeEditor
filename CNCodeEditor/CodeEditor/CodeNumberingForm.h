#ifndef _CODENUMBERINGFORM_H
#define _CODENUMBERINGFORM_H

#include <afxwin.h>

#define CODENUMBERFORMWIDTH 50

typedef signed long int Long;

class UIGraph;

class CodeNumberingForm : public CWnd {
public:
	CodeNumberingForm(CWnd* parent = NULL);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void UpdateNumber();
	Long AddNumber(Long number);
	Long RemoveNumber();

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

public:
	CWnd* parent;
	UIGraph* numberStick;
};

#endif //_CODENUMBERINGFORM_H