// READ ME FIRST 闭环
// THE REGISTER SETTINGS PROVIDED BELOW ARE GIVEN BY AXIGN BV TO THE
// CUSTOMER ON A BEST EFFORT BASIS AND SHOULD ONLY BE TREATED AS AXIGN’S
// ADVICE TO THE CUSTOMER HOW TO PROGRAM AXIGN’S PRODUCT IN ORDER TO BEST
// ACHIEVE THE DESIRED SPECIFICATIONS.
// IT IS THE CUSTOMER’S SOLE RESPONSIBILITY TO DECIDE WHAT VALUES WILL BE
// WRITTEN IN WHICH REGISTERS WHEN USING AXIGN’S PRODUCT IN THE CUSTOMER’S
// FINAL APPLICATION.
// IN NO EVENT SHALL AXIGN BV BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
// CONSEQUENTIAL, LOSS OF PROFITS OR OTHER INDIRECT DAMAGES OF ANY KIND, OR
// FOR EXEMPLARY OR PUNITIVE DAMAGES OF ANY KIND, WHETHER OR NOT THE
// CUSTOMER WAS ADVISED OF OR COULD FORESEE THE POSSIBILITY OF SUCH DAMAGES.
// NOR SHALL AXIGN BV BE LIABLE FOR ANY INJURY, LOSS OR DAMAGE, INCLUDING
// LOSS OF USE OR DESTRUCTION OF ANY PROPERTY, HOWSOEVER ARISING, THAT
// MAY BE SUSTAINED BY CUSTOMER OR BY ANY OTHER PERSON AS A RESULT OF OR
// IN CONNECTION WITH THE USE OF THE REGISTER SETTINGS.


// Array of register write commands to send to the AX5689
// Settings for AX5689 Reference Design 4 x MP1916, 8 x GaNFET
// Audio input format: I2S 2 channels 192 kHz sample rate 
// 2 x BTL, output filter: L = 4.7 uH, C = 2.2 uF
// ADC input resistors: 10000 Ohm (0.1 %)
// PVDD = 50
// Date: 2025/01/09 15:04:04

