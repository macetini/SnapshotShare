#include <FApp.h>
#include "ShowHttpResult.h"
#include "FormManager.h"
#include "ImageViewer.h"
#include "Main.h"
#include "Util.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Media;
using namespace Osp::Io;
using namespace Osp::Content;

Osp::Base::String ShowHttpResult::K_SCREEN_ID = "ShowHttpResult";

ShowHttpResult::ShowHttpResult(void)
{

}

ShowHttpResult::~ShowHttpResult(void) {
}

bool ShowHttpResult::Initialize() {
	Form::Construct(L"IDF_SHOW_HTTP_RESULT");

	return true;
}

result ShowHttpResult::OnInitializing(void) {

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	result r = E_SUCCESS;

	AddOrientationEventListener(*this);

	//Add Footer Buttons
	Footer* pFooter = GetFooter();
	pFooter->SetBackButton();
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_NORMAL, Color(82, 37, 98));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_PRESSED, Color(160, 80, 8));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_HIGHLIGHTED, Color(160, 80, 8));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_DISABLED, Color(82, 37, 98));

	Bitmap* pBitmap;
	Bitmap* pBitmapPressed;
	Bitmap* pBitmapDone;
	Bitmap* pBitmapDoneP;
	Bitmap* pBitmapCancel;
	Bitmap* pBitmapCancelP;

	pBitmap = pAppResource->GetBitmapN(L"btn1_hor.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapDone = pAppResource->GetBitmapN(L"tick.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapDoneP = pAppResource->GetBitmapN(L"tickp.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapCancel = pAppResource->GetBitmapN(L"cancel.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapCancelP = pAppResource->GetBitmapN(L"cancelp.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapPressed = pAppResource->GetBitmapN(L"btn2.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);

	FooterItem footerUndo;
	footerUndo.Construct(ID_FOOTERBTN_UNDO);
	footerUndo.SetText(L"Undo");
	footerUndo.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, pBitmap);
	footerUndo.SetBackgroundBitmap(FOOTER_ITEM_STATUS_PRESSED, pBitmapPressed);
	footerUndo.SetBackgroundBitmap(FOOTER_ITEM_STATUS_DISABLED, pBitmap);

	pFooter->AddItem(footerUndo);

	FooterItem footerRedo;
	footerRedo.Construct(ID_FOOTERBTN_REDO);
	footerRedo.SetText(L"Redo");
	footerRedo.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, pBitmap);
	footerRedo.SetBackgroundBitmap(FOOTER_ITEM_STATUS_PRESSED, pBitmapPressed);
	footerRedo.SetBackgroundBitmap(FOOTER_ITEM_STATUS_DISABLED, pBitmap);

	pFooter->AddItem(footerRedo);

	FooterItem footerSave;
	footerSave.Construct(ID_FOOTERBTN_SAVE);
	footerSave.SetText(L"Save");
	footerSave.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, pBitmap);
	footerSave.SetBackgroundBitmap(FOOTER_ITEM_STATUS_PRESSED, pBitmapPressed);
	footerSave.SetBackgroundBitmap(FOOTER_ITEM_STATUS_DISABLED, pBitmap);

	pFooter->AddItem(footerSave);

	pFooter->AddActionEventListener(*this);

	delete pBitmap;
	delete pBitmapPressed;
	delete pBitmapDone;
	delete pBitmapDoneP;
	delete pBitmapCancel;
	delete pBitmapCancelP;

	pAppResource = null;
	pBitmap = null;
	pBitmapPressed = null;
	pBitmapDone = null;
	pBitmapDoneP = null;
	pBitmapCancel = null;
	pBitmapCancelP = null;

	SetFormBackEventListener(this);

	return r;
}

result ShowHttpResult::AddToPanel(Control& control) {
	result r = E_FAILURE;

	if (__pMainPanel) {
		r = __pMainPanel->AddControl(control);
	}

	return r;
}

result ShowHttpResult::RemoveFromPanel(Control& control) {
	result r = E_FAILURE;

	if (__pMainPanel) {
		r = __pMainPanel->RemoveControl(control);
	}

	return r;
}

GalleryItem* ShowHttpResult::CreateItem(int index) {
	GalleryItem* pItem;
	pItem = new GalleryItem();
	pItem->Construct(*__pCurrentBitmap, GALLERY_IMAGE_ROTATION_0);

	return pItem;

}

bool ShowHttpResult::DeleteItem(int index, GalleryItem* pItem) {
	delete pItem;
	pItem = NULL;

	return true;
}

