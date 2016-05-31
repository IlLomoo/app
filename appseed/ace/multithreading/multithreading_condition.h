#pragma once


class CLASS_DECL_AURA condition :
   virtual public sync_object
{
public:

#if defined(LINUX) || defined(APPLEOS) || defined(SOLARIS)

   int_ptr           m_object;
   bool              m_bManualEvent;
   bool              m_bSignaled;  // meaningful only when m_bManualEvent
   int32_t           m_iSignalId;  // meaningful only when m_bManualEvent

#elif defined(ANDROID)

   bool              m_bSignaled;
   int               m_iHold;
   pthread_mutex_t   m_mutex;
   pthread_cond_t    m_cond;

#else

   CRITICAL_SECTION     m_sect;
   CONDITION_VARIABLE   m_var;

#endif


   condition(::ace::application * papp);
   virtual ~condition();


   using sync_object::lock;
   virtual bool lock(const duration & durationTimeout = duration::infinite());

   using sync_object::unlock;
   virtual bool unlock();

   virtual void * get_os_data() const;

   bool pulse();
   bool SetEvent();
   bool set_event() { return SetEvent(); }
   //bool PulseEvent();
   bool ResetEvent();


/**
* \brief	Session independent waitables (windows version)
*/

	///  \brief		waits for an event forever
	virtual void wait ();

	///  \brief		waits for an event for a specified time
	///  \param		duration time period to wait for an event
	///  \return	waiting action result as WaitResult
   virtual wait_result wait (const duration & duration);

   bool is_signaled() const;

};

