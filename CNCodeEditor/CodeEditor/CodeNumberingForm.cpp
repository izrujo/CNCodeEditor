#include "CodeNumberingForm.h"
#include "CodeEditor.h"
#include "UIGraphs.h"
#include "UIGraphFactory.h"
#include "GraphVisitors.h"
#include "resource.h"
#include "CodeEditingForm.h"

#include "../TextEditor/Glyph.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/Font.h"

BEGIN_MESSAGE_MAP(CodeNumberingForm, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CodeNumberingForm::CodeNumberingForm(CWnd* parent) {
	this->parent = parent;
	this->numberStick = NULL;
}

int CodeNumberingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	CRect rect;
	this->GetClientRect(rect);

	this->numberStick = new NumberStick;

	UIGraphFactory graphFactory;

	CodeEditor* codeEditor = static_cast<CodeEditor*>(this->parent);
	Long y = rect.top;
	Long fontHeight = codeEditor->codeEditingForm->characterMetrics->GetHeight();
	string number;
	UIGraph* lineNumber;
	Long i = 1;
	while (i <= codeEditor->codeEditingForm->note->GetLength()) {
		number = to_string(i);
		lineNumber = graphFactory.Make(LINENUMBER, rect.left, y, rect.Width(), fontHeight, number);
		this->numberStick->Add(lineNumber);
		y += fontHeight;
		i++;
	}

	return 0;
}

void CodeNumberingForm::OnClose() {
	if (this->numberStick != NULL) {
		delete this->numberStick;
	}

	CWnd::OnClose();
}

void CodeNumberingForm::OnPaint() {
	CPaintDC dc(this);

	CRect rect;
	this->GetClientRect(rect);

	CodeEditingForm* codeEditingForm = static_cast<CodeEditor*>(this->parent)->codeEditingForm;

	dc.FillSolidRect(&rect, codeEditingForm->backgroundColor);

	CFont* oldFont;
	CFont font;
	codeEditingForm->font->Create(font);
	oldFont = dc.SelectObject(&font);
	COLORREF textColor = codeEditingForm->font->GetColor();
	COLORREF oldColor = dc.SetTextColor(textColor);


	GraphVisitor* visitor = new GraphDrawingVisitor(&dc, codeEditingForm->scrollController);
	this->numberStick->Accept(visitor);

	dc.SetTextColor(oldColor);
	dc.SelectObject(oldFont);

	if (visitor != NULL) {
		delete visitor;
	}
}

void CodeNumberingForm::OnSize(UINT nType, int cx, int cy) {
	CRect rect;
	this->GetClientRect(rect);

	CWnd::OnSize(nType, cx, cy);
}

BOOL CodeNumberingForm::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void CodeNumberingForm::UpdateNumber() {
	CRect rect;
	this->GetClientRect(rect);

	if (this->numberStick != NULL) {
		delete this->numberStick;
	}
	this->numberStick = new NumberStick;

	UIGraphFactory graphFactory;

	CodeEditor* codeEditor = static_cast<CodeEditor*>(this->parent);
	Long y = rect.top;
	Long fontHeight = codeEditor->codeEditingForm->characterMetrics->GetHeight();
	string number;
	UIGraph* lineNumber;
	Long i = 1;
	while (i <= codeEditor->codeEditingForm->note->GetLength()) {
		number = to_string(i);
		lineNumber = graphFactory.Make(LINENUMBER, rect.left, y, rect.Width(), fontHeight, number);
		this->numberStick->Add(lineNumber);
		y += fontHeight;
		i++;
	}
	this->Invalidate();
}

Long CodeNumberingForm::AddNumber(Long number) {
	CodeEditor* codeEditor = static_cast<CodeEditor*>(this->parent);
	UIGraphFactory graphFactory;
	UIGraph* last = this->numberStick->GetAt(this->numberStick->GetLength() - 1);

	Long y = last->GetY() + last->GetHeight();
	string num = to_string(number);
	UIGraph* addingNumber = graphFactory.Make(LINENUMBER, last->GetX(), y, last->GetWidth(), last->GetHeight(), num);
	Long index = this->numberStick->Add(addingNumber);

	this->Invalidate();

	return index;
}

Long CodeNumberingForm::RemoveNumber() {
	Long index = this->numberStick->Remove(this->numberStick->GetLength() - 1);

	this->Invalidate();

	return index;
}