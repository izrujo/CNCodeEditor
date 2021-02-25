#include "CodeInputHelper.h"
#include "CodeEditingForm.h"
#include "Block.h"

#include "../TextEditor/Glyph.h"

CodeInputHelper::CodeInputHelper(CodeEditingForm* codeEditingForm) {
	this->codeEditingForm = codeEditingForm;
}

CodeInputHelper::CodeInputHelper(const CodeInputHelper& source) {
	this->codeEditingForm = source.codeEditingForm;
}

CodeInputHelper::~CodeInputHelper() {

}

CodeInputHelper& CodeInputHelper::operator=(const CodeInputHelper& source) {
	this->codeEditingForm = source.codeEditingForm;

	return *this;
}

void CodeInputHelper::Help() {
	//1. 루트 블록을 없애다.
	if (this->codeEditingForm->root != NULL) {
		delete this->codeEditingForm->root;
	}

	//2. 루트 블록을 만들다.
	Long level = 0;
	Long noteLength = this->codeEditingForm->note->GetLength();
	Long tail = noteLength - 1;
	this->codeEditingForm->root = new Block(level, 0, tail);

	//3. 줄 개수만큼 반복하다.
	Block* now = this->codeEditingForm->root;
	Block* block;
	Glyph* line;
	Glyph* character;
	string content;
	Long j;
	Long i = 0;
	while (i < noteLength) {
		line = this->codeEditingForm->note->GetAt(i);
		//3.1. 글자 개수만큼 반복하다.
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			content = character->GetContent();
			//3.1.1. 글자의 내용이 { 이면
			if (content == "{") {
				//3.1.1.1. 레벨을 올리다.
				level++;
				//3.1.1.2. 블록을 만들다.
				block = new Block(level, i, tail);
				//3.1.1.3. 현재 블록에 추가하다.
				now->Add(block);
				//3.1.1.4. 추가한 하위 블록으로 이동하다.
				now = block;
			}
			//3.1.2. 글자의 내용이 } 이면
			else if (content == "}") {
				//3.1.2.1. 현재 블록을 닫다.
				now->Close(i);
				//3.1.2.2. 레벨을 내리다.
				level--;
				//3.1.2.3. 상위 블록으로 이동하다.
				now = this->codeEditingForm->root->Find(i, level);
			}
			j++;
		}
		i++;
	}
}