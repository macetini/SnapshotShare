#include "ThresholdTool.h"
#include "Editor.h"

using namespace Osp::Graphics;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui::Controls;

ThresholdTool::ThresholdTool() :
	__pEditBmp(NULL), __pProcessBmp(NULL), __pCancelEditBmp(NULL) {

}

ThresholdTool::~ThresholdTool() {

}

void ThresholdTool::Launch(Bitmap* pEditbmp) {

	if (__pEditBmp)
		delete __pEditBmp;

	__pEditBmp = Util::CopyBitmapN(*pEditbmp);

	if (__pCancelEditBmp)
		delete __pCancelEditBmp;

	__pCancelEditBmp = Util::CopyBitmapN(*pEditbmp);

	Process();
	ApplyThresholdValue(122);
}

void ThresholdTool::Process() {

	__offset = 0;

	//launch slider
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);
	pEditor->ActivateSlider(true, this, L"Threshold", 1, 255);

	pEditor = null;

}

void ThresholdTool::OnAdjustmentValueChanged(const Osp::Ui::Control &source,
		int adjustment) {
	ApplyThresholdValue(adjustment);
}

void ThresholdTool::ApplyThresholdValue(int offset) {

	__pProcessBmp = Util::CopyBitmapN(*__pEditBmp);

	if (__pProcessBmp == NULL) {
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

	if (bitsPerPixel == 32) {
		bytesPerPixel = 4;
	} else if (bitsPerPixel == 24) {
		bytesPerPixel = 3;
	}

	double newRed;
	double newBlue;
	double newGreen;
	//double scaledoffset = (offset + 100.0) / 100.0;
	double scaledoffset = offset * 3;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			double* color = ((double *) ((byte *) info.pPixels + y * info.pitch
					+ x * bytesPerPixel));

			byte* blue = (byte*) color;
			byte* green = blue + 1;
			byte* red = green + 1;

			newRed = (double(*red));
			newBlue = (double(*blue));
			newGreen = (double(*green));

			//newRed = (double(*red) / 255.0 - 0.5) * scaledoffset;
			//newBlue = (double(*blue) / 255.0 - 0.5) * scaledoffset;
			//newGreen = (double(*green) / 255.0 - 0.5) * scaledoffset;

			//newRed = (newRed + 0.5) * 255.0;
			//newBlue = (newBlue + 0.5) * 255.0;
			//newGreen = (newGreen + 0.5) * 255.0;

			//AppLog("---RGB: %f ---", newRed + newBlue + newGreen);

			//AppLog("--- Offset %d ---", offset);

			double rgbSUM = newRed + newBlue + newGreen;

			if (rgbSUM > scaledoffset) {
				newRed = 255;
				newBlue = 255;
				newGreen = 255;
			} else {
				newRed = 0;
				newBlue = 0;
				newGreen = 0;
			}

			/*newRed = newRed > 255 ? 255 : newRed;
			 newRed = newRed < 0 ? 0 : newRed;
			 newBlue = newBlue > 255 ? 255 : newBlue;
			 newBlue = newBlue < 0 ? 0 : newBlue;
			 newGreen = newGreen > 255 ? 255 : newGreen;
			 newGreen = newGreen < 0 ? 0 : newGreen;*/
			*red = (byte) newRed;
			*green = (byte) newGreen;
			*blue = (byte) newBlue;

		}
	}

	__pProcessBmp->Unlock();

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	pEditor->SetCurrentBitmap(__pProcessBmp);

	pEditor->DrawImageOnCanvas();

	pEditor->RedrawSlider();

	pEditor = null;

	__offset = offset;

}

void ThresholdTool::OnSliderSubmit() {
	Exit();
}

void ThresholdTool::EditCancel() {

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	pEditor->SetCurrentBitmap(__pCancelEditBmp);

	pEditor->DrawImageOnCanvas();

	pEditor = null;

}

void ThresholdTool::OnSliderCancel() {
	//ApplyThresholdValue(0);
	EditCancel();
	Exit();
}

void ThresholdTool::Exit() {
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	ArrayList* pArgList = new ArrayList();
	pArgList->Construct(1);

	pArgList->Add(*(new Integer(__offset)));
	pEditor->SendUserEvent(K_TOOL_ID, pArgList);

	pEditor = null;

	delete __pEditBmp;
	__pEditBmp = null;
}
