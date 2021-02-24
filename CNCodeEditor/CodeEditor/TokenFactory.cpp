#include "TokenFactory.h"

TokenFactory::TokenFactory() {
	Long i = 0;
	this->dataTypes[i++] = "int";
	this->dataTypes[i++] = "short";
	this->dataTypes[i++] = "long";
	this->dataTypes[i++] = "float";
	this->dataTypes[i++] = "double";
	this->dataTypes[i++] = "char";
	this->dataTypes[i++] = "void";
	this->dataTypes[i++] = "auto";
	this->dataTypes[i++] = "signed";
	this->dataTypes[i++] = "unsigned";
	this->dataTypes[i++] = "typedef";
	this->dataTypes[i++] = "struct";
	this->dataTypes[i++] = "union";
	i = 0;
	this->keywords[i++] = "if";
	this->keywords[i++] = "else";
	this->keywords[i++] = "switch";
	this->keywords[i++] = "case";
	this->keywords[i++] = "default";
	this->keywords[i++] = "while";
	this->keywords[i++] = "for";
	this->keywords[i++] = "do";
	this->keywords[i++] = "return";
	i = 0;
	this->preprocessors[i++] = "#";
	this->preprocessors[i++] = "#include";
	this->preprocessors[i++] = "#define";
	this->preprocessors[i++] = "#ifndef";
	this->preprocessors[i++] = "#endif";
	this->preprocessors[i++] = "#if";
	this->preprocessors[i++] = "#pragma";
	i = 0;
	this->operators[i++] = "+";
	this->operators[i++] = "-";
	this->operators[i++] = "*";
	this->operators[i++] = "/";
	this->operators[i++] = "%";
	this->operators[i++] = "^";
	this->operators[i++] = "+=";
	this->operators[i++] = "-=";
	this->operators[i++] = "*=";
	this->operators[i++] = "/=";
	this->operators[i++] = "%=";
	this->operators[i++] = "^=";
	this->operators[i++] = "<";
	this->operators[i++] = ">";
	this->operators[i++] = "=";
	this->operators[i++] = "!";
	this->operators[i++] = "<=";
	this->operators[i++] = ">=";
	this->operators[i++] = "==";
	this->operators[i++] = "!=";
	this->operators[i++] = "&";
	this->operators[i++] = "&&";
	this->operators[i++] = "|";
	this->operators[i++] = "||";
	this->operators[i++] = "<<";
	this->operators[i++] = ">>";
	this->operators[i++] = "?";
	this->operators[i++] = "~";
	this->operators[i++] = "[";
	this->operators[i++] = "]";
	this->operators[i++] = "[]";
	i = 0;
	this->punctuations[i++] = "(";
	this->punctuations[i++] = ")";
	this->punctuations[i++] = "{";
	this->punctuations[i++] = "}";
	this->punctuations[i++] = ",";
	this->punctuations[i++] = ";";
	this->punctuations[i++] = ":";

	this->literalState = false;
	this->annotationState = false;
}

//TokenFactory::TokenFactory(const TokenFactory& source) {
//	
//}

TokenFactory::~TokenFactory() {

}

