#include "Tokens.h"

Token::Token(string content, int color) 
	: content(content) {
	this->color = color;
}

Token::Token(const Token& source) 
	: content(source.content) {
	this->color = source.color;
}

Token::~Token() {

}

Token& Token::operator=(const Token& source) {
	this->content = source.content;
	this->color = source.color;

	return *this;
}

//DataType
DataType::DataType(string content, int color) 
	: Token(content, color) {
}

DataType::DataType(const DataType& source) 
	: Token(source) {

}

DataType::~DataType() {

}

DataType& DataType::operator=(const DataType& source) {
	Token::operator=(source);

	return *this;
}

Token* DataType::Clone() {
	return new DataType(*this);
}

string DataType::GetType() {
	return "DataType";
}

//Keyword
Keyword::Keyword(string content, int color)
	: Token(content, color) {
}

Keyword::Keyword(const Keyword& source)
	: Token(source) {

}

Keyword::~Keyword() {

}

Keyword& Keyword::operator=(const Keyword& source) {
	Token::operator=(source);

	return *this;
}

Token* Keyword::Clone() {
	return new Keyword(*this);
}

string Keyword::GetType() {
	return "Keyword";
}

//Preprocessor
Preprocessor::Preprocessor(string content, int color)
	: Token(content, color) {
}

Preprocessor::Preprocessor(const Preprocessor& source)
	: Token(source) {

}

Preprocessor::~Preprocessor() {

}

Preprocessor& Preprocessor::operator=(const Preprocessor& source) {
	Token::operator=(source);

	return *this;
}

Token* Preprocessor::Clone() {
	return new Preprocessor(*this);
}

string Preprocessor::GetType() {
	return "Preprocessor";
}

//Operator
Operator::Operator(string content, int color)
	: Token(content, color) {
}

Operator::Operator(const Operator& source)
	: Token(source) {

}

Operator::~Operator() {

}

Operator& Operator::operator=(const Operator& source) {
	Token::operator=(source);

	return *this;
}

Token* Operator::Clone() {
	return new Operator(*this);
}

string Operator::GetType() {
	return "Operator";
}

//Punctuation
Punctuation::Punctuation(string content, int color)
	: Token(content, color) {
}

Punctuation::Punctuation(const Punctuation& source)
	: Token(source) {

}

Punctuation::~Punctuation() {

}

Punctuation& Punctuation::operator=(const Punctuation& source) {
	Token::operator=(source);

	return *this;
}

Token* Punctuation::Clone() {
	return new Punctuation(*this);
}

string Punctuation::GetType() {
	return "Punctuation";
}

//Identifier
Identifier::Identifier(string content, int color)
	: Token(content, color) {
}

Identifier::Identifier(const Identifier& source)
	: Token(source) {

}

Identifier::~Identifier() {

}

Identifier& Identifier::operator=(const Identifier& source) {
	Token::operator=(source);

	return *this;
}

Token* Identifier::Clone() {
	return new Identifier(*this);
}

string Identifier::GetType() {
	return "Identifier";
}

//SymbolicConstant
SymbolicConstant::SymbolicConstant(string content, int color)
	: Token(content, color) {
}

SymbolicConstant::SymbolicConstant(const SymbolicConstant& source)
	: Token(source) {

}

SymbolicConstant::~SymbolicConstant() {

}

SymbolicConstant& SymbolicConstant::operator=(const SymbolicConstant& source) {
	Token::operator=(source);

	return *this;
}

Token* SymbolicConstant::Clone() {
	return new SymbolicConstant(*this);
}

string SymbolicConstant::GetType() {
	return "SymbolicConstant";
}

//Literal
Literal::Literal(string content, int color)
	: Token(content, color) {
}

Literal::Literal(const Literal& source)
	: Token(source) {

}

Literal::~Literal() {

}

Literal& Literal::operator=(const Literal& source) {
	Token::operator=(source);

	return *this;
}

Token* Literal::Clone() {
	return new Literal(*this);
}

string Literal::GetType() {
	return "Literal";
}

//Annotation
Annotation::Annotation(string content, int color)
	: Token(content, color) {
}

Annotation::Annotation(const Annotation& source)
	: Token(source) {

}

Annotation::~Annotation() {

}

Annotation& Annotation::operator=(const Annotation& source) {
	Token::operator=(source);

	return *this;
}

Token* Annotation::Clone() {
	return new Annotation(*this);
}

string Annotation::GetType() {
	return "Annotation";
}