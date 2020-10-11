#include "ContrastTool.h"
#include "Editor.h"

using namespace Osp::Graphics;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui::Controls;

ContrastTool::ContrastTool() :
	__pEditBmp(NULL), __pProcessBmp(NULL)
{

}

ContrastTool::~ContrastTool()
{

}

void ContrastTool::Launch(Bitmap* pEditbmp)
{

	if (__pEditBmp)
	{
		delete __pEditBmp;
	}
	__pEditBmp = Util::CopyBitmapN(*pEditbmp);

	Process();
}

void ContrastTool::Process()
{

	__offset = 0;

	//launch slider
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->ActivateSlider(true, this, L"Contrast", -99, 100);

	pEditor = null;

}

void ContrastTool::OnAdjustmentValueChanged(const Osp::Ui::Control &source, int adjustment)
{
	ApplyContrastValue(adjustment);
}

void ContrastTool::ApplyContrastValue(int offset)
{

	__pProcessBmp = Util::CopyBitmapN(*__pEditBmp);

	if (__pProcessBmp == NULL)
	{
		return;
	}

	int width = 0;
	int height = 0;
	int bitsPerPixel = 0;
	int bytesPerPixel = 0;
	BufferInfo info;

	__pProcessBmp->Lock(info);

	width = info.width;
	height = info.height;
	bitsPerPixel = info.bitsPerPixel;

	if (bitsPerPixel == 32)
	{
		bytesPerPixel = 4;
	}
	else if (bitsPerPixel == 24)
	{
		bytesPerPixel = 3;
	}

	double newRed;
	double newBlue;
	double newGreen;
	double scaledoffset = (offset + 100.0) / 100.0;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			double* color = ((double *) ((byte *) info.pPixels + y * info.pitch + x * bytesPerPixel));
			byte* blue = (byte*) color;
			byte* green = blue + 1;
			byte* red = green + 1;

			newRed = (double(*red) / 255.0 - 0.5) * scaledoffset;
			newBlue = (double(*blue) / 255.0 - 0.5) * scaledoffset;
			newGreen = (double(*green) / 255.0 - 0.5) * scaledoffset;

			newRed = (newRed + 0.5) * 255.0;
			newBlue = (newBlue + 0.5) * 255.0;
			newGreen = (newGreen + 0.5) * 255.0;

			newRed = newRed > 255 ? 255 : newRed;
			newRed = newRed < 0 ? 0 : newRed;
			newBlue = newBlue > 255 ? 255 : newBlue;
			newBlue = newBlue < 0 ? 0 : newBlue;
			newGreen = newGreen > 255 ? 255 : newGreen;
			newGreen = newGreen < 0 ? 0 : newGreen;
			*red = (byte) newRed;
			*green = (byte) newGreen;
			*blue = (byte) newBlue;

		}
	}

	__pProcessBmp->Unlock();

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	pEditor->SetCurrentBitmap(__pProcessBmp);

	pEditor->DrawImageOnCanvas();

	pEditor->RedrawSlider();

	pEditor = null;

	__offset = offset;

}

void ContrastTool::OnSliderSubmit()
{
	Exit();
}

void ContrastTool::OnSliderCancel()
{
	ApplyContrastValue(0);
	Exit();
}

void ContrastTool::Exit()
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	ArrayList* pArgList = new ArrayList();
	pArgList->Construct(1);

	pArgList->Add(*(new Integer(__offset)));
	pEditor->SendUserEvent(K_TOOL_ID, pArgList);

	pEditor = null;

	delete __pEditBmp;
	__pEditBmp = null;
}
