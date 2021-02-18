#include "KeyActionFactory.h"
#include "KeyActions.h"
#include "CodeEditingForm.h"
#include <WinUser.h>

KeyActionFactory::KeyActionFactory(CodeEditingForm* codeEditingForm) {
	this->codeEditingForm = codeEditingForm;
}

KeyActionFactory::KeyActionFactory(const KeyActionFactory& source) {
	this->codeEditingForm = source.codeEditingForm;
}

KeyActionFactory::~KeyActionFactory() {

}

KeyActionFactory& KeyActionFactory::operator=(const KeyActionFactory& source) {
	this->codeEditingForm = source.codeEditingForm;

	return *this;
}

KeyAction* KeyActionFactory::Make(UINT nChar) {
	KeyAction* keyAction = 0;

	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	SHORT isShift = GetKeyState(VK_SHIFT) & 0X8000;

	//Shift + Ctrl + KEY
	if (isShift && isCtrl && nChar == 0x53) { //추가 : SaveAs
		keyAction = new CtrlShiftSKeyAction(this->codeEditingForm);
	}
	//Ctrl + KEY
	else if (isCtrl && nChar == 0x4E) { //추가 : New
		keyAction = new CtrlNKeyAction(this->codeEditingForm);
	}
	else if (isCtrl && nChar == 0x4F) { //추가 : Open
		keyAction = new CtrlOKeyAction(this->codeEditingForm);
	}
	else if (isCtrl && nChar == 0x53) { //추가 : Save
		keyAction = new CtrlSKeyAction(this->codeEditingForm);
	}
	else if (isCtrl && nChar == 0x50) { //추가 : Print
		keyAction = new CtrlPKeyAction(this->codeEditingForm);
	}
	else if (isCtrl && nChar == 0x74) {
		keyAction = new CtrlF5KeyAction(this->codeEditingForm);
	}
	else if (isCtrl && nChar == 0x76) {
		keyAction = new CtrlF7KeyAction(this->codeEditingForm);
	}

	return keyAction;
}