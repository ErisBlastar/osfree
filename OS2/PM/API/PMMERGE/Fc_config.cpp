/* Fc_config.cpp */
/* client config: write/read and analyze client's ini file
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fc_config.hpp"


int F_ClientConfig::Read(char *fname)
{  int id,rc;
   FILE *fp;
   char str[256],nameClass[128],name[128],par[128];
   fp = fopen(fname,"r");
   if(fp == NULL)
         return 1;
   do
   {  rc = ReadStr(fp, str,nameClass,name,par);
      if(!rc)
      { //  if(!strcmpi(nameClass,"prg"))
                 AnalizeRecodrRead(name,par);
        //  else
      }
   } while(!rc);

   fclose(fp);


   return 0;
}

int F_ClientConfig::ReadStr(FILE *fp, char *str, char *nameClass, char *name, char *par )
{  int i,iscomment,l;
   char *pstr;
/* �⠥� ��ப� */
M: pstr= fgets(str,128,fp);
   if(pstr == NULL) return 1; //EOF
/* ������㥬 ��ப� �㫥��� ����� */
   l = strlen(str);
   if(str[l-1] == '\n')
   {  str[--l] = 0;
   }
   if(l == 0) goto M;
/* ������㥬 �������ਨ */
   iscomment = 0;
   for(i=0;i<l;i++)
   { if(str[i] > 32)
     {   if(str[i] == ';') iscomment = 1;
         break;
     }
   }
   if(iscomment) goto M;
   pstr = strstr(str,"=");
   if(pstr == NULL)
        goto M; // ������㥬 ��ப� ��� "="
   *pstr = 0;
   strcpy(par,pstr+1);      // �⠥� ��ࠬ����
   pstr = strstr(str,".");  // �饬 ��� ��� ��।������ ����� �����
   if(pstr == NULL)
   {
//  ��ப� ��� "."
      nameClass[0] = 0;
      sscanf(str,"%s",name);
   } else {
      *pstr = 0;
      sscanf(str,"%s",nameClass);
      sscanf(pstr+1,"%s",name); // �⠥� ��� ��ࠬ���
   }
   return 0;
}

int F_ClientConfig::AnalizeRecodrRead(char *name, char *par)
{   static char *lsNames[]=
     { "debugOptions","remote",NULL
     };
      int i,i1,is=0,npar=0,rc;

     for(i=0;lsNames[i];i++)
     {   if(!strcmp(name,lsNames[i]))
         {  is = 1;
            npar = i;
            break;
         }
     }
     if(!is) return 1;
     switch(npar)
     {
        case 0:
           sscanf(par,"%s",&debugOptions);
          break;
        case 1:  /* drivepar0 */
           sscanf(par,"%s", ExternMachineName);
         break;
//        case 2:
//           sscanf(par,"%lf",&RfocusX);
//          break;
   }
    return 0;
}


