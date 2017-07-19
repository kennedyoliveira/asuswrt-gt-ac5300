/* 
* <:copyright-BRCM:2002:proprietary:standard
* 
*    Copyright (c) 2002 Broadcom 
*    All Rights Reserved
* 
*  This program is the proprietary software of Broadcom and/or its
*  licensors, and may only be used, duplicated, modified or distributed pursuant
*  to the terms and conditions of a separate, written license agreement executed
*  between you and Broadcom (an "Authorized License").  Except as set forth in
*  an Authorized License, Broadcom grants no license (express or implied), right
*  to use, or waiver of any kind with respect to the Software, and Broadcom
*  expressly reserves all rights in and to the Software and all intellectual
*  property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
*  NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
*  BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
* 
*  Except as expressly set forth in the Authorized License,
* 
*  1. This program, including its structure, sequence and organization,
*     constitutes the valuable trade secrets of Broadcom, and you shall use
*     all reasonable efforts to protect the confidentiality thereof, and to
*     use this information only in connection with your use of Broadcom
*     integrated circuit products.
* 
*  2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*     AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*     WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*     RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
*     ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
*     FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
*     COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
*     TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
*     PERFORMANCE OF THE SOFTWARE.
* 
*  3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
*     ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*     INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
*     WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*     IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
*     OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*     SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
*     SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
*     LIMITED REMEDY.
* :>
*/

/****************************************************************************
 *
 * AdslCoreFrame.c -- Frame allcation/freeing functions
 *
 * Description:
 *
 * Authors: Ilya Stomakhin
 *
 * $Revision: 1.1 $
 *
 * $Id: AdslCoreFrame.c,v 1.1 2004/04/08 21:24:49 ilyas Exp $
 *
 * $Log: AdslCoreFrame.c,v $
 * Revision 1.1  2004/04/08 21:24:49  ilyas
 * Initial CVS checkin. Version A2p014
 *
 *
 *****************************************************************************/

#include "softdsl/SoftDsl.h"
#include "softdsl/BlankList.h"

#include "AdslCoreFrame.h"

#if defined(__KERNEL__) || defined(TARG_OS_RTEMS) || defined(_CFE_) || defined(_NOOS)
#include "BcmOs.h"
#else
#include <stdlib.h>
#endif

Public void * AdslCoreFrameAllocMemForFrames(ulong frameNum)
{
	void	**pFreeFrameList;
	uchar	*frPtr;
	ulong	i;

	/* allocate memory to hold the head and frameNum dslFrame buffers */

	pFreeFrameList = (void *) calloc (1, sizeof (void *) + sizeof(dslFrame) * frameNum);
	if (NULL == pFreeFrameList)
		return NULL;

	/* make a list of free dslFrame buffers */

	*pFreeFrameList = NULL;
	frPtr = (uchar *) (pFreeFrameList + 1) + sizeof(dslFrame) * (frameNum - 1);
	for (i = 0; i < frameNum; i++) {
		BlankListAdd(pFreeFrameList, (void*)frPtr);
		frPtr -= sizeof(dslFrame);
	}
	return pFreeFrameList;
}

Public void AdslCoreFrameFreeMemForFrames(void *hMem)
{
	free (hMem);
}

Public dslFrame * AdslCoreFrameAllocFrame(void *handle)
{
	dslFrame	*pFr;

	pFr = BlankListGet(handle);
	return pFr;
}

Public void AdslCoreFrameFreeFrame(void *handle, dslFrame *pFrame)
{
	BlankListAdd(handle, pFrame);
}

Public void * AdslCoreFrameAllocMemForBuffers(void **ppMemPool, ulong bufNum, ulong memSize)
{
	void	**pFreeBufList;
	uchar	*frPtr;
	ulong	i;
	
	/* allocate memory to hold the head and bufNum dslFrame buffers */

	pFreeBufList = (void *) calloc (1, sizeof (void *) + sizeof(dslFrameBuffer) * bufNum + memSize);
	if (NULL == pFreeBufList)
		return NULL;

	/* make a list of free dslFrame buffers */

	*pFreeBufList = NULL;
	frPtr = (uchar *) (pFreeBufList + 1) + sizeof(dslFrameBuffer) * (bufNum - 1);
	for (i = 0; i < bufNum; i++) {
		BlankListAdd(pFreeBufList, (void*)frPtr);
		frPtr -= sizeof(dslFrameBuffer);
	}

	*ppMemPool = (void *) pFreeBufList;
	return (uchar *) (pFreeBufList + 1) + sizeof(dslFrameBuffer) * bufNum;
}

Public void AdslCoreFrameFreeMemForBuffers(void *hMem, ulong memSize, void *pMemPool)
{
	free (pMemPool);
}

Public dslFrameBuffer * AdslCoreFrameAllocBuffer(void *handle, void *pMem, ulong length)
{
	dslFrameBuffer	*pBuf;

	pBuf = BlankListGet(handle);
	pBuf->pData  = pMem;
	pBuf->length = length;

	return pBuf;
}

Public void AdslCoreFrameFreeBuffer(void *handle, dslFrameBuffer *pBuf)
{
	BlankListAdd(handle, pBuf);
}

Public ulong AdslCoreFrame2Id(void *handle, dslFrame *pFrame)
{
	return ((uchar *)pFrame - (uchar *)handle - sizeof (void *)) / sizeof(dslFrame);
}

Public void * AdslCoreFrameId2Frame(void *handle, ulong frameId)
{
	return (uchar *)handle + sizeof(void *) + sizeof(dslFrame)*frameId;
}