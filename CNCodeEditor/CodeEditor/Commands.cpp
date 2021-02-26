#include "Commands.h"
#include "CodeEditor.h"
#include "FileManager.h"
#include "Document.h"
#include "FileDialog.h"
#include "PageSetupDialog.h"
#include "PreviewForm.h"
#include "PrintInformation.h"
#include "PrintingVisitor.h"
#include "PrintStateDialog.h"
#include "Printer.h"
#include "PrintJobManager.h"
#include "Files.h"
#include "FileFactory.h"
#include "OutputForm.h"
#include "CodeNumberingForm.h"
#include "CodeEditingForm.h"
#include "TokenFactory.h"
#include "../TextEditor/Font.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/ScrollController.h"
#include "../TextEditor/HistoryBook.h"
#include "../TextEditor/Scanner.h"
#include "../TextEditor/Selection.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Note.h"
#include "../TextEditor/CaretController.h"
#include "../TextEditor/Scroll.h"
#include "../TextEditor/FindReplaceDialog.h"
#include "../TextEditor/DummyLine.h"
#include "../TextEditor/DummyManager.h"

#include "resource.h"
#include "../TextEditor/resource.h"

#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>
#include <dlgs.h>
#include <winspool.h>

#pragma warning(disable:4996)

//Command
Command::Command(CodeEditor* codeEditor) {
	this->codeEditor = codeEditor;
}

Command::Command(const Command& source) {
	this->codeEditor = source.codeEditor;
}

Command::~Command() {

}

Command& Command::operator=(const Command& source) {
	this->codeEditor = source.codeEditor;

	return *this;
}

//FontCommand
FontCommand::FontCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

FontCommand::FontCommand(const FontCommand& source)
	: Command(source) {

}

FontCommand::~FontCommand() {

}

FontCommand& FontCommand::operator=(const FontCommand& source) {
	Command::operator=(source);

	return *this;
}

void FontCommand::Execute() {
	LOGFONT logFont = this->codeEditor->codeEditingForm->font->GetFont();
	COLORREF color;
	CFontDialog fontDialog(&logFont);
	int ret = fontDialog.DoModal();
	if (ret == IDOK) {
		//폰트 대화 상자에서 폰트 정보를 가져온다.
		fontDialog.GetCurrentFont(&logFont);
		color = this->codeEditor->codeEditingForm->font->GetColor();
		if (this->codeEditor->codeEditingForm->font != NULL) {
			delete this->codeEditor->codeEditingForm->font;
		}
		this->codeEditor->codeEditingForm->font = new Font(logFont, color, this->codeEditor->codeEditingForm);

		this->codeEditor->codeEditingForm->note->Paint(color);

		if (this->codeEditor->codeEditingForm->characterMetrics != NULL) {
			delete this->codeEditor->codeEditingForm->characterMetrics;
		}
		this->codeEditor->codeEditingForm->characterMetrics = new CharacterMetrics(this->codeEditor, this->codeEditor->codeEditingForm->font);

		//=====자동 개행 처리=====
		if (this->codeEditor->codeEditingForm->GetIsLockedHScroll() == TRUE) {
			CRect rect;
			this->codeEditor->codeEditingForm->GetClientRect(rect);
			DummyManager dummyManager(this->codeEditor->codeEditingForm->note, this->codeEditor->codeEditingForm->characterMetrics, rect.Width());

			Long row = this->codeEditor->codeEditingForm->note->GetCurrent();
			Long column = this->codeEditor->codeEditingForm->current->GetCurrent();
			Long distance = dummyManager.CountDistance(row, column);
			Long i = 0;
			while (i < this->codeEditor->codeEditingForm->note->GetLength()) {
				dummyManager.Unfold(i);
				i++;
			}
			i = 0;
			while (i < this->codeEditor->codeEditingForm->note->GetLength()) {
				i = dummyManager.Fold(i);
				i++;
			}
			dummyManager.CountIndex(distance, &row, &column);
			this->codeEditor->codeEditingForm->note->Move(row);
			this->codeEditor->codeEditingForm->current = this->codeEditor->codeEditingForm->note->GetAt(row);
			this->codeEditor->codeEditingForm->current->Move(column);
		}
		//=====자동 개행 처리=====

		this->codeEditor->codeNumberingForm->UpdateNumber();
	}
}

string FontCommand::GetType() {
	return "Font";
}

Command* FontCommand::Clone() {
	return new FontCommand(*this);
}

//ThemaCommand
ThemaCommand::ThemaCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

