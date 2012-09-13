/****************************************************************************
**
** Name:         nfc_hal_int_api.h
**
** Description:  Internal NFC HAL API functions.
**
** Copyright (c) 2009-2012, BROADCOM Inc., All Rights Reserved.
** Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/
#ifndef NFC_HAL_INT_API_H
#define NFC_HAL_INT_API_H

/****************************************************************************
** Device Configuration definitions
****************************************************************************/

#define NFC_HAL_PLL_325_SETCONFIG_PARAM_LEN     (2 + NCI_PARAM_LEN_PLL325_CFG_PARAM)

/* Crystal Frequency Index (in 1 KHz) */
enum
{
    NFC_HAL_XTAL_INDEX_9600,
    NFC_HAL_XTAL_INDEX_13000,
    NFC_HAL_XTAL_INDEX_16200,
    NFC_HAL_XTAL_INDEX_19200,
    NFC_HAL_XTAL_INDEX_24000,
    NFC_HAL_XTAL_INDEX_26000,
    NFC_HAL_XTAL_INDEX_38400,
    NFC_HAL_XTAL_INDEX_52000,
    NFC_HAL_XTAL_INDEX_37400,
    NFC_HAL_XTAL_INDEX_MAX
};
typedef UINT8 tNFC_HAL_XTAL_INDEX;

/* Broadcom specific device initialization before sending NCI reset */
#define NFC_HAL_DEV_INIT_FLAGS_SET_XTAL_FREQ  0x02    /* set crystal frequency    */
typedef UINT8 tNFC_HAL_DEV_INIT_FLAGS;

typedef struct
{
    tNFC_HAL_DEV_INIT_FLAGS flags;
    UINT16                  xtal_freq;
} tNFC_HAL_DEV_INIT_CFG;

typedef void (tHAL_NFC_STATUS_CBACK) (tHAL_NFC_STATUS status);

/*****************************************************************************
**  Low Power Mode definitions
*****************************************************************************/

#define NFC_HAL_LP_SNOOZE_MODE_NONE      0x00    /* Snooze mode disabled    */
#define NFC_HAL_LP_SNOOZE_MODE_UART      0x01    /* Snooze mode for UART    */
#define NFC_HAL_LP_SNOOZE_MODE_SPI_I2C   0x08    /* Snooze mode for SPI/I2C */

#define NFC_HAL_LP_ACTIVE_LOW            0x00    /* high to low voltage is asserting */
#define NFC_HAL_LP_ACTIVE_HIGH           0x01    /* low to high voltage is asserting */

/*****************************************************************************
**  Patch RAM Constants
*****************************************************************************/

/* patch format type */
#define NFC_HAL_PRM_FORMAT_BIN  0x00
#define NFC_HAL_PRM_FORMAT_HCD  0x01
#define NFC_HAL_PRM_FORMAT_NCD  0x02
typedef UINT8 tNFC_HAL_PRM_FORMAT;

/*****************************************************************************
**  Patch RAM Callback for event notificaton
*****************************************************************************/
/* Events for tNFC_HAL_PRM_CBACK */
enum
{
    NFC_HAL_PRM_CONTINUE_EVT,
    NFC_HAL_PRM_COMPLETE_EVT,
    NFC_HAL_PRM_ABORT_EVT,
    NFC_HAL_PRM_ABORT_INVALID_PATCH_EVT,       /* Patch is invalid (bad version, project id, or chip)  */
    NFC_HAL_PRM_ABORT_BAD_SIGNATURE_EVT,       /* Patch has invalid signature                          */
    NFC_HAL_PRM_SPD_GET_PATCHFILE_HDR_EVT,     /* Secure Patch Download: request for patchfile header  */
    NFC_HAL_PRM_SPD_GET_NEXT_PATCH             /* Get first command of next patch in patchfile         */
};

typedef void (tNFC_HAL_PRM_CBACK) (UINT8 event);


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
**
** Function         HAL_NfcPreInitDone
**
** Description      Notify that pre-initialization of NFCC is complete
**
** Returns          void
**
*******************************************************************************/
void HAL_NfcPreInitDone (tHAL_NFC_STATUS status);

/*******************************************************************************
**
** Function         HAL_NfcSetSnoozeMode
**
** Description      Set snooze mode
**                  snooze_mode
**                      NFC_HAL_LP_SNOOZE_MODE_NONE - Snooze mode disabled
**                      NFC_HAL_LP_SNOOZE_MODE_UART - Snooze mode for UART
**                      NFC_HAL_LP_SNOOZE_MODE_SPI_I2C - Snooze mode for SPI/I2C
**
**                  idle_threshold_dh/idle_threshold_nfcc
**                      Idle Threshold Host in 100ms unit
**
**                  nfc_wake_active_mode/dh_wake_active_mode
**                      NFC_HAL_LP_ACTIVE_LOW - high to low voltage is asserting
**                      NFC_HAL_LP_ACTIVE_HIGH - low to high voltage is asserting
**
**                  p_snooze_cback
**                      Notify status of operation
**
** Returns          tHAL_NFC_STATUS
**
*******************************************************************************/
tHAL_NFC_STATUS HAL_NfcSetSnoozeMode (UINT8 snooze_mode,
                                      UINT8 idle_threshold_dh,
                                      UINT8 idle_threshold_nfcc,
                                      UINT8 nfc_wake_active_mode,
                                      UINT8 dh_wake_active_mode,
                                      tHAL_NFC_STATUS_CBACK *p_snooze_cback);

