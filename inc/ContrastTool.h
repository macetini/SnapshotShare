#ifndef CONTRASTTOOL_H_
#define CONTRASTTOOL_H_

#include <FUi.h>
#include "ISliderEnabledTool.h"
#include "FormManager.h"
#include "Util.h"

class ContrastTool:
		public ISliderEnabledTool
{
public:
	ContrastTool();
	virtual ~ContrastTool();

	void Launch(Osp::Graphics::Bitmap* pEditbmp);
	void Exit();

	void OnAdjustmentValueChanged(const Osp::Ui::Control &source, int adjustment);
	void OnSliderSubmit(void);
	void OnSliderCancel(void);

	static const int K_TOOL_ID = 1003;

private:
	void Process();
	void ApplyContrastValue(int offset);

	Osp::Graphics::Bitmap* __pEditBmp;
	Osp::Graphics::Bitmap* __pProcessBmp;
	int __offset;
};

#endif /* CONTRASTTOOL_H_ */