ThemaCommand::ThemaCommand(const ThemaCommand& source)
	: Command(source) {

}

ThemaCommand::~ThemaCommand() {

}

ThemaCommand& ThemaCommand::operator=(const ThemaCommand& source) {
	Command::operator=(source);

	return *this;
}

void ThemaCommand::Execute() {
	if (this->codeEditor->codeEditingForm->backgroundColor == RGB(255, 255, 255)) {
		this->codeEditor->codeEditingForm->backgroundColor = RGB(30, 30, 30);

		this->codeEditor->codeEditingForm->tokenFactory->Brighten(false);

		LOGFONT logFont = this->codeEditor->codeEditingForm->font->GetFont();
		if (this->codeEditor->codeEditingForm->font != NULL) {
			delete this->codeEditor->codeEditingForm->font;
		}
		this->codeEditor->codeEditingForm->font = new Font(logFont, RGB(255, 255, 255), this->codeEditor->codeEditingForm);
		this->codeEditor->codeEditingForm->note->Paint(RGB(255, 255, 255));
	}
	else if (this->codeEditor->codeEditingForm->backgroundColor == RGB(30, 30, 30)) {
		this->codeEditor->codeEditingForm->backgroundColor = RGB(255, 255, 255);
		
		this->codeEditor->codeEditingForm->tokenFactory->Brighten(true);
		
		LOGFONT logFont = this->codeEditor->codeEditingForm->font->GetFont();
		if (this->codeEditor->codeEditingForm->font != NULL) {
			delete this->codeEditor->codeEditingForm->font;
		}
		this->codeEditor->codeEditingForm->font = new Font(logFont, RGB(0, 0, 0), this->codeEditor->codeEditingForm);
		this->codeEditor->codeEditingForm->note->Paint(RGB(0, 0, 0));
	}

	MENUINFO menuInfo;
	this->codeEditor->menu.GetMenuInfo(&menuInfo);
	menuInfo.fMask |= MIM_BACKGROUND;
	menuInfo.hbrBack = ::CreateSolidBrush(this->codeEditor->codeEditingForm->backgroundColor);
	this->codeEditor->menu.SetMenuInfo(&menuInfo);

	this->codeEditor->codeNumberingForm->Invalidate();

	if (this->codeEditor->outputForm != NULL) {
		this->codeEditor->outputForm->SendMessage(WM_CLOSE);
		this->codeEditor->outputForm = NULL;
	}
}

string ThemaCommand::GetType() {
	return "Thema";
}

Command* ThemaCommand::Clone() {
	return new ThemaCommand(*this);
}

//NewCommand
NewCommand::NewCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

NewCommand::NewCommand(const NewCommand& source)
	: Command(source) {
}

NewCommand::~NewCommand() {

}

NewCommand& NewCommand::operator=(const NewCommand& source) {
	Command::operator=(source);

	return *this;
}

void NewCommand::Execute() {
	FileManager fileManager(this->codeEditor);
	int message;
	bool isDirty = this->codeEditor->document->GetIsDirty();
	string fileName = this->codeEditor->document->GetPathName();

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C 소스";

	if (isDirty == true) {
		CString messageText;
		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "CNCodeEditor", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "제목 없음") {
				FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->codeEditor->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->codeEditor->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(tailOfTitle.c_str());
					this->codeEditor->SetWindowTextA((LPCTSTR)title);
				}
			}
			else {
				//===== See annotation of Save() in Files.h =====
				string encodingType = "ANSI";
				this->codeEditor->document->SetEncodingType(encodingType);
				//===============================================

				fileManager.Save();
			}
		}
	}

	if (isDirty == false || message != IDCANCEL) {
		//새로 만들기 처리
		fileManager.New();
		string pathName = "제목 없음";
		string title = pathName + tailOfTitle;
		this->codeEditor->SetWindowTextA(title.c_str());

		this->codeEditor->document->SetPathName(pathName);

		this->codeEditor->document->SetIsDirty(false);
	}

	this->codeEditor->codeNumberingForm->UpdateNumber();
}

string NewCommand::GetType() {
	return "New";
}

Command* NewCommand::Clone() {
	return new NewCommand(*this);
}

//OpenCommand
OpenCommand::OpenCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

OpenCommand::OpenCommand(const OpenCommand& source)
	: Command(source) {
}

OpenCommand::~OpenCommand() {

}

OpenCommand& OpenCommand::operator=(const OpenCommand& source) {
	Command::operator=(source);

	return *this;
}

