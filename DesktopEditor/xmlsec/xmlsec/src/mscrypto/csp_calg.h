/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (c) 2005-2006 Cryptocom LTD (http://www.cryptocom.ru).
 * All rights reserved.
 */
#ifndef CRYPTOCOM_CSP_CALG_H
#define CRYPTOCOM_CSP_CALG_H

#define ALG_TYPE_GR3410 (7 << 9)

#define ALG_SID_MAGPRO_R3410_94                 64
#define ALG_SID_MAGPRO_R3410_94_EPHEM   65
#define ALG_SID_MAGPRO_R3410_2001               66
#define ALG_SID_MAGPRO_R3410_2001_EPHEM 67
#define ALG_SID_MAGPRO_28147_89                 68
#define ALG_SID_GR3411                                  30
#define ALG_SID_G28147                                  30

#define ALG_SID_GR3410                                  30
#define ALG_SID_DH_EX_SF                                30
#define ALG_SID_DH_EX_EPHEM                             31
#define ALG_SID_PRO_AGREEDKEY_DH                33
#define ALG_SID_PRO_SIMMETRYKEY                 34
#define ALG_SID_GR3410EL                                35
#define ALG_SID_DH_EL_SF                                36
#define ALG_SID_DH_EL_EPHEM                             37

/*! \defgroup CALG_MAGPRO CALG_MAGPRO
 *  \brief The description of CALG_MAGPRO
 *
 * @{
 */


#define CALG_MAGPRO_SIGN_R3410_94       (ALG_CLASS_SIGNATURE | ALG_TYPE_GR3410 | ALG_SID_MAGPRO_R3410_94)

#define CALG_MAGPRO_SIGN_R3410_2001     (ALG_CLASS_SIGNATURE | ALG_TYPE_GR3410 | ALG_SID_MAGPRO_R3410_2001)

#define CALG_MAGPRO_DH_R3410_94         (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_MAGPRO_R3410_94)

#define CALG_MAGPRO_DH_R3410_2001       (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_MAGPRO_R3410_2001)

#define CALG_MAGPRO_DH_R3410_94_EPHEM   (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_MAGPRO_R3410_94_EPHEM)

#define CALG_MAGPRO_DH_R3410_2001_EPHEM (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_MAGPRO_R3410_2001_EPHEM)

#define CALG_MAGPRO_HASH_R3411_94       (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_GR3411)

#define CALG_MAGPRO_HASH_28147_89       (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_MAGPRO_28147_89)

#define CALG_MAGPRO_ENCR_28147_89       (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_BLOCK | ALG_SID_G28147)

#define CALG_GR3410                                             (ALG_CLASS_SIGNATURE | ALG_TYPE_GR3410 | ALG_SID_GR3410)

#define CALG_GR3410EL                                   (ALG_CLASS_SIGNATURE | ALG_TYPE_GR3410 | ALG_SID_GR3410EL)

#define CALG_DH_EX_SF                                   (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EX_SF)

#define CALG_DH_EX_EPHEM                                (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EX_EPHEM)

#define CALG_DH_EL_SF                                   (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EL_SF)

#define CALG_DH_EL_EPHEM                                (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EL_EPHEM)

/*! @} */
/*! \defgroup PROV_TYPE PROV_TYPE
 *  \brief The description of PROV_MAGPRO_GOST
 *
 * @{
 */
#define PROV_MAGPRO_GOST            501
#define MAGPRO_CSP_A                "MagPro CSP"
#define MAGPRO_CSP_W                L"MagPro CSP"
#ifdef UNICODE
#define MAGPRO_CSP MAGPRO_CSP_W
#else
#define MAGPRO_CSP MAGPRO_CSP_A
#endif

#define PROV_CRYPTOPRO_GOST         75
#define CRYPTOPRO_CSP_A             "CryptoPro CSP"
#define CRYPTOPRO_CSP_W             L"CryptoPro CSP"
#ifdef UNICODE
#define CRYPTOPRO_CSP CRYPTOPRO_CSP_W
#else
#define CRYPTOPRO_CSP CRYPTOPRO_CSP_A
#endif

/*! @} */
/*! \defgroup PP_MAGPRO PP_MAGPRO
 *
 * @{
 */

#define PP_RNGTYPE                      201
#define PP_RNGSHARED            202
#define PP_SETUP_UI                     203

/*! @} */

#endif //CRYPTOCOM_CSP_CALG_H
