/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "dbg_log.h"

char *dbg_evt_type_map[DBG_EVT_MAX_EVT] = { "START",
                        "RESET",
                        "SETUP",
                        "READ",
                        "WRITE",
                        "USB_INT_START",
                        "USB_INT_END",
                        "IN",
                        "IN_SPUR",
                        "OUT",
                        "OUT_SPUR",
                        "OUT_DMA",
                        "OUT_DMA_END",
                        "DMA_INT_START",
                        "DMA_INT_SPUR",
                        "DMA_INT_IN",
                        "DMA_INT_OUT",
                        "DMA_INT_END",
                        "REQ_LODGE",
                        "REQ_LODGE_DMA",
                        "REQ_REMOVE",
                        "REQ_REMOVE_DMA",
                        "CLR_OUTPKTRDY",
                        "OUTCOUNT",
                        "REQLEN",
                        "ACTLEN",
                        "SETUP_IDLE",
                        "SETUP_NODATA",
                        "SETUP_DATA_OUT",
                        "SETUP_DATA_IN",
                        "SETUP_END",
                        "SENT_STALL",
                        "ACKSTAT",
                        "TRIGGER" };



typedef struct {
  uint32_t seqno;
  uint32_t time;
  dbg_evt_type_t evt;
  uint32_t data_ptr;
  char text[32];
} dbg_log_entry_t;


/* Global logfile */
#define LOGSIZE 512
dbg_log_entry_t dbg_log[LOGSIZE];
unsigned int dbg_head = 0;
unsigned int dbg_tail = 0;

int dbg_log_init(void)
{
  dbg_head = dbg_tail = 0;
  bzero(dbg_log, LOGSIZE * sizeof(dbg_log_entry_t));

  return 0;
}

int dbg_log_add(uint32_t t, dbg_evt_type_t e, uint32_t e_p, char *txt)
{
  if (e >= DBG_EVT_MAX_EVT) {
    return -1;
  }

  /* Tail always points to next item to be used */
  dbg_log[dbg_tail].time = t;
  dbg_log[dbg_tail].evt = e;
  dbg_log[dbg_tail].data_ptr = e_p;
  strncpy(dbg_log[dbg_tail].text, txt, 32);

  if (dbg_head == dbg_tail) {
    /* Empty list */
    dbg_log[dbg_tail].seqno = 0;
    dbg_tail++;
  } else {
    if (!dbg_tail) {
      dbg_log[dbg_tail].seqno = dbg_log[LOGSIZE-1].seqno + 1;
    } else {
      dbg_log[dbg_tail].seqno = dbg_log[dbg_tail-1].seqno + 1;
    }
    dbg_tail = (dbg_tail+1) % LOGSIZE;
    if (dbg_tail == dbg_head) {
      /* Move head up one */
      dbg_head = (dbg_tail+1) % LOGSIZE;
    }
  }

  return 0;
}

/* num > 0: print that number of recent events (backwards), otherwise print entire log (forwards) */
void dbg_log_print(int num)
{
  unsigned int x;
  uint8_t y[33];

  printf("------------------ Debug log ------------------\n");
  if (num > 0) {
    printf("Not yet implemented\n");
  } else {
    for (x = dbg_head; x != dbg_tail; x = (x+1) % LOGSIZE) {
      memcpy(y, dbg_log[x].text, 32);
      y[32] = 0;
      printf("Seq %04u (@ %04u) Event=%16s Data=0x%08x Text=[ %s ]\n",
         dbg_log[x].seqno, dbg_log[x].time,
         dbg_evt_type_map[dbg_log[x].evt],
         dbg_log[x].data_ptr, y);
    }
  }
  printf("-----------------------------------------------\n");
}