void OpenCommand::Execute() {
	FileManager fileManager(this->codeEditor);
	int message;
	bool isDirty = this->codeEditor->document->GetIsDirty();
	string fileName = this->codeEditor->document->GetPathName();

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C 소스";

	if (isDirty == true) {
		CString messageText;
		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "CNCodeEditor", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "제목 없음") {
				FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->codeEditor->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->codeEditor->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(tailOfTitle.c_str());
					this->codeEditor->SetWindowTextA((LPCTSTR)title);
				}
			}
			else {
				//===== See annotation of Save() in Files.h =====
				string encodingType = "ANSI";
				this->codeEditor->document->SetEncodingType(encodingType);
				//===============================================

				fileManager.Save();
			}
		}
	}

	if (isDirty == false || message != IDCANCEL) {
		FileDialog fileDialog(TRUE, format.c_str(), "*", 524326, formatlist.c_str());
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();
			this->codeEditor->document->SetPathName((LPCTSTR)pathName);

			fileManager.Load();

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(tailOfTitle.c_str());
			this->codeEditor->SetWindowTextA((LPCTSTR)title);

			this->codeEditor->document->SetIsDirty(false);

			this->codeEditor->codeNumberingForm->UpdateNumber();
		}
	}
}

string OpenCommand::GetType() {
	return "Open";
}

Command* OpenCommand::Clone() {
	return new OpenCommand(*this);
}

//SaveCommand
SaveCommand::SaveCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

SaveCommand::SaveCommand(const SaveCommand& source)
	: Command(source) {
}

SaveCommand::~SaveCommand() {

}

SaveCommand& SaveCommand::operator=(const SaveCommand& source) {
	Command::operator=(source);

	return *this;
}

void SaveCommand::Execute() {
	FileManager fileManager(this->codeEditor);
	string fileName = this->codeEditor->document->GetPathName();

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C 소스";

	int ret = IDOK;
	FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
	if (fileName == "제목 없음") {
		ret = fileDialog.DoModal();
		if (ret == IDOK) {
			string encodingType = fileDialog.GetEncodingType();
			this->codeEditor->document->SetEncodingType(encodingType);
			CString pathName = fileDialog.GetPathName();
			this->codeEditor->document->SetPathName((LPCTSTR)pathName);

			fileManager.Save();

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(tailOfTitle.c_str());
			this->codeEditor->SetWindowTextA((LPCTSTR)title);
		}
	}
	else {
		//===== See annotation of Save() in Files.h =====
		string encodingType = "ANSI";
		this->codeEditor->document->SetEncodingType(encodingType);
		//===============================================

		fileManager.Save();
	}

	if (this->codeEditor->document->GetIsDirty() == true && ret == IDOK) {
		CString title;
		this->codeEditor->GetWindowText(title);
		title.Delete(0);
		this->codeEditor->SetWindowTextA(title);
		this->codeEditor->document->SetIsDirty(false);
	}
}

string SaveCommand::GetType() {
	return "Save";
}

Command* SaveCommand::Clone() {
	return new SaveCommand(*this);
}

//SaveAsCommand
SaveAsCommand::SaveAsCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

SaveAsCommand::SaveAsCommand(const SaveAsCommand& source)
	: Command(source) {
}

SaveAsCommand::~SaveAsCommand() {

}

SaveAsCommand& SaveAsCommand::operator=(const SaveAsCommand& source) {
	Command::operator=(source);

	return *this;
}

void SaveAsCommand::Execute() {
	FileManager fileManager(this->codeEditor);

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C 소스";

	FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
	int ret = fileDialog.DoModal();
	if (ret == IDOK) {
		string encodingType = fileDialog.GetEncodingType();
		this->codeEditor->document->SetEncodingType(encodingType);
		CString pathName = fileDialog.GetPathName();
		this->codeEditor->document->SetPathName((LPCTSTR)pathName);

		fileManager.Save();

		CString title = fileDialog.GetFileTitle();
		title.AppendFormat(tailOfTitle.c_str());
		this->codeEditor->SetWindowTextA((LPCTSTR)title);
	}

	if (this->codeEditor->document->GetIsDirty() == true && ret == IDOK) {
		CString title;
		this->codeEditor->GetWindowText(title);
		title.Delete(0);
		this->codeEditor->SetWindowTextA(title);
		this->codeEditor->document->SetIsDirty(false);
	}
}

string SaveAsCommand::GetType() {
	return "SaveAs";
}

