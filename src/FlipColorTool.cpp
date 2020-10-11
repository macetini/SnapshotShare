#include "FlipColorTool.h"
#include "Editor.h"

using namespace Osp::Graphics;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui::Controls;

FlipColorTool::FlipColorTool() :
	__pEditBmp(NULL), __pProcessBmp(NULL){

}

FlipColorTool::~FlipColorTool() {

}

void FlipColorTool::Launch(Bitmap* pEditbmp) {

	if (__pEditBmp)
		delete __pEditBmp;

	__pEditBmp = Util::CopyBitmapN(*pEditbmp);

	Process();
}

void FlipColorTool::Process() {

	AppLog("--- Flip Color ---");
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

			int rgbSUM = newRed + newBlue + newGreen;

			if (rgbSUM == 0) {
				newRed = 255;
				newBlue = 255;
				newGreen = 255;
			} else {
				newRed = 0;
				newBlue = 0;
				newGreen = 0;
			}

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

	Exit();

}

void FlipColorTool::Exit() {

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	pEditor->SendUserEvent(K_TOOL_ID, null);

	pEditor = null;

	delete __pEditBmp;
	__pEditBmp = null;
}
