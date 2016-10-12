#if defined(MCHECK) || defined(__VLD) || defined(__MCRTDBG)

#error "MCHECK, __VLD or __MCRTDBG must not be defined for MEMDLEAK builds"

#endif

#ifdef WINDOWSEX
#include <mmsystem.h>
#endif // WINDOWSEX

#pragma comment(lib, "winmm.lib")


#define MEMDLEAK_DEFAULT 0

int g_iGlobalMemdleakEnabled;

thread_int_ptr < int_ptr > t_iMemdleak;

extern CLASS_DECL_AURA exception::engine * g_ee;

mutex * g_pmutgen = NULL;

extern memdleak_block * s_pmemdleakList;

thread_pointer < memdleak_block > t_plastblock;

void memdleak_init();
void memdleak_term();


BEGIN_EXTERN_C



void * aligned_memory_alloc(size_t size)
{

   void * p;

   p = unaligned_memory_alloc(size);

   //zero(p, size);

   return p;

}

void * unaligned_memory_alloc(size_t size)
{

   void * p;

   size_t nAllocSize = size + sizeof(memdleak_block);

   memdleak_block * pblock;

   pblock = (memdleak_block *) ::system_heap_alloc(nAllocSize);

   pblock->m_iBlockUse = 0;

   if (g_ee == NULL)
   {
//      pblock->m_puiStack = NULL;
      pblock->m_iStack = 0;
      pblock->m_pszFileName = NULL;
}
   else
   {
      //string strCallStack;
      //g_ee->stack_trace(1);
      pblock->m_iStack = sizeof(pblock->m_puiStack) / sizeof(pblock->m_puiStack[0]);
      g_ee->backtrace(pblock->m_puiStack, pblock->m_iStack);
      pblock->m_pszFileName = NULL;
      //pblock->m_pszFileName = strdup(pszFileName); // not trackable, at least think so certainly causes memory leak
   }

   ::lemon::set_maximum(pblock->m_uiLine);

   pblock->m_size = nAllocSize;

   synch_lock lock(g_pmutgen);

   pblock->m_pprevious = NULL;

   pblock->m_pnext = s_pmemdleakList;

   if (s_pmemdleakList != NULL)
   {

      s_pmemdleakList->m_pprevious = pblock;

   }

   s_pmemdleakList = pblock;

   t_plastblock = pblock;

   lock.unlock();

   p = &pblock[1];



   //zero(p, size);

   return p;

}


