#include <FUi.h>
#include <FApp.h>
#include "CroppingRectangle.h"
#include "Editor.h"
#include "Util.h"

using namespace Osp::Base::Collection;
using namespace Osp::Base::Utility;
using namespace Osp::Graphics;
using namespace Osp::Ui::Controls;
using namespace Osp::Ui;

CroppingRectangle::CroppingRectangle() :
	__pCanvas(NULL)
{

}

CroppingRectangle::~CroppingRectangle()
{
	if (__pCanvas)
	{
		__pCanvas->Clear();
		delete __pCanvas;
		__pCanvas = NULL;
	}
}

result CroppingRectangle::Init(Dimension dimension, Rectangle draggableArea)
{
	result r = E_SUCCESS;

	Construct(draggableArea, GROUP_STYLE_NONE);
	__box.width = dimension.width;
	__box.height = dimension.height;

	SetSize(draggableArea.width, draggableArea.height);
	SetPosition(0, 0);

	SetDraggableArea(draggableArea);
	Reset();

	return r;
}

Rectangle CroppingRectangle::GetBoundingBox()
{
	return __box;
}

void CroppingRectangle::Reset()
{

	int x = (__draggableArea.width - __box.width) / 2;
	int y = (__draggableArea.height - __box.height) / 2;

	__box.x = x;
	__box.y = y;
}

void CroppingRectangle::SetDraggableArea(Rectangle draggableArea)
{
	__draggableArea = draggableArea;
}

result CroppingRectangle::OnInitializing()
{
	result r = E_SUCCESS;

	__pCanvas = GetCanvasN();

	AddTouchEventListener(*this);

	__mode = K_MODE_DRAG;

	DrawRectangle();

	return r;
}

result CroppingRectangle::OnDraw()
{
	DrawRectangle();

	return E_SUCCESS;
}

void CroppingRectangle::DrawRectangle()
{
	__pCanvas->Clear();

	__pCanvas->SetLineStyle(LINE_STYLE_SOLID);
	__pCanvas->SetLineWidth(1);

	__pCanvas->SetForegroundColor(Color::COLOR_RED);
	__pCanvas->DrawRectangle(__box);

	if (__mode == K_MODE_RESIZE)
	{
		int newW = __box.width;
		int newH = __box.height;

		if ((__box.y + __box.height) >= __draggableArea.height)
		{
			int temp = (__box.y + __box.height) - __draggableArea.height;
			newH = __box.height - temp;
		}
		if ((__box.x + __box.width) >= __draggableArea.width)
		{
			int temp = (__box.x + __box.width) - __draggableArea.width;
			newW = __box.width - temp;
		}
		__box.SetSize(newW, newH);
		if (__leftTop == true)
			__pCanvas->FillRectangle(Color::COLOR_RED, Rectangle(__box.x - 5, __box.y - 5, 10, 10));
		if (__rightTop == true)
			__pCanvas->FillRectangle(Color::COLOR_RED, Rectangle(__box.x + __box.width - 5, __box.y - 5, 10, 10));
		if (__leftBottom == true)
			__pCanvas->FillRectangle(Color::COLOR_RED, Rectangle(__box.x - 5, __box.y + __box.height - 5, 10, 10));
		if (__rightBottom == true)
			__pCanvas->FillRectangle(Color::COLOR_RED, Rectangle(__box.x + __box.width - 5, __box.y + __box.height
					- 5, 10, 10));
	}

	Show();
}

void CroppingRectangle::ToggleMode()
{
	if (__mode == K_MODE_DRAG)
	{
		__mode = K_MODE_RESIZE;
	}
	else
	{
		__mode = K_MODE_DRAG;
	}

	DrawRectangle();
}

void CroppingRectangle::OnTouchDoublePressed(const Osp::Ui::Control &source,
		const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo)
{

}

void CroppingRectangle::OnTouchFocusIn(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
		const Osp::Ui::TouchEventInfo &TouchInfo)
{
}
void CroppingRectangle::OnTouchFocusOut(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
		const Osp::Ui::TouchEventInfo &touchInfo)
{
}
void CroppingRectangle::OnTouchLongPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
		const Osp::Ui::TouchEventInfo &touchInfo)
{

}

