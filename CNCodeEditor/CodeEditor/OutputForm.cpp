#include "OutputForm.h"
#include "CodeEditor.h"
#include "UIGraphs.h"
#include "UIGraphFactory.h"
#include "GraphVisitors.h"
#include "Files.h"
#include "CodeNumberingForm.h"
#include "CodeEditingForm.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/Visitor.h"
#include "../TextEditor/DrawingVisitor.h"
#include "../TextEditor/Font.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/ScrollController.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Scanner.h"
#include "../TextEditor/resource.h"

BEGIN_MESSAGE_MAP(OutputForm, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

OutputForm::OutputForm(CWnd* parent, string result, string resultFile) {
	this->parent = parent;
	this->textEditingForm = NULL;
	this->result = result;
	this->resultFile = resultFile;
	this->resultFile2 = "";
	this->windowCaption = NULL;
	this->windowCloseButton = NULL;
}

int OutputForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	CRect rect;
	this->GetClientRect(rect);
	Long height = 30;

	UIGraphFactory graphFactory;
	this->windowCaption = graphFactory.Make(WINDOWCAPTION, rect.left, rect.top, rect.Width() - height, height, "Output");
	this->windowCloseButton = graphFactory.Make(WINDOWCLOSEBUTTON, rect.right - height, rect.top, height, height);

	rect.top += height;

	this->textEditingForm = new TextEditingForm;
	this->textEditingForm->Create(NULL, "CNTextEditor", WS_CHILD,
		rect, this, NULL, NULL);
	this->textEditingForm->ShowWindow(SW_SHOW);
	this->textEditingForm->UpdateWindow();
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_LOCKHSCROLL, 0));

	GlyphFactory glyphFactory;

	//this->result += "\r\nPress any key to close this window...";

	Scanner scanner(this->result);
	while (scanner.IsEnd() == FALSE) {
		string token = scanner.GetToken();
		Glyph* glyph = glyphFactory.Make(token.c_str());
		if (token != "\r\n") {
			this->textEditingForm->current->Add(glyph);
		}
		else {
			Long index = this->textEditingForm->note->Add(glyph);
			this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
		}
		scanner.Next();
	}

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLEND, 0));

	CodeEditor* codeEditor = static_cast<CodeEditor*>(this->parent);
	this->textEditingForm->backgroundColor = codeEditor->codeEditingForm->backgroundColor;
	LOGFONT logFont = codeEditor->codeEditingForm->font->GetFont();
	COLORREF color = codeEditor->codeEditingForm->font->GetColor();
	if (this->textEditingForm->font != NULL) {
		delete this->textEditingForm->font;
	}
	this->textEditingForm->font = new Font(logFont, color, this->textEditingForm);
	this->textEditingForm->note->Paint(color);

	this->textEditingForm->SetWindowPos(this->textEditingForm, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE);

	return 0;
}

void OutputForm::OnClose() {
	if (this->textEditingForm != NULL) {
		delete this->textEditingForm;
		this->textEditingForm = NULL;
	}
	if (this->windowCaption != NULL) {
		delete this->windowCaption;
	}
	if (this->windowCloseButton != NULL) {
		delete this->windowCloseButton;
	}

	string cmd = "del " + this->resultFile;
	system(cmd.c_str());

	if (this->resultFile2 != "") {
		cmd = "del " + this->resultFile2;
		system(cmd.c_str());
	}

	CRect rect;
	this->parent->GetClientRect(rect);

	CodeEditor* codeEditor = static_cast<CodeEditor*>(this->parent);

	Long width = rect.Width() - CODENUMBERFORMWIDTH;
	rect.left += CODENUMBERFORMWIDTH;
	rect.right = rect.left + width;
	codeEditor->codeEditingForm->MoveWindow(rect);

	rect.left -= CODENUMBERFORMWIDTH;
	rect.right = rect.left + CODENUMBERFORMWIDTH - 2;
	rect.bottom -= 26; //수평 스크롤 바 높이.
	codeEditor->codeNumberingForm->MoveWindow(rect);

	codeEditor->outputForm = NULL;

	CWnd::OnClose();
}

void OutputForm::OnPaint() {
	CPaintDC dc(this);

	COLORREF oldbkColor = dc.SetBkColor(RGB(0, 122, 204));
	COLORREF oldTextColor = dc.SetTextColor(this->textEditingForm->font->GetColor());

	CFont* oldFont;
	CFont font;
	this->textEditingForm->font->Create(font);
	oldFont = dc.SelectObject(&font);

	GraphVisitor* visitor = new GraphDrawingVisitor(&dc);
	this->windowCaption->Accept(visitor);
	this->windowCloseButton->Accept(visitor);

	if (visitor != NULL) {
		delete visitor;
	}

	dc.SetBkColor(oldbkColor);
	dc.SetTextColor(oldTextColor);
}

void OutputForm::OnSize(UINT nType, int cx, int cy) {
	CRect rect;
	this->GetClientRect(rect);

	Long height = 30;
	UIGraphFactory graphFactory;
	if (this->windowCaption != NULL) {
		delete this->windowCaption;
		this->windowCaption = graphFactory.Make(WINDOWCAPTION, rect.left, rect.top, rect.Width() - height, height, "Output");
	}
	if (this->windowCloseButton != NULL) {
		delete this->windowCloseButton;
		this->windowCloseButton = graphFactory.Make(WINDOWCLOSEBUTTON, rect.right - height, rect.top, height, height);
	}

	if (this->textEditingForm != NULL) {
		rect.top += height;

		this->textEditingForm->MoveWindow(rect);
	}

	CWnd::OnSize(nType, cx, cy);
}

void OutputForm::OnSetFocus(CWnd* pOldWnd) {
	CWnd::OnSetFocus(pOldWnd);
	this->textEditingForm->SetFocus();
}

void OutputForm::OnKillFocus(CWnd* pNewWnd) {
	CWnd::OnKillFocus(pNewWnd);
}

void OutputForm::OnLButtonUp(UINT nFlags, CPoint point) {
	Long x = this->windowCloseButton->GetX();
	Long y = this->windowCloseButton->GetY();
	Long width = this->windowCloseButton->GetWidth();
	Long height = this->windowCloseButton->GetHeight();
	if (point.x > x && point.x < x + width && point.y >y && point.y < y + height) {
		this->SendMessage(WM_CLOSE);
	}
}

void OutputForm::AppendResult(string result, string resultFile) {
	this->resultFile2 = resultFile;

	GlyphFactory glyphFactory;

	Glyph* line = glyphFactory.Make("\r\n");
	this->textEditingForm->note->Add(line);
	line = glyphFactory.Make("\r\n");
	Long current = this->textEditingForm->note->Add(line);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(current);

	Scanner scanner(result);
	while (scanner.IsEnd() == FALSE) {
		string token = scanner.GetToken();
		Glyph* glyph = glyphFactory.Make(token.c_str());
		if (token != "\r\n") {
			this->textEditingForm->current->Add(glyph);
		}
		else {
			Long index = this->textEditingForm->note->Add(glyph);
			this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
		}
		scanner.Next();
	}

	this->textEditingForm->note->Paint(this->textEditingForm->font->GetColor());

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLEND, 0));

	CRect rect;
	this->textEditingForm->GetClientRect(rect);
	this->textEditingForm->SetWindowPos(this->textEditingForm, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE);
}