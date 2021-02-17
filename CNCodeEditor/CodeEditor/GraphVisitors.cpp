#include "GraphVisitors.h"
#include "UIGraphs.h"

GraphVisitor::GraphVisitor() {

}

GraphVisitor::~GraphVisitor() {

}

//GraphDrawingVisitor
GraphDrawingVisitor::GraphDrawingVisitor(CDC* dc) {
	this->dc = dc;
}

GraphDrawingVisitor::~GraphDrawingVisitor() {

}

void GraphDrawingVisitor::Visit(WindowCaption* element) {
	Long x = element->GetX();
	Long y = element->GetY();
	Long width = element->GetWidth();
	Long height = element->GetHeight();

	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen *oldPen = this->dc->SelectObject(&pen);

	this->dc->MoveTo(x, y);
	this->dc->LineTo(x + width, y);
	this->dc->MoveTo(x, y + height);
	this->dc->LineTo(x + width, y + height);

	this->dc->SelectObject(oldPen);

	CFont *font = this->dc->GetCurrentFont();
	LOGFONT logFont;
	font->GetLogFont(&logFont);
	Long contentHeight = logFont.lfHeight;
	CRect rect(x+10, y + (height - contentHeight) / 2, x + width, y + height);
	this->dc->DrawText(element->GetContent().c_str(), rect, DT_LEFT | DT_VCENTER);
}

void GraphDrawingVisitor::Visit(WindowCloseButton* element) {
	Long x = element->GetX();
	Long y = element->GetY();
	Long width = element->GetWidth();
	Long height = element->GetHeight();
	float hhhWidth = width / 8.0F; //half of half of half of width
	float hhhHeight = height / 8.0F;

	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldPen = this->dc->SelectObject(&pen);

	this->dc->MoveTo(x, y);
	this->dc->LineTo(x + width, y);
	this->dc->MoveTo(x, y+height);
	this->dc->LineTo(x + width, y+height);

	this->dc->MoveTo(x + hhhWidth * 2, y + hhhHeight * 2);
	this->dc->LineTo(x + hhhWidth * 6, y + hhhHeight * 6);

	this->dc->MoveTo(x + hhhWidth * 6, y + hhhHeight * 2);
	this->dc->LineTo(x + hhhWidth * 2, y + hhhHeight * 6);

	this->dc->SelectObject(oldPen);
}