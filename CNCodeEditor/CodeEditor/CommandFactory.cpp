#include "CommandFactory.h"
#include "Commands.h"
#include "CodeEditingForm.h"
#include "resource.h"

CommandFactory::CommandFactory(CodeEditingForm* codeEditingForm) {
	this->codeEditingForm = codeEditingForm;
}

CommandFactory::CommandFactory(const CommandFactory& source) {
	this->codeEditingForm = source.codeEditingForm;
}

CommandFactory::~CommandFactory() {

}

CommandFactory& CommandFactory::operator=(const CommandFactory& source) {
	this->codeEditingForm = source.codeEditingForm;

	return *this;
}

Command* CommandFactory::Make(int uID) {
	Command* command = 0;
	if (uID == IDM_FORMAT_FONT) {
		command = new FontCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_NEW) {
		command = new NewCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_OPEN) {
		command = new OpenCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_SAVE) {
		command = new SaveCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_SAVEAS) {
		command = new SaveAsCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_PAGESETUP) {
		command = new PageSetupCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_PRINT) {
		command = new PrintCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_PREVIEW) {
		command = new PreviewCommand(this->codeEditingForm);
	}
	else if (uID == IDM_FILE_CLOSE) {
		command = new CloseCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_COPY) {
		command = new CopyCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_PASTE) {
		command = new PasteCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_CUT) {
		command = new CutCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_SELECTALL) {
		command = new SelectAllCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_DELETE) {
		command = new DeleteSelectionCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_UNDO) {
		command = new UndoCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_REDO) {
		command = new RedoCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_FIND) {
		command = new FindCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EDIT_REPLACE) {
		command = new ReplaceCommand(this->codeEditingForm);
	}
	else if (uID == IDC_REPORT_DIRTY) {
		command = new ReportDirtyCommand(this->codeEditingForm);
	}
	else if (uID == IDM_EXECUTE_C) {
		command = new CCompileCommand(this->codeEditingForm);
	}

	return command;
}