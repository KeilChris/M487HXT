/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * @brief    The clock can be input from XT1_IN as the main frequency of the MCU
 *           and can be output on CLKO.
 *
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Set GPIO operation mode */
    GPIO_SetMode(PF, (BIT2|BIT3),  GPIO_MODE_INPUT);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O (XT1) Multi-function                                                                           */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set GPF multi-function pins for XT1 IN and OUT */
    SYS->GPF_MFPL &= ~(SYS_GPF_MFPL_PF3MFP_Msk | SYS_GPF_MFPL_PF2MFP_Msk);
    SYS->GPF_MFPL |= (SYS_GPF_MFPL_PF3MFP_GPIO | SYS_GPF_MFPL_PF2MFP_GPIO);

    /* Enable HXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as HCLK_CLOCK */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLKSEL_Msk)) | CLK_CLKSEL0_HCLKSEL_HXT;

    /* Output selected clock to CKO, CKO Clock = HCLK / 2^(1 + 1) */
    CLK_EnableCKO(CLK_CLKSEL1_CLKOSEL_HXT, 1, 0);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O (CLKO) Multi-function                                                                          */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PC multi-function pins for CLKO(PC.13) */
    SYS->GPC_MFPH &= ~SYS_GPC_MFPH_PC13MFP_Msk;
    SYS->GPC_MFPH |= SYS_GPC_MFPH_PC13MFP_CLKO;

    /* Lock protected registers */
    SYS_LockReg();
}

int main()
{
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();
	
    printf("+--------------------------------------------------------+\n");
    printf("|         HXT external clock input Sample Code           |\n");
    printf("+--------------------------------------------------------+\n");
	printf("Please input clock from XT1_IN(PF.3) as the HXT clock source\n");
	printf("HXT will output clock to the CLKO(PC.13) pin\n");
    printf("\n");

    while(1);
}

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
