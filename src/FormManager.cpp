#include "FormManager.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;

#define K_HASHMAP_SIZE 5

//Initialise the static instance - for singleton designs
FormManager* FormManager::__pFormManager = NULL;

FormManager::FormManager() :
	__pHmForms(NULL)
{
	AppLog("Created Form Manager.");
}

FormManager::~FormManager()
{
	RemoveAll();
	if (__pHmForms)
	{
		delete __pHmForms;
		__pHmForms = NULL;
	}
}

FormManager* FormManager::GetInstance()
{
	if (!FormManager::__pFormManager)
	{
		FormManager::__pFormManager = new FormManager();
		FormManager::__pFormManager->Initialize();
	}

	return FormManager::__pFormManager;
}

void FormManager::Initialize()
{
	__pHmForms = new HashMap();
	__pHmForms->Construct(K_HASHMAP_SIZE);
}

result FormManager::Register(String formId, Form* form)
{

	result r = E_FAILURE;

	if (!form)
	{
		return r;
	}

	r = __pHmForms->Add(*new String(formId), *form);

	return r;
}

result FormManager::UnRegister(String formId)
{

	result r = E_FAILURE;

	Form* form = (Form*) __pHmForms->GetValue(formId);
	Frame* pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();

	r = __pHmForms->Remove(formId, false);
	TryCatch(r==E_SUCCESS,,"Form remove failed in FormManager::UnRegister %s",GetErrorMessage(r));

	r = pFrame->RemoveControl(*form);

	TryCatch(r==E_SUCCESS,,"RemoveControl failed in FormManager::UnRegister %s",GetErrorMessage(r));

	CATCH:

	return r;
}

result FormManager::RemoveAll()
{

	result r = E_SUCCESS;

	__pHmForms->RemoveAll();

	return r;
}

Form* FormManager::GetFormById(String formId)
{
	Form* form = (Form*) __pHmForms->GetValue(formId);

	return form;
}

Form* FormManager::Forward()
{

	Form* prevForm = Jump(nextFormId);
	nextFormId = L"";

	return prevForm;
}

Form* FormManager::Back()
{
	Form* prevForm = Jump(backFormId);
	nextFormId = currentFormId;

	return prevForm;
}

Form* FormManager::Jump(String screenId)
{

	result r = E_SUCCESS;

	Form* oldForm = (Form*) __pHmForms->GetValue(currentFormId);

	Form* newForm = (Form*) __pHmForms->GetValue(screenId);

	if (IsFailed(GetLastResult()))
	{
		AppLog("Screen not found %d", GetLastResult());
		return oldForm;
	}

	Frame* pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();

	if (newForm->GetParent() == NULL)
	{
		r = pFrame->AddControl(*newForm);
		if (IsFailed(r))
		{
			AppLog("Could not add %ls to Frame!", screenId.GetPointer());
		}
	}

	r = pFrame->SetCurrentForm(*newForm);
	if (IsFailed(r))
	{
		AppLog("Could not set %ls as current form!", screenId.GetPointer());
	}

	r = newForm->Draw();
	if (IsFailed(r))
	{
		AppLog("Could not draw %ls as  form!", screenId.GetPointer());
	}

	newForm->Show();

	backFormId = currentFormId;
	currentFormId = screenId;

	return oldForm;
}

