#include "CommandFactory.h"
#include "Commands.h"
#include "CodeEditor.h"
#include "resource.h"

CommandFactory::CommandFactory(CodeEditor* codeEditor) {
	this->codeEditor = codeEditor;
}

CommandFactory::CommandFactory(const CommandFactory& source) {
	this->codeEditor = source.codeEditor;
}

CommandFactory::~CommandFactory() {

}

CommandFactory& CommandFactory::operator=(const CommandFactory& source) {
	this->codeEditor = source.codeEditor;

	return *this;
}

Command* CommandFactory::Make(int uID) {
	Command* command = 0;
	if (uID == IDM_FORMAT_FONT) {
		command = new FontCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_NEW) {
		command = new NewCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_OPEN) {
		command = new OpenCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_SAVE) {
		command = new SaveCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_SAVEAS) {
		command = new SaveAsCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_PAGESETUP) {
		command = new PageSetupCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_PRINT) {
		command = new PrintCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_PREVIEW) {
		command = new PreviewCommand(this->codeEditor);
	}
	else if (uID == IDM_FILE_CLOSE) {
		command = new CloseCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_COPY) {
		command = new CopyCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_PASTE) {
		command = new PasteCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_CUT) {
		command = new CutCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_SELECTALL) {
		command = new SelectAllCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_DELETE) {
		command = new DeleteSelectionCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_UNDO) {
		command = new UndoCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_REDO) {
		command = new RedoCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_FIND) {
		command = new FindCommand(this->codeEditor);
	}
	else if (uID == IDM_EDIT_REPLACE) {
		command = new ReplaceCommand(this->codeEditor);
	}
	else if (uID == IDC_REPORT_DIRTY) {
		command = new ReportDirtyCommand(this->codeEditor);
	}
	else if (uID == IDM_C_COMPILE) {
		command = new CCompileCommand(this->codeEditor);
	}
	else if (uID == IDM_C_LINK) {
		command = new CLinkCommand(this->codeEditor);
	}
	else if (uID == IDM_C_LOAD) {
		command = new CLoadCommand(this->codeEditor);
	}

	return command;
}