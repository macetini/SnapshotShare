#include "ImageViewer.h"
#include "Editor.h"
#include "ImageGallery.h"
#include "FormManager.h"

#include <FApp.h>
#include <FNet.h>
#include <FMedia.h>
#include <FGraphics.h>
#include <FIo.h>
#include <FUi.h>
#include <FBase.h>
#include <FText.h>
#include <FBaseInteger.h>

using namespace Osp::Base;
using namespace Osp::Graphics;
using namespace Osp::System;
using namespace Osp::App;
using namespace Osp::Ui::Controls;
using namespace Osp::Net::Http;
using namespace Osp::Media;
using namespace Osp::Io;
using namespace Osp::Base::Utility;
using namespace Osp::Base::Collection;
using namespace Osp::Text;
using namespace Osp::Net;
using namespace Osp::Ui;
using namespace Osp::Base::Runtime;
using namespace Osp::Social::Services;
using namespace Osp::Content;

Osp::Base::String ImageViewer::K_SCREEN_ID = L"ImageViewer";

ImageViewer::ImageViewer() :
	__pGallery(NULL) {
	__pPopup = null;
	__pPhotoName = null;
}

ImageViewer::~ImageViewer() {
}

bool ImageViewer::Initialize() {
	result r = Form::Construct(FORM_STYLE_NORMAL | FORM_STYLE_HEADER
			| FORM_STYLE_FOOTER | FORM_STYLE_INDICATOR);

	SetBackgroundColor(Color::COLOR_BLACK);
	SetOrientation(ORIENTATION_AUTOMATIC);

	if (IsFailed(r)) {
		AppLog("Form::Construct() is failed by %s.", GetErrorMessage(r));

		return false;
	}

	if (__pGallery == NULL) {
		__pGallery = new Gallery();

		Rectangle rectangle = GetClientAreaBounds();
		rectangle.x = 0;
		rectangle.y = 0;

		r = __pGallery->Construct(rectangle);
		if (IsFailed(r)) {
			AppLog("Fail to construct the Gallery instance.");
			return false;
		}

		__pGallery->SetTextOfEmptyGallery(L"Image could not be displayed.");
		__pGallery->SetItemProvider(*this);
		AddControl(*__pGallery);
	}

	return true;
}

void ImageViewer::SetImagePath(String filePath) {
	__currentFilePath = filePath;
	String fileName = L"";
	int index;
	__currentFilePath.LastIndexOf(L"/", __currentFilePath.GetLength() - 1,
			index);
	__currentFilePath.SubString(index + 1, fileName);

	Header* pHeader = GetHeader();
	pHeader->SetTitleText(fileName);
	pHeader->SetTitleTextColor(Color::COLOR_WHITE);

	__pGallery->UpdateGallery();

	pHeader = null;
}

result ImageViewer::OnInitializing() {
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	result r = E_SUCCESS;

	Footer* pFooter = GetFooter();

	Bitmap* pBitmapButtonPressed;
	Bitmap* pBitmapButtonNormal;
	Bitmap* pBitmapHeaderBg;

	pBitmapButtonNormal = pAppResource->GetBitmapN(L"btn1_hor.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapButtonPressed = pAppResource->GetBitmapN(L"btn2.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapHeaderBg = pAppResource->GetBitmapN(L"backgh.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);

	Header* pHeader = GetHeader();
	pHeader->SetBackgroundBitmap(pBitmapHeaderBg);

	pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem post;
	post.Construct(K_FB_POST);
	post.SetText(L"Get Text");

	post.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, pBitmapButtonNormal);
	post.SetBackgroundBitmap(FOOTER_ITEM_STATUS_PRESSED, pBitmapButtonPressed);
	pFooter->AddItem(post);

	FooterItem edit;
	edit.Construct(K_EDIT_IMAGE);
	edit.SetText(L"Edit");

	edit.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, pBitmapButtonNormal);
	edit.SetBackgroundBitmap(FOOTER_ITEM_STATUS_PRESSED, pBitmapButtonPressed);

	pFooter->AddItem(edit);

	pFooter->SetBackButton();
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_NORMAL, Color(82, 37, 98));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_PRESSED, Color(160, 80, 8));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_HIGHLIGHTED, Color(160, 80, 8));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_DISABLED, Color(82, 37, 98));

	SetFormBackEventListener(this);
	AddOrientationEventListener(*this);

	pFooter->AddActionEventListener(*this);

	delete pBitmapButtonPressed;
	delete pBitmapButtonNormal;
	delete pBitmapHeaderBg;

	pHeader = null;
	pFooter = null;
	pBitmapButtonPressed = null;
	pBitmapButtonNormal = null;
	pBitmapHeaderBg = null;

	return r;
}

