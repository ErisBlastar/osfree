/*
 *  API implementations
 *  parts (on the client side)
 *  These functions are those which are
 *  exported from the KAL.DLL virtual library.
 */

#include <l4/l4rm/l4rm.h>
#include <l4/env/env.h>
#include <l4/env/errno.h>
#include <l4/log/l4log.h>
#include <l4/names/libnames.h>
#include <l4/thread/thread.h>
#include <l4/semaphore/semaphore.h>
#include <l4/lock/lock.h>
#include <l4/dm_generic/consts.h>
#include <l4/dm_mem/dm_mem.h>
#include <l4/sys/kdebug.h>
#include <l4/generic_ts/generic_ts.h>

#include <l4/os3/gcc_os2def.h>
#include <l4/os3/apistub.h>

#include <dice/dice.h>

#include <l4/os2srv/os2server-client.h>
#include <l4/execsrv/os2exec-client.h>

#include <handlemgr.h>
#include <stacksw.h>

char LOG_tag[9];

struct kal_init_struct
{
  l4_threadid_t fs;
  l4_threadid_t os2srv;
  l4_threadid_t execsrv;
  unsigned long stack;
  void *l4rm_detach;
  void *l4rm_do_attach;
  void *l4rm_lookup;
  void *l4rm_lookup_region;
  void *l4rm_do_reserve;
  void *l4rm_set_userptr;
  void *l4rm_get_userptr;
  void *l4rm_area_release;
  void *l4rm_area_release_addr;
  void *l4env_get_default_dsm;
  CORBA_Environment *env;
  char *logtag;
};

#define SEMTYPE_EVENT    0
#define SEMTYPE_MUTEX    1
#define SEMTYPR_MUXWAIT  2

/* Semaphore handle table pointer */
HANDLE_TABLE *htSem;
/* Handle table element           */
typedef struct _SEM
{
  struct _SEM *pNext;
  char        szName[CCHMAXPATH];
  char        cShared;
  char        cType;
  ULONG       ulRefCnt; 
  union
  {
    l4semaphore_t evt;
    l4lock_t mtx;
    struct _SEM *mux;
  }           uSem;
} SEM, *PSEM;

l4_threadid_t fs;
l4_threadid_t os2srv;
l4_threadid_t execsrv;
unsigned long __stack;

static ULONG CurMaxFH = 20;

L4_CV int
(*l4rm_do_attach_ptr)(const l4dm_dataspace_t * ds, l4_uint32_t area, l4_addr_t * addr,
               l4_size_t size, l4_offs_t ds_offs, l4_uint32_t flags);

L4_CV int
(*l4rm_detach_ptr)(const void * addr);

L4_CV int
(*l4rm_lookup_ptr)(const void * addr, l4_addr_t * map_addr, l4_size_t * map_size,
            l4dm_dataspace_t * ds, l4_offs_t * offset, l4_threadid_t * pager);

L4_CV int
(*l4rm_lookup_region_ptr)(const void * addr, l4_addr_t * map_addr,
                   l4_size_t * map_size, l4dm_dataspace_t * ds,
                   l4_offs_t * offset, l4_threadid_t * pager);

L4_CV int
(*l4rm_do_reserve_ptr)(l4_addr_t * addr, l4_size_t size, l4_uint32_t flags,
                l4_uint32_t * area);

L4_CV int
(*l4rm_set_userptr_ptr)(const void * addr, void * ptr);

L4_CV void *
(*l4rm_get_userptr_ptr)(const void * addr);

L4_CV int
(*l4rm_area_release_ptr)(l4_uint32_t area);

L4_CV int
(*l4rm_area_release_addr_ptr)(const void * ptr);

L4_CV l4_threadid_t
(*l4env_get_default_dsm_ptr)(void);

//int l4env_argc;
//char **l4env_argv;

unsigned long
pvtLoadModule(char *pszName,
              unsigned long cbName,
              char const *pszModname,
              os2exec_module_t *s,
              unsigned long *phmod);

void kalInit(struct kal_init_struct *s)
{
  strncpy(LOG_tag, s->logtag, 9);
  LOG_tag[8] = '\0';
  LOG("s=0x%x", s);
  fs      = s->fs;
  os2srv  = s->os2srv;
  execsrv = s->execsrv;
  LOG("execsrv=%u.%u", s->execsrv.id.task, s->execsrv.id.lthread);
  LOG("execsrv2=%u.%u", execsrv.id.task, execsrv.id.lthread);
  __stack = s->stack;
  l4rm_do_attach_ptr = s->l4rm_do_attach;
  l4rm_detach_ptr    = s->l4rm_detach;
  l4rm_lookup_ptr    = s->l4rm_lookup;
  l4rm_lookup_region_ptr = s->l4rm_lookup_region;
  l4rm_do_reserve_ptr    = s->l4rm_do_reserve;
  l4rm_set_userptr_ptr   = s->l4rm_set_userptr; 
  l4rm_get_userptr_ptr   = s->l4rm_get_userptr; 
  l4rm_area_release_ptr  = s->l4rm_area_release;
  l4rm_area_release_addr_ptr = s->l4rm_area_release_addr;
  l4env_get_default_dsm_ptr  = s->l4env_get_default_dsm;
}
  
/* Wrappers */

L4_CV int
l4rm_do_attach(const l4dm_dataspace_t * ds, l4_uint32_t area, l4_addr_t * addr,
               l4_size_t size, l4_offs_t ds_offs, l4_uint32_t flags)
{
  return l4rm_do_attach_ptr(ds, area, addr,
               size, ds_offs, flags);
}

L4_CV int
l4rm_detach(const void * addr)
{
  return l4rm_detach_ptr(addr);
}

L4_CV int
l4rm_lookup(const void * addr, l4_addr_t * map_addr, l4_size_t * map_size,
            l4dm_dataspace_t * ds, l4_offs_t * offset, l4_threadid_t * pager)
{
  return l4rm_lookup_ptr(addr, map_addr, map_size,
            ds, offset, pager);
}

