#ifndef BORDERTOOL_H_
#define BORDERTOOL_H_

#include <FGraphics.h>
#include <FMedia.h>

class BorderTool
{
public:
	BorderTool();
	virtual ~BorderTool();

	void Launch(Osp::Graphics::Bitmap* pEditbmp);
	void SetBorderId(int borderId);
	int GetBorderSize(Osp::Graphics::Dimension size);
	void Exit();

	static const int K_TOOL_ID = 1007;
	static const int K_BORDER_1 = 1;
	static const int K_BORDER_2 = 2;

private:
	void Process();

	Osp::Graphics::Bitmap* __pEditBmp;
	int __borderId;

	static const int O_PORTRAIT = 0;
	static const int O_LANDSCAPE = 1;

	static const int WVGA_PORTRAIT_WIDTH = 480;
	static const int WVGA_PORTRAIT_HEIGHT = 800;
	static const int WVGA_LANDSCAPE_WIDTH = 800;
	static const int WVGA_LANDSCAPE_HEIGHT = 480;

	static const int HVGA_PORTRAIT_WIDTH = 320;
	static const int HVGA_PORTRAIT_HEIGHT = 480;
	static const int HVGA_LANDSCAPE_WIDTH = 480;
	static const int HVGA_LANDSCAPE_HEIGHT = 320;
};

#endif /* BORDERTOOL_H_ */
