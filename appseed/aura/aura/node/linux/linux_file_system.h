#pragma once


namespace linux
{



   class CLASS_DECL_AURA file_system :
      virtual public ::file::system
   {
   public:



      file_system(::aura::application *  papp);
      virtual ~file_system();




   };


} // namespace ca2


