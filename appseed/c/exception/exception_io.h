#pragma once


class CLASS_DECL_c io_exception :
   virtual public simple_exception
{
public:


   io_exception(sp(base_application) papp);
   io_exception(sp(base_application) papp, const char * pszMessage);
   virtual ~io_exception();


};











