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
	//1. 현재 줄에 해당하는 리스트를 가져온다.
	TokenList* list = this->tokenBook->GetAt(this->tokenBook->GetCurrent());
	Token* token;

	string content = character->GetContent();

	bool literalState = this->tokenFactory->GetLiteralState();
	bool annotationState = this->tokenFactory->GetAnnotationState();
	bool isOperator = this->tokenFactory->SearchIsOperator(content);
	bool isPunctuation = this->tokenFactory->SearchIsPunctuation(content);
	bool isCurrentOperator = this->tokenFactory->SearchIsOperator(this->current);
	bool isCurrentPunctuation = this->tokenFactory->SearchIsPunctuation(this->current);
	//2. 구분 가능한 문자이면 현재(문자열)를 초기화한다. 
	// 리터럴 상태 또는 단행 주석 상태 또는 다행 주석 상태가 아니고, (리터럴 또는 주석이면 모든 문자가 구분문자가 아니다.)
	// 탭 또는 공백 또는 개행 또는 " 또는 ' 또는 /(연산자에묻어감) 또는
	// 구두점 또는
	// 이전 글자가 구두점이면 구두점이 아닌 모든 문자 또는
	// 이전 글자가 연산자가 아니면 연산자 또는
	// 이전 글자가 연산자이면 연산자가 아닌 모든 문자 또는
	// 이전 글자가 탭 또는 공백 또는 " 또는 ' 이면(리터럴아닌상태) 모든 문자가 구분 가능한 문자이다.
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

	//3. 현재(문자열)에 글자의 내용을 더한다.
	this->current += content;

	if (content == "\"") { // <> 안 리터럴도 고민해보기.
		if (literalState == false) {
			this->bigLiteral = true;
		}
		else if (this->bigLiteral == true) {
			this->bigLiteral = false;
		}
	}
	else if (content == "\'") { // <> 안 리터럴도 고민해보기.
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

	//4. 현재(문자열)로 토큰을 만들다.
	token = this->tokenFactory->Make(this->current);
	if (token != 0) { //5. 토큰이 만들어졌으면
		if (this->current.length() > 1 && list->GetLength() > 0) { //5.1. 현재(문자열)의 글자 수가 2개 이상이면 마지막 토큰을 지우다.
			list->Remove(list->GetLength() - 1);
		}
		//5.2. 토큰을 리스트에 추가하다.
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
	nowContent[3] = "\tint i = 1; // 초기화";
	nowContent[4] = "\twhile(i<=10){";
	nowContent[5] = "\t\tprintf(\"%d\\n\", i);";
	nowContent[6] = "\t\ti++;";
	nowContent[7] = "\t}/*반복문";
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

			//1. 현재 줄에 해당하는 리스트를 가져온다.
			Token* token;

			string content(1, nowContent[a].at(b));
			bool isOperator = this->tokenFactory->SearchIsOperator(content);
			bool isPunctuation = this->tokenFactory->SearchIsPunctuation(content);
			bool isCurrentOperator = this->tokenFactory->SearchIsOperator(current);
			bool isCurrentPunctuation = this->tokenFactory->SearchIsPunctuation(current);
			//2. 구분 가능한 문자이면 현재(문자열)를 초기화한다. 
			// 리터럴 상태 또는 단행 주석 상태 또는 다행 주석 상태가 아니고, (리터럴 또는 주석이면 모든 문자가 구분문자가 아니다.
			// 1 탭 또는 공백 또는 개행 또는 " 또는 ' 또는 /(연산자에묻어감) 또는
			// 2 구두점 또는 
			// 3 현재(문자열)가 구두점이면 구두점을 제외한 모든 문자 또는
			// 4 현재(문자열)가 연산자이면 연산자를 제외한 모든 문자 또는
			// 5 현재(문자열)가 연산자가 아닌데 연산자이면
			// 6 현재(문자열)가 탭 또는 공백이면 모든 문자
			// 다행 주석 상태이면 공백 포함 모든 문자가 포함되어야한다.
			if (literalState == false && singleAnnotationState == false && multiAnnotationState == false
				&& (content == "\t" || content == " " || content == "\"" || content == "\'"
					|| isPunctuation == true
					|| (isCurrentPunctuation == true && isPunctuation == false)
					|| (isCurrentOperator == true && isOperator == false)
					|| (isCurrentOperator == false && isOperator == true)
					|| (current == "\t" || current == " "))) {
				current = "";
			}

			//3. 현재(문자열)에 글자의 내용을 더한다.
			current += content;

			//4. 현재(문자열) 맨 앞이 "이면
			if (current.at(0) == '\"') {
				//4.1. 리터럴 상태이다.
				literalState = true;
				//4.2. 맨 뒤가 "이면 리터럴 상태가 아니다.
				if (current.length() > 1 && current.at(current.length() - 1) == '\"') {
					literalState = false;
				}
			}

			//5. 현재(문자열) 맨 앞이 '이면
			if (current.at(0) == '\'') {
				//5.1. 리터럴 상태이다.
				literalState = true;
				//5.2. 맨 뒤가 '이면 리터럴 상태가 아니다.
				if (current.length() > 1 && current.at(current.length() - 1) == '\'') {
					literalState = false;
				}
			}

			//6. 현재(문자열)가 2개 이상이면
			if (current.length() > 1) {
				//6.1. 맨 앞이 // 이면 단행 주석 상태이다.
				//6.2. 맨 앞이 /* 이면 다행 주석 상태이다.
				if (current.at(0) == '/') {
					if (current.at(1) == '/') {
						singleAnnotationState = true;
					}
					else if (current.at(1) == '*') {
						multiAnnotationState = true;
					}
				}
			}

			//4. 현재(문자열)로 토큰을 만들다.
			token = this->tokenFactory->Make(current);
			if (token != 0) { //5. 토큰이 만들어졌으면
				if (current.length() > 1 && tokenList->GetLength() > 0) { //5.1. 현재(문자열)의 글자 수가 2개 이상이면 마지막 토큰을 지우다.
					tokenList->Remove(tokenList->GetLength() - 1);
				}
				//5.2. 토큰을 리스트에 추가하다.
				tokenList->Add(token);
			}

			if (current.length() > 1
				&& (current.at(current.length() - 1) == '/'
					&& current.at(current.length() - 2) == '*')) {
				multiAnnotationState = false;
			}

			//출력
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