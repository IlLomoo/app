#include "framework.h"


namespace hotplugin
{

   ::axis::system * g_paxissystem = NULL;

   int g_iSystemCount = 0;

   uint32_t c_cdecl axis_system_main(LPVOID lpVoid);


   CLASS_DECL_AXIS ::axis::system * get_axis_system()
   {
      
      return g_paxissystem;

   }

   CLASS_DECL_AXIS bool defer_start_axis_system()
   {

      if(g_paxissystem != NULL)
         return true;

      g_iSystemCount++;

      try
      {

         g_paxissystem = new ::axis::system(NULL);

         if(file_exists_dup("C:\\ca2\\config\\plugin\\npca2_beg_debug_box.txt"))
         {

            debug_box("hotplugin boxmain NP_Initialize","ZZZzzz hotplugin box",MB_OK);

         }

         ::set_thread(g_paxissystem);

         g_paxissystem->m_bMatterFromHttpCache = true;

         g_paxissystem->m_bSystemSynchronizedCursor = false;

         g_paxissystem->m_bShouldInitializeGTwf = false;

         g_paxissystem->m_bEnableOnDemandDrawing = false;

         g_paxissystem->construct(NULL);

#ifdef WINDOWS

         g_paxissystem->m_hinstance = (HINSTANCE)get_hinstance;

#endif

         xxdebug_box("box1","box1",MB_ICONINFORMATION);

#ifdef WINDOWS

         set_main_thread((HTHREAD) GetCurrentThread());

         set_main_thread_id(GetCurrentThreadId());

#endif

         g_paxissystem->m_bReady = false;

         ::create_thread(NULL,0,&axis_system_main,NULL,0,NULL);
         
      }
      catch(...)
      {

         return false;

         // debug_box("failed to create system", "npca2", 0);
         //      return NULL;
      }


      return true;

   }

   uint32_t c_cdecl axis_system_main(LPVOID lpVoid)
   {

      int32_t iReturnCode = 0;

      try
      {

         if(!g_paxissystem->pre_run())
         {

            if(g_paxissystem->m_iReturnCode == 0)
            {

               g_paxissystem->m_iReturnCode = -1;

            }

            g_paxissystem->m_bReady = true;

            return -1;

         }

      }
      catch(...)
      {

         if(g_paxissystem->m_iReturnCode == 0)
         {

            g_paxissystem->m_iReturnCode = -1;

         }

         g_paxissystem->m_bReady = true;

         return -1;

      }

      return g_paxissystem->main();

   }

#ifdef WINDOWS

   HINSTANCE g_hinstance = NULL; // hotplugin plugin dll HINSTANCE

   CLASS_DECL_AXIS HINSTANCE get_hinstance()
   {

      return g_hinstance;

   }

   CLASS_DECL_AXIS void set_hinstance(HINSTANCE hinstance)
   {

      g_hinstance = hinstance;

   }

#endif


   CLASS_DECL_AXIS void defer_stop_axis_system()
   {

      g_iSystemCount--;

      if(g_iSystemCount == 0)
      {

         if(g_paxissystem != NULL)
         {

            g_paxissystem->post_thread_message(WM_QUIT);

            g_paxissystem = NULL;

         }

         __wait_threading_count(minutes(1));

      }

   }

} // namespace hotplugin