Command* SaveAsCommand::Clone() {
	return new SaveAsCommand(*this);
}

//CloseCommand
CloseCommand::CloseCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

CloseCommand::CloseCommand(const CloseCommand& source)
	: Command(source) {
}

CloseCommand::~CloseCommand() {

}

CloseCommand& CloseCommand::operator=(const CloseCommand& source) {
	Command::operator=(source);

	return *this;
}

void CloseCommand::Execute() {
	this->codeEditor->SendMessage(WM_CLOSE, 0);
}

string CloseCommand::GetType() {
	return "Close";
}

Command* CloseCommand::Clone() {
	return new CloseCommand(*this);
}

//PageSetupCommand
PageSetupCommand::PageSetupCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

PageSetupCommand::PageSetupCommand(const PageSetupCommand& source)
	: Command(source) {
}

PageSetupCommand::~PageSetupCommand() {

}

PageSetupCommand& PageSetupCommand::operator=(const PageSetupCommand& source) {
	Command::operator=(source);

	return *this;
}

void PageSetupCommand::Execute() {
	PageSetupDialog pageSetupDialog(this->codeEditor);
	pageSetupDialog.DoModal();
	this->codeEditor->document->deviceMode = (DEVMODE*)GlobalLock(pageSetupDialog.psd.hDevMode);
	GlobalUnlock(pageSetupDialog.psd.hDevMode);
}

string PageSetupCommand::GetType() {
	return "PageSetup";
}

Command* PageSetupCommand::Clone() {
	return new PageSetupCommand(*this);
}

//PrintCommand
PrintCommand::PrintCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

PrintCommand::PrintCommand(const PrintCommand& source)
	: Command(source) {
}

PrintCommand::~PrintCommand() {

}

PrintCommand& PrintCommand::operator=(const PrintCommand& source) {
	Command::operator=(source);

	return *this;
}

void PrintCommand::Execute() {
	CPrintDialog pd(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_NOSELECTION,
		this->codeEditor);
	if (IDOK == pd.DoModal()) {
		CString deviceName = pd.GetDeviceName();
		memcpy(this->codeEditor->document->deviceMode->dmDeviceName, (VOID*)LPCTSTR(deviceName), 32);

		Glyph* note = this->codeEditor->codeEditingForm->note->Clone();
		if (this->codeEditor->codeEditingForm->GetIsLockedHScroll() == TRUE) {
			CRect rect;
			this->codeEditor->GetClientRect(rect);
			DummyManager dummyManager(note, this->codeEditor->codeEditingForm->characterMetrics, rect.Width());
			Long i = 0;
			while (i < note->GetLength()) {
				dummyManager.Unfold(i);
				i++;
			}
		}

		PrintInformation* printInformation = new PrintInformation(this->codeEditor, note);

		if (printInformation->printerDC.StartDocA(this->codeEditor->document->GetPathName().c_str()) < 0) {
			AfxMessageBox(_T("Printer wouldn't initialize"));
		}
		else {
			this->codeEditor->printStateDialog = new PrintStateDialog(this->codeEditor);
			this->codeEditor->printStateDialog->SetActiveWindow();
			this->codeEditor->printStateDialog->ShowWindow(TRUE);

			Printer printer(this->codeEditor, printInformation);
			printer.Print();

			this->codeEditor->printJobManager->Check(this->codeEditor->printStateDialog);
		}
	}
}

string PrintCommand::GetType() {
	return "Print";
}

Command* PrintCommand::Clone() {
	return new PrintCommand(*this);
}

//PreviewCommand
PreviewCommand::PreviewCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

PreviewCommand::PreviewCommand(const PreviewCommand& source)
	: Command(source) {
}

PreviewCommand::~PreviewCommand() {

}

PreviewCommand& PreviewCommand::operator=(const PreviewCommand& source) {
	Command::operator=(source);

	return *this;
}

void PreviewCommand::Execute() {
	PreviewForm* previewForm = new PreviewForm(this->codeEditor);
	previewForm->Create(NULL, "인쇄 미리 보기", 13565952UL, CRect(0, 0, 1200, 875));
	previewForm->ShowWindow(SW_NORMAL);
	previewForm->UpdateWindow();
}

string PreviewCommand::GetType() {
	return "Preview";
}

Command* PreviewCommand::Clone() {
	return new PreviewCommand(*this);
}

//CopyCommand
CopyCommand::CopyCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

CopyCommand::CopyCommand(const CopyCommand& source)
	: Command(source) {
}

