/****************************************************************************
**
**  Name:       NfcAdaptation.h
**
**  Function    This file contains platform-specific NFC adaptation logic
**
**  Copyright (c) 2011-2012, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#pragma once
#include <pthread.h>
#ifndef UINT32
typedef unsigned long   UINT32;
#endif
#include "nfc_hal_api.h"
#include <hardware/nfc.h>


class ThreadMutex
{
public:
    ThreadMutex();
    virtual ~ThreadMutex();
    void lock();
    void unlock();
    operator pthread_mutex_t* () {return &mMutex;}
private:
    pthread_mutex_t mMutex;
};

class ThreadCondVar : public ThreadMutex
{
public:
    ThreadCondVar();
    virtual ~ThreadCondVar();
    void signal();
    void wait();
    operator pthread_cond_t* () {return &mCondVar;}
    operator pthread_mutex_t* () {return ThreadMutex::operator pthread_mutex_t*();}
private:
    pthread_cond_t  mCondVar;
};

class AutoThreadMutex
{
public:
    AutoThreadMutex(ThreadMutex &m);
    virtual ~AutoThreadMutex();
    operator ThreadMutex& ()          {return mm;}
    operator pthread_mutex_t* () {return (pthread_mutex_t*)mm;}
private:
    ThreadMutex  &mm;
};

class NfcAdaptation
{
public:
    virtual ~NfcAdaptation();
    void    Initialize();
    void    Finalize();
    static  NfcAdaptation& GetInstance();
    tHAL_NFC_ENTRY* GetHalEntryFuncs ();

private:
    NfcAdaptation();
    void    signal();
    static  NfcAdaptation* mpInstance;
    static  ThreadMutex sLock;
    ThreadCondVar    mCondVar;
    pthread_t mThreadId;
    tHAL_NFC_ENTRY   mHalEntryFuncs; // function pointers for HAL entry points
    static nfc_nci_device_t* mHalDeviceContext;
    static tHAL_NFC_CBACK* mHalCallback;

    static UINT32 NFCA_TASK (UINT32 arg);
    static UINT32 Thread (UINT32 arg);
    void InitializeHalDeviceContext ();
    static void HalDeviceContextCallback (nfc_event_t event, nfc_event_data_t* p_data);

    static void HalInitialize ();
    static void HalTerminate ();
    static void HalOpen (tHAL_NFC_CBACK* p_hal_cback);
    static void HalClose ();
    static void HalCoreInitialized (UINT8* p_core_init_rsp_params);
    static void HalWrite (UINT16 data_len, UINT8* p_data);
    static BOOLEAN HalPrediscover ();
    static void HalControlGranted ();
    static void HalPowerCycle ();
};