L4_CV int
l4rm_lookup_region(const void * addr, l4_addr_t * map_addr,
                   l4_size_t * map_size, l4dm_dataspace_t * ds,
                   l4_offs_t * offset, l4_threadid_t * pager)
{
  return l4rm_lookup_region_ptr(addr, map_addr,
                   map_size, ds, offset, pager);
}

L4_CV int
l4rm_do_reserve(l4_addr_t * addr, l4_size_t size, l4_uint32_t flags,
                l4_uint32_t * area)
{
  return l4rm_do_reserve_ptr(addr, size, flags, area);
}

L4_CV int
l4rm_set_userptr(const void * addr, void * ptr)
{
  return l4rm_set_userptr_ptr(addr, ptr);
}

L4_CV void *
l4rm_get_userptr(const void * addr)
{
  return l4rm_get_userptr_ptr(addr);
}

L4_CV int
l4rm_area_release(l4_uint32_t area)
{
  return l4rm_area_release_ptr(area);
}

L4_CV int
l4rm_area_release_addr(const void * ptr)
{
  return l4rm_area_release_addr_ptr(ptr);
}

L4_CV l4_threadid_t
l4env_get_default_dsm(void)
{
  return l4env_get_default_dsm_ptr();
}

int
strlstlen(char *p)
{
  int l, len = 0;

  if (!p || !*p)
    return 2;
    
  while (*p) // skip all lines; break if NULL
  {
    /* skip one string */
    l = strlen(p) + 1;
    /* skip NULL character */
    p   += l;
    len += l;
  }

  if (!len)
    len++;
    
  len++; // include NULL symbol

  return len;
}


APIRET CDECL
kalOpenL (PSZ pszFileName,
          HFILE *phFile,
	  ULONG *pulAction,
	  LONGLONG cbFile,
	  ULONG ulAttribute,
	  ULONG fsOpenFlags,
	  ULONG fsOpenMode,
	  PEAOP2 peaop2)
{
  CORBA_Environment env = dice_default_environment;
  EAOP2 eaop2;
  APIRET  rc;

  STKIN
  LOG("pszFileName=%s", pszFileName);
  LOG("ulAction=%x", *pulAction);
  LOG("cbFile=%u", cbFile);
  LOG("ulAttribute=%x", ulAttribute);
  LOG("fsOpenFlags=%x", fsOpenFlags);
  LOG("fsOpenMode=%x", fsOpenMode);
  if (peaop2 == NULL)
    peaop2 = &eaop2;
  rc = os2fs_dos_OpenL_call (&fs, pszFileName, phFile,
                      pulAction, cbFile, ulAttribute,
                      fsOpenFlags, fsOpenMode, peaop2, &env);
  LOG("hFile=%x", *phFile);
  LOG("rc=%x", rc);
  LOG("exit");
  STKOUT
  return rc;
}

APIRET CDECL
kalFSCtl (PVOID pData,
          ULONG cbData,
	  PULONG pcbData,
	  PVOID pParms,
	  ULONG cbParms,
	  PULONG pcbParms,
	  ULONG function,
	  PSZ pszRoute,
	  HFILE hFile,
	  ULONG method)
{
  CORBA_Environment env = dice_default_environment;
  APIRET  rc;

  STKIN
  LOG("pData=%x", pData);
  LOG("cbData=%u", cbData);
  LOG("pParms=%x", pParms);
  LOG("cbParms=%u", cbParms);
  LOG("function=%x", function);
  LOG("pszRoute=%s", pszRoute);
  LOG("hFile=%x", hFile);
  LOG("method=%x", method);
  // ...
  LOG("*pcbData=%u", *pcbData);
  LOG("*pcbParms=%u", *pcbParms);
  STKOUT
  return rc;
}

APIRET CDECL
kalRead (HFILE hFile, PVOID pBuffer,
            ULONG cbRead, PULONG pcbActual)
{
  CORBA_Environment env = dice_default_environment;
  APIRET  rc;

  STKIN

  LOG("started");
  LOG("hFile=%x", hFile);
  LOG("pBuffer=%x", pBuffer);
  LOG("cbRead=%u", cbRead);
  
  if (!cbRead)
  {
    STKOUT
    return 0; /* NO_ERROR */
  }

  rc = os2fs_dos_Read_call(&fs, hFile, &pBuffer, &cbRead, &env);

  *pcbActual = cbRead;

  LOG("*pcbActual=%u", *pcbActual);
  LOG("ended");
  STKOUT
  return rc;
}


APIRET CDECL
kalWrite (HFILE hFile, PVOID pBuffer,
              ULONG cbWrite, PULONG pcbActual)
{
  CORBA_Environment env = dice_default_environment;
  APIRET  rc;

  STKIN

  LOG("started");
  LOG("hFile=%x", hFile);
  LOG("pBuffer=%x", pBuffer);
  LOG("cbWrite=%u", cbWrite);

  if (!cbWrite)
  {
    STKOUT
    return 0; /* NO_ERROR */
  }

  rc = os2fs_dos_Write_call(&fs, hFile, pBuffer, &cbWrite, &env);

  *pcbActual = cbWrite;

  LOG("*pcbActual=%u", *pcbActual);
  LOG("ended");
  STKOUT
  return rc;
}

APIRET CDECL
kalLogWrite (PSZ s)
{
  STKIN
  LOG_printf("%s", s);
  STKOUT
  return 0; /* NO_ERROR */
}

VOID CDECL
kalExit(ULONG action, ULONG result)
{
  CORBA_Environment env = dice_default_environment;
  STKIN
  // send OS/2 server a message that we want to terminate
  LOG("action=%u", action);
  LOG("result=%u", result);
  os2server_dos_Exit_send(&os2srv, action, result, &env);
  // tell L4 task server that we want to terminate
  //l4_ipc_sleep(L4_IPC_NEVER);
  l4ts_exit();
  STKOUT
}


APIRET CDECL
kalQueryCurrentDisk(PULONG pdisknum,
                        PULONG plogical)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  rc = os2server_dos_QueryCurrentDisk_call(&os2srv, pdisknum, plogical, &env);
  LOG("*pdisknum=%u", *pdisknum);
  LOG("*logical=%x", *plogical);
  STKOUT
  return rc;
}

