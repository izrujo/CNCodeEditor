#ifndef _TOKENFACTORY_H
#define _TOKENFACTORY_H

#define DATATYPESCOUNT 13
#define KEYWORDSCOUNT 9
#define PREPROCESSORSCOUNT 7
#define OPERATORSCOUNT 31
#define PUNCTUATIONSCOUNT 7

#include "Tokens.h"

typedef signed long int Long;

class TokenFactory {
public:
	TokenFactory(bool isBright = true);
	//TokenFactory(const TokenFactory& source);
	~TokenFactory();

	Token* Make(string content);

	bool SearchIsOperator(string content);
	bool SearchIsPunctuation(string content);

	void SetLiteralState(bool literalState);
	void SetAnnotationState(bool annotationState);
	void Brighten(bool isBright = true);

	bool GetLiteralState() const;
	bool GetAnnotationState() const;

private:
	string dataTypes[DATATYPESCOUNT];
	string keywords[KEYWORDSCOUNT];
	string preprocessors[PREPROCESSORSCOUNT];
	string operators[OPERATORSCOUNT];
	string punctuations[PUNCTUATIONSCOUNT];

	bool literalState;
	bool annotationState;
	bool isBright;
};

inline bool TokenFactory::GetLiteralState() const {
	return this->literalState;
}

inline bool TokenFactory::GetAnnotationState() const {
	return this->annotationState;
}

#endif //_TOKENFACTORY_H