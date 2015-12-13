#ifndef ETMEx_H
#define ETMEx_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */

/** \brief  Structure type to access the Embedded Trace Macrocell (ETM).
 */
typedef struct
{
    __IO uint32_t ETMCR;                /*!< Offset: 0x000 (R/W) Main Control Register                                */
    __I  uint32_t ETMCCR;               /*!< Offset: 0x004 (R/W) Configuration Code Register                          */
    __IO uint32_t ETMTRIGGER;           /*!< Offset: 0x008 (R/W) Trigger Event Register                               */
    __IO uint32_t ETMASICCR;            /*!< Offset: 0x00C (R/W) ASIC Control Register                                */
    __IO uint32_t ETMSR;                /*!< Offset: 0x010 (R/W) ETM Status Register                                  */
    __I  uint32_t ETMSCR;               /*!< Offset: 0x014 (R/W) System Configuration Register                        */
    //Trace Enable configuration
    __IO uint32_t ETMTSSCR;             /*!< Offset: 0x018 (R/W) TraceEnable Start/Stop Control Register              */
    __IO uint32_t ETMTECR2;             /*!< Offset: 0x01C (R/W) TraceEnable Control 2 Register                       */
    __IO uint32_t ETMTEEVR;             /*!< Offset: 0x020 (R/W) TraceEnable Event Register                           */
    __IO uint32_t ETMTECR1;             /*!< Offset: 0x024 (R/W) TraceEnable Control 1 Register                       */
    //FIFOFULL configuration
    __IO uint32_t EMTFFRR;              /*!< Offset: 0x028 (R/W) FIFOFULL Region Register                             */
    __IO uint32_t EMTFFLR;              /*!< Offset: 0x02C (R/W) FIFOFULL Level Register                              */
    //ViewData configuration
    __IO uint32_t ETMVDEVR;             /*!< Offset: 0x030 (R/W) ViewData Event Register*/
    __IO uint32_t ETMVDCR1;             /*!< Offset: 0x034 (R/W) ViewData Control 1 Register                          */
    __IO uint32_t ETMVDCR2;             /*!< Offset: 0x038 (R/W) ViewData Control 2 Register                          */
    __IO uint32_t ETMVDCR3;             /*!< Offset: 0x03C (R/W) ViewData Control 3 Register                          */
    //Address comparators
    __IO uint32_t ETMACVR[16];          /*!< Offset: 0x040 (R/W) Address Comparator Value Registers (1-16)            */
    __IO uint32_t ETMACTR[16];          /*!< Offset: 0x080 (R/W) Address Comparator Access Type Registers (1-16)      */
    //Data value comparators
    __IO uint32_t ETMDCVR[16];          /*!< Offset: 0x0C0 (R/W) Data Comparator Value Registers (1-16)               */
    __IO uint32_t ETMDCMR[16];          /*!< Offset: 0x100 (R/W) Data Comparator Mask Registers (1-16)                */
    //Counters
    __IO uint32_t ETMCNTRLDVR[4];       /*!< Offset: 0x140 (R/W) Counter Reload Value Registers (1-4)                 */
    __IO uint32_t ETMCNTENR[4];         /*!< Offset: 0x150 (R/W) Counter Enable Registers (1-4)*/
    __IO uint32_t ETMCNTRLDEVR[4];      /*!< Offset: 0x160 (R/W) Counter Reload Event Registers (1-4)                 */
    __IO uint32_t ETMCNTVR[4];          /*!< Offset: 0x170 (R/W) Counter Value Registers (1-4)                        */
    //Sequencer
    __IO uint32_t ETMSQabEVR[6];        /*!< Offset: 0x180 (R/W) Sequencer State Transition Event Registers           */
         uint32_t RESERVED0;
    __IO uint32_t ETMSQR;               /*!< Offset: 0x19C (R/W) Current Sequencer State Register                     */
    __IO uint32_t ETMEXTOUTEVR[4];      /*!< Offset: 0x1A0 (R/W) External Output Event Registers                      */
    //Context ID comparators
    __IO uint32_t ETMCIDVR[3];          /*!< Offset: 0x1B0 (R/W) Context ID Comparator Value Registers                */
    __IO uint32_t ETMCIDCMR;            /*!< Offset: 0x1BC (R/W) Context ID Comparator Mask Register                  */
    //Other ETM Trace registers
    __IO uint32_t ETMIMPSEC[8];         /*!< Offset: 0x1C0 (R/W) Implementation specific registers                    */
    __IO uint32_t ETMSYNCFR;            /*!< Offset: 0x1E0 (R/W) Synchronization Frequency Register                   */
    __I  uint32_t ETMIDR;               /*!< Offset: 0x1E4 (R/ ) ID Register*/
    __I  uint32_t ETMCCER;              /*!< Offset: 0x1E8 (R/ ) Configuration Code Extension Register                */
    __IO uint32_t ETMEXTINSELR;         /*!< Offset: 0x1EC (R/ ) Extended External Input Selection Register           */
    __IO uint32_t ETMTESSEICR;          /*!< Offset: 0x1F0 (R/W) TraceEnable Start/Stop EmbeddedICE Control Register  */
    __IO uint32_t ETMEIBCR;             /*!< Offset: 0x1F4 (R/W) EmbeddedICE BehaviorControl Register                  */
    __IO uint32_t ETMTSEVR;             /*!< Offset: 0x1F8 (R/W) Timestamp Event Register                             */
    __IO uint32_t ETMAUXCR;             /*!< Offset: 0x1FC (R/W) Auxiliary Control Register                           */
    __IO uint32_t ETMTRACEIDR;          /*!< Offset: 0x200 CoreSight Trace ID Register                                 */
         uint32_t RESERVED1;
    __IO uint32_t ETMIDR2;              /*!< Offset: 0x208 ETM ID Register 2                                           */
         uint32_t RESERVED2[13];
    __IO uint32_t ETMVMIDCVR;           /*!< Offset: 0x240 VMID Comparator Value Register                             */
         uint32_t RESERVED3[47];
    //Operating system save and restore registers
    __IO uint32_t ETMOSLAR;             /*!< Offset: 0x300 OS Lock Access Register                                    */
    __IO uint32_t ETMOSLSR;             /*!< Offset: 0x304 OS Lock Status Register                                    */
    __IO uint32_t ETMOSSRR;             /*!< Offset: 0x308 OS Save and Restore Register                               */
         uint32_t RESERVED4;
    //Other ETM Management registers
    __IO uint32_t ETMPDCR;              /*!< Offset: 0x310 Power Down Control Register                                */
    __IO uint32_t ETMPDSR;              /*!< Offset: 0x310 Device Power-Down Status Register                          */
         uint32_t RESERVED5[698];
         uint32_t INTEGRATION[64];
    __IO uint32_t ETMITCTRL;            /*!< Offset: 0xF00 Integration Mode Control Register                          */
         uint32_t RESERVED6[39];
    __IO uint32_t ETMCLAIMSET;          /*!< Offset: 0xFA0 Claim Tag Set Register                                     */
    __IO uint32_t ETMCLAIMCLR;          /*!< Offset: 0xFA4 Claim Tag Clear Register                                   */
         uint32_t RESERVED8[2];
    __IO uint32_t ETMLAR;               /*!< Offset: 0xFB0 Lock Access Register                                       */
    __IO uint32_t ETMLSR;               /*!< Offset: 0xFB4 Lock Status Register                                       */
    __IO uint32_t ETMAUTHSTATUS;        /*!< Offset: 0xFB8 Authentication Status Register                             */
         uint32_t RESERVED9[3];
    __IO uint32_t ETMDEVID;             /*!< Offset: 0xFC8 CoreSight Device Configuration Register                    */
    __IO uint32_t ETMDEVTYPE;           /*!< Offset: 0xFCC CoreSight Device Type Register                             */
    //Peripheral and Component ID registers
    __I  uint32_t ETMPIDR4;             /*!< Offset: 0xFD0 Peripheral ID4 Register                                    */
    __I  uint32_t ETMPIDR5;             /*!< Offset: 0xFD4 Peripheral ID5 Register                                    */
    __I  uint32_t ETMPIDR6;             /*!< Offset: 0xFD8 Peripheral ID6 Register                                    */
    __I  uint32_t ETMPIDR7;             /*!< Offset: 0xFDC Peripheral ID7 Register                                    */
    __I  uint32_t ETMPIDR0;             /*!< Offset: 0xFE0 Peripheral ID0 Register                                    */
    __I  uint32_t ETMPIDR1;             /*!< Offset: 0xFE4 Peripheral ID1 Register                                    */
    __I  uint32_t ETMPIDR2;             /*!< Offset: 0xFE8 Peripheral ID2 Register                                    */
    __I  uint32_t ETMPIDR3;             /*!< Offset: 0xFEC Peripheral ID3 Register                                    */
    __I  uint32_t ETMCIDR0;             /*!< Offset: 0xFF0 Component ID0 Register                                     */
    __I  uint32_t ETMCIDR1;             /*!< Offset: 0xFF4 Component ID1 Register                                     */
    __I  uint32_t ETMCIDR2;             /*!< Offset: 0xFF8 Component ID2 Registe                                      */
    __I  uint32_t ETMCIDR3;             /*!< Offset: 0xFF CComponent ID3 Register                                     */
} ETM_Type;