APIRET CDECL
kalSetCurrentDir(PSZ pszDir)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("pszDir=%s", pszDir);
  rc = os2server_dos_SetCurrentDir_call(&os2srv, pszDir, &env);
  STKOUT
  return rc;
}

APIRET CDECL
kalSetDefaultDisk(ULONG disknum)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("disknum=%u", disknum);
  rc = os2server_dos_SetDefaultDisk_call(&os2srv, disknum, &env);
  STKOUT
  return rc;
}

APIRET CDECL
kalQueryCurrentDir(ULONG disknum,
                       PBYTE pBuf,
                       PULONG pcbBuf)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  char buf[0x100];
  STKIN
  LOG("disknum=%u", disknum);
  rc = os2server_dos_QueryCurrentDir_call(&os2srv, disknum, &pBuf, pcbBuf, &env);
  strncpy(buf, pBuf, *pcbBuf);
  buf[*pcbBuf] = '\0';
  LOG("dir=%s", buf);
  STKOUT
  return rc;
}


APIRET CDECL
kalQueryProcAddr(ULONG hmod,
                     ULONG ordinal,
                     const PSZ  pszName,
                     void  **ppfn)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("hmod=%x", hmod);
  LOG("ordinal=%u", ordinal);
  LOG("pszName=%s", pszName);
  rc = os2exec_query_procaddr_call(&execsrv, hmod, ordinal,
                                   pszName, (void **)ppfn, &env);
  LOG("*ppfn=%x", *ppfn);
  STKOUT
  return rc;
}

APIRET CDECL
kalQueryModuleHandle(const char *pszModname,
                     unsigned long *phmod)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("pszModname=%s", pszModname);
  rc = os2exec_query_modhandle_call(&execsrv, pszModname,
                                    phmod, &env);
  LOG("*phmod=%x", *phmod);
  STKOUT
  return rc;
}

APIRET CDECL
kalQueryModuleName(unsigned long hmod, unsigned long cbBuf, char *pBuf)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("hmod=%x", hmod);
  LOG("cbBuf=%u", cbBuf);
  rc = os2exec_query_modname_call(&execsrv, hmod,
                                  cbBuf, &pBuf, &env);
  LOG("pBuf=%s", pBuf);
  STKOUT
  return rc;
}

/** attach dataspace to our address space. (any free address) */
int
attach_ds(l4dm_dataspace_t *ds, l4_uint32_t flags, l4_addr_t *addr)
{
  int error;
  l4_size_t size;
 
  if ((error = l4dm_mem_size(ds, &size)))
    {
      printf("Error %d (%s) getting size of dataspace\n",
	  error, l4env_errstr(error));
      return error;
    }
  
  if ((error = l4rm_attach(ds, size, 0, flags, addr)))
    {
      printf("Error %d (%s) attaching dataspace\n",
	  error, l4env_errstr(error));
      return error;
    }
  return 0;
}

/** attach dataspace to our address space. (concrete address) */
int
attach_ds_reg(l4dm_dataspace_t ds, l4_uint32_t flags, l4_addr_t addr)
{
  int error;
  l4_size_t size;
  l4_addr_t a = addr;

  /* get dataspace size */
  LOG("1");
  if ((error = l4dm_mem_size(&ds, &size)))
    {
      printf("Error %d (%s) getting size of dataspace\n",
	  error, l4env_errstr(error));
      return error;
    }

  /* attach it to a given region */  
  LOG("2");
  if ((error = l4rm_attach_to_region(&ds, a, size, 0, flags)))
    {
      printf("Error %d (%s) attaching dataspace\n",
	  error, l4env_errstr(error));
      return error;
    }

  LOG("3");
  return 0;
}

/** attach dataspace to our address space. (concrete address) */
int
attach_ds_area(l4dm_dataspace_t ds, l4_uint32_t area, l4_uint32_t flags, l4_addr_t addr)
{
  int error;
  l4_size_t size;
  l4_addr_t a = addr;

  /* get dataspace size */
  if ((error = l4dm_mem_size(&ds, &size)))
    {
      printf("Error %d (%s) getting size of dataspace\n",
	  error, l4env_errstr(error));
      return error;
    }

  /* attach it to a given region */  
  if (error = l4rm_area_attach_to_region(&ds, area,
                       addr, size, 0, flags))
    {
      printf("Error %d (%s) attaching dataspace\n",
	  error, l4env_errstr(error));
      return error;
    }

  return 0;
}

/*  Attaches all sections
 *  for a given module
 */
int
attach_module (ULONG hmod)
{
  CORBA_Environment env = dice_default_environment;
  l4exec_section_t sect;
  l4dm_dataspace_t ds, area_ds;
  l4_uint32_t flags;
  l4_addr_t   addr, map_addr;
  l4_size_t   map_size;
  l4_offs_t   offset;
  l4_threadid_t pager;
  unsigned type;
  int index;
  ULONG rc;

  index = 0; rc = 0;
  while (!os2exec_getsect_call (&execsrv, hmod, &index, &sect, &env) && !rc)
  {
    ds    = sect.ds; 
    addr  = sect.addr;
    type  = sect.info.type;
    flags = 0;  

    LOG("dataspace %x", ds); 
    LOG("addr %x", addr);
    
    if (type & L4_DSTYPE_READ)
      flags |= L4DM_READ;

    if (type & L4_DSTYPE_WRITE)
      flags |= L4DM_WRITE;

    LOG("1");
    if ((rc = l4rm_lookup(addr, &map_addr, &map_size,
                    &area_ds, &offset, &pager)) != L4RM_REGION_DATASPACE)
    {
      LOG("2");
      rc = attach_ds_reg (ds, flags, addr);
      LOG("3");
      if (!rc) 
        LOG("attached");
      else
      {
        LOG("attach_ds_reg returned %u", rc);
        break;
      }
    }
    else
    {
      LOG("map_addr=%x, map_size=%u, area_ds=%x",
          map_addr, map_size, area_ds);
      break;
    }
  }

  LOG("3");

  return 0; //rc;
}


/*  Attaches recursively a module and
 *  all its dependencies
 */
