#ifndef _TOKENBOOK_H
#define _TOKENBOOK_H

#include "../Utilities/Array.h"

class TokenList;

class TokenBook {
public:
	TokenBook(Long capacity = 32);
	TokenBook(const TokenBook& source);
	~TokenBook();
	TokenBook& operator=(const TokenBook& source);

	Long Add(TokenList* token);
	Long Add(Long index, TokenList* token);
	Long Remove(Long index);
	TokenList* GetAt(Long index);

	Long Move(Long index);

	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;

private:
	Array<TokenList*> tokenLists;
	Long capacity;
	Long length;
	Long current;
};

inline Long TokenBook::GetCapacity() const {
	return this->capacity;
}

inline Long TokenBook::GetLength() const {
	return this->length;
}

inline Long TokenBook::GetCurrent() const {
	return this->current;
}

#endif //_TOKENBOOK_H