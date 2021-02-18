#ifndef _KEYACTIONS_H
#define _KEYACTIONS_H

#include <afxwin.h>

class CodeEditingForm;

class KeyAction {
public:
	KeyAction(CodeEditingForm* codeEditingForm = 0);
	KeyAction(const KeyAction& source);
	virtual ~KeyAction() = 0;
	KeyAction& operator=(const KeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
protected:
	CodeEditingForm* codeEditingForm;
};

class CtrlNKeyAction : public KeyAction {
public:
	CtrlNKeyAction(CodeEditingForm* codeEditingForm = 0);
	CtrlNKeyAction(const CtrlNKeyAction& source);
	virtual ~CtrlNKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlNKeyAction& operator =(const CtrlNKeyAction& source);
};

class CtrlOKeyAction : public KeyAction {
public:
	CtrlOKeyAction(CodeEditingForm* codeEditingForm = 0);
	CtrlOKeyAction(const CtrlOKeyAction& source);
	virtual ~CtrlOKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlOKeyAction& operator =(const CtrlOKeyAction& source);
};

class CtrlSKeyAction : public KeyAction {
public:
	CtrlSKeyAction(CodeEditingForm* codeEditingForm = 0);
	CtrlSKeyAction(const CtrlSKeyAction& source);
	virtual ~CtrlSKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlSKeyAction& operator =(const CtrlSKeyAction& source);
};

class CtrlShiftSKeyAction : public KeyAction {
public:
	CtrlShiftSKeyAction(CodeEditingForm* codeEditingForm = 0);
	CtrlShiftSKeyAction(const CtrlShiftSKeyAction& source);
	virtual ~CtrlShiftSKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlShiftSKeyAction& operator =(const CtrlShiftSKeyAction& source);
};

class CtrlPKeyAction : public KeyAction {
public:
	CtrlPKeyAction(CodeEditingForm* codeEditingForm = 0);
	CtrlPKeyAction(const CtrlPKeyAction& source);
	virtual ~CtrlPKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlPKeyAction& operator =(const CtrlPKeyAction& source);
};

//CtrlF5KeyAction
class CtrlF5KeyAction : public KeyAction {
public:
	CtrlF5KeyAction(CodeEditingForm* codeEditingForm = 0);
	CtrlF5KeyAction(const CtrlF5KeyAction& source);
	virtual ~CtrlF5KeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlF5KeyAction& operator =(const CtrlF5KeyAction& source);
};

//CtrlF7KeyAction
class CtrlF7KeyAction : public KeyAction {
public:
	CtrlF7KeyAction(CodeEditingForm* codeEditingForm = 0);
	CtrlF7KeyAction(const CtrlF7KeyAction& source);
	virtual ~CtrlF7KeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlF7KeyAction& operator =(const CtrlF7KeyAction& source);
};

#endif //_KEYACTIONS_H