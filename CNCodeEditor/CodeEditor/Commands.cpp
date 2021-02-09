#include "Commands.h"
#include "CodeEditingForm.h"
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
#include "ConsoleForm.h"
#include "../TextEditor/TextEditingForm.h"
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
Command::Command(CodeEditingForm* codeEditingForm) {
	this->codeEditingForm = codeEditingForm;
}

Command::Command(const Command& source) {
	this->codeEditingForm = source.codeEditingForm;
}

Command::~Command() {

}

Command& Command::operator=(const Command& source) {
	this->codeEditingForm = source.codeEditingForm;

	return *this;
}

//FontCommand
FontCommand::FontCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	LOGFONT logFont = this->codeEditingForm->textEditingForm->font->GetFont();
	COLORREF color;
	CFontDialog fontDialog(&logFont);
	int ret = fontDialog.DoModal();
	if (ret == IDOK) {
		//��Ʈ ��ȭ ���ڿ��� ��Ʈ ������ �����´�.
		fontDialog.GetCurrentFont(&logFont);
		color = fontDialog.GetColor();
		if (this->codeEditingForm->textEditingForm->font != NULL) {
			delete this->codeEditingForm->textEditingForm->font;
		}
		this->codeEditingForm->textEditingForm->font = new Font(logFont, color, this->codeEditingForm);
		if (this->codeEditingForm->textEditingForm->characterMetrics != NULL) {
			delete this->codeEditingForm->textEditingForm->characterMetrics;
		}
		this->codeEditingForm->textEditingForm->characterMetrics = new CharacterMetrics(this->codeEditingForm, this->codeEditingForm->textEditingForm->font);

		//=====�ڵ� ���� ó��=====
		if (this->codeEditingForm->textEditingForm->GetIsLockedHScroll() == TRUE) {
			CRect rect;
			this->codeEditingForm->textEditingForm->GetClientRect(rect);
			DummyManager dummyManager(this->codeEditingForm->textEditingForm->note, this->codeEditingForm->textEditingForm->characterMetrics, rect.Width());

			Long row = this->codeEditingForm->textEditingForm->note->GetCurrent();
			Long column = this->codeEditingForm->textEditingForm->current->GetCurrent();
			Long distance = dummyManager.CountDistance(row, column);
			Long i = 0;
			while (i < this->codeEditingForm->textEditingForm->note->GetLength()) {
				dummyManager.Unfold(i);
				i++;
			}
			i = 0;
			while (i < this->codeEditingForm->textEditingForm->note->GetLength()) {
				i = dummyManager.Fold(i);
				i++;
			}
			dummyManager.CountIndex(distance, &row, &column);
			this->codeEditingForm->textEditingForm->note->Move(row);
			this->codeEditingForm->textEditingForm->current = this->codeEditingForm->textEditingForm->note->GetAt(row);
			this->codeEditingForm->textEditingForm->current->Move(column);
		}
		//=====�ڵ� ���� ó��=====
	}
}

string FontCommand::GetType() {
	return "Font";
}

Command* FontCommand::Clone() {
	return new FontCommand(*this);
}

//NewCommand
NewCommand::NewCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	FileManager fileManager(this->codeEditingForm);
	int message;
	bool isDirty = this->codeEditingForm->document->GetIsDirty();
	string fileName = this->codeEditingForm->document->GetPathName();

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C �ҽ�";

	if (isDirty == true) {
		CString messageText;
		messageText.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "CNCodeEditor", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "���� ����") {
				FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->codeEditingForm->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->codeEditingForm->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(tailOfTitle.c_str());
					this->codeEditingForm->SetWindowTextA((LPCTSTR)title);
				}
			}
			else {
				//===== See annotation of Save() in Files.h =====
				string encodingType = "ANSI";
				this->codeEditingForm->document->SetEncodingType(encodingType);
				//===============================================

				fileManager.Save();
			}
		}
	}

	if (isDirty == false || message != IDCANCEL) {
		//���� ����� ó��
		fileManager.New();
		string pathName = "���� ����";
		string title = pathName + tailOfTitle;
		this->codeEditingForm->SetWindowTextA(title.c_str());

		this->codeEditingForm->document->SetPathName(pathName);

		this->codeEditingForm->document->SetIsDirty(false);
	}
}