/*******************************************************************************
**
** Function         HAL_NfcPrmDownloadStart
**
** Description      Initiate patch download
**
** Input Params
**                  format_type     patch format type
**                                  (NFC_HAL_PRM_FORMAT_BIN, NFC_HAL_PRM_FORMAT_HCD, or
**                                   NFC_HAL_PRM_FORMAT_NCD)
**
**                  dest_address    destination adderess (needed for BIN format only)
**
**                  p_patchram_buf  pointer to patchram buffer. If NULL,
**                                  then app must call HAL_NfcPrmDownloadContinue when
**                                  NFC_HAL_PRM_CONTINUE_EVT is received, to send the next
**                                  segment of patchram
**
**                  patchram_len    size of p_patchram_buf (if non-NULL)
**
**                  patchram_delay  The delay after each patch.
**                                  If the given value is less than the size of the patchram,
**                                  the size of patchram is used instead.
**
**                  p_cback         callback for download status
**
**
** Returns          TRUE if successful, otherwise FALSE
**
**
*******************************************************************************/
BOOLEAN HAL_NfcPrmDownloadStart (tNFC_HAL_PRM_FORMAT format_type,
                                 UINT32              dest_address,
                                 UINT8               *p_patchram_buf,
                                 UINT32              patchram_len,
                                 UINT32              patchram_delay,
                                 tNFC_HAL_PRM_CBACK  *p_cback);

/*******************************************************************************
**
** Function         HAL_NfcPrmDownloadContinue
**
** Description      Send next segment of patchram to controller. Called when
**                  NFC_HAL_PRM_CONTINUE_EVT is received.
**
**                  Only needed if HAL_NfcPrmDownloadStart was called with
**                  p_patchram_buf=NULL
**
** Input Params     p_patch_data    pointer to patch data
**                  patch_data_len  patch data len
**
** Returns          TRUE if successful, otherwise FALSE
**
*******************************************************************************/
BOOLEAN HAL_NfcPrmDownloadContinue (UINT8 *p_patch_data,
                                    UINT16 patch_data_len);

/*******************************************************************************
**
** Function         HAL_NfcPrmSetI2cPatch
**
** Description      Specify patchfile for BCM20791B3 I2C fix. This fix
**                  must be downloaded prior to initial patch download for I2C
**                  transport
**
** Input Params     p_i2c_patchfile_buf: pointer to patch for i2c fix
**                  i2c_patchfile_len: length of patch
**                  prei2c_delay: the delay before downloading main patch
**                                if 0 is given, NFC_HAL_PRM_POST_I2C_FIX_DELAY is used instead.
**
** Returns          Nothing
**
**
*******************************************************************************/
void HAL_NfcPrmSetI2cPatch (UINT8 *p_i2c_patchfile_buf,
                      UINT16 i2c_patchfile_len, UINT32 prei2c_delay);

/*******************************************************************************
**
** Function         HAL_NfcPrmSetSpdNciCmdPayloadSize
**
** Description      Set Host-to-NFCC NCI message size for secure patch download
**
**                  This API must be called before calling HAL_NfcPrmDownloadStart.
**                  If the API is not called, then PRM will use the default
**                  message size.
**
**                  Typically, this API is only called for platforms that have
**                  message-size limitations in the transport/driver.
**
**                  Valid message size range: NFC_HAL_PRM_MIN_NCI_CMD_PAYLOAD_SIZE to 255.
**
** Returns          HAL_NFC_STATUS_OK if successful
**                  HAL_NFC_STATUS_FAILED otherwise
**
**
*******************************************************************************/
tHAL_NFC_STATUS HAL_NfcPrmSetSpdNciCmdPayloadSize (UINT8 max_payload_size);

/*******************************************************************************
**
** Function         HAL_NfcSetTraceLevel
**
** Description      This function sets the trace level for HAL.  If called with
**                  a value of 0xFF, it simply returns the current trace level.
**
** Returns          The new or current trace level
**
*******************************************************************************/
UINT8 HAL_NfcSetTraceLevel (UINT8 new_level);


#ifdef __cplusplus
}
#endif

#endif /* NFC_HAL_INT_API_H */