typedef enum
{
  SINGLE_ADDRESS_COMPARATOR_1 = 0,
  SINGLE_ADDRESS_COMPARATOR_2,
  SINGLE_ADDRESS_COMPARATOR_3,
  SINGLE_ADDRESS_COMPARATOR_4,
  SINGLE_ADDRESS_COMPARATOR_5,
  SINGLE_ADDRESS_COMPARATOR_6,
  SINGLE_ADDRESS_COMPARATOR_7,
  SINGLE_ADDRESS_COMPARATOR_8,
  SINGLE_ADDRESS_COMPARATOR_9,
  SINGLE_ADDRESS_COMPARATOR_10,
  SINGLE_ADDRESS_COMPARATOR_11,
  SINGLE_ADDRESS_COMPARATOR_12,
  SINGLE_ADDRESS_COMPARATOR_13,
  SINGLE_ADDRESS_COMPARATOR_14,
  SINGLE_ADDRESS_COMPARATOR_15,
  SINGLE_ADDRESS_COMPARATOR_16,
  ADDRESS_RANGE_COMPARATOR_1,
  ADDRESS_RANGE_COMPARATOR_2,
  ADDRESS_RANGE_COMPARATOR_3,
  ADDRESS_RANGE_COMPARATOR_4,
  ADDRESS_RANGE_COMPARATOR_5,
  ADDRESS_RANGE_COMPARATOR_6,
  ADDRESS_RANGE_COMPARATOR_7,
  ADDRESS_RANGE_COMPARATOR_8,
  INSTRUMENTATION_RESOURCE_1,
  INSTRUMENTATION_RESOURCE_2,
  INSTRUMENTATION_RESOURCE_3,
  INSTRUMENTATION_RESOURCE_4,
  WATCHPOINT_COMPARATOR_1,
  WATCHPOINT_COMPARATOR_2,
  WATCHPOINT_COMPARATOR_3,
  WATCHPOINT_COMPARATOR_4,
  WATCHPOINT_COMPARATOR_5,
  WATCHPOINT_COMPARATOR_6,
  WATCHPOINT_COMPARATOR_7,
  WATCHPOINT_COMPARATOR_8,
  MEMORY_MAP_DECODES_1,
  MEMORY_MAP_DECODES_2,
  MEMORY_MAP_DECODES_3,
  MEMORY_MAP_DECODES_4,
  MEMORY_MAP_DECODES_5,
  MEMORY_MAP_DECODES_6,
  MEMORY_MAP_DECODES_7,
  MEMORY_MAP_DECODES_8,
  MEMORY_MAP_DECODES_9,
  MEMORY_MAP_DECODES_10,
  MEMORY_MAP_DECODES_11,
  MEMORY_MAP_DECODES_12,
  MEMORY_MAP_DECODES_13,
  MEMORY_MAP_DECODES_14,
  MEMORY_MAP_DECODES_15,
  MEMORY_MAP_DECODES_16,
  COUNTER_1,
  COUNTER_2,
  COUNTER_3,
  COUNTER_4,
  SEQUENCER_IN_STATE_1,
  SEQUENCER_IN_STATE_2,
  SEQUENCER_IN_STATE_3,
  CONTEXTID_COMPARATOR_1 = 88,
  CONTEXTID_COMPARATOR_2,
  CONTEXTID_COMPARATOR_3,
  VMID_COMPARATOR,
  TRACE_STARTSTOP_RESOUCE = 95,
  EXTERNAL_INPUT_1,
  EXTERNAL_INPUT_2,
  EXTERNAL_INPUT_3,
  EXTERNAL_INPUT_4,
  EXTENDED_EXTERNAL_INPUT_SELECTOR_1 = 104,
  EXTENDED_EXTERNAL_INPUT_SELECTOR_2,
  EXTENDED_EXTERNAL_INPUT_SELECTOR_3,
  EXTENDED_EXTERNAL_INPUT_SELECTOR_4,
  PROCESSOR_IN_NON_SECURE_STATE =109,
  TRACE_PROHIBITED,
  HARD_WIRED_INPUT,
}ETMEvent_Resources;

