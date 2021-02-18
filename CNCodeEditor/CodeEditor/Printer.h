#ifndef _PRINTER_H
#define _PRINTER_H

#include <afxwin.h>

typedef signed long int Long;

class CodeEditor;
class PrintInformation;

class Printer {
public:
	Printer(CodeEditor* codeEditor = 0, PrintInformation *printInformation = 0);
	Printer(const Printer& source);
	~Printer();
	Printer& operator=(const Printer& source);

	void Print();

private:
	CodeEditor* codeEditor;
	PrintInformation* printInformation;
};


#endif //_PRINTER_H