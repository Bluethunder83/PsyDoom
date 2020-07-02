#include "cdmaptbl.h"

// The start sector index and size of each file in the game on the CD-ROM.
//
// Note that the start sector DOES NOT include the 150 sector 'lead-in/toc' track on the CD-ROM.
// Therefore you must add 150 sectors to this count to get the actual physical sector number.
//
const PsxCd_MapTblEntry CD_MAP_TBL[] = {
    { 0x0018, 0x44     },   // SYSTEM_CNF
    { 0x0019, 0xDD800  },   // SLUS_000_77
    { 0x01D5, 0x8000   },   // DEMO1_LMP
    { 0x01E5, 0x8000   },   // DEMO2_LMP
    { 0x01F5, 0xA07F0  },   // MAPSPR60_IMG
    { 0x0336, 0x311520 },   // MOVIE_STR
    { 0x0897, 0x68800  },   // PSXDOOM_EXE
    { 0x0968, 0x4955C8 },   // PSXDOOM_WAD
    { 0x1295, 0x6E24   },   // MAP01_WAD
    { 0x12A3, 0xE3C0   },   // MAP02_WAD
    { 0x12C0, 0xD518   },   // MAP03_WAD
    { 0x12DB, 0xB42C   },   // MAP04_WAD
    { 0x12F2, 0xAF2C   },   // MAP05_WAD
    { 0x1308, 0xF094   },   // MAP06_WAD
    { 0x1327, 0xD73C   },   // MAP07_WAD
    { 0x1342, 0x5F60   },   // MAP08_WAD
    { 0x134E, 0x90208  },   // MAPSPR01_IMG
    { 0x146F, 0x8382C  },   // MAPSPR02_IMG
    { 0x1577, 0x839F4  },   // MAPSPR03_IMG
    { 0x167F, 0x926FC  },   // MAPSPR04_IMG
    { 0x17A4, 0x84790  },   // MAPSPR05_IMG
    { 0x18AD, 0x83DBC  },   // MAPSPR06_IMG
    { 0x19B5, 0x94F68  },   // MAPSPR07_IMG
    { 0x1ADF, 0x8AF28  },   // MAPSPR08_IMG
    { 0x1BF5, 0x16278  },   // MAPTEX01_IMG
    { 0x1C22, 0x1815C  },   // MAPTEX02_IMG
    { 0x1C53, 0x153F8  },   // MAPTEX03_IMG
    { 0x1C7E, 0x15E80  },   // MAPTEX04_IMG
    { 0x1CAA, 0x1797C  },   // MAPTEX05_IMG
    { 0x1CDA, 0x18ECC  },   // MAPTEX06_IMG
    { 0x1D0C, 0x171A8  },   // MAPTEX07_IMG
    { 0x1D3B, 0x15BC0  },   // MAPTEX08_IMG
    { 0x1D68, 0x5BD8   },   // MAP09_WAD
    { 0x1D74, 0xB860   },   // MAP10_WAD
    { 0x1D8C, 0x9100   },   // MAP11_WAD
    { 0x1D9F, 0xE664   },   // MAP12_WAD
    { 0x1DBC, 0xA1EC   },   // MAP13_WAD
    { 0x1DD1, 0xA310   },   // MAP14_WAD
    { 0x1DE6, 0x9E04   },   // MAP15_WAD
    { 0x1DFA, 0x339C   },   // MAP16_WAD
    { 0x1E01, 0xA65B8  },   // MAPSPR09_IMG
    { 0x1F4E, 0x836FC  },   // MAPSPR10_IMG
    { 0x2055, 0xCCDAC  },   // MAPSPR11_IMG
    { 0x21EF, 0xADFF4  },   // MAPSPR12_IMG
    { 0x234B, 0xC721C  },   // MAPSPR13_IMG
    { 0x24DA, 0xCF738  },   // MAPSPR14_IMG
    { 0x2679, 0xCFE20  },   // MAPSPR15_IMG
    { 0x2819, 0x88DB4  },   // MAPSPR16_IMG
    { 0x292B, 0x1C1E0  },   // MAPTEX09_IMG
    { 0x2964, 0xFC9C   },   // MAPTEX10_IMG
    { 0x2984, 0x1CBC4  },   // MAPTEX11_IMG
    { 0x29BE, 0x17724  },   // MAPTEX12_IMG
    { 0x29ED, 0x1C0C0  },   // MAPTEX13_IMG
    { 0x2A26, 0x188EC  },   // MAPTEX14_IMG
    { 0x2A58, 0x1CD0C  },   // MAPTEX15_IMG
    { 0x2A92, 0x12758  },   // MAPTEX16_IMG
    { 0x2AB8, 0x6650   },   // MAP17_WAD
    { 0x2AC5, 0x881C   },   // MAP18_WAD
    { 0x2AD7, 0x9254   },   // MAP19_WAD
    { 0x2AEA, 0x10800  },   // MAP20_WAD
    { 0x2B0B, 0xADF8   },   // MAP21_WAD
    { 0x2B21, 0xD3BC   },   // MAP22_WAD
    { 0x2B3C, 0x3F70   },   // MAP23_WAD
    { 0x2B44, 0x62A8   },   // MAP24_WAD
    { 0x2B51, 0xA0234  },   // MAPSPR17_IMG
    { 0x2C92, 0xB83B0  },   // MAPSPR18_IMG
    { 0x2E03, 0xADCF0  },   // MAPSPR19_IMG
    { 0x2F5F, 0x94BD0  },   // MAPSPR20_IMG
    { 0x3089, 0xE7438  },   // MAPSPR21_IMG
    { 0x3258, 0xAA82C  },   // MAPSPR22_IMG
    { 0x33AE, 0xBF3BC  },   // MAPSPR23_IMG
    { 0x352D, 0x7AE40  },   // MAPSPR24_IMG
    { 0x3623, 0x171EC  },   // MAPTEX17_IMG
    { 0x3652, 0x185F0  },   // MAPTEX18_IMG
    { 0x3683, 0x16038  },   // MAPTEX19_IMG
    { 0x36B0, 0x1B9BC  },   // MAPTEX20_IMG
    { 0x36E8, 0x1D10C  },   // MAPTEX21_IMG
    { 0x3723, 0x18DC0  },   // MAPTEX22_IMG
    { 0x3755, 0x16D44  },   // MAPTEX23_IMG
    { 0x3783, 0x1DD94  },   // MAPTEX24_IMG
    { 0x37C0, 0x8CFC   },   // MAP25_WAD
    { 0x37D2, 0xD7BC   },   // MAP26_WAD
    { 0x37ED, 0x96DC   },   // MAP27_WAD
    { 0x3800, 0x116E0  },   // MAP28_WAD
    { 0x3823, 0x13514  },   // MAP29_WAD
    { 0x384A, 0x11420  },   // MAP30_WAD
    { 0x386D, 0x5BFC   },   // MAP31_WAD
    { 0x3879, 0x7590   },   // MAP32_WAD
    { 0x3888, 0xA0D08  },   // MAPSPR25_IMG
    { 0x39CA, 0xC6258  },   // MAPSPR26_IMG
    { 0x3B57, 0x96B04  },   // MAPSPR27_IMG
    { 0x3C85, 0xAE3AC  },   // MAPSPR28_IMG
    { 0x3DE2, 0xA8A54  },   // MAPSPR29_IMG
    { 0x3F34, 0xA0C48  },   // MAPSPR30_IMG
    { 0x4076, 0x4A29C  },   // MAPSPR31_IMG
    { 0x410B, 0x6EC64  },   // MAPSPR32_IMG
    { 0x41E9, 0x1D6C8  },   // MAPTEX25_IMG
    { 0x4224, 0x17580  },   // MAPTEX26_IMG
    { 0x4253, 0x20C00  },   // MAPTEX27_IMG
    { 0x4295, 0x21E28  },   // MAPTEX28_IMG
    { 0x42D9, 0x1C44C  },   // MAPTEX29_IMG
    { 0x4312, 0x1A474  },   // MAPTEX30_IMG
    { 0x4347, 0x1B2C8  },   // MAPTEX31_IMG
    { 0x437E, 0x18658  },   // MAPTEX32_IMG
    { 0x43B0, 0x8678   },   // MAP33_WAD
    { 0x43C1, 0x7C44   },   // MAP34_WAD
    { 0x43D1, 0xD3F8   },   // MAP35_WAD
    { 0x43EC, 0xF164   },   // MAP36_WAD
    { 0x440B, 0x31D8   },   // MAP37_WAD
    { 0x4412, 0x9DDC   },   // MAP38_WAD
    { 0x4426, 0xB560   },   // MAP39_WAD
    { 0x443D, 0xFDCC   },   // MAP40_WAD
    { 0x445D, 0x82F38  },   // MAPSPR33_IMG
    { 0x4563, 0x84064  },   // MAPSPR34_IMG
    { 0x466C, 0xC01E8  },   // MAPSPR35_IMG
    { 0x47ED, 0xBEC18  },   // MAPSPR36_IMG
    { 0x496B, 0x89A10  },   // MAPSPR37_IMG
    { 0x4A7F, 0xD9A50  },   // MAPSPR38_IMG
    { 0x4C33, 0xE1EB0  },   // MAPSPR39_IMG
    { 0x4DF7, 0xB14D8  },   // MAPSPR40_IMG
    { 0x4F5A, 0x16AA0  },   // MAPTEX33_IMG
    { 0x4F88, 0x13738  },   // MAPTEX34_IMG
    { 0x4FAF, 0x17594  },   // MAPTEX35_IMG
    { 0x4FDE, 0x18E1C  },   // MAPTEX36_IMG
    { 0x5010, 0x108A0  },   // MAPTEX37_IMG
    { 0x5032, 0x2117C  },   // MAPTEX38_IMG
    { 0x5075, 0x1AC64  },   // MAPTEX39_IMG
    { 0x50AB, 0x1A9A8  },   // MAPTEX40_IMG
    { 0x50E2, 0xDD4C   },   // MAP41_WAD
    { 0x50FE, 0xCF24   },   // MAP42_WAD
    { 0x5118, 0x15C84  },   // MAP43_WAD
    { 0x5144, 0xD520   },   // MAP44_WAD
    { 0x515F, 0x12F58  },   // MAP45_WAD
    { 0x5185, 0xD310   },   // MAP46_WAD
    { 0x51A0, 0x16E9C  },   // MAP47_WAD
    { 0x51CE, 0x6E38   },   // MAP48_WAD
    { 0x51DC, 0xB6CB4  },   // MAPSPR41_IMG
    { 0x534A, 0xC5614  },   // MAPSPR42_IMG
    { 0x54D5, 0x8DF58  },   // MAPSPR43_IMG
    { 0x55F1, 0xCDC88  },   // MAPSPR44_IMG
    { 0x578D, 0xA5534  },   // MAPSPR45_IMG
    { 0x58D8, 0xC592C  },   // MAPSPR46_IMG
    { 0x5A64, 0x96D38  },   // MAPSPR47_IMG
    { 0x5B92, 0xD7200  },   // MAPSPR48_IMG
    { 0x5D41, 0x201A8  },   // MAPTEX41_IMG
    { 0x5D82, 0x211D8  },   // MAPTEX42_IMG
    { 0x5DC5, 0x1B9AC  },   // MAPTEX43_IMG
    { 0x5DFD, 0x1F650  },   // MAPTEX44_IMG
    { 0x5E3C, 0x21154  },   // MAPTEX45_IMG
    { 0x5E7F, 0x2185C  },   // MAPTEX46_IMG
    { 0x5EC3, 0x1F324  },   // MAPTEX47_IMG
    { 0x5F02, 0x24224  },   // MAPTEX48_IMG
    { 0x5F4C, 0x8960   },   // MAP49_WAD
    { 0x5F5E, 0x8048   },   // MAP50_WAD
    { 0x5F6F, 0xCF80   },   // MAP51_WAD
    { 0x5F89, 0xE98C   },   // MAP52_WAD
    { 0x5FA7, 0xFA24   },   // MAP53_WAD
    { 0x5FC7, 0x5870   },   // MAP54_WAD
    { 0x5FD3, 0x2664   },   // MAP55_WAD
    { 0x5FD8, 0xA000   },   // MAP56_WAD
    { 0x5FEC, 0x8308C  },   // MAPSPR49_IMG
    { 0x60F3, 0xD23D0  },   // MAPSPR50_IMG
    { 0x6298, 0xC684C  },   // MAPSPR51_IMG
    { 0x6426, 0xB4640  },   // MAPSPR52_IMG
    { 0x658F, 0xBA7CC  },   // MAPSPR53_IMG
    { 0x6704, 0xDB764  },   // MAPSPR54_IMG
    { 0x68BB, 0x661F0  },   // MAPSPR55_IMG
    { 0x6988, 0x92CF0  },   // MAPSPR56_IMG
    { 0x6AAE, 0x2038C  },   // MAPTEX49_IMG
    { 0x6AEF, 0x237BC  },   // MAPTEX50_IMG
    { 0x6B36, 0x27754  },   // MAPTEX51_IMG
    { 0x6B85, 0x1F750  },   // MAPTEX52_IMG
    { 0x6BC4, 0x2612C  },   // MAPTEX53_IMG
    { 0x6C11, 0x19FAC  },   // MAPTEX54_IMG
    { 0x6C45, 0xC4E8   },   // MAPTEX55_IMG
    { 0x6C5E, 0x15228  },   // MAPTEX56_IMG
    { 0x6C8A, 0x784C   },   // MAP57_WAD
    { 0x6C9A, 0x1673C  },   // MAP58_WAD
    { 0x6CC7, 0x92D8   },   // MAP59_WAD
    { 0x6CDA, 0x8000   },   // MAP60_PAD
    { 0x6CEA, 0x8000   },   // MAP61_PAD
    { 0x6CFA, 0x8000   },   // MAP62_PAD
    { 0x6D0A, 0x8000   },   // MAP63_PAD
    { 0x6D1A, 0x8000   },   // MAP64_PAD
    { 0x6D2A, 0xA82AC  },   // MAPSPR57_IMG
    { 0x6E7B, 0xA16A4  },   // MAPSPR58_IMG
    { 0x6FBE, 0x65E14  },   // MAPSPR59_IMG
    { 0x708A, 0x8000   },   // MAPSPR60_PAD
    { 0x709A, 0x8000   },   // MAPSPR61_PAD
    { 0x70AA, 0x8000   },   // MAPSPR62_PAD
    { 0x70BA, 0x8000   },   // MAPSPR63_PAD
    { 0x70CA, 0x8000   },   // MAPSPR64_PAD
    { 0x70DA, 0x1CAD8  },   // MAPTEX57_IMG
    { 0x7114, 0x201D8  },   // MAPTEX58_IMG
    { 0x7155, 0x1E674  },   // MAPTEX59_IMG
    { 0x7192, 0x8000   },   // MAPTEX60_PAD
    { 0x71A2, 0x8000   },   // MAPTEX61_PAD
    { 0x71B2, 0x8000   },   // MAPTEX62_PAD
    { 0x71C2, 0x8000   },   // MAPTEX63_PAD
    { 0x71D2, 0x8000   },   // MAPTEX64_PAD
    { 0x71E3, 0x2B3D0  },   // DOOMSFX_LCD
    { 0x723A, 0xD8CE   },   // DOOMSFX_WMD
    { 0x7256, 0xEF30   },   // MUSLEV1_LCD
    { 0x7274, 0x12DE0  },   // MUSLEV10_LCD
    { 0x729A, 0x192D0  },   // MUSLEV11_LCD
    { 0x72CD, 0x1B560  },   // MUSLEV12_LCD
    { 0x7304, 0x120F0  },   // MUSLEV13_LCD
    { 0x7329, 0x198D0  },   // MUSLEV14_LCD
    { 0x735D, 0x14DF0  },   // MUSLEV15_LCD
    { 0x7387, 0x11560  },   // MUSLEV16_LCD
    { 0x73AA, 0x14680  },   // MUSLEV17_LCD
    { 0x73D3, 0x16BD0  },   // MUSLEV18_LCD
    { 0x7401, 0x16970  },   // MUSLEV19_LCD
    { 0x742F, 0x106D0  },   // MUSLEV2_LCD
    { 0x7450, 0x1D8A0  },   // MUSLEV20_LCD
    { 0x748C, 0x21530  },   // MUSLEV3_LCD
    { 0x74CF, 0x193A0  },   // MUSLEV4_LCD
    { 0x7502, 0x1C720  },   // MUSLEV5_LCD
    { 0x753B, 0x18B30  },   // MUSLEV6_LCD
    { 0x756D, 0x19850  },   // MUSLEV7_LCD
    { 0x75A1, 0x1C8D0  },   // MUSLEV8_LCD
    { 0x75DB, 0x1D980  },   // MUSLEV9_LCD
    { 0x7618, 0x23110  },   // MAP01_LCD
    { 0x765F, 0x22FD0  },   // MAP02_LCD
    { 0x76A5, 0x22FD0  },   // MAP03_LCD
    { 0x76EB, 0x290B0  },   // MAP04_LCD
    { 0x773E, 0x22FD0  },   // MAP05_LCD
    { 0x7784, 0x22FD0  },   // MAP06_LCD
    { 0x77CA, 0x2CF60  },   // MAP07_LCD
    { 0x7824, 0x20320  },   // MAP08_LCD
    { 0x7865, 0x30CF0  },   // MAP09_LCD
    { 0x78C7, 0x26B70  },   // MAP10_LCD
    { 0x7915, 0x38100  },   // MAP11_LCD
    { 0x7986, 0x31580  },   // MAP12_LCD
    { 0x79E9, 0x37660  },   // MAP13_LCD
    { 0x7A58, 0x3B510  },   // MAP14_LCD
    { 0x7ACF, 0x39B10  },   // MAP15_LCD
    { 0x7B43, 0x20E90  },   // MAP16_LCD
    { 0x7B85, 0x31690  },   // MAP17_LCD
    { 0x7BE8, 0x359F0  },   // MAP18_LCD
    { 0x7C54, 0x31580  },   // MAP19_LCD
    { 0x7CB7, 0x26010  },   // MAP20_LCD
    { 0x7D05, 0x3C4F0  },   // MAP21_LCD
    { 0x7D7E, 0x29BD0  },   // MAP22_LCD
    { 0x7DD2, 0x217F0  },   // MAP23_LCD
    { 0x7E15, 0x27440  },   // MAP24_LCD
    { 0x7E64, 0x30CF0  },   // MAP25_LCD
    { 0x7EC6, 0x359F0  },   // MAP26_LCD
    { 0x7F32, 0x2C400  },   // MAP27_LCD
    { 0x7F8B, 0x30CF0  },   // MAP28_LCD
    { 0x7FED, 0x2C400  },   // MAP29_LCD
    { 0x8046, 0x326A0  },   // MAP30_LCD
    { 0x80AB, 0x1B360  },   // MAP31_LCD
    { 0x80E2, 0x22FD0  },   // MAP32_LCD
    { 0x8128, 0x22FD0  },   // MAP33_LCD
    { 0x816E, 0x22FD0  },   // MAP34_LCD
    { 0x81B4, 0x30CF0  },   // MAP35_LCD
    { 0x8216, 0x36550  },   // MAP36_LCD
    { 0x8283, 0x1DD80  },   // MAP37_LCD
    { 0x82BF, 0x26A90  },   // MAP38_LCD
    { 0x830D, 0x3AC80  },   // MAP39_LCD
    { 0x8383, 0x33400  },   // MAP40_LCD
    { 0x83EB, 0x2E280  },   // MAP41_LCD
    { 0x8448, 0x39BC0  },   // MAP42_LCD
    { 0x84BC, 0x28150  },   // MAP43_LCD
    { 0x850D, 0x35A90  },   // MAP44_LCD
    { 0x8579, 0x27700  },   // MAP45_LCD
    { 0x85C8, 0x35B00  },   // MAP46_LCD
    { 0x8634, 0x2C880  },   // MAP47_LCD
    { 0x868E, 0x374B0  },   // MAP48_LCD
    { 0x86FD, 0x27440  },   // MAP49_LCD
    { 0x874C, 0x3D0D0  },   // MAP50_LCD
    { 0x87C7, 0x33140  },   // MAP51_LCD
    { 0x882E, 0x30CF0  },   // MAP52_LCD
    { 0x8890, 0x36550  },   // MAP53_LCD
    { 0x88FD, 0x15E70  },   // MAP54_LCD
    { 0x8929, 0x13C60  },   // MAP55_LCD
    { 0x8951, 0x290B0  },   // MAP56_LCD
    { 0x89A4, 0x1D660  },   // MAP57_LCD
    { 0x89DF, 0x326A0  },   // MAP58_LCD
    { 0x8A44, 0x11100  },   // MAP59_LCD
    { 0x8A67, 0x67FB0  },   // MAP60_LCD
    { 0x0000, 0x00000  },   // END
};