string NewCommand::GetType() {
	return "New";
}

Command* NewCommand::Clone() {
	return new NewCommand(*this);
}

//OpenCommand
OpenCommand::OpenCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	FileManager fileManager(this->codeEditingForm);
	int message;
	bool isDirty = this->codeEditingForm->document->GetIsDirty();
	string fileName = this->codeEditingForm->document->GetPathName();

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C �ҽ�";

	if (isDirty == true) {
		CString messageText;
		messageText.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "CNCodeEditor", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "���� ����") {
				FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->codeEditingForm->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->codeEditingForm->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(tailOfTitle.c_str());
					this->codeEditingForm->SetWindowTextA((LPCTSTR)title);
				}
			}
			else {
				//===== See annotation of Save() in Files.h =====
				string encodingType = "ANSI";
				this->codeEditingForm->document->SetEncodingType(encodingType);
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
			this->codeEditingForm->document->SetPathName((LPCTSTR)pathName);

			fileManager.Load();

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(tailOfTitle.c_str());
			this->codeEditingForm->SetWindowTextA((LPCTSTR)title);

			this->codeEditingForm->document->SetIsDirty(false);
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
SaveCommand::SaveCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	FileManager fileManager(this->codeEditingForm);
	string fileName = this->codeEditingForm->document->GetPathName();

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C �ҽ�";

	int ret = IDOK;
	FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
	if (fileName == "���� ����") {
		ret = fileDialog.DoModal();
		if (ret == IDOK) {
			string encodingType = fileDialog.GetEncodingType();
			this->codeEditingForm->document->SetEncodingType(encodingType);
			CString pathName = fileDialog.GetPathName();
			this->codeEditingForm->document->SetPathName((LPCTSTR)pathName);

			fileManager.Save();

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(tailOfTitle.c_str());
			this->codeEditingForm->SetWindowTextA((LPCTSTR)title);
		}
	}
	else {
		//===== See annotation of Save() in Files.h =====
		string encodingType = "ANSI";
		this->codeEditingForm->document->SetEncodingType(encodingType);
		//===============================================

		fileManager.Save();
	}

	if (this->codeEditingForm->document->GetIsDirty() == true && ret == IDOK) {
		CString title;
		this->codeEditingForm->GetWindowText(title);
		title.Delete(0);
		this->codeEditingForm->SetWindowTextA(title);
		this->codeEditingForm->document->SetIsDirty(false);
	}
}

string SaveCommand::GetType() {
	return "Save";
}

Command* SaveCommand::Clone() {
	return new SaveCommand(*this);
}

//SaveAsCommand
SaveAsCommand::SaveAsCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	FileManager fileManager(this->codeEditingForm);

	string format = "c";
	string formatlist = "C Source(*.c) | *.c ||";
	string tailOfTitle = " - C �ҽ�";

	FileDialog fileDialog(FALSE, format.c_str(), "*", 524326, formatlist.c_str());
	int ret = fileDialog.DoModal();
	if (ret == IDOK) {
		string encodingType = fileDialog.GetEncodingType();
		this->codeEditingForm->document->SetEncodingType(encodingType);
		CString pathName = fileDialog.GetPathName();
		this->codeEditingForm->document->SetPathName((LPCTSTR)pathName);

		fileManager.Save();

		CString title = fileDialog.GetFileTitle();
		title.AppendFormat(tailOfTitle.c_str());
		this->codeEditingForm->SetWindowTextA((LPCTSTR)title);
	}

	if (this->codeEditingForm->document->GetIsDirty() == true && ret == IDOK) {
		CString title;
		this->codeEditingForm->GetWindowText(title);
		title.Delete(0);
		this->codeEditingForm->SetWindowTextA(title);
		this->codeEditingForm->document->SetIsDirty(false);
	}
}

