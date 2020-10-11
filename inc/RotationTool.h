#ifndef ROTATIONTOOL_H_
#define ROTATIONTOOL_H_

#include <FGraphics.h>
#include "Util.h"

class RotationTool:
		public Osp::Ui::Controls::Form,
		public Osp::Ui::IActionEventListener,
		public Osp::Ui::IOrientationEventListener
{
public:
	RotationTool();
	virtual ~RotationTool();

	void Launch(Osp::Graphics::Bitmap* pEditBmp);
	void Exit(void);
	void OnOrientationChanged(const Osp::Ui::Control &source, Osp::Ui::OrientationStatus orientationStatus);
	void SetRotationAngle(Osp::Media::ImageRotationType dir);

	static const int K_TOOL_ID = 1005;

private:
	static const int K_BTN_SIZE = 80;

	static const int K_CW_BTN = 1;
	static const int K_ACW_BTN = -1;
	static const int K_DONE_BTN = 0;

	void Process(void);

	void CreateButtons(int i);
	void RemoveButtons();

	void OnActionPerformed(const Osp::Ui::Control &source, int actionId);

	Osp::Graphics::Bitmap* __pEditBmp;
	Osp::Media::ImageRotationType __direction;

	Osp::Ui::Controls::Button* __pCwBtn;
	Osp::Ui::Controls::Button* __pAcwBtn;
	Osp::Ui::Controls::Button* __pDoneBtn;
};

#endif /* ROTATIONTOOL_H_ */
