#ifndef ICAMERAHANDLERCALLBACK_H_
#define ICAMERAHANDLERCALLBACK_H_

class ICameraHandlerCallBack
{
public:
	virtual ~ICameraHandlerCallBack()
	{
	}
	;

	virtual void OnCameraHandlerComplete(Osp::Base::String imagePath) = 0;
};

#endif /* ICAMERAHANDLERCALLBACK_H_ */
