#include "CodeInputHelper.h"
#include "CodeEditingForm.h"
#include "Block.h"
#include "Tokens.h"
#include "TokenList.h"
#include "TokenBook.h"

#include "../TextEditor/Glyph.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Characters.h"
#include "../TextEditor/resource.h"

#include "../Utilities/String.h"

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

void CodeInputHelper::UpdateBlock() {
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

void CodeInputHelper::CleanRow() {
	GlyphFactory glyphFactory;
	Glyph* blank = glyphFactory.Make(" ");
	Long count = 0;
	Long column = this->codeEditingForm->current->GetCurrent();
	this->codeEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_HOME, 0));
	Long current = this->codeEditingForm->tokenBook->Move(this->codeEditingForm->note->GetCurrent());
	TokenList* list = this->codeEditingForm->tokenBook->GetAt(current);
	Glyph* character;
	String content;
	String tokenContent;
	Token* token;
	Long j;
	Long index = 0;
	Long i = 0;
	while (i < list->GetLength()) { //1. ����Ʈ�� ��ū ������ŭ �ݺ��Ѵ�.
		token = list->GetAt(i); //1.1. ��ū�� ��������.
		tokenContent = token->GetContent(); //1.2. ��ū�� ������ ��������.
		content = this->codeEditingForm->current->GetContent();
		index = content.Find(tokenContent, index); //1.3. ���� ���� ���뿡�� ��ū�� ������ ã��.
		// ã������ �׸��� �Ǿ��̾ƴϸ�
		if (index > 0) {
			// ���� ��ū�� �����ڰ� ÷�ڿ����ڰ� �ƴϰ� ������ �����ڰ� �ƴϰ� ��������(index-1)�� ������ �ƴϸ� �տ� ������ �Է��Ѵ�.
			//������ ������ = ���� ��ū�� �ĺ��ڰ� �ƴ� *
			if (token->GetType() == "Operator" && (token->GetContent() != "[" && token->GetContent() != "]" && token->GetContent() != "[]")
				&& !(i > 0 && token->GetContent() == "*" 
					&& list->GetAt(i - 1)->GetType() != "Identifier" && list->GetAt(i - 1)->GetType() != "SymbolicConstant")
				&& this->codeEditingForm->current->GetAt(index - 1)->GetContent() != " ") {
				this->codeEditingForm->current->Add(index, blank->Clone());
				count++;
				index++;
			}
			// ���� ��ū�� �������̰� ������ { �̰� �������ڰ� ������ �ƴϸ� �տ� ������ �Է��Ѵ�.
			else if (token->GetType() == "Punctuation" && token->GetContent() == "{"
				&& this->codeEditingForm->current->GetAt(index - 1)->GetContent() != " ") {
				this->codeEditingForm->current->Add(index, blank->Clone());
				count++;
				index++;
			}
		}
		j = 0;
		//1.4. ��ū ������ ���� ������ŭ �ݺ��Ѵ�.
		while (j < tokenContent.GetLength() && index != -1) {
			character = this->codeEditingForm->current->GetAt(index++); //1.4.1. �ٿ��� ���� ��ū�� �ش��ϴ� ���ڸ� ��������.
			if (dynamic_cast<DoubleByteCharacter*>(character)) {
				j++;
			}
			j++;
		}
		this->codeEditingForm->current->Move(index);
		//1.5. ���� ��ū�� Ű����� ��������(index)�� ������ �ƴϸ� ������ �Է��Ѵ�.
		if (token->GetType() == "Keyword" && this->codeEditingForm->current->GetAt(index)->GetContent() != " ") {
			this->codeEditingForm->current->Add(index, blank->Clone());
			count++;
		}
		//1.6. ���� ��ū�� �������̰� ������ , �̰� �������ڰ� ������ �ƴϸ� ������ �Է��Ѵ�.
		else if (token->GetType() == "Punctuation" && token->GetContent() == ","
			&& this->codeEditingForm->current->GetAt(index)->GetContent() != " ") {
			this->codeEditingForm->current->Add(index, blank->Clone());
			count++;
		}
		//1.7. ���� ��ū�� �������̰� ���� ���ڰ� ������ �ƴϸ� ������ �Է��Ѵ�.
		else if (token->GetType() == "Operator" && (token->GetContent() != "[" && token->GetContent() != "]" && token->GetContent() != "[]")
			&& this->codeEditingForm->current->GetAt(index)->GetContent() != " ") {
			this->codeEditingForm->current->Add(index, blank->Clone());
			count++;
		}
		i++;
	}
	this->codeEditingForm->current->Move(column + count);

	if (blank != NULL) {
		delete blank;
	}
}