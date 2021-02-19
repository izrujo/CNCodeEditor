#include "CodeEditingForm.h"
#include "CodeEditor.h"
#include "KeyActionFactory.h"
#include "KeyActions.h"
#include "CodeNumberingForm.h"

#include "resource.h"

#include "../TextEditor/Glyph.h"
#include "../TextEditor/resource.h"

BEGIN_MESSAGE_MAP(CodeEditingForm, TextEditingForm)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(IDCNT_EDIT_WRITE, IDCNT_EDIT_REDO, OnEditCommandRange)
	ON_COMMAND_RANGE(IDCNT_MOVE_LEFT, IDCNT_SELECTMOVE_CTRLEND, OnMoveCommandRange)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

CodeEditingForm::CodeEditingForm()
	: TextEditingForm() {

}

int CodeEditingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	TextEditingForm::OnCreate(lpCreateStruct);

	return 0;
}

void CodeEditingForm::OnClose() {
	TextEditingForm::OnClose();
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

	if (uID == IDCNT_EDIT_WRITE) {
		if (this->GetCurrentCharacter() == VK_RETURN) {
			Glyph* previousLine = this->note->GetAt(this->note->GetCurrent() - 1);
			Glyph* previousCharacter = 0;
			string previousContent;
			if (previousLine->GetLength() > 0) {
				previousCharacter = previousLine->GetAt(previousLine->GetLength() - 1);
				previousContent = previousCharacter->GetContent();
			}
			Long count = 0;
			Long i = 0;
			Glyph* tab = previousLine->GetAt(i++);
			while (i < previousLine->GetLength() && tab->GetContent() == "        ") {
				count++;
				tab = previousLine->GetAt(i++);
			}

			Long tabToInput = count + 1;
			if (previousCharacter == 0 || previousContent == ";"
				|| previousContent == "}" || previousContent == ")" || previousContent == "]" || previousContent == ">") {
				tabToInput--;
			}
			i = 1;
			while (i <= tabToInput) {
				this->SendMessage(WM_CHAR, MAKEWPARAM(VK_TAB, 0));
				i++;
			}
		}
		else if (this->GetCurrentCharacter() == 125) {
			Long previousColumn = this->current->GetCurrent() - 2;
			if (previousColumn < 0) {
				previousColumn++;
			}
			Glyph* previousCharacter = this->current->GetAt(previousColumn);
			if (previousCharacter->GetContent() == "        ") {
				this->current->Move(previousColumn);
				this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETE, 0));
				this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_RIGHT, 0));
			}
		}
	}
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