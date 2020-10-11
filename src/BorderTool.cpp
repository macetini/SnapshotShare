#include "BorderTool.h"
#include "Editor.h"
#include "Util.h"

using namespace Osp::Graphics;
using namespace Osp::Media;
using namespace Osp::Base;
using namespace Osp::Ui::Controls;
using namespace Osp::Ui;

BorderTool::BorderTool()
{

}

BorderTool::~BorderTool()
{

}

void BorderTool::Launch(Bitmap* pEditbmp)
{
	__pEditBmp = Util::CopyBitmapN(*pEditbmp);

	Process();
}

void BorderTool::SetBorderId(int borderId)
{
	__borderId = borderId;
}

void BorderTool::Process()
{

	int width;
	int height;
	int borderSize;

	BufferInfo b;
	__pEditBmp->Lock(b);

	Dimension size;
	size.width = b.width;
	size.height = b.height;

	borderSize = GetBorderSize(size);

	if (borderSize == 0)
	{
		return;
	}

	Image* pBitmapDecoder = new Image();
	ByteBuffer* pBorderbytes;

	result r = pBitmapDecoder->Construct();
	if (IsFailed(r))
	{
		AppLog("Failed to construct Image");

		return;
	}

	//We use PNG as they allow transparency
	String fileName = L"/Home/border";
	fileName.Append(__borderId);
	fileName.Append("_");
	fileName.Append(borderSize);
	fileName.Append(".png");

	pBorderbytes = pBitmapDecoder->DecodeToBufferN(fileName, BITMAP_PIXEL_FORMAT_ARGB8888, width, height);

	int hTiles = int(size.width / width);
	int vTiles = int(size.height / height);

	Bitmap* pBorder;
	pBorder = new Bitmap();

	pBorder->Construct(*pBorderbytes, Dimension(width, height), BITMAP_PIXEL_FORMAT_ARGB8888, BUFFER_SCALING_NONE);

	Point pt;
	for (int i = 0; i < hTiles; i++)
	{
		for (int j = 0; j < vTiles; j++)
		{
			if ((i == 0 || i == (hTiles - 1)) || (j == 0 || j == (vTiles - 1)))
			{
				pt.x = i * (size.width / hTiles);
				pt.y = j * (size.height / vTiles);
				__pEditBmp->Merge(pt, *pBorder, Rectangle(0, 0, width, height));
			}
		}
	}

	delete pBitmapDecoder;
	delete pBorder;
	delete pBorderbytes;

	pBitmapDecoder = null;
	pBorder = null;
	pBorderbytes = null;

	Exit();

}

void BorderTool::Exit()
{
	Editor* pEditor = (Editor *) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);
	pEditor->SetCurrentBitmap(__pEditBmp);
	pEditor->SendUserEvent(K_TOOL_ID, null);

	pEditor = null;
}

int BorderTool::GetBorderSize(Dimension size)
{
	Editor* pEditorForm = (Editor*) FormManager::GetInstance()->GetFormById(Editor::K_SCREEN_ID);

	int chkWidth, chkHeight, orient;

	if (pEditorForm->GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT || pEditorForm->GetOrientationStatus()
			== ORIENTATION_STATUS_PORTRAIT_REVERSE)
	{
		orient = O_PORTRAIT;
	}
	else
	{
		orient = O_LANDSCAPE;
	}

	switch (orient)
	{
		case O_PORTRAIT:
		{
			if (pEditorForm->GetWidth() == WVGA_PORTRAIT_WIDTH)
			{
				chkWidth = WVGA_PORTRAIT_WIDTH;
				chkHeight = WVGA_PORTRAIT_HEIGHT;
			}
			else
			{
				chkWidth = HVGA_PORTRAIT_WIDTH;
				chkHeight = HVGA_PORTRAIT_HEIGHT;
			}
		}
		break;

		case O_LANDSCAPE:
		{
			if (pEditorForm->GetWidth() == WVGA_LANDSCAPE_WIDTH)
			{
				chkWidth = WVGA_LANDSCAPE_WIDTH;
				chkHeight = WVGA_LANDSCAPE_HEIGHT;
			}
			else
			{
				chkWidth = HVGA_LANDSCAPE_WIDTH;
				chkHeight = HVGA_LANDSCAPE_HEIGHT;
			}
		}
		break;
	}

	int border = 0;

	if (size.width < (chkWidth / 2) || size.height < (chkHeight / 3))
		border = 25;

	if (size.width < (chkWidth / 3) || size.height < (chkHeight / 5))
		border = 20;

	if (size.width < (chkWidth / 5) || size.height < (chkHeight / 8))
		border = 15;

	if (size.width < (chkWidth / 10) || size.height < (chkHeight / 16))
		border = 8;

	if (border == 0)
	{
		border = 32;
	}

	pEditorForm = null;
	return border;
}
