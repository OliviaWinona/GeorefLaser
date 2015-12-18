//XWait
//François Becirspahic  -Projet caméra numérique 

#ifndef _XWAIT_H
#define _XWAIT_H

#include <string>

//-----------------------------------------------------------------------------
//La classe XWait permet de gerer les temps d'attente lors des traitements longs
//-----------------------------------------------------------------------------
class XWait 
{
protected:
	int		m_nMin;
	int		m_nMax;
	int		m_nStep;
	bool	m_bCancel;
public:
	XWait(int min=0, int max=100, int step=1) : m_nMin(min), m_nMax(max), m_nStep(step) {m_bCancel = false;}
	virtual ~XWait() {;}

	virtual void SetRange(int min, int max) { m_nMin = min ; m_nMax = max;}
	virtual void SetStep(int step) { m_nStep = step;}
	virtual void StepIt() { m_nStep++;}
	virtual void SetStatus(const char* ) {;}
	virtual void Cancel() { m_bCancel = true;}
	virtual bool CheckCancel() { return m_bCancel;}
	virtual bool IsMultiThreadSafe() { return false;}

	friend void XWaitRange(XWait* wait, int min, int max)
											{ if (wait != NULL) wait->SetRange(min, max);}
	friend void XWaitStep(XWait* wait, int step) { if (wait != NULL) wait->SetStep(step);}
	friend void XWaitStepIt(XWait* wait) { if (wait != NULL) wait->StepIt();}
	friend void XWaitStatus(XWait* wait, const char* s) { if (wait != NULL) wait->SetStatus(s);}
	friend void XWaitStatus(XWait* wait, std::string& s) { XWaitStatus(wait, s.c_str());}
	friend void XWaitCancel(XWait* wait) { if (wait != NULL) wait->Cancel();}
	friend bool XWaitCheckCancel(XWait* wait) { if (wait != NULL) return wait->CheckCancel(); return false;}
};


#endif //_XWAIT_H