void CroppingRectangle::OnTouchPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
		const Osp::Ui::TouchEventInfo &touchInfo)
{
	if (IsOnBoxBorder(currentPosition))
	{
		__mode = K_MODE_RESIZE;
	}
	else
	{
		__mode = K_MODE_DRAG;
	}

	__lastTouch = currentPosition;
	DrawRectangle();

}
void CroppingRectangle::OnTouchReleased(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
		const Osp::Ui::TouchEventInfo &touchInfo)
{
	SetFlagFalse();

}
void CroppingRectangle::OnTouchMoved(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
		const Osp::Ui::TouchEventInfo &touchInfo)
{
	int dx = currentPosition.x - __lastTouch.x;
	int dy = currentPosition.y - __lastTouch.y;

	int width = 0;
	int height = 0;

	int x = __box.x + dx;
	int y = __box.y + dy;

	if (__rightTop == true)
	{
		__box.y = __box.y + dy;

		width = __box.width + dx > 0 ? __box.width + dx : 8;
		height = __box.height - dy > 0 ? __box.height - dy : 8;

	}

	if (__leftTop == true)
	{
		__box.x = __box.x + dx;
		__box.y = __box.y + dy;

		width = __box.width - dx > 0 ? __box.width - dx : 8;
		height = __box.height - dy > 0 ? __box.height - dy : 8;

	}

	if (__leftBottom == true)
	{
		__box.x = __box.x + dx;

		width = __box.width - dx > 0 ? __box.width - dx : 8;
		height = __box.height + dy > 0 ? __box.height + dy : 8;

	}

	if (__rightBottom == true)
	{
		width = __box.width + dx > 0 ? __box.width + dx : 8;
		height = __box.height + dy > 0 ? __box.height + dy : 8;
	}

	if (__mode == K_MODE_DRAG)
	{
		__box.x = x;
		__box.y = y;
		CheckBoxBounds(null, null, null, null);
	}
	else
	{
		CheckBoxBounds(width, height, dx, dy);
	}

	DrawRectangle();
	__lastTouch = Point(currentPosition.x, currentPosition.y);
	GetParent()->Show(); //update the main form
}

bool CroppingRectangle::IsOnBoxBorder(Point clickedPoint)
{

	GetBoxPoints();
	SetFlagFalse();

	if (Math::Abs(clickedPoint.x - __leftTopX) < 30 && Math::Abs(clickedPoint.y - __leftTopY) < 25)
	{
		__leftTop = true;
		return true;
	}

	if (Math::Abs(clickedPoint.x - __leftBottomX) < 30 && Math::Abs(clickedPoint.y - __leftBottomY) < 25)
	{
		__leftBottom = true;
		return true;

	}
	if (Math::Abs(clickedPoint.x - __rightTopX) < 30 && Math::Abs(clickedPoint.y - __rightTopY) < 25)
	{
		__rightTop = true;
		return true;

	}
	if (Math::Abs(clickedPoint.x - __rightBottomX) < 30 && Math::Abs(clickedPoint.y - __rightBottomY) < 25)
	{
		__rightBottom = true;
		return true;
	}

	return false;
}

bool CroppingRectangle::IsWithinBox(Point clickedPoint)
{

	return (__box.Contains(clickedPoint));

}

result CroppingRectangle::OnTerminating()
{

	result r = E_SUCCESS;

	if (__pCanvas)
	{
		__pCanvas->Clear();
		delete __pCanvas;
		__pCanvas = NULL;
	}

	return r;
}

void CroppingRectangle::SetFlagFalse()
{
	__rightBottom = false;
	__rightTop = false;
	__leftBottom = false;
	__leftTop = false;
}

void CroppingRectangle::GetBoxPoints()
{
	__leftTopX = __box.x;
	__leftTopY = __box.y;
	__leftBottomX = __box.x;
	__leftBottomY = __box.y + __box.height;
	__rightTopX = __box.x + __box.width;
	__rightTopY = __leftTopY;
	__rightBottomX = __rightTopX;
	__rightBottomY = __leftBottomY;
}

void CroppingRectangle::CheckBoxBounds(int width, int height, int dx, int dy)
{

	if (__box.x <= __draggableArea.x)
	{
		if (__leftTop == true || __mode == K_MODE_DRAG || __leftBottom == true)
		{
			__box.x = __draggableArea.x;
			width = width + dx;
		}
	}

	if (__box.x + __box.width >= __draggableArea.width)
	{
		if (__rightTop == true || __mode == K_MODE_DRAG || __rightBottom == true)
		{
			__box.x = __draggableArea.width - __box.width;
			width = width - dx;
		}
	}

	if (__box.y <= __draggableArea.y)
	{
		if (__leftTop == true || __mode == K_MODE_DRAG || __rightTop == true)
		{
			__box.y = __draggableArea.y;
			height = height + dy;
		}
	}

	if ((__box.y + __box.height) >= __draggableArea.height)
	{
		if (__rightBottom == true || __mode == K_MODE_DRAG || __leftBottom == true)
		{
			__box.y = __draggableArea.height - __box.height;
			height = height - dy;
		}
	}

	if (__mode == K_MODE_RESIZE)
		__box.SetSize(Dimension(width, height));

}
