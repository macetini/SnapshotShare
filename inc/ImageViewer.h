#ifndef IMAGEVIEWER_H_
#define IMAGEVIEWER_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FNet.h>

#include "ICameraHandlerCallBack.h"
#include "IHttpListener.h"
#include "IMessageBoxListener.h"

class ImageViewer: public Osp::Ui::Controls::Form,
		public ICameraHandlerCallBack,
		public Osp::Ui::IActionEventListener,
		public Osp::Ui::Controls::IFormBackEventListener,
		public Osp::Ui::Controls::IGalleryItemProvider,
		public Osp::Ui::IOrientationEventListener,
		public Osp::Base::Runtime::ITimerEventListener,
		public Osp::Net::Http::IHttpTransactionEventListener,
		public Osp::Net::Http::IHttpProgressEventListener {
public:

	static Osp::Base::String K_SCREEN_ID;
	static const int K_EDIT_IMAGE = 0;
	static const int K_FB_POST = 1;

	void CreateHttpResultPopup(Osp::Base::String);

	ImageViewer();
	virtual ~ImageViewer();

	bool Initialize();
	void SetImagePath(Osp::Base::String filePath);
	void ShowPopupLabel();
	void HidePopup();
	void ShowMsg(Osp::Base::String msg);
	void ResizeLodaingPopup();
	void ResizeDescBox();
	Osp::Base::String GetPhotoName();
	void CreateLoadingPopup();

	void OnCameraHandlerComplete(Osp::Base::String imagePath);

	result OnInitializing(void);
	result OnTerminating(void);

	void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	void OnFormBackRequested(Osp::Ui::Controls::Form &source);

	Osp::Ui::Controls::GalleryItem * CreateItem(int index);
	bool DeleteItem(int index, Osp::Ui::Controls::GalleryItem *pItem);
	int GetItemCount(void);

	void OnUserEventReceivedN(RequestId requestId,
			Osp::Base::Collection::IList* pArgs);
	void OnOrientationChanged(const Osp::Ui::Control &source,
			Osp::Ui::OrientationStatus orientationStatus);

	void OnTimerExpired(Osp::Base::Runtime::Timer &timer);

private:
	static const int K_IMG_WIDTH = 800;
	static const int K_IMG_HEIGHT = 600;

	void DisplayResult(void);

	void LaunchEditor(void);
	void GotoImageGallery(void);

	Osp::Base::String __currentFilePath;

	Osp::Ui::Controls::Gallery* __pGallery;
	Osp::Ui::Controls::Popup* __pPopup;
	Osp::Base::String* __pPhotoName;
	Osp::Base::String __body;
	bool __flag;

public:
	void OnTransactionReadyToRead(Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction,
			int availableBodyLen);
	void OnTransactionAborted(Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction, result r);
	void OnTransactionReadyToWrite(Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction,
			int recommendedChunkSize);
	void OnTransactionHeaderCompleted(Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen,
			bool bAuthRequired);
	void OnTransactionCompleted(Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction);
	void OnTransactionCertVerificationRequiredN(
			Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction,
			Osp::Base::String* pCert);

	void OnHttpUploadInProgress(Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction,
			long long currentLength, long long totalLength);
	void OnHttpDownloadInProgress(Osp::Net::Http::HttpSession& httpSession,
			Osp::Net::Http::HttpTransaction& httpTransaction,
			long long currentLength, long long totalLength);

public:
	Osp::Net::Http::HttpSession *__pHttpSession;
	Osp::Net::Http::HttpTransaction* pHttpTransaction;
	Osp::Net::Http::HttpMultipartEntity* pMultipartEntity;
	//Osp::Media::Player* pPlayer;

	void Upload(Osp::Base::String hostAddr, Osp::Base::String uri, Osp::Base::String* pProxyAddr, Osp::Net::Http::HttpMultipartEntity* pMultipartEntity);
	void GetTextFromServer(Osp::Base::String path);
	void GetSoundFromServer(Osp::Base::String body);
	result SaveHTTPResponse(void);
	result SaveTxTToLocalHome(Osp::Base::String filename, Osp::Base::String content);
	result SaveTxTToMediaOther(Osp::Base::String path);
	Osp::Base::String GetFilenameFromPath(Osp::Base::String path);
	result PrepareImgForUpload(Osp::Base::String path);
};

#endif /* IMAGEVIEWER_H_ */
