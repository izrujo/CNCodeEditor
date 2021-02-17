#ifndef _GRAPHVISITORS_H
#define _GRAPHVISITORS_H

#include <afxwin.h>

class WindowCaption;
class WindowCloseButton;

class GraphVisitor {
public:
	GraphVisitor();
	virtual ~GraphVisitor() = 0;

	virtual void Visit(WindowCaption* element) = 0;
	virtual void Visit(WindowCloseButton* element) = 0;
};

//GraphDrawingVisitor
class GraphDrawingVisitor : public GraphVisitor {
public:
	GraphDrawingVisitor(CDC* dc = 0);
	virtual ~GraphDrawingVisitor();

	virtual void Visit(WindowCaption* element);
	virtual void Visit(WindowCloseButton* element);

private:
	CDC* dc;
};


#endif //_GRAPHVISITORS_H