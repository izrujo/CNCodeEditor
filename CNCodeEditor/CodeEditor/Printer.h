#ifndef _PRINTER_H
#define _PRINTER_H

#include <afxwin.h>

typedef signed long int Long;

class CodeEditingForm;
class PrintInformation;

class Printer {
public:
	Printer(CodeEditingForm* codeEditingForm = 0, PrintInformation *printInformation = 0);
	Printer(const Printer& source);
	~Printer();
	Printer& operator=(const Printer& source);

	void Print();

private:
	CodeEditingForm* codeEditingForm;
	PrintInformation* printInformation;
};


#endif //_PRINTER_H