result ImageViewer::OnTerminating() {
	result r = E_SUCCESS;

	return r;
}

void ImageViewer::Upload(String hostAddr, String uri, String* pProxyAddr,
		HttpMultipartEntity* pMultipartEntity) {

	AppLog("--- Uploading ---");
}

result ImageViewer::PrepareImgForUpload(String path) {

	result r = E_SUCCESS;

	Image* pImageDecoder = new Image();

	r = pImageDecoder->Construct();
	if (IsFailed(r)) {
		AppLog("Failed to construct Image");
		return r;
	}

	//String uploadImgPath(L"/Home/upload.jpg");

	Bitmap *pUploadImage = pImageDecoder->DecodeN(path,
			BITMAP_PIXEL_FORMAT_RGB565, Editor::K_IMG_WIDTH,
			Editor::K_IMG_HEIGHT);

	r = pImageDecoder->EncodeToFile(*pUploadImage, IMG_FORMAT_JPG, path, true);

	if (IsFailed(r)) {
		AppLog("Failed to encode to upload file");
		return r;
	}

	return r;
}

void ImageViewer::GetTextFromServer(String path) {

	AppLog("--- GetTextFromServer ---");

	String* pProxyAddr = null;
	String hostAddr = L"http://macetini.mooo.com";
	String uri = L"http://macetini.mooo.com/upload/text/index.php";

	pMultipartEntity = null;
	pHttpTransaction = null;
	HttpRequest* pHttpRequest = null;

	__pHttpSession = new HttpSession();

	// Create HTTP Session
	__pHttpSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, pProxyAddr,
			hostAddr, null);

	pHttpTransaction = __pHttpSession->OpenTransactionN();

	pHttpTransaction->AddHttpTransactionListener(*this);
	pHttpTransaction->SetHttpProgressListener(*this);

	pHttpRequest = pHttpTransaction->GetRequest();
	pHttpRequest->SetMethod(NET_HTTP_METHOD_POST);
	pHttpRequest->SetUri(uri);

	PrepareImgForUpload(path);

	pMultipartEntity = new HttpMultipartEntity();
	pMultipartEntity->Construct();
	pMultipartEntity->AddFilePart(L"photoFile", path);
	pHttpRequest->SetEntity(*pMultipartEntity);

	pHttpTransaction->SetUserObject(pMultipartEntity);

	pHttpTransaction->Submit();
}

void ImageViewer::GetSoundFromServer(String body) {

	AppLog("--- GetSoundFromServer ---");

	if (body == null || body.GetLength() == 0) {
		AppLog("--- GetSoundFromServer: No text! ---");
		return;
	}

	//String hostAddr = L"http://macetini.mooo.com";
	String uri = L"http://macetini.mooo.com/upload/sound/index.php";
	//String* pProxyAddr = null;

	pMultipartEntity = null;
	pHttpTransaction = null;
	HttpRequest* pHttpRequest = null;

	//__pHttpSession = new HttpSession();

	// Create HTTP Session
	//__pHttpSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, pProxyAddr, hostAddr, null);

	pHttpTransaction = __pHttpSession->OpenTransactionN();

	pHttpTransaction->AddHttpTransactionListener(*this);
	pHttpTransaction->SetHttpProgressListener(*this);

	pHttpRequest = pHttpTransaction->GetRequest();
	pHttpRequest->SetMethod(NET_HTTP_METHOD_POST);
	pHttpRequest->SetUri(uri);

	AppLog("String to upload: %ls", body.GetPointer());

	pMultipartEntity = null;
	pMultipartEntity = new HttpMultipartEntity();
	pMultipartEntity->Construct();

	//Encoding* pEnc = Encoding::GetEncodingN(L"UTF8");

	//pMultipartEntity->AddFilePart(L"body", L"/Home/body.txt");//., L"text/plain", L"UTF8", utf8 );
	pMultipartEntity->AddStringPart(L"body", body);//, L"text/plain", L"UTF8", *pEnc);
	pHttpRequest->SetEntity(*pMultipartEntity);

	pHttpTransaction->SetUserObject(pMultipartEntity);

	pHttpTransaction->Submit();
}

