#include "FileManager.h"
#include "Files.h"
#include "FileFactory.h"
#include "CodeEditor.h"
#include "CodeEditingForm.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Scanner.h"
#include "../TextEditor/ScrollController.h"
#include "Document.h"

FileManager::FileManager(CodeEditor* codeEditor) {
	this->codeEditor = codeEditor;
}

FileManager::FileManager(const FileManager& source) {
	this->codeEditor = source.codeEditor;
}

FileManager::~FileManager() {

}

FileManager& FileManager::operator=(const FileManager& source) {
	this->codeEditor = source.codeEditor;

	return *this;
}

void FileManager::Save() {
	string filePathName = this->codeEditor->document->GetPathName();
	string encodingType = this->codeEditor->document->GetEncodingType();

	FileFactory fileFactory;
	File* file = fileFactory.MakeSaveFile(filePathName, encodingType);

	string content = this->codeEditor->codeEditingForm->note->GetContent();

	file->Save(content);

	if (file != 0) {
		delete file;
	}
}

void FileManager::Load() {
	string filePathName = this->codeEditor->document->GetPathName();

	FileFactory fileFactory;
	File *file = fileFactory.MakeOpenFile(filePathName);
	this->codeEditor->document->SetEncodingType(file->GetType());
	string content = file->Load();

	if (file != 0) {
		delete file;
	}

	if (this->codeEditor->codeEditingForm->note != NULL) {
		delete this->codeEditor->codeEditingForm->note;
	}
	GlyphFactory glyphFactory;
	this->codeEditor->codeEditingForm->note = glyphFactory.Make("");
	this->codeEditor->codeEditingForm->current = glyphFactory.Make("\r\n");
	this->codeEditor->codeEditingForm->note->Add(this->codeEditor->codeEditingForm->current);
	Scanner scanner(content);
	while (scanner.IsEnd() == FALSE) {
		string token = scanner.GetToken();
		Glyph* glyph = glyphFactory.Make(token.c_str());
		if (token != "\r\n") {
			this->codeEditor->codeEditingForm->current->Add(glyph);
		}
		else {
			Long index = this->codeEditor->codeEditingForm->note->Add(glyph);
			this->codeEditor->codeEditingForm->current = this->codeEditor->codeEditingForm->note->GetAt(index);
		}
		scanner.Next();
	}

	Long index = this->codeEditor->codeEditingForm->note->First();
	this->codeEditor->codeEditingForm->current = this->codeEditor->codeEditingForm->note->GetAt(index);
	this->codeEditor->codeEditingForm->current->First();

	if (this->codeEditor->codeEditingForm->scrollController != NULL) {
		delete this->codeEditor->codeEditingForm->scrollController;
		this->codeEditor->codeEditingForm->scrollController = new ScrollController(this->codeEditor->codeEditingForm);
	}
	this->codeEditor->codeEditingForm->Notify();
}

void FileManager::New() {
	if (this->codeEditor->codeEditingForm->note != NULL) {
		delete this->codeEditor->codeEditingForm->note;
	}
	GlyphFactory glyphFactory;
	this->codeEditor->codeEditingForm->note = glyphFactory.Make("");
	Glyph* line = glyphFactory.Make("\r\n");
	this->codeEditor->codeEditingForm->note->Add(line);

	Long index = this->codeEditor->codeEditingForm->note->First();
	this->codeEditor->codeEditingForm->current = this->codeEditor->codeEditingForm->note->GetAt(index);
	this->codeEditor->codeEditingForm->current->First();

	if (this->codeEditor->codeEditingForm->scrollController != NULL) {
		delete this->codeEditor->codeEditingForm->scrollController;
		this->codeEditor->codeEditingForm->scrollController = new ScrollController(this->codeEditor->codeEditingForm);
	}
	this->codeEditor->codeEditingForm->Notify();
}