#include "TokenVisitors.h"
#include "TokenBook.h"
#include "TokenList.h"
#include "Tokens.h"
#include "TokenFactory.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/Note.h"
#include "../TextEditor/Line.h"
#include "../TextEditor/Characters.h"

#include "../Utilities/String.h"

TokenMakingVisitor::TokenMakingVisitor(TokenBook* tokenBook, TokenFactory* tokenFactory)
	: current("") {
	this->tokenBook = tokenBook;
	this->tokenFactory = tokenFactory;
	this->smallLiteral = false;
	this->bigLiteral = false;
	this->singleAnnotation = false;
	this->multiAnnotation = false;
}

TokenMakingVisitor::~TokenMakingVisitor() {

}

void TokenMakingVisitor::Visit(Note* note) {
	TokenList* list;
	Glyph* line;

	Long i = 0;
	while (i < note->GetLength()) {
		list = new TokenList;
		this->tokenBook->Add(list);

		line = note->GetAt(i);
		line->Accept(this);

		i++;
	}
}

void TokenMakingVisitor::Visit(Line* line) {
	this->tokenFactory->SetLiteralState(false);
	this->tokenFactory->SetAnnotationState(false);
	this->current = "";
	this->smallLiteral = false;
	this->bigLiteral = false;

	Glyph* character;
	Long i = 0;
	while (i < line->GetLength()) {
		character = line->GetAt(i);
		character->Accept(this);
		i++;
	}

	if (this->singleAnnotation == true && this->tokenFactory->GetAnnotationState() == true) {
		this->singleAnnotation = false;
		this->tokenFactory->SetAnnotationState(false);
	}
}

void TokenMakingVisitor::Visit(Character* character) {
	//1. ���� �ٿ� �ش��ϴ� ����Ʈ�� �����´�.
	TokenList* list = this->tokenBook->GetAt(this->tokenBook->GetCurrent());
	Token* token;

	string content = character->GetContent();

	bool literalState = this->tokenFactory->GetLiteralState();
	bool annotationState = this->tokenFactory->GetAnnotationState();
	bool isOperator = this->tokenFactory->SearchIsOperator(content);
	bool isPunctuation = this->tokenFactory->SearchIsPunctuation(content);
	bool isCurrentOperator = this->tokenFactory->SearchIsOperator(this->current);
	bool isCurrentPunctuation = this->tokenFactory->SearchIsPunctuation(this->current);
	//2. ���� ������ �����̸� ����(���ڿ�)�� �ʱ�ȭ�Ѵ�. 
	// ���ͷ� ���� �Ǵ� ���� �ּ� ���� �Ǵ� ���� �ּ� ���°� �ƴϰ�, (���ͷ� �Ǵ� �ּ��̸� ��� ���ڰ� ���й��ڰ� �ƴϴ�.)
	// �� �Ǵ� ���� �Ǵ� ���� �Ǵ� " �Ǵ� ' �Ǵ� /(�����ڿ����) �Ǵ�
	// ������ �Ǵ�
	// ���� ���ڰ� �������̸� �������� �ƴ� ��� ���� �Ǵ�
	// ���� ���ڰ� �����ڰ� �ƴϸ� ������ �Ǵ�
	// ���� ���ڰ� �������̸� �����ڰ� �ƴ� ��� ���� �Ǵ�
	// ���� ���ڰ� �� �Ǵ� ���� �Ǵ� " �Ǵ� ' �̸�(���ͷ��ƴѻ���) ��� ���ڰ� ���� ������ �����̴�.
	if (literalState == false && annotationState == false
		&& (content == "\t" || content == " " || content == "\"" || content == "\'"
			|| isPunctuation == true
			|| (isCurrentPunctuation == true && isPunctuation == false)
			|| (isCurrentOperator == false && isOperator == true)
			|| (isCurrentOperator == true && isOperator == false)
			|| (this->current == "\t" || this->current == " "
				|| this->current.length() > 0 && (this->current.at(this->current.length() - 1) == '\"'
					|| this->current.at(this->current.length() - 1) == '\'')))) {
		this->current = "";
	}

	//3. ����(���ڿ�)�� ������ ������ ���Ѵ�.
	this->current += content;

	if (content == "\"") { // <> �� ���ͷ��� ����غ���.
		if (literalState == false) {
			this->bigLiteral = true;
		}
		else if (this->bigLiteral == true) {
			this->bigLiteral = false;
		}
	}
	else if (content == "\'") { // <> �� ���ͷ��� ����غ���.
		if (literalState == false) {
			this->smallLiteral = true;
		}
		else if (this->smallLiteral == true) {
			this->smallLiteral = false;
		}
	}

	if (this->current.length() > 1) {
		if (this->current.at(this->current.length() - 2) == '/') {
			if (content == "/" && this->singleAnnotation == false && this->multiAnnotation == false) {
				this->singleAnnotation = true;
			}
			else if (content == "*" && this->multiAnnotation == false && this->singleAnnotation == false) {
				this->multiAnnotation = true;
			}
		}
		else if (this->current.at(this->current.length() - 2) == '*'
			&& content == "/" && this->multiAnnotation == true) {
			this->multiAnnotation = false;
		}
	}

	//4. ����(���ڿ�)�� ��ū�� �����.
	token = this->tokenFactory->Make(this->current);
	if (token != 0) { //5. ��ū�� �����������
		if (this->current.length() > 1 && list->GetLength() > 0) { //5.1. ����(���ڿ�)�� ���� ���� 2�� �̻��̸� ������ ��ū�� �����.
			list->Remove(list->GetLength() - 1);
		}
		//5.2. ��ū�� ����Ʈ�� �߰��ϴ�.
		list->Add(token);
	}

	if (this->smallLiteral == true || this->bigLiteral == true) {
		this->tokenFactory->SetLiteralState(true);
	}
	else {
		this->tokenFactory->SetLiteralState(false);
	}

	if (this->singleAnnotation == true || this->multiAnnotation == true) {
		this->tokenFactory->SetAnnotationState(true);
	}
	else {
		this->tokenFactory->SetAnnotationState(false);
	}
}

