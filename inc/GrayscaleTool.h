#ifndef GRAYSCALETOOL_H_
#define GRAYSCALETOOL_H_

#include <FGraphics.h>

class GrayscaleTool
{
public:
	GrayscaleTool();
	virtual ~GrayscaleTool();

	void Launch(Osp::Graphics::Bitmap* pEditbmp);
	void Exit();

	static const int K_TOOL_ID = 1001;

private:
	void Process();

	Osp::Graphics::Bitmap* __pEditBmp;
};

#endif /* GRAYSCALETOOL_H_ */
