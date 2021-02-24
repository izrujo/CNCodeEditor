#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

using namespace std;

class Token {
public:
	Token(string content = "", int color = 0x000000);
	Token(const Token& source);
	virtual ~Token() = 0;
	Token& operator=(const Token& source);

	virtual Token* Clone() = 0;

	virtual string GetContent() const;
	virtual int GetColor() const;
	virtual string GetType() = 0;

protected:
	string content;
	int color;
};

inline string Token::GetContent() const {
	return this->content;
}

inline int Token::GetColor() const {
	return this->color;
}

//DataType
class DataType : public Token {
public:
	DataType(string content = "", int color = 0xFF0000);
	DataType(const DataType& source);
	virtual ~DataType();
	DataType& operator=(const DataType& source);

	virtual Token* Clone();
	virtual string GetType();
};

//Keyword
class Keyword : public Token {
public:
	Keyword(string content = "", int color = 0xFF00FF);
	Keyword(const Keyword& source);
	virtual ~Keyword();
	Keyword& operator=(const Keyword& source);

	virtual Token* Clone();
	virtual string GetType();
};

//Preprocessor
class Preprocessor : public Token {
public:
	Preprocessor(string content = "", int color = 0x808080);
	Preprocessor(const Preprocessor& source);
	virtual ~Preprocessor();
	Preprocessor& operator=(const Preprocessor& source);

	virtual Token* Clone();
	virtual string GetType();
};

//Operator
class Operator : public Token {
public:
	Operator(string content = "", int color = 0x000000);
	Operator(const Operator& source);
	virtual ~Operator();
	Operator& operator=(const Operator& source);

	virtual Token* Clone();
	virtual string GetType();
};

//Punctuation
class Punctuation : public Token {
public:
	Punctuation(string content = "", int color = 0x000000);
	Punctuation(const Punctuation& source);
	virtual ~Punctuation();
	Punctuation& operator=(const Punctuation& source);

	virtual Token* Clone();
	virtual string GetType();
};

//Identifier
class Identifier : public Token {
public:
	Identifier(string content = "", int color = 0x000000);
	Identifier(const Identifier& source);
	virtual ~Identifier();
	Identifier& operator=(const Identifier& source);

	virtual Token* Clone();
	virtual string GetType();
};

//SymbolicConstant
class SymbolicConstant : public Token {
public:
	SymbolicConstant(string content = "", int color = 0x800080);
	SymbolicConstant(const SymbolicConstant& source);
	virtual ~SymbolicConstant();
	SymbolicConstant& operator=(const SymbolicConstant& source);

	virtual Token* Clone();
	virtual string GetType();
};

//Literal
class Literal : public Token {
public:
	Literal(string content = "", int color = 0x0096FF);
	Literal(const Literal& source);
	virtual ~Literal();
	Literal& operator=(const Literal& source);

	virtual Token* Clone();
	virtual string GetType();
};

//Annotation
class Annotation : public Token {
public:
	Annotation(string content = "", int color = 0x008000);
	Annotation(const Annotation& source);
	virtual ~Annotation();
	Annotation& operator=(const Annotation& source);

	virtual Token* Clone();
	virtual string GetType();
};

#endif //_TOKEN_H