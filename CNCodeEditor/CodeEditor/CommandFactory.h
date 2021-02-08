//CommandFactory.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : Create command object.
MVC : ?
Author : Chan Hyung Kim
Written Date : 2019.05
Recently Updated : 2019.05
**************************************************/

#ifndef _COMMANDFACTORY_H
#define _COMMANDFACTORY_H

class CodeEditingForm;
class Command;

class CommandFactory {
public:
	CommandFactory(CodeEditingForm* codeEditingForm = 0);
	CommandFactory(const CommandFactory& source);
	~CommandFactory();
	CommandFactory& operator=(const CommandFactory& source);

	Command* Make(int uID);
private:
	CodeEditingForm* codeEditingForm;
};

#endif //_COMMANDFACTORY_H