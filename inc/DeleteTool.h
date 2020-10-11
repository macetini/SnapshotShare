#ifndef DELETETOOL_H_
#define DELETETOOL_H_

#include <FUi.h>
#include "CroppingRectangle.h"

class DeleteTool:
		public Osp::Ui::IActionEventListener,
		public Osp::Ui::IOrientationEventListener
{
public:
	DeleteTool();
	virtual ~DeleteTool();

	void OnCropDone(Osp::Graphics::Rectangle box);

	void Launch(Osp::Graphics::Bitmap* pEditBmp);
	void Exit(void);

	void OnActionPerformed(const Osp::Ui::Control &source, int actionId);
	void OnOrientationChanged(const Osp::Ui::Control &source, Osp::Ui::OrientationStatus orientationStatus);

	static const int K_TOOL_ID = 1007;

private:
	static const int K_BTN_DONE = 1;
	static const int K_BTN_CANCEL = 0;

	void Process(Osp::Graphics::Rectangle box);
	void DrawRectangle(void);
	void RemoveRectangle(void);

	void CreatePanel();
	void RemovePanel();

	void CreateButtons();
	void RemoveButtons();

	Osp::Graphics::Point __lastTouch;

	Osp::Graphics::Bitmap* __pEditBmp;
	Osp::Graphics::Bitmap* __pProcessBmp;
	Osp::Ui::Controls::Button* __pDoneBtn;
	Osp::Ui::Controls::Button* __pCancelBtn;

	CroppingRectangle* __pCropBox;
	Osp::Ui::Controls::Panel* __pPanel;

};

#endif /* DELETETOOL_H_ */
