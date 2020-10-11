#ifndef _Main_H_
#define _Main_H_

#include <FBase.h>
#include <FUi.h>
#include <FContent.h>
#include <FApp.h>
#include "CameraCaptureHandler.h"

class Main:
		public Osp::Ui::Controls::Form,
		public Osp::Ui::IActionEventListener
{

public:
	Main(void);
	virtual ~Main(void);
	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	result OnDraw(void);

	static Osp::Base::String K_SCREEN_ID;

protected:
	static const int ID_BTN_GALLERY = 101;
	static const int ID_BTN_CAPTURE = 102;

	Osp::Ui::Controls::Button* __pButtonGallery;
	Osp::Ui::Controls::Button* __pButtonCapture;

private:
	void LoadGallery(void);
	void LaunchCamera(void);

	CameraCaptureHandler* __pCamHandler;

};

#endif	//_Main_H_