int
attach_all (ULONG hmod)
{
  CORBA_Environment env = dice_default_environment;
  ULONG imp_hmod, rc = 0;
  int index = 0;

  LOG("attach_all called");
  rc = attach_module(hmod);
  
  if (rc)
    return rc;
    
  while (!os2exec_getimp_call (&execsrv, hmod, &index, &imp_hmod, &env) && !rc)
  {
    if (!imp_hmod) // KAL fake module: no need to attach sections
      continue;
    //  return 0;
  
    rc = attach_all(imp_hmod);
  }

  LOG("attach_all returned: %u", rc);
  
  return rc;
}

unsigned long
pvtLoadModule(char *pszName,
              unsigned long cbName,
              char const *pszModname,
              os2exec_module_t *s,
              unsigned long *phmod)
{
  CORBA_Environment env = dice_default_environment;
  l4dm_dataspace_t ds = L4DM_INVALID_DATASPACE;
  ULONG hmod, rc;

  LOG("PvtLoadModule called");
  LOG("execsrv=%u.%u", execsrv.id.task, execsrv.id.lthread);
  LOG("env=0x%x", env);
  LOG("pszModname=%s", pszModname);
  rc = os2exec_open_call (&execsrv, pszModname, &ds,
                          1, &hmod, &env);
  if (rc)
    return rc;

  *phmod = hmod;

  LOG("os2exec_open_call() called, rc=%d", rc);
  rc = os2exec_load_call (&execsrv, hmod, s, &env);

  if (rc)
    return rc;

  LOG("os2exec_load_call() called, rc=%d", rc);
  rc = os2exec_share_call (&execsrv, hmod, &env);

  if (rc)
    return rc;

  LOG("os2exec_share_call() called, rc=%d", rc);
  rc = attach_all(hmod);

  if (rc)
    return rc;

  LOG("attach_all() called, rc=%d", rc);

  return 0;
}


APIRET CDECL
kalLoadModule(PSZ pszName,
                  ULONG cbName,
                  char const *pszModname,
                  PULONG phmod)
{
  os2exec_module_t s;
  int rc;
  STKIN
  LOG("pszName=%s");
  LOG("cbName=%u", cbName);
  rc = pvtLoadModule(pszName, cbName, pszModname,
                       &s, phmod);
  LOG("pszModname=%s", pszModname);
  LOG("*phmod=%x", *phmod);
  STKOUT
  return rc;
}

#define PT_16BIT 0
#define PT_32BIT 1

APIRET CDECL
kalQueryProcType(HMODULE hmod,
                 ULONG ordinal,
		 PSZ pszName,
		 PULONG pulProcType)
{
  void *pfn;
  APIRET rc;

  STKIN
  rc = kalQueryProcAddr(hmod, ordinal, pszName, &pfn);
  if (rc) return rc;
  if (pfn)
    *pulProcType = PT_32BIT; 
  STKOUT
  return NO_ERROR;
}


APIRET CDECL
kalQueryAppType(PSZ pszName,
                PULONG pFlags)
{
  APIRET rc;

  STKIN
  // ...
  STKOUT
  return rc;
}


APIRET CDECL
kalExecPgm(char *pObjname,
           long cbObjname,
           unsigned long execFlag,
           char *pArg,
           char *pEnv,
           struct _RESULTCODES *pRes,
           char *pName)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;
  int i, j, l;

  STKIN
  LOG("started");
  LOG("cbObjname=%x", (unsigned)cbObjname);
  LOG("pName=%s", pName);
  LOG("execFlag=%x", (unsigned)execFlag);
  
  if (pArg == NULL)
  {
    pArg = "\0\0";
    i = 2;
  }
  else
    i = strlstlen(pArg);

  if (pEnv == NULL)
  {
    pEnv = "\0\0";
    j = 2;
  }
  else
    j = strlstlen(pEnv);
  
  l = strlstlen(pArg);
  LOG("pArg len=%d", l);
  LOG("pEnv len=%d", strlstlen(pEnv));
  LOG("pArg=%x", (unsigned)pArg);
  LOG("len of pArg=%d", l);
  rc =  os2server_dos_ExecPgm_call (&os2srv, &pObjname,
                        &cbObjname, execFlag, pArg, i,
			pEnv, j,
                        pRes, pName, &env);
  LOG("pRes=%x", (unsigned)pRes);
  LOG("pObjname=%x",  (unsigned)pObjname);
  LOG("ended");
  STKOUT
  return rc;
}

APIRET CDECL
kalError(ULONG error)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("error=%x", error);
  rc = os2server_dos_Error_call (&os2srv, error, &env);
  STKOUT
  return rc;
}


//#define PAG_COMMIT   0x00000010
//#define OBJ_TILE     0x00000040

//#define PAG_EXECUTE  0x00000004
//#define PAG_READ     0x00000001
//#define PAG_WRITE    0x00000002
//#define PAG_GUARD    0x00000008

typedef struct
{
  char name[256];        // name for named shared mem
  l4_uint32_t   rights;  // OS/2-style access flags
  l4_uint32_t   area;    // area id
} vmdata_t;

