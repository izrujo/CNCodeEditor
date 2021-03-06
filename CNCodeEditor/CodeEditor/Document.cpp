#include "Document.h"
#include "CodeEditor.h"
#include <afxdlgs.h>

Document::Document(CodeEditor* codeEditor) 
	: pathName("���� ����"), encodingType("ANSI"), 
	/*paperSize(21000, 29700),*/ margins(20, 25, 20, 25), header(""), footer("") {
	this->codeEditor = codeEditor;
	this->isDirty = false;
	this->isVertical = true;

	CPrintDialog printDialog(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION | PD_RETURNDEFAULT, codeEditor);
	printDialog.GetDefaults();
	this->deviceMode = printDialog.GetDevMode();
}

Document::Document(const Document& source) 
	: pathName(source.pathName), encodingType(source.encodingType), 
	/*paperSize(source.paperSize),*/ margins(source.margins), header(source.header), footer(source.footer) {
	this->codeEditor = source.codeEditor;
	this->isDirty = source.isDirty;
	this->isVertical = source.isVertical;
	this->deviceMode = source.deviceMode;
}

Document::~Document() {
}

Document& Document::operator=(const Document& source) {
	this->codeEditor = source.codeEditor;
	this->isDirty = source.isDirty;
	this->pathName = source.pathName;
	this->encodingType = source.encodingType;
	//this->paperSize = source.paperSize;
	this->isVertical = source.isVertical;
	this->margins = source.margins;
	this->header = source.header;
	this->footer = source.footer;
	this->deviceMode = source.deviceMode;

	return *this;
}

void Document::SetIsDirty(bool isDirty) {
	this->isDirty = isDirty;
}

void Document::SetPathName(string pathName) {
	this->pathName = pathName;
}

void Document::SetEncodingType(string encodingType) {
	this->encodingType = encodingType;
}

//void Document::SetPaperSize(CSize paperSize) {
//	this->paperSize = paperSize;
//}

void Document::SetIsVertical(bool isVertical) {
	this->isVertical = isVertical;
}

void Document::SetMargins(CRect margins) {
	this->margins = margins;
}

void Document::SetHeader(string header) {
	this->header = header;
}

void Document::SetFooter(string footer) {
	this->footer = footer;
}