void TokenMakingVisitor::Visit(Book* book) {
	//Not implement
}

//TokenDrawingVisitor
TokenDrawingVisitor::TokenDrawingVisitor(TokenBook* tokenBook) {
	this->tokenBook = tokenBook;
}

TokenDrawingVisitor::~TokenDrawingVisitor() {

}

void TokenDrawingVisitor::Visit(Note* note) {
	Glyph* line;
	Long i = 0;
	while (i < note->GetLength()) {
		line = note->GetAt(i);
		this->tokenBook->Move(i);
		line->Accept(this);
		i++;
	}
}

void TokenDrawingVisitor::Visit(Line* line) {
	Glyph* character;
	String content = line->GetContent();
	String tokenContent;
	TokenList* list = this->tokenBook->GetAt(this->tokenBook->GetCurrent());
	Token* token;
	Long j;
	Long index = 0;
	Long i = 0;
	while (i < list->GetLength()) {
		token = list->GetAt(i);
		tokenContent = token->GetContent();
		index = content.Find(tokenContent, index);
		j = 0;
		while (j < tokenContent.GetLength() && index != -1) {
			character = line->GetAt(index++);
			character->Paint(token->GetColor());
			if (dynamic_cast<DoubleByteCharacter*>(character)) {
				j++;
			}
			j++;
		}
		i++;
	}
}

void TokenDrawingVisitor::Visit(Character* character) {

}

void TokenDrawingVisitor::Visit(Book* book) {

}

#if 0
//test
#include <iostream>

