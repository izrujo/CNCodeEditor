#include "TokenColor.h"

TokenColor::TokenColor(bool isBright) {
	if (isBright == true) {
		this->dataTypes = 0xFF0000;
		this->keywords = 0xFF00FF;
		this->preprocessors = 0x808080;
		this->operators = 0x000000;
		this->punctuations = 0x000000;
		this->identifiers = 0x000000;
		this->symbolicConstants = 0x800080;
		this->literals = 0x0096FF;
		this->annotations = 0x008000;
	}
	else {
		this->dataTypes = 0xD69669;
		this->keywords = 0xBEB7FF;
		this->preprocessors = 0x8D9B98;
		this->operators = 0xDCDAC6;
		this->punctuations = 0xDCDAC6;
		this->identifiers = 0xDCDAC6;
		this->symbolicConstants = 0xD68BA0;
		this->literals = 0x569CD6;
		this->annotations = 0x57A43A;
	}
}

TokenColor::TokenColor(const TokenColor& source) {
	this->dataTypes = source.dataTypes;
	this->keywords = source.keywords;
	this->preprocessors = source.preprocessors;
	this->operators = source.operators;
	this->punctuations = source.punctuations;
	this->identifiers = source.identifiers;
	this->symbolicConstants = source.symbolicConstants;
	this->literals = source.literals;
	this->annotations = source.annotations;
}

TokenColor::~TokenColor() {

}

TokenColor& TokenColor::operator=(const TokenColor& source) {
	this->dataTypes = source.dataTypes;
	this->keywords = source.keywords;
	this->preprocessors = source.preprocessors;
	this->operators = source.operators;
	this->punctuations = source.punctuations;
	this->identifiers = source.identifiers;
	this->symbolicConstants = source.symbolicConstants;
	this->literals = source.literals;
	this->annotations = source.annotations;

	return *this;
}