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
	afx_msg void OnCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

};

#endif //_CODEEDITINGFORM_H