#ifndef _IMAGEGALLERY_H_
#define _IMAGEGALLERY_H_

#include <FBase.h>
#include <FUi.h>
#include <FContent.h>
#include <FApp.h>

class ImageGallery:
		public Osp::Ui::Controls::Form,
		public Osp::Ui::Controls::IFormBackEventListener,
		public Osp::Ui::Controls::IIconListViewItemProvider,
		public Osp::Ui::Controls::IIconListViewItemEventListener
{

public:
	ImageGallery(void);
	virtual ~ImageGallery(void);

	bool Initialize();
	result OnInitializing(void);
	result OnTerminating(void);
	result OnDraw(void);

	static Osp::Base::String K_SCREEN_ID;

	//Methods for ListIconView
	Osp::Ui::Controls::IconListViewItem* CreateItem(int index);
	bool DeleteItem(int index, Osp::Ui::Controls::IconListViewItem *pItem);
	int GetItemCount(void);

	//List Icon view listener
	void OnIconListViewItemStateChanged(Osp::Ui::Controls::IconListView & listView, int index,
			Osp::Ui::Controls::IconListViewItemStatus status);

protected:
	static const int ID_FOOTERBTN_VIEW = 201;
	static const int ID_FOOTERBTN_POST = 202;

private:
	void LoadGallery(void);

	void GoToMainScreen(void);
	void DisplayImage(void);

	Osp::Base::Collection::ArrayList* __pItemList;
	Osp::Base::Collection::ArrayList* __pFilePathList;
	Osp::Ui::Controls::IconListView* __pIconList;

	int __currentSelectedIndex;

	void OnFormBackRequested(Osp::Ui::Controls::Form &source);
};

#endif
