#ifndef ISLIDERENABLEDTOOL_H_
#define ISLIDERENABLEDTOOL_H_

class ISliderEnabledTool:
		public Osp::Ui::IAdjustmentEventListener
{
public:
	virtual ~ISliderEnabledTool()
	{
	}

	virtual void OnSliderSubmit(void) = 0;
	virtual void OnSliderCancel(void) = 0;
};

#endif /* ISLIDERENABLEDTOOL_H_ */
