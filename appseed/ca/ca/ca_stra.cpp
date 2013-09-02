#include "framework.h"


stringa::stringa()
{
}

stringa::~stringa()
{
}


void stringa::trim()
{
   for(int32_t ui = 0; ui < this->get_count(); ui++)
   {
      this->element_at(ui).trim();
   }
}

void stringa::remove_empty()
{
   for(int32_t ui = 0; ui < this->get_count();)
   {
      if(this->element_at(ui).length() == 0)
      {
         remove_at(ui);
      }
      else
      {
         ui++;
      }
   }
}

::count stringa::remove(const char * psz)
{
   ::count iCount = 0;
   for(index ui = 0; ui < this->get_count();)
   {
      if(this->element_at(ui) == psz)
      {
         remove_at(ui);
         iCount++;
      }
      else
      {
         ui++;
      }
   }
   return iCount;
}


index  stringa::find_ci(const char * psz, index iStart)
{
   if(iStart < 0)
      iStart = 0;
   for(index ui = iStart; ui < this->get_count();)
   {
      if(!stricmp_dup(this->element_at(ui), psz))
      {
         return ui;
      }
      else
      {
         ui++;
      }
   }
   return -1;
}

index  stringa::add_unique_ci(const char * psz)
{
   index iFind = find_ci(psz);
   if(iFind < 0)
   {
      add(psz);
      return get_count() - 1;
   }
   else
   {
      return iFind;
   }
}


void stringa::add_tokens(const char * psz, const char * pszSeparator)
{
	
   const char * pszPosBeg = psz;

	const char * pszPosEnd;

	::count iLenSeparator = strlen_dup(pszSeparator);

	vsstring str;

	while((pszPosEnd = strstr_dup(pszPosBeg, pszSeparator)))
	{

		str = vsstring(pszPosBeg, pszPosEnd - pszPosBeg);
      add(str);
      pszPosBeg = pszPosEnd + iLenSeparator;

	}

   if(pszPosBeg - psz == strlen_dup(psz))
		return;
	
   str = vsstring(pszPosBeg);
	
   add(str);

}


vsstring stringa::encode_v16()
{
   vsstring strEncode;
   for(int32_t ui = 0; ui < this->get_count(); ui++)
   {
      vsstring & str = this->element_at(ui);
      for(int32_t uj = 0; uj < str.length(); uj++)
      {
         char sz[32];
         itoa_dup(sz, str.m_psz[uj], 16);
         if(strlen_dup(sz) == 0)
         {
            strEncode += "00";
         }
         else if(strlen_dup(sz) == 1)
         {
            strEncode += "0";
            strEncode += sz;
         }
         else if(strlen_dup(sz) == 2)
         {
            strEncode += sz;
         }
      }
      strEncode += "00";
   }
   return strEncode;
}



void stringa::decode_v16(const char * psz)
{
   int32_t iSize = 1024;
   char * str = NULL;
   if(psz == NULL)
      return;
   while(*psz != '\0')
   {
      psz++;
      if(*psz == '\0')
         break;
      char sz[3];
      sz[0] = psz[-1];
      sz[1] = psz[0];
      sz[2] = '\0';
      const char * pszEnd;
      int32_t iConversion = ::atoi_dup(sz, &pszEnd, 16);
      char ch = static_cast < char > (iConversion);
      if(ch == '\0')
      {
         add(str);
         str = NULL;
         iSize = 1024;
      }
      else
      {
         if(str == NULL)
         {
            str = (char *) _ca_alloc(iSize);
         }
         else if(iSize < (strlen_dup(str) + 1))
         {
            char * strOld = str;
            iSize += 1024;
            char * strNew = (char *) _ca_alloc(iSize);
            strcpy_dup(strNew, strOld);
            str = strNew;
         }
         sz[0] = ch;
         sz[1] = '\0';
         strcat_dup(str, sz);
      }
      psz++;
   }

}

bool stringa::spa_insert(const char * psz)
{
   if(psz == NULL)
      return false;
   if(strlen_dup(psz) == 0)
      return false;
   vsstring str(psz);
   str.trim();
   if(str.length() == 0)
      return false;

   index iL = 0;

   index iU = this->get_count();

   index iM;

   int32_t iCmp;

   while(true)
   {
      iM = (iL + iU) / 2;
      if(iL >= iU)
         break;
      if(iM >= this->get_count())
         break;
      if(iM < 0)
         break;
      iCmp = strcmp_dup(operator[](iM), psz);
      if(iCmp < 0)
      {
         iL = iM + 1;
      }
      else if(iCmp > 0)
      {
         iU = iM;
      }
      else
      {
         return false;
      }
   }
   insert_at(iM, psz);
   return true;
}
