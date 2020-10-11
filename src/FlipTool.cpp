#include <FApp.h>
#include "FlipTool.h"
#include "Editor.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Media;

FlipTool::FlipTool() :
	__pEditBmp(NULL)
{

}

FlipTool::~FlipTool()
{

}

void FlipTool::Launch(Bitmap* pEditbmp)
{

	__pEditBmp = Util::CopyBitmapN(*pEditbmp);
	__direction = IMAGE_FLIP_NONE;

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->AddOrientationEventListener(*this);

	if (pEditor->GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT || pEditor->GetOrientationStatus()
			== ORIENTATION_STATUS_PORTRAIT_REVERSE)
		CreateButtons(0);
	else
		CreateButtons(1);

	pEditor = null;
}

void FlipTool::OnOrientationChanged(const Control &source, OrientationStatus orientationStatus)
{
	int i;
	RemoveButtons();
	if (orientationStatus == ORIENTATION_STATUS_PORTRAIT || orientationStatus == ORIENTATION_STATUS_PORTRAIT_REVERSE)
		i = 0;
	else
		i = 1;
	CreateButtons(i);
}

void FlipTool::CreateButtons(int mode)
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	Bitmap* pBitmapH;
	Bitmap* pBitmapV;
	Bitmap* pBitmapHP;
	Bitmap* pBitmapVP;
	Bitmap* pBitmapDone;
	Bitmap* pBitmapDoneP;

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	switch (mode)
	{
		case 0:
		{
			pBitmapH = pAppResource->GetBitmapN(L"fliphorizontal.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapV = pAppResource->GetBitmapN(L"flipvertical.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapHP = pAppResource->GetBitmapN(L"fliphorizontalP.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapVP = pAppResource->GetBitmapN(L"flipverticalP.png", BITMAP_PIXEL_FORMAT_ARGB8888);

		}
		break;
		case 1:
		{
			pBitmapH = pAppResource->GetBitmapN(L"landfliphorizontal.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapV = pAppResource->GetBitmapN(L"landflipvertical.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapHP = pAppResource->GetBitmapN(L"landfliphorizontalP.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapVP = pAppResource->GetBitmapN(L"landflipverticalP.png", BITMAP_PIXEL_FORMAT_ARGB8888);

		}
	}

	pBitmapDone = pAppResource->GetBitmapN(L"tick.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapDoneP = pAppResource->GetBitmapN(L"tickp.png", BITMAP_PIXEL_FORMAT_ARGB8888);

	Rectangle canvasBound = Rectangle(pEditor->GetImageBound());

	int btnwidth = canvasBound.width / 6;
	int btnheight = K_BTN_SIZE;

	Rectangle rectHFBtn = Rectangle(canvasBound.x + (canvasBound.width / 6), canvasBound.y + canvasBound.height
			- btnheight, btnwidth, btnheight);
	Rectangle rectVFBtn = Rectangle(canvasBound.x + 2 * (canvasBound.width / 6), canvasBound.y + canvasBound.height
			- btnheight, btnwidth, btnheight);
	Rectangle rectDoneBtn = Rectangle(canvasBound.x + 4 * (canvasBound.width / 6), canvasBound.y + canvasBound.height
			- btnheight, btnwidth, btnheight);

	__pHfBtn = new Button();
	__pHfBtn->Construct(rectHFBtn);
	__pHfBtn->SetActionId(K_HF_BTN);
	__pHfBtn->SetNormalBackgroundBitmap(*pBitmapH);
	__pHfBtn->SetPressedBackgroundBitmap(*pBitmapHP);
	__pHfBtn->AddActionEventListener(*this);

	__pVfBtn = new Button();
	__pVfBtn->Construct(rectVFBtn);
	__pVfBtn->SetActionId(K_VF_BTN);
	__pVfBtn->SetNormalBackgroundBitmap(*pBitmapV);
	__pVfBtn->SetPressedBackgroundBitmap(*pBitmapVP);
	__pVfBtn->AddActionEventListener(*this);

	__pDoneBtn = new Button();
	__pDoneBtn->Construct(rectDoneBtn);
	__pDoneBtn->SetNormalBackgroundBitmap(*pBitmapDone);
	__pDoneBtn->SetPressedBackgroundBitmap(*pBitmapDoneP);
	__pDoneBtn->SetTextColor(Color::COLOR_WHITE);
	__pDoneBtn->SetActionId(K_DONE_BTN);
	__pDoneBtn->AddActionEventListener(*this);

	pEditor->AddToPanel(*__pHfBtn);
	pEditor->AddToPanel(*__pVfBtn);
	pEditor->AddToPanel(*__pDoneBtn);

	pEditor->Draw();
	pEditor->Show();

	pEditor = null;

	delete pBitmapH;
	delete pBitmapV;
	delete pBitmapHP;
	delete pBitmapVP;
	delete pBitmapDone;
	delete pBitmapDoneP;

	pAppResource = null;
	pBitmapH = null;
	pBitmapV = null;
	pBitmapHP = null;
	pBitmapVP = null;
	pBitmapDone = null;
	pBitmapDone = null;

}

void FlipTool::RemoveButtons()
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	if (__pVfBtn != NULL && __pHfBtn != NULL && __pDoneBtn != NULL)
	{

		pEditor->RemoveFromPanel(*__pVfBtn);
		pEditor->RemoveFromPanel(*__pHfBtn);
		pEditor->RemoveFromPanel(*__pDoneBtn);

		__pVfBtn = NULL;
		__pHfBtn = NULL;
		__pDoneBtn = NULL;
	}
	pEditor = null;
}

void FlipTool::OnActionPerformed(const Osp::Ui::Control &source, int actionId)
{
	switch (actionId)
	{
		case K_HF_BTN:
		{
			SetFlipDirection(IMAGE_FLIP_HORIZONTAL);
			Process();
			break;
		}
		case K_VF_BTN:
		{
			SetFlipDirection(IMAGE_FLIP_VERTICAL);
			Process();
			break;
		}
		case K_DONE_BTN:
		{
			Exit();
			break;
		}

	}

}

void FlipTool::SetFlipDirection(ImageFlipType dir)
{
	__direction = dir;
}
void FlipTool::Process()
{
	BufferInfo bufInfo;
	ByteBuffer srcBuf, dstBuf;
	Bitmap* pDstBmp = null;
	result r = E_SUCCESS;
	byte* pPixels;
	int bytesPerLine;

	__pEditBmp->Lock(bufInfo);
	srcBuf.Construct(bufInfo.height * bufInfo.width * bufInfo.bitsPerPixel / 8);
	pPixels = (byte*) bufInfo.pPixels;
	bytesPerLine = bufInfo.width * bufInfo.bitsPerPixel / 8;

	// copy pixel data into buf
	if (bytesPerLine != bufInfo.pitch)
	{
		for (int i = 0; i < bufInfo.height; i++)
		{
			srcBuf.SetArray(pPixels, 0, bytesPerLine);
			pPixels += bufInfo.pitch;
		}
	}
	else
	{
		srcBuf.SetArray(pPixels, 0, bufInfo.pitch * bufInfo.height);
	}
	__pEditBmp->Unlock();
	srcBuf.Flip();
	dstBuf.Construct(bufInfo.height * bufInfo.width * bufInfo.bitsPerPixel / 8);

	r = ImageUtil::Flip(srcBuf, dstBuf, Dimension(bufInfo.width, bufInfo.height), __direction,
			MEDIA_PIXEL_FORMAT_BGRA8888);
	TryCatch(r == E_SUCCESS, , "ImageUtil::Flip failed:%s", GetErrorMessage(r));
	pDstBmp = new Bitmap();
	pDstBmp->Construct(dstBuf, Dimension(bufInfo.width, bufInfo.height), BITMAP_PIXEL_FORMAT_ARGB8888,
			BUFFER_SCALING_NONE);

	CATCH: SetLastResult(r);

	__pEditBmp = Util::CopyBitmapN(*pDstBmp);

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->SetCurrentBitmap(pDstBmp);

	pEditor->DrawImageOnCanvas();
	pEditor->RedrawSlider();

	delete pPixels;

	pPixels = null;
	pEditor = null;

}

void FlipTool::Exit()
{
	RemoveButtons();

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	pEditor->RemoveOrientationEventListener(*this);

	ArrayList* pArg;
	pArg = new ArrayList();
	pArg->Construct(1);

	int dir = __direction == IMAGE_FLIP_NONE ? 0 : 1;
	pArg->Add(*(new Integer(dir)));
	pEditor->SendUserEvent(K_TOOL_ID, pArg);

	delete __pEditBmp;
	__pEditBmp = null;
}
