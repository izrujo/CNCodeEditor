#include "FileManager.h"
#include "Files.h"
#include "FileFactory.h"
#include "CodeEditingForm.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Scanner.h"
#include "../TextEditor/ScrollController.h"
#include "Document.h"

FileManager::FileManager(CodeEditingForm* codeEditingForm) {
	this->codeEditingForm = codeEditingForm;
}

FileManager::FileManager(const FileManager& source) {
	this->codeEditingForm = source.codeEditingForm;
}

FileManager::~FileManager() {

}

FileManager& FileManager::operator=(const FileManager& source) {
	this->codeEditingForm = source.codeEditingForm;

	return *this;
}

void FileManager::Save() {
	string filePathName = this->codeEditingForm->document->GetPathName();
	string encodingType = this->codeEditingForm->document->GetEncodingType();

	FileFactory fileFactory;
	File* file = fileFactory.MakeSaveFile(filePathName, encodingType);

	string content = this->codeEditingForm->textEditingForm->note->GetContent();

	file->Save(content);

	if (file != 0) {
		delete file;
	}
}

void FileManager::Load() {
	string filePathName = this->codeEditingForm->document->GetPathName();

	FileFactory fileFactory;
	File *file = fileFactory.MakeOpenFile(filePathName);
	this->codeEditingForm->document->SetEncodingType(file->GetType());
	string content = file->Load();

	if (file != 0) {
		delete file;
	}

	if (this->codeEditingForm->textEditingForm->note != NULL) {
		delete this->codeEditingForm->textEditingForm->note;
	}
	GlyphFactory glyphFactory;
	this->codeEditingForm->textEditingForm->note = glyphFactory.Make("");
	this->codeEditingForm->textEditingForm->current = glyphFactory.Make("\r\n");
	this->codeEditingForm->textEditingForm->note->Add(this->codeEditingForm->textEditingForm->current);
	Scanner scanner(content);
	while (scanner.IsEnd() == FALSE) {
		string token = scanner.GetToken();
		Glyph* glyph = glyphFactory.Make(token.c_str());
		if (token != "\r\n") {
			this->codeEditingForm->textEditingForm->current->Add(glyph);
		}
		else {
			Long index = this->codeEditingForm->textEditingForm->note->Add(glyph);
			this->codeEditingForm->textEditingForm->current = this->codeEditingForm->textEditingForm->note->GetAt(index);
		}
		scanner.Next();
	}

	Long index = this->codeEditingForm->textEditingForm->note->First();
	this->codeEditingForm->textEditingForm->current = this->codeEditingForm->textEditingForm->note->GetAt(index);
	this->codeEditingForm->textEditingForm->current->First();

	if (this->codeEditingForm->textEditingForm->scrollController != NULL) {
		delete this->codeEditingForm->textEditingForm->scrollController;
		this->codeEditingForm->textEditingForm->scrollController = new ScrollController(this->codeEditingForm->textEditingForm);
	}
	this->codeEditingForm->textEditingForm->Notify();
}

void FileManager::New() {
	if (this->codeEditingForm->textEditingForm->note != NULL) {
		delete this->codeEditingForm->textEditingForm->note;
	}
	GlyphFactory glyphFactory;
	this->codeEditingForm->textEditingForm->note = glyphFactory.Make("");
	Glyph* line = glyphFactory.Make("\r\n");
	this->codeEditingForm->textEditingForm->note->Add(line);

	Long index = this->codeEditingForm->textEditingForm->note->First();
	this->codeEditingForm->textEditingForm->current = this->codeEditingForm->textEditingForm->note->GetAt(index);
	this->codeEditingForm->textEditingForm->current->First();

	if (this->codeEditingForm->textEditingForm->scrollController != NULL) {
		delete this->codeEditingForm->textEditingForm->scrollController;
		this->codeEditingForm->textEditingForm->scrollController = new ScrollController(this->codeEditingForm->textEditingForm);
	}
	this->codeEditingForm->textEditingForm->Notify();
}