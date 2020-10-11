#include <FApp.h>
#include "Main.h"
#include "SnapshotShare.h"
#include "FormManager.h"
#include "ImageGallery.h"
#include "CameraCaptureHandler.h"
#include "ImageViewer.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Content;
using namespace Osp::Graphics;
using namespace Osp::App;
using namespace Osp::Media;

Osp::Base::String Main::K_SCREEN_ID = "Main";

Main::Main(void) :
	__pCamHandler(NULL)
{
}

Main::~Main(void)
{
	if (__pCamHandler)
	{
		delete __pCamHandler;
		__pCamHandler = NULL;
	}
}

bool Main::Initialize()
{
	Construct(L"IDF_Main");

	return true;
}

result Main::OnInitializing(void)
{
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	result r = E_SUCCESS;

	Bitmap* pBitmapCam;
	Bitmap* pBitmapView;
	Bitmap* pBitmapCamP;
	Bitmap* pBitmapViewP;
	Bitmap* pBitmapBg;

	pBitmapCam = pAppResource->GetBitmapN(L"capture-image.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapView = pAppResource->GetBitmapN(L"view-edit.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapCamP = pAppResource->GetBitmapN(L"capture-imageP.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapViewP = pAppResource->GetBitmapN(L"view-editP.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapBg = pAppResource->GetBitmapN(L"mainbg.png", BITMAP_PIXEL_FORMAT_ARGB8888);

	//Gallery Button
	__pButtonGallery = static_cast <Button *> (GetControl(L"IDC_BTN_GALLERY"));
	if (__pButtonGallery != null)
	{
		__pButtonGallery->SetActionId(ID_BTN_GALLERY);
		__pButtonGallery->AddActionEventListener(*this);
		__pButtonGallery->SetNormalBackgroundBitmap(*pBitmapView);
		__pButtonGallery->SetPressedBackgroundBitmap(*pBitmapViewP);

	}

	//Capture Button
	__pButtonCapture = static_cast <Button *> (GetControl(L"IDC_BTN_CAPTURE"));
	if (__pButtonCapture != null)
	{
		__pButtonCapture->SetActionId(ID_BTN_CAPTURE);
		__pButtonCapture->AddActionEventListener(*this);
		__pButtonCapture->SetNormalBackgroundBitmap(*pBitmapCam);
		__pButtonCapture->SetPressedBackgroundBitmap(*pBitmapCamP);
	}

	Label* pLabel;
	pLabel = static_cast <Label *> (GetControl(L"IDC_LABEL1"));
	pLabel->SetBackgroundBitmap(*pBitmapBg);

	delete pBitmapCam;
	delete pBitmapView;
	delete pBitmapCamP;
	delete pBitmapViewP;
	delete pBitmapBg;

	pAppResource = null;
	pBitmapCam = null;
	pBitmapView = null;
	pBitmapCamP = null;
	pBitmapViewP = null;
	pBitmapBg = null;

	return r;
}
result Main::OnDraw()
{
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	result r;
	Canvas* pCanvas;
	Bitmap* pBitmap;

	if (GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT || GetOrientationStatus()
			== ORIENTATION_STATUS_PORTRAIT_REVERSE)
	{
		pBitmap = pAppResource->GetBitmapN(L"BG.png", BITMAP_PIXEL_FORMAT_ARGB8888);

	}
	else
	{
		pBitmap = pAppResource->GetBitmapN(L"BGR.png", BITMAP_PIXEL_FORMAT_ARGB8888);

	}
	if (!pBitmap)
	{
		AppLog("Failed to decode image!");
	}

	r = pBitmap->Construct(*pBitmap, GetBounds());

	pCanvas = this->GetCanvasN();
	pCanvas->DrawBitmap(Point(0, 0), *pBitmap);

	delete pCanvas;
	delete pBitmap;

	pCanvas = null;
	pAppResource = null;
	pBitmap = null;

	return r;
}

result Main::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}

void Main::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch (actionId)
	{
		case ID_BTN_GALLERY:
		{
			//Load Image Gallery
			LoadGallery();

			break;
		}

		case ID_BTN_CAPTURE:
		{
			//Launch the Camera
			LaunchCamera();

			break;
		}

		default:
		break;
	}
}

void Main::LoadGallery()
{
	ImageGallery* pGallery;
	pGallery = (ImageGallery*) FormManager::GetInstance()->GetFormById(ImageGallery::K_SCREEN_ID);

	if (!(pGallery))
	{
		pGallery = new ImageGallery();
		if (!pGallery)
		{
			return;
		}
		Boolean res = pGallery->Initialize();
		FormManager::GetInstance()->Register(ImageGallery::K_SCREEN_ID, pGallery);
	}

	FormManager::GetInstance()->Jump(ImageGallery::K_SCREEN_ID);

	return;

}

void Main::LaunchCamera()
{
	ArrayList* pDataList = new ArrayList();
	String* pData = new String(L"type:camera");

	pDataList->Construct();
	pDataList->Add(*pData);

	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_PROVIDER_CAMERA, APPCONTROL_OPERATION_CAPTURE);
	if (pAc)
	{
		if (__pCamHandler == NULL)
		{
			__pCamHandler = new CameraCaptureHandler();

			//Create call back that is to be invoked once image is captured
			ImageViewer* pViewer = (ImageViewer*) FormManager::GetInstance()->GetFormById(ImageViewer::K_SCREEN_ID);
			if (pViewer == NULL)
			{
				pViewer = new ImageViewer();
				if (pViewer)
				{
					pViewer->Initialize();
					FormManager::GetInstance()->Register(ImageViewer::K_SCREEN_ID, pViewer);
				}
				else
				{
					AppLog("Failed to create Editor Form.");

					goto EXIT;
				}
			}

			//Construct with call back of type ICamHandlerCallBack
			__pCamHandler->Construct(pViewer);
		}
		if (__pCamHandler)
		{
			pAc->Start(pDataList, __pCamHandler);
		}

	}

	EXIT: if (pAc)
	{
		delete pAc;
	}
	pDataList->RemoveAll(true);
	delete pDataList;

}

