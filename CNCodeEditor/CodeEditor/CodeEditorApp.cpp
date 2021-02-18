#include "CodeEditorApp.h"
#include "CodeEditor.h"

BOOL CodeEditorApp::InitInstance() {
	CodeEditor* codeEditor = new CodeEditor;
	codeEditor->Create(NULL, "CNCodeEditor");
	codeEditor->ShowWindow(SW_MAXIMIZE);
	codeEditor->UpdateWindow();
	this->m_pMainWnd = codeEditor;

	return TRUE;
}
CodeEditorApp codeEditorApp;