String ImageViewer::GetFilenameFromPath(String path) {

	int lastIndexOf;
	path.LastIndexOf('/', path.GetLength() - 1, lastIndexOf);

	AppLog("--- Index of / - %d ---", lastIndexOf);

	String filenameExt;

	path.SubString(lastIndexOf + 1, filenameExt);

	AppLog("---filenameExt %ls ---", filenameExt.GetPointer());

	int indexOf;
	filenameExt.IndexOf('.', 0, indexOf);

	String filename;
	filenameExt.SubString(0, indexOf, filename);

	AppLog("---filename %ls ---", filename.GetPointer());

	return filename;
}

void AudioAppControlPlaySample(void) {

	ArrayList* pDataList = null;
	pDataList = new ArrayList();
	pDataList->Construct();
	pDataList->Add(*new String(L"type:audio"));
	pDataList->Add(*new String(
			L"path:http://macetini.mooo.com/upload/img/img.wav"));
	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_PROVIDER_AUDIO,
			APPCONTROL_OPERATION_PLAY);
	if (pAc) {
		pAc->Start(pDataList, null);
		delete pAc;
	}
	pDataList->RemoveAll(true);
	delete pDataList;
}

result ImageViewer::SaveTxTToMediaOther(String filename) {

	result r = E_SUCCESS;

	filename.Append(".txt");

	ContentId contentId;
	ContentManager contentManager;
	DirEnumerator *pDirEnum = null;

	String sourcePath(L"/Home/");
	sourcePath.Append(filename);

	String destinationPath(L"/Media/Others/");
	destinationPath.Append(filename);

	AppLog("--- Other Save dest: %ls ---", destinationPath.GetPointer());

	OtherContentInfo *otherContentInfo = new OtherContentInfo;
	r = otherContentInfo->Construct(&destinationPath);

	result contRes = contentManager.Construct();

	switch (r) {
	case E_FILE_NOT_FOUND:
		contentId = contentManager.CreateContent(sourcePath, destinationPath,
				true);
		break;
	case E_FILE_ALREADY_EXIST:
		contentId = contentManager.CreateContent(*otherContentInfo);
		contentManager.UpdateContent(*otherContentInfo);
		break;
	}

	delete otherContentInfo;
	delete pDirEnum;

	return E_SUCCESS;
}

result ImageViewer::SaveTxTToLocalHome(String filename, String content) {

	filename.Append(".txt");

	File *pFile;
	String filePath(L"/Home/");

	filePath.Append(filename);

	AppLog("--- filepath %ls ---", filePath.GetPointer());

	pFile = new File();
	pFile->Construct(filePath, L"w");
	pFile->Write(content);
	pFile->Flush();
	delete pFile;

	return E_SUCCESS;
}

result ImageViewer::SaveHTTPResponse(void) {

	AppLog("--- SaveHTTPResponse ---");
	EditArea *pEditArea;
	pEditArea = static_cast<EditArea *> (__pPopup->GetControl(
			L"ID_EDIT_HTTP_RESULT"));

	String filename = GetFilenameFromPath(__currentFilePath);

	SaveTxTToLocalHome(filename, __body);

	SaveTxTToMediaOther(filename);

	return E_SUCCESS;
}