CopyCommand::~CopyCommand() {

}

CopyCommand& CopyCommand::operator=(const CopyCommand& source) {
	Command::operator=(source);

	return *this;
}

void CopyCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_COPY, 0));
}

string CopyCommand::GetType() {
	return "Copy";
}

Command* CopyCommand::Clone() {
	return new CopyCommand(*this);
}

//PasteCommand
PasteCommand::PasteCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {

}

PasteCommand::PasteCommand(const PasteCommand& source)
	: Command(source) {

}

PasteCommand::~PasteCommand() {

}

PasteCommand& PasteCommand::operator=(const PasteCommand& source) {
	Command::operator=(source);

	return *this;
}

void PasteCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_PASTE, 0));
}

string PasteCommand::GetType() {
	return "Paste";
}

Command* PasteCommand::Clone() {
	return new PasteCommand(*this);
}

//CutCommand
CutCommand::CutCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {

}

CutCommand::CutCommand(const CutCommand& source)
	: Command(source) {

}

CutCommand::~CutCommand() {

}

CutCommand& CutCommand::operator=(const CutCommand& source) {
	Command::operator=(source);

	return *this;
}

void CutCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_CUT, 0));
}

string CutCommand::GetType() {
	return "Cut";
}

Command* CutCommand::Clone() {
	return new CutCommand(*this);
}

//SelectAllCommand
SelectAllCommand::SelectAllCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

SelectAllCommand::SelectAllCommand(const SelectAllCommand& source)
	: Command(source) {
}

SelectAllCommand::~SelectAllCommand() {

}

SelectAllCommand& SelectAllCommand::operator=(const SelectAllCommand& source) {
	Command::operator=(source);

	return *this;
}

void SelectAllCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_SELECTALL, 0));
}

string SelectAllCommand::GetType() {
	return "SelectAll";
}

Command* SelectAllCommand::Clone() {
	return new SelectAllCommand(*this);
}

//DeleteSelectionCommand
DeleteSelectionCommand::DeleteSelectionCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {

}

DeleteSelectionCommand::DeleteSelectionCommand(const DeleteSelectionCommand& source)
	: Command(source) {

}

DeleteSelectionCommand::~DeleteSelectionCommand() {
}

DeleteSelectionCommand& DeleteSelectionCommand::operator=(const DeleteSelectionCommand& source) {
	Command::operator=(source);

	return *this;
}

void DeleteSelectionCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
}

string DeleteSelectionCommand::GetType() {
	return "DeleteSelection";
}

Command* DeleteSelectionCommand::Clone() {
	return new DeleteSelectionCommand(*this);
}

//UndoCommand
UndoCommand::UndoCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

UndoCommand::UndoCommand(const UndoCommand& source)
	: Command(source) {
}

UndoCommand::~UndoCommand() {

}

UndoCommand& UndoCommand::operator=(const UndoCommand& source) {
	Command::operator=(source);

	return *this;
}

void UndoCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_UNDO, 0));
}

string UndoCommand::GetType() {
	return "Undo";
}

Command* UndoCommand::Clone() {
	return new UndoCommand(*this);
}

//RedoCommand
RedoCommand::RedoCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

RedoCommand::RedoCommand(const RedoCommand& source)
	: Command(source) {
}

RedoCommand::~RedoCommand() {

}

RedoCommand& RedoCommand::operator=(const RedoCommand& source) {
	Command::operator=(source);

	return *this;
}

void RedoCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REDO, 0));
}

string RedoCommand::GetType() {
	return "Redo";
}

Command* RedoCommand::Clone() {
	return new RedoCommand(*this);
}

//FindCommand
FindCommand::FindCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

FindCommand::FindCommand(const FindCommand& source)
	: Command(source) {
}

FindCommand::~FindCommand() {

}

FindCommand& FindCommand::operator=(const FindCommand& source) {
	Command::operator=(source);

	return *this;
}

void FindCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_FIND, 0));
}

string FindCommand::GetType() {
	return "Find";
}

Command* FindCommand::Clone() {
	return new FindCommand(*this);
}

//ReplaceCommand
ReplaceCommand::ReplaceCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

ReplaceCommand::ReplaceCommand(const ReplaceCommand& source)
	: Command(source) {
}

ReplaceCommand::~ReplaceCommand() {

}

ReplaceCommand& ReplaceCommand::operator=(const ReplaceCommand& source) {
	Command::operator=(source);

	return *this;
}

void ReplaceCommand::Execute() {
	this->codeEditor->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REPLACE, 0));
}

