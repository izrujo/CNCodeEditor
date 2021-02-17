#include "OutputForm.h"
#include "CodeEditingForm.h"
#include "UIGraphs.h"
#include "UIGraphFactory.h"
#include "GraphVisitors.h"
#include "Files.h"
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
	static_cast<CodeEditingForm*>(this->parent)->textEditingForm->MoveWindow(rect);
	static_cast<CodeEditingForm*>(this->parent)->outputForm = NULL;

	CWnd::OnClose();
}

void OutputForm::OnPaint() {
	CPaintDC dc(this);

	GraphVisitor* visitor = new GraphDrawingVisitor(&dc);
	this->windowCaption->Accept(visitor);
	this->windowCloseButton->Accept(visitor);

	if (visitor != NULL) {
		delete visitor;
	}
}

void OutputForm::OnSize(UINT nType, int cx, int cy) {
	CRect rect;
	this->GetClientRect(rect);

	if (this->textEditingForm != NULL) {
		rect.top += 30;

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

	Glyph* line= glyphFactory.Make("\r\n");
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

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLEND, 0));
}