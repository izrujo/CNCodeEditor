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
	//1. ��Ʈ ����� ���ִ�.
	if (this->codeEditingForm->root != NULL) {
		delete this->codeEditingForm->root;
	}

	//2. ��Ʈ ����� �����.
	Long level = 0;
	Long noteLength = this->codeEditingForm->note->GetLength();
	Long tail = noteLength - 1;
	this->codeEditingForm->root = new Block(level, 0, tail);

	//3. �� ������ŭ �ݺ��ϴ�.
	Block* now = this->codeEditingForm->root;
	Block* block;
	Glyph* line;
	Glyph* character;
	string content;
	Long j;
	Long i = 0;
	while (i < noteLength) {
		line = this->codeEditingForm->note->GetAt(i);
		//3.1. ���� ������ŭ �ݺ��ϴ�.
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			content = character->GetContent();
			//3.1.1. ������ ������ { �̸�
			if (content == "{") {
				//3.1.1.1. ������ �ø���.
				level++;
				//3.1.1.2. ����� �����.
				block = new Block(level, i, tail);
				//3.1.1.3. ���� ��Ͽ� �߰��ϴ�.
				now->Add(block);
				//3.1.1.4. �߰��� ���� ������� �̵��ϴ�.
				now = block;
			}
			//3.1.2. ������ ������ } �̸�
			else if (content == "}") {
				//3.1.2.1. ���� ����� �ݴ�.
				now->Close(i);
				//3.1.2.2. ������ ������.
				level--;
				//3.1.2.3. ���� ������� �̵��ϴ�.
				now = this->codeEditingForm->root->Find(i, level);
			}
			j++;
		}
		i++;
	}
}