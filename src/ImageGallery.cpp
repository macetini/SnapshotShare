#include <FMedia.h>
#include "SnapshotShare.h"
#include "ImageGallery.h"
#include "Main.h"
#include "FormManager.h"
#include "Editor.h"
#include "Util.h"
#include "ImageViewer.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Media;
using namespace Osp::Graphics;
using namespace Osp::Media;
using namespace Osp::Content;
using namespace Osp::App;

#define K_FOOTER_VIEW 100
#define K_FOOTER_POST 101
#define K_FOOTER_BACK 102

Osp::Base::String ImageGallery::K_SCREEN_ID = L"ImageGallery";

ImageGallery::ImageGallery(void) :
	__pItemList(NULL), __pFilePathList(NULL), __pIconList(NULL)
{
}

ImageGallery::~ImageGallery(void)
{
	if (__pItemList)
	{
		__pItemList->RemoveAll(true);
		delete __pItemList;
		__pItemList = NULL;
	}
}

bool ImageGallery::Initialize()
{
	Form::Construct(L"IDF_IMAGES");

	return true;
}

result ImageGallery::OnInitializing(void)
{
	result r = E_SUCCESS;

	Footer* pFooter = GetFooter();
	pFooter->SetBackButton();
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_NORMAL, Color(82, 37, 98));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_PRESSED, Color(160, 80, 8));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_HIGHLIGHTED, Color(160, 80, 8));
	pFooter->SetButtonColor(BUTTON_ITEM_STATUS_DISABLED, Color(82, 37, 98));
	SetFormBackEventListener(this);

	__pIconList = new IconListView();
	if (!__pIconList)
	{
		AppLog("Image List not found.");

		return r;
	}

	Rectangle rect = GetClientAreaBounds();

	__pIconList->Construct(Rectangle(0, 0, rect.width, rect.height), Dimension(150, 150), ICON_LIST_VIEW_STYLE_NORMAL,
			ICON_LIST_VIEW_SCROLL_DIRECTION_VERTICAL);
	__pIconList->SetItemProvider(*this);
	__pIconList->SetItemSpacing(2, 2);
	__pIconList->SetItemBorderStyle(ICON_LIST_VIEW_ITEM_BORDER_STYLE_SHADOW);
	__pIconList->AddIconListViewItemEventListener(*this);

	AddControl(*__pIconList);

	return r;
}

void ImageGallery::OnIconListViewItemStateChanged(IconListView & listView, int index, IconListViewItemStatus status)
{
	__currentSelectedIndex = index;
	DisplayImage();

}

