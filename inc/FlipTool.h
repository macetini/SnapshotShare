#ifndef FLIPTOOL_H_
#define FLIPTOOL_H_

#include <FGraphics.h>
#include "Util.h"

class FlipTool:
		public Osp::Ui::Controls::Form,
		public Osp::Ui::IActionEventListener,
		public Osp::Ui::IOrientationEventListener
{
public:
	FlipTool();
	virtual ~FlipTool();

	result Construct();

	void Launch(Osp::Graphics::Bitmap* pEditBmp);
	void OnOrientationChanged(const Osp::Ui::Control &source, Osp::Ui::OrientationStatus orientationStatus);
	void Exit(void);

	void SetFlipDirection(Osp::Media::ImageFlipType dir);

	static const int K_TOOL_ID = 1004;

private:
	static const int K_BTN_SIZE = 80;

	static const int K_HF_BTN = 1;
	static const int K_VF_BTN = -1;
	static const int K_DONE_BTN = 0;

	void Process(void);

	void CreateButtons(int i);
	void RemoveButtons();

	void OnActionPerformed(const Osp::Ui::Control &source, int actionId);

	Osp::Graphics::Bitmap* __pEditBmp;
	Osp::Media::ImageFlipType __direction;

	Osp::Ui::Controls::Button* __pHfBtn;
	Osp::Ui::Controls::Button* __pVfBtn;
	Osp::Ui::Controls::Button* __pDoneBtn;

};

#endif /* FLIPTOOL_H_ */
