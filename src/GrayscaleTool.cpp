#include "GrayscaleTool.h"
#include "FormManager.h"
#include "Editor.h"

using namespace Osp::Graphics;

GrayscaleTool::GrayscaleTool() :
	__pEditBmp(NULL)
{
}

GrayscaleTool::~GrayscaleTool()
{
}

void GrayscaleTool::Launch(Bitmap* pEditbmp)
{
	__pEditBmp = Util::CopyBitmapN(*pEditbmp);
	Process();
}

void GrayscaleTool::Process()
{
	if (__pEditBmp == NULL)
	{
		return;
	}

	int width = 0;
	int height = 0;
	int bitsPerPixel = 0;
	int bytesPerPixel = 0;
	BufferInfo info;

	__pEditBmp->Lock(info);

	width = info.width;
	height = info.height;
	bitsPerPixel = info.bitsPerPixel;

	bytesPerPixel = bitsPerPixel / 8;

	byte gray;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int* color = ((int *) ((byte *) info.pPixels + y * info.pitch + x * bytesPerPixel));
			byte* blue = (byte*) color;
			byte* green = blue + 1;
			byte* red = green + 1;

			gray = ((*blue) * 0.11) + ((*green) * 0.59) + ((*red) * 0.3);
			*red = gray;
			*green = gray;
			*blue = gray;

		}
	}
	__pEditBmp->Unlock();
	Exit();
}
void GrayscaleTool::Exit()
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->SetCurrentBitmap(__pEditBmp);
	pEditor->SendUserEvent(K_TOOL_ID, null);

	pEditor = null;

}
