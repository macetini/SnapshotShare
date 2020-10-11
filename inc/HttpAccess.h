/*
 * HttpAccess.h
 */

#ifndef HTTPACCESS_H_
#define HTTPACCESS_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FNet.h>

#include "IHttpListener.h"

class HttpAccess: public Osp::Net::Http::IHttpTransactionEventListener,
		public Osp::Net::Http::IHttpProgressEventListener

{
public:
	Osp::Net::Http::HttpSession* __pHttpSession;
	HttpAccess();
	~HttpAccess();
	void Upload(Osp::Base::String filepath);
private:
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
};

#endif /* HTTPACCESS_H_ */
