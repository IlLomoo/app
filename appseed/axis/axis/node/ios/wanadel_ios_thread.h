#pragma once




namespace ios
{

   
   class CLASS_DECL_BASE thread :
      virtual public ::ansios::thread_impl
   {
   public:
      
      
      thread(sp(::aura::application) papp);
      virtual ~thread();
      
      
   };
   
   
} // namespace ios