Token* TokenFactory::Make(string content) {
	Token* token = 0;
	bool isMade = false;
	Long length = content.length();
	
	if (this->annotationState == true && isMade == false) {
		isMade = true;
		token = new Annotation(content);
	}
	
	if (this->literalState == true && isMade == false) {
		isMade = true;
		token = new Literal(content);
	}

	//���ͷ��� �ƴϰ�, �ּ��� �ƴϰ�, 
	// �ڷ��� �迭 �ȿ� ��ġ�ϴ� ���ڿ��� �ϳ��� ������ �ڷ����̴�.
	Long i = 0;
	while (i < DATATYPESCOUNT && isMade == false) {
		if (this->dataTypes[i] == content) {
			isMade = true;
			token = new DataType(content);
		}
		i++;
	}

	//���ͷ��� �ƴϰ�, �ּ��� �ƴϰ�, �ڷ����� �ƴϰ�, 
	// Ű���� �迭 �ȿ� ��ġ�ϴ� ���ڿ��� �ϳ��� ������ Ű�����̴�.
	i = 0;
	while (i < KEYWORDSCOUNT && isMade == false) {
		if (this->keywords[i] == content) {
			isMade = true;
			token = new Keyword(content);
		}
		i++;
	}

	//���ͷ��� �ƴϰ�, �ּ��� �ƴϰ�, �ڷ����� �ƴϰ�, Ű���尡 �ƴϰ�, 
	// ��ó���� �迭 �ȿ� ��ġ�ϴ� ���ڿ��� �ϳ��� ������ ��ó�����̴�.
	i = 0;
	while (i < PREPROCESSORSCOUNT && isMade == false) {
		if (this->preprocessors[i] == content) {
			isMade = true;
			token = new Preprocessor(content);
		}
		i++;
	}

	//���ͷ��� �ƴϰ�, �ּ��� �ƴϰ�, �ڷ����� �ƴϰ�, Ű���尡 �ƴϰ�, ��ó���Ⱑ �ƴϰ�, 
	// ������ �迭 �ȿ� ��ġ�ϴ� ���ڿ��� �ϳ��� ������ �������̴�.
	i = 0;
	while (i < OPERATORSCOUNT && isMade == false) {
		if (this->operators[i] == content) {
			isMade = true;
			token = new Operator(content);
		}
		i++;
	}

	//���ͷ��� �ƴϰ�, �ּ��� �ƴϰ�, �ڷ����� �ƴϰ�, Ű���尡 �ƴϰ�, ��ó���Ⱑ �ƴϰ�, �����ڰ� �ƴϰ�, 
	// ������ �迭 �ȿ� ��ġ�ϴ� ���ڿ��� �ϳ��� ������ �������̴�.
	i = 0;
	while (i < PUNCTUATIONSCOUNT && isMade == false) {
		if (this->punctuations[i] == content) {
			isMade = true;
			token = new Punctuation(content);
		}
		i++;
	}

	//���ͷ��� �ƴϰ�, �ּ��� �ƴϰ�, �ڷ����� �ƴϰ�, Ű���尡 �ƴϰ�, ��ó���Ⱑ �ƴϰ�, �����ڰ� �ƴϰ�, �������� �ƴϰ�, 
	// �ϳ��� �ҹ��ڰ� ������ �ĺ����̴�.
	// ��� �빮�ڸ� ��ȣ����̴�.
	bool isHangle = false;
	int isUpper = 1;
	i = 0;
	while (i < content.length() && isMade == false && isUpper != 0 && isHangle == false) {
		if ((content.at(i) & 0x80) == 0x80) {
			isHangle = true;
		}
		else {
			isUpper = isupper(content.at(i));
		}
		i++;
	}
	if (isMade == false && isHangle == false && (content.at(0) != ' ' && content.at(0) != '\t')) {
		if (isUpper == 0) {
			token = new Identifier(content);
		}
		else {
			token = new SymbolicConstant(content);
		}
	}

	return token;
}

bool TokenFactory::SearchIsOperator(string content) {
	bool isOperator = false;
	Long i = 0;
	while (i < OPERATORSCOUNT && isOperator == false) {
		if (this->operators[i] == content) {
			isOperator = true;
		}
		i++;
	}

	return isOperator;
}

bool TokenFactory::SearchIsPunctuation(string content) {
	bool isPunctuation = false;
	Long i = 0;
	while (i < PUNCTUATIONSCOUNT && isPunctuation == false) {
		if (this->punctuations[i] == content) {
			isPunctuation = true;
		}
		i++;
	}

	return isPunctuation;
}

void TokenFactory::SetLiteralState(bool literalState) {
	this->literalState = literalState;
}

void TokenFactory::SetAnnotationState(bool annotationState) {
	this->annotationState = annotationState;
}