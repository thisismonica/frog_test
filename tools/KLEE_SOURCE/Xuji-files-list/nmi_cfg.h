/***********************************************************************
*
*            Copyright 1993 Mentor Graphics Corporation
*                         All Rights Reserved.
*
* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS
* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS
* SUBJECT TO LICENSE TERMS.
*
************************************************************************

************************************************************************
*
*   FILE NAME
*
*       nmi_cfg.h
*
*   COMPONENT
*
*       NMI - Nucleus Middleware Initialization
*
*   DESCRIPTION
*
*       This file contains configuration settings used by Nucleus
*       middleware initialization functions to start the initialization
*       process of middleware products.
*
*       NOTE: These are set by the user.
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       nucleus.h                                   System header file
*
***********************************************************************/

#ifndef NMI_CFG_H
#define NMI_CFG_H

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

/* Include required header files */
#include    "plus/nucleus.h"

/* Set the necessary defines to NU_TRUE to initialize a Nucleus middleware product.
   NOTE: Add any new define here for each MW entry in NMI_Product_Registry[].
   NOTE: Add new externs below for each MW entry in NMI_Product_Registry[].  */

#define     PLUS_SERIAL_IO_NMI_INIT         NU_TRUE     /* Nucleus PLUS Serial I/O */
#define     AGENT_NMI_INIT                  NU_AGENT_ENABLE   /* Nucleus Agent */
#define     NPP_BASE_NMI_INIT               NU_FALSE    /* Nucleus C++ BASE */
#define     NPP_FILE_NMI_INIT               NU_FALSE    /* Nucleus C++ FILE */
#define     NPP_NET_NMI_INIT                NU_FALSE    /* Nucleus C++ NET */
#define     NPP_PLUS_NMI_INIT               NU_FALSE    /* Nucleus C++ PLUS */
#define     CAN_NMI_INIT                    NU_FALSE    /* Nucleus CAN */
#define     CANOPEN_NMI_INIT                NU_FALSE    /* Nucleus CANopen */
#define     CANOPEN_DS401_NMI_INIT          NU_FALSE    /* Nucleus CANopen DS401 */
#define     CANOPEN_DS404_NMI_INIT          NU_FALSE    /* Nucleus CANopen DS404 */
#define     CANOPEN_DS405_NMI_INIT          NU_FALSE    /* Nucleus CANopen DS405 */
#define     CIPHER_SUITE_NMI_INIT           NU_FALSE    /* Nucleus Cipher Suite */
#define     DHCP_RELAY_NMI_INIT             NU_FALSE    /* Nucleus DHCP Relay Agent */
#define     DHCP_SRV_NMI_INIT               NU_FALSE    /* Nucleus DHCP Server */
#define     DOT1X_NMI_INIT                  NU_FALSE    /* Nucleus 802.1X SUP */
#define     EMAIL_SMTP_SERVER_NMI_INIT      NU_FALSE    /* Nucleus EMAIL SMTP Server */
#define     PPTP_NMI_INIT                   NU_FALSE    /* Nucleus PPTP */
#define     RTP_NMI_INIT                    NU_FALSE    /* Nucleus RTP */
#define     XPROT_FTP_SERVER_NMI_INIT       NU_FALSE    /* Nucleus Extended Protocol FTP Server */
#define     XPROT_TFTP_SERVER_NMI_INIT      NU_FALSE    /* Nucleus Extended Protocol TFTP Server */
#define     FAL_NMI_INIT                    NU_FALSE    /* FAL */
#define     FILE_NMI_INIT                   NU_FALSE    /* Nucleus FILE */
#define     GRAFIX_RS_NMI_INIT              NU_FALSE    /* Nucleus GRAFIX RS */
#define     GRAFIX_WT_NMI_INIT              NU_FALSE    /* Nucleus GRAFIX WT */
#define     I2C_NMI_INIT                    NU_FALSE    /* Nucleus I2C */
#define     IKE_NMI_INIT                    NU_FALSE    /* Nucleus Internet Key Exchange */
#define     INFLEXION_NMI_INIT              NU_FALSE    /* Nucleus Inflexion UI Engine */
#define     IPC_NMI_INIT                    NU_FALSE    /* Nucleus Inter-Processor Communication */
#define     IPSEC_NMI_INIT                  NU_FALSE    /* Nucleus Internet Protocol security */
#define     L2TP_NMI_INIT                   NU_FALSE    /* Nucleus Layer Two Tunneling Protocol */
#define     NAFEM_NMI_INIT                  NU_FALSE    /* Nucleus NAFEM */
#define     NAT_NMI_INIT                    NU_FALSE    /* Nucleus NAT */
#define     NET_NMI_INIT                    NU_FALSE    /* Nucleus NET */
#define     PCI_NMI_INIT                    NU_FALSE    /* Nucleus PCI */
#define     PCIX_NMI_INIT                   NU_FALSE    /* Nucleus PCI-X */
#define     POSIX_NMI_INIT                  NU_FALSE    /* Nucleus POSIX */
#define     RSTP_NMI_INIT                   NU_FALSE    /* Nucleus RSTP */
#define     SECSH_SERVER_NMI_INIT           NU_FALSE    /* Nucleus secSH Server */
#define     SECSH_FTP_SERVER_NMI_INIT       NU_FALSE    /* Nucleus secSH FTP Server */
#define     SHELL_NMI_INIT                  NU_FALSE    /* Nucleus SHELL */
#define     SNMP_NMI_INIT                   NU_FALSE    /* Nucleus SNMP */
#define     SNTP_CLIENT_NMI_INIT            NU_FALSE    /* Nucleus SNTP Client */
#define     SPAN_NMI_INIT                   NU_FALSE    /* Nucleus SPAN */
#define     SPI_NMI_INIT                    NU_FALSE    /* Nucleus SPI */
#define     SSL_NMI_INIT                    NU_FALSE    /* Nucleus SSL */
#define     UIPLUS_NMI_INIT                 NU_FALSE    /* Nucleus µiPLUS */
#define     USB_NMI_INIT                    NU_FALSE    /* Nucleus USB products */
#define     DPS_NMI_INIT                    NU_FALSE    /* Nucleus PictBridge */
#define     MTP_NMI_INIT                    NU_FALSE    /* Nucleus MTP */
#define     WEBBROWSE_NMI_INIT              NU_FALSE    /* Nucleus WebBrowse */
#define     WEBSERV_NMI_INIT                NU_FALSE    /* Nucleus WebServ */
#define     MMF_NMI_INIT                    NU_FALSE    /* Nucleus Multimedia Framework */


