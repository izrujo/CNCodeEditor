#include "CodeEditor.h"
#include "CommandFactory.h"
#include "Commands.h"
#include "Document.h"
#include "PrintJobManager.h"
#include "PrintStateDialog.h"
#include "OutputForm.h"
#include "CodeNumberingForm.h"
#include "CodeEditingForm.h"
#include "TokenBook.h"
#include "TokenVisitors.h"

#include "../TextEditor/Glyph.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/ScrollController.h"
#include "../TextEditor/CNTKeyActionFactory.h"
#include "../TextEditor/CNTKeyActions.h"

#include "resource.h"
#include "../TextEditor/resource.h"

#include <WinUser.h>
#include <imm.h>
#include <afxdlgs.h>

BEGIN_MESSAGE_MAP(CodeEditor, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND_RANGE(IDM_FILE_NEW, IDM_FORMAT_THEMA, OnCommandRange)
	ON_COMMAND_RANGE(IDM_FILE_CLOSE, IDC_REPORT_DIRTY, OnSimpleCommandRange)
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_THREADNOTIFY, OnThreadNotify)
	ON_WM_NCPAINT()
	//ON_UPDATE_COMMAND_UI_RANGE(IDM_FORMAT_WORDWRAP, IDM_FORMAT_WORDWRAP, OnUpdateCommandUIRange)
END_MESSAGE_MAP()

CodeEditor::CodeEditor() {
	this->codeEditingForm = NULL;
	this->document = NULL;
	this->printJobManager = NULL;
	this->printStateDialog = NULL;
	this->outputForm = NULL;
	this->codeNumberingForm = NULL;

	this->isCompiled = FALSE;
	this->isLinked = FALSE;
}

int CodeEditor::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	CRect rect;
	this->GetClientRect(rect);

	Long textEditingFormWidth = rect.Width() - CODENUMBERFORMWIDTH;

	rect.left += CODENUMBERFORMWIDTH;
	rect.right = rect.left + textEditingFormWidth;
	this->codeEditingForm = new CodeEditingForm;
	this->codeEditingForm->Create(NULL, "CNTextEditor", WS_CHILD,
		rect, this, NULL, NULL);
	this->codeEditingForm->ShowWindow(SW_SHOW);
	this->codeEditingForm->UpdateWindow();
	this->codeEditingForm->SetFocus();
	this->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_UNLOCKHISTORYBOOK, 0));
	this->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_UNLOCKFINDREPLACEDIALOG, 0));

	rect.left -= CODENUMBERFORMWIDTH;
	rect.right = rect.left + CODENUMBERFORMWIDTH - 2;
	rect.bottom -= 26; //수평 스크롤 바 높이.
	this->codeNumberingForm = new CodeNumberingForm(this);
	this->codeNumberingForm->Create(NULL, "CodeNumber", WS_CHILD,
		rect, this, NULL, NULL);
	this->codeNumberingForm->ShowWindow(SW_SHOW);
	this->codeNumberingForm->UpdateWindow();

	this->menu.LoadMenuA(IDR_MENU1);
	this->SetMenu(&this->menu);
	MENUINFO menuInfo = {};
	menuInfo.cbSize = sizeof(MENUINFO);
	BOOL ret = this->menu.GetMenuInfo(&menuInfo);
	DWORD error = GetLastError();
	menuInfo.fMask = MIM_BACKGROUND;
	menuInfo.hbrBack = ::CreateSolidBrush(RGB(30, 30, 30));
	ret = this->menu.SetMenuInfo(&menuInfo);

	this->document = new Document(this);

	this->printJobManager = new PrintJobManager(this);

	return 0;
}

