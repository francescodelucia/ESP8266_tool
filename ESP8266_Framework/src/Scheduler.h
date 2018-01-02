#ifndef _SCHEDULER_
#define _SCHEDULER_

extern "C" {
#include "user_interface.h"
}

typedef void(*CallBackRoiutine)();

struct SchedulerTask
{
	void*  cBackRoutine;
	int ms_event;
	long ms_event_last_run;
	uint8_t enable;
};

class Scheduler{
	private:
	
		int _ID_EVENT;
		
		struct SchedulerTask scheduler[10];
		
	public:
		Scheduler()
		{
			this->_ID_EVENT= 0;
			memset((void*)&scheduler,0,sizeof(SchedulerTask)*10);
		}
		
		int AddEvent(void *pCallBack,int millisecond_event)
		{
			this->scheduler[_ID_EVENT].cBackRoutine = pCallBack;
			this->scheduler[_ID_EVENT].ms_event = millisecond_event;
			this->scheduler[_ID_EVENT].ms_event_last_run = millis();
			this->scheduler[_ID_EVENT].enable = 1;
			this->_ID_EVENT ++;			
			return this->_ID_EVENT;
		} 
		int RemoveEvent(int ID_EVENT)
		{
			memset((void*)&scheduler[ID_EVENT],0,sizeof(SchedulerTask));
		}
		void DisableEvent(int ID_EVENT)
		{
			this->scheduler[_ID_EVENT].enable = 0;
		}
		void ModifyEvent(int ID_EVENT,void *pCallBack,int millisecond_event)
		{
			this->scheduler[_ID_EVENT].cBackRoutine = pCallBack;
			this->scheduler[_ID_EVENT].ms_event = millisecond_event;
			this->scheduler[_ID_EVENT].ms_event_last_run = millis();
		}
		void DisableAllEvent()
		{
			for(int k=0;k<10;k++)
			{
				if(this->scheduler[k].cBackRoutine != 0) {
					this->scheduler[_ID_EVENT].enable = 0;
				}
			}
		}
		void RestoreAllEvent()
		{
			for(int k=0;k<10;k++)
			{
				if(this->scheduler[k].cBackRoutine != 0) {
					this->scheduler[_ID_EVENT].enable = 1;
					this->scheduler[_ID_EVENT].ms_event_last_run = millis();
				}
			}
		}
		void RunScheduler()
		{			
			for(int k=0;k<10;k++)
			{
				if(this->scheduler[k].enable > 0) {
					ExecTask(&(this->scheduler[k]));
				}
			}
		}
	private:
		void ExecTask(SchedulerTask *pTask)
		{			
			if( pTask->cBackRoutine!=NULL)
			{
				if (millis() > pTask->ms_event_last_run + pTask->ms_event)  {
					CallBackRoiutine cbr = (CallBackRoiutine)pTask->cBackRoutine;				
					cbr();
					pTask->ms_event_last_run = millis();
				}
			}
		}
};
#endif
