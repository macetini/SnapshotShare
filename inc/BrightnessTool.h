#ifndef BRIGHTNESSTOOL_H_
#define BRIGHTNESSTOOL_H_

#include <FUi.h>
#include <FGraphics.h>

#include "ISliderEnabledTool.h"
#include "FormManager.h"
#include "Util.h"

class BrightnessTool:
		public ISliderEnabledTool
{
public:
	BrightnessTool();
	virtual ~BrightnessTool();

	void Launch(Osp::Graphics::Bitmap* pEditbmp);
	void Exit();

	void OnAdjustmentValueChanged(const Osp::Ui::Control &source, int adjustment);
	void OnSliderSubmit(void);
	void OnSliderCancel(void);

	static const int K_TOOL_ID = 1002;

private:
	void Process();
	void ApplyBrightnessValue(int offset);

	Osp::Graphics::Bitmap* __pEditBmp;
	Osp::Graphics::Bitmap* __pProcessBmp;
	int __offset;
};

#endif /* BRIGHTNESSTOOL_H_ */
