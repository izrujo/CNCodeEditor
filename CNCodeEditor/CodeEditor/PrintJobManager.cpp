#include "PrintJobManager.h"
#include "CodeEditor.h"
#include "Document.h"
#include <afxwin.h>
#include "PrintingVisitor.h"
#include "PrintStateDialog.h"

#include <winspool.h>

PrintJobManager::PrintJobManager(CodeEditor* codeEditor) {
	this->codeEditor = codeEditor;
	this->thread = NULL;
	this->printStateDialog = NULL;
	this->isChecking = FALSE;
	this->isPausing = FALSE;
}

PrintJobManager::PrintJobManager(const PrintJobManager& source) {
	this->codeEditor = source.codeEditor;
	this->thread = source.thread;
	this->printStateDialog = source.printStateDialog;
	this->isChecking = source.isChecking;
	this->isPausing = source.isPausing;
}

PrintJobManager::~PrintJobManager() {
	this->isChecking = FALSE;
	if (this->thread != NULL) {
		//DWORD ret;
		//::GetExitCodeThread(this->thread->m_hThread, &ret);
		if (this->isPausing == TRUE) {
			this->thread->ResumeThread();
		}
		::WaitForSingleObject(this->thread->m_hThread, INFINITE);
	}
}

PrintJobManager& PrintJobManager::operator=(const PrintJobManager& source) {
	this->codeEditor = source.codeEditor;
	this->thread = source.thread;
	this->printStateDialog = source.printStateDialog;
	this->isChecking = source.isChecking;
	this->isPausing = source.isPausing;

	return *this;
}

void PrintJobManager::Check(PrintStateDialog* printStateDialog) {
	this->isChecking = TRUE;
	this->printStateDialog = printStateDialog;

	this->thread = AfxBeginThread(CheckThread, (LPVOID)this);
}

void PrintJobManager::Pause() {
	this->isChecking = FALSE;
	this->isPausing = TRUE;
	this->thread->SuspendThread();
}

void PrintJobManager::Resume() {
	this->isChecking = TRUE;
	this->isPausing = FALSE;
	this->thread->ResumeThread();
}

void PrintJobManager::End() {
	this->isChecking = FALSE;
	if (this->thread != NULL) {
		if (this->isPausing == TRUE) {
			this->thread->ResumeThread();
		}
		::WaitForSingleObject(this->thread->m_hThread, INFINITE);

	}
	this->isPausing = FALSE;
}

UINT PrintJobManager::CheckThread(LPVOID pParam) {
	Sleep(1000);
	PrintJobManager* printJobManager = (PrintJobManager*)pParam;

	HANDLE hPrinter; //Printer handle variable
	DWORD dwNeeded, dwReturned; //Mem needed, jobs found, variable for loop
	JOB_INFO_1* pJobInfo; // pointer to structure

	//Find printer handle
	OpenPrinter((LPSTR)printJobManager->codeEditor->document->deviceMode->dmDeviceName, &hPrinter, NULL);

	//Get amount of memory needed
	EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, NULL, 0, &dwNeeded, &dwReturned);
	//Allocate the memory, if you cant end function
	pJobInfo = (JOB_INFO_1*)malloc(dwNeeded);

	//Get job info structure
	EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, (LPBYTE)pJobInfo, dwNeeded, &dwNeeded, &dwReturned);
	while (dwReturned > 0 && printJobManager->isChecking == TRUE) {
		EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, (LPBYTE)pJobInfo, dwNeeded, &dwNeeded, &dwReturned);
	}

	ClosePrinter(hPrinter);
	free(pJobInfo);

	if (dwReturned <= 0 && printJobManager->isChecking == TRUE) {
		printJobManager->codeEditor->SendMessage(WM_THREADNOTIFY, 0, 0);
	}

	printJobManager->isChecking = FALSE;

	return 0;
}