string ReplaceCommand::GetType() {
	return "Replace";
}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}

//ReportDirtyCommand
ReportDirtyCommand::ReportDirtyCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

ReportDirtyCommand::ReportDirtyCommand(const ReportDirtyCommand& source)
	: Command(source) {
}

ReportDirtyCommand::~ReportDirtyCommand() {

}

ReportDirtyCommand& ReportDirtyCommand::operator=(const ReportDirtyCommand& source) {
	Command::operator=(source);

	return *this;
}

void ReportDirtyCommand::Execute() {
	bool isDirty = this->codeEditor->document->GetIsDirty();
	if (isDirty == false) {
		CString title;
		this->codeEditor->GetWindowText(title);
		title.Insert(0, '*');
		isDirty = true;
		this->codeEditor->SetWindowTextA(title);
		this->codeEditor->document->SetIsDirty(isDirty);
	}
}

string ReportDirtyCommand::GetType() {
	return "ReportDirty";
}

Command* ReportDirtyCommand::Clone() {
	return new ReportDirtyCommand(*this);
}

//CCompileCommand
CCompileCommand::CCompileCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

CCompileCommand::CCompileCommand(const CCompileCommand& source)
	: Command(source) {
}

CCompileCommand::~CCompileCommand() {
	
}

CCompileCommand& CCompileCommand::operator=(const CCompileCommand& source) {
	Command::operator=(source);

	return *this;
}

void CCompileCommand::Execute() {
	//현재 파일 자동 저장해야함. 실행 전에.
	this->codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVE, 0));

	string pathName = this->codeEditor->document->GetPathName();

	if (pathName != "제목 없음") {

		if (this->codeEditor->outputForm != NULL) {
			this->codeEditor->outputForm->SendMessage(WM_CLOSE);
			delete this->codeEditor->outputForm;
			this->codeEditor->outputForm = NULL;
		}
		CRect rect;
		this->codeEditor->GetClientRect(rect);
		this->codeEditor->codeEditingForm->MoveWindow(rect);

		//tcc 컴파일러를 이용해 현재 파일을(C 소스 파일임을 전제) 컴파일하고 그 결과를 저장한다.
		/*string cmd = ".\\tcc\\tcc -run ";
		cmd += this->codeEditor->document->GetPathName();
		cmd += " > outTemp.txt 2>&1";
		system(cmd.c_str());*/

		String fileName(pathName);
		Long index = fileName.ReversedFind('\\');
		fileName.Delete(0, index + 1);
		char extension1[3] = ".c";
		char extension2[3] = ".o";
		fileName.Replace(extension1, extension2);
		string objectName = fileName.GetString();
		string cmd = "del " + objectName;
		system(cmd.c_str());

		string resultFileName = "temp\\CompileResult.txt";

		cmd = "gcc -c " + pathName + " > " + resultFileName + " 2>&1"; //컴파일
		system(cmd.c_str());

		File* file = new AnsiFile(resultFileName); //FileFactory::MakeOpenFile에 오류가 있는 것 같다.ansi인데 utf16le로 읽어냄.
		this->codeEditor->document->SetEncodingType(file->GetType());
		string result = file->Load();
		index = result.find("error");
		if (index == -1) {
			result += "\r\nCompile Succeed.\r\n";
		}
		else {
			result += "\r\nCompile Failed.\r\n";
		}

		if (file != 0) {
			delete file;
		}

		this->codeEditor->document->SetPathName(pathName);

		this->codeEditor->GetClientRect(rect);
		CRect outputRect = rect;
		outputRect.top += (outputRect.Height() / 4) * 3;
		rect.bottom = outputRect.top;

		Long width = rect.Width() - CODENUMBERFORMWIDTH;
		rect.left += CODENUMBERFORMWIDTH;
		rect.right = rect.left + width;
		this->codeEditor->codeEditingForm->MoveWindow(rect);

		rect.left -= CODENUMBERFORMWIDTH;
		rect.right = rect.left + CODENUMBERFORMWIDTH - 2;
		rect.bottom -= 26; //수평 스크롤 바 높이.
		this->codeEditor->codeNumberingForm->MoveWindow(rect);

		this->codeEditor->outputForm = new OutputForm(this->codeEditor, result, resultFileName);
		this->codeEditor->outputForm->Create(NULL, "Output", WS_CHILD,
			outputRect, this->codeEditor, NULL, NULL);
		this->codeEditor->outputForm->ShowWindow(SW_SHOW);
		this->codeEditor->outputForm->UpdateWindow();

		this->codeEditor->SetIsCompiled(TRUE);
		this->codeEditor->SetIsLinked(FALSE);
	}
}

