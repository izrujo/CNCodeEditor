#ifndef _CODEEDITINGFORM_H
#define _CODEEDITINGFORM_H

#include "../TextEditor/TextEditingForm.h"

class CodeEditingForm : public TextEditingForm {
public:
	CodeEditingForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
protected:
	afx_msg void OnClose();
	afx_msg void OnEditCommandRange(UINT uID);
	afx_msg void OnMoveCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()

};

#endif //_CODEEDITINGFORM_H