string SaveAsCommand::GetType() {
	return "SaveAs";
}

Command* SaveAsCommand::Clone() {
	return new SaveAsCommand(*this);
}

//CloseCommand
CloseCommand::CloseCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	this->codeEditingForm->SendMessage(WM_CLOSE, 0);
}

string CloseCommand::GetType() {
	return "Close";
}

Command* CloseCommand::Clone() {
	return new CloseCommand(*this);
}

//PageSetupCommand
PageSetupCommand::PageSetupCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	PageSetupDialog pageSetupDialog(this->codeEditingForm);
	pageSetupDialog.DoModal();
	this->codeEditingForm->document->deviceMode = (DEVMODE*)GlobalLock(pageSetupDialog.psd.hDevMode);
	GlobalUnlock(pageSetupDialog.psd.hDevMode);
}

string PageSetupCommand::GetType() {
	return "PageSetup";
}

Command* PageSetupCommand::Clone() {
	return new PageSetupCommand(*this);
}

//PrintCommand
PrintCommand::PrintCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
		this->codeEditingForm);
	if (IDOK == pd.DoModal()) {
		CString deviceName = pd.GetDeviceName();
		memcpy(this->codeEditingForm->document->deviceMode->dmDeviceName, (VOID*)LPCTSTR(deviceName), 32);

		Glyph* note = this->codeEditingForm->textEditingForm->note->Clone();
		if (this->codeEditingForm->textEditingForm->GetIsLockedHScroll() == TRUE) {
			CRect rect;
			this->codeEditingForm->GetClientRect(rect);
			DummyManager dummyManager(note, this->codeEditingForm->textEditingForm->characterMetrics, rect.Width());
			Long i = 0;
			while (i < note->GetLength()) {
				dummyManager.Unfold(i);
				i++;
			}
		}

		PrintInformation* printInformation = new PrintInformation(this->codeEditingForm, note);

		if (printInformation->printerDC.StartDocA(this->codeEditingForm->document->GetPathName().c_str()) < 0) {
			AfxMessageBox(_T("Printer wouldn't initialize"));
		}
		else {
			this->codeEditingForm->printStateDialog = new PrintStateDialog(this->codeEditingForm);
			this->codeEditingForm->printStateDialog->SetActiveWindow();
			this->codeEditingForm->printStateDialog->ShowWindow(TRUE);

			Printer printer(this->codeEditingForm, printInformation);
			printer.Print();

			this->codeEditingForm->printJobManager->Check(this->codeEditingForm->printStateDialog);
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
PreviewCommand::PreviewCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	PreviewForm* previewForm = new PreviewForm(this->codeEditingForm);
	previewForm->Create(NULL, "�μ� �̸� ����", 13565952UL, CRect(0, 0, 1200, 875));
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
CopyCommand::CopyCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_COPY, 0));
}

string CopyCommand::GetType() {
	return "Copy";
}

Command* CopyCommand::Clone() {
	return new CopyCommand(*this);
}

//PasteCommand
PasteCommand::PasteCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {

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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_PASTE, 0));
}

string PasteCommand::GetType() {
	return "Paste";
}

Command* PasteCommand::Clone() {
	return new PasteCommand(*this);
}

//CutCommand
CutCommand::CutCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {

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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_CUT, 0));
}

string CutCommand::GetType() {
	return "Cut";
}

Command* CutCommand::Clone() {
	return new CutCommand(*this);
}

//SelectAllCommand
SelectAllCommand::SelectAllCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_SELECTALL, 0));
}

string SelectAllCommand::GetType() {
	return "SelectAll";
}

Command* SelectAllCommand::Clone() {
	return new SelectAllCommand(*this);
}

//DeleteSelectionCommand
DeleteSelectionCommand::DeleteSelectionCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {

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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
}

