//Commands.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : 
MVC : ?
Author : Chan Hyung Kim
Written Date : 2019.05
Recently Updated : 2020.07.17
**************************************************/

#ifndef _COMMAND_H
#define _COMMAND_H

#include <afxwin.h> //TCHAR
#include <iostream>
#include "../Utilities/Array.h"
using namespace std;

class CodeEditingForm;
class Glyph;
class Note;

//Command
class Command {
public:
	Command(CodeEditingForm* codeEditingForm = 0);
	Command(const Command& source);
	virtual ~Command() = 0;
	Command& operator=(const Command& source);

	virtual void Execute() = 0;

	virtual string GetType() = 0;
	virtual Command* Clone() = 0;

protected:
	CodeEditingForm* codeEditingForm;
};

//FontCommand
class FontCommand : public Command {
public:
	FontCommand(CodeEditingForm *codeEditingForm = 0);
	FontCommand(const FontCommand& source);
	~FontCommand();
	FontCommand& operator=(const FontCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//NewCommand
class NewCommand : public Command {
public:
	NewCommand(CodeEditingForm *codeEditingForm = 0);
	NewCommand(const NewCommand& source);
	virtual ~NewCommand();
	NewCommand& operator=(const NewCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//OpenCommand
class OpenCommand : public Command {
public:
	OpenCommand(CodeEditingForm *codeEditingForm = 0);
	OpenCommand(const OpenCommand& source);
	virtual ~OpenCommand();
	OpenCommand& operator=(const OpenCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//SaveCommand
class SaveCommand : public Command {
public:
	SaveCommand(CodeEditingForm* codeEditingForm = 0);
	SaveCommand(const SaveCommand& source);
	virtual ~SaveCommand();
	SaveCommand& operator=(const SaveCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//SaveAsCommand
class SaveAsCommand : public Command {
public:
	SaveAsCommand(CodeEditingForm* codeEditingForm = 0);
	SaveAsCommand(const SaveAsCommand& source);
	virtual ~SaveAsCommand();
	SaveAsCommand& operator=(const SaveAsCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CloseCommand
class CloseCommand : public Command {
public:
	CloseCommand(CodeEditingForm* codeEditingForm = 0);
	CloseCommand(const CloseCommand& source);
	virtual ~CloseCommand();
	CloseCommand& operator=(const CloseCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PageSetupCommand
class PageSetupCommand : public Command {
public:
	PageSetupCommand(CodeEditingForm* codeEditingForm = 0);
	PageSetupCommand(const PageSetupCommand& source);
	virtual ~PageSetupCommand();
	PageSetupCommand& operator=(const PageSetupCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PrintCommand
class PrintCommand : public Command {
public:
	PrintCommand(CodeEditingForm* codeEditingForm = 0);
	PrintCommand(const PrintCommand& source);
	virtual ~PrintCommand();
	PrintCommand& operator=(const PrintCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PreviewCommand
class PreviewCommand : public Command {
public:
	PreviewCommand(CodeEditingForm* codeEditingForm = 0);
	PreviewCommand(const PreviewCommand& source);
	virtual ~PreviewCommand();
	PreviewCommand& operator=(const PreviewCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CopyCommand
class CopyCommand : public Command {
public:
	CopyCommand(CodeEditingForm* codeEditingForm = 0);
	CopyCommand(const CopyCommand& source);
	virtual ~CopyCommand();
	CopyCommand& operator=(const CopyCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PasteCommand
class PasteCommand : public Command {
public:
	PasteCommand(CodeEditingForm* codeEditingForm = 0);
	PasteCommand(const PasteCommand& source);
	virtual ~PasteCommand();
	PasteCommand& operator=(const PasteCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CutCommand
class CutCommand : public Command {
public:
	CutCommand(CodeEditingForm* codeEditingForm = 0);
	CutCommand(const CutCommand& source);
	virtual ~CutCommand();
	CutCommand& operator=(const CutCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//SelectAllCommand
class SelectAllCommand : public Command {
public:
	SelectAllCommand(CodeEditingForm* codeEditingForm = 0);
	SelectAllCommand(const SelectAllCommand& source);
	virtual ~SelectAllCommand();
	SelectAllCommand& operator=(const SelectAllCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//DeleteSelectionCommand
class DeleteSelectionCommand : public Command {
public:
	DeleteSelectionCommand(CodeEditingForm* codeEditingForm = 0);
	DeleteSelectionCommand(const DeleteSelectionCommand& source);
	virtual ~DeleteSelectionCommand();
	DeleteSelectionCommand& operator=(const DeleteSelectionCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//UndoCommand
class UndoCommand : public Command {
public:
	UndoCommand(CodeEditingForm* codeEditingForm = 0);
	UndoCommand(const UndoCommand& source);
	virtual ~UndoCommand();
	UndoCommand& operator=(const UndoCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//RedoCommand
class RedoCommand : public Command {
public:
	RedoCommand(CodeEditingForm* codeEditingForm = 0);
	RedoCommand(const RedoCommand& source);
	virtual ~RedoCommand();
	RedoCommand& operator=(const RedoCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//FindCommand
class FindCommand : public Command {
public:
	FindCommand(CodeEditingForm* codeEditingForm = 0);
	FindCommand(const FindCommand& source);
	virtual ~FindCommand();
	FindCommand& operator=(const FindCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//ReplaceCommand
class ReplaceCommand : public Command {
public:
	ReplaceCommand(CodeEditingForm* codeEditingForm = 0);
	ReplaceCommand(const ReplaceCommand& source);
	virtual ~ReplaceCommand();
	ReplaceCommand& operator=(const ReplaceCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//ReportDirtyCommand
class ReportDirtyCommand : public Command {
public:
	ReportDirtyCommand(CodeEditingForm* codeEditingForm = 0);
	ReportDirtyCommand(const ReportDirtyCommand& source);
	virtual ~ReportDirtyCommand();
	ReportDirtyCommand& operator=(const ReportDirtyCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CCompileCommand
class CCompileCommand : public Command {
public:
	CCompileCommand(CodeEditingForm* codeEditingForm = 0);
	CCompileCommand(const CCompileCommand& source);
	virtual ~CCompileCommand();
	CCompileCommand& operator=(const CCompileCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CLinkCommand
class CLinkCommand : public Command {
public:
	CLinkCommand(CodeEditingForm* codeEditingForm = 0);
	CLinkCommand(const CLinkCommand& source);
	virtual ~CLinkCommand();
	CLinkCommand& operator=(const CLinkCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CLoadCommand
class CLoadCommand : public Command {
public:
	CLoadCommand(CodeEditingForm* codeEditingForm = 0);
	CLoadCommand(const CLoadCommand& source);
	virtual ~CLoadCommand();
	CLoadCommand& operator=(const CLoadCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

#endif //_COMMAND_H