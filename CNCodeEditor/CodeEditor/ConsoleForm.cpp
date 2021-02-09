#include "ConsoleForm.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/Visitor.h"
#include "../TextEditor/DrawingVisitor.h"
#include "../TextEditor/Font.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/ScrollController.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Scanner.h"

BEGIN_MESSAGE_MAP(ConsoleForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

ConsoleForm::ConsoleForm(CWnd* parent, string result, string exeName) {
	this->parent = parent;
	this->textEditingForm = NULL;
	this->result = result;
	this->exeName = exeName;
}

int ConsoleForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	CRect rect;
	this->GetClientRect(rect);

	this->textEditingForm = new TextEditingForm;
	this->textEditingForm->Create(NULL, "CNTextEditor", WS_CHILD,
		rect, this, NULL, NULL);
	this->textEditingForm->ShowWindow(SW_SHOW);
	this->textEditingForm->UpdateWindow();
	this->textEditingForm->SetFocus();

	GlyphFactory glyphFactory;
	
	this->result += "\r\nPress any key to close this window...";

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

	Long caretIndex = this->textEditingForm->note->Last();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(caretIndex);
	this->textEditingForm->current->Last();

	return 0;
}

void ConsoleForm::OnClose() {
	if (this->textEditingForm != NULL) {
		delete this->textEditingForm;
		this->textEditingForm = NULL;
	}
	//exe파일 지워주기
	string cmd = "del " + this->exeName + ".exe";
	system(cmd.c_str());
	cmd = "del outTemp.txt";
	system(cmd.c_str());

	CFrameWnd::OnClose();
}

void ConsoleForm::OnSize(UINT nType, int cx, int cy) {
	CRect rect;
	this->GetClientRect(rect);

	if (this->textEditingForm != NULL) {
		this->textEditingForm->MoveWindow(rect);
	}

	CFrameWnd::OnSize(nType, cx, cy);
}

void ConsoleForm::OnSetFocus(CWnd* pOldWnd) {
	CFrameWnd::OnSetFocus(pOldWnd);
	this->textEditingForm->SetFocus();
}

void ConsoleForm::OnKillFocus(CWnd* pNewWnd) {
	CFrameWnd::OnKillFocus(pNewWnd);
}