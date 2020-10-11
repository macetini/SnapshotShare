#ifndef IMESSAGEBOXLISTENER_H_
#define IMESSAGEBOXLISTENER_H_

class IMessageBoxListener
{
public:
	virtual ~IMessageBoxListener()
	{
	}
	;

	virtual void OnMessageBoxResult(int messageId, int result) = 0;
};

#endif /* IMESSAGEBOXLISTENER_H_ */