int main(int argc, char* argv[]) {
	string nowContent[12];
	nowContent[0] = "#include <stdio.h>";
	nowContent[1] = "";
	nowContent[2] = "int main(int argc, char*argv[]){";
	nowContent[3] = "\tint i = 1; // �ʱ�ȭ";
	nowContent[4] = "\twhile(i<=10){";
	nowContent[5] = "\t\tprintf(\"%d\\n\", i);";
	nowContent[6] = "\t\ti++;";
	nowContent[7] = "\t}/*�ݺ���";
	nowContent[8] = "  ffffffqwerqwerqwer\t";
	nowContent[9] = "\t123123123123*/";
	nowContent[10] = "\treturn 0;";
	nowContent[11] = "}";

	bool literalState = false;
	bool singleAnnotationState = false;
	bool multiAnnotationState = false;
	TokenBook tokenBook;
	TokenFactory tokenFactory;
	Long a = 0;
	while (a < 11) {
		printf("\nTokenList %d\n", a + 1);
		TokenList* tokenList = new TokenList;
		tokenBook.Add(tokenList);
		literalState = false;
		singleAnnotationState = false;
		string current = "";
		Long b = 0;
		while (b < nowContent[a].length()) {

			//1. ���� �ٿ� �ش��ϴ� ����Ʈ�� �����´�.
			Token* token;

			string content(1, nowContent[a].at(b));
			bool isOperator = this->tokenFactory->SearchIsOperator(content);
			bool isPunctuation = this->tokenFactory->SearchIsPunctuation(content);
			bool isCurrentOperator = this->tokenFactory->SearchIsOperator(current);
			bool isCurrentPunctuation = this->tokenFactory->SearchIsPunctuation(current);
			//2. ���� ������ �����̸� ����(���ڿ�)�� �ʱ�ȭ�Ѵ�. 
			// ���ͷ� ���� �Ǵ� ���� �ּ� ���� �Ǵ� ���� �ּ� ���°� �ƴϰ�, (���ͷ� �Ǵ� �ּ��̸� ��� ���ڰ� ���й��ڰ� �ƴϴ�.
			// 1 �� �Ǵ� ���� �Ǵ� ���� �Ǵ� " �Ǵ� ' �Ǵ� /(�����ڿ����) �Ǵ�
			// 2 ������ �Ǵ� 
			// 3 ����(���ڿ�)�� �������̸� �������� ������ ��� ���� �Ǵ�
			// 4 ����(���ڿ�)�� �������̸� �����ڸ� ������ ��� ���� �Ǵ�
			// 5 ����(���ڿ�)�� �����ڰ� �ƴѵ� �������̸�
			// 6 ����(���ڿ�)�� �� �Ǵ� �����̸� ��� ����
			// ���� �ּ� �����̸� ���� ���� ��� ���ڰ� ���ԵǾ���Ѵ�.
			if (literalState == false && singleAnnotationState == false && multiAnnotationState == false
				&& (content == "\t" || content == " " || content == "\"" || content == "\'"
					|| isPunctuation == true
					|| (isCurrentPunctuation == true && isPunctuation == false)
					|| (isCurrentOperator == true && isOperator == false)
					|| (isCurrentOperator == false && isOperator == true)
					|| (current == "\t" || current == " "))) {
				current = "";
			}

			//3. ����(���ڿ�)�� ������ ������ ���Ѵ�.
			current += content;

			//4. ����(���ڿ�) �� ���� "�̸�
			if (current.at(0) == '\"') {
				//4.1. ���ͷ� �����̴�.
				literalState = true;
				//4.2. �� �ڰ� "�̸� ���ͷ� ���°� �ƴϴ�.
				if (current.length() > 1 && current.at(current.length() - 1) == '\"') {
					literalState = false;
				}
			}

			//5. ����(���ڿ�) �� ���� '�̸�
			if (current.at(0) == '\'') {
				//5.1. ���ͷ� �����̴�.
				literalState = true;
				//5.2. �� �ڰ� '�̸� ���ͷ� ���°� �ƴϴ�.
				if (current.length() > 1 && current.at(current.length() - 1) == '\'') {
					literalState = false;
				}
			}

			//6. ����(���ڿ�)�� 2�� �̻��̸�
			if (current.length() > 1) {
				//6.1. �� ���� // �̸� ���� �ּ� �����̴�.
				//6.2. �� ���� /* �̸� ���� �ּ� �����̴�.
				if (current.at(0) == '/') {
					if (current.at(1) == '/') {
						singleAnnotationState = true;
					}
					else if (current.at(1) == '*') {
						multiAnnotationState = true;
					}
				}
			}

			//4. ����(���ڿ�)�� ��ū�� �����.
			token = this->tokenFactory->Make(current);
			if (token != 0) { //5. ��ū�� �����������
				if (current.length() > 1 && tokenList->GetLength() > 0) { //5.1. ����(���ڿ�)�� ���� ���� 2�� �̻��̸� ������ ��ū�� �����.
					tokenList->Remove(tokenList->GetLength() - 1);
				}
				//5.2. ��ū�� ����Ʈ�� �߰��ϴ�.
				tokenList->Add(token);
			}

			if (current.length() > 1
				&& (current.at(current.length() - 1) == '/'
					&& current.at(current.length() - 2) == '*')) {
				multiAnnotationState = false;
			}

			//���
			if (token != 0) {
				cout << tokenList->GetAt(tokenList->GetLength() - 1)->GetContent() << " : "
					<< tokenList->GetAt(tokenList->GetLength() - 1)->GetType()
					<< endl;

			}
			else {
				cout << "No Added" << endl;
			}

			b++;
		}

		Token* token;
		if (multiAnnotationState == true && tokenList->GetLength() < 1) {
			token = new Annotation(current);
			tokenList->Add(token);
		}

		tokenList = tokenBook.GetAt(a);
		b = 0;
		while (b < tokenList->GetLength()) {
			token = tokenList->GetAt(b);
			cout << endl << b + 1 << " " << token->GetContent() << " : " << token->GetType() << endl;
			b++;
		}

		a++;
	}


	return 0;
}
#endif