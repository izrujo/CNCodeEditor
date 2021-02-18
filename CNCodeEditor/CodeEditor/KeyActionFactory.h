#ifndef _KEYACTIONFACTORY_H
#define _KEYACTIONFACTORY_H

#include <afxwin.h>

class KeyAction;
class CodeEditingForm;

class KeyActionFactory {
public:
	KeyActionFactory(CodeEditingForm* codeEditingForm = 0);
	KeyActionFactory(const KeyActionFactory& source);
	~KeyActionFactory();
	KeyActionFactory& operator=(const KeyActionFactory& source);

	KeyAction* Make(UINT nChar);
private:
	CodeEditingForm* codeEditingForm;
};

#endif // _KEYACTIONFACTORY_H