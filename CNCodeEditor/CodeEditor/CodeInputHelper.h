#ifndef _CODEINPUTHELPER_H
#define _CODEINPUTHELPER_H

class CodeEditingForm;

class CodeInputHelper {
public:
	CodeInputHelper(CodeEditingForm* codeEditingForm = 0);
	CodeInputHelper(const CodeInputHelper& source);
	~CodeInputHelper();
	CodeInputHelper& operator=(const CodeInputHelper& source);

	void Help();

private:
	CodeEditingForm* codeEditingForm;
};

#endif //_CODEINPUTHELPER_H