extern const char* const CD_MAP_FILENAMES[] = {
    "SYSTEM.CNF",
    "SLUS_000.77",
    "DEMO1.LMP",
    "DEMO2.LMP",
    "MAPSPR60.IMG",
    "MOVIE.STR",
    "PSXDOOM.EXE",
    "PSXDOOM.WAD",
    "MAP01.WAD",
    "MAP02.WAD",
    "MAP03.WAD",
    "MAP04.WAD",
    "MAP05.WAD",
    "MAP06.WAD",
    "MAP07.WAD",
    "MAP08.WAD",
    "MAPSPR01.IMG",
    "MAPSPR02.IMG",
    "MAPSPR03.IMG",
    "MAPSPR04.IMG",
    "MAPSPR05.IMG",
    "MAPSPR06.IMG",
    "MAPSPR07.IMG",
    "MAPSPR08.IMG",
    "MAPTEX01.IMG",
    "MAPTEX02.IMG",
    "MAPTEX03.IMG",
    "MAPTEX04.IMG",
    "MAPTEX05.IMG",
    "MAPTEX06.IMG",
    "MAPTEX07.IMG",
    "MAPTEX08.IMG",
    "MAP09.WAD",
    "MAP10.WAD",
    "MAP11.WAD",
    "MAP12.WAD",
    "MAP13.WAD",
    "MAP14.WAD",
    "MAP15.WAD",
    "MAP16.WAD",
    "MAPSPR09.IMG",
    "MAPSPR10.IMG",
    "MAPSPR11.IMG",
    "MAPSPR12.IMG",
    "MAPSPR13.IMG",
    "MAPSPR14.IMG",
    "MAPSPR15.IMG",
    "MAPSPR16.IMG",
    "MAPTEX09.IMG",
    "MAPTEX10.IMG",
    "MAPTEX11.IMG",
    "MAPTEX12.IMG",
    "MAPTEX13.IMG",
    "MAPTEX14.IMG",
    "MAPTEX15.IMG",
    "MAPTEX16.IMG",
    "MAP17.WAD",
    "MAP18.WAD",
    "MAP19.WAD",
    "MAP20.WAD",
    "MAP21.WAD",
    "MAP22.WAD",
    "MAP23.WAD",
    "MAP24.WAD",
    "MAPSPR17.IMG",
    "MAPSPR18.IMG",
    "MAPSPR19.IMG",
    "MAPSPR20.IMG",
    "MAPSPR21.IMG",
    "MAPSPR22.IMG",
    "MAPSPR23.IMG",
    "MAPSPR24.IMG",
    "MAPTEX17.IMG",
    "MAPTEX18.IMG",
    "MAPTEX19.IMG",
    "MAPTEX20.IMG",
    "MAPTEX21.IMG",
    "MAPTEX22.IMG",
    "MAPTEX23.IMG",
    "MAPTEX24.IMG",
    "MAP25.WAD",
    "MAP26.WAD",
    "MAP27.WAD",
    "MAP28.WAD",
    "MAP29.WAD",
    "MAP30.WAD",
    "MAP31.WAD",
    "MAP32.WAD",
    "MAPSPR25.IMG",
    "MAPSPR26.IMG",
    "MAPSPR27.IMG",
    "MAPSPR28.IMG",
    "MAPSPR29.IMG",
    "MAPSPR30.IMG",
    "MAPSPR31.IMG",
    "MAPSPR32.IMG",
    "MAPTEX25.IMG",
    "MAPTEX26.IMG",
    "MAPTEX27.IMG",
    "MAPTEX28.IMG",
    "MAPTEX29.IMG",
    "MAPTEX30.IMG",
    "MAPTEX31.IMG",
    "MAPTEX32.IMG",
    "MAP33.WAD",
    "MAP34.WAD",
    "MAP35.WAD",
    "MAP36.WAD",
    "MAP37.WAD",
    "MAP38.WAD",
    "MAP39.WAD",
    "MAP40.WAD",
    "MAPSPR33.IMG",
    "MAPSPR34.IMG",
    "MAPSPR35.IMG",
    "MAPSPR36.IMG",
    "MAPSPR37.IMG",
    "MAPSPR38.IMG",
    "MAPSPR39.IMG",
    "MAPSPR40.IMG",
    "MAPTEX33.IMG",
    "MAPTEX34.IMG",
    "MAPTEX35.IMG",
    "MAPTEX36.IMG",
    "MAPTEX37.IMG",
    "MAPTEX38.IMG",
    "MAPTEX39.IMG",
    "MAPTEX40.IMG",
    "MAP41.WAD",
    "MAP42.WAD",
    "MAP43.WAD",
    "MAP44.WAD",
    "MAP45.WAD",
    "MAP46.WAD",
    "MAP47.WAD",
    "MAP48.WAD",
    "MAPSPR41.IMG",
    "MAPSPR42.IMG",
    "MAPSPR43.IMG",
    "MAPSPR44.IMG",
    "MAPSPR45.IMG",
    "MAPSPR46.IMG",
    "MAPSPR47.IMG",
    "MAPSPR48.IMG",
    "MAPTEX41.IMG",
    "MAPTEX42.IMG",
    "MAPTEX43.IMG",
    "MAPTEX44.IMG",
    "MAPTEX45.IMG",
    "MAPTEX46.IMG",
    "MAPTEX47.IMG",
    "MAPTEX48.IMG",
    "MAP49.WAD",
    "MAP50.WAD",
    "MAP51.WAD",
    "MAP52.WAD",
    "MAP53.WAD",
    "MAP54.WAD",
    "MAP55.WAD",
    "MAP56.WAD",
    "MAPSPR49.IMG",
    "MAPSPR50.IMG",
    "MAPSPR51.IMG",
    "MAPSPR52.IMG",
    "MAPSPR53.IMG",
    "MAPSPR54.IMG",
    "MAPSPR55.IMG",
    "MAPSPR56.IMG",
    "MAPTEX49.IMG",
    "MAPTEX50.IMG",
    "MAPTEX51.IMG",
    "MAPTEX52.IMG",
    "MAPTEX53.IMG",
    "MAPTEX54.IMG",
    "MAPTEX55.IMG",
    "MAPTEX56.IMG",
    "MAP57.WAD",
    "MAP58.WAD",
    "MAP59.WAD",
    "MAP60.PAD",
    "MAP61.PAD",
    "MAP62.PAD",
    "MAP63.PAD",
    "MAP64.PAD",
    "MAPSPR57.IMG",
    "MAPSPR58.IMG",
    "MAPSPR59.IMG",
    "MAPSPR60.PAD",
    "MAPSPR61.PAD",
    "MAPSPR62.PAD",
    "MAPSPR63.PAD",
    "MAPSPR64.PAD",
    "MAPTEX57.IMG",
    "MAPTEX58.IMG",
    "MAPTEX59.IMG",
    "MAPTEX60.PAD",
    "MAPTEX61.PAD",
    "MAPTEX62.PAD",
    "MAPTEX63.PAD",
    "MAPTEX64.PAD",
    "DOOMSFX.LCD",
    "DOOMSFX.WMD",
    "MUSLEV1.LCD",
    "MUSLEV10.LCD",
    "MUSLEV11.LCD",
    "MUSLEV12.LCD",
    "MUSLEV13.LCD",
    "MUSLEV14.LCD",
    "MUSLEV15.LCD",
    "MUSLEV16.LCD",
    "MUSLEV17.LCD",
    "MUSLEV18.LCD",
    "MUSLEV19.LCD",
    "MUSLEV2.LCD",
    "MUSLEV20.LCD",
    "MUSLEV3.LCD",
    "MUSLEV4.LCD",
    "MUSLEV5.LCD",
    "MUSLEV6.LCD",
    "MUSLEV7.LCD",
    "MUSLEV8.LCD",
    "MUSLEV9.LCD",
    "MAP01.LCD",
    "MAP02.LCD",
    "MAP03.LCD",
    "MAP04.LCD",
    "MAP05.LCD",
    "MAP06.LCD",
    "MAP07.LCD",
    "MAP08.LCD",
    "MAP09.LCD",
    "MAP10.LCD",
    "MAP11.LCD",
    "MAP12.LCD",
    "MAP13.LCD",
    "MAP14.LCD",
    "MAP15.LCD",
    "MAP16.LCD",
    "MAP17.LCD",
    "MAP18.LCD",
    "MAP19.LCD",
    "MAP20.LCD",
    "MAP21.LCD",
    "MAP22.LCD",
    "MAP23.LCD",
    "MAP24.LCD",
    "MAP25.LCD",
    "MAP26.LCD",
    "MAP27.LCD",
    "MAP28.LCD",
    "MAP29.LCD",
    "MAP30.LCD",
    "MAP31.LCD",
    "MAP32.LCD",
    "MAP33.LCD",
    "MAP34.LCD",
    "MAP35.LCD",
    "MAP36.LCD",
    "MAP37.LCD",
    "MAP38.LCD",
    "MAP39.LCD",
    "MAP40.LCD",
    "MAP41.LCD",
    "MAP42.LCD",
    "MAP43.LCD",
    "MAP44.LCD",
    "MAP45.LCD",
    "MAP46.LCD",
    "MAP47.LCD",
    "MAP48.LCD",
    "MAP49.LCD",
    "MAP50.LCD",
    "MAP51.LCD",
    "MAP52.LCD",
    "MAP53.LCD",
    "MAP54.LCD",
    "MAP55.LCD",
    "MAP56.LCD",
    "MAP57.LCD",
    "MAP58.LCD",
    "MAP59.LCD",
    "MAP60.LCD",
    ""
};
