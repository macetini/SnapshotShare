#include <FApp.h>
#include "RotationTool.h"
#include "Editor.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Media;
using namespace Osp::Io;

RotationTool::RotationTool() :
	__pEditBmp(NULL)
{

}

RotationTool::~RotationTool()
{

}

void RotationTool::Launch(Bitmap* pEditbmp)
{

	__pEditBmp = Util::CopyBitmapN(*pEditbmp);
	__direction = IMAGE_ROTATION_0;

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->AddOrientationEventListener(*this);

	if (pEditor->GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT || pEditor->GetOrientationStatus()
			== ORIENTATION_STATUS_PORTRAIT_REVERSE)
		CreateButtons(0);
	else
		CreateButtons(1);

	pEditor = null;
}

void RotationTool::OnOrientationChanged(const Control &source, OrientationStatus orientationStatus)
{
	int i;
	RemoveButtons();
	if (orientationStatus == ORIENTATION_STATUS_PORTRAIT || orientationStatus == ORIENTATION_STATUS_PORTRAIT_REVERSE)
	{
		i = 0;
	}
	else
	{
		i = 1;
	}
	CreateButtons(i);
}

void RotationTool::CreateButtons(int mode)
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	Rectangle canvasBound = Rectangle(pEditor->GetImageBound());

	int btnwidth = canvasBound.width / 6;
	int btnheight = K_BTN_SIZE;

	Rectangle rectCWBtn = Rectangle(canvasBound.x + (canvasBound.width / 6), canvasBound.y + canvasBound.height
			- btnheight, btnwidth, btnheight);
	Rectangle rectACWBtn = Rectangle(canvasBound.x + 2 * (canvasBound.width / 6), canvasBound.y + canvasBound.height
			- btnheight, btnwidth, btnheight);
	Rectangle rectDoneBtn = Rectangle(canvasBound.x + 4 * (canvasBound.width / 6), canvasBound.y + canvasBound.height
			- btnheight, btnwidth, btnheight);

	__pCwBtn = new Button();
	__pCwBtn->Construct(rectCWBtn);
	__pCwBtn->SetActionId(K_CW_BTN);
	__pCwBtn->AddActionEventListener(*this);

	__pAcwBtn = new Button();
	__pAcwBtn->Construct(rectACWBtn);
	__pAcwBtn->SetActionId(K_ACW_BTN);
	__pAcwBtn->AddActionEventListener(*this);

	__pDoneBtn = new Button();
	__pDoneBtn->Construct(rectDoneBtn);
	__pDoneBtn->SetActionId(K_DONE_BTN);
	__pDoneBtn->AddActionEventListener(*this);

	Bitmap* pBitmapCw;
	Bitmap* pBitmapAcw;
	Bitmap* pBitmapCwP;
	Bitmap* pBitmapAcwP;
	Bitmap* pBitmapDone;
	Bitmap* pBitmapDoneP;

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	switch (mode)
	{
		case 0:
		{
			pBitmapCw = pAppResource->GetBitmapN(L"clockwise.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapAcw = pAppResource->GetBitmapN(L"anticlockwise.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapCwP = pAppResource->GetBitmapN(L"clockwisep.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapAcwP = pAppResource->GetBitmapN(L"anticlockwisep.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapDone = pAppResource->GetBitmapN(L"tick.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapDoneP = pAppResource->GetBitmapN(L"tickp.png", BITMAP_PIXEL_FORMAT_ARGB8888);

		}
		break;
		case 1:
		{
			pBitmapCw = pAppResource->GetBitmapN(L"landclockwise.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapAcw = pAppResource->GetBitmapN(L"landanticlockwise.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapCwP = pAppResource->GetBitmapN(L"landclockwisep.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapAcwP = pAppResource->GetBitmapN(L"landanticlockwisep.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapDone = pAppResource->GetBitmapN(L"tick.png", BITMAP_PIXEL_FORMAT_ARGB8888);
			pBitmapDoneP = pAppResource->GetBitmapN(L"tickp.png", BITMAP_PIXEL_FORMAT_ARGB8888);

		}
	}

	if (__pCwBtn == null)
	{
		AppLog("Null");

	}
	__pCwBtn->SetNormalBackgroundBitmap(*pBitmapCw);
	__pCwBtn->SetPressedBackgroundBitmap(*pBitmapCwP);

	__pAcwBtn->SetNormalBackgroundBitmap(*pBitmapAcw);
	__pAcwBtn->SetPressedBackgroundBitmap(*pBitmapAcwP);

	__pDoneBtn->SetNormalBackgroundBitmap(*pBitmapDone);
	__pDoneBtn->SetPressedBackgroundBitmap(*pBitmapDoneP);

	pEditor->AddToPanel(*__pCwBtn);
	pEditor->AddToPanel(*__pAcwBtn);
	pEditor->AddToPanel(*__pDoneBtn);

	pEditor = null;

	__pDoneBtn->Draw();
	__pDoneBtn->Show();
	__pCwBtn->Draw();
	__pCwBtn->Show();
	__pAcwBtn->Draw();
	__pAcwBtn->Show();

	delete pBitmapCw;
	delete pBitmapCwP;
	delete pBitmapAcw;
	delete pBitmapAcwP;
	delete pBitmapDone;
	delete pBitmapDoneP;

	pAppResource = null;
	pBitmapCw = null;
	pBitmapCwP = null;
	pBitmapAcw = null;
	pBitmapAcwP = null;
	pBitmapDone = null;
	pBitmapDoneP = null;
}

void RotationTool::RemoveButtons()
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	if (__pAcwBtn != NULL && __pCwBtn != NULL && __pDoneBtn != NULL)
	{

		pEditor->RemoveFromPanel(*__pAcwBtn);
		pEditor->RemoveFromPanel(*__pCwBtn);
		pEditor->RemoveFromPanel(*__pDoneBtn);

		__pAcwBtn = NULL;
		__pCwBtn = NULL;
		__pDoneBtn = NULL;
	}
	pEditor = null;

}

void RotationTool::OnActionPerformed(const Osp::Ui::Control &source, int actionId)
{
	switch (actionId)
	{
		case K_CW_BTN:
		{
			SetRotationAngle(IMAGE_ROTATION_90);
			Process();
			break;
		}
		case K_ACW_BTN:
		{
			SetRotationAngle(IMAGE_ROTATION_270);
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

void RotationTool::SetRotationAngle(ImageRotationType dir)
{
	__direction = dir;
}
void RotationTool::Process()
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

	r = ImageUtil::Rotate(srcBuf, dstBuf, Dimension(bufInfo.width, bufInfo.height), __direction,
			MEDIA_PIXEL_FORMAT_BGRA8888);

	Dimension destDim;

	TryCatch(r == E_SUCCESS ,, "ImageUtil::Flip failed:%s", GetErrorMessage(r));

	pDstBmp = new Bitmap();

	if (__direction == IMAGE_ROTATION_90 || __direction == IMAGE_ROTATION_270)
	{
		destDim = Dimension(bufInfo.height, bufInfo.width);
	}
	else
	{
		destDim = Dimension(bufInfo.width, bufInfo.height);
	}
	pDstBmp->Construct(dstBuf, destDim, BITMAP_PIXEL_FORMAT_ARGB8888, BUFFER_SCALING_NONE);

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

void RotationTool::Exit()
{
	RemoveButtons();

	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	pEditor->RemoveOrientationEventListener(*this);
	ArrayList* pArg;
	pArg = new ArrayList();
	pArg->Construct(1);

	int dir = __direction == IMAGE_ROTATION_0 ? 0 : 1;
	pArg->Add(*(new Integer(dir)));
	pEditor->SendUserEvent(K_TOOL_ID, pArg);

	pEditor = null;

	delete __pEditBmp;
	__pEditBmp = null;
}