void CodeEditor::OnClose() {
	if (this->outputForm != NULL) {
		this->outputForm->SendMessage(WM_CLOSE);
	}
	if (this->codeEditingForm != NULL) {
		delete this->codeEditingForm;
		this->codeEditingForm = NULL;
	}
	if (this->codeNumberingForm != NULL) {
		delete this->codeNumberingForm;
		this->codeNumberingForm = NULL;
	}
	if (this->document != NULL) {
		delete this->document;
	}
	if (this->printJobManager != NULL) {
		delete this->printJobManager;
	}
	if (this->printStateDialog != NULL) {
		delete this->printStateDialog;
	}

	CFrameWnd::OnClose();
}

void CodeEditor::OnPaint() {
	CPaintDC dc(this);

	CRect rect;
	this->GetClientRect(rect);
	if (this->outputForm != NULL) {
		rect.bottom = rect.bottom - (rect.Height() / 4);
	}

	dc.FillSolidRect(rect, RGB(235, 235, 235));
}

void CodeEditor::OnSize(UINT nType, int cx, int cy) {
	CRect rect;
	this->GetClientRect(rect);

	if (this->codeEditingForm != NULL && this->codeNumberingForm != NULL) {
		if (this->outputForm != NULL) {
			CRect outputRect = rect;
			outputRect.top += (outputRect.Height() / 4) * 3;
			this->outputForm->MoveWindow(outputRect);
			rect.bottom = outputRect.top;

		}
		Long width = rect.Width() - CODENUMBERFORMWIDTH;
		rect.left += CODENUMBERFORMWIDTH;
		rect.right = rect.left + width;
		this->codeEditingForm->MoveWindow(rect);

		rect.left -= CODENUMBERFORMWIDTH;
		rect.right = rect.left + CODENUMBERFORMWIDTH - 2;
		rect.bottom -= 26; //수평 스크롤 바 높이.
		this->codeNumberingForm->MoveWindow(rect);
	}

	CFrameWnd::OnSize(nType, cx, cy);
}

void CodeEditor::OnSetFocus(CWnd* pOldWnd) {
	CFrameWnd::OnSetFocus(pOldWnd);
	if (this->codeEditingForm != NULL) {
		this->codeEditingForm->SetFocus();
	}
}

void CodeEditor::OnKillFocus(CWnd* pNewWnd) {
	CFrameWnd::OnKillFocus(pNewWnd);
}

void CodeEditor::OnCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		delete command;
	}

	if (this->codeEditingForm->tokenBook != NULL) {
		delete this->codeEditingForm->tokenBook;
	}
	this->codeEditingForm->tokenBook = new TokenBook;
	Visitor* tokenMakingVisitor = new TokenMakingVisitor(this->codeEditingForm->tokenBook, this->codeEditingForm->tokenFactory);

	this->codeEditingForm->note->Accept(tokenMakingVisitor);

	if (tokenMakingVisitor != NULL) {
		delete tokenMakingVisitor;
	}

	if (this->codeEditingForm->scrollController != NULL) {
		delete this->codeEditingForm->scrollController;
	}
	this->codeEditingForm->scrollController = new ScrollController(this->codeEditingForm);

	this->codeEditingForm->Notify();
	this->codeEditingForm->Invalidate();

	Long x = this->codeEditingForm->characterMetrics->GetX(this->codeEditingForm->current) + 1; // 
	Long y = this->codeEditingForm->characterMetrics->GetY(this->codeEditingForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->codeEditingForm->scrollController->SmartScrollToPoint(x, y);
}

void CodeEditor::OnSimpleCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		delete command;
	}
}

void CodeEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CNTKeyActionFactory keyActionFactory(this->codeEditingForm);
	CNTKeyAction* keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);

		delete keyAction;
	}
	this->codeEditingForm->Notify();
	this->codeEditingForm->Invalidate();
}

LRESULT CodeEditor::OnThreadNotify(WPARAM wParam, LPARAM lParam) {
	if (this->printStateDialog != NULL) {
		this->printStateDialog->DestroyWindow();
		this->printStateDialog = NULL;
	}

	return 0;
}