typedef enum
{
  A = 0,
  NOT_A,
  A_AND_B,
  NOT_A_AND_B,
  NOT_A_AND_NOT_B,
  A_OR_B,
  NOT_A_OR_B,
  NOT_A_OR_NOT_B,
}ETMEvent_FunctionEncoding;

#define ETM_ETMCR_TIMESTAMP_ENABLE_Pos                                        28
#define ETM_ETMCR_TIMESTAMP_ENABLE_Msk                                        (1UL << ETM_ETMCR_TIMESTAMP_ENABLE_Pos)
#define ETM_ETMCR_DATAONLY_MODE_Pos                                           20
#define ETM_ETMCR_DATAONLY_MODE_Msk                                           (1UL << ETM_ETMCR_DATAONLY_MODE_Pos)
#define ETM_ETMCR_ETMEN_Pos                                                   11
#define ETM_ETMCR_ETMEN_Msk                                                   (1UL << ETM_ETMCR_ETMEN_Pos)
#define ETM_ETMCR_BRANCH_OUTPUT_Pos                                           8
#define ETM_ETMCR_BRANCH_OUTPUT_Msk                                           (1UL << ETM_ETMCR_BRANCH_OUTPUT_Pos)
#define ETM_ETMCR_STALL_PROCESSOR_Pos                                         7
#define ETM_ETMCR_STALL_PROCESSOR_Msk                                         (1UL << ETM_ETMCR_STALL_PROCESSOR_Pos)
#define ETM_ETMCR_ETMPOWERDOWN_Pos                                            0
#define ETM_ETMCR_ETMPOWERDOWN_Msk                                            (1UL << ETM_ETMCR_ETMPOWERDOWN_Pos)
#define ETM_ETMCR_ETMPROGBIT_Pos                                              10
#define ETM_ETMCR_ETMPROGBIT_Msk                                              (1UL << ETM_ETMCR_ETMPROGBIT_Pos)

