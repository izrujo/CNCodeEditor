#include "CodeEditorApp.h"
#include "CodeEditingForm.h"

BOOL CodeEditorApp::InitInstance() {
	CodeEditingForm* codeEditingForm = new CodeEditingForm;
	codeEditingForm->Create(NULL, "CNCodeEditor");
	codeEditingForm->ShowWindow(SW_MAXIMIZE);
	codeEditingForm->UpdateWindow();
	this->m_pMainWnd = codeEditingForm;

	return TRUE;
}
CodeEditorApp codeEditorApp;