APIRET CDECL
kalAllocMem(PVOID *ppb,
            ULONG cb,
	    ULONG flags)
{
  l4_uint32_t rights = 0;
  l4_uint32_t area;
  l4dm_dataspace_t ds;
  l4_addr_t addr;
  vmdata_t  *ptr;
  int rc;

  STKIN
  LOG("cb=%d", cb);
  LOG("flags=%x", flags);

  if (flags & PAG_READ)
    rights |= L4DM_READ;

  if (flags & PAG_WRITE)
    rights |= L4DM_WRITE;

  if (flags & PAG_EXECUTE)
    rights |= L4DM_READ;

  LOG("0");
  rc = l4rm_area_reserve(cb, 0, &addr, &area);
  LOG("1");

  if (rc < 0)
  {
    STKOUT
    switch (-rc)
    {
      case L4_ENOMEM:
      case L4_ENOTFOUND:
        return 8; /* ERROR_NOT_ENOUGH_MEMORY */
      default:
        return ERROR_ACCESS_DENIED;
    }
  }

  LOG("2");
  ptr = (vmdata_t *)malloc(sizeof(vmdata_t));
  LOG("3");
  l4rm_set_userptr(addr, ptr);

  ptr->rights = (l4_uint32_t)flags;
  ptr->area   = area;

  LOG("4");
  if (flags & PAG_COMMIT)
  {
    /* Create a dataspace of a given size */
    LOG("5");
    rc = l4dm_mem_open(L4DM_DEFAULT_DSM, cb,
               4096, rights, "DosAllocMem dataspace", &ds);

    if (rc < 0)
    {
      STKOUT
      return 8; /* ERROR_NOT_ENOUGH_MEMORY */
    }

    /* attach the created dataspace to our address space */
    //rc = attach_ds(&ds, rights, &addr);
    LOG("6");
    rc = attach_ds_area(ds, area, rights, addr);

    //enter_kdebug(">");
    if (rc)
    {
      STKOUT
      return 8; /* What to return? */
    }
  }

  LOG("7");
  *ppb = (void *)addr;

  LOG("*ppb=%x", addr);
  //enter_kdebug(">");
  STKOUT
  return 0; /* NO_ERROR */
}

APIRET CDECL
kalFreeMem(PVOID pb)
{
  CORBA_Environment env = dice_default_environment;
  vmdata_t *ptr;
  l4_addr_t addr;
  l4_size_t size;
  l4_offs_t offset;
  l4_threadid_t pager;
  l4dm_dataspace_t ds;
  int rc, ret;

  STKIN
  LOG("pb=%x", pb);
  
  ret = l4rm_lookup_region((l4_addr_t)pb, &addr, &size, &ds,
                     &offset, &pager);
  
  switch (ret)
  {
    case L4RM_REGION_RESERVED:
      break;
    case L4RM_REGION_DATASPACE:
      rc = l4rm_detach(addr);
      if (ret)
      {
        STKOUT
        return 5; /* ERROR_ACCESS_DENIED */
      }
      break;
    default:
      STKOUT
      return ERROR_INVALID_ADDRESS;
  }
    
  rc = l4rm_area_release(addr);
  
  if (rc)
  {
    STKOUT
    return ERROR_ACCESS_DENIED;
  }

  if (ptr = l4rm_get_userptr(addr)) 
  {
    if (ptr->rights & PAG_SHARED)
      os2exec_release_sharemem_call(&execsrv, addr, &env);
    
    l4rm_area_release(ptr->area);    
  }
  
  if (ret == L4RM_REGION_DATASPACE)
  {
    rc = l4dm_close(&ds);
  
    if (rc)
    {
      STKOUT
      return 5; /* ERROR_ACCESS_DENIED */      
    }
  }

  STKOUT
  return 0; /* NO_ERROR */
}


APIRET CDECL
kalSetMem(PVOID pb,
          ULONG cb,
	  ULONG flags)
{
  CORBA_Environment env = dice_default_environment;
  l4_uint32_t area;
  vmdata_t *ptr;
  l4_uint32_t rights = 0;
  l4_addr_t addr;
  l4_size_t size;
  l4_offs_t offset;
  l4_threadid_t pager;
  l4dm_dataspace_t ds;
  int rc, ret;

  STKIN
  LOG("pb=%x", pb);
  LOG("cb=%u", cb);
  LOG("flags=%x", flags);
  
  if (flags & PAG_READ)
    rights |= L4DM_READ;

  if (flags & PAG_WRITE)
    rights |= L4DM_WRITE;

  if (flags & PAG_EXECUTE)
    rights |= L4DM_READ;
  
  rc = l4rm_lookup_region((l4_addr_t)pb, &addr, &size, &ds,
                     &offset, &pager);
		       
  switch (rc)
  {
    case L4RM_REGION_RESERVED:
      break;
    case L4RM_REGION_DATASPACE:
      rc = l4rm_detach(addr);
      if (rc)
      {
        STKOUT
        return ERROR_ACCESS_DENIED;
      }
      break;
    default:
      STKOUT
      return ERROR_INVALID_ADDRESS;
  }

  ptr = l4rm_get_userptr(addr);  
  if (ptr) area = ptr->area;

  if (rc == L4RM_REGION_RESERVED)
  {
    /* Create a dataspace of a given size */
    ret = l4dm_mem_open(L4DM_DEFAULT_DSM, cb,
               4096, rights, "DosAllocMem dataspace", &ds);

    //enter_kdebug(">");
    if (ret < 0)
    {
      STKOUT
      return 8; /* ERROR_NOT_ENOUGH_MEMORY */
    }
  }
  else if (rc == L4RM_REGION_DATASPACE)
  {
    /* decommit memory */
    ret = l4rm_detach(addr);
  
    if (ret)
    {
      STKOUT
      return 5; /* ERROR_ACCESS_DENIED */
    }
  }
  
  if (!(flags & PAG_DECOMMIT))
  {
    if (flags & PAG_DEFAULT)
    {
      rc = l4dm_mem_resize(&ds, cb);

      if (rc)
      {
        switch (-rc)
        {
  	  case L4_ENOMEM:
	    return ERROR_NOT_ENOUGH_MEMORY;
	  default:
	    return ERROR_ACCESS_DENIED;
        }
      }
    }

    if (flags & PAG_COMMIT)
    {
      /* attach the created dataspace to our address space */
      //rc = attach_ds_reg(ds, rights, addr);
      rc = attach_ds_area(ds, area, rights, addr);

      if (rc)
      {
        STKOUT
        return 8; /* What to return? */
      }
    }
  }

  STKOUT
  return rc;
}

