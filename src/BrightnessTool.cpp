#include "BrightnessTool.h"
#include "Editor.h"

using namespace Osp::Graphics;
using namespace Osp::Ui::Controls;
using namespace Osp::Base;
using namespace Osp::Base::Collection;

BrightnessTool::BrightnessTool() :
	__pEditBmp(NULL), __pProcessBmp(NULL)
{

}

BrightnessTool::~BrightnessTool()
{

}

void BrightnessTool::Launch(Bitmap* pEditbmp)
{

	__offset = 0;

	if (__pEditBmp)
	{
		delete __pEditBmp;
	}
	__pEditBmp = Util::CopyBitmapN(*pEditbmp);

	Process();
}

void BrightnessTool::Process()
{
	//launch slider
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->ActivateSlider(true, this, L"Brightness", -99, 100);
	pEditor = null;
}

void BrightnessTool::OnAdjustmentValueChanged(const Osp::Ui::Control &source, int adjustment)
{
	ApplyBrightnessValue(adjustment);
}

void BrightnessTool::ApplyBrightnessValue(int offset)
{

	__pProcessBmp = Util::CopyBitmapN(*__pEditBmp);

	offset = offset * 255 / 100;

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

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int* color = ((int *) ((byte *) info.pPixels + y * info.pitch + x * bytesPerPixel));
			byte* blue = (byte*) color;
			byte* green = blue + 1;
			byte* red = green + 1;

			int newRed = *red + offset;
			int newBlue = *blue + offset;
			int newGreen = *green + offset;

			newRed = newRed > 255 ? 255 : newRed;
			newRed = newRed < 0 ? 0 : newRed;
			newBlue = newBlue > 255 ? 255 : newBlue;
			newBlue = newBlue < 0 ? 0 : newBlue;
			newGreen = newGreen > 255 ? 255 : newGreen;
			newGreen = newGreen < 0 ? 0 : newGreen;

			*red = newRed;
			*green = newGreen;
			*blue = newBlue;

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

void BrightnessTool::OnSliderSubmit()
{
	Exit();
}

void BrightnessTool::OnSliderCancel()
{
	ApplyBrightnessValue(0);
	Exit();
}

void BrightnessTool::Exit()
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
