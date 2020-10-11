#ifndef FORMMANAGER_H_
#define FORMMANAGER_H_

#include <FUi.h>
#include <FBase.h>
#include <FApp.h>

class FormManager
{
public:
	virtual ~FormManager();
	static FormManager* GetInstance();

	Osp::Ui::Controls::Form* Forward(void);
	Osp::Ui::Controls::Form* Back(void);
	Osp::Ui::Controls::Form* Jump(Osp::Base::String screenId);

	result Register(Osp::Base::String formId, Osp::Ui::Controls::Form* pForm);
	result UnRegister(Osp::Base::String formId);
	result RemoveAll();

	Osp::Ui::Controls::Form* GetFormById(Osp::Base::String screenId);

private:
	FormManager();
	void Initialize();

	static FormManager* __pFormManager;

	Osp::Base::Collection::HashMap* __pHmForms;
	Osp::Base::String currentFormId;
	Osp::Base::String backFormId;
	Osp::Base::String nextFormId;
};

#endif /* FORMMANAGER_H_ */