APIRET CDECL
kalQueryMem(PVOID  pb,
            PULONG pcb,
	    PULONG pflags)
{
  CORBA_Environment env = dice_default_environment;
  l4_uint32_t area;
  vmdata_t *ptr;
  l4_uint32_t rights = 0;
  l4_addr_t addr;
  l4_size_t size = 0;
  l4_offs_t offset;
  l4_threadid_t pager;
  l4dm_dataspace_t ds;
  int rc, ret;

  STKIN
  rc = l4rm_lookup_region((l4_addr_t)pb, &addr, &size, &ds,
                     &offset, &pager);
  
  switch (rc)
  {
    case L4RM_REGION_RESERVED:
    case L4RM_REGION_DATASPACE:
      if (ptr = l4rm_get_userptr(addr)) 
	rights = ptr->rights;
      break;
    case L4RM_REGION_FREE:
      rights = PAG_FREE; 
      break;
    default:
      STKOUT
      return ERROR_INVALID_ADDRESS;
  }

  if ((l4_addr_t)pb - addr <= L4_PAGESIZE)
    rights |= PAG_BASE;

  *pcb = size;
  *pflags = rights;

  STKOUT
  return rc;
}

APIRET CDECL
kalAllocSharedMem(PPVOID ppb,
                  PSZ    pszName,
		  ULONG  cb,
		  ULONG  flags)
{
  CORBA_Environment env = dice_default_environment;
  l4_uint32_t rights = 0;
  l4_uint32_t area;
  l4dm_dataspace_t ds;
  l4_addr_t addr;
  vmdata_t  *ptr;
  int rc;

  STKIN
  if (flags & PAG_READ)
    rights |= L4DM_READ;

  if (flags & PAG_WRITE)
    rights |= L4DM_WRITE;

  if (flags & PAG_EXECUTE)
    rights |= L4DM_READ;

  rc = os2exec_alloc_sharemem_call (&execsrv, cb, 0, &addr, &env);

  if (rc)
  {
    STKOUT
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  rc = l4rm_area_reserve_region(addr, cb, 0, &area);

  if (rc < 0)
  {
    STKOUT
    switch (-rc)
    {
      case L4_ENOMEM:
      case L4_ENOTFOUND:
        return ERROR_NOT_ENOUGH_MEMORY;
      default:
        return ERROR_ACCESS_DENIED;
    }
  }

  ptr = (vmdata_t *)malloc(sizeof(vmdata_t));
  l4rm_set_userptr(addr, ptr);

  ptr->rights = (l4_uint32_t)flags;
  ptr->rights |= PAG_SHARED;
  ptr->area   = area;
  if (pszName) strcpy(ptr->name, pszName);

  if (flags & PAG_COMMIT)
  {
    /* Create a dataspace of a given size */
    rc = l4dm_mem_open(L4DM_DEFAULT_DSM, cb,
               4096, rights, "DosAllocMem dataspace", &ds);

    if (rc < 0)
    {
      STKOUT
      return 8; /* ERROR_NOT_ENOUGH_MEMORY */
    }

    /* attach the created dataspace to our address space */
    //rc = attach_ds(&ds, rights, &addr);
    rc = attach_ds_area(ds, area, rights, addr);

    //enter_kdebug(">");
    if (rc)
    {
      STKOUT
      return 8; /* What to return? */
    }
  }

  *ppb = (void *)addr;

  LOG("*ppb=%x", addr);
  STKOUT
  return rc;
}

APIRET CDECL
kalResetBuffer(HFILE handle)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("handle=%x", handle);
  rc = os2fs_dos_ResetBuffer_call (&fs, handle, &env);
  STKOUT
  return rc;
}

APIRET CDECL
kalSetFilePtrL(HFILE handle,
               LONGLONG ib,
	       ULONG method,
	       PULONGLONG ibActual)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("handle=%x", handle);
  LOG("ib=%d", ib);
  LOG("method=%x", method);
  rc = os2fs_dos_SetFilePtrL_call (&fs, handle, ib,
                                  method, ibActual, &env);
  LOG("*ibActual=%d", *ibActual);
  LOG("rc=%u", rc);
  STKOUT
  return rc;
}

APIRET CDECL
kalClose(HFILE handle)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("handle=%x", handle);
  rc = os2fs_dos_Close_call (&fs, handle, &env);
  STKOUT
  return rc;
}

APIRET CDECL
kalQueryHType(HFILE handle,
              PULONG pType,
	      PULONG pAttr)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("handle=%x", handle);
  rc = os2fs_dos_QueryHType_call(&fs, handle, pType, pAttr, &env);
  LOG("Type=%x", *pType);
  LOG("Attr=%x", *pAttr);
  STKOUT
  return rc;
}

APIRET CDECL
kalQueryDBCSEnv(ULONG cb,
                COUNTRYCODE *pcc,
		PBYTE pBuf)
{
  CORBA_Environment env = dice_default_environment;
  int rc;
  STKIN
  LOG("cb=%u", cb);
  LOG("pcc=%x", pcc);
  rc = os2server_dos_QueryDBCSEnv_call (&os2srv, &cb, pcc, &pBuf, &env);
  LOG("pBuf=%x", pBuf);
  STKOUT
  return rc;
}

APIRET CDECL
kalQueryCp(ULONG cb,
           PULONG arCP,
	   PULONG pcCP)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;
  //enter_kdebug(">");
  STKIN
  LOG("cb=%u", cb);
  rc = os2server_dos_QueryCp_call (&os2srv, &cb, &arCP, &env);
  //enter_kdebug(">");
  *pcCP = cb;
  LOG("arCP=%x", arCP);
  LOG("pcCP=%x", pcCP);
  LOG("*arCP=%u", *arCP);
  LOG("*pcCP=%u", *pcCP);
  STKOUT
  return rc;
}

