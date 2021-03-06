/* ***** BEGIN LICENSE BLOCK *****   
 * Source last modified: $Id: radec_couple.c,v 1.1.1.1 2007/12/07 08:11:48 zpxu Exp $ 
 * 
 * REALNETWORKS CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM   
 * Portions Copyright (c) 1995-2005 RealNetworks, Inc.   
 * All Rights Reserved.   
 *   
 * The contents of this file, and the files included with this file, 
 * are subject to the current version of the RealNetworks Community 
 * Source License (the "RCSL"), including Attachment G and any 
 * applicable attachments, all available at 
 * http://www.helixcommunity.org/content/rcsl.  You may also obtain 
 * the license terms directly from RealNetworks.  You may not use this 
 * file except in compliance with the RCSL and its Attachments. There 
 * are no redistribution rights for the source code of this 
 * file. Please see the applicable RCSL for the rights, obligations 
 * and limitations governing use of the contents of the file. 
 *   
 * This file is part of the Helix DNA Technology. RealNetworks is the 
 * developer of the Original Code and owns the copyrights in the 
 * portions it created. 
 *   
 * This file, and the files included with this file, is distributed 
 * and made available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS 
 * ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET 
 * ENJOYMENT OR NON-INFRINGEMENT. 
 *   
 * Technology Compatibility Kit Test Suite(s) Location:   
 * https://rarvcode-tck.helixcommunity.org   
 *   
 * Contributor(s):   
 *   
 * ***** END LICENSE BLOCK ***** */  

/**************************************************************************** */
/*    Socrates Software Ltd : Toshiba Group Company */
/*    DESCRIPTION OF CHANGES: */
/*    
/*		1. Data overlay performed
/*		2. Debug trace features added
/*
 *    CONTRIBUTORS : Vinayak Bhat,Deepak,Sudeendra,Naveen
 *   */
/**************************************************************************** */

#include "radec_defines.h"
#include "radec_assembly.h"

#ifdef RA8_DEBUG
extern struct SRA8_Debug sRA8_Debug;
#endif

#define 	 DMEM_CPLINDEX		0x202FB0

#ifdef __DMA_AUDIOTAB__
#pragma section const .audiotabs
#endif
/* coupling band widths, based on Zwicker critical-bandwidth */
static const int cplband[MAXREGNS] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	11, 11,
	12, 12,
	13, 13,
	14, 14, 14,
	15, 15, 15, 15,
	16, 16, 16, 16, 16,
	17, 17, 17, 17, 17, 17,
	18, 18, 18, 18, 18, 18, 18,
	19, 19, 19, 19, 19, 19, 19, 19, 19
};

/* Lookup tables for reconstructed scaling factors - format = Q31 */
static const int cplScale[3 + 7 + 15 + 31 + 63] = {

	/* cplScale2[3] */
	0x79fc945f, 0x5a82799a, 0x26c59af8,

	/* cplScale3[7] */
	0x7d9a93a4, 0x77ef8978, 0x701e9f27, 0x5a82799a, 
	0x3dc04491, 0x2cb7094c, 0x18a6b4ed,

	/* cplScale4[15] */
	0x7ee90962, 0x7c936c4f, 0x79fc9465, 0x770e9d8f, 
	0x73a45b28, 0x6f749c4e, 0x69c04836, 0x5a82799a, 
	0x481dac45, 0x3ef11955, 0x36df3415, 0x2f011d16, 
	0x26c59ae8, 0x1d688afa, 0x10aaa6fd,

	/* cplScale5[31] */
	0x7f7a8410, 0x7e66f9cf, 0x7d46e886, 0x7c18c36e, 
	0x7adaa745, 0x798a3db9, 0x78249362, 0x76a5d7e1, 
	0x7508f83e, 0x7346f452, 0x7155abbc, 0x6f257fbd, 
	0x6c9c00f0, 0x6985577e, 0x655acbf3, 0x5a82799a, 
	0x4e2ca29a, 0x4873cee8, 0x43bc1a9b, 0x3f7c6426, 
	0x3b7ddcf1, 0x37a26159, 0x33d59802, 0x30072e43, 
	0x2c27c13f, 0x28266a5d, 0x23edbab6, 0x1f5e6927, 
	0x1a42fec7, 0x14294028, 0x0b8ab0db,

	/* cplScale6[63] */
	0x7fbea8be, 0x7f39f9ab, 0x7eb28121, 0x7e281805, 
	0x7d9a93a8, 0x7d09c543, 0x7c75796f, 0x7bdd7778, 
	0x7b418090, 0x7aa14ee1, 0x79fc9466, 0x7952f983, 
	0x78a41b51, 0x77ef897b, 0x7734c38d, 0x76733593, 
	0x75aa33b1, 0x74d8f46c, 0x73fe8908, 0x7319d333, 
	0x722976ad, 0x712bc4eb, 0x701e9f29, 0x6eff48fd, 
	0x6dca2025, 0x6c7a2377, 0x6b081945, 0x6968e250, 
	0x6789b6cc, 0x6545d976, 0x623e1923, 0x5a82799a, 
	0x520d1a91, 0x4e47c3aa, 0x4b4243b5, 0x489d2fc3, 
	0x46338b6a, 0x43f2498b, 0x41cdfe96, 0x3fbf1c82, 
	0x3dc0448d, 0x3bcd6c20, 0x39e361f6, 0x37ff83c3, 
	0x361f8e06,	0x34417ac7, 0x326368ee, 0x3083887b, 
	0x2ea0091f, 0x2cb70944, 0x2ac683d0, 0x28cc3a99, 
	0x26c59ae4, 0x24af97f2, 0x22867502, 0x20457311, 
	0x1de64c41, 0x1b604f6f, 0x18a6b4dc, 0x15a521e0, 
	0x1237205d, 0x0e0d0479, 0x08144f82
};

