#ifndef UTIL_H_
#define UTIL_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>
#include <FMedia.h>
#include "IMessageBoxListener.h"

class Util
{
public:
	static void ShowMessage(int message, Osp::Base::String title, Osp::Base::String msg,
			IMessageBoxListener* pListener, Osp::Ui::Controls::MessageBoxStyle mode =
					Osp::Ui::Controls::MSGBOX_STYLE_OK);
	static void Alert_Box(Osp::Base::String title, Osp::Base::String msg);
	static Osp::Graphics::Bitmap* CopyBitmapN(Osp::Graphics::Bitmap& sourceBmp);
};

#endif /* UTIL_H_ */
