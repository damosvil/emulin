/*
 * ui.h
 *
 *  Created on: 13 sept. 2019
 *      Author: iso9660
 */

#ifndef UI_UI_H_
#define UI_UI_H_

#define INT_0_63_EXPR			(gpointer)"^[0-9]$|^[0-5][0-9]$|^6[0-3]$"
#define INT_1_8_EXPR			(gpointer)"^[1-8]$"
#define INT_1_4_EXPR			(gpointer)"^[1-4]$"
#define INT3_EXPR				(gpointer)"^[0-9]{1,2}$|^[0-1][0-9]{2}$|^2[0-4][0-9]$|^25[0-5]$|^0x[0-9A-Fa-f]{0,2}$"
#define INT5_EXPR				(gpointer)"^[0-9]{1,4}$|^[0-5][0-9]{4}$|^6[0-4][0-9]{3}$|^65[0-4][0-9]{2}$|^655[0-2][0-9]$|^6553[0-5]$|^0x[0-9A-Fa-f]{0,4}$"
#define NAME_EXPR				(gpointer)"^[A-Za-z]{1}[A-Za-z_0-9]{0,31}$"
#define SFLOAT_EXPR				(gpointer)"^[0-9]{1,2}[.]{0,1}[0-9]{0,3}$"
#define RAW_DATA_5				"^0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2}$"
#define RAW_DATA_8				"^0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2} 0x[0-9A-Fa-f]{1,2}$"


#endif /* UI_UI_H_ */
