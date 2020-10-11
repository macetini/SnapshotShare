#ifndef FLIPCOLORTOOL_H_
#define FLIPCOLORTOOL_H_

#include <FUi.h>
#include "FormManager.h"
#include "Util.h"

class FlipColorTool
{
public:
	FlipColorTool();
	virtual ~FlipColorTool();

	void Launch(Osp::Graphics::Bitmap* pEditbmp);
	void Exit();

	static const int K_TOOL_ID = 1008;

private:
	void Process();

	Osp::Graphics::Bitmap* __pEditBmp;
	Osp::Graphics::Bitmap* __pProcessBmp;

};

#endif /*FLIPCOLORTOOL_H_ */
