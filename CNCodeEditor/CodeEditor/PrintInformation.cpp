#include "PrintInformation.h"
#include "CodeEditingForm.h"
#include "Document.h"
#include "Book.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/Font.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/DummyManager.h"

#include <afxdlgs.h>

PrintInformation::PrintInformation(CodeEditingForm* codeEditingForm, Glyph* note) {
	this->printerDC.CreateDC("WINSPOOL", (LPCTSTR)codeEditingForm->document->deviceMode->dmDeviceName,
		NULL, codeEditingForm->document->deviceMode);

	Long dpi = this->printerDC.GetDeviceCaps(LOGPIXELSX);

	LOGFONT logFont = codeEditingForm->textEditingForm->font->GetFont();
	logFont.lfHeight = logFont.lfHeight * dpi / 144;
	logFont.lfWidth = logFont.lfWidth * dpi / 144;
	this->font = new Font(logFont, codeEditingForm->textEditingForm->font->GetColor(), codeEditingForm);

	this->characterMetrics = new CharacterMetrics(codeEditingForm, this->font);

	Long deviceWidth = this->printerDC.GetDeviceCaps(PHYSICALWIDTH);
	Long deviceHeight = this->printerDC.GetDeviceCaps(PHYSICALHEIGHT);
	CRect deviceMargin = codeEditingForm->document->GetMargins();
	float milimeterPerInch = 25.4F;
	deviceMargin.left = deviceMargin.left * (dpi / milimeterPerInch);
	deviceMargin.top = deviceMargin.top * (dpi / milimeterPerInch);
	deviceMargin.right = deviceMargin.right * (dpi / milimeterPerInch);
	deviceMargin.bottom = deviceMargin.bottom * (dpi / milimeterPerInch);
	Long top = deviceMargin.top;
	Long bottom = deviceMargin.bottom;

	string header = codeEditingForm->document->GetHeader();
	if (header != "") {
		top += this->characterMetrics->GetHeight();
	}
	string footer = codeEditingForm->document->GetFooter();
	if (footer != "") {
		bottom += this->characterMetrics->GetHeight();
	}

	DummyManager dummyManager(note, this->characterMetrics,
		deviceWidth - (deviceMargin.left + deviceMargin.right));
	this->book = new Book;

	Long i = 0;
	while (i < note->GetLength()) {
		i = dummyManager.Fold(i);
		i++;
	}

	Glyph* nextNote;
	Long lineHeight = this->characterMetrics->GetHeight();
	Long dcHeight = deviceHeight - (top + bottom) - lineHeight;
	Long height;
	while (note->GetLength() > 0) {
		height = 0;
		i = 0;
		while (i < note->GetLength() && height < dcHeight) {
			height += lineHeight;
			i++;
		}
		nextNote = note->Divide(i);
		this->book->Add(note);
		note = nextNote;
	}
	if (note != NULL) {
		delete note;
	}
	this->book->Move(0);
}

PrintInformation::~PrintInformation() {
	if (this->book != NULL) {
		delete this->book;
	}
	if (this->font != NULL) {
		delete this->font;
	}
	if (this->characterMetrics != NULL) {
		delete this->characterMetrics;
	}
	if (this->printerDC != NULL) {
		this->printerDC.DeleteDC();
	}
}