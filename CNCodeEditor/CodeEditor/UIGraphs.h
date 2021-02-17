#ifndef _UIGRAPHS_H
#define _UIGRAPHS_H

#define WINDOWCAPTION 901
#define WINDOWCLOSEBUTTON 902

#include <string>
using namespace std;

typedef signed long int Long;

class GraphVisitor;

class UIGraph {
public:
	UIGraph(Long x, Long y, Long width, Long height, string content);
	UIGraph(const UIGraph& source);
	virtual ~UIGraph() = 0;
	UIGraph& operator=(const UIGraph& source);

	virtual UIGraph* Clone() = 0;
	virtual void Accept(GraphVisitor* visitor) = 0;

	virtual Long GetX() const;
	virtual Long GetY() const;
	virtual Long GetWidth() const;
	virtual Long GetHeight() const;
	virtual string GetContent() const;

protected:
	Long x;
	Long y;
	Long width;
	Long height;
	string content;
};

inline Long UIGraph::GetX() const {
	return this->x;
}

inline Long UIGraph::GetY() const {
	return this->y;
}

inline Long UIGraph::GetWidth() const {
	return this->width;
}

inline Long UIGraph::GetHeight() const {
	return this->height;
}

inline string UIGraph::GetContent() const {
	return this->content;
}

//WindowCaption
class WindowCaption : public UIGraph {
public:
	WindowCaption(Long x, Long y, Long width, Long height, string content);
	WindowCaption(const WindowCaption& source);
	virtual ~WindowCaption();
	WindowCaption& operator=(const WindowCaption& source);

	virtual UIGraph* Clone();
	virtual void Accept(GraphVisitor* visitor);
};

//WindowCloseButton
class WindowCloseButton : public UIGraph {
public:
	WindowCloseButton(Long x, Long y, Long width, Long height, string content);
	WindowCloseButton(const WindowCloseButton& source);
	virtual ~WindowCloseButton();
	WindowCloseButton& operator=(const WindowCloseButton& source);

	virtual UIGraph* Clone();
	virtual void Accept(GraphVisitor* visitor);
};

#endif //_UIGRAPHS_H