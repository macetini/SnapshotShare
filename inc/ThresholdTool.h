#ifndef THRESHOLDTOOL_H_
#define THRESHOLDTOOL_H_

#include <FUi.h>
#include "ISliderEnabledTool.h"
#include "FormManager.h"
#include "Util.h"

class ThresholdTool:
		public ISliderEnabledTool
{
public:
	ThresholdTool();
	virtual ~ThresholdTool();

	void Launch(Osp::Graphics::Bitmap* pEditbmp);
	void Exit();

	void OnAdjustmentValueChanged(const Osp::Ui::Control &source, int adjustment);
	void OnSliderSubmit(void);
	void OnSliderCancel(void);

	static const int K_TOOL_ID = 1003;

private:
	void Process();
	void ApplyThresholdValue(int offset);
	void EditCancel();

	Osp::Graphics::Bitmap* __pEditBmp;
	Osp::Graphics::Bitmap* __pProcessBmp;

	Osp::Graphics::Bitmap* __pCancelEditBmp;

	int __offset;
};

#endif /* THRESHOLDTOOL_H_ */