result SaveWavToMediaOther(String filename) {

	result r = E_SUCCESS;

	filename.Append(".wav");

	ContentId contentId;
	ContentManager contentManager;
	DirEnumerator *pDirEnum = null;

	String sourcePath(L"/Home/");
	sourcePath.Append(filename);

	String destinationPath(L"/Media/Sounds/");
	destinationPath.Append(filename);

	AppLog("--- Sound Save dest: %ls ---", destinationPath.GetPointer());

	OtherContentInfo *otherContentInfo = new OtherContentInfo;
	r = otherContentInfo->Construct(&destinationPath);

	result contRes = contentManager.Construct();

	switch (r) {
	case E_FILE_NOT_FOUND:
		contentId = contentManager.CreateContent(sourcePath, destinationPath,
				true);
		break;
	case E_FILE_ALREADY_EXIST:
		contentId = contentManager.CreateContent(*otherContentInfo);
		contentManager.UpdateContent(*otherContentInfo);
		break;
	}

	delete otherContentInfo;
	delete pDirEnum;

	return E_SUCCESS;
}

result SaveWavToLocalHome(String filename, String content) {

	AppLog("--- SaveWavToLocalHome ---");

	filename.Append(".wav");

	File *pFile;
	String filePath(L"/Home/");

	filePath.Append(filename);

	AppLog("--- filepath %ls ---", filePath.GetPointer());

	pFile = new File();
	pFile->Construct(filePath, L"w");
	pFile->Write(content);
	pFile->Flush();
	delete pFile;

	return E_SUCCESS;
}

result SaveHTTPSoundResponse(String soundBody) {

	AppLog("--- SaveHTTPSoundResponse ---");

	//String filename = GetFilenameFromPath(__currentFilePath);

	SaveWavToLocalHome("img", soundBody);

	SaveWavToMediaOther("img");

	return E_SUCCESS;
}

void ImageViewer::OnActionPerformed(const Control& source, int actionId) {
	switch (actionId) {
	case K_EDIT_IMAGE:
		LaunchEditor();
		break;

	case K_FB_POST: {

		__pPhotoName = new String();

		AppLog("Image file path: %ls", __currentFilePath.GetPointer());

		__flag = false;
		CreateLoadingPopup();
		ShowPopupLabel();
		GetTextFromServer(__currentFilePath);

		delete __pPhotoName;
		__pPhotoName = null;

	}
		break;

	case 201: {
		AppLog("---PopUpSave---");

		result r = SaveHTTPResponse();
	}
		break;

	case 202: {
		AppLog("---HttpResultExit---");

		HidePopup();
	}
		break;
	case 203: {
		AppLog("---HttpAbort---");

		if (pHttpTransaction != null)
			delete pHttpTransaction;

		if (pMultipartEntity != null)
			delete pMultipartEntity;

		if (__pHttpSession != null)
			delete __pHttpSession;

		HidePopup();
		__flag = false;
	}
		break;
	case 204: {

		AppLog("--- 204 ---");

		EditArea *pEditArea;
		pEditArea = static_cast<EditArea *> (__pPopup->GetControl(
				L"ID_EDIT_HTTP_RESULT"));

		__body = pEditArea->GetText();

		GetSoundFromServer(__body);
	}
		break;
	case 205: {
		AppLog("--- 205 ---");

		AudioAppControlPlaySample();
	}
		break;
	}
}

void ImageViewer::OnTransactionHeaderCompleted(
		Osp::Net::Http::HttpSession& httpSession,
		Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen,
		bool bAuthRequired) {

	AppLog("---HeaderCompleted---");
}

String DecodeBase64toUTF8(String body64) {

	String retVal;

	ByteBuffer* pDecodedBuffer = StringUtil::DecodeBase64StringN(body64);

	int limit = pDecodedBuffer->GetLimit();
	pDecodedBuffer->Flip(POSITION_TO_ZERO);
	pDecodedBuffer->SetLimit(limit);

	Utf8Decoder utf8;
	McharBuffer* pChars = utf8.GetCharsN(*pDecodedBuffer, true);

	StringUtil::MbToString(*pChars, retVal);

	AppLog("--- DecodeBase64toUTF8: %ls ---", retVal.GetPointer());

	delete pChars;

	return retVal;
}

String GetResponseBodyAsString(ByteBuffer *pBody) {

	//result r = E_SUCCESS;

	//String body
	String retVal = null;

	int limit = pBody->GetLimit();
	pBody->Flip(POSITION_TO_ZERO);
	pBody->SetLimit(limit);
	Utf8Decoder utf8;
	McharBuffer* pChars = utf8.GetCharsN(*pBody, true);

	if (pChars == null) {
		AppLog("Error while encoding HTTP response to McharBuffer.");
		goto fault;
	}

	StringUtil::MbToString(*pChars, retVal);

	fault: delete pChars;

	return retVal;
}

