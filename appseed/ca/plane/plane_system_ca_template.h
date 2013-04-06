#pragma once


namespace ca
{
 

   template < class T >
   void smart_pointer <T>::create(allocer palloc)
   {
      static class id idType = Sys(palloc->m_papp).type_info < T > ().m_id;
      if(m_p != ::null())
         ::ca::release(m_p);
      sp(::ca::ca) pca = Sys(palloc->m_papp).alloc(palloc->m_papp, idType);
      if(pca.is_set())
      {
         m_p = dynamic_cast < T * >(pca.m_p);
         if(m_p != ::null())
         {
            ::ca::add_ref(m_p);
         }
      }
   }

   template < class T >
   sp(T) smart_pointer <T>::clone() const   
   {
      if(m_p == ::null())
         return ::null();
      if(m_p->get_app() == ::null())
         return ::null();
      return Sys(m_p->m_papp).clone(m_p);
   }

   template < class T >
   void smart_pointer <T>::release()
   {

      ::ca::release(m_p);

   }


} // namespace ca




   template < size_t _Bits >
   inline ::ca::byte_output_stream & operator << (::ca::byte_output_stream & _Ostr, const bitset<_Bits>& _Right)
   {	
      // insert bitset as a string
	   return (_Ostr << _Right.template to_string());
   }

		// TEMPLATE operator>>
   template < size_t _Bits >
	inline ::ca::byte_input_stream & operator >>( ::ca::byte_input_stream &  _Istr, bitset<_Bits>& _Right)
	{	
      // extract bitset as a string
   	string _Str;
	
      _Istr >> _Str;

   	_Right = bitset<_Bits>(_Str);	// convert string and store
	   
      return (_Istr);

	}
