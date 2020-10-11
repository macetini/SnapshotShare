#ifndef CAMERACAPTUREHANDLER_H_
#define CAMERACAPTUREHANDLER_H_

#include <FUi.h>
#include <FApp.h>
#include <FMedia.h>

#include "ICameraHandlerCallBack.h"

class CameraCaptureHandler:
		public Osp::App::IAppControlEventListener
{
public:
	CameraCaptureHandler();
	virtual ~CameraCaptureHandler();

	result Construct(ICameraHandlerCallBack* pCallBack);

private:
	void OnAppControlCompleted(const Osp::Base::String& appControlId, const Osp::Base::String& operationId,
			const Osp::Base::Collection::IList* pResultList);

	ICameraHandlerCallBack* __pCaptureCallBack;
};

#endif /* CAMERACAPTUREHANDLER_H_ */
