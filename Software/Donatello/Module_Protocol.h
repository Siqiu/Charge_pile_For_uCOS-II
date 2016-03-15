/**
  ******************************************************************************
  * @file    Module_Protocol.h
  * @author  Donatello
  * @version V1.0
  * @date    2015.9.16
  * @brief
  ******************************************************************************
  */
#ifndef   __MODULE_PROTOCOL_H__
#define   __MODULE_PROTOCOL_H__
#include "includes.h"

/* Exported valable -------------------------------------------------------- */
typedef enum {
	PW_OK = 0,				/* (0) Succeeded */
	PW_HEAD_1_ERR,			/* (1) check frame head error */
	PW_ADDR_ERR,			/* (2) check frame address error */
	PW_HEAD_2_ERR,			/* (3) The physical drive cannot work */
	PW_OPERATIONCODE_ERR,	/* (4) Could not find the file */
	PW_NO_PATH,				/* (5) Could not find the path */
	PW_INVALID_NAME,		/* (6) The path name format is invalid */
	PW_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	PW_EXIST,				/* (8) Access denied due to prohibited access */
	PW_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	PW_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	PW_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	PW_NOT_ENABLED,			/* (12) The volume has no work area */
	PW_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	PW_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any parameter error */
	PW_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	PW_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	PW_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	PW_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_LOCK */
	PW_INVALID_PARAMETER	/* (19) Given parameter is invalid */
} PWRESULT;

/* Public functions ---------------------------------------------------------*/


void UardDmaFlow(void);

/* private functions---------------------------------------------------------*/


#endif    /* __MODULE_PROTOCOL_H__ */