result ImageGallery::OnDraw()
{
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();

	result r;
	Footer* pFooter = GetFooter();
	Bitmap* pBitmap;
	Bitmap* pIconBitmap;
	Canvas* pCanvas;
	Bitmap* pBitmapFooter;

	if (ImageGallery::GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT || ImageGallery::GetOrientationStatus()
			== ORIENTATION_STATUS_PORTRAIT_REVERSE)
	{
		pBitmap = pAppResource->GetBitmapN(L"BG.png", BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapFooter = pAppResource->GetBitmapN(L"backgp.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	}

	else
	{
		pBitmap = pAppResource->GetBitmapN(L"BGR.png", BITMAP_PIXEL_FORMAT_ARGB8888);
		pBitmapFooter = pAppResource->GetBitmapN(L"backg.png", BITMAP_PIXEL_FORMAT_ARGB8888);
	}
	pFooter->SetBackgroundBitmap(pBitmapFooter);
	if (!pBitmap)
	{
		AppLog("Failed to decode image!");
	}

	pCanvas = this->GetCanvasN();
	pCanvas->DrawBitmap(Point(0, 0), *pBitmap);

	delete pCanvas;
	delete pBitmap;

	pCanvas = null;
	pBitmap = null;

	LoadGallery();

	pIconBitmap = pAppResource->GetBitmapN(L"iconlist.png", BITMAP_PIXEL_FORMAT_ARGB8888);

	Rectangle screenBound = GetClientAreaBounds();
	__pIconList->SetSize(screenBound.width, screenBound.height);
	__pIconList->SetBackgroundBitmap(pIconBitmap);
	r = __pIconList->UpdateList();

	pAppResource = null;

	delete pIconBitmap;
	pIconBitmap = null;

	return r;
}

void ImageGallery::OnFormBackRequested(Form& source)
{
	GoToMainScreen();
}

result ImageGallery::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}

void ImageGallery::LoadGallery(void)
{
	result r;

	String sortColumn = L"";
	String strQuery = L"";
	String ch;

	int totalPage = 0;
	int totalCount = 0;
	int nItemCount = 0;
	int nStartIndex = 0;
	int nIndex = 0;

	ContentSearch contentSearch;
	contentSearch.Construct(CONTENT_TYPE_IMAGE);

	if (__pItemList)
	{
		__pItemList->RemoveAll(true);
		delete __pItemList;
		__pItemList = NULL;
	}
	__pItemList = new ArrayList();

	if (__pFilePathList)
	{
		__pFilePathList->RemoveAll(true);
		delete __pFilePathList;
		__pFilePathList = NULL;
	}
	__pFilePathList = new ArrayList();

	IList* pImageList = contentSearch.SearchN(1, 1000, totalPage, totalCount, strQuery, sortColumn, SORT_ORDER_NONE);

	if (pImageList)
	{
		nItemCount = pImageList->GetCount();

	}

	for (int i = 0; i < nItemCount; i++)
	{
		ContentSearchResult* pInfo = (ContentSearchResult*) pImageList->GetAt(i);
		TryCatch(!IsFailed(GetLastResult()), r = GetLastResult(), "GetAt() is failed.");

		String tmpContentPath = ((ContentInfo*) pInfo->GetContentInfo())->GetContentPath();
		String strName;

		if (tmpContentPath.IsEmpty())
		{
			strName.Clear();
		}
		else
		{
			nStartIndex = tmpContentPath.GetLength() - 1;
			nIndex = 0;

			strName = tmpContentPath;
		}
		String* pFilePath = new String(strName);

		__pFilePathList->Add(*pFilePath);

		ImageMetadata* pImgMeta = ContentManagerUtil::GetImageMetaN(*pFilePath);

		if (pImgMeta == NULL)
		{
			return;
		}

		int nIndex;
		String fileName;
		result r = pFilePath->LastIndexOf("/", pFilePath->GetLength() - 1, nIndex);

		//Create substring of path to get File name
		if (r == E_SUCCESS && nIndex > 0)
		{
			r = pFilePath->SubString(nIndex + 1, strName);
			if (r != E_SUCCESS)
			{
				fileName = *pFilePath;
			}
		}
		else
		{
			fileName = *pFilePath;
		}

		Bitmap* pThumbnail = null;
		pThumbnail = pImgMeta->GetThumbnailN();

		if (pThumbnail == null)
		{
			return;
		}
		IconListViewItem* pItem = new IconListViewItem();
		r = pItem->Construct(*pThumbnail, &fileName, pThumbnail, pThumbnail);

		r = __pItemList->Add(*pItem);

		delete pImgMeta;
		delete pThumbnail;
	}

	CATCH: pImageList->RemoveAll(true);
	delete pImageList;

}

IconListViewItem* ImageGallery::CreateItem(int index)
{
	IconListViewItem* pItem = static_cast <IconListViewItem *> (__pItemList->GetAt(index));

	if (IsFailed(GetLastResult()))
	{
		AppLog("Item not found in list.");

		return NULL;
	}

	return pItem;

}
bool ImageGallery::DeleteItem(int index, Osp::Ui::Controls::IconListViewItem *pItem)
{
	result r = E_SUCCESS;

	return !(IsFailed(r));
}
int ImageGallery::GetItemCount(void)
{
	return __pItemList->GetCount();
}

void ImageGallery::GoToMainScreen()
{
	Form* pMainForm = FormManager::GetInstance()->GetFormById(Main::K_SCREEN_ID);
	if (!pMainForm)
	{
		AppLog("Main Form not found");

		return;
	}

	FormManager::GetInstance()->Jump(Main::K_SCREEN_ID);
}

void ImageGallery::DisplayImage()
{

	ImageViewer* pViewer = (ImageViewer*) FormManager::GetInstance()->GetFormById(ImageViewer::K_SCREEN_ID);

	if (pViewer == NULL)
	{
		pViewer = new ImageViewer();
		bool r = pViewer->Initialize();
		if (r)
		{
			FormManager::GetInstance()->Register(ImageViewer::K_SCREEN_ID, pViewer);
		}
	}

	String filePath = *(static_cast <String *> (__pFilePathList->GetAt(__currentSelectedIndex)));
	pViewer->SetImagePath(filePath);
	FormManager::GetInstance()->Jump(ImageViewer::K_SCREEN_ID);

	__pItemList->RemoveAll(true);

}