String getHTTPContent(String body) {

	AppLog("--- getHTTPContent ---");

	String retVal = null;

	if (body.GetLength() > 7)
		body.SubString(7, retVal);
	else
		AppLog("HTTPContent string error, curr length %d < 7 !", body.GetLength());

	return retVal;
}

String getHTTPType(String body) {
	AppLog("--- getHTTPType ---");

	String retVal;

	if (body.GetLength() >= 7)
		body.SubString(0, 7, retVal);
	else
		AppLog("HTTPType string error, curr length %d < 7 !", body.GetLength());

	return retVal;
}

void ImageViewer::OnTransactionReadyToRead(HttpSession& httpSession,
		HttpTransaction& httpTransaction, int recommendedChunkSize) {

	AppLog("---Ready---");

	Progress *pProgress;
	pProgress = static_cast<Progress *> (__pPopup->GetControl(
			L"ID_PROGRESS_PBAR"));

	if (pProgress != null)
		HidePopup();

	HttpResponse* pHttpResponse = null;

	// Get Response
	pHttpResponse = httpTransaction.GetResponse();

	// Check the Status Code
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK) {

		String body = GetResponseBodyAsString(pHttpResponse->ReadBodyN());

		AppLog("--- ResponseBodyAsString: %ls ---", body.GetPointer());

		String HTTPType = getHTTPType(body);

		AppLog("--- HTTPType: %ls ---", HTTPType.GetPointer());

		String HTTPBody = getHTTPContent(body);

		AppLog("--- HTTPBody: %ls ---", HTTPBody.GetPointer());

		if (HTTPType.Equals(L"_TEXT__", true) && HTTPBody != null) {

			AppLog("--- %TEXT_% ---");

			__flag = true;

			__body = DecodeBase64toUTF8(HTTPBody);

			CreateHttpResultPopup(__body);
			ShowPopupLabel();

			__pPopup->SetTitleText("Image Text");
		}

		if (HTTPType.Equals(L"_SOUND_", true)) {

			AppLog("--- SOUND ---");

			AppLog("---Decoded HTTP result sound %ls ---", __body.GetPointer());

			__pPopup->SetTitleText("Image Sound");

		}
	}
}

void ImageViewer::OnTransactionReadyToWrite(HttpSession& httpSession,
		HttpTransaction& httpTransaction, int recommendedChunkSize) {
}

void ImageViewer::OnTransactionCompleted(HttpSession& httpSession,
		HttpTransaction& httpTransaction) {

	AppLog("---Completed---");
	HttpMultipartEntity* pMultipartEntity =
			static_cast<HttpMultipartEntity*> (httpTransaction.GetUserObject());

	if (pMultipartEntity)
		delete pMultipartEntity;

	delete &httpTransaction;

	__pPopup->SetTitleText(L"Finished");
}

void ImageViewer::OnTransactionAborted(HttpSession& httpSession,
		HttpTransaction& httpTransaction, result r) {
	AppLog("---Aborted result(%s)---", GetErrorMessage(r));

	HttpMultipartEntity* pMultipartEntity =
			static_cast<HttpMultipartEntity*> (httpTransaction.GetUserObject());

	if (pMultipartEntity)
		delete pMultipartEntity;

	delete &httpTransaction;
}

void ImageViewer::OnHttpUploadInProgress(
		Osp::Net::Http::HttpSession& httpSession,
		Osp::Net::Http::HttpTransaction& httpTransaction,
		long long currentLength, long long totalLength) {

	AppLog("--- OnHttpUploadInProgress ----");

	if (__pPopup == null)
		return;

	int progressVal;
	progressVal = static_cast<float> (currentLength)
			/ static_cast<float> (totalLength) * 100;

	Progress *pProgress;
	pProgress = static_cast<Progress *> (__pPopup->GetControl(
			L"ID_PROGRESS_PBAR"));

	if (pProgress != null)
		pProgress->SetValue(progressVal);

	String progreesText(null);

	if (progressVal < 99) {
		progreesText.Append("Uploading: ");

		progreesText.Append(Integer::ToString(progressVal));
		progreesText.Append(" %");
	} else if (progressVal >= 99)
		progreesText.Append("Please wait... ");

	__pPopup->SetTitleText(progreesText);
	__pPopup->Show();
	__pPopup->RequestRedraw();

	AppLog("---Upload Current Bytes: %lld, Total Bytes: %lld, progress: %d---", currentLength, totalLength, progressVal);
}

