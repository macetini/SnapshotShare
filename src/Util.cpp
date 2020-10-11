#include "Util.h"

#define K_DEBUG 0

using namespace Osp::Base;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Media;
//used for debugging
void Util::Alert_Box(String title, String msg)
{

	if (K_DEBUG == 1)
	{
		ShowMessage(0, title, msg, NULL);
	}
}

//Displays a popup message with OK button
void Util::ShowMessage(int messageId, String title, String msg, IMessageBoxListener* pListener, MessageBoxStyle mode)
{
	MessageBox messageBox;
	messageBox.Construct(title, msg, mode, 0);

	int modalResult = 0;
	messageBox.ShowAndWait(modalResult);

	if (pListener)
	{
		pListener->OnMessageBoxResult(messageId, modalResult);
	}
}

//Makes a copy of the source bitmap without scaling
Bitmap* Util::CopyBitmapN(Bitmap& sourceBmp)
{
	BufferInfo bufInfo;
	ByteBuffer srcBuf;
	Bitmap* pDstBmp = null;
	result r = E_SUCCESS;
	byte* pPixels;
	int bytesPerLine;

	sourceBmp.Lock(bufInfo);
	srcBuf.Construct(bufInfo.height * bufInfo.width * bufInfo.bitsPerPixel / 8);
	pPixels = (byte*) bufInfo.pPixels;
	bytesPerLine = bufInfo.width * bufInfo.bitsPerPixel / 8;
	// copy pixel data into buf
	if (bytesPerLine != bufInfo.pitch)
	{
		for (int i = 0; i < bufInfo.height; i++)
		{
			srcBuf.SetArray(pPixels, 0, bytesPerLine);
			pPixels += bufInfo.pitch;
		}
	}
	else
	{
		srcBuf.SetArray(pPixels, 0, bufInfo.pitch * bufInfo.height);
	}
	sourceBmp.Unlock();
	srcBuf.Flip();

	pDstBmp = new Bitmap();
	pDstBmp->Construct(srcBuf, Dimension(bufInfo.width, bufInfo.height), BITMAP_PIXEL_FORMAT_ARGB8888,
			BUFFER_SCALING_NONE);

	SetLastResult(r);

	return pDstBmp;

}
