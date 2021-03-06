#include "CodeEditingForm.h"
#include "CodeEditor.h"
#include "KeyActionFactory.h"
#include "KeyActions.h"
#include "CodeNumberingForm.h"
#include "TokenVisitors.h"
#include "TokenBook.h"
#include "TokenFactory.h"
#include "Block.h"
#include "CodeInputHelper.h"

#include "resource.h"

#include "../TextEditor/Glyph.h"
#include "../TextEditor/Font.h"
#include "../TextEditor/resource.h"

BEGIN_MESSAGE_MAP(CodeEditingForm, TextEditingForm)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_COMMAND_RANGE(IDCNT_EDIT_WRITE, IDCNT_EDIT_REDO, OnEditCommandRange)
	ON_COMMAND_RANGE(IDCNT_MOVE_LEFT, IDCNT_SELECTMOVE_CTRLEND, OnMoveCommandRange)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

CodeEditingForm::CodeEditingForm()
	: TextEditingForm() {
	this->tokenBook = NULL;
	this->tokenFactory = NULL;
	this->root = NULL;
	this->codeInputHelper = NULL;
}

int CodeEditingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	TextEditingForm::OnCreate(lpCreateStruct);

	this->tokenFactory = new TokenFactory;

	this->tokenBook = new TokenBook;
	Visitor* tokenMakingVisitor = new TokenMakingVisitor(this->tokenBook, this->tokenFactory);

	this->note->Accept(tokenMakingVisitor);

	if (tokenMakingVisitor != NULL) {
		delete tokenMakingVisitor;
	}

	this->root = new Block(0, 0, this->note->GetLength() - 1);

	this->codeInputHelper = new CodeInputHelper(this);

	return 0;
}

void CodeEditingForm::OnClose() {
	if (this->tokenBook != NULL) {
		delete this->tokenBook;
	}
	if (this->tokenFactory != NULL) {
		delete this->tokenFactory;
	}
	if (this->root != NULL) {
		delete this->root;
	}
	if (this->codeInputHelper != NULL) {
		delete this->codeInputHelper;
	}

	TextEditingForm::OnClose();
}

void CodeEditingForm::OnPaint() {
	Visitor* tokenDrawingVisitor = new TokenDrawingVisitor(this->tokenBook);

	this->note->Accept(tokenDrawingVisitor);

	if (tokenDrawingVisitor != NULL) {
		delete tokenDrawingVisitor;
	}

	TextEditingForm::OnPaint();
}

void CodeEditingForm::OnEditCommandRange(UINT uID) {
	Long previousLength = this->note->GetLength();

	TextEditingForm::OnEditCommandRange(uID);

	Long currentLength = this->note->GetLength();

	bool isDirty = false;
	if (uID == IDCNT_EDIT_WRITE &&
		(this->GetCurrentCharacter() >= 32 || this->GetCurrentCharacter() == VK_TAB
			|| this->GetCurrentCharacter() == VK_RETURN)
		|| uID == IDCNT_EDIT_IMECOMPOSITION || uID == IDCNT_EDIT_IMECHAR || uID == IDCNT_EDIT_DELETE
		|| uID == IDCNT_EDIT_DELETESELECTION || uID == IDCNT_EDIT_PASTE
		|| uID == IDCNT_EDIT_UNDO || uID == IDCNT_EDIT_REDO) {
		isDirty = true;
	}

	CodeEditor* codeEditor = (CodeEditor*)this->GetParent();
	if (isDirty == true) {
		codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
	}

	if (currentLength != previousLength &&
		(uID == IDCNT_EDIT_WRITE || uID == IDCNT_EDIT_IMECOMPOSITION || uID == IDCNT_EDIT_IMECHAR
			|| uID == IDCNT_EDIT_PASTE || uID == IDCNT_EDIT_DELETE || uID == IDCNT_EDIT_DELETESELECTION
			|| uID == IDCNT_EDIT_UNDO || uID == IDCNT_EDIT_REDO)) {
		Long i = previousLength + 1;
		while (i <= currentLength) {
			codeEditor->codeNumberingForm->AddNumber(i);
			i++;
		}
		i = previousLength - 1;
		while (i >= currentLength) {
			codeEditor->codeNumberingForm->RemoveNumber();
			i--;
		}
	}

	if (this->tokenBook != NULL) {
		delete this->tokenBook;
	}
	this->tokenBook = new TokenBook;
	Visitor* tokenMakingVisitor = new TokenMakingVisitor(this->tokenBook, this->tokenFactory);

	this->note->Accept(tokenMakingVisitor);

	if (tokenMakingVisitor != NULL) {
		delete tokenMakingVisitor;
	}

	this->codeInputHelper->UpdateBlock();
	if (uID == IDCNT_EDIT_WRITE) {
		Long currentRow = this->note->GetCurrent();
		if (this->GetCurrentCharacter() == VK_RETURN) {
			string afterContent = "";
			Long currentColumn = this->current->GetCurrent();
			if (currentColumn < this->current->GetLength()) {
				afterContent = this->current->GetAt(this->current->GetCurrent())->GetContent();
			}
			//Block* now = this->root->Find(currentRow);
			Long i = 1;
			if (afterContent == "}") {
				while (i <= this->root->Find(currentRow)->GetLevel() - 1) {
					this->SendMessage(WM_CHAR, MAKEWPARAM(VK_TAB, 0));
					i++;
				}
				i = 1;
				while (i <= this->root->Find(currentRow)->GetLevel()) {
					this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_LEFT, 0));
					i++;
				}
				this->SendMessage(WM_CHAR, MAKEWPARAM(VK_RETURN, 0));
			}
			else {
				i = 1;
				while (i <= this->root->Find(currentRow)->GetLevel()) {
					this->SendMessage(WM_CHAR, MAKEWPARAM(VK_TAB, 0));
					i++;
				}
			}
		}
		else if (this->GetCurrentCharacter() == 123) { // {
			this->codeInputHelper->CleanRow();
			this->SendMessage(WM_CHAR, MAKEWPARAM(125, 0));
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_LEFT, 0));
		}
		else if (this->GetCurrentCharacter() == 59) { // ;
			this->codeInputHelper->CleanRow();
		}
	}
	this->Notify();
	this->Invalidate();
}

void CodeEditingForm::OnMoveCommandRange(UINT uID) {
	TextEditingForm::OnMoveCommandRange(uID);

	static_cast<CodeEditor*>(this->GetParent())->codeNumberingForm->Invalidate();
}

void CodeEditingForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyActionFactory keyActionFactory(this);
	KeyAction* keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		delete keyAction;
	}

	TextEditingForm::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CodeEditingForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	TextEditingForm::OnHScroll(nSBCode, nPos, pScrollBar);

	static_cast<CodeEditor*>(this->GetParent())->codeNumberingForm->Invalidate();
}

void CodeEditingForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	TextEditingForm::OnVScroll(nSBCode, nPos, pScrollBar);

	static_cast<CodeEditor*>(this->GetParent())->codeNumberingForm->Invalidate();
}

BOOL CodeEditingForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	TextEditingForm::OnMouseWheel(nFlags, zDelta, pt);

	static_cast<CodeEditor*>(this->GetParent())->codeNumberingForm->Invalidate();

	return TRUE;
}