void * aligned_memory_alloc_dbg(size_t size, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{

   void * p;


   p = unaligned_memory_alloc(size);


    //zero(p, size);

    return p;

}


void * unaligned_memory_alloc_dbg(size_t size, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{

   void * p;


   p = unaligned_memory_alloc(size);


   //zero(p, size);

   return p;

}






#undef memory_alloc

void * memory_alloc(size_t size)
{

#if defined(APPLEOS)

   return aligned_memory_alloc(size);

#else

   return unaligned_memory_alloc(size);

#endif

}


void * memory_alloc_no_track(size_t size)
{


#if defined(APPLEOS)

   return aligned_memory_alloc(size);

#else

   return unaligned_memory_alloc(size);

#endif


}




void * memory_calloc(size_t size, size_t bytes)
{

   return memory_alloc(size * bytes);

}


void * memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{

   return unaligned_memory_alloc_dbg(nSize, nBlockUse, szFileName, nLine);

}


void * memory_realloc(void * pmemory, size_t nSize)
{

   return memory_realloc_dbg(pmemory, nSize, 0, NULL, -1);

}





void * memory_realloc_dbg(void * pmemory, size_t size, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{

   size_t nAllocSize = size + sizeof(memdleak_block);

   memdleak_block * pblock = &((memdleak_block *)pmemory)[-1];

   synch_lock lock(g_pmutgen);

   if (s_pmemdleakList == pblock)
   {
      s_pmemdleakList = s_pmemdleakList->m_pnext;
      s_pmemdleakList->m_pprevious = NULL;
   }
   else
   {
      pblock->m_pprevious->m_pnext = pblock->m_pnext;
      if (pblock->m_pnext != NULL)
      {
         pblock->m_pnext->m_pprevious = pblock->m_pprevious;
      }
   }

   //if (pblock->m_pszFileName)
     // ::system_heap_free((void *)pblock->m_pszFileName);
   //if (pblock->m_puiStack)
     // ::system_heap_free((void *)pblock->m_puiStack);

   size_t * psizeNew = NULL;

   pblock = (memdleak_block *) ::system_heap_realloc(pblock, size + sizeof(memdleak_block));

   pblock->m_iBlockUse = nBlockUse;
   if (g_ee == NULL)
   {
      pblock->m_iStack = 0;
      pblock->m_pszFileName = NULL;

   }
   else
   {
      //string strCallStack;
      //g_ee->stack_trace(1);
      pblock->m_iStack = sizeof(pblock->m_puiStack) / sizeof(pblock->m_puiStack[0]);
      g_ee->backtrace(pblock->m_puiStack, pblock->m_iStack);
      pblock->m_pszFileName = NULL;
   }

   ::lemon::set_maximum(pblock->m_uiLine);

   pblock->m_size = nAllocSize;

   pblock->m_pprevious = NULL;

   pblock->m_pnext = s_pmemdleakList;

   if (s_pmemdleakList != NULL)
   {

      s_pmemdleakList->m_pprevious = pblock;

   }

   s_pmemdleakList = pblock;
   lock.unlock();


   return &pblock[1];


}


void memory_free(void * pmemory)
{

   return memory_free_dbg(pmemory, 0);

}






void memory_free_dbg(void * pmemory, int32_t iBlockType)
{

   memdleak_block * pblock = &((memdleak_block *)pmemory)[-1];

   synch_lock lock(g_pmutgen);

   if (s_pmemdleakList == pblock)
   {
      s_pmemdleakList = s_pmemdleakList->m_pnext;
      if (s_pmemdleakList != NULL)
      {
         s_pmemdleakList->m_pprevious = NULL;

      }
   }
   else
   {
      if (pblock->m_pprevious != NULL)
      {
         pblock->m_pprevious->m_pnext = pblock->m_pnext;
      }
      if (pblock->m_pnext != NULL)
      {
         pblock->m_pnext->m_pprevious = pblock->m_pprevious;
      }
   }
   //if (pblock->m_pszFileName)
     // ::system_heap_free((void *)pblock->m_pszFileName);
   //if (pblock->m_puiStack)
     // ::system_heap_free((void *)pblock->m_puiStack);

      return ::system_heap_free(pblock);


}



size_t memory_size(void * pmemory)
{

   return memory_size_dbg(pmemory, _NORMAL_BLOCK);

}


size_t memory_size_dbg(void * pmemory, int32_t iBlockType)
{


   memdleak_block * pblock = &((memdleak_block *)pmemory)[-1];

   return pblock->m_size;


}
















CLASS_DECL_AURA int  memdleak_enabled()
{

   return global_memdleak_enabled() && (t_iMemdleak == (int_ptr) 1 || t_iMemdleak == (int_ptr)0);

}

CLASS_DECL_AURA void memdleak_enable(int enable)
{

   if (!enable)
   {

      t_iMemdleak = 1;

   }
   else
   {

      t_iMemdleak = -1;

   }


}

CLASS_DECL_AURA int  global_memdleak_enabled()
{

   if (g_iGlobalMemdleakEnabled == 0)
   {
      
      bool bMemdleak = false;
      
#ifdef WINDOWS

      uint32_t dwFileAttributes = GetFileAttributesW(L"C:\\archive\\ca2\\config\\system\\memdleak.txt");
      
      bMemdleak = dwFileAttributes != INVALID_FILE_ATTRIBUTES && (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0
      
#else
      
      bMemdleak = ::file_exists_dup("/archive/ca2/config/system/memdleak.txt");
      
#endif

      if (bMemdleak)
      {

         g_iGlobalMemdleakEnabled = 1;

      }
      else
      {

         if (MEMDLEAK_DEFAULT)
         {

            g_iGlobalMemdleakEnabled = 1;

         }
         else
         {

            g_iGlobalMemdleakEnabled = -1;

         }

      }

   }

   return g_iGlobalMemdleakEnabled == 1;

}


END_EXTERN_C




void memdleak_init()
{

   g_pmutgen = new mutex();

}


void memdleak_term()
{

   ::aura::del(g_pmutgen);

}
