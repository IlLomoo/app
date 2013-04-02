#include "framework.h"


namespace ca
{


   namespace fs
   {


      fs::fs()
      {
      }

      fs::~fs()
      {
      }

      bool fs::initialize()
      {

         if(Application.is_cube())
         {
            System.factory().creatable_small < ::fs::main_view > ();
            System.factory().creatable_small < ::fs::tree > ();
            System.factory().creatable_small < ::fs::list > ();
            System.factory().creatable_small < ::fs::list_item > ();
         }


         if(!::fs::fs::initialize())
            return false;


         return true;
      }
 


   } // namespace fs


} // namespace ca


