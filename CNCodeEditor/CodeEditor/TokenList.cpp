#include "TokenList.h"
#include "Tokens.h"

TokenList::TokenList(Long capacity)
	: tokens(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}

TokenList::TokenList(const TokenList& source) 
	: tokens(source.tokens) {
	Long i = 0;
	while (i < source.length) {
		this->tokens.Modify(i, (const_cast<TokenList&>(source)).tokens[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}

TokenList::~TokenList() {
	Long i = 0;
	while (i < this->length) {
		if (this->tokens[i] != 0) {
			delete this->tokens[i];
		}
		i++;
	}
}

TokenList& TokenList::operator=(const TokenList& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->tokens[i] != 0) {
			delete this->tokens[i];
		}
		i++;
	}
	this->tokens = source.tokens;
	i = 0;
	while (i < source.length) {
		this->tokens.Modify(i, (const_cast<TokenList&>(source)).tokens[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;

	return *this;
}

Long TokenList::Add(Token* token) {
	Long index;
	if (this->length < this->capacity) {
		index = this->tokens.Store(this->length, token);
	}
	else {
		index = this->tokens.AppendFromRear(token);
		this->capacity++;
	}
	this->length++;
	this->current = index + 1;

	return index;
}

Long TokenList::Add(Long index, Token* token) {
	index = this->tokens.Insert(index, token);
	this->length++;
	this->capacity++;
	this->current = index + 1;

	return index;
}

Long TokenList::Remove(Long index) {
	if (this->tokens[index] != 0) {
		delete this->tokens.GetAt(index);
	}
	this->current = index;
	index = this->tokens.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

Token* TokenList::GetAt(Long index) {
	return this->tokens.GetAt(index);
}

TokenList* TokenList::Clone() {
	return new TokenList(*this);
}