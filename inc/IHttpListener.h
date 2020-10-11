#ifndef IHTTPLISTENER_H_
#define IHTTPLISTENER_H_

class HttpAccess;
class IHttpListener
{
public:
	virtual ~IHttpListener()
	{
	}
	virtual void OnHttpTransactionProgress(long long currentLength, long long totalLength) =0;
	virtual void OnHttpTransactionComplete(HttpAccess& source) = 0;
	virtual void OnHttpTransactionError(HttpAccess& source) = 0;
	virtual void OnHttpTransactionAborted(HttpAccess& source) = 0;
};
#endif /* IHTTPLISTENER_H_ */