static const int cplScaleOffset[7] = { 0, 0, 0, 3, 10, 25, 56};
#ifdef __DMA_AUDIOTAB__
#pragma section const
#endif

/**************************************************************************************
 * Function:    DecodeCoupleInfo
 *
 * Description: decode indices for joint stereo scale factors
 *
 * Inputs:      pointer to initialized Gecko2Info struct
 *              number of bits remaining in bitstream for this frame
 *
 * Outputs:     filled-in cplindex[] buffer, with one index per coupling band
 *
 * Return:      number of bits remaining in bitstream, -1 if out-of-bits
 *
 * Notes:       couple index info can either be direct-coded or Huffman-coded
 **************************************************************************************/
int DecodeCoupleInfo(Gecko2Info *gi,SBitStreamInfo *bsi, int availbits)
{
	int bandstart = cplband[gi->sHdInfo.cplStart];
	int bandend = cplband[gi->sHdInfo.nRegions - 1];
	int hufmode, nbits, b, cache;
	unsigned char FuncDetect =0;

#ifdef __DMA_COUPLE__
	int *cplindex = (int*)DMEM_CPLINDEX;
#else
	int *cplindex = gi->sDecBuf.cplindex;
#endif


	if (availbits <= 0)
		return -1;

	/* one-bit flag indicating Huffman or direct coding */
	hufmode = GetBits(bsi, 1, 1);
	availbits=availbits-1;
#ifdef RA8_DEBUG
	sRA8_Debug.iHuffmode=hufmode;
	sRA8_Debug.iBandStart=bandstart;
	sRA8_Debug.iBandEnd=bandend;
#endif
	if (hufmode) 
    {
		/* HUFFMAN */
		for (b = bandstart; b <= bandend; b++) 
        {
			cache = GetBits(bsi, MAX_HUFF_BITS, 0);
			nbits = DecodeHuffmanScalar(huffTabCouple, &huffTabCoupleInfo[gi->sHdInfo.cplQbits - HUFFTAB_COUPLE_OFFSET], cache, cplindex + b,&FuncDetect);
			if (nbits == -1)
				return -1;
			if (nbits > availbits)
				return -1;
			availbits =availbits- nbits;
			AdvanceBitstream(bsi, nbits);
		}
	} else {
		/* DIRECT */
		if (availbits < (gi->sHdInfo.cplQbits*(bandend - bandstart + 1)) )
			return -1;
		for (b = bandstart; b <= bandend; b++) {
			cplindex[b] = GetBits(bsi, gi->sHdInfo.cplQbits, 1);	
			availbits =availbits- gi->sHdInfo.cplQbits;
		}
	}
#ifdef RA8_DEBUG
	for (b = bandstart; b <= bandend; b++) 
	{
		sRA8_Debug.iCplIndex[b]=gi->sDecBuf.cplindex[b];
	}
#endif
	return availbits;
}

/**************************************************************************************
 * Function:    JointDecodeMLT
 *
 * Description: decode the jointly-coded MLT
 *
 * Inputs:      pointer to initialized Gecko2Info struct
 *              mltleft[0, ... , cplStart-1]  has non-coupled coefficients for left
 *              mltrght[0, ... , cplStart-1]  has non-coupled coefficients for right
 *              mltleft[cplStart, ... , cRegions] has coupled coefficients
 *
 * Outputs:     mltleft[0, ... , nRegions-1]  has reconstructed coefficients for left
 *              mltrght[0, ... , nRegions-1]  has reconstructed coefficients for right
 *
 * Return:      none
 **************************************************************************************/
void JointDecodeMLT(Gecko2Info *gi, int *mltleft, int *mltrght)
{
	int scaleleft, scalerght;
	int i, r, q;
	int cplquant, cploffset;

#ifdef __DMA_COUPLE__
	        int *cplindex = (int*)DMEM_CPLINDEX;
#else
		int *cplindex = gi->sDecBuf.cplindex;
#endif	

	cplquant = (1 << gi->sHdInfo.cplQbits) - 1;	/* quant levels */
	cploffset = cplScaleOffset[gi->sHdInfo.cplQbits];

	/* reconstruct the stereo channels */
	for (r = gi->sHdInfo.cplStart; r < gi->sHdInfo.nRegions; r++) {

		/*
		 * dequantize the expanded coupling ratio
		 * expand = (q - (cplquant>>1)) * (2.0f/cplquant);
		 *
		 * square-law compression
		 * ratio = sqrt(fabs(expand));
		 * if (expand < 0.0f) ratio = -ratio;
		 *
		 * reconstruct the scaling factors
		 * scaleleft = sqrt(0.5f - 0.5f * ratio);
		 * scalerght = sqrt(0.5f + 0.5f * ratio);
		 */
		q = cplindex[cplband[r]];
		scaleleft = cplScale[cploffset + q];
		scalerght = cplScale[cploffset + cplquant - 1 - q];

		/* drop extra sign bit (max gain = 0.998) */
		for (i = 0; i < NBINS; i++) {
			mltrght[NBINS*r + i] = MULSHIFT32(scalerght, mltleft[NBINS*r + i]) << 1;
			mltleft[NBINS*r + i] = MULSHIFT32(scaleleft, mltleft[NBINS*r + i]) << 1;
		}
	}

	/* set non-coded regions to zero */
	for (i = gi->sHdInfo.nRegions * NBINS; i < gi->sHdInfo.nSamples; i++) {
		mltleft[i] = 0;
		mltrght[i] = 0;
	}
	
	return;
}
