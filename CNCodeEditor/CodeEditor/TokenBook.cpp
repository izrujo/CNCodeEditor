#include "TokenBook.h"
#include "TokenList.h"

TokenBook::TokenBook(Long capacity)
	: tokenLists(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}

TokenBook::TokenBook(const TokenBook& source)
	: tokenLists(source.tokenLists) {
	Long i = 0;
	while (i < source.length) {
		this->tokenLists.Modify(i, (const_cast<TokenBook&>(source)).tokenLists[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

TokenBook::~TokenBook() {
	Long i = 0;
	while (i < this->length) {
		if (this->tokenLists[i] != 0) {
			delete this->tokenLists[i];
		}
		i++;
	}
}

TokenBook& TokenBook::operator=(const TokenBook& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->tokenLists[i] != 0) {
			delete this->tokenLists[i];
		}
		i++;
	}
	this->tokenLists = source.tokenLists;
	i = 0;
	while (i < source.length) {
		this->tokenLists.Modify(i, (const_cast<TokenBook&>(source)).tokenLists[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}

Long TokenBook::Add(TokenList* tokenList) {
	Long index;
	if (this->length < this->capacity) {
		index = this->tokenLists.Store(this->length, tokenList);
	}
	else {
		index = this->tokenLists.AppendFromRear(tokenList);
		this->capacity++;
	}
	this->length++;
	this->current = index;

	return index;
}

Long TokenBook::Add(Long index, TokenList* tokenList) {
	index = this->tokenLists.Insert(index, tokenList);
	this->length++;
	this->capacity++;
	this->current = index;

	return index;
}

Long TokenBook::Remove(Long index) {
	if (this->tokenLists[index] != 0) {
		delete this->tokenLists.GetAt(index);
	}
	this->current = index;
	index = this->tokenLists.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

TokenList* TokenBook::GetAt(Long index) {
	return this->tokenLists.GetAt(index);
}

Long TokenBook::Move(Long index) {
	this->current = index;

	return this->current;
}