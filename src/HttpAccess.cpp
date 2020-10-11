#include "HttpAccess.h"
#include "ImageViewer.h"
#include "FormManager.h"
#include "Util.h"

#include <FMedia.h>
#include <FGraphics.h>
#include <FIo.h>
#include <FUi.h>
#include <FBase.h>
#include <FText.h>

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

HttpAccess::HttpAccess(){
}

HttpAccess::~HttpAccess() {

}

void HttpAccess::Upload(Osp::Base::String filepath) {

	String* pProxyAddr = null;
	String hostAddr = L"http://127.0.0.1:8887";
	String uri = L"http://127.0.0.1:8887/upload/index.php";

	HttpMultipartEntity* pMultipartEntity = null;
	HttpTransaction* pHttpTransaction = null;
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

	pMultipartEntity = new HttpMultipartEntity();
	pMultipartEntity->Construct();
	pMultipartEntity->AddFilePart(L"photoFile", filepath);
	pHttpRequest->SetEntity(*pMultipartEntity);

	pHttpTransaction->SetUserObject(pMultipartEntity);

	//AppLog("%ls", filepath.GetPointer());

	pHttpTransaction->Submit();

}

void HttpAccess::OnHttpDownloadInProgress(HttpSession &httpSession,
		HttpTransaction &httpTransaction, long long currentLength,
		long long totalLength) {
	AppLog("---Download Current Bytes: %lld, Total Bytes: %lld---", currentLength, totalLength);

}
void HttpAccess::OnHttpUploadInProgress(HttpSession &httpSession,
		HttpTransaction &httpTransaction, long long currentLength,
		long long totalLength) {
	AppLog("---Upload Current Bytes: %lld, Total Bytes: %lld---", currentLength, totalLength);

}

void HttpAccess::OnTransactionReadyToRead(HttpSession& httpSession,
		HttpTransaction& httpTransaction, int recommendedChunkSize){

	AppLog("--- Ready ---");
	/*ByteBuffer *pBody = null;
	HttpResponse* pHttpResponse = null;
	HttpHeader* pHttpHeader = null;

	// Get Response
	pHttpResponse = httpTransaction.GetResponse();

	// Check the Status Code
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK) {
		// Get Http Header
		pHttpHeader = pHttpResponse->GetHeader();

		// Read the body from the HttpResponse
		pBody = pHttpResponse->ReadBodyN();

		int limit = pBody->GetLimit();
		pBody->Flip(POSITION_TO_ZERO);
		pBody->SetLimit(limit);
		Utf8Decoder utf8;
		McharBuffer* pChars = utf8.GetCharsN(*pBody, true);

		String body64;
		StringUtil::MbToString(*pChars, body64);

		AppLog("%ls", body64.GetPointer());

		ByteBuffer* pDecodedBuffer = StringUtil::DecodeBase64StringN(body64);

		String body;

		limit = pDecodedBuffer->GetLimit();
		pDecodedBuffer->Flip(POSITION_TO_ZERO);
		pDecodedBuffer->SetLimit(limit);
		pChars = utf8.GetCharsN(*pDecodedBuffer, true);

		StringUtil::MbToString(*pChars, body);

		AppLog("%ls", body.GetPointer());
	}*/
}

void HttpAccess::OnTransactionAborted(HttpSession& httpSession,
		HttpTransaction& httpTransaction, result r) {

	AppLog("---Aborted result(%s)---", GetErrorMessage(r));
		HttpMultipartEntity* pMultipartEntity =
				static_cast<HttpMultipartEntity*> (httpTransaction.GetUserObject());

		if (pMultipartEntity)
			delete pMultipartEntity;

		delete &httpTransaction;
}

void HttpAccess::OnTransactionReadyToWrite(HttpSession& httpSession,
		HttpTransaction& httpTransaction, int recommendedChunkSize) {

}

void HttpAccess::OnTransactionHeaderCompleted(HttpSession& httpSession,
		HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired) {

	AppLog("---Completed---");
		HttpMultipartEntity* pMultipartEntity =
				static_cast<HttpMultipartEntity*> (httpTransaction.GetUserObject());

		if (pMultipartEntity)
			delete pMultipartEntity;

		delete &httpTransaction;
}

void HttpAccess::OnTransactionCompleted(HttpSession& httpSession,
		HttpTransaction& httpTransaction) {
}

void HttpAccess::OnTransactionCertVerificationRequiredN(
		HttpSession& httpSession, HttpTransaction& httpTransaction,
		String* pCert) {

}
