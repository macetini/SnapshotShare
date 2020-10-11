#include <FApp.h>
#include "CropTool.h"
#include "Editor.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Graphics;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Media;

CropTool::CropTool() :
	__pEditBmp(NULL), __pDoneBtn(NULL), __pCancelBtn(NULL), __pCropBox(NULL), __pPanel(NULL)
{

}

CropTool::~CropTool()
{

}

void CropTool::Launch(Bitmap* pEditbmp)
{
	__pEditBmp = Util::CopyBitmapN(*pEditbmp);

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->AddOrientationEventListener(*this);

	pEditor = null;

	CreatePanel();
	CreateButtons();
	DrawRectangle();

	__pPanel->Draw();
	__pPanel->Show();

}

void CropTool::OnActionPerformed(const Control &source, int actionId)
{
	switch (actionId)
	{
		case K_BTN_DONE:
		{
			Rectangle rect = __pCropBox->GetBoundingBox();
			Process(rect);

			break;
		}

		case K_BTN_CANCEL:
		{
			Exit();
			break;
		}
	}
}

void CropTool::OnOrientationChanged(const Control &source, OrientationStatus orientationStatus)
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	RemoveButtons();
	RemoveRectangle();
	RemovePanel();

	CreatePanel();
	CreateButtons();
	DrawRectangle();

	pEditor->Draw();

	pEditor = null;

}

void CropTool::CreatePanel()
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	if (__pPanel == NULL)
	{
		__pPanel = new Panel();
		Rectangle bounds = pEditor->GetImageBound();
		__pPanel->Construct(bounds, GROUP_STYLE_NONE);
	}

	pEditor->AddToPanel(*__pPanel);

	pEditor = null;
}

void CropTool::RemovePanel()
{

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	if (__pPanel)
	{
		pEditor->RemoveFromPanel(*__pPanel);
		__pPanel = NULL;
	}

	pEditor = null;
}

void CropTool::CreateButtons()
{

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	Rectangle canvasBound = __pPanel->GetBounds();

	int buttonWidth = canvasBound.width / 7;
	int buttonHeight = buttonWidth;

	Rectangle rectDoneBtn = Rectangle(canvasBound.width / 2 - buttonWidth - 40, canvasBound.height - buttonHeight,
			buttonWidth, buttonHeight);
	Bitmap* pBitmapDone;
	Bitmap* pBitmapCancel;
	Bitmap* pBitmapDoneP;
	Bitmap* pBitmapCancelP;
	Image* pBitmapDecoder = new Image();
	pBitmapDecoder->Construct();
	pBitmapDone = pAppResource->GetBitmapN(L"tick.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapCancel = pAppResource->GetBitmapN(L"cancel.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapDoneP = pAppResource->GetBitmapN(L"tickp.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapCancelP = pAppResource->GetBitmapN(L"cancelp.png", BITMAP_PIXEL_FORMAT_ARGB8888);

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

void CropTool::RemoveButtons()
{
	if (__pDoneBtn)
	{
		__pPanel->RemoveControl(*__pDoneBtn);
		__pPanel->RemoveControl(*__pCancelBtn);

		__pDoneBtn = NULL;
	}

}

void CropTool::DrawRectangle()
{

	if (__pCropBox == NULL)
	{
		__pCropBox = new CroppingRectangle();
		__pCropBox->Init(Dimension(100, 100), __pPanel->GetBounds());
	}

	__pPanel->AddControl(*__pCropBox);
	__pPanel->SetControlAlwaysAtBottom(*__pCropBox, true);

}

void CropTool::RemoveRectangle()
{

	if (__pCropBox)
	{
		__pPanel->RemoveControl(*__pCropBox);

		__pCropBox = NULL;
	}

}

void CropTool::OnCropDone(Rectangle box)
{
	Process(box);
}

void CropTool::Process(Rectangle box)
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	Bitmap* pDstBmp;
	pDstBmp = new Bitmap();

	//In order to have the cropped image size in the same proportion of the original,
	//We have to re-scale as per original bitmap size

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
	result r = pDstBmp->Construct(*__pEditBmp, cropRect);
	if (IsFailed(r))
	{
		AppLog("Bitmap construct failed in CropTool::Process");

		return;
	}

	pEditor->SetCurrentBitmap(pDstBmp);

	pEditor = null;

	Exit();

}

void CropTool::Exit()
{
	RemoveButtons();
	RemoveRectangle();
	RemovePanel();

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->RemoveOrientationEventListener(*this);
	pEditor->SendUserEvent(K_TOOL_ID, null);

	pEditor = null;

	delete __pEditBmp;
	__pEditBmp = null;
}

