#pragma once


class data;


class CLASS_DECL_c data_listener :
   virtual public object
{
public:

   
   comparable_array < data * > m_dataptra;

   
   data_listener(sp(base_application) papp);
   virtual ~data_listener();


   bool listen(data * pdata, bool bListen = true);


   virtual void on_update_data(data * pdata, int32_t iHint);

};




