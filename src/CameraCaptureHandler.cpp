#include "CameraCaptureHandler.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Media;

CameraCaptureHandler::CameraCaptureHandler() :
	__pCaptureCallBack(NULL)
{

}

CameraCaptureHandler::~CameraCaptureHandler()
{
	__pCaptureCallBack = NULL;
}

result CameraCaptureHandler::Construct(ICameraHandlerCallBack* pCallBack)
{
	__pCaptureCallBack = pCallBack;

	return E_SUCCESS;
}

void CameraCaptureHandler::OnAppControlCompleted(const Osp::Base::String& appControlId,
		const Osp::Base::String& operationId, const Osp::Base::Collection::IList* pResultList)
{
	//This method is invoked when an application control callback event occurs.

	Osp::Base::String* pCaptureResult = null;
	if (appControlId.Equals(APPCONTROL_PROVIDER_CAMERA) && operationId.Equals(APPCONTROL_OPERATION_CAPTURE))
	{
		pCaptureResult = (String*) pResultList->GetAt(0);
		if (pCaptureResult->Equals(String(APPCONTROL_RESULT_SUCCEEDED)))
		{
			String imagePath;
			imagePath.Append(String(*(String*) (pResultList->GetAt(1))));

			__pCaptureCallBack->OnCameraHandlerComplete(imagePath);

		}
		else if (pCaptureResult->Equals(String(APPCONTROL_RESULT_CANCELED)))
		{
			AppLog("Capture Canceled");
		}
		else if (pCaptureResult->Equals(String(APPCONTROL_RESULT_FAILED)))
		{
			AppLog("Capture Failed");
		}

	}

}
