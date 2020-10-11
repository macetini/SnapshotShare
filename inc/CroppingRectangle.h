#ifndef CROPPINGRECTANGLE_H_
#define CROPPINGRECTANGLE_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>

class CroppingRectangle:
		public Osp::Ui::Controls::Panel,
		public Osp::Ui::ITouchEventListener
{
public:
	CroppingRectangle();
	virtual ~CroppingRectangle();

	static const int K_MODE_DRAG = 1;
	static const int K_MODE_RESIZE = 2;
	static const int K_EVT_CROP_DONE = 700;
	static const int K_EVT_ORIENTATION_CHANGE = 701;

	result Init(Osp::Graphics::Dimension dimension, Osp::Graphics::Rectangle draggableArea);
	result OnInitializing(void);
	result OnDraw(void);
	result OnTerminating(void);

	Osp::Graphics::Rectangle GetBoundingBox(void);

	void OnTouchDoublePressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
			const Osp::Ui::TouchEventInfo &touchInfo);
	void OnTouchFocusIn(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
			const Osp::Ui::TouchEventInfo &touchInfo);
	void OnTouchFocusOut(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
			const Osp::Ui::TouchEventInfo &touchInfo);
	void OnTouchLongPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
			const Osp::Ui::TouchEventInfo &touchInfo);
	void OnTouchMoved(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
			const Osp::Ui::TouchEventInfo &touchInfo);
	void OnTouchPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
			const Osp::Ui::TouchEventInfo &touchInfo);
	void OnTouchReleased(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition,
			const Osp::Ui::TouchEventInfo &touchInfo);

	void DrawRectangle(void);

	void Reset(void);

	void SetDraggableArea(Osp::Graphics::Rectangle draggableArea);
	void GetBoxPoints();
	void CheckBoxBounds(int width, int height, int dx, int dy);

private:

	Osp::Graphics::Canvas* __pCanvas;
	Osp::Graphics::Rectangle __box;
	Osp::Graphics::Rectangle __draggableArea;

	Osp::Graphics::Point __lastTouch;

	int __mode;
	bool __rightBottom;
	bool __rightTop;
	bool __leftBottom;
	bool __leftTop;

	int __leftTopX;
	int __leftTopY;
	int __leftBottomX;
	int __leftBottomY;
	int __rightTopX;
	int __rightTopY;
	int __rightBottomX;
	int __rightBottomY;

	void ToggleMode(void);
	void SetFlagFalse();

	bool IsWithinBox(Osp::Graphics::Point clickedPoint);
	bool IsOnBoxBorder(Osp::Graphics::Point clickedPoint);
};

#endif /* CROPPINGRECTANGLE_H_ */
