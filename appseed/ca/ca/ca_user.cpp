#include "framework.h"



namespace ca
{


    thread_base::thread_base()
    {

        m_pthread = NULL;

    }


    thread_base::~thread_base()
    {

    }


    HTHREAD thread_base::get_os_handle() const
    {

        return NULL;

    }


   int thread_base::get_x_window_count() const
   {

      return 0;

   }


   void thread_base::step_timer()
   {

   }

   bool thread_base::get_run()
   {
      return true;
   }

} // namespace ca


mutex & user_mutex()
{

   static mutex * s_pmutexUser = new mutex();

   return *s_pmutexUser;

}
