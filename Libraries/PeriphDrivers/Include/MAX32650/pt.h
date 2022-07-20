/**
 * @file  pt.h
 * @brief Pulse Train data types, definitions and function prototypes.
 */

/* *****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-01-09 11:49:38 -0600 (Tue, 09 Jan 2018) $
 * $Revision: 32758 $
 *
 ***************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _PT_H_
#define _PT_H_

/* **** Includes **** */
#include "ptg_regs.h"
#include "pt_regs.h"
#include "mxc_assert.h"
#include "mxc_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup periphlibs
 * @defgroup pt Pulse Train Engine
 * @brief This is the high level API for the pulse train engine.
 * @{
 */

/**
 * Structure type for pulse train mode configuration.
 * @note       Do not use for square wave
 */
typedef struct {
    unsigned channel;       /**< PT Channel to use */
    uint32_t bps;           /**< pulse train bit rate */
    uint32_t pattern;       /**< Output pattern to shift out, starts at LSB */
    uint8_t ptLength;       /**< Number of bits in pulse train, 0 = 32bits, 1 = non valid , 2 = 2 bits, ... */
    uint16_t loop;          /**< Number of times to repeat the train, 0 = continuous */
    uint16_t loopDelay;     /**< Delay between loops specified in multiples of bps */
    uint8_t outputSelect;   /**< Select 0 or 1. See pins_me10.c for the pin selection for each channel. Note: not all selections valid depending on package type (See UG section 5.2). */
} mxc_pt_cfg_t;

/**
 * @brief      Enumeration type for the system clock scale types
 */
typedef enum {
    MXC_PT_CLK_DIV1,
    MXC_PT_CLK_DIV2,
    MXC_PT_CLK_DIV4,
    MXC_PT_CLK_DIV8,
    MXC_PT_CLK_DIV16,
    MXC_PT_CLK_DIV32,
    MXC_PT_CLK_DIV64,
    MXC_PT_CLK_DIV128,
} mxc_clk_scale_t;

/**
 * @brief      This function initializes the pulse trains to a known stopped
 *             state and sets the global PT clock scale.
 * @param      ptg        pointer to pulse train global bus to use.
 * @param      clk_scale  Scale the system clock for the global PT clock.
 */
void MXC_PT_Init (mxc_ptg_regs_t *ptg, mxc_clk_scale_t clk_scale);

/**
 * @brief      Shutdown the pulse train channel/channels.
 * @details    Shutdown pulse train and if all pluse trains are shut down then turn off pulse train clock.
 * @note       Shutdown pulse train channel/channels and delete config.
 *
 * @param      ptg    Pointer to pulse train global bus to use.
 * @param      pts    Pulse train channel to operate on.
 *
 * @return     #E_NO_ERROR if everything is successful, \ref MXC_Error_Codes
 *             "error" if unsuccessful.
 */
void MXC_PT_Shutdown (mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief      Configures the pulse train in the specified mode.
 * @details    The parameters in the config structure must be set before calling
 *             this function. This function should be used for configuring pulse
 *             train mode only.
 * @note       The pulse train cannot be running when this function is called.
 *
 * @param      ptg     Pointer to pulse train global bus to use.
 * @param      cfg     Pointer to pulse train configuration.
 *
 * @return     #E_NO_ERROR if everything is successful, @ref MXC_Error_Codes
 *             "error" if unsuccessful.
 */
int MXC_PT_Config (mxc_ptg_regs_t *ptg, mxc_pt_cfg_t *cfg);

/**
 * @brief   Configures the pulse train in the square wave mode.
 * @details This function should be used for configuring square wave mode only.
 * @note    The pulse train cannot be running when this function is called
 *
 * @param   ptg             Pointer to pulse train global bus to use.
 * @param   channel         Pulse train channel to operate on
 * @param   freq            Square wave output frequency in Hz
 * @param   outputSelect    Select the output to route the pulse train channel to. 0 for output 0, non-zero for output 1.
 *
 * @returns #E_NO_ERROR if everything is successful, \ref MXC_Error_Codes "error" if unsuccessful.
 */
int MXC_PT_SqrWaveConfig (mxc_ptg_regs_t *ptg, unsigned channel, uint32_t freq, uint8_t outputSelect);

/**
 * @brief   Starts the pulse train specified.
 *
 * @param   ptg   Pointer to pulse train global bus to use.
 * @param   pts   Pulse train pts to start. Set the bits of pulse 
 *                trains to check Bit0-\>pt0, Bit1-\>pt1... etc.
 */
void MXC_PT_Start (mxc_ptg_regs_t *ptg, unsigned pts);

/**
 * @brief   Stops a pulse train.
 *
 * @param   ptg   Pointer to pulse train global bus to use.
 * @param   pts   Pulse train pts to stop. Set the bits of pulse 
 *                trains to check Bit0-\>pt0, Bit1-\>pt1... etc.
 */
void MXC_PT_Stop (mxc_ptg_regs_t *ptg, unsigned pts);

/**
 * @brief   Determines if the pulse train is running.
 *
 * @param   ptg   Pointer to pulse train global bus to use.
 * @param   pts   Set the bits of pulse trains to check Bit0-\>pt0,
 *                   Bit1-\>pt1... etc.
 *
 * @return  0       Pulse train is off.
 * @return  \>0     Pulse train is on.
 */
uint32_t MXC_PT_IsActive (mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief   Sets the pattern of the pulse train
 *
 * @param   pts     Pulse train pts to operate on.
 * @param   pattern Output pattern.
 *
 */
void MXC_PT_SetPattern (unsigned pts, uint32_t pattern);

/**
 * @brief      Enable pulse train interrupt.
 *
 * @param      ptg   Pointer to pulse train global bus to use.
 * @param      pts   Bit mask of which pulse trains to enable. Set the bit
 *                   position of each pulse train to enable it. Bit0-\>pt0,
 *                   Bit1-\>pt1... etc, 1 will enable the interrupt, 0 to leave
 *                   a PT channel in its current state.
 */
void MXC_PT_EnableInt (mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief      Disable pulse train interrupt.
 *
 * @param      ptg   Pointer to pulse train global bus to use.
 * @param      pts   Bit mask of what pulse trains to disable. Set the bit
 *                   position of each pulse train to disable it. Bit0-\>pt0,
 *                   Bit1-\>pt1... etc, 1 will disable the interrupt, 0 to leave
 *                   a PT channel in its current state.
 */
void MXC_PT_DisableInt (mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief      Gets the pulse trains's interrupt flags.
 *
 * @param      ptg   Pointer to pulse train global bus to use.
 *
 * @return     The Pulse Train Interrupt Flags, \ref PTG_INTFL_Register Register
 *             for details.
 */
uint32_t MXC_PT_GetFlags (mxc_ptg_regs_t *ptg);

/**
 * @brief      Clears the pulse train's interrupt flag.
 *
 * @param      ptg    pointer to pulse train global bus to use.
 * @param      flags  bits to clear, see \ref PTG_INTFL Register for details.
 */
void MXC_PT_ClearFlags (mxc_ptg_regs_t *ptg, uint32_t flags);

/**
 * @brief      Setup and enables a pulse train to restart after another pulse
 *             train has exited its loop. Each pulse train can have up to two
 *             restart triggers.
 *
 * @param      start         Pulse train channel to start on the stop event.
 * @param      stop          Pulse train channel to trigger the stop event.
 * @param      restartIndex  selects which restart trigger to set, 0 (x) or 1 (y).
 */
void MXC_PT_EnableRestart (unsigned start, unsigned stop, uint8_t restartIndex);

/**
 * @brief      Disable the restart for the specified pulse train
 *
 * @param      channel       Pulse train channel to disable restart for.
 * @param      restartIndex  selects which restart trigger to disable, 0 (x) or 1 (y).
 */
void MXC_PT_DisableRestart (unsigned channel, uint8_t restartIndex);

/**
 * @brief      Resynchronize individual pulse trains together. Resync will stop
 *             those resync_pts; others will be still running
 *
 * @param      ptg  pointer to pulse train global bus to use.
 * @param      pts  pulse train modules that need to be re-synced by bit
 *                        number. Bit0-\>pt0, Bit1-\>pt1... etc.
 */
void MXC_PT_Resync (mxc_ptg_regs_t *ptg, uint32_t pts);

/**@} end of group pt*/

#ifdef __cplusplus
}
#endif

#endif /* _PT_H_ */