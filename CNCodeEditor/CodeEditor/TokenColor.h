#ifndef _TOKENCOLOR_H
#define _TOKENCOLOR_H

class TokenColor {
public:
	TokenColor(bool isBright = true);
	TokenColor(const TokenColor& source);
	~TokenColor();
	TokenColor& operator=(const TokenColor& source);

public:
	int dataTypes;
	int keywords;
	int preprocessors;
	int operators;
	int punctuations;
	int identifiers;
	int symbolicConstants;
	int literals;
	int annotations;
};

#endif //_TOKENCOLOR_H