int ShowHttpResult::GetItemCount() {
	return 1;
}

void ShowHttpResult::OnFormBackRequested(Osp::Ui::Controls::Form &source) {
	ImageViewer* pImageViewer =
			(ImageViewer *) FormManager::GetInstance()->GetFormById(
					ImageViewer::K_SCREEN_ID);

	pImageViewer->SetImagePath(__currentImagePath);
	pImageViewer = null;

	ClearShowHttpResult();
	FormManager::GetInstance()->Back();
}

void ShowHttpResult::OnActionPerformed(const Osp::Ui::Control &source,
		int actionId) {

	switch (actionId) {
	case ShowHttpResult::ID_FOOTERBTN_SAVE: {
		break;
	}
	case ShowHttpResult::ID_FOOTERBTN_UNDO: {
		break;
	}
	case ShowHttpResult::ID_FOOTERBTN_REDO: {
		OnRedo();
		break;
	}

	case ID_HEADERBTN_COLORBRIGHTNESS: {
		ApplyBrightness();

		break;
	}

	case ID_HEADERBTN_COLORCONTRAST: {
		ApplyContrast();

		break;
	}

	case ID_HEADERBTN_COLORGRAYSCALE: {
		ApplyGrayScaleFilter();

		break;
	}

	case ID_HEADERBTN_IMGSIZECROP: {
		ApplyCropEffect();
		break;
	}

	case ID_HEADERBTN_IMGSIZEROTATE: {
		ApplyRotateEffect();
		break;
	}

	case ID_HEADERBTN_IMGSIZEFLIP: {
		ApplyFlipEffect();

		break;
	}

	case ID_HEADERBTN_BORDER1: {
		ApplyBorderEffect(BorderTool::K_BORDER_1);

		break;
	}
	case ID_HEADERBTN_BORDER2: {
		ApplyBorderEffect(BorderTool::K_BORDER_2);

		break;
	}

	case ID_SLIDER_DONE: {
		if (__pSliderListener) {
			__pSliderListener->OnSliderSubmit();
			ActivateSlider(false, NULL);
		}
		break;
	}
	case ID_SLIDER_CANCEL: {
		if (__pSliderListener) {
			__pSliderListener->OnSliderCancel();
			ActivateSlider(false, NULL);
		}
		break;
	}
	}
}

result ShowHttpResult::OnTerminating(void) {
	result r = E_SUCCESS;

	return r;
}

