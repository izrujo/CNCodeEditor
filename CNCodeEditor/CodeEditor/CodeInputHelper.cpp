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
	while (i < list->GetLength()) { //1. 리스트의 토큰 개수만큼 반복한다.
		token = list->GetAt(i); //1.1. 토큰을 가져오다.
		tokenContent = token->GetContent(); //1.2. 토큰의 내용을 가져오다.
		content = this->codeEditingForm->current->GetContent();
		index = content.Find(tokenContent, index); //1.3. 현재 줄의 내용에서 토큰의 내용을 찾다.
		// 찾았으면 그리고 맨앞이아니면
		if (index > 0) {
			// 현재 토큰이 연산자고 첨자연산자가 아니고 포인터 연산자가 아니고 이전글자(index-1)이 공백이 아니면 앞에 공백을 입력한다.
			//포인터 연산자 = 직전 토큰이 식별자가 아닌 *
			if (token->GetType() == "Operator" && (token->GetContent() != "[" && token->GetContent() != "]" && token->GetContent() != "[]")
				&& !(i > 0 && token->GetContent() == "*" 
					&& list->GetAt(i - 1)->GetType() != "Identifier" && list->GetAt(i - 1)->GetType() != "SymbolicConstant")
				&& this->codeEditingForm->current->GetAt(index - 1)->GetContent() != " ") {
				this->codeEditingForm->current->Add(index, blank->Clone());
				count++;
				index++;
			}
			// 현재 토큰이 구두점이고 내용이 { 이고 이전글자가 공백이 아니면 앞에 공백을 입력한다.
			else if (token->GetType() == "Punctuation" && token->GetContent() == "{"
				&& this->codeEditingForm->current->GetAt(index - 1)->GetContent() != " ") {
				this->codeEditingForm->current->Add(index, blank->Clone());
				count++;
				index++;
			}
		}
		j = 0;
		//1.4. 토큰 내용의 글자 개수만큼 반복한다.
		while (j < tokenContent.GetLength() && index != -1) {
			character = this->codeEditingForm->current->GetAt(index++); //1.4.1. 줄에서 현재 토큰에 해당하는 글자를 가져오다.
			if (dynamic_cast<DoubleByteCharacter*>(character)) {
				j++;
			}
			j++;
		}
		this->codeEditingForm->current->Move(index);
		//1.5. 현재 토큰이 키워드고 다음글자(index)가 공백이 아니면 공백을 입력한다.
		if (token->GetType() == "Keyword" && this->codeEditingForm->current->GetAt(index)->GetContent() != " ") {
			this->codeEditingForm->current->Add(index, blank->Clone());
			count++;
		}
		//1.6. 현재 토큰이 구두점이고 내용이 , 이고 다음글자가 공백이 아니면 공백을 입력한다.
		else if (token->GetType() == "Punctuation" && token->GetContent() == ","
			&& this->codeEditingForm->current->GetAt(index)->GetContent() != " ") {
			this->codeEditingForm->current->Add(index, blank->Clone());
			count++;
		}
		//1.7. 현재 토큰이 연산자이고 다음 글자가 공백이 아니면 공백을 입력한다.
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