string DeleteSelectionCommand::GetType() {
	return "DeleteSelection";
}

Command* DeleteSelectionCommand::Clone() {
	return new DeleteSelectionCommand(*this);
}

//UndoCommand
UndoCommand::UndoCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_UNDO, 0));
}

string UndoCommand::GetType() {
	return "Undo";
}

Command* UndoCommand::Clone() {
	return new UndoCommand(*this);
}

//RedoCommand
RedoCommand::RedoCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REDO, 0));
}

string RedoCommand::GetType() {
	return "Redo";
}

Command* RedoCommand::Clone() {
	return new RedoCommand(*this);
}

//FindCommand
FindCommand::FindCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_FIND, 0));
}

string FindCommand::GetType() {
	return "Find";
}

Command* FindCommand::Clone() {
	return new FindCommand(*this);
}

//ReplaceCommand
ReplaceCommand::ReplaceCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	this->codeEditingForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REPLACE, 0));
}

string ReplaceCommand::GetType() {
	return "Replace";
}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}

//ReportDirtyCommand
ReportDirtyCommand::ReportDirtyCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	bool isDirty = this->codeEditingForm->document->GetIsDirty();
	if (isDirty == false) {
		CString title;
		this->codeEditingForm->GetWindowText(title);
		title.Insert(0, '*');
		isDirty = true;
		this->codeEditingForm->SetWindowTextA(title);
		this->codeEditingForm->document->SetIsDirty(isDirty);
	}
}

string ReportDirtyCommand::GetType() {
	return "ReportDirty";
}

Command* ReportDirtyCommand::Clone() {
	return new ReportDirtyCommand(*this);
}

//CCompileCommand
CCompileCommand::CCompileCommand(CodeEditingForm* codeEditingForm)
	: Command(codeEditingForm) {
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
	//���� ���� �ڵ� �����ؾ���. ���� ����.
	this->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVE, 0));

	//tcc �����Ϸ��� �̿��� ���� ������(C �ҽ� �������� ����) �������ϰ� �� ����� �����Ѵ�.
	/*string cmd = ".\\tcc\\tcc -run ";
	cmd += this->codeEditingForm->document->GetPathName();
	cmd += " > outTemp.txt 2>&1";
	system(cmd.c_str());*/

	string pathName = this->codeEditingForm->document->GetPathName();
	string fileName = pathName;
	Long i = fileName.length();
	char character = fileName.at(i - 1);
	while (i > 0 && character != '\\') {
		character = fileName.at(--i);
	}
	fileName = fileName.erase(0, i+1);
	
	string exeName = fileName;
	i = exeName.length();
	character = exeName.at(i - 1);
	while (i > 0 && character != '.') {
		character = exeName.at(--i);
	}
	exeName = exeName.erase(i);

	string cmd = "gcc -o " + exeName + " " + pathName; //������, ��ũ
	system(cmd.c_str());

	cmd = exeName + " > outTemp.txt 2>&1"; //���� ������ �� ��.
	system(cmd.c_str());

	//������ ����� �����´�.
	int message;
	string resultFileName = "outTemp.txt";

	File* file = new AnsiFile(resultFileName); //FileFactory::MakeOpenFile�� ������ �ִ� �� ����.ansi�ε� utf16le�� �о.
	this->codeEditingForm->document->SetEncodingType(file->GetType());
	string result = file->Load();

	if (file != 0) {
		delete file;
	}

	this->codeEditingForm->document->SetPathName(pathName);

	ConsoleForm* consoleForm = new ConsoleForm(this->codeEditingForm, result, exeName);
	consoleForm->Create(NULL, "����� �ܼ�");
	consoleForm->ShowWindow(SW_SHOW);
	consoleForm->UpdateWindow();
}

string CCompileCommand::GetType() {
	return "CCompile";
}

Command* CCompileCommand::Clone() {
	return new CCompileCommand(*this);
}