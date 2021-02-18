#include "KeyActions.h"
#include "CodeEditor.h"
#include "CodeEditingForm.h"
#include "resource.h"

//KeyAction
KeyAction::KeyAction(CodeEditingForm* codeEditingForm) {
	this->codeEditingForm = codeEditingForm;
}

KeyAction::KeyAction(const KeyAction& source) {
	this->codeEditingForm = source.codeEditingForm;
}

KeyAction::~KeyAction() {

}

KeyAction& KeyAction::operator=(const KeyAction& source) {
	this->codeEditingForm = source.codeEditingForm;

	return *this;
}

//CtrlNKeyAction
CtrlNKeyAction::CtrlNKeyAction(CodeEditingForm* codeEditingForm)
	: KeyAction(codeEditingForm) {
}

CtrlNKeyAction::CtrlNKeyAction(const CtrlNKeyAction& source)
	: KeyAction(source) {
}

CtrlNKeyAction::~CtrlNKeyAction() {
}

void CtrlNKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CodeEditor* codeEditor = (CodeEditor*)this->codeEditingForm->GetParent();
	codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_NEW, 0));
}

CtrlNKeyAction& CtrlNKeyAction::operator =(const CtrlNKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

//CtrlOKeyAction
CtrlOKeyAction::CtrlOKeyAction(CodeEditingForm* codeEditingForm)
	: KeyAction(codeEditingForm) {
}

CtrlOKeyAction::CtrlOKeyAction(const CtrlOKeyAction& source)
	: KeyAction(source) {
}

CtrlOKeyAction::~CtrlOKeyAction() {
}

void CtrlOKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CodeEditor* codeEditor = (CodeEditor*)this->codeEditingForm->GetParent();
	codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_OPEN, 0));
}

CtrlOKeyAction& CtrlOKeyAction::operator =(const CtrlOKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

//CtrlSKeyAction
CtrlSKeyAction::CtrlSKeyAction(CodeEditingForm* codeEditingForm)
	: KeyAction(codeEditingForm) {
}

CtrlSKeyAction::CtrlSKeyAction(const CtrlSKeyAction& source)
	: KeyAction(source) {
}

CtrlSKeyAction::~CtrlSKeyAction() {
}

void CtrlSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CodeEditor* codeEditor = (CodeEditor*)this->codeEditingForm->GetParent();
	codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVE, 0));
}

CtrlSKeyAction& CtrlSKeyAction::operator =(const CtrlSKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

//CtrlShiftSKeyAction
CtrlShiftSKeyAction::CtrlShiftSKeyAction(CodeEditingForm* codeEditingForm)
	: KeyAction(codeEditingForm) {
}

CtrlShiftSKeyAction::CtrlShiftSKeyAction(const CtrlShiftSKeyAction& source)
	: KeyAction(source) {
}

CtrlShiftSKeyAction::~CtrlShiftSKeyAction() {
}

void CtrlShiftSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CodeEditor* codeEditor = (CodeEditor*)this->codeEditingForm->GetParent();
	codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVEAS, 0));
}

CtrlShiftSKeyAction& CtrlShiftSKeyAction::operator =(const CtrlShiftSKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

//CtrlPKeyAction
CtrlPKeyAction::CtrlPKeyAction(CodeEditingForm* codeEditingForm)
	: KeyAction(codeEditingForm) {
}

CtrlPKeyAction::CtrlPKeyAction(const CtrlPKeyAction& source)
	: KeyAction(source) {
}

CtrlPKeyAction::~CtrlPKeyAction() {
}

void CtrlPKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CodeEditor* codeEditor = (CodeEditor*)this->codeEditingForm->GetParent();
	codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_PRINT, 0));
}

CtrlPKeyAction& CtrlPKeyAction::operator =(const CtrlPKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

//CtrlF5KeyAction
CtrlF5KeyAction::CtrlF5KeyAction(CodeEditingForm* codeEditingForm)
	: KeyAction(codeEditingForm) {

}

CtrlF5KeyAction::CtrlF5KeyAction(const CtrlF5KeyAction& source)
	: KeyAction(source) {

}

CtrlF5KeyAction::~CtrlF5KeyAction() {

}

CtrlF5KeyAction& CtrlF5KeyAction::operator =(const CtrlF5KeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void CtrlF5KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CodeEditor* codeEditor = (CodeEditor*)this->codeEditingForm->GetParent();
	codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_C_LOAD, 0));
}

//CtrlF7KeyAction
CtrlF7KeyAction::CtrlF7KeyAction(CodeEditingForm* codeEditingForm)
	: KeyAction(codeEditingForm) {

}

CtrlF7KeyAction::CtrlF7KeyAction(const CtrlF7KeyAction& source)
	: KeyAction(source) {

}

CtrlF7KeyAction::~CtrlF7KeyAction() {

}

CtrlF7KeyAction& CtrlF7KeyAction::operator =(const CtrlF7KeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void CtrlF7KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CodeEditor* codeEditor = (CodeEditor*)this->codeEditingForm->GetParent();
	codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_C_COMPILE, 0));
}