#define ETM_ETMCCR_ETMIDR_PRESENT_Pos                                         31
#define ETM_ETMCCR_ETMIDR_PRESENT_Msk                                         (1UL << ETM_ETMCCR_ETMIDR_PRESENT_Pos)
#define ETM_ETMCCR_COPROCESSOR_MEMORYACCESS_SUPPORT_Pos                       27
#define ETM_ETMCCR_COPROCESSOR_MEMORYACCESS_SUPPORT_Msk                       (1UL << ETM_ETMCCR_COPROCESSOR_MEMORYACCESS_SUPPORT_Pos)
#define ETM_ETMCCR_TRACE_STARTSTOP_BLOCK_PRESENT_Pos                          26
#define ETM_ETMCCR_TRACE_STARTSTOP_BLOCK_PRESENT_Msk                          (1UL << ETM_ETMCCR_TRACE_STARTSTOP_BLOCK_PRESENT_Pos)
#define ETM_ETMCCR_NUMBER_OF_CONTEXTID_COMPARATOR_Pos                         24
#define ETM_ETMCCR_NUMBER_OF_CONTEXTID_COMPARATOR_Msk                         (3UL << ETM_ETMCCR_NUMBER_OF_CONTEXTID_COMPARATOR_Pos)
#define ETM_ETMCCR_FIFOFULL_LOGIC_PRESENT_Pos                                 23
#define ETM_ETMCCR_FIFOFULL_LOGIC_PRESENT_Msk                                 (1UL << ETM_ETMCCR_FIFOFULL_LOGIC_PRESENT_Pos)
#define ETM_ETMCCR_NUMBER_OF_EXTERNAL_OUTPUTS_Pos                             20
#define ETM_ETMCCR_NUMBER_OF_EXTERNAL_OUTPUTS_Msk                             (7UL << ETM_ETMCCR_NUMBER_OF_EXTERNAL_OUTPUTS_Pos)
#define ETM_ETMCCR_NUMBER_OF_EXTERNAL_INPUTS_Pos                              17
#define ETM_ETMCCR_NUMBER_OF_EXTERNAL_INPUTS_Msk                              (7UL << ETM_ETMCCR_NUMBER_OF_EXTERNAL_INPUTS_Pos)
#define ETM_ETMCCR_SEQUENCER_PRESENT_Pos                                      16
#define ETM_ETMCCR_SEQUENCER_PRESENT_Msk                                      (1UL << ETM_ETMCCR_SEQUENCER_PRESENT_Pos)
#define ETM_ETMCCR_NUMBER_OF_COUNTERS_Pos                                     13
#define ETM_ETMCCR_NUMBER_OF_COUNTERS_Msk                                     (7UL << ETM_ETMCCR_NUMBER_OF_COUNTERS_Pos)
#define ETM_ETMCCR_NUMBER_OF_MEMORYMAP_DECODERS_Pos                           8
#define ETM_ETMCCR_NUMBER_OF_MEMORYMAP_DECODERS_Msk                           (31UL << ETM_ETMCCR_NUMBER_OF_MEMORYMAP_DECODERS_Pos)
#define ETM_ETMCCR_NUMBER_OF_DATAVALUE_COMPARATOR_Pos                         4
#define ETM_ETMCCR_NUMBER_OF_DATAVALUE_COMPARATOR_Msk                         (15UL << ETM_ETMCCR_NUMBER_OF_DATAVALUE_COMPARATOR_Pos)
#define ETM_ETMCCR_NUMBER_OF_ADDRESS_COMPARATOR_PAIRS_Pos                     0
#define ETM_ETMCCR_NUMBER_OF_ADDRESS_COMPARATOR_PAIRS_Msk                     (15UL << ETM_ETMCCR_NUMBER_OF_ADDRESS_COMPARATOR_PAIRS_Pos)

