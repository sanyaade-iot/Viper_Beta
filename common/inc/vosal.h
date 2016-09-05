/*
{{ project }}
Copyright (C) {{ year }}  {{ organization }}

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __VOSAL__
#define __VOSAL__

#include "viperlib.h"
/* Viper Operative System Abstraction Layer */
#include "vboard.h"


/* MACROS */

#define VTIME_IMMEDIATE  ((uint32_t)0)
#define VTIME_INFINITE  ((uint32_t)-1)
#define VRES_OK 0
#define VRES_TIMEOUT -1
#define VRES_RESET -2

/* TYPES */
typedef void *VThread;
typedef void *VSemaphore;
typedef void *VMutex;
typedef void *VSysTimer;
typedef void *VMailBox;
typedef void (*vsystimer_fn)(void *);
typedef void (*vos_irq_handler)(void);

typedef struct {
  uint32_t      *init_stack;
  vos_irq_handler  reset_vector;
  vos_irq_handler  nmi_vector;
  vos_irq_handler  hardfault_vector;
  vos_irq_handler  memmanage_vector;
  vos_irq_handler  busfault_vector;
  vos_irq_handler  usagefault_vector;
  vos_irq_handler  vector1c;
  vos_irq_handler  vector20;
  vos_irq_handler  vector24;
  vos_irq_handler  vector28;
  vos_irq_handler  svcall_vector;
  vos_irq_handler  debugmonitor_vector;
  vos_irq_handler  vector34;
  vos_irq_handler  pendsv_vector;
  vos_irq_handler  systick_vector;
  vos_irq_handler  vectors[CORTEX_VECTOR_COUNT];
} vos_vectors;


/* STATIC */
extern VSemaphore _memlock;
extern VSemaphore _dbglock;
extern VSemaphore _gillock;
extern volatile uint32_t _systime_seconds;
extern volatile uint32_t _systime_millis;
extern volatile uint32_t _system_frequency;


#define VOS_PRIO_IDLE 0
#define VOS_PRIO_LOWEST 1
#define VOS_PRIO_LOWER 2
#define VOS_PRIO_LOW 3
#define VOS_PRIO_NORMAL 4
#define VOS_PRIO_HIGH 5
#define VOS_PRIO_HIGHER 6
#define VOS_PRIO_HIGHEST 7


/* SYSTEM */
void vosInit(void *data);
void vosSysLock(void);
void vosSysUnlock(void);
void vosSysLockIsr(void);
void vosSysUnlockIsr(void);
//isr drama
extern void _port_irq_epilogue(void);
#define vosEnterIsr()
#define vosExitIsr() _port_irq_epilogue()

uint32_t time_to_ticks(uint32_t time);
uint32_t* vosTicks(void);
uint32_t vosMillis(void);
vos_irq_handler vosInstallHandler(uint32_t hpos, vos_irq_handler fn);

/* Threads */

#define VTHREAD_READY      0
#define VTHREAD_RUNNING    1
#define VTHREAD_WAITING    2
#define VTHREAD_INACTIVE   3

VThread vosThCreate(uint32_t size, int32_t prio, void *fn, void *arg, void *data);
void vosThDestroy(VThread th);
void vosThSetData(VThread th, void *data);
void *vosThGetData(VThread th);
uint32_t vosThGetId(VThread th);
uint32_t vosThGetStatus(VThread th);
void vosThSetPriority(int32_t prio);
int32_t vosThGetPriority(void);
VThread vosThCurrent(void);
void vosThSleep(uint32_t time);
void vosThYield(void);
void vosThSuspend(void);
VThread vosThResume(VThread th);
VThread vosThResumeIsr(VThread th);
void vosThReschedule(void);

/* Semaphores */
VSemaphore vosSemCreate(uint32_t n);
void vosSemReset(VSemaphore sem);
int32_t vosSemGetValue(VSemaphore sem);
void vosSemSignal(VSemaphore sem);
void vosSemSignalCap(VSemaphore sem, uint32_t cap);
void vosSemSignalIsr(VSemaphore sem);
int32_t vosSemWaitTimeout(VSemaphore sem, uint32_t timeout);
#define vosSemWait(sem) vosSemWaitTimeout(sem,VTIME_INFINITE);
void vosSemDestroy(VSemaphore sem);


/* Mutexes */
#ifndef VM_NO_MUTEXES
VMutex vosMtxCreate(void);
void vosMtxLock(VMutex mtx);
int32_t vosMtxTryLock(VMutex mtx);
VMutex vosMtxUnlock(VMutex mtx);
void vosMtxDestroy(VMutex mtx);
#endif

/* Mailboxes */
VMailBox vosMBoxCreate(int n);
void vosMBoxDestroy(VMailBox mb);
int vosMBoxPostTimeout(VMailBox mb, void *msg, uint32_t timeout);
int vosMBoxFetchTimeout(VMailBox mb, void **msgp, uint32_t timeout);
#define vosMBoxPost(mb,msgp) vosMBoxPostTimeout(mb,msgp,VTIME_INFINITE)
#define vosMBoxFetch(mb,msgp) vosMBoxFetchTimeout(mb,msgp,VTIME_INFINITE)
int vosMBoxPostIsr(VMailBox mb, void *msg);
int vosMBoxFetchIsr(VMailBox mb, void **msgp);
int vosMBoxUsedSlots(VMailBox mb);
int vosMBoxFreeSlots(VMailBox mb);



/* Timers */
#define ticks_to_micros(tk) ((tk)/(_system_frequency/1000000))
#define ticks_to_millis(tk) ((tk)/(_system_frequency/1000))
#define ticks_to_seconds(tk) ((tk)/(_system_frequency))
VSysTimer vosTimerCreate(void);
uint32_t vosTimerReadMillis(VSysTimer tm);
uint32_t vosTimerReadMicros(VSysTimer tm);
void vosTimerOneShot(VSysTimer tm, uint32_t time, vsystimer_fn fn, void *arg);
void vosTimerRecurrent(VSysTimer tm, uint32_t time, vsystimer_fn fn, void *arg);
void vosTimerReset(VSysTimer tm);
void vosTimerDestroy(VSysTimer tm);
void sleep_polling(uint32_t time);

/* Queues */
typedef void *VQueue;
VQueue vosQueueCreate(uint32_t size);
int vosQueueRead(VQueue q, uint8_t *buf, uint32_t len);
int vosQueueWrite(VQueue q, uint8_t *buf, uint32_t len);


#endif