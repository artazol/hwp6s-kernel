/****************************************************************************
*
*    Copyright (c) 2005 - 2013 by Vivante Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Vivante Corporation. This is proprietary information owned by
*    Vivante Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Vivante Corporation.
*
*****************************************************************************/


#ifndef __gc_hal_user_buffer_h_
#define __gc_hal_user_buffer_h_


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************\
************************ Command Buffer and Event Objects **********************
\******************************************************************************/

/* The number of context buffers per user. */
#define gcdCONTEXT_BUFFER_COUNT 2

/* State delta record. */
typedef struct _gcsSTATE_DELTA_RECORD * gcsSTATE_DELTA_RECORD_PTR;
typedef struct _gcsSTATE_DELTA_RECORD
{
    /* State address. */
    gctUINT                     address;

    /* State mask. */
    gctUINT32                   mask;

    /* State data. */
    gctUINT32                   data;
}
gcsSTATE_DELTA_RECORD;

/* State delta. */
typedef struct _gcsSTATE_DELTA
{
    /* For debugging: the number of delta in the order of creation. */
#if gcmIS_DEBUG(gcdDEBUG_CODE)
    gctUINT                     num;
#endif

    /* Main state delta ID. Every time state delta structure gets reinitialized,
       main ID is incremented. If main state ID overflows, all map entry IDs get
       reinitialized to make sure there is no potential erroneous match after
       the overflow.*/
    gctUINT                     id;

    /* The number of contexts pending modification by the delta. */
    gctINT                      refCount;

    /* Vertex element count for the delta buffer. */
    gctUINT                     elementCount;

    /* Number of states currently stored in the record array. */
    gctUINT                     recordCount;

    /* Record array; holds all modified states in gcsSTATE_DELTA_RECORD. */
    gctUINT64                   recordArray;

    /* Map entry ID is used for map entry validation. If map entry ID does not
       match the main state delta ID, the entry and the corresponding state are
       considered not in use. */
    gctUINT64                   mapEntryID;
    gctUINT                     mapEntryIDSize;

    /* If the map entry ID matches the main state delta ID, index points to
       the state record in the record array. */
    gctUINT64                   mapEntryIndex;

    /* Previous and next state deltas in gcsSTATE_DELTA. */
    gctUINT64                   prev;
    gctUINT64                   next;
}
gcsSTATE_DELTA;

/* Command buffer object. */
struct _gcoCMDBUF
{
    /* The object. */
    gcsOBJECT                   object;

    /* Command buffer entry and exit pipes. */
    gcePIPE_SELECT              entryPipe;
    gcePIPE_SELECT              exitPipe;

    /* Feature usage flags. */
    gctBOOL                     using2D;
    gctBOOL                     using3D;
    gctBOOL                     usingFilterBlit;
    gctBOOL                     usingPalette;

    /* Physical address of command buffer. Just a name. */
    gctUINT32                   physical;

    /* Logical address of command buffer. */
    gctUINT64                   logical;

    /* Number of bytes in command buffer. */
    gctUINT                     bytes;

    /* Start offset into the command buffer. */
    gctUINT                     startOffset;

    /* Current offset into the command buffer. */
    gctUINT                     offset;

    /* Number of free bytes in command buffer. */
    gctUINT                     free;

    /* Location of the last reserved area. */
    gctUINT64                   lastReserve;
    gctUINT                     lastOffset;

#if gcdSECURE_USER
    /* Hint array for the current command buffer. */
    gctUINT                     hintArraySize;
    gctUINT64                   hintArray;
    gctUINT64                   hintArrayTail;
#endif

#if gcmIS_DEBUG(gcdDEBUG_CODE)
    /* Last load state command location and hardware address. */
    gctUINT64                   lastLoadStatePtr;
    gctUINT32                   lastLoadStateAddress;
    gctUINT32                   lastLoadStateCount;
#endif
};

typedef struct _gcsQUEUE
{
    /* Pointer to next gcsQUEUE structure in gcsQUEUE. */
    gctUINT64                   next;

    /* Event information. */
    gcsHAL_INTERFACE            iface;
}
gcsQUEUE;

/* Event queue. */
struct _gcoQUEUE
{
    /* The object. */
    gcsOBJECT                   object;

    /* Pointer to current event queue. */
    gcsQUEUE_PTR                head;
    gcsQUEUE_PTR                tail;

    /* List of free records. */
    gcsQUEUE_PTR                freeList;
    #define gcdIN_QUEUE_RECORD_LIMIT 16
    /* Number of records currently in queue */
    gctUINT32                   recordCount;
};

#ifdef __cplusplus
}
#endif

#endif /* __gc_hal_user_buffer_h_ */