void ImageViewer::OnHttpDownloadInProgress(
		Osp::Net::Http::HttpSession& httpSession,
		Osp::Net::Http::HttpTransaction& httpTransaction,
		long long currentLength, long long totalLength) {

	if (__pPopup == null)
		return;

	unsigned short progressVal;
	progressVal = static_cast<float> (currentLength)
			/ static_cast<float> (totalLength) * 100;

	String progreesText("Downloading: ");

	progreesText.Append(Integer::ToString(progressVal));
	progreesText.Append(" %");

	Progress *pProgress;
	pProgress = static_cast<Progress *> (__pPopup->GetControl(
			L"ID_PROGRESS_PBAR"));

	__pPopup->SetTitleText(progreesText);
	__pPopup->Show();
	__pPopup->RequestRedraw();

	AppLog("---Downloaded Current Bytes: %lld, Total Bytes: %lld---", currentLength, totalLength);
}

void ImageViewer::OnFormBackRequested(Form& source) {
	GotoImageGallery();
}

void ImageViewer::OnCameraHandlerComplete(String imagePath) {
	SetImagePath(imagePath);
	if (FormManager::GetInstance()->GetFormById(K_SCREEN_ID)) {
		FormManager::GetInstance()->Jump(ImageViewer::K_SCREEN_ID);
	}
}

void ImageViewer::GotoImageGallery() {
	ImageGallery* pGalleryForm =
			(ImageGallery*) FormManager::GetInstance()->GetFormById(
					ImageGallery::K_SCREEN_ID);

	if (pGalleryForm == NULL) {
		pGalleryForm = new ImageGallery();
		pGalleryForm->Initialize();

		FormManager::GetInstance()->Register(ImageGallery::K_SCREEN_ID,
				pGalleryForm);
	}

	FormManager::GetInstance()->Jump(ImageGallery::K_SCREEN_ID);
}

void ImageViewer::LaunchEditor() {

	Editor* pEditorForm = (Editor*) FormManager::GetInstance()->GetFormById(
			Editor::K_SCREEN_ID);

	if (pEditorForm == NULL) {
		pEditorForm = new Editor();
		pEditorForm->Initialize();

		FormManager::GetInstance()->Register(Editor::K_SCREEN_ID, pEditorForm);
	}

	pEditorForm->SetImagePath(__currentFilePath);

	FormManager::GetInstance()->Jump(Editor::K_SCREEN_ID);

}

GalleryItem* ImageViewer::CreateItem(int index) {
	Image* pImg;
	pImg = new Image();
	pImg->Construct();

	Bitmap* pImgBmp = pImg->DecodeN(__currentFilePath,
			BITMAP_PIXEL_FORMAT_ARGB8888, K_IMG_WIDTH, K_IMG_HEIGHT);
	GalleryItem* pItem;
	pItem = new GalleryItem();
	pItem->Construct(*pImgBmp, GALLERY_IMAGE_ROTATION_0);

	delete pImgBmp;
	delete pImg;

	return pItem;

}

bool ImageViewer::DeleteItem(int index, GalleryItem* pItem) {
	delete pItem;

	return true;
}

int ImageViewer::GetItemCount() {
	return 1;
}

void ImageViewer::OnUserEventReceivedN(RequestId requestId,
		Osp::Base::Collection::IList* pArgs) {
	switch (requestId) {

	case 101: {
		ShowPopupLabel();
	}
		break;
	}

}

void ImageViewer::ShowPopupLabel() {

	__pPopup->Show();
	__pPopup->RequestRedraw();

}

void ImageViewer::HidePopup() {
	__pPopup->SetShowState(false);
	__pPopup->RemoveAllControls();

	delete __pPopup;
	__pPopup = null;
}

