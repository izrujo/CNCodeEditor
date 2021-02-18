//FileManager.h
/**************************************************
Classification : Control class
Function : File process
MVC : Control
Author : Chan Hyung Kim
Written Date : 2020.07.17
Recently Updated : 2020.07.17
**************************************************/

#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <iostream>

using namespace std;

class CodeEditor;

class FileManager {
public:
	FileManager(CodeEditor* codeEditor = 0);
	FileManager(const FileManager& source);
	~FileManager();
	FileManager& operator=(const FileManager& source);

	void Save();
	void Load();
	void New();
private:
	CodeEditor* codeEditor;
};

#endif //_FILEMANAGER_H