APIRET CDECL
kalGetInfoBlocks(PTIB *pptib, PPIB *pppib)
{
  CORBA_Environment env = dice_default_environment;
  l4dm_dataspace_t ds;
  l4_addr_t addr;
  l4_offs_t tib_offs, pib_offs;
  APIRET rc;

  STKIN
  /* get the dataspace with info blocks */
  rc = os2server_dos_GetInfoBlocks_call (&os2srv, &ds,
                           &tib_offs, &pib_offs, &env);

  if (rc)
  {
    STKOUT
    return rc;
  }
 
  /* attach it */ 
  rc = attach_ds(&ds, L4DM_RW, &addr);
  if (rc)
  {
    LOG("error attaching ds!");
    STKOUT
    return rc;
  }
  else
    LOG("ds attached");

  *pptib = (PTIB)((char *)addr + tib_offs);
  *pppib = (PPIB)((char *)addr + pib_offs);

  (*pptib)->tib_ptib2 = (PTIB2)((char *)(*pptib)->tib_ptib2 + addr);
  
  /* fixup fields */
  (*pppib)->pib_pchcmd = (char *)((*pppib)->pib_pchcmd) + addr;
  (*pppib)->pib_pchenv = (char *)((*pppib)->pib_pchenv) + addr;

  /* detach dataspace */
  rc = l4rm_detach(addr);
  
  if (rc)
  {
    LOG("error detaching ds!");
    STKOUT
    return rc;
  }
  else
    LOG("ds detached");

  /* then attach it readonly */
  rc = attach_ds_reg(ds, L4DM_RO, addr);
  
  if (rc)
  {
    LOG("error attaching ds ro!");
    STKOUT
    return rc;
  }
  else
    LOG("ds attached again ro");

  LOG("*pptib=%x", *pptib);
  LOG("*pppib=%x", *pppib);
  STKOUT
  return 0; /* NO_ERROR */
}

APIRET CDECL
kalSetMaxFH(ULONG cFH)
{
  CurMaxFH = cFH;
  LOG("cFH=%u", cFH);

  return 0; /* NO_ERROR */
}

APIRET CDECL
kalSetRelMaxFH(PLONG pcbReqCount, PULONG pcbCurMaxFH)
{
  CurMaxFH += *pcbReqCount;
  *pcbCurMaxFH = CurMaxFH;
  LOG("*pcbReqCount=%d", *pcbReqCount);
  LOG("CurMaxFH=%u", CurMaxFH);

  return 0; /* NO_ERROR */
}

APIRET CDECL
kalSleep(ULONG ms)
{
  STKIN
  LOG("ms=%u", ms);
  l4_sleep(ms);
  STKOUT

  return 0; /* NO_ERROR */
}

APIRET CDECL
kalDupHandle(HFILE hFile, HFILE *phFile2)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("hFile=%x", hFile);
  rc = os2fs_dos_DupHandle_call(&fs, hFile, phFile2, &env);
  LOG("*phFile2=%x", *phFile2);
  STKOUT
  return rc; /* NO_ERROR */
}

APIRET CDECL
kalDelete(PSZ pszFileName)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("pszFileName=%s", pszFileName);
  rc = os2fs_dos_Delete_call (&fs, pszFileName, &env);
  STKOUT
  return rc; /* NO_ERROR */
}

APIRET CDECL
kalForceDelete(PSZ pszFileName)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("pszFileName=%s", pszFileName);
  rc = os2fs_dos_ForceDelete_call (&fs, pszFileName, &env);
  STKOUT
  return rc; /* NO_ERROR */
}

APIRET CDECL
kalDeleteDir(PSZ pszDirName)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("pszDirName=%s", pszDirName);
  rc = os2fs_dos_DeleteDir_call (&fs, pszDirName, &env);
  STKOUT
  return rc; /* NO_ERROR */
}

APIRET CDECL
kalCreateDir(PSZ pszDirName, PEAOP2 peaop2)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("pszDirName=%s", pszDirName);
  LOG("peaop2=%x", peaop2);
  rc = os2fs_dos_CreateDir_call (&fs, pszDirName, peaop2, &env);
  STKOUT
  return rc; /* NO_ERROR */
}


APIRET CDECL
kalFindFirst(char  *pszFileSpec,
             HDIR  *phDir,
             ULONG flAttribute,
             PVOID pFindBuf,
             ULONG cbBuf,
             ULONG *pcFileNames,
             ULONG ulInfolevel)
{
  CORBA_Environment env = dice_default_environment;
  char  buf[256];
  char  str[256];
  ULONG disk, map;
  char  drv;
  int   len = 0, i;
  char  *s;
  APIRET rc;

  STKIN
  LOG("pszFileSpec=%s", pszFileSpec);
  LOG("*phDir=%d", *phDir);
  LOG("flAttribute=%x", flAttribute);
  LOG("ulInfolevel=%d", ulInfolevel);
  LOG("pFindBuf=%x", pFindBuf);
  LOG("cbBuf=%u", cbBuf);

  /* if no path specified, add the current dir */
  if (pszFileSpec[1] != ':')
  {
    /* query current disk */
    rc = kalQueryCurrentDisk(&disk, &map);
    drv = disk - 1 + 'A';
  
    len = 0; buf[0] = '\0';
    if (pszFileSpec[0] != '\\')
    {
      /* query current dir  */
      rc = kalQueryCurrentDir(0, buf, &len);
      rc = kalQueryCurrentDir(0, buf, &len);
    }

    if (len + strlen(pszFileSpec) + 3 > 256)
      return ERROR_FILENAME_EXCED_RANGE;

    i = 0;
    str[i++] = drv;
    str[i++] = ':';
    str[i] = '\0';

    if (pszFileSpec[0] != '\\') 
    {
      str[i++] = '\\';
      str[i] = '\0';
      strcat(str, buf);
      
      if (str[len + i - 2] != '\\') 
      {
        str[len + i - 1] = '\\';
        len++;
      }
      str[len + i - 1] = '\0';
    }

    s = strcat(str, pszFileSpec);
  }
  else
    s = pszFileSpec;

  LOG("buf=%s", buf);
  LOG("filespec=%s", s);
  rc = os2fs_dos_FindFirst_call(&fs, s, phDir,
                                flAttribute, &pFindBuf, &cbBuf,
                                pcFileNames, ulInfolevel, &env);
  LOG("*pcFileNames=%u", *pcFileNames);
  LOG("pFindBuf=%x", pFindBuf);
  LOG("cbBuf=%u", cbBuf);
  LOG("filename=%s", ((FILEFINDBUF3 *)pFindBuf)->achName);
  LOG("achname offset=%u", (ULONG)&(((PFILEFINDBUF3)pFindBuf)->achName) - (ULONG)pFindBuf);
  LOG("rc=%u", rc);
  STKOUT
  return rc;
}