result ShowHttpResult::OnDraw() {
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	Footer* pFooter = GetFooter();
	Bitmap* pBitmapFooter;

	if (GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT
			|| GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT_REVERSE) {
		pBitmapFooter = pAppResource->GetBitmapN(L"backgp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
	} else {
		pBitmapFooter = pAppResource->GetBitmapN(L"backg.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
	}
	pFooter->SetBackgroundBitmap(pBitmapFooter);
	result r = E_SUCCESS;
	DrawImageOnCanvas();
	ResetSliderPosition();
	SetTitle();
	UpdateUndoRedo();

	delete pBitmapFooter;

	pBitmapFooter = null;
	pAppResource = null;

	return r;
}

void ShowHttpResult::OnUserEventReceivedN(RequestId requestId, IList* pArgs) {
	bool changeOccured = true;

	switch (requestId) {
	case CroppingRectangle::K_EVT_CROP_DONE: {
		if (__pCropTool) {
			Rectangle box = *(Rectangle *) pArgs->GetAt(0);
			__pCropTool->OnCropDone(box);
			changeOccured = false;
			delete __pCropTool;
			__pCropTool = null;
			break;
		}

	}

	case BrightnessTool::K_TOOL_ID: {
		if (((Integer *) (pArgs->GetAt(0)))->CompareTo(Integer(0)) == 0) {
			changeOccured = false;
		}
		if (__pBrightnessTool) {
			delete __pBrightnessTool;
			__pBrightnessTool = null;
		}

		break;
	}
	case ContrastTool::K_TOOL_ID: {
		if (((Integer *) (pArgs->GetAt(0)))->CompareTo(Integer(0)) == 0) {

			changeOccured = false;
		}
		if (__pContrastTool) {
			delete __pContrastTool;
			__pContrastTool = null;
		}

		break;
	}
	case FlipTool::K_TOOL_ID: {
		if (((Integer *) (pArgs->GetAt(0)))->CompareTo(Integer(0)) == 0) {
			changeOccured = false;
		}
		if (__pFlipTool) {
			delete __pFlipTool;
			__pFlipTool = null;
		}

		break;
	}
	case RotationTool::K_TOOL_ID: {
		if (((Integer *) (pArgs->GetAt(0)))->CompareTo(Integer(0)) == 0) {

			changeOccured = false;
		}
		if (__pRotateTool) {
			delete __pRotateTool;
			__pRotateTool = null;
		}

		break;

		case BorderTool::K_TOOL_ID:
		{
			if (__pBorderTool) {
				delete __pBorderTool;
				__pBorderTool = null;
			}
		}
		break;
		case GrayscaleTool::K_TOOL_ID:
		{
			if (__pGrayscaleTool) {
				delete __pGrayscaleTool;
				__pGrayscaleTool = null;
			}
		}
		break;
		case CropTool::K_TOOL_ID:
		{
			if (__pCropTool) {
				delete __pCropTool;
				__pCropTool = null;
			}
		}
		break;
	}
	}

	if (requestId > 1000) {
		if (changeOccured) {

			__historyState = K_HIST_REDO;

		} else {
			__pBackupArray->RemoveAt(0, true);
		}
		TrimBackUpArray();
		EnableUi(true);
		DrawImageOnCanvas();

	}

	if (pArgs) {
		pArgs->RemoveAll(true);
		delete pArgs;
		pArgs = null;
	}

}

void ShowHttpResult::SetImagePath(String path) {
	__currentImagePath = path;
	__isScaled = true;

	LoadImage();

	__historyState = K_HIST_NONE;

}

void ShowHttpResult::SetCurrentBitmap(Bitmap* pNewBmp) {
	delete __pCurrentBitmap;
	__pCurrentBitmap = NULL;
	__pCurrentBitmap = Util::CopyBitmapN(*pNewBmp);

	delete pNewBmp;
	pNewBmp = NULL;

}

void ShowHttpResult::SetTitle() {
	String fileName = L"";
	int index;
	__currentImagePath.LastIndexOf(L"/", __currentImagePath.GetLength() - 1,
			index);
	__currentImagePath.SubString(index + 1, fileName);

	Header* pHeader = GetHeader();
	pHeader->SetTitleText(fileName);
	pHeader->SetTitleTextColor(Color::COLOR_WHITE);

}

void ShowHttpResult::LoadImage() {
	result r;

	Image* pBitmapDecoder = new Image();

	r = pBitmapDecoder->Construct();
	if (IsFailed(r)) {
		AppLog("Failed to construct Image");

		return;
	}

	if (__pCurrentBitmap) {
		delete __pCurrentBitmap;
		__pCurrentBitmap = NULL;

	}

	__pCurrentBitmap = pBitmapDecoder->DecodeN(__currentImagePath,
			BITMAP_PIXEL_FORMAT_ARGB8888, K_IMG_WIDTH, K_IMG_HEIGHT);
	if (IsFailed(GetLastResult())) {
		AppLog("Could not load Image %ls",GetErrorMessage(GetLastResult()));

		return;
	}

	delete pBitmapDecoder;
	pBitmapDecoder = null;
}

Rectangle ShowHttpResult::GetImageBound() {
	//return __pGallery->GetBounds();
}

void ShowHttpResult::DrawImageOnCanvas() {
	/*__pGallery->UpdateGallery();
	 __pGallery->Draw();
	 __pGallery->Show();*/

}

Rectangle ShowHttpResult::GetImageBounds() {
	return __bmpRectangle;
}

Dimension ShowHttpResult::GetImageSize() {
	return __imageSize;
}

void ShowHttpResult::ApplyFlipEffect() {
}

void ShowHttpResult::ApplyRotateEffect() {
}

void ShowHttpResult::ApplyCropEffect() {
}

void ShowHttpResult::ApplyBorderEffect(int borderId) {
}

void ShowHttpResult::BackUpCurrentBitmap() {
}

void ShowHttpResult::TrimBackUpArray() {
}

void ShowHttpResult::OnSave() {
}

void ShowHttpResult::OnMessageBoxResult(int messageId, int result) {
}

void ShowHttpResult::SaveImage(bool createNew) {
	String filepath = String(__currentImagePath);
	Image* pImage = new Image();
	ImageContentInfo info;
	ContentManager contentMgr;

	result r = pImage->Construct();
	if (IsFailed(r))
		goto CATCH;

	int insertindex;
	if (createNew) {
		do {
			filepath.LastIndexOf(String("."), filepath.GetLength() - 1,
					insertindex);
			if (insertindex < 0) {
				insertindex = filepath.GetLength() - 1;
			}
			filepath.Insert("_edited", insertindex);
		} while (File::IsFileExist(filepath));
	}

	r = pImage->EncodeToFile(*__pCurrentBitmap, IMG_FORMAT_JPG, filepath, true);

	if (IsFailed(r)) {
		AppLog("File could not be saved.");

		goto CATCH;
	} else {
		SetImagePath(filepath);

	}

	info.Construct(filepath);
	contentMgr.Construct();

	contentMgr.CreateContent(info);

	CATCH: if (pImage)
		delete pImage;

}

void ShowHttpResult::UpdateUndoRedo() {

	Footer* pFooter = GetFooter();

	if (__historyState == K_HIST_NONE) {
		pFooter->SetItemEnabled(0, false);
		pFooter->SetItemEnabled(1, false);
	} else if (__historyState == K_HIST_UNDO) {
		pFooter->SetItemEnabled(0, false);
		pFooter->SetItemEnabled(1, true);
	} else {
		pFooter->SetItemEnabled(0, true);
		pFooter->SetItemEnabled(1, false);
	}

	pFooter->Draw();
	pFooter->Show();

}

void ShowHttpResult::EnableUi(bool enabled) {
	Footer* pFooter = GetFooter();

	pFooter->SetBackButtonEnabled(enabled);
	int totalItems = pFooter->GetItemCount();
	for (int i = 0; i < totalItems; i++) {
		pFooter->SetItemEnabled(i, enabled);
	}

	if (enabled) {
		UpdateUndoRedo();
	} else {

	}

	pFooter->Draw();
	pFooter->Show();
}

void ShowHttpResult::ClearShowHttpResult() {
	if (__pCurrentBitmap) {
		delete __pCurrentBitmap;
		__pCurrentBitmap = NULL;
	}

	__pBackupArray->RemoveAll(true);
	UpdateUndoRedo();
}

void ShowHttpResult::OnOrientationChanged(const Osp::Ui::Control &source,
		Osp::Ui::OrientationStatus orientationStatus) {
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	Bitmap* pBitmapColor;
	Bitmap* pBitmapTransform;
	Bitmap* pBitmapBorder;
	Bitmap* pBitmapTransformP;
	Bitmap* pBitmapBorderP;
	Bitmap* pBitmapColorP;

	Rectangle rect;
	rect = GetClientAreaBounds();

	switch (orientationStatus) {
	case ORIENTATION_STATUS_PORTRAIT: {
		protrait:

		pBitmapColor = pAppResource->GetBitmapN(L"brushicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransform = pAppResource->GetBitmapN(L"sizeicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapBorder = pAppResource->GetBitmapN(L"bordericon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapColorP = pAppResource->GetBitmapN(L"brushiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransformP = pAppResource->GetBitmapN(L"sizeiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapBorderP = pAppResource->GetBitmapN(L"bordericonp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
	}
		break;

	case ORIENTATION_STATUS_LANDSCAPE: {
		landscape:

		pBitmapColor = pAppResource->GetBitmapN(L"landbrushicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransform = pAppResource->GetBitmapN(L"landtransformicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapBorder = pAppResource->GetBitmapN(L"landbordericon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapColorP = pAppResource->GetBitmapN(L"landbrushiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransformP = pAppResource->GetBitmapN(L"landtransformiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapBorderP = pAppResource->GetBitmapN(L"landbordericonp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);

		//__pMainPanel->SetBounds(valW, 0, rect.width - valW, rect.height);
		//__pGallery->SetBounds(0, 0, rect.width - valW, rect.height);

	}
		break;

	case ORIENTATION_STATUS_PORTRAIT_REVERSE: {
	}
		break;

	case ORIENTATION_STATUS_LANDSCAPE_REVERSE: {
	}
		break;
	case ORIENTATION_STATUS_NONE: {

	}
		break;
	}

	delete pBitmapColor;
	delete pBitmapTransform;
	delete pBitmapBorder;
	delete pBitmapTransformP;
	delete pBitmapBorderP;
	delete pBitmapColorP;

	pAppResource = null;
	pBitmapColor = null;
	pBitmapTransform = null;
	pBitmapBorder = null;
	pBitmapTransformP = null;
	pBitmapBorderP = null;
	pBitmapColorP = null;

}

