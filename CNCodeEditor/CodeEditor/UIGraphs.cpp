#include "UIGraphs.h"
#include "GraphVisitors.h"

UIGraph::UIGraph(Long x, Long y, Long width, Long height, string content) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->content = content;
}

UIGraph::UIGraph(const UIGraph& source) {
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	this->content = source.content;
}

UIGraph::~UIGraph() {

}

UIGraph& UIGraph::operator=(const UIGraph& source) {
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	this->content = source.content;

	return *this;
}

//WindowCaption
WindowCaption::WindowCaption(Long x, Long y, Long width, Long height, string content) 
	: UIGraph(x, y, width, height, content) {

}

WindowCaption::WindowCaption(const WindowCaption& source) 
	: UIGraph(source) {

}

WindowCaption::~WindowCaption() {

}

WindowCaption& WindowCaption::operator=(const WindowCaption& source) {
	UIGraph::operator=(source);

	return *this;
}

UIGraph* WindowCaption::Clone() {
	return new WindowCaption(*this);
}

void WindowCaption::Accept(GraphVisitor* visitor) {
	visitor->Visit(this);
}

//WindowCloseButton
WindowCloseButton::WindowCloseButton(Long x, Long y, Long width, Long height, string content)
	: UIGraph(x, y, width, height, content) {

}

WindowCloseButton::WindowCloseButton(const WindowCloseButton& source)
	: UIGraph(source) {

}

WindowCloseButton::~WindowCloseButton() {

}

WindowCloseButton& WindowCloseButton::operator=(const WindowCloseButton& source) {
	UIGraph::operator=(source);

	return *this;
}

UIGraph* WindowCloseButton::Clone() {
	return new WindowCloseButton(*this);
}

void WindowCloseButton::Accept(GraphVisitor* visitor) {
	visitor->Visit(this);
}