APIRET CDECL
kalFindNext(HDIR  hDir,
            PVOID pFindBuf,
            ULONG cbBuf,
            ULONG *pcFileNames)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("hDir=%d", hDir);
  LOG("pFindBuf=%x", pFindBuf);
  LOG("cbBuf=%u", cbBuf);
  rc = os2fs_dos_FindNext_call(&fs, hDir, &pFindBuf,
                               &cbBuf, pcFileNames, &env);
  LOG("*pcFileNames=%d", *pcFileNames);
  LOG("pFindBuf=%x", pFindBuf);
  LOG("cbBuf=%u", cbBuf);
  LOG("filename=%s", ((FILEFINDBUF3 *)pFindBuf)->achName);
  LOG("rc=%u", rc);
  STKOUT
  return rc;
}


APIRET CDECL
kalFindClose(HDIR hDir)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("hDir=%d", hDir);
  rc = os2fs_dos_FindClose_call(&fs, hDir, &env);
  STKOUT
  return rc;
}


APIRET CDECL
kalQueryFHState(HFILE hFile,
                PULONG pMode)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("hFile=%u", hFile);
  rc = os2fs_dos_QueryFHState_call(&fs, hFile, pMode, &env);
  LOG("*pMode=%x", *pMode);
  STKOUT
  return rc;
}


APIRET CDECL
kalSetFHState(HFILE hFile,
              ULONG pMode)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("hFile=%u", hFile);
  LOG("pMode=%x", pMode);
  rc = os2fs_dos_SetFHState_call(&fs, hFile, pMode, &env);
  STKOUT
  return rc;
}

APIRET CDECL
kalQueryFileInfo(HFILE hf,
                 ULONG ulInfoLevel,
                 char *pInfo,
                 ULONG cbInfoBuf)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("hf=%x", hf);
  LOG("ulInfoLevel=%u", ulInfoLevel);
  rc = os2fs_dos_QueryFileInfo_call(&fs, hf, ulInfoLevel,
                                    &pInfo, &cbInfoBuf, &env);
  STKOUT
  return rc;
}


APIRET CDECL
kalQueryPathInfo(PSZ pszPathName,
                 ULONG ulInfoLevel,
                 PVOID pInfo,
                 ULONG cbInfoBuf)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("pszPathName=%s", pszPathName);
  LOG("ulInfoLevel=%u", ulInfoLevel);
  rc = os2fs_dos_QueryPathInfo_call(&fs, pszPathName, ulInfoLevel,
                                    &pInfo, &cbInfoBuf, &env);
  STKOUT
  return rc;
}


APIRET CDECL
kalSetFileSizeL(HFILE hFile,
                long long cbSize)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("hFile=%x", hFile);
  LOG("cbSize=%u", cbSize);
  rc = os2fs_dos_SetFileSizeL_call(&fs, hFile, cbSize, &env);
  STKOUT
  return rc;
}

APIRET CDECL
kalMove(PSZ pszOld, PSZ pszNew)
{
  CORBA_Environment env = dice_default_environment;
  APIRET rc;

  STKIN
  LOG("pszOld=%s", pszOld);
  LOG("pszNew=%s", pszNew);
  // return an error while it is unimplemented
  rc = ERROR_INVALID_PARAMETER;
  STKOUT
  return rc;
}

APIRET CDECL
kalOpenEventSem(PSZ pszName,
                PHEV phev)
{
  APIRET rc;
  SEM    *sem;

  if (phev && (pszName || *pszName))
    return ERROR_INVALID_PARAMETER;

  if (*phev)
  {
    // open by handle
    if (!HndIsValidIndexHandle(htSem, *phev, (HANDLE **)&sem))
    {
      if (sem->cType == SEMTYPE_EVENT && sem->cShared)
      {
        if (!sem->ulRefCnt)
	  return ERROR_TOO_MANY_OPENS;

        // increment refcount
        sem->ulRefCnt++;
	
        return NO_ERROR;
      }
    }
    else
      return ERROR_INVALID_HANDLE;
  }
  else
  {
    // open by name
    if (strstr(pszName, "\\SEM32\\") != pszName)
      return ERROR_INVALID_NAME;
      
    for (sem = (SEM *)(htSem->pFirstHandle); sem; sem = sem->pNext)
      if (sem->szName && !strcmp(sem->szName, pszName))
        break;
	
    if (sem)
    {
      if (!sem->ulRefCnt)
        return ERROR_TOO_MANY_OPENS;
  
      // increment refcount
      sem->ulRefCnt++;

      *phev = (ULONG)(((PCHAR)sem - (PCHAR)htSem->pFirstHandle) / htSem->ulHandleSize);
      return NO_ERROR;
    }
    else
      return ERROR_SEM_NOT_FOUND;
  }

  return NO_ERROR;
}

APIRET CDECL
kalCloseEventSem(HEV hev)
{

}

APIRET CDECL
kalCreateEventSem(PSZ pszName,
                  PHEV phev,
		  ULONG flags,
		  BOOL32 fState)
{
  APIRET rc;
  HEV    hev;
  SEM    *sem;

  STKIN

  if (fState > 1 || flags > DC_SEM_SHARED)
    return ERROR_INVALID_PARAMETER;

  if (pszName && *pszName && strstr(pszName, "\\SEM32\\") != pszName)
    return ERROR_INVALID_NAME;

  if (!kalOpenEventSemaphore(pszName, &hev))
  {
    kalCloseEventSem(hev);
    return ERROR_DUPLICATE_NAME;
  }

  /* allocate a hev for a new event semaphore */
  if (rc = HndAllocateHandle(htSem, &hev, (HANDLE **)&sem))
    return rc;

  if (!pszName || !*pszName)
  {
    // shared semaphore
    sem->cShared = flags;
  }
  else
  {
    // set name  
    strncpy(sem->szName, pszName, CCHMAXPATH);
    sem->szName[CCHMAXPATH - 1] = '\0';
    sem->cShared = TRUE;
  }

  sem->cType = SEMTYPE_EVENT;

  // set initial state
  sem->uSem.evt  = L4SEMAPHORE_INIT(fState);
  sem->ulRefCnt = 1;
  
  // return handle
  *phev = hev;
  STKOUT
  return NO_ERROR;
}