const uint16_t amplifierCommands[] = {
0x0012, 0x0371, // SerialDataReceiver
0x0013, 0x0000, // SerialDataRXChannelPointers1
0x0014, 0x1111, // SerialDataRXChannelPointers2
0x0000, 0x00FF, // Programmingmode
0x0042, 0x0002, // ChannelAvolumeVolumeRampstepsize
0x0040, 0x0000, // ChannelAvolumeVolumeRampvalue1
0x0041, 0x8FF6, // ChannelAvolumeVolumeRampvalue2
0x0000, 0x0000, // Programmingmode
0x0000, 0x0011, // Programmingmode
0x004D, 0x0000, // ChannelAloopfilterfeedin3PCMcoefficientstage1
0x004E, 0x0000, // ChannelAloopfilterfeedin3PCMcoefficientstage2
0x004F, 0x0000, // ChannelAloopfilterfeedin3PCMcoefficientstage3
0x0050, 0xBDF7, // ChannelAloopfilterfeedin3PCMcoefficientstage4
0x0051, 0xFF04, // ChannelAloopfilterfeedin3PCMcoefficientstage5
0x0052, 0xFDA2, // ChannelAloopfilterfeedin3PCMcoefficientstage6
0x0053, 0xFFDD, // ChannelAloopfilterfeedin3PCMcoefficientstage7
0x0054, 0x0004, // ChannelAloopfilterfeedoutStageoutenable
0x0055, 0x0000, // ChannelAloopfilterfeedoutInputFFcoefficient
0x0056, 0x0001, // ChannelAloopfilterstage1FFcoefficientstage1
0x0057, 0x0945, // ChannelAloopfilterstage1FBcoefficientstage1
0x0058, 0x0203, // ChannelAloopfilterstage1MACcontrolstage1
0x0059, 0x0A61, // ChannelAloopfilterstage2FFcoefficientstage2
0x005A, 0x0465, // ChannelAloopfilterstage2FBcoefficientstage2
0x005B, 0x0203, // ChannelAloopfilterstage2MACcontrolstage2
0x005C, 0x0BB3, // ChannelAloopfilterstage3FFcoefficientstage3
0x005D, 0x0BA3, // ChannelAloopfilterstage3FBcoefficientstage3
0x005E, 0x0203, // ChannelAloopfilterstage3MACcontrolstage3
0x005F, 0x0001, // ChannelAloopfilterstage4FFcoefficientstage4
0x0060, 0x0557, // ChannelAloopfilterstage4FBcoefficientstage4
0x0061, 0x0201, // ChannelAloopfilterstage4MACcontrolstage4
0x0062, 0x0A47, // ChannelAloopfilterstage5FFcoefficientstage5
0x0063, 0x09B8, // ChannelAloopfilterstage5FBcoefficientstage5
0x0064, 0x0201, // ChannelAloopfilterstage5MACcontrolstage5
0x0065, 0x0A18, // ChannelAloopfilterstage6FFcoefficientstage6
0x0066, 0x04CB, // ChannelAloopfilterstage6FBcoefficientstage6
0x0067, 0x0201, // ChannelAloopfilterstage6MACcontrolstage6
0x0068, 0x0BB3, // ChannelAloopfilterstage7FFcoefficientstage7
0x0069, 0x0837, // ChannelAloopfilterstage7FBcoefficientstage7
0x006A, 0x021F, // ChannelAloopfilterstage7MACcontrolstage7
0x0000, 0x0000, // Programmingmode
0x0000, 0x0022, // Programmingmode
0x008D, 0xC14B, // ChannelBloopfilterfeedin3PCMcoefficientstage1
0x008E, 0xBD6E, // ChannelBloopfilterfeedin3PCMcoefficientstage2
0x008F, 0x1A98, // ChannelBloopfilterfeedin3PCMcoefficientstage3
0x0090, 0xFF84, // ChannelBloopfilterfeedin3PCMcoefficientstage4
0x0091, 0x0002, // ChannelBloopfilterfeedin3PCMcoefficientstage5
0x0092, 0x0000, // ChannelBloopfilterfeedin3PCMcoefficientstage6
0x0093, 0x0000, // ChannelBloopfilterfeedin3PCMcoefficientstage7
0x0094, 0x0000, // ChannelBloopfilterfeedoutStageoutenable
0x0095, 0x0000, // ChannelBloopfilterfeedoutInputFFcoefficient
0x0096, 0x0001, // ChannelBloopfilterstage1FFcoefficientstage1
0x0097, 0x0A97, // ChannelBloopfilterstage1FBcoefficientstage1
0x0098, 0x0211, // ChannelBloopfilterstage1MACcontrolstage1
0x0099, 0x05B7, // ChannelBloopfilterstage2FFcoefficientstage2
0x009A, 0x0546, // ChannelBloopfilterstage2FBcoefficientstage2
0x009B, 0x0211, // ChannelBloopfilterstage2MACcontrolstage2
0x009C, 0x0A58, // ChannelBloopfilterstage3FFcoefficientstage3
0x009D, 0x0B08, // ChannelBloopfilterstage3FBcoefficientstage3
0x009E, 0x0211, // ChannelBloopfilterstage3MACcontrolstage3
0x009F, 0x0B97, // ChannelBloopfilterstage4FFcoefficientstage4
0x00A0, 0x0417, // ChannelBloopfilterstage4FBcoefficientstage4
0x00A1, 0x0211, // ChannelBloopfilterstage4MACcontrolstage4
0x00A2, 0x0897, // ChannelBloopfilterstage5FFcoefficientstage5
0x00A3, 0x0927, // ChannelBloopfilterstage5FBcoefficientstage5
0x00A4, 0x0211, // ChannelBloopfilterstage5MACcontrolstage5
0x00A5, 0x06C8, // ChannelBloopfilterstage6FFcoefficientstage6
0x00A6, 0x05E8, // ChannelBloopfilterstage6FBcoefficientstage6
0x00A7, 0x0211, // ChannelBloopfilterstage6MACcontrolstage6
0x00A8, 0x0AE6, // ChannelBloopfilterstage7FFcoefficientstage7
0x00A9, 0x0B16, // ChannelBloopfilterstage7FBcoefficientstage7
0x00AA, 0x0211, // ChannelBloopfilterstage7MACcontrolstage7
0x0000, 0x0000, // Programmingmode
0x0000, 0x0044, // Programmingmode
0x00CD, 0x0000, // ChannelCloopfilterfeedin3PCMcoefficientstage1
0x00CE, 0x0000, // ChannelCloopfilterfeedin3PCMcoefficientstage2
0x00CF, 0x0000, // ChannelCloopfilterfeedin3PCMcoefficientstage3
0x00D0, 0x0000, // ChannelCloopfilterfeedin3PCMcoefficientstage4
0x00D1, 0x0000, // ChannelCloopfilterfeedin3PCMcoefficientstage5
0x00D2, 0x0000, // ChannelCloopfilterfeedin3PCMcoefficientstage6
0x00D3, 0x0000, // ChannelCloopfilterfeedin3PCMcoefficientstage7
0x00D4, 0x007F, // ChannelCloopfilterfeedoutStageoutenable
0x00D5, 0x0000, // ChannelCloopfilterfeedoutInputFFcoefficient
0x00D6, 0x0B70, // ChannelCloopfilterstage1FFcoefficientstage1
0x00D7, 0x0001, // ChannelCloopfilterstage1FBcoefficientstage1
0x00D8, 0x0215, // ChannelCloopfilterstage1MACcontrolstage1
0x00D9, 0x04B7, // ChannelCloopfilterstage2FFcoefficientstage2
0x00DA, 0x0BFB, // ChannelCloopfilterstage2FBcoefficientstage2
0x00DB, 0x0217, // ChannelCloopfilterstage2MACcontrolstage2
0x00DC, 0x0576, // ChannelCloopfilterstage3FFcoefficientstage3
0x00DD, 0x0FDC, // ChannelCloopfilterstage3FBcoefficientstage3
0x00DE, 0x0215, // ChannelCloopfilterstage3MACcontrolstage3
0x00DF, 0x07D8, // ChannelCloopfilterstage4FFcoefficientstage4
0x00E0, 0x0425, // ChannelCloopfilterstage4FBcoefficientstage4
0x00E1, 0x0217, // ChannelCloopfilterstage4MACcontrolstage4
0x00E2, 0x0B47, // ChannelCloopfilterstage5FFcoefficientstage5
0x00E3, 0x09A6, // ChannelCloopfilterstage5FBcoefficientstage5
0x00E4, 0x0219, // ChannelCloopfilterstage5MACcontrolstage5
0x00E5, 0x0816, // ChannelCloopfilterstage6FFcoefficientstage6
0x00E6, 0x0B56, // ChannelCloopfilterstage6FBcoefficientstage6
0x00E7, 0x0215, // ChannelCloopfilterstage6MACcontrolstage6
0x00E8, 0x066A, // ChannelCloopfilterstage7FFcoefficientstage7
0x00E9, 0x0F9C, // ChannelCloopfilterstage7FBcoefficientstage7
0x00EA, 0x0219, // ChannelCloopfilterstage7MACcontrolstage7
0x0000, 0x0000, // Programmingmode
0x0000, 0x0088, // Programmingmode
0x010D, 0xC14B, // ChannelDloopfilterfeedin3PCMcoefficientstage1
0x010E, 0x27E1, // ChannelDloopfilterfeedin3PCMcoefficientstage2
0x010F, 0xD501, // ChannelDloopfilterfeedin3PCMcoefficientstage3
0x0110, 0x00FD, // ChannelDloopfilterfeedin3PCMcoefficientstage4
0x0111, 0xFFFC, // ChannelDloopfilterfeedin3PCMcoefficientstage5
0x0112, 0x0000, // ChannelDloopfilterfeedin3PCMcoefficientstage6
0x0113, 0x0000, // ChannelDloopfilterfeedin3PCMcoefficientstage7
0x0114, 0x0040, // ChannelDloopfilterfeedoutStageoutenable
0x0115, 0x0000, // ChannelDloopfilterfeedoutInputFFcoefficient
0x0116, 0x0001, // ChannelDloopfilterstage1FFcoefficientstage1
0x0117, 0x0A97, // ChannelDloopfilterstage1FBcoefficientstage1
0x0118, 0x0211, // ChannelDloopfilterstage1MACcontrolstage1
0x0119, 0x0946, // ChannelDloopfilterstage2FFcoefficientstage2
0x011A, 0x0405, // ChannelDloopfilterstage2FBcoefficientstage2
0x011B, 0x0211, // ChannelDloopfilterstage2MACcontrolstage2
0x011C, 0x0889, // ChannelDloopfilterstage3FFcoefficientstage3
0x011D, 0x0B08, // ChannelDloopfilterstage3FBcoefficientstage3
0x011E, 0x0211, // ChannelDloopfilterstage3MACcontrolstage3
0x011F, 0x0B57, // ChannelDloopfilterstage4FFcoefficientstage4
0x0120, 0x0407, // ChannelDloopfilterstage4FBcoefficientstage4
0x0121, 0x0211, // ChannelDloopfilterstage4MACcontrolstage4
0x0122, 0x0887, // ChannelDloopfilterstage5FFcoefficientstage5
0x0123, 0x0927, // ChannelDloopfilterstage5FBcoefficientstage5
0x0124, 0x0211, // ChannelDloopfilterstage5MACcontrolstage5
0x0125, 0x0968, // ChannelDloopfilterstage6FFcoefficientstage6
0x0126, 0x0A4C, // ChannelDloopfilterstage6FBcoefficientstage6
0x0127, 0x0211, // ChannelDloopfilterstage6MACcontrolstage6
0x0128, 0x0532, // ChannelDloopfilterstage7FFcoefficientstage7
0x0129, 0x0B16, // ChannelDloopfilterstage7FBcoefficientstage7
0x012A, 0x0211, // ChannelDloopfilterstage7MACcontrolstage7
0x0000, 0x0000, // Programmingmode
0x0084, 0xAAAA, // ChannelBloopfilterfeedin1Inputmask1
0x0083, 0x0003, // ChannelBloopfilterfeedin1Inputcontrol1
0x0087, 0x0000, // ChannelBloopfilterfeedin1Ramp1stepsize
0x0085, 0x0000, // ChannelBloopfilterfeedin1Ramp1value1
0x0086, 0x0000, // ChannelBloopfilterfeedin1Ramp1value2
0x0089, 0xAAAA, // ChannelBloopfilterfeedin2Inputmask2
0x0088, 0x0003, // ChannelBloopfilterfeedin2Inputcontrol2
0x008C, 0x0000, // ChannelBloopfilterfeedin2Ramp2stepsize
0x008A, 0x0000, // ChannelBloopfilterfeedin2Ramp2value1
0x008B, 0x0000, // ChannelBloopfilterfeedin2Ramp2value2
0x0104, 0xAAAA, // ChannelDloopfilterfeedin1Inputmask1
0x0103, 0x0003, // ChannelDloopfilterfeedin1Inputcontrol1
0x0107, 0x0000, // ChannelDloopfilterfeedin1Ramp1stepsize
0x0105, 0x0000, // ChannelDloopfilterfeedin1Ramp1value1
0x0106, 0x0000, // ChannelDloopfilterfeedin1Ramp1value2
0x0109, 0xAAAA, // ChannelDloopfilterfeedin2Inputmask2
0x0108, 0x0003, // ChannelDloopfilterfeedin2Inputcontrol2
0x010C, 0x0000, // ChannelDloopfilterfeedin2Ramp2stepsize
0x010A, 0x0000, // ChannelDloopfilterfeedin2Ramp2value1
0x010B, 0x0000, // ChannelDloopfilterfeedin2Ramp2value2
0x0184, 0xAAAA, // ChannelFloopfilterfeedin1Inputmask1
0x0183, 0x0003, // ChannelFloopfilterfeedin1Inputcontrol1
0x0187, 0x0000, // ChannelFloopfilterfeedin1Ramp1stepsize
0x0185, 0x0000, // ChannelFloopfilterfeedin1Ramp1value1
0x0186, 0x0000, // ChannelFloopfilterfeedin1Ramp1value2
0x0189, 0xAAAA, // ChannelFloopfilterfeedin2Inputmask2
0x0188, 0x0003, // ChannelFloopfilterfeedin2Inputcontrol2
0x018C, 0x0000, // ChannelFloopfilterfeedin2Ramp2stepsize
0x018A, 0x0000, // ChannelFloopfilterfeedin2Ramp2value1
0x018B, 0x0000, // ChannelFloopfilterfeedin2Ramp2value2
0x0204, 0xAAAA, // ChannelHloopfilterfeedin1Inputmask1
0x0203, 0x0003, // ChannelHloopfilterfeedin1Inputcontrol1
0x0207, 0x0000, // ChannelHloopfilterfeedin1Ramp1stepsize
0x0205, 0x0000, // ChannelHloopfilterfeedin1Ramp1value1
0x0206, 0x0000, // ChannelHloopfilterfeedin1Ramp1value2
0x0209, 0xAAAA, // ChannelHloopfilterfeedin2Inputmask2
0x0208, 0x0003, // ChannelHloopfilterfeedin2Inputcontrol2
0x020C, 0x0000, // ChannelHloopfilterfeedin2Ramp2stepsize
0x020A, 0x0000, // ChannelHloopfilterfeedin2Ramp2value1
0x020B, 0x0000, // ChannelHloopfilterfeedin2Ramp2value2
0x0044, 0xAA00, // ChannelAloopfilterfeedin1Inputmask1
0x0043, 0x0741, // ChannelAloopfilterfeedin1Inputcontrol1
0x0047, 0x010D, // ChannelAloopfilterfeedin1Ramp1stepsize
0x0045, 0x0000, // ChannelAloopfilterfeedin1Ramp1value1
0x0046, 0x0800, // ChannelAloopfilterfeedin1Ramp1value2
0x0049, 0xAAAA, // ChannelAloopfilterfeedin2Inputmask2
0x0048, 0x0041, // ChannelAloopfilterfeedin2Inputcontrol2
0x004C, 0x0006, // ChannelAloopfilterfeedin2Ramp2stepsize
0x004A, 0x0000, // ChannelAloopfilterfeedin2Ramp2value1
0x004B, 0x002A, // ChannelAloopfilterfeedin2Ramp2value2
0x00C4, 0xAA11, // ChannelCloopfilterfeedin1Inputmask1
0x00C3, 0x0713, // ChannelCloopfilterfeedin1Inputcontrol1
0x00C7, 0x0096, // ChannelCloopfilterfeedin1Ramp1stepsize
0x00C5, 0x0000, // ChannelCloopfilterfeedin1Ramp1value1
0x00C6, 0xFB8F, // ChannelCloopfilterfeedin1Ramp1value2
0x00C9, 0xAA98, // ChannelCloopfilterfeedin2Inputmask2
0x00C8, 0x078C, // ChannelCloopfilterfeedin2Inputcontrol2
0x00CC, 0x0105, // ChannelCloopfilterfeedin2Ramp2stepsize
0x00CA, 0x0000, // ChannelCloopfilterfeedin2Ramp2value1
0x00CB, 0xF841, // ChannelCloopfilterfeedin2Ramp2value2
0x0144, 0x00AA, // ChannelEloopfilterfeedin1Inputmask1
0x0143, 0x0741, // ChannelEloopfilterfeedin1Inputcontrol1
0x0147, 0x010D, // ChannelEloopfilterfeedin1Ramp1stepsize
0x0145, 0x0000, // ChannelEloopfilterfeedin1Ramp1value1
0x0146, 0x0800, // ChannelEloopfilterfeedin1Ramp1value2
0x0149, 0xAAAA, // ChannelEloopfilterfeedin2Inputmask2
0x0148, 0x0041, // ChannelEloopfilterfeedin2Inputcontrol2
0x014C, 0x0006, // ChannelEloopfilterfeedin2Ramp2stepsize
0x014A, 0x0000, // ChannelEloopfilterfeedin2Ramp2value1
0x014B, 0x002A, // ChannelEloopfilterfeedin2Ramp2value2
0x01C4, 0x11AA, // ChannelGloopfilterfeedin1Inputmask1
0x01C3, 0x0713, // ChannelGloopfilterfeedin1Inputcontrol1
0x01C7, 0x0096, // ChannelGloopfilterfeedin1Ramp1stepsize
0x01C5, 0x0000, // ChannelGloopfilterfeedin1Ramp1value1
0x01C6, 0xFB8F, // ChannelGloopfilterfeedin1Ramp1value2
0x01C9, 0x98AA, // ChannelGloopfilterfeedin2Inputmask2
0x01C8, 0x078C, // ChannelGloopfilterfeedin2Inputcontrol2
0x01CC, 0x0105, // ChannelGloopfilterfeedin2Ramp2stepsize
0x01CA, 0x0000, // ChannelGloopfilterfeedin2Ramp2value1
0x01CB, 0xF841, // ChannelGloopfilterfeedin2Ramp2value2
0x0015, 0x0000, // ButterflyMixerElementAB
0x0017, 0x0000, // ButterflyMixerElementEF
0x0016, 0x0120, // ButterflyMixerElementCD
0x0018, 0x0120, // ButterflyMixerElementGH
0x0019, 0x0235, // ButterflyMixerElementAC
0x001A, 0x0235, // ButterflyMixerElementBD
0x001B, 0x0235, // ButterflyMixerElementEG
0x001C, 0x0235, // ButterflyMixerElementFH
0x001D, 0x0400, // ButterflyMixerElementAE
0x001E, 0x0400, // ButterflyMixerElementBF
0x001F, 0x0400, // ButterflyMixerElementCG
0x0020, 0x0400, // ButterflyMixerElementDH
0x0000, 0x0011, // Programmingmode
0x006B, 0x0000, // ChannelAPWMPWMcarrierinitrise
0x006C, 0x3F34, // ChannelAPWMPWMcarrierinitfall
0x006D, 0x3ECE, // ChannelAPWMPWMcarrierthrise
0x006E, 0x0066, // ChannelAPWMPWMcarrierthfall
0x006F, 0x019A, // ChannelAPWMPWMstepsizeinitrise
0x0070, 0xFE66, // ChannelAPWMPWMstepsizeinitfall
0x0071, 0x0000, // ChannelAPWMPWMstepsizedeltarise
0x0072, 0x0000, // ChannelAPWMPWMstepsizedeltafall
0x0073, 0x3F34, // ChannelAPWMPWMstartcarrier
0x0074, 0xFE66, // ChannelAPWMPWMstartstepsize
0x0075, 0x001A, // ChannelAPWMPWMcontrol
0x0000, 0x0000, // Programmingmode
0x0000, 0x0044, // Programmingmode
0x00EB, 0x0000, // ChannelCPWMPWMcarrierinitrise
0x00EC, 0x3F34, // ChannelCPWMPWMcarrierinitfall
0x00ED, 0x3ECE, // ChannelCPWMPWMcarrierthrise
0x00EE, 0x0066, // ChannelCPWMPWMcarrierthfall
0x00EF, 0x019A, // ChannelCPWMPWMstepsizeinitrise
0x00F0, 0xFE66, // ChannelCPWMPWMstepsizeinitfall
0x00F1, 0x0000, // ChannelCPWMPWMstepsizedeltarise
0x00F2, 0x0000, // ChannelCPWMPWMstepsizedeltafall
0x00F3, 0x3F34, // ChannelCPWMPWMstartcarrier
0x00F4, 0xFE66, // ChannelCPWMPWMstartstepsize
0x00F5, 0x001A, // ChannelCPWMPWMcontrol
0x0000, 0x0000, // Programmingmode
0x0000, 0x0022, // Programmingmode
0x00AB, 0x0000, // ChannelBPWMPWMcarrierinitrise
0x00AC, 0x3F34, // ChannelBPWMPWMcarrierinitfall
0x00AD, 0x3ECE, // ChannelBPWMPWMcarrierthrise
0x00AE, 0x0066, // ChannelBPWMPWMcarrierthfall
0x00AF, 0x019A, // ChannelBPWMPWMstepsizeinitrise
0x00B0, 0xFE66, // ChannelBPWMPWMstepsizeinitfall
0x00B1, 0x0000, // ChannelBPWMPWMstepsizedeltarise
0x00B2, 0x0000, // ChannelBPWMPWMstepsizedeltafall
0x00B3, 0x3F34, // ChannelBPWMPWMstartcarrier
0x00B4, 0xFE66, // ChannelBPWMPWMstartstepsize
0x00B5, 0x001E, // ChannelBPWMPWMcontrol
0x0000, 0x0000, // Programmingmode
0x0000, 0x0088, // Programmingmode
0x012B, 0x0000, // ChannelDPWMPWMcarrierinitrise
0x012C, 0x3F34, // ChannelDPWMPWMcarrierinitfall
0x012D, 0x3ECE, // ChannelDPWMPWMcarrierthrise
0x012E, 0x0066, // ChannelDPWMPWMcarrierthfall
0x012F, 0x019A, // ChannelDPWMPWMstepsizeinitrise
0x0130, 0xFE66, // ChannelDPWMPWMstepsizeinitfall
0x0131, 0x0000, // ChannelDPWMPWMstepsizedeltarise
0x0132, 0x0000, // ChannelDPWMPWMstepsizedeltafall
0x0133, 0x3F34, // ChannelDPWMPWMstartcarrier
0x0134, 0xFE66, // ChannelDPWMPWMstartstepsize
0x0135, 0x001E, // ChannelDPWMPWMcontrol
0x0000, 0x0000, // Programmingmode
0x0030, 0x0000, // Crossbaroutputselect1
0x0031, 0x1249, // Crossbaroutputselect2
0x0032, 0x2492, // Crossbaroutputselect3
0x0033, 0x36DB, // Crossbaroutputselect4
0x0034, 0x4924, // Crossbaroutputselect5
0x0035, 0x5B6D, // Crossbaroutputselect6
0x0036, 0x6DB6, // Crossbaroutputselect7
0x0037, 0x7FFF, // Crossbaroutputselect8
0x0000, 0x00FF, // Programmingmode
0x0076, 0x47DF, // ChannelAADCdigitalADCenables
0x0077, 0x062F, // ChannelAADCdigitalADCdither
0x0078, 0x6018, // ChannelAADCdigitalADCclipfeedback1
0x0079, 0x1818, // ChannelAADCdigitalADCclipfeedback2
0x007A, 0x007A, // ChannelAADCdigitalADCmisc
0x007B, 0x1E1A, // ChannelAADCanalogADCanalog1
0x007C, 0x0008, // ChannelAADCanalogADCanalog2
0x0000, 0x0000, // Programmingmode
0x0021, 0x1EF9, // ChargePumpControl
0x0022, 0x038C, // ChargePumpGain
0x0023, 0x00FA, // ChargePumpCMweight
0x0024, 0x0000, // ChargePumpDMweight
0x0025, 0x0001, // ReferenceGeneratorControl
0x0026, 0x2F11, // ReferenceGeneratorAnalog1
0x0027, 0x0000, // ReferenceGeneratorAnalog2
0x0028, 0x67D5, // ReferenceADCenables
0x0029, 0x0626, // ReferenceADCdither
0x002A, 0x6018, // ReferenceADCclipfeedback1
0x002B, 0x1818, // ReferenceADCclipfeedback2
0x002C, 0x007A, // ReferenceADCmisc
0x002D, 0x0616, // ReferenceADCanalog1
0x002E, 0x0000, // ReferenceADCanalog2
0x0038, 0x0C35, // ExpertFaultMask
0x0001, 0xFF00, // MainChannelEnable
0x0002, 0x17FF, // MiscellaneousEnable
0x0003, 0x0000, // FeedinRampSelect
0x0004, 0x00FF, // PCMVolumeRampSelect
0x003C, 0x0000, // Statusinformation1
0x003D, 0xFFFF, // Statusinformation2
0x003E, 0x0030, // Statusinformation3
0xFFFF          // Terminate Array 
};

#define REG01_VALUE 0xFFFF
#define REG01_VALUE_DEFAULT 0xFF00
#define REG02_PSTART_DISABLE 0x17FF
#define REG02_PSTART_ENABLE 0x1FFF
#define REG03_VALUE 0x5500
#define REG03_VALUE_DEFAULT 0x0000
#define REG07_VALUE 0x0001
#define REG07_VALUE_DEFAULT 0x0064
#define REG56_VALUE 0x0C35
#define REG56_RESET_VALUE 0x0435

float PVDD = 50;
uint32_t SAMPLE_RATE = 192000;
float UVP = 45;
float OVP = 55;
uint16_t VolumeHex = 0x8FF6;