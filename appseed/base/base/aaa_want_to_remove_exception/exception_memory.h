#pragma once


class CLASS_DECL_BASE memory_exception : 
   virtual public simple_exception
{
public:


   memory_exception(sp(::axis::application) papp);
   memory_exception(sp(::axis::application) papp, const char * pszMessage);
   virtual ~memory_exception();


};


#if defined(SOLARIS)


namespace std
{

   
   class CLASS_DECL_BASE bad_alloc :
      virtual public memory_exception
   {
   public:
      
      
      bad_alloc() : 
         element(::get_thread_app()), 
         ::call_stack(::get_thread_app()), 
         ::exception::base(::get_thread_app()),
         simple_exception(::get_thread_app()),
         memory_exception(::get_thread_app())
      {
         
      }
         
      virtual ~bad_alloc() {}
   
      
   };


} 

#endif



