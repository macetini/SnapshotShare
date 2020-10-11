#include <FApp.h>
#include "Editor.h"
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

Osp::Base::String Editor::K_SCREEN_ID = "Editor";

Editor::Editor(void) :
	__pMainPanel(NULL), __pColorMenu(NULL), __pSizeMenu(NULL), __pBorderMenu(
			NULL), __pCurrentBitmap(NULL), __pBackupArray(NULL),
			__pGrayscaleTool(NULL), __pThresholdTool(NULL), __pFlipTool(NULL),
			__pRotateTool(NULL), __pCropTool(NULL), __pSliderListener(NULL) //, __pBrightnessTool(NULL), __pBorderTool(NULL)
{

}

Editor::~Editor(void) {
}

bool Editor::Initialize() {
	Form::Construct(L"IDF_EDIT");

	return true;
}

result Editor::OnInitializing(void) {

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	result r = E_SUCCESS;
	Button* pButtonColor = static_cast<Button *> (GetControl("IDC_BTN_COLOR"));
	Button* pButtonResize =
			static_cast<Button *> (GetControl("IDC_BTN_RESIZE"));
	//Button* pButtonBorder = static_cast <Button *> (GetControl("IDC_BTN_BORDER"));

	//pButtonBorder->SetEnabled(false);

	if (pButtonColor != null) {
		pButtonColor->SetActionId(ID_HEADERBTN_COLOR);
		pButtonColor->AddActionEventListener(*this);
	}

	if (pButtonResize != null) {
		pButtonResize->SetActionId(ID_HEADERBTN_IMGSIZE);
		pButtonResize->AddActionEventListener(*this);
	}

	/*if (pButtonBorder != null)
	 {
	 pButtonBorder->SetActionId(ID_HEADERBTN_BORDERS);
	 //pButtonBorder->AddActionEventListener(*this);
	 }*/

	__pMainPanel = static_cast<Panel *> (GetControl("IDC_MAINPANEL"));

	if (__pMainPanel == NULL) {
		AppLog("Could not construct main panel");

		return E_FAILURE;
	}

	__pGallery = static_cast<Gallery *> (__pMainPanel->GetControl(
			L"IDC_GALLERY"));
	if (__pGallery) {
		__pGallery->SetTextOfEmptyGallery(L"Image could not be displayed.");
		r = __pGallery->SetItemProvider(*this);
	}
	__pMainPanel->SetControlAlwaysAtBottom(*__pGallery, true);

	AddOrientationEventListener(*this);

	__pBackupArray = new ArrayList();
	__pBackupArray->Construct();

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

	__pSlider = static_cast<Slider *> (__pMainPanel->GetControl(L"IDC_SLIDER"));
	if (__pSlider) {
		Rectangle sliderBound = __pSlider->GetBounds();

		Rectangle sliderbtnBound;
		sliderbtnBound.x = __pSlider->GetX() + __pSlider->GetWidth();
		sliderbtnBound.y = sliderBound.y;
		sliderbtnBound.width = K_SLIDER_BUTTON_SIZE;
		sliderbtnBound.height = sliderBound.height;

		__pSliderSubmitBtn = new Button();
		__pSliderSubmitBtn->Construct(sliderbtnBound);
		__pSliderSubmitBtn->SetActionId(ID_SLIDER_DONE);
		__pSliderSubmitBtn->SetNormalBackgroundBitmap(*pBitmapDone);
		__pSliderSubmitBtn->SetPressedBackgroundBitmap(*pBitmapDoneP);
		__pSliderSubmitBtn->AddActionEventListener(*this);

		__pSliderCancelBtn = new Button();
		__pSliderCancelBtn->Construct(sliderbtnBound);
		__pSliderCancelBtn->SetActionId(ID_SLIDER_CANCEL);
		__pSliderCancelBtn->SetNormalBackgroundBitmap(*pBitmapCancel);
		__pSliderCancelBtn->SetPressedBackgroundBitmap(*pBitmapCancelP);
		__pSliderCancelBtn->AddActionEventListener(*this);

		__pSlider->SetShowState(false);
		__pSliderSubmitBtn->SetShowState(false);
		__pSliderCancelBtn->SetShowState(false);

		AddToPanel(*__pSliderSubmitBtn);
		AddToPanel(*__pSliderCancelBtn);
	}

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

result Editor::AddToPanel(Control& control) {
	result r = E_FAILURE;

	if (__pMainPanel) {
		r = __pMainPanel->AddControl(control);
	}

	return r;
}

result Editor::RemoveFromPanel(Control& control) {
	result r = E_FAILURE;

	if (__pMainPanel) {
		r = __pMainPanel->RemoveControl(control);
	}

	return r;
}

GalleryItem* Editor::CreateItem(int index) {
	GalleryItem* pItem;
	pItem = new GalleryItem();
	pItem->Construct(*__pCurrentBitmap, GALLERY_IMAGE_ROTATION_0);

	return pItem;

}

bool Editor::DeleteItem(int index, GalleryItem* pItem) {
	delete pItem;
	pItem = NULL;

	return true;
}

int Editor::GetItemCount() {
	return 1;
}

void Editor::ActivateSlider(bool activate, ISliderEnabledTool* pListener,
		String title, int min, int max) {
	if (activate) {
		__pSliderListener = pListener;
		__pSlider->AddAdjustmentEventListener(*__pSliderListener);
		__pSlider->SetRange(min, max);
		__pSlider->SetTitleText(title);
		__pSlider->SetValue((max + min) / 2);
	} else {
		__pSlider->RemoveAdjustmentEventListener(*__pSliderListener);
	}

	__pSlider->SetShowState(activate);
	__pSliderSubmitBtn->SetShowState(activate);
	__pSliderCancelBtn->SetShowState(activate);

	__pMainPanel->Draw();
	__pMainPanel->Show();
}

void Editor::ResetSliderPosition() {
	Rectangle galleryBound = __pGallery->GetBounds();
	Rectangle sliderBound = __pGallery->GetBounds();
	sliderBound.height = 106;
	sliderBound.width -= 2 * K_SLIDER_BUTTON_SIZE;
	sliderBound.x = 5;//galleryBound.x + (galleryBound.width - sliderBound.width) / 2 - K_SLIDER_BUTTON_SIZE;
	sliderBound.y = 20;//galleryBound.y + (galleryBound.height - sliderBound.height) / 2;

	__pSlider->SetPosition(Point(sliderBound.x, sliderBound.y));
	__pSlider->SetSize(sliderBound.width, sliderBound.height);

	Rectangle sliderbtnBound;
	sliderbtnBound.x = __pSlider->GetX() + __pSlider->GetWidth();
	sliderbtnBound.y = sliderBound.y;
	sliderbtnBound.width = K_SLIDER_BUTTON_SIZE;
	sliderbtnBound.height = sliderBound.height;

	Rectangle sliderbtnCancelBound;
	sliderbtnCancelBound.x = sliderbtnBound.x + sliderbtnBound.width;
	sliderbtnCancelBound.y = sliderbtnBound.y;
	sliderbtnCancelBound.width = K_SLIDER_BUTTON_SIZE;
	sliderbtnCancelBound.height = sliderbtnBound.height;

	__pSliderSubmitBtn->SetPosition(Point(sliderbtnBound.x, sliderbtnBound.y));
	__pSliderCancelBtn->SetPosition(Point(sliderbtnCancelBound.x,
			sliderbtnCancelBound.y));

	RedrawSlider();
}

void Editor::RedrawSlider() {
	__pSliderSubmitBtn->Draw();
	__pSliderSubmitBtn->Show();
	__pSliderCancelBtn->Draw();
	__pSliderCancelBtn->Show();
	__pSlider->Draw();
	__pSlider->Show();
}

void Editor::OnFormBackRequested(Osp::Ui::Controls::Form &source) {
	ImageViewer* pImageViewer =
			(ImageViewer *) FormManager::GetInstance()->GetFormById(
					ImageViewer::K_SCREEN_ID);

	pImageViewer->SetImagePath(__currentImagePath);
	pImageViewer = null;

	ClearEditor();
	FormManager::GetInstance()->Back();
}

void Editor::OnActionPerformed(const Osp::Ui::Control &source, int actionId) {

	switch (actionId) {
	case Editor::ID_FOOTERBTN_SAVE: {
		OnSave();

		break;
	}
	case Editor::ID_FOOTERBTN_UNDO: {
		OnUndo();
		break;
	}
	case Editor::ID_FOOTERBTN_REDO: {
		OnRedo();
		break;
	}
	case Editor::ID_HEADERBTN_COLOR: {
		Point pt;
		if (GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT
				|| GetOrientationStatus()
						== ORIENTATION_STATUS_PORTRAIT_REVERSE)

		{
			pt = Point(source.GetX() + source.GetWidth() / 2, source.GetY()
					+ source.GetHeight() + GetHeader()->GetHeight());
		} else {
			pt = Point(source.GetX() + source.GetWidth(), source.GetY()
					+ source.GetHeight() / 2 + GetHeader()->GetHeight() / 2);
		}
		if (__pColorMenu == NULL) {

			__pColorMenu = new ContextMenu();
			__pColorMenu->Construct(pt, CONTEXT_MENU_STYLE_LIST);
			__pColorMenu->SetColor(Color::COLOR_VIOLET);
			__pColorMenu->SetItemColor(CONTEXT_MENU_ITEM_STATUS_PRESSED, Color(
					238, 154, 0));
			__pColorMenu->AddItem("Grayscale", ID_HEADERBTN_COLORGRAYSCALE);
			__pColorMenu->AddItem("Threshold", ID_HEADERBTN_COLORTHRESHOLD);
			__pColorMenu->AddItem("Flip Color", ID_HEADERBTN_COLORFLIP);
			__pColorMenu->AddItem("Clear", ID_HEADERBTN_COLORDELETE);
			//__pColorMenu->AddItem("Brightness", ID_HEADERBTN_COLORBRIGHTNESS);
			__pColorMenu->AddActionEventListener(*this);
		}

		__pColorMenu->SetShowState(true);
		__pColorMenu->SetPosition(pt);
		__pColorMenu->Show();

		break;
	}
		/*case Editor::ID_HEADERBTN_BORDERS:
		 {
		 Point pt;
		 if (GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT || GetOrientationStatus()
		 == ORIENTATION_STATUS_PORTRAIT_REVERSE)

		 {
		 pt = Point(source.GetX() + source.GetWidth() / 2, source.GetY() + source.GetHeight()
		 + GetHeader()->GetHeight());
		 }
		 else
		 {
		 pt = Point(source.GetX() + source.GetWidth(), source.GetY() + source.GetHeight() / 2
		 + GetHeader()->GetHeight() / 2);
		 }
		 if (__pSizeMenu == NULL)
		 {

		 __pSizeMenu = new ContextMenu();
		 __pSizeMenu->Construct(pt, CONTEXT_MENU_STYLE_LIST);
		 __pSizeMenu->SetColor(Color::COLOR_VIOLET);
		 __pSizeMenu->SetItemColor(CONTEXT_MENU_ITEM_STATUS_PRESSED, Color(238, 154, 0));

		 __pSizeMenu->AddItem("Border 1", ID_HEADERBTN_BORDER1);
		 __pSizeMenu->AddItem("Border 2", ID_HEADERBTN_BORDER2);

		 __pSizeMenu->AddActionEventListener(*this);
		 }

		 __pSizeMenu->SetShowState(true);
		 __pSizeMenu->SetPosition(pt);
		 __pSizeMenu->Show();

		 break;
		 }*/
	case Editor::ID_HEADERBTN_IMGSIZE: {
		Point pt;
		if (GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT
				|| GetOrientationStatus()
						== ORIENTATION_STATUS_PORTRAIT_REVERSE)

		{
			pt = Point(source.GetX() + source.GetWidth() / 2, source.GetY()
					+ source.GetHeight() + GetHeader()->GetHeight());
		} else {
			pt = Point(source.GetX() + source.GetWidth(), source.GetY()
					+ source.GetHeight() / 2 + GetHeader()->GetHeight() / 2);
		}
		if (__pBorderMenu == NULL) {
			__pBorderMenu = new ContextMenu();
			__pBorderMenu->Construct(pt, CONTEXT_MENU_STYLE_LIST);
			__pBorderMenu->SetColor(Color::COLOR_VIOLET);
			__pBorderMenu->SetItemColor(CONTEXT_MENU_ITEM_STATUS_PRESSED,
					Color(238, 154, 0));

			__pBorderMenu->AddItem("Crop", ID_HEADERBTN_IMGSIZECROP);
			__pBorderMenu->AddItem("Rotate", ID_HEADERBTN_IMGSIZEROTATE);
			__pBorderMenu->AddItem("Flip", ID_HEADERBTN_IMGSIZEFLIP);

			__pBorderMenu->AddActionEventListener(*this);
		}
		__pBorderMenu->SetShowState(true);
		__pBorderMenu->SetPosition(pt);
		__pBorderMenu->Show();

		break;
	}

		/*case ID_HEADERBTN_COLORBRIGHTNESS:
		 {
		 ApplyBrightness();

		 break;
		 }*/

	case ID_HEADERBTN_COLORTHRESHOLD: {
		ApplyThreshold();

		break;
	}

	case ID_HEADERBTN_COLORFLIP:
	{
		ApplyFlipColor();

		break;
	}

	case ID_HEADERBTN_COLORDELETE: {
		ApplyDelete();

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

		/*case ID_HEADERBTN_BORDER1:
		 {
		 ApplyBorderEffect(BorderTool::K_BORDER_1);

		 break;
		 }
		 case ID_HEADERBTN_BORDER2:
		 {
		 ApplyBorderEffect(BorderTool::K_BORDER_2);

		 break;
		 }*/

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

result Editor::OnTerminating(void) {
	result r = E_SUCCESS;

	return r;
}

result Editor::OnDraw() {
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

void Editor::OnUserEventReceivedN(RequestId requestId, IList* pArgs) {
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

		/*case BrightnessTool::K_TOOL_ID:
		 {
		 if (((Integer *) (pArgs->GetAt(0)))->CompareTo(Integer(0)) == 0)
		 {
		 changeOccured = false;
		 }
		 if (__pBrightnessTool)
		 {
		 delete __pBrightnessTool;
		 __pBrightnessTool = null;
		 }

		 break;
		 }*/
	case ThresholdTool::K_TOOL_ID: {
		if (((Integer *) (pArgs->GetAt(0)))->CompareTo(Integer(0)) == 0) {

			changeOccured = false;
		}
		if (__pThresholdTool) {
			delete __pThresholdTool;
			__pThresholdTool = null;
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

		/*case BorderTool::K_TOOL_ID:
		 {
		 if (__pBorderTool)
		 {
		 delete __pBorderTool;
		 __pBorderTool = null;
		 }
		 }
		 break;*/
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

void Editor::SetImagePath(String path) {
	__currentImagePath = path;
	__isScaled = true;

	LoadImage();

	__historyState = K_HIST_NONE;

}

void Editor::SetCurrentBitmap(Bitmap* pNewBmp) {
	delete __pCurrentBitmap;
	__pCurrentBitmap = NULL;
	__pCurrentBitmap = Util::CopyBitmapN(*pNewBmp);

	delete pNewBmp;
	pNewBmp = NULL;

}

void Editor::SetTitle() {
	String fileName = L"";
	int index;
	__currentImagePath.LastIndexOf(L"/", __currentImagePath.GetLength() - 1,
			index);
	__currentImagePath.SubString(index + 1, fileName);

	Header* pHeader = GetHeader();
	pHeader->SetTitleText(fileName);
	pHeader->SetTitleTextColor(Color::COLOR_WHITE);

}

void Editor::LoadImage() {
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

Rectangle Editor::GetImageBound() {
	return __pGallery->GetBounds();
}

void Editor::DrawImageOnCanvas() {
	__pGallery->UpdateGallery();
	__pGallery->Draw();
	__pGallery->Show();

}

Rectangle Editor::GetImageBounds() {
	return __bmpRectangle;
}

Dimension Editor::GetImageSize() {
	return __imageSize;
}

void Editor::ApplyGrayScaleFilter() {

	EnableUi(false);
	BackUpCurrentBitmap();
	if (__pGrayscaleTool == NULL) {
		__pGrayscaleTool = new GrayscaleTool();
	}

	__pGrayscaleTool->Launch(__pCurrentBitmap);

}

/*void Editor::ApplyBrightness()
 {

 EnableUi(false);
 BackUpCurrentBitmap();
 if (__pBrightnessTool == NULL)
 {
 __pBrightnessTool = new BrightnessTool();
 }

 __pBrightnessTool->Launch(__pCurrentBitmap);
 }*/

void Editor::ApplyThreshold() {
	EnableUi(false);
	BackUpCurrentBitmap();
	if (__pThresholdTool == NULL) {
		__pThresholdTool = new ThresholdTool();
	}

	__pThresholdTool->Launch(__pCurrentBitmap);
}

void Editor::ApplyFlipColor() {
	EnableUi(false);
	BackUpCurrentBitmap();
	if (__pFlipColorTool == NULL) {
		__pFlipColorTool = new FlipColorTool();
	}

	__pFlipColorTool->Launch(__pCurrentBitmap);
}

void Editor::ApplyDelete() {
	EnableUi(false);
	BackUpCurrentBitmap();
	if (__pDeleteTool == NULL) {
		__pDeleteTool = new DeleteTool();
	}

	__pDeleteTool->Launch(__pCurrentBitmap);
}

void Editor::ApplyFlipEffect() {
	EnableUi(false);
	BackUpCurrentBitmap();

	if (__pFlipTool == NULL) {
		__pFlipTool = new FlipTool();
	}
	__pFlipTool->Launch(__pCurrentBitmap);
}

void Editor::ApplyRotateEffect() {
	EnableUi(false);
	BackUpCurrentBitmap();

	if (__pRotateTool == NULL) {
		__pRotateTool = new RotationTool();
	}
	__pRotateTool->Launch(__pCurrentBitmap);

}

void Editor::ApplyCropEffect() {
	EnableUi(false);
	BackUpCurrentBitmap();

	if (__pCropTool == NULL) {
		__pCropTool = new CropTool();
	}
	__pCropTool->Launch(__pCurrentBitmap);
}

/*void Editor::ApplyBorderEffect(int borderId)
 {
 EnableUi(false);

 if (__pCurrentBitmap->GetWidth() < 20 || __pCurrentBitmap->GetHeight() < 20)
 {
 MessageBox messageBox;
 messageBox.Construct(L"","Cannot Add Border\nImage is too small", MSGBOX_STYLE_NONE, 2000);

 int modalResult = 0;
 messageBox.ShowAndWait(modalResult);
 EnableUi(true);
 return;

 }

 BackUpCurrentBitmap();

 if (__pBorderTool == NULL)
 {
 __pBorderTool = new BorderTool();
 }
 __pBorderTool->SetBorderId(borderId);
 __pBorderTool->Launch(__pCurrentBitmap);

 }*/

void Editor::BackUpCurrentBitmap() {
	Bitmap* pPrevBitmap = Util::CopyBitmapN(*__pCurrentBitmap);
	__pBackupArray->InsertAt(*pPrevBitmap, 0);
}

void Editor::TrimBackUpArray() {
	if (__pBackupArray->GetCount() > 1) {
		__pBackupArray->RemoveItems(1, __pBackupArray->GetCount() - 1, true);
	}
}

void Editor::OnUndo() {
	if (__pCurrentBitmap) {
		Bitmap* pTempBmp = __pCurrentBitmap;
		Bitmap* pLastBackedupBitmap = (Bitmap *) __pBackupArray->GetAt(0);
		__pCurrentBitmap = Util::CopyBitmapN(*pLastBackedupBitmap);

		__pBackupArray->InsertAt(*pTempBmp, 0);
		TrimBackUpArray();

	}
	__historyState = K_HIST_UNDO;
	UpdateUndoRedo();
	DrawImageOnCanvas();

}

void Editor::OnRedo() {
	if (__pCurrentBitmap) {
		Bitmap* pTempBmp = __pCurrentBitmap;
		Bitmap* pLastBackedupBitmap = (Bitmap *) __pBackupArray->GetAt(0);
		__pCurrentBitmap = Util::CopyBitmapN(*pLastBackedupBitmap);

		__pBackupArray->InsertAt(*pTempBmp, 0);
		TrimBackUpArray();
	}
	__historyState = K_HIST_REDO;
	UpdateUndoRedo();

	DrawImageOnCanvas();
}

void Editor::OnSave() {
	Util::ShowMessage(ID_MSG_SAVE, "Save Image...",
			"Are you sure you want to save your changes", this,
			MSGBOX_STYLE_YESNO);
}

void Editor::OnMessageBoxResult(int messageId, int result) {
	switch (messageId) {
	case ID_MSG_SAVE: {
		if (result == MSGBOX_RESULT_YES) {
			String filepath = String(__currentImagePath);

			if (File::IsFileExist(filepath)) {
				Util::ShowMessage(ID_MSG_SAVEREPLACE, "Save Image...",
						"Do you want to keep your original file?", this,
						MSGBOX_STYLE_YESNO);
			}

		}
		break;
	}
	case ID_MSG_SAVEREPLACE: {
		bool createNew;
		if (result == MSGBOX_RESULT_YES) {
			createNew = true;
		} else {
			createNew = false;
		}
		SaveImage(createNew);
		break;
	}

		EnableUi(true);
	}

}

void Editor::SaveImage(bool createNew) {
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

void Editor::UpdateUndoRedo() {

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

void Editor::EnableUi(bool enabled) {
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

	Button* pButtonColor = static_cast<Button *> (GetControl(L"IDC_BTN_COLOR"));
	Button* pButtonResize =
			static_cast<Button *> (GetControl(L"IDC_BTN_RESIZE"));
	//Button* pButtonBorder = static_cast <Button *> (GetControl(L"IDC_BTN_BORDER"));

	pButtonColor->SetEnabled(enabled);
	pButtonColor->Draw();
	pButtonColor->Show();
	pButtonResize->SetEnabled(enabled);
	pButtonResize->Draw();
	pButtonResize->Show();
	/*pButtonBorder->SetEnabled(false);
	 pButtonBorder->Draw();
	 pButtonBorder->Show();*/

}

void Editor::ClearEditor() {
	if (__pCurrentBitmap) {
		delete __pCurrentBitmap;
		__pCurrentBitmap = NULL;
	}

	__pBackupArray->RemoveAll(true);
	UpdateUndoRedo();
}

void Editor::OnOrientationChanged(const Osp::Ui::Control &source,
		Osp::Ui::OrientationStatus orientationStatus) {
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	Bitmap* pBitmapColor;
	Bitmap* pBitmapTransform;
	//Bitmap* pBitmapBorder;
	Bitmap* pBitmapTransformP;
	//Bitmap* pBitmapBorderP;
	Bitmap* pBitmapColorP;

	Button* pButtonColor = static_cast<Button *> (GetControl("IDC_BTN_COLOR"));
	Button* pButtonResize =
			static_cast<Button *> (GetControl("IDC_BTN_RESIZE"));
	//Button* pButtonBorder = static_cast <Button *> (GetControl("IDC_BTN_BORDER"));

	Rectangle rect;
	rect = GetClientAreaBounds();
	int valH = pButtonColor->GetHeight();
	int valW = pButtonColor->GetWidth();

	switch (orientationStatus) {
	case ORIENTATION_STATUS_PORTRAIT: {
		protrait:

		pBitmapColor = pAppResource->GetBitmapN(L"brushicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransform = pAppResource->GetBitmapN(L"sizeicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		//pBitmapBorder = pAppResource->GetBitmapN(L"bordericon.png", BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapColorP = pAppResource->GetBitmapN(L"brushiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransformP = pAppResource->GetBitmapN(L"sizeiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		//pBitmapBorderP = pAppResource->GetBitmapN(L"bordericon.png", BITMAP_PIXEL_FORMAT_ARGB8888);

		__pMainPanel->SetBounds(0, valH, rect.width, rect.height - valH);
		__pGallery->SetBounds(0, 0, rect.width, (rect.height - (valH)));
	}
		break;

	case ORIENTATION_STATUS_LANDSCAPE: {
		landscape:

		pButtonColor->SetPosition(0, 0);
		pButtonColor->SetSize(120, 176);
		pButtonResize->SetPosition(0, 171);
		pButtonResize->SetSize(120, 176);

		pBitmapColor = pAppResource->GetBitmapN(L"landbrushicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransform = pAppResource->GetBitmapN(L"landtransformicon.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		//pBitmapBorder = pAppResource->GetBitmapN(L"landbordericon.png", BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapColorP = pAppResource->GetBitmapN(L"landbrushiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapTransformP = pAppResource->GetBitmapN(L"landtransformiconp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
		//pBitmapBorderP = pAppResource->GetBitmapN(L"landbordericonp.png", BITMAP_PIXEL_FORMAT_ARGB8888);


		__pMainPanel->SetBounds(valW, 0, rect.width - valW, rect.height);
		__pGallery->SetBounds(0, 0, rect.width - valW, rect.height);

	}
		break;

	case ORIENTATION_STATUS_PORTRAIT_REVERSE: {
		goto protrait;
	}
		break;

	case ORIENTATION_STATUS_LANDSCAPE_REVERSE: {
		goto landscape;
	}
		break;
	case ORIENTATION_STATUS_NONE: {

	}
		break;

	}

	{
		pButtonColor->SetNormalBackgroundBitmap(*pBitmapColor);
		pButtonColor->SetPressedBackgroundBitmap(*pBitmapColorP);
	}

	if (pButtonResize != null) {
		pButtonResize->SetNormalBackgroundBitmap(*pBitmapTransform);
		pButtonResize->SetPressedBackgroundBitmap(*pBitmapTransformP);
	}

	/*if (pButtonBorder != null)
	 {
	 pButtonBorder->SetNormalBackgroundBitmap(*pBitmapBorder);
	 pButtonBorder->SetPressedBackgroundBitmap(*pBitmapBorderP);
	 }*/

	Draw();

	delete pBitmapColor;
	delete pBitmapTransform;
	//delete pBitmapBorder;
	delete pBitmapTransformP;
	//delete pBitmapBorderP;
	delete pBitmapColorP;

	pAppResource = null;
	pBitmapColor = null;
	pBitmapTransform = null;
	//pBitmapBorder = null;
	pBitmapTransformP = null;
	//pBitmapBorderP = null;
	pBitmapColorP = null;

}

