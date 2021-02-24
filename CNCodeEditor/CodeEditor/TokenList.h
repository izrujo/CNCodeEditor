#ifndef _TOKENLIST_H
#define _TOKENLIST_H

#include "../Utilities/Array.h"

class Token;

class TokenList {
public:
	TokenList(Long capacity = 32);
	TokenList(const TokenList& source);
	~TokenList();
	TokenList& operator=(const TokenList& source);

	Long Add(Token* token);
	Long Add(Long index, Token* token);
	Long Remove(Long index);
	Token* GetAt(Long index);

	TokenList* Clone();

	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;

private:
	Array<Token*> tokens;
	Long capacity;
	Long length;
	Long current;
};

inline Long TokenList::GetCapacity() const {
	return this->capacity;
}

inline Long TokenList::GetLength() const {
	return this->length;
}

inline Long TokenList::GetCurrent() const {
	return this->current;
}

#endif //_TOKENLIST_H