string CCompileCommand::GetType() {
	return "CCompile";
}

Command* CCompileCommand::Clone() {
	return new CCompileCommand(*this);
}

//CLinkCommand
CLinkCommand::CLinkCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

CLinkCommand::CLinkCommand(const CLinkCommand& source)
	: Command(source) {
}

CLinkCommand::~CLinkCommand() {

}

CLinkCommand& CLinkCommand::operator=(const CLinkCommand& source) {
	Command::operator=(source);

	return *this;
}

void CLinkCommand::Execute() {
	//컴파일 안했으면 컴파일도.
	//gcc -o filename.o

	string pathName = this->codeEditor->document->GetPathName();

	String fileName(pathName);
	Long index = fileName.ReversedFind('\\');
	fileName.Delete(0, index + 1);
	char extension1[3] = ".c";
	char extension2[3] = ".o";
	char extension3[5] = ".exe";
	fileName.Replace(extension1, extension2);
	string objectName = fileName.GetString();
	fileName.Replace(extension2, extension3);
	string exeName = fileName.GetString();
	string resultFileName = "temp\\LinkResult.txt";

	string cmd = "gcc -o " + exeName + " " + objectName + " > " + resultFileName + " 2>&1";
	system(cmd.c_str());

	cmd = "del " + objectName;
	system(cmd.c_str());

	File* file = new AnsiFile(resultFileName); //FileFactory::MakeOpenFile에 오류가 있는 것 같다.ansi인데 utf16le로 읽어냄.
	this->codeEditor->document->SetEncodingType(file->GetType());
	string result = file->Load();
	if (result.length() < 1) {
		result += "\r\nLink Succeed.\r\n";
	}
	else {
		result += "\r\nLink Failed.\r\n";
	}

	if (file != 0) {
		delete file;
	}

	this->codeEditor->document->SetPathName(pathName);

	if (this->codeEditor->outputForm == NULL) {
		CRect rect;
		this->codeEditor->GetClientRect(rect);
		CRect outputRect = rect;
		outputRect.top += (outputRect.Height() / 4) * 3;
		rect.bottom = outputRect.top;

		Long width = rect.Width() - CODENUMBERFORMWIDTH;
		rect.left += CODENUMBERFORMWIDTH;
		rect.right = rect.left + width;
		this->codeEditor->codeEditingForm->MoveWindow(rect);

		rect.left -= CODENUMBERFORMWIDTH;
		rect.right = rect.left + CODENUMBERFORMWIDTH - 2;
		rect.bottom -= 26; //수평 스크롤 바 높이.
		this->codeEditor->codeNumberingForm->MoveWindow(rect);

		this->codeEditor->outputForm = new OutputForm(this->codeEditor, result, resultFileName);
		this->codeEditor->outputForm->Create(NULL, "Output", WS_CHILD,
			outputRect, this->codeEditor, NULL, NULL);
		this->codeEditor->outputForm->ShowWindow(SW_SHOW);
		this->codeEditor->outputForm->UpdateWindow();
	}
	else {
		this->codeEditor->outputForm->AppendResult(result, resultFileName);
	}

	this->codeEditor->SetIsLinked(TRUE);
}

string CLinkCommand::GetType() {
	return "CLink";
}

Command* CLinkCommand::Clone() {
	return new CLinkCommand(*this);
}

//CLoadCommand
CLoadCommand::CLoadCommand(CodeEditor* codeEditor)
	: Command(codeEditor) {
}

CLoadCommand::CLoadCommand(const CLoadCommand& source)
	: Command(source) {
}

CLoadCommand::~CLoadCommand() {

}

CLoadCommand& CLoadCommand::operator=(const CLoadCommand& source) {
	Command::operator=(source);

	return *this;
}

