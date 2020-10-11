#include <FApp.h>
#include "DeleteTool.h"
#include "Editor.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Graphics;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Media;

DeleteTool::DeleteTool() :
	__pEditBmp(NULL), __pDoneBtn(NULL), __pCancelBtn(NULL), __pCropBox(NULL),
			__pPanel(NULL) {

}

DeleteTool::~DeleteTool() {

}

void DeleteTool::Launch(Bitmap* pEditbmp) {
	__pEditBmp = Util::CopyBitmapN(*pEditbmp);

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);
	pEditor->AddOrientationEventListener(*this);

	pEditor = null;

	CreatePanel();
	CreateButtons();
	DrawRectangle();

	__pPanel->Draw();
	__pPanel->Show();

}

void DeleteTool::OnActionPerformed(const Control &source, int actionId) {
	switch (actionId) {
	case K_BTN_DONE: {
		Rectangle rect = __pCropBox->GetBoundingBox();
		Process(rect);

		break;
	}

	case K_BTN_CANCEL: {
		Exit();
		break;
	}
	}
}

void DeleteTool::OnOrientationChanged(const Control &source,
		OrientationStatus orientationStatus) {
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	RemoveButtons();
	RemoveRectangle();
	RemovePanel();

	CreatePanel();
	CreateButtons();
	DrawRectangle();

	pEditor->Draw();

	pEditor = null;

}

void DeleteTool::CreatePanel() {
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	if (__pPanel == NULL) {
		__pPanel = new Panel();
		Rectangle bounds = pEditor->GetImageBound();
		__pPanel->Construct(bounds, GROUP_STYLE_NONE);
	}

	pEditor->AddToPanel(*__pPanel);

	pEditor = null;
}

void DeleteTool::RemovePanel() {

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	if (__pPanel) {
		pEditor->RemoveFromPanel(*__pPanel);
		__pPanel = NULL;
	}

	pEditor = null;
}

void DeleteTool::CreateButtons() {

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	Rectangle canvasBound = __pPanel->GetBounds();

	int buttonWidth = canvasBound.width / 7;
	int buttonHeight = buttonWidth;

	Rectangle rectDoneBtn = Rectangle(canvasBound.width / 2 - buttonWidth - 40,
			canvasBound.height - buttonHeight, buttonWidth, buttonHeight);
	Bitmap* pBitmapDone;
	Bitmap* pBitmapCancel;
	Bitmap* pBitmapDoneP;
	Bitmap* pBitmapCancelP;
	Image* pBitmapDecoder = new Image();
	pBitmapDecoder->Construct();
	pBitmapDone = pAppResource->GetBitmapN(L"tick.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapCancel = pAppResource->GetBitmapN(L"cancel.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapDoneP = pAppResource->GetBitmapN(L"tickp.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapCancelP = pAppResource->GetBitmapN(L"cancelp.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);

	__pDoneBtn = new Button();
	__pDoneBtn->Construct(rectDoneBtn);
	__pDoneBtn->SetActionId(K_BTN_DONE);
	__pDoneBtn->SetNormalBackgroundBitmap(*pBitmapDone);
	__pDoneBtn->SetPressedBackgroundBitmap(*pBitmapDoneP);
	__pDoneBtn->SetHighlightedBackgroundBitmap(*pBitmapDoneP);
	__pDoneBtn->AddActionEventListener(*this);
	__pDoneBtn->SetName(L"CROP_DONE_BTN");

	rectDoneBtn.x = canvasBound.width / 2;
	__pCancelBtn = new Button();
	__pCancelBtn->Construct(rectDoneBtn);
	__pCancelBtn->SetActionId(K_BTN_CANCEL);
	__pCancelBtn->SetNormalBackgroundBitmap(*pBitmapCancel);
	__pCancelBtn->SetPressedBackgroundBitmap(*pBitmapCancelP);
	__pCancelBtn->SetHighlightedBackgroundBitmap(*pBitmapCancelP);
	__pCancelBtn->AddActionEventListener(*this);
	__pCancelBtn->SetName(L"CROP_CANCEL_BTN");

	__pPanel->AddControl(*__pDoneBtn);
	__pPanel->AddControl(*__pCancelBtn);

	delete pBitmapDone;
	delete pBitmapDoneP;
	delete pBitmapCancel;
	delete pBitmapCancelP;

	pAppResource = null;
	pBitmapDone = null;
	pBitmapDoneP = null;
	pBitmapCancel = null;
	pBitmapCancelP = null;

}

void DeleteTool::RemoveButtons() {
	if (__pDoneBtn) {
		__pPanel->RemoveControl(*__pDoneBtn);
		__pPanel->RemoveControl(*__pCancelBtn);

		__pDoneBtn = NULL;
	}

}

void DeleteTool::DrawRectangle() {

	if (__pCropBox == NULL) {
		__pCropBox = new CroppingRectangle();
		__pCropBox->Init(Dimension(100, 100), __pPanel->GetBounds());
	}

	__pPanel->AddControl(*__pCropBox);
	__pPanel->SetControlAlwaysAtBottom(*__pCropBox, true);

}

void DeleteTool::RemoveRectangle() {

	if (__pCropBox) {
		__pPanel->RemoveControl(*__pCropBox);

		__pCropBox = NULL;
	}

}

void DeleteTool::OnCropDone(Rectangle box) {
	Process(box);
}

void DeleteTool::Process(Rectangle box) {

	__pProcessBmp = Util::CopyBitmapN(*__pEditBmp);

	if (__pProcessBmp == NULL) {
		return;
	}

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	Rectangle galleryBound = pEditor->GetImageBound();

	float hScale = float(__pEditBmp->GetWidth()) / float(galleryBound.width);
	float vScale = float(__pEditBmp->GetHeight()) / float(galleryBound.height);
	float scale = Float::Compare(hScale, vScale) > 0 ? hScale : vScale;

	int scaledWidth;
	float w = float(__pEditBmp->GetWidth()) / scale;
	scaledWidth = int(w);
	int scaledHeight;
	float h = float(__pEditBmp->GetHeight()) / scale;
	scaledHeight = int(h);

	int imgX = (galleryBound.width - scaledWidth) / 2;
	int imgY = (galleryBound.height - scaledHeight) / 2;
	int left = (box.x - imgX) * scale;
	int top = (box.y - imgY) * scale;
	int width = box.width * scale;
	int height = box.height * scale;

	Rectangle cropRect = Rectangle(left, top, width, height);

	int x1 = left;
	int y1 = top;

	int x2 = left + width;
	int y2 = top + height;

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

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			double* color = ((double *) ((byte *) info.pPixels + y * info.pitch
					+ x * bytesPerPixel));

			byte* blue = (byte*) color;
			byte* green = blue + 1;
			byte* red = green + 1;

			if (x > x1 && x < x2 && y > y1 && y < y2) {

				*red = (byte) 255;
				*green = (byte) 255;
				*blue = (byte) 255;
			}

		}
	}

	__pProcessBmp->Unlock();

	pEditor->SetCurrentBitmap(__pProcessBmp);

	pEditor->DrawImageOnCanvas();

	pEditor->RedrawSlider();

	pEditor = null;

	Exit();

}

void DeleteTool::Exit() {
	RemoveButtons();
	RemoveRectangle();
	RemovePanel();

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);
	pEditor->RemoveOrientationEventListener(*this);
	pEditor->SendUserEvent(K_TOOL_ID, null);

	pEditor = null;

	delete __pEditBmp;
	__pEditBmp = null;
}