void ImageViewer::ShowMsg(String msg) {
	MessageBox messageBox;
	messageBox.Construct(L"", msg, MSGBOX_STYLE_NONE, 2000);

	int modalResult = 0;
	messageBox.ShowAndWait(modalResult);
}

void ImageViewer::OnOrientationChanged(const Osp::Ui::Control &source,
		Osp::Ui::OrientationStatus orientationStatus) {
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	Footer* pFooter = GetFooter();
	Bitmap* pBitmapFooterBg;

	if (GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT
			|| GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT_REVERSE) {
		pBitmapFooterBg = pAppResource->GetBitmapN(L"backgp.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);

	} else {
		pBitmapFooterBg = pAppResource->GetBitmapN(L"backg.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);
	}

	pFooter->SetBackgroundBitmap(pBitmapFooterBg);

	Rectangle rectangle = GetClientAreaBounds();
	rectangle.x = 0;
	rectangle.y = 0;

	__pGallery->SetBounds(rectangle);

	delete pBitmapFooterBg;

	pBitmapFooterBg = null;

	if (__pPopup != null) {
		if (__flag == false)
			ResizeLodaingPopup();
		else
			ResizeDescBox();
	}

	Draw();
}

void ImageViewer::ResizeDescBox() {

	AppLog("--- ResizeDescBox ---");

	int x, y, w, h;
	ImageViewer::GetBounds(x, y, w, h);

	__pPopup->SetSize(w, h);

	EditArea* pEditArea = static_cast<EditArea*> (__pPopup->GetControl(
			L"ID_EDIT_HTTP_RESULT"));
	Button* pButtonSave = static_cast<Button*> (__pPopup->GetControl(
			L"ID_POPUP_SAVE"));
	Button* pButtonExit = static_cast<Button*> (__pPopup->GetControl(
			L"ID_POPUP_EXIT"));
	Button* pButtonGetSound = static_cast<Button*> (__pPopup->GetControl(
			L"ID_GET_SOUND"));
	Button* pButtonPlaySound = static_cast<Button*> (__pPopup->GetControl(
			L"ID_PLAY_SOUND"));

	pEditArea->SetSize(w, h - 170);

	pButtonSave->SetPosition(w - 110, h - 155);
	pButtonExit->SetPosition(w - 205, h - 155);
	pButtonGetSound->SetPosition(w - 330, h - 155);
	pButtonPlaySound->SetPosition(w - 460, h - 155);

	ImageViewer* pImageViewer =
			(ImageViewer *) FormManager::GetInstance()->GetFormById(
					ImageViewer::K_SCREEN_ID);

	if (pImageViewer->GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT
			|| pImageViewer->GetOrientationStatus()
					== ORIENTATION_STATUS_PORTRAIT_REVERSE) {

		AppLog("--- Portrait ---");

		//In case I need it

	} else {

		AppLog("--- Landscape ---");
		//In case I need it
	}

	__pPopup->Show();

}

void ImageViewer::ResizeLodaingPopup() {

	AppLog("--- ResizeLoadingPopUp ---");

	int x, y, w, h;
	ImageViewer::GetBounds(x, y, w, h);

	Progress *pProgress = static_cast<Progress*> (__pPopup->GetControl(
			L"ID_PROGRESS_PBAR"));
	Button *pButton = static_cast<Button*> (__pPopup->GetControl(
			L"ID_TRANSACTION_ABORT"));

	__pPopup->SetSize(w, 220);
	pProgress->SetPosition(0, 30);
	pProgress->SetSize(w, 20);
	pButton->SetPosition(w / 2 - 55, pProgress->GetY() + 28);

	ImageViewer* pImageViewer =
			(ImageViewer *) FormManager::GetInstance()->GetFormById(
					ImageViewer::K_SCREEN_ID);

	if (pImageViewer->GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT
			|| pImageViewer->GetOrientationStatus()
					== ORIENTATION_STATUS_PORTRAIT_REVERSE) {

		AppLog("--- Portrait ---");
		//In case I need it

	} else {

		AppLog("--- Landscape ---");
		//In case I need it
	}

	__pPopup->Show();
}

void ImageViewer::OnTimerExpired(Osp::Base::Runtime::Timer &timer) {
	HidePopup();
}

void ImageViewer::CreateHttpResultPopup(Osp::Base::String Text) {

	AppLog("--- CreateHttpResultPopup ---");

	int x, y, w, h;
	ImageViewer::GetBounds(x, y, w, h);
	Rectangle r = GetClientAreaBounds();
	Dimension dimen;

	dimen.SetSize(w, h);

	__pPopup = new Popup();
	__pPopup->Construct(true, dimen);

	__pPopup->SetTitleText("Image Text");

	EditArea* pEditArea = new EditArea();
	pEditArea->Construct(Rectangle(0, 0, w, h - 170));
	//pEditArea->AddTextEventListener(*this);

	pEditArea->SetName(L"ID_EDIT_HTTP_RESULT");
	pEditArea->SetText(Text);

	// Adds an EditArea to the Form

	Point point;
	point.SetPosition(x, y);
	__pPopup->TranslateToClientAreaPosition(point);

	Button* pButtonExit = new Button();
	pButtonExit->Construct(Rectangle(w - 110, h - 155, 90, 60));
	pButtonExit->SetText(L"Exit");
	pButtonExit->SetTextSize(25);
	pButtonExit->SetName(L"ID_POPUP_EXIT");
	pButtonExit->SetActionId(202);
	pButtonExit->AddActionEventListener(*this);

	Button* pButtonSave = new Button();
	pButtonSave->Construct(Rectangle(w - 205, h - 155, 90, 60));
	pButtonSave->SetText(L"Save");
	pButtonSave->SetTextSize(25);
	pButtonSave->SetName(L"ID_POPUP_SAVE");
	pButtonSave->SetActionId(201);
	pButtonSave->AddActionEventListener(*this);

	Button* pButtonGetSound = new Button();
	pButtonGetSound->Construct(Rectangle(w - 330, h - 155, 120, 60));
	pButtonGetSound->SetText(L"Sound");
	pButtonGetSound->SetTextSize(25);
	pButtonGetSound->SetName(L"ID_GET_SOUND");
	pButtonGetSound->SetActionId(204);
	pButtonGetSound->AddActionEventListener(*this);

	Button* pButtonPlaySound = new Button();
	pButtonPlaySound->Construct(Rectangle(w - 460, h - 155, 125, 60));
	pButtonPlaySound->SetText(L"Play");
	pButtonPlaySound->SetTextSize(25);
	pButtonPlaySound->SetName(L"ID_PLAY_SOUND");
	pButtonPlaySound->SetActionId(205);
	pButtonPlaySound->AddActionEventListener(*this);

	__pPopup->AddControl(*pButtonSave);
	__pPopup->AddControl(*pButtonExit);
	__pPopup->AddControl(*pEditArea);
	__pPopup->AddControl(*pButtonGetSound);
	__pPopup->AddControl(*pButtonPlaySound);
}

void ImageViewer::CreateLoadingPopup() {
	int x, y, w, h;
	ImageViewer::GetBounds(x, y, w, h);
	Rectangle r = GetClientAreaBounds();
	Dimension dimen;

	dimen.SetSize(w, 220);

	__pPopup = new Popup();
	__pPopup->Construct(true, dimen);
	__pPopup->SetTitleText("Uploading: 0 %");

	Point point;
	point.SetPosition(x, h / 2);
	__pPopup->TranslateToClientAreaPosition(point);

	Progress *pProgress = new Progress;
	pProgress->Construct(Rectangle(0, 30, w, 20), 1, 100);
	pProgress->SetName(L"ID_PROGRESS_PBAR");

	Button* pButtonAbort = new Button();
	pButtonAbort->Construct(Rectangle(w / 2 - 55, pProgress->GetY() + 28, 100,
			60));
	pButtonAbort->SetText(L"Abort");
	pButtonAbort->SetTextSize(40);
	pButtonAbort->SetName(L"ID_TRANSACTION_ABORT");
	pButtonAbort->SetActionId(203);
	pButtonAbort->AddActionEventListener(*this);

	__pPopup->AddControl(*pProgress);
	__pPopup->AddControl(*pButtonAbort);
}