void CLoadCommand::Execute() {
	if (this->codeEditor->GetIsCompiled() == FALSE) {
		this->codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_C_COMPILE, 0));
	}
	string pathName = this->codeEditor->document->GetPathName();

	if (pathName != "제목 없음") {

		if (this->codeEditor->GetIsLinked() == FALSE) {
			this->codeEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_C_LINK, 0));
		}


		string tempDirectory = "temp\\";

		//2.1. c 파일을 연다. full\\path\\filename.c
		FileFactory fileFactory;
		File* file = fileFactory.MakeOpenFile(pathName);
		this->codeEditor->document->SetEncodingType(file->GetType());
		string content = file->Load();
		if (file != 0) {
			delete file;
		}

		//2.2. 파일을 수정한다. temp\\CNTemp.c
		//-main이 int로 시작하면 return 0;을 찾고 void면 가장 뒤의 }를 찾는다.
		Long last = content.length();
		Long index = content.find("main(");
		Long rindex = content.rfind("int", index);
		if (rindex != -1 && rindex <= index-4) {
			rindex = content.rfind("return 0;", last);
		}
		else {
			rindex = content.rfind("}", last);
		}
		if (rindex != index) {
			string additionalCodes = "printf(\"This process exited with code 0.\\n\");\n"
				"printf(\"Press any key to close this window . . . \\n\");\n"
				"getch();\n";
			content.insert(rindex, additionalCodes);
		}
		//-찾은 곳 이전에 다음 문자열을 넣는다.
		//printf("This process exited with code 0.\n");\n
		//printf("Press any key to close this window . . . ");\n
		//getch();\n
		string tempSource = tempDirectory + "CNTemp.c";
		file = fileFactory.MakeSaveFile(tempSource, this->codeEditor->document->GetEncodingType());
		file->Save(content);
		if (file != 0) {
			delete file;
		}

		//2.3. 수정한 파일을 컴파일한다. temp\\CNTemp.exe
		string tempExe = tempDirectory + "CNTemp.exe";
		string tempResult = tempDirectory + "TempResult.txt";
		string cmd = "gcc -o " + tempExe + " " + tempSource + " > " + tempResult + " 2>&1"; //컴파일, 링크
		system(cmd.c_str());

		//tempResult를 읽어서 'error'가 없으면(warning은 허용) 컴파일/링크에 성공한 것이므로 실행한다. 
		//그렇지 않으면 오류이므로 실행하지 않는다.
		//ChildProcess가 실행될 동안 tempExe파일을 제거할 수 없어 이런 방식을 취한다.
		file = fileFactory.MakeOpenFile(tempResult);
		this->codeEditor->document->SetEncodingType(file->GetType());
		content = file->Load();
		if (file != 0) {
			delete file;
		}
		index = content.find("error");

		if (index == -1) {
			//2.4. 수정한 exe를 실행한다.
			TCHAR szCmdline[] = TEXT("ChildProcess");
			PROCESS_INFORMATION piProcInfo;
			STARTUPINFO siStartInfo;
			BOOL bSuccess = FALSE;

			// Set up members of the PROCESS_INFORMATION structure. 

			ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

			// Set up members of the STARTUPINFO structure. 
			// This structure specifies the STDIN and STDOUT handles for redirection.

			ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
			siStartInfo.cb = sizeof(STARTUPINFO);
			siStartInfo.hStdError = NULL;
			siStartInfo.hStdOutput = NULL;
			siStartInfo.hStdInput = NULL;
			siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
			siStartInfo.lpTitle = (LPSTR)"디버그 콘솔";

			bSuccess = CreateProcess(tempExe.c_str(),
				szCmdline,      // command line 
				NULL,          // process security attributes 
				NULL,          // primary thread security attributes 
				TRUE,          // handles are inherited 
				CREATE_NEW_CONSOLE,             // creation flags 
				NULL,          // use parent's environment 
				NULL,          // use parent's current directory 
				&siStartInfo,  // STARTUPINFO pointer 
				&piProcInfo);  // receives PROCESS_INFORMATION 

			 // If an error occurs, exit the application. 
			if (!bSuccess) {
			}
			else
			{
				// Close handles to the child process and its primary thread.
				// Some applications might keep these handles to monitor the status
				// of the child process, for example.
				CloseHandle(piProcInfo.hProcess);
				CloseHandle(piProcInfo.hThread);

				// Close handles to the stdin and stdout pipes no longer needed by the child process.
				// If they are not explicitly closed, there is no way to recognize that the child process has ended.
			}
		}
		//2.5. 프로세스가 종료되면 수정한 c와 exe를 삭제한다.
		cmd = "del " + tempSource;
		system(cmd.c_str());
		cmd = "del " + tempResult;
		system(cmd.c_str());

		this->codeEditor->SetIsCompiled(FALSE);
		this->codeEditor->SetIsLinked(FALSE);
	}
}

string CLoadCommand::GetType() {
	return "CLoad";
}

Command* CLoadCommand::Clone() {
	return new CLoadCommand(*this);
}