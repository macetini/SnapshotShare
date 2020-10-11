#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <FBase.h>
#include <FUi.h>
#include <FMedia.h>
#include <FIo.h>
#include <FGraphics.h>
#include "GrayscaleTool.h"
//#include "BrightnessTool.h"
#include "ThresholdTool.h"
#include "FlipTool.h"
#include "FlipColorTool.h"
#include "DeleteTool.h"
#include "RotationTool.h"
#include "CropTool.h"
#include "IMessageBoxListener.h"

class Editor:
		public Osp::Ui::Controls::Form,
		public Osp::Ui::IActionEventListener,
		public Osp::Ui::Controls::IGalleryItemProvider,
		public IMessageBoxListener,
		public Osp::Ui::Controls::IFormBackEventListener,
		public Osp::Ui::IOrientationEventListener
{
public:
	Editor(void);
	virtual ~Editor(void);
	bool Initialize();
	result OnInitializing(void);
	result OnDraw(void);
	result OnTerminating(void);

	static const int K_IMG_WIDTH = 1024;
	static const int K_IMG_HEIGHT = 768;

	Osp::Graphics::Rectangle GetImageBound();

	Osp::Ui::Controls::GalleryItem * CreateItem(int index);
	bool DeleteItem(int index, Osp::Ui::Controls::GalleryItem *pItem);
	int GetItemCount(void);

	void ActivateSlider(bool activate, ISliderEnabledTool* pListener, Osp::Base::String title = L"", int min = 0,
			int max = 100);
	void RedrawSlider(void);

	result AddToPanel(Osp::Ui::Control& control);
	result RemoveFromPanel(Osp::Ui::Control& control);

	void SetImagePath(Osp::Base::String imagePath);

	static Osp::Base::String K_SCREEN_ID;
	void DrawImageOnCanvas(void);
	void SetCurrentBitmap(Osp::Graphics::Bitmap* pNewBmp);

	Osp::Graphics::Rectangle GetImageBounds();
	Osp::Graphics::Dimension GetImageSize();

	void OnMessageBoxResult(int messageId, int result);

	void OnUserEventReceivedN(RequestId requestId, Osp::Base::Collection::IList* pArgs);
	void OnOrientationChanged(const Osp::Ui::Control &source, Osp::Ui::OrientationStatus orientationStatus);

	int GetTopBarHt(void);
	int GetLeftPanelWidth(void);

	void SaveButtonStatus(bool state);

	Osp::Graphics::Rectangle GetActualBounds(void);

private:

	static int const K_SLIDER_BUTTON_SIZE = 100;

	static const int ID_FOOTERBTN_SAVE = 301;
	static const int ID_FOOTERBTN_UNDO = 302;
	static const int ID_FOOTERBTN_REDO = 303;

	static const int ID_HEADERBTN_COLOR = 304;
	static const int ID_HEADERBTN_COLORGRAYSCALE = 3041;
	//static const int ID_HEADERBTN_COLORBRIGHTNESS = 3042;
	static const int ID_HEADERBTN_COLORTHRESHOLD = 3042;
	static const int ID_HEADERBTN_COLORDELETE = 3043;
	static const int ID_HEADERBTN_COLORFLIP = 3044;

	static const int ID_HEADERBTN_IMGSIZE = 305;
	static const int ID_HEADERBTN_IMGSIZECROP = 3051;
	static const int ID_HEADERBTN_IMGSIZEROTATE = 3052;
	static const int ID_HEADERBTN_IMGSIZEFLIP = 3053;

	/*static const int ID_HEADERBTN_BORDERS = 306;
	static const int ID_HEADERBTN_BORDER1 = 3061;
	static const int ID_HEADERBTN_BORDER2 = 3062;*/

	static const int ID_SLIDER_DONE = 307;
	static const int ID_SLIDER_CANCEL = 308;

	static const int ID_MSG_SAVE = 310;
	static const int ID_MSG_SAVEREPLACE = 311;

	static const int K_HIST_NONE = 0;
	static const int K_HIST_UNDO = 1;
	static const int K_HIST_REDO = 2;

	void LoadImage();

	void OnActionPerformed(const Osp::Ui::Control &source, int actionId);

	void OnFormBackRequested(Osp::Ui::Controls::Form &source);

	void OnUndo(void);
	void OnRedo(void);
	void OnSave(void);
	void SaveImage(bool createNew);
	void BackUpCurrentBitmap(void);
	void TrimBackUpArray(void);

	void ApplyGrayScaleFilter(void);
	//void ApplyBrightness(void);
	void ApplyThreshold(void);
	void ApplyFlipColor(void);
	void ApplyDelete(void);
	void ApplyFlipEffect(void);
	void ApplyRotateEffect(void);
	void ApplyCropEffect(void);
	void ApplyBorderEffect(int borderId);

	void ResetSliderPosition(void);

	void SetTitle(void);
	void UpdateUndoRedo();
	void EnableUi(bool enabled);

	void ClearEditor();

	int __historyState;

	Osp::Base::String __currentImagePath;

	Osp::Ui::Controls::Panel* __pMainPanel;
	Osp::Ui::Controls::Gallery* __pGallery;
	Osp::Ui::Controls::Form* __pForm;

	Osp::Ui::Controls::ContextMenu* __pColorMenu;
	Osp::Ui::Controls::ContextMenu* __pSizeMenu;
	Osp::Ui::Controls::ContextMenu* __pBorderMenu;

	Osp::Ui::Controls::Slider* __pSlider;
	Osp::Ui::Controls::Button* __pSliderSubmitBtn;
	Osp::Ui::Controls::Button* __pSliderCancelBtn;

	Osp::Graphics::Bitmap* __pCurrentBitmap;
	Osp::Base::Collection::ArrayList* __pBackupArray;

	GrayscaleTool* __pGrayscaleTool;
	//BrightnessTool* __pBrightnessTool;
	ThresholdTool* __pThresholdTool;
	FlipColorTool* __pFlipColorTool;
	DeleteTool* __pDeleteTool;
	FlipTool* __pFlipTool;
	RotationTool* __pRotateTool;
	CropTool* __pCropTool;
	//BorderTool* __pBorderTool;

	ISliderEnabledTool* __pSliderListener;

	bool __isScaled;
	Osp::Graphics::Point __lastTouch;
	Osp::Graphics::Rectangle __bmpRectangle;
	Osp::Graphics::Dimension __imageSize;

};

#endif /* _EDITOR_H_ */
