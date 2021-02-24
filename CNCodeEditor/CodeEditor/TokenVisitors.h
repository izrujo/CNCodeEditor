#ifndef _TOKENVISITORS_H
#define _TOKENVISITORS_H

#include "../TextEditor/Visitor.h"

#include <string>

using namespace std;

class TokenBook;
class TokenFactory;

class TokenMakingVisitor : public Visitor {
public:
	TokenMakingVisitor(TokenBook* tokenBook = 0, TokenFactory* tokenFactory = 0);
	virtual ~TokenMakingVisitor();

	virtual void Visit(Note* note);
	virtual void Visit(Line* line);
	virtual void Visit(Character* character);
	virtual void Visit(Book* book); //Not implement

private:
	TokenBook* tokenBook;
	TokenFactory* tokenFactory;
	string current;
	bool smallLiteral;
	bool bigLiteral;
	bool singleAnnotation;
	bool multiAnnotation;
};

class TokenDrawingVisitor : public Visitor {
public:
	TokenDrawingVisitor(TokenBook* tokenBook = 0);
	virtual ~TokenDrawingVisitor();

	virtual void Visit(Note* note);
	virtual void Visit(Line* line);
	virtual void Visit(Character* character);
	virtual void Visit(Book* book);
	

private:
	TokenBook* tokenBook;
};

#endif //_TOKENVISITORS_H