/* Each Nucleus middleware product must provide:
   o A registration function                - used ONLY by NMI
   o An NMI ID to pass to NMI functions     - used by anyone

   NOTE: Add any new define above for each MW entry in NMI_Product_Registry[].
   NOTE: Add new externs here for each MW entry in NMI_Product_Registry[] */

#if (PLUS_SERIAL_IO_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  PLUS_SERIAL_IO_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      PLUS_SERIAL_IO_NMI_Id;
#endif

#if (AGENT_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  AGENT_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      AGENT_NMI_Id;
#endif

#if (NPP_BASE_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  NPP_BASE_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      NPP_BASE_NMI_Id;
#endif

#if (NPP_FILE_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  NPP_FILE_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      NPP_FILE_NMI_Id;
#endif

#if (NPP_NET_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  NPP_NET_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      NPP_NET_NMI_Id;
#endif

#if (NPP_PLUS_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  NPP_PLUS_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      NPP_PLUS_NMI_Id;
#endif

#if (CAN_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  CAN_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      CAN_NMI_Id;
#endif

#if (CANOPEN_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  CANOPEN_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      CANOPEN_NMI_Id;
#endif

#if (CANOPEN_DS401_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  CANOPEN_DS401_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      CANOPEN_DS401_NMI_Id;
#endif

#if (CANOPEN_DS404_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  CANOPEN_DS404_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      CANOPEN_DS404_NMI_Id;
#endif

#if (CANOPEN_DS405_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  CANOPEN_DS405_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      CANOPEN_DS405_NMI_Id;
#endif

#if (CIPHER_SUITE_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  CIPHER_SUITE_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      CIPHER_SUITE_NMI_Id;
#endif

#if (DHCP_RELAY_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  DHCP_RELAY_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      DHCP_RELAY_NMI_Id;
#endif

#if (DHCP_SRV_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  DHCP_SRV_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      DHCP_SRV_NMI_Id;
#endif

#if (DOT1X_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  DOT1X_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      DOT1X_NMI_Id;
#endif

#if (EMAIL_SMTP_SERVER_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  EMAIL_SMTP_SERVER_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      EMAIL_SMTP_SERVER_NMI_Id;
#endif

#if (PPTP_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  PPTP_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      PPTP_NMI_Id;
#endif

#if (RTP_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  RTP_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      RTP_NMI_Id;
#endif

#if (XPROT_FTP_SERVER_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  XPROT_FTP_SERVER_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      XPROT_FTP_SERVER_NMI_Id;
#endif

#if (XPROT_TFTP_SERVER_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  XPROT_TFTP_SERVER_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      XPROT_TFTP_SERVER_NMI_Id;
#endif

#if (FAL_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  FAL_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      FAL_NMI_Id;
#endif

#if (FILE_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  FILE_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      FILE_NMI_Id;
#endif

#if (GRAFIX_RS_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  GRAFIX_RS_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      GRAFIX_RS_NMI_Id;
#endif

#if (GRAFIX_WT_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  GRAFIX_WT_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      GRAFIX_WT_NMI_Id;
#endif

#if (I2C_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  I2C_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      I2C_NMI_Id;
#endif

#if (IKE_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  IKE_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      IKE_NMI_Id;
#endif

#if (INFLEXION_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  IFX_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      IFX_NMI_Id;
#endif

#if (IPC_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  IPC_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      IPC_NMI_Id;
#endif

#if (IPSEC_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  IPSEC_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      IPSEC_NMI_Id;
#endif

#if (L2TP_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  L2TP_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      L2TP_NMI_Id;
#endif

#if (NAFEM_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  NAFEM_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      NAFEM_NMI_Id;
#endif

#if (NAT_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  NAT_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      NAT_NMI_Id;
#endif

#if (NET_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  NET_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      NET_NMI_Id;
#endif

#if (PCI_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  PCI_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      PCI_NMI_Id;
#endif

#if (PCIX_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  PCIX_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      PCIX_NMI_Id;
#endif

#if (POSIX_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  POSIX_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      POSIX_NMI_Id;
#endif

#if (RSTP_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  RSTP_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      RSTP_NMI_Id;
#endif

#if (SECSH_SERVER_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SECSH_SVR_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SECSH_SVR_NMI_Id;
#endif

#if (SECSH_FTP_SERVER_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SECSH_FTP_SVR_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SECSH_FTP_SVR_NMI_Id;
#endif

#if (SHELL_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SHELL_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SHELL_NMI_Id;
#endif

#if (SNMP_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SNMP_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SNMP_NMI_Id;
#endif

#if (SNTP_CLIENT_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SNTP_CLIENT_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SNTP_CLIENT_NMI_Id;
#endif

#if (SPAN_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SPAN_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SPAN_NMI_Id;
#endif

#if (SPI_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SPI_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SPI_NMI_Id;
#endif

#if (SSL_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  SSL_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      SSL_NMI_Id;
#endif

#if (UIPLUS_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  UIPLUS_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      UIPLUS_NMI_Id;
#endif

#if (USB_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  USB_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      USB_NMI_Id;
#endif

#if (DPS_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  DPS_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      DPS_NMI_Id;
#endif

#if (MTP_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  MTP_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      MTP_NMI_Id;
#endif

#if (WEBBROWSE_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  WEBBROWSE_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      WEBBROWSE_NMI_Id;
#endif

#if (WEBSERV_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS  WEBSERV_NMI_Registration(NMI_MW_REG_DATA *);
extern const CHAR*      WEBSERV_NMI_Id;
#endif

#if (MMF_NMI_INIT == NU_TRUE)
extern NMI_INIT_STATUS MMF_NMI_Registration(NMI_MW_REG_DATA *reg_data_ptr);
extern const CHAR*     MMF_NMI_Id;
#endif

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif  /* NMI_CFG_H */