#define ETM_ETMCCER_TIMESTAMP_PACKET_SIZE_Pos                                 29
#define ETM_ETMCCER_TIMESTAMP_PACKET_SIZE_Msk                                 (1UL << ETM_ETMCCER_TIMESTAMP_PACKET_SIZE_Pos) 
#define ETM_ETMCCER_TIMESTAMP_PACKET_ENCODING_Pos                             28
#define ETM_ETMCCER_TIMESTAMP_PACKET_ENCODING_Msk                             (1UL << ETM_ETMCCER_TIMESTAMP_PACKET_ENCODING_Pos) 
#define ETM_ETMCCER_REDUCED_FUNCTION_COUNTER_Pos                              27
#define ETM_ETMCCER_REDUCED_FUNCTION_COUNTER_Msk                              (1UL << ETM_ETMCCER_REDUCED_FUNCTION_COUNTER_Pos)
#define ETM_ETMCCER_VIRTUALIZATION_EXTERNSION_Pos                             26
#define ETM_ETMCCER_VIRTUALIZATION_EXTERNSION_Msk                             (1UL << ETM_ETMCCER_VIRTUALIZATION_EXTERNSION_Pos)
#define ETM_ETMCCER_TIMESTAMPING_IMPLEMENTATION_Pos                           22
#define ETM_ETMCCER_TIMESTAMPING_IMPLEMENTATION_Msk                           (1UL << ETM_ETMCCER_TIMESTAMPING_IMPLEMENTATION_Pos) 
#define ETM_ETMCCER_ETMEIBCR_IMPLEMENTED_Pos                                  21      
#define ETM_ETMCCER_ETMEIBCR_IMPLEMENTED_Msk                                  (1UL << ETM_ETMCCER_ETMEIBCR_IMPLEMENTED_Pos)
#define ETM_ETMCCER_TRACE_BLOCK_USE_EMBEDDEDICE_WATCHPOINT_INPUTS_Pos         20
#define ETM_ETMCCER_TRACE_BLOCK_USE_EMBEDDEDICE_WATCHPOINT_INPUTS_Msk         (1UL << ETM_ETMCCER_TRACE_BLOCK_USE_EMBEDDEDICE_WATCHPOINT_INPUTS_Pos)   
#define ETM_ETMCCER_NUMBER_OF_EMBEDDEDICE_WATCHPOINT_INPUTS_Pos               16         
#define ETM_ETMCCER_NUMBER_OF_EMBEDDEDICE_WATCHPOINT_INPUTS_Msk               (15UL << ETM_ETMCCER_NUMBER_OF_EMBEDDEDICE_WATCHPOINT_INPUTS_Pos)
#define ETM_ETMCCER_NUMBER_OF_INSTRUMENTATION_RESOURCES_Pos                   13         
#define ETM_ETMCCER_NUMBER_OF_INSTRUMENTATION_RESOURCES_Msk                   (7UL << ETM_ETMCCER_NUMBER_OF_INSTRUMENTATION_RESOURCES_Pos) 
#define ETM_ETMCCER_DATA_ADDRESS_COMPARISON_SUPPORT_Pos                       12         
#define ETM_ETMCCER_DATA_ADDRESS_COMPARISON_SUPPORT_Msk                       (1UL << ETM_ETMCCER_DATA_ADDRESS_COMPARISON_SUPPORT_Pos)
#define ETM_ETMCCER_ALL_REGISTERS_READABLE_Pos                                11         
#define ETM_ETMCCER_ALL_REGISTERS_READABLE_Msk                                (1UL << ETM_ETMCCER_ALL_REGISTERS_READABLE_Pos)
#define ETM_ETMCCER_SIZE_OF_EXTENDED_EXTERNAL_INPUT_BUS_Pos                   3                       
#define ETM_ETMCCER_SIZE_OF_EXTENDED_EXTERNAL_INPUT_BUS_Msk                   (255UL << ETM_ETMCCER_SIZE_OF_EXTENDED_EXTERNAL_INPUT_BUS_Pos)
#define ETM_ETMCCER_NUMBER_OF_EXTENDED_EXTERNAL_INPUT_SELECTORS_Pos           0             
#define ETM_ETMCCER_NUMBER_OF_EXTENDED_EXTERNAL_INPUT_SELECTORS_Msk           (7UL << ETM_ETMCCER_NUMBER_OF_EXTENDED_EXTERNAL_INPUT_SELECTORS_Pos)                

#define ETM_ETMLSR_ETMLOCKEDUNLOCKED_Pos                    1
#define ETM_ETMLSR_ETMLOCKEDUNLOCKED_Msk                    (1UL << ETM_ETMLSR_ETMLOCKEDUNLOCKED_Pos)

#define ETM_ETMSR_PROGBIT_Pos                               1
#define ETM_ETMSR_PROGBIT_Msk                               (1UL << ETM_ETMSR_PROGBIT_Pos)

#define ETM_BASE  (0xE0041000UL)             /*!< ETM Base Address                   */
#define ETM       ((ETM_Type *)   ETM_BASE)  /*!< ETM configuration struct           */

#define ETM_UNLOCK_KEY 0xC5ACCE55
#define ETM_LOCK       0

// #define ETM_TRACE_REGISTER_UNLOCK  0
// #define ETM_TRACE_REGISTER_LOCK    0xC5ACCE55


#define CORESIGHT_ETM_M4_ID        0x4114F250
#endif // ETMEx_H