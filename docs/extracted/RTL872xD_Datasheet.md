UM0401
RTL872xD Datasheet
Realtek Semiconductor Corp.
No. 2, Innovation Road II, Hsinchu Science Park, Hsinchu 300, Taiwan
Tel.: +886-3-578-0211. Fax: +886-3-577-6047
www.realtek.com

RTL872xD Datasheet
COPYRIGHT
©2021 Realtek Semiconductor Corp. All rights reserved. No part of this document may be reproduced, transmitted, transcribed, stored in a
retrieval system, or translated into any language in any form or by any means without the written permission of Realtek Semiconductor Corp.
DISCLAIMER
Please Read Carefully:
Realtek Semiconductor Corp., (Realtek) reserves the right to make corrections, enhancements, improvements and other changes to its
products and services. Buyers should obtain the latest relevant information before placing orders and should verify that such information is
current and complete.
Reproduction of significant portions in Realtek data sheets is permissible only if reproduction is without alteration and is accompanied by all
associated warranties, conditions, limitations, and notices. Realtek is not responsible or liable for such reproduced documentation. Information
of third parties may be subject to additional restrictions.
Buyers and others who are developing systems that incorporate Realtek products (collectively, “Customers”) understand and agree that
Customers remain responsible for using their independent analysis, evaluation and judgment in designing their applications and that
Customers have full and exclusive responsibility to assure the safety of Customers' applications and compliance of their applications (and of all
Realtek products used in or for Customers’ applications) with all applicable regulations, laws and other applicable requirements. Designer
represents that, with respect to their applications, Customer has all the necessary expertise to create and implement safeguards that (1)
anticipate dangerous consequences of failures, (2) monitor failures and their consequences, and (3) lessen the likelihood of failures that might
cause harm and take appropriate actions. Customer agrees that prior to using or distributing any applications that include Realtek products,
Customer will thoroughly test such applications and the functionality of such Realtek products as used in such applications.
Realtek’s provision of technical, application or other design advice, quality characterization, reliability data or other services or information,
including, but not limited to, reference designs and materials relating to evaluation kits, (collectively, “Resources”) are intended to assist
designers who are developing applications that incorporate Realtek products; by downloading, accessing or using Realtek’s Resources in any
way, Customer (individually or, if Customer is acting on behalf of a company, Customer’s company) agrees to use any particular Realtek
Resources solely for this purpose and subject to the terms of this Notice.
Realtek’s provision of Realtek Resources does not expand or otherwise alter Realtek’s applicable published warranties or warranty disclaimers
for Realtek’s products, and no additional obligations or liabilities arise from Realtek providing such Realtek Resources. Realtek reserves the
right to make corrections, enhancements, improvements and other changes to its Realtek Resources. Realtek has not conducted any testing
other than that specifically described in the published documentation for a particular Realtek Resource.
Customer is authorized to use, copy and modify any individual Realtek Resource only in connection with the development of applications that
include the Realtek product(s) identified in such Realtek Resource. No other license, express or implied, by estoppel or otherwise to any other
Realtek intellectual property right, and no license to any technology or intellectual property right of Realtek or any third party is granted
herein, including but not limited to any patent right, copyright, mask work right, or other intellectual property right relating to any
combination, machine, or process in which Realtek products or services are used. Information regarding or referencing third-party products or
services does not constitute a license to use such products or services, or a warranty or endorsement thereof. Use of Realtek Resources may
require a license from a third party under the patents or other intellectual property of the third party, or a license from Realtek under the
patents or other Realtek’s intellectual property.
Realtek’s Resources are provided “as is” and with all faults. Realtek disclaims all other warranties or representations, express or implied,
regarding resources or use thereof, including but not limited to accuracy or completeness, title, any epidemic failure warranty and any implied
warranties of merchantability, fitness for a particular purpose, and non-infringement of any third party intellectual property rights.
Realtek shall not be liable for and shall not defend or indemnify Customer against any claim, including but not limited to any infringement claim
that related to or is based on any combination of products even if described in Realtek Resources or otherwise. In no event shall Realtek be
liable for any actual, direct, special, collateral, indirect, punitive, incidental, consequential or exemplary damages in connection with or arising
out of Realtek’s Resources or use thereof, and regardless of whether Realtek has been advised of the possibility of such damages. Realtek is not
responsible for any failure to meet such industry standard requirements.
Where Realtek specifically promotes products as facilitating functional safety or as compliant with industry functional safety standards, such
products are intended to help enable customers to design and create their own applications that meet applicable functional safety standards
and requirements. Using products in an application does not by itself establish any safety features in the application. Customers must ensure
compliance with safety-related requirements and standards applicable to their applications. Designer may not use any Realtek products in life-
critical medical equipment unless authorized officers of the parties have executed a special contract specifically governing such use. Life-critical
medical equipment is medical equipment where failure of such equipment would cause serious bodily injury or death. Such equipment
includes, without limitation, all medical devices identified by the U.S.FDA as Class III devices and equivalent classifications outside the U.S.
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
2

RTL872xD Datasheet
Customers agree that it has the necessary expertise to select the product with the appropriate qualification designation for their applications
and that proper product selection is at Customers’ own risk. Customers are solely responsible for compliance with all legal and regulatory
requirements in connection with such selection.
Customer will fully indemnify Realtek and its representatives against any damages, costs, losses, and/or liabilities arising out of Designer’s non-
compliance with the terms and provisions of this Notice.
TRADEMARKS
Realtek is a trademark of Realtek Semiconductor Corporation. Other names mentioned in this document are trademarks/registered trademarks
of their respective owners.
USING THIS DOCUMENT
This document is intended for the software engineer’s reference and provides detailed programming information.
Though every effort has been made to ensure that this document is current and accurate, more information may have become available
subsequent to the production of this guide.
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
3

  RTL872xD Datasheet
Ordering Information
|     | RTL87 | 2 1 | D M |   V | T 1 |   CG |
| --- | ----- | --- | --- | --- | --- | ---- |
IoT Device
Realtek 32-bit microcontroller
Wireless
1 = Wi-Fi only
2 = Combo (Wi-Fi + BT)
Function
0 = Basic function
1 = With Codec
2 = With SD/LCD
Band type
CS = Single band
D = Dual band
MCM type
N = Single die
M = MCM PSRAM
F = MCM Flash
Temperature range
T =  40°C ~ 105°C
Others =  20°C ~ 85°C
Flash memory size (if stacked Flash)
1 = 4M bytes
IC packaging standard
CG = Compound green
CGT = Tape & Reel
| Part Number        |     | Package  |     | Status             |     |     |
| ------------------ | --- | -------- | --- | ------------------ | --- | --- |
| RTL8720DN-VA1-CG   |     | QFN48    |     | MP                 |     |     |
| RTL8720DN-VA1-CGT  |     | QFN48    |     | tape-and-reel, MP  |     |     |
| RTL8720DN-VT1-CG   |     | QFN48    |     | MP                 |     |     |
| RTL8720DM-VA1-CG   |     | QFN48    |     | MP                 |     |     |
| RTL8720DF-VA1-CG   |     | QFN48    |     | MP                 |     |     |
| RTL8720DF-VT1-CG   |     | QFN48    |     | MP                 |     |     |
| RTL8721DM-VA1-CG   |     | QFN68    |     | MP                 |     |     |
| RTL8721DM-VA1-CGT  |     | QFN68    |     | tape-and-reel, MP  |     |     |
| RTL8722DM-VA1-CG   |     | QFN88    |     | MP                 |     |     |
| RTL8722DM-VA1-CGT  |     | QFN88    |     | tape-and-reel, MP  |     |     |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
4

Contents
Contents
Ordering Information .................................................................................................................................................................. 4
Contents ...................................................................................................................................................................................... 5
1 Product Overview ................................................................................................................................................................. 8
1.1 General Description ............................................................................................................................................................... 8
1.2 System Architecture .............................................................................................................................................................. 8
1.3 Features ................................................................................................................................................................................. 9
1.3.1 System and Memory ....................................................................................................................................................... 9
1.3.2 Wireless ........................................................................................................................................................................ 10
1.3.3 Security ......................................................................................................................................................................... 10
1.3.4 Communication Interface ............................................................................................................................................. 10
1.3.5 Audio ............................................................................................................................................................................ 12
1.3.6 Timer............................................................................................................................................................................. 13
1.3.7 Human-Machine Interaction ........................................................................................................................................ 13
1.3.8 Analog .......................................................................................................................................................................... 14
1.4 Package Comparison ........................................................................................................................................................... 14
1.4.1 Peripherals .................................................................................................................................................................... 14
1.4.2 Memory ........................................................................................................................................................................ 15
2 Package .............................................................................................................................................................................. 17
2.1 Package Types ..................................................................................................................................................................... 17
2.1.1 QFN48 ........................................................................................................................................................................... 19
2.1.2 QFN68 ........................................................................................................................................................................... 21
2.1.3 QFN88 ........................................................................................................................................................................... 22
2.2 Low Power Pins .................................................................................................................................................................... 23
2.3 Pin Default Configuration .................................................................................................................................................... 23
2.4 Trap Pins .............................................................................................................................................................................. 23
3 Analog Pin Descriptions ...................................................................................................................................................... 25
3.1 Power on Trap Pin ............................................................................................................................................................... 25
3.2 RF Pins ................................................................................................................................................................................. 25
3.3 Chip Enable Pin .................................................................................................................................................................... 25
3.4 Power Pins ........................................................................................................................................................................... 25
3.5 XTAL Pins ............................................................................................................................................................................. 26
3.6 ADC and Cap-Touch Pins ..................................................................................................................................................... 26
3.7 USB Pins ............................................................................................................................................................................... 27
3.8 Audio Codec Pins ................................................................................................................................................................. 27
4 Memory Organization ......................................................................................................................................................... 28
4.1 Introduction ......................................................................................................................................................................... 28
4.2 KM4 Memory ....................................................................................................................................................................... 28
4.2.1 Memory Map and Register Boundary Addresses ......................................................................................................... 28
4.2.2 Embedded SRAM .......................................................................................................................................................... 29
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
5

RTL872xD Datasheet
4.2.3 Extension SRAM ............................................................................................................................................................ 30
4.3 KM0 Memory ....................................................................................................................................................................... 30
4.3.1 Memory Map and Register Boundary Addresses ......................................................................................................... 30
4.3.2 Embedded SRAM .......................................................................................................................................................... 30
4.4 Retention SRAM .................................................................................................................................................................. 30
4.5 SPI Flash Memory ................................................................................................................................................................ 31
4.6 PSRAM ................................................................................................................................................................................. 31
5 Nested Vectored Interrupt Controller (NVIC) ...................................................................................................................... 32
5.1 Features ............................................................................................................................................................................... 32
5.2 NVIC Diagram....................................................................................................................................................................... 32
5.3 NVIC Table ........................................................................................................................................................................... 32
6 Radio Characteristics .......................................................................................................................................................... 35
6.1 RF Block Diagram ................................................................................................................................................................. 35
6.2 Wi-Fi Radio Characteristics .................................................................................................................................................. 35
6.2.1 Wi-Fi 2.4GHz Band RF Receiver Specifications.............................................................................................................. 35
6.2.2 Wi-Fi 2.4GHz Band RF Transmitter Specifications ........................................................................................................ 36
6.2.3 Wi-Fi 5GHz Band RF Receiver Specifications ................................................................................................................. 37
6.2.4 Wi-Fi 5GHz Band RF Transmitter Specifications ........................................................................................................... 37
6.3 BT Radio Characteristics ...................................................................................................................................................... 38
6.3.1 BT RF Transmitter Specifications .................................................................................................................................. 38
6.3.2 BT RF Receiver Specifications........................................................................................................................................ 38
7 Electrical Characteristics ..................................................................................................................................................... 39
7.1 Temperature Limit Ratings .................................................................................................................................................. 39
7.2 ESD Characteristics .............................................................................................................................................................. 39
7.3 DC Characteristics ................................................................................................................................................................ 39
7.3.1 Power Supply ................................................................................................................................................................ 39
7.3.2 Digital I/O Pin ............................................................................................................................................................... 40
7.4 ADC Characteristics ............................................................................................................................................................. 40
7.5 XTAL Oscillator ..................................................................................................................................................................... 41
7.6 Power Consumption ............................................................................................................................................................ 42
7.7 Power State and Power Sequence ....................................................................................................................................... 42
7.7.1 Power on or Resuming from Deepsleep Sequence ....................................................................................................... 42
7.7.2 Shutdown Sequence ..................................................................................................................................................... 42
7.8 I/O Port Characteristics ....................................................................................................................................................... 43
7.8.1 GPIO Sink and Source Current ...................................................................................................................................... 43
7.8.2 GPIO Pull Low Restriction ............................................................................................................................................. 44
8 Package Specifications ........................................................................................................................................................ 45
8.1 QFN48 .................................................................................................................................................................................. 45
8.2 QFN68 .................................................................................................................................................................................. 46
8.3 QFN88 .................................................................................................................................................................................. 47
List of Tables .............................................................................................................................................................................. 49
List of Figures ............................................................................................................................................................................ 50
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
6

Contents
Revision History......................................................................................................................................................................... 51
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
7

  RTL872xD Datasheet
1  Product Overview
1.1  General Description
RTL872xD is a highly integrated single-chip low power dual bands (2.4GHz and 5GHz) Wireless LAN (WLAN) and Bluetooth Low Energy (BLE 5.0)
communication controller. It consists of a high performance MCU (Armv8-M, Cortex-M33 instruction set compatible) called Real-M300 (or KM4
thereafter) and a low power MCU (Armv8-M, Cortex-M23 instruction set compatible) called Real-M200 (or KM0 thereafter), WLAN (802.11
a/b/g/n) MAC, an 1T1R capable WLAN baseband, RF, Bluetooth and peripherals.
High speed connectivity interfaces, SDIO and USB are provided. There are also audio codec, Key-Scan and touch keys integrated into this IC.
Besides, flexible design configures GPIO to different functions according to applications.
RTL872xD also integrates memories (ROM/SRAM/PSRAM) for IoT (Internet of Things) Wi-Fi protocol functions and applications. The user-
friendly development kits (SDK and HDK) are supported to customers for developing IoT applications.
The KM4 MCU is a 32-bit core that offers system enhancements such as low power consumption, enhanced debug features, floating point
computation, DSP instructions and a high level of support block integration. The KM4 MCU incorporates a 3-stage pipeline.
The KM0 coprocessor is an energy-efficient and easy-to-use 32-bit core which is code- and tool-compatible with the KM4 core. The KM0
coprocessor offers up to 20MHz performance with a simple instruction set and reduced code size.
1.2  System Architecture
The system architecture of RTL872xD is shown in Figure 1-1.
D a s h e d   b o x e s   a re  o p t i o nal features based on
|     |     | HS_IPC | Power on in deepsleep mode | di ff e r e n t   p a rt   n u m b e r s : |
| --- | --- | ------ | -------------------------- | ------------------------------------------ |

|     |     | IR (Infrared Ray) | Power on in sleep mode | P S RAM |
| --- | --- | ----------------- | ---------------------- | ------- |
   LC D
|     |     |          | Power down in sleep mode |    U S B         |
| --- | --- | -------- | ------------------------ | ----------------- |
|     |     | HS_UART0 |                          |    SD I O Device |
  SD Host
KM4 CPU @ 200MHz SPI0 H/S @50MHz SPI1 H @25MHz   RSIP (Flash Decryption)
  Cap-Touch
| Serial Wire Debug (SWD) |     | USI (UART/SPI/I2C) |     |     |
| ----------------------- | --- | ------------------ | --- | --- |
DSP FPU
Crypto Engine
| SAU x8 IDAU x8  | HS_SRAM 512KB | LCD         |                           |                       |
| --------------- | ------------- | ----------- | ------------------------- | --------------------- |
|                 |               |             | Stereo DAC                | Wi-Fi MAC             |
| MPU x8 MPU_S x4 | PSRAM 4MB     | USB         |                           |                       |
|                 |               |             | MIC in ADC                | 802.11 a/b/g/n        |
| NVIC            | LP_SRAM 64KB  |             |                           |                       |
|                 |               | SDIO Device | DMIC (Digital Microphone) | BLE 5.0               |
| 32KB I-Cache    | SPI Flash     | SD Host     |                           | Wi-Fi/BT Co-existence |
Earphone Driver
| 4KB D-Cache | Retention SRAM 1KB | HS_GDMA                |       |             |
| ----------- | ------------------ | ---------------------- | ----- | ----------- |
|             |                    |                        | I2S   | 2.4GHz 5GHz |
| KM4 CPU     | Memory             | High Speed Peripherals |       |             |
|             |                    |                        | Audio | Wireless    |
Bus Matrix
KM0 CPU @ 20MHz PMU Key-Scan @ 6x6/4x8 LP_PWM x6 @ 40MHz Cap-Touch x4
Serial Wire Debug (SWD) Reset Control GPIO x58 LP_TIM x4 @ 32K
12-bit SAR ADC
| MPU x4       |               | IPC                            | RTC                |           |
| ------------ | ------------- | ------------------------------ | ------------------ | --------- |
|              | Clock Control |                                |                    | VBAT @ 5V |
| NVIC         | eFuse         | I2C x1                         | Q-Decoder          | Analog    |
| 16KB I-Cache | System        | LP_UART0 LP_UART1 (LOG)        | LP_WDT             |           |
| 4KB D-Cache  |               | LP_GDMA x1                     | HS_PWM x18 @ 40MHz |           |
| KM0 CPU      |               | SGPIO (One Wire Communication) |                    |           |
HS_TIM x4 @ 32KHz
|     |     | RSIP (Flash Decryption) | HS_WDT |     |
| --- | --- | ----------------------- | ------ | --- |
|     |     | Low Power Peripherals   | Timer  |     |
Figure 1-1 System architecture
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
8

Product Overview
In RTL872xD, the main system consists of 32-bit multilayer AXI bus matrix that interconnects all the masters and the slaves. The bus matrix
provides access from a master to a slave, enabling concurrent access and efficient operation even when several high-speed peripherals work
simultaneously.
A multilayer AXI bus matrix connects the CPU buses and other bus masters to peripherals in a flexible manner that optimizes performance by
allowing peripherals on different slave ports of the matrix to be accessed simultaneously by different bus masters.
APB peripherals are connected to the AXI bus matrix via APB buses using separate slave ports from the multilayer AXI bus matrix. This allows
for better performance by reducing collisions between the CPU and the DMA controller, and also for peripherals on the asynchronous bridge to
have a fixed clock that does not track the system clock.
1.3 Features
1.3.1 System and Memory
The system and memory features of RTL872xD are listed in Table 1-1.
Table 1-1 System and memory features
Items Description
Processor  Dual processor core
 KM4: Armv8-M architecture with Cortex-M33 instruction set compatible
 KM0: Armv8-M architecture with Cortex-M23 instruction set compatible
 Equal access to address space including SRAM, peripherals and registers
KM4 CPU  Cortex-M33 instruction set compatible
 Floating Point Unit (FPU)
 DSP
 TrustZone-M
 Running at a frequency of up to 200MHz (configurable)
 Memory Protection Unit (MPU)
 Built-in Nested Vectored Interrupt Controller (NVIC)
 Non-maskable Interrupt (NMI) with a selection of sources
 Serial Wire Debug (SWD) with 8 break points and 4 watch points (without Serial Wire Output (SWO)
for enhanced debug capabilities)
 System tick timer
 32KB I-Cache and 4KB D-Cache
KM0 CPU  Cortex-M23 instruction set compatible
 Running at a frequency of up to 20MHz
 Built-in Nested Vectored Interrupt Controller (NVIC)
 Non-maskable Interrupt (NMI) with a selection of sources
 SWD with 4 break points and 2 watch points
 System tick timer
 16KB I-Cache and 4KB D-Cache
KM4 CPU On-Chip memory  Up to 512KB contiguous main SRAM @200MHz
 Optional 4MB PSRAM @ 50MHz, 8-bit DDR, refer to 1.4.2
KM0 CPU On-Chip memory  Up to 64KB contiguous main SRAM
 Up to 1KB retention SRAM for keeping data in power saving modes
GDMA  KM4 and KM0 both have a GDMA controller
 HS-GDMA0 supports six channels with TrustZone-M
 LP-GDMA0 supports six channels without TrustZone-M
Flash  Optional internal 4M bytes (32M bits) Flash, refer to 1.4.2
 SPI/DSPI/QSPI1 Flash controller with cache
 Flash In-Circuit Programming (ICP) supported
General-Purpose I/O (GPIO)  Up to 64 General-Purpose I/O (GPIO) pins. All GPIOs have configurable pull-up/pull-down resistors.
 GPIO interrupt trigger could be configured with rising, falling or both input edges.
IPC Inter-Processor communication
1. Only QFN88 supports QSPI, while QFN48 and QFN68 just support SPI/DSPI.
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
9

RTL872xD Datasheet
1.3.2 Wireless
The wireless features of RTL872xD are listed in Table 1-2.
Table 1-2 Wireless features
Items Description
Wi-Fi  802.11 a/b/g/n 1x1, 2.4GHz & 5GHz
 Supports 20MHz/40MHz up to MCS7
 Low power architecture
 Low power Tx/Rx for short range application @1.8V
 Low power beacon listen mode
 Low power Rx mode
 Very low power suspends mode (DLPS)
 Built-in PA, also supports external PA and LNA
 Supports Antenna diversity
 Internal PTA interface for arbitrating data transmission between Wi-Fi and internal Bluetooth or external 2.4G devices
Bluetooth  BLE 5.0
 Both central and peripheral modes
 High power mode (8dbm, shares the same PA with Wi-Fi)
 Internal co-existence mechanism between Wi-Fi and BT to share the same antenna
1.3.3 Security
The Security features of RTL872xD are listed in Table 1-3.
Table 1-3 Security features
Items Description
Hardware engine AES/DES/SHA hardware engine
TrustZone TrustZone-M supported
Secure boot Secure boot supported
SWD protection Debug port access protection and prohibition modes
eFuse protection Secure eFuse
RSIP Flash decryption on-the-fly
1.3.4 Communication Interface
The communication interface features of RTL872xD are listed in Table 1-4.
Table 1-4 Communication interface features
Items Description
SD/SDIO  SD card host supported
 SDIO 2.0 SDR25 supported
 Realtek SPI provides high efficiency SPI interface with interrupt and full duplex mode
 SDIO inic mode (SDIO to Wi-Fi transformation)
 Clock rate variable up to 50MHz
 Internal DMA supported
 SDIO device time consuming from power on to initialization completion: 64.14635ms
USB  USB 2.0
 HS/FS/LS mode
 Internal DMA support, DMA works based on register settings
 1.5KByte bulk-in buffer and 1.5KByte bulk-out buffer
SPI  Motorola SPI Serial interface operation supported
 Master or slave operation mode supported
 Providing two SPI ports:
 SPI0 (High speed): configured as master or slave with Max. baud rate: 50MHz.
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
10

Product Overview
 SPI1 (Normal speed): configured as master with Max. baud rate: 25MHz.
 DMA interface for DMA transfer supported
 Independent masking of interrupts
 FIFO depth – The transmit and receive FIFO buffers 64 words deep. The FIFO width is fixed at 16
bits.
 Hardware/software slave-select – Dedicated hardware slave-select lines can be used or software
control can be used to target the serial-slave device
 Programmable features:
 Clock bit-rate – Dynamic control of the serial bit rate of the data transfer; used in only serial-
master mode of operation.
 Data item size (4 to 16 bits) – Item size of each data transfer under the control of the
programmer.
 Configurable clock polarity and phase
 Programmable delay on the sample time of the received serial data bit (rxd), when configured
in Master Mode; enables programmable control of routing delays resulting in higher serial
data-bit rates.
UART (HS_UART/LP_UART)  UART format: 1 start bit, 7/8 data bits, 0/1 parity bit and 1/2 stop bit
 Support up to 6MHz baud rate
 Auto flow control supported
 Interrupt control supported
 IrDA supported
 Loopback mode for test
 Fractional baud rate generator
 Low power mode for Rx path
 Monitor and eliminate Rx baud rate error and own frequency drift automatically for Rx path
 DMA mode supported
 LP_UART: DMA mode on KM4 platform not supported
 Option for UART Rx to be DMA flow controller
IR (Infra Ray)  Carrier frequency from 25kHz to 500kHz
 Duty from 1/2 to 1/5
 IR diode input supported
 IR receiver module input supported
 32*4 bytes Tx FIFO
 32*4 bytes Rx FIFO
 Tx carrier frequency can be configured
 Tx carrier duty cycle can be configured
One wire (SGPIO)  One wire communication interface for security element
 Timer Mode:
 Rx and Multiple Timer are 16-bit timer with a 16-bit prescaler.
 Rx and Multiple Timer can stop, reset, and interrupt by match events.
 Rx and multiple timer/counter can stop and reset to each other.
 Capture Mode:
 Rx timer can be captured by capture events.
 Capture events can be Rx trigger events or the multiple match events.
 The capture value can be transferred to ‘0’ or ‘1’ by comparing the value.
 Counter Mode:
 Multiple Counter can count Rx trigger events.
 External Output Mode:
 External output can set high, low or toggle on the match event.
 Get the serial input:
 Shift the input value to a 32-bit FIFO by match events.
 Send the serial output:
 Send the ‘0’or ‘1’ waveform by shifting the output value of a 32-bit FIFO. This output value
decides that the external match uses the match value of group 0 or group 1.
 Change the output value by using the multiple FIFO data to update the multiple match value.
 Monitor Mode:
 Monitor the receiving value to make the interrupt in the power saving mode.
I2C  Two-wire I2C serial interface – consists of a serial data line (SDA) and a serial clock (SCL)
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
11

RTL872xD Datasheet
 One I2C port supported
 Two Speed mode:
 Standard (up to 100Kbps)
 Fast (up to 400Kbps)
 Master or Slave I2C operation
 7- or 10-bit addressing
 Transmit and receive buffers with depth of 16
 Tx and Rx DMA support
 Multi-master ability including bus arbitration scheme
 Slave mode address match wakeup for power save (up to 100Kbps)
 Clock stretch in master/slave mode
 7- or 10-bit combined format transfers
 General Call
 Component parameters for configurable software driver support (programmable SDA hold time,
slave address, etc.)
 Filter to eliminate the glitches on signal of SDA and SCL. Programmable digital noise Filter.
 Status flags (Bus busy flag, activity flag, FIFO status flag, etc.) and Error flags (arbitration lost,
acknowledge failure, etc.)
USI (Universal Serial interface)  Configured as SPI, UART or I2C
 USI I2C can support High Speed Mode
1.3.5 Audio
The audio features of RTL872xD are listed in Table 1-5.
Table 1-5 Audio features
Items Description
Audio DAC and earphone  Sampling Frequency: 8/16/32/44.1/48/88.2/96kHz
driver  Integrates earphone driver
 40mW on 16Ω load
 20mW on 32Ω load
 Gain Control in DAC Path
 Gain Step: 0.375dB/step
 Gain Range: -64.5dB ~ 0dB
 Audio output mode
 Line-Out Cap-less mode (QFN88)
 Line-Out Differential mode (QFN88)
 Line-Out Single-ended mode
Audio ADC  Sampling Frequency: 8/16/32/44.1/48/88.2/96kHz
 ADC input gain range
 Gain Step: 0.375dB/step
 Gain Range: -17.625dB ~ 30dB (digital)
 MIC input boost gain stage: 0/20/30/40dB (analog)
 Audio input mode
 Line-In
 Analog MICx2 or Digital MIC x2
I2S  Sample rate
 8/12/16/24/32/48/64/96/192/384/7.35/11.025/14.7/22.05/29.4/44.1/58.8/88.2/176.4kHz
 I2S channel number
 mono
 stereo
 5.1 channel
 Sample bit for mono
 16-bit
 32-bit
 Sample bit for stereo & 5.1 channel
 16-bit
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
12

Product Overview
 24-bit
 32-bit
 Integrated DMA engine to minimize the software efforts
 Mono and stereo Tx or Rx, or Tx & Rx mode
 5.1 Tx mode (DAC) supported, Rx mode (ADC) not supported
 PCM mode not supported
1.3.6 Timer
The timer features of RTL872xD are listed in Table 1-6.
Table 1-6 Timer features
Items Description
Basic Timers  Channels: x1
(HS_TIM0 ~ HS_TIM3)  Clock source: 32kHz
(LP_TIM0 ~ LP_TIM3)  Resolution: 32-bit
 Counter mode: up
 Interrupt generation
 Sleep mode wakeup
PWM Timers (HS_TIM5 & LP_TIM5)  Channels: HS_TIM5 x18, LP_TIM5 x6
 Clock source: XTAL
 Resolution: 16-bit
 Prescaler: 8-bit
 Counter mode: up
 Statistics pulse width
 Statistics pulse counter
 Input capture pin: x2
 Interrupt generation
 LP_TIM5 can work at sleep mode
Pulse Timers (HS_TIM4 & LP_TIM4)  Channels: HS_TIM5 x18, LP_TIM5 x6
 Clock source: XTAL
 Resolution: 16-bit
 Prescaler: 8-bit
 Counter mode: up
 One pulse mode
 PWM mode with polarity selection
 Input capture pin: x2
 Interrupt generation
Real-Time clock (RTC)  Independent BCD timer/counter
 Time with seconds, minutes, hours, days (12- or 24-hour format)
 Daylight saving compensation programmable by software
 One programmable alarm with interrupt function. The alarm can be triggered by any
combination of the time fields.
 Maskable interrupts/events
 Alarm
 Digital calibration circuit
 Register write protection
1.3.7 Human-Machine Interaction
The human-machine interaction (HMI) features of RTL872xD are listed in Table 1-7.
Table 1-7 Human- machine interaction features
Items Description
Key-matrix  Up to 8*8 (64) Keypad Array with use of 16 GPIOs
 Configurable Rows and Columns of Keypad Array
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
13

RTL872xD Datasheet
 Hardware debounce with configurable time at each scan
 Configurable Scan Clock, Scan Interval and Release Time
 Support interrupts, provide interrupts mask, interrupts clear, interrupts status
 Multi-keys detect
 FIFO with width of 12 bits and depth of 16 to store Key Press and Release Events
 Low power mode. Key press event can wakeup CPU from sleep.
Cap-Touch  4 capacitive sensor channels
 Automatic channel scan: hardware scan each enabled channel automatically in sequence
 Programmable scan period: sample number and scan interval
 Difference or Absolute threshold judgement mode (with ETC function enable)
 Automatic environment sensor capacitance tracking and calibration (ETC)
 Hardware baseline initial automatically
 Automatic baseline and threshold update for different noise environment
 Programmable button debounce function
 Programmable interrupt enabled for each interrupt source
 4*12 bits FIFO
 Low power consumption
LCD  Thin Film Transistor (TFT) color display
 8-bit MCU I8080 parallel interface
 Resolution of 8-bit mode, (1024x1024) for still picture display
 Resolution of 8-bit mode, (645x645) for dynamic display when refresh rate is 30F/S
 6-bit RGB parallel interface
 Resolution of 6-bit mode, less than (527x527) for dynamic display when refresh rate is 60F/S
 RGB565 data format for input & output
 Programmable timings for different display panels
 Programmable polarity for HSYNC, VSYNC and Data Enable
 DMA frame buffer for RGB/MCU I/F
 I/O mode for MCU I/F
1.3.8 Analog
The analog features of RTL872xD are listed in Table 1-8.
Table 1-8 Analog features
Items Description
Data capture ADC and voltage comparator  Resolution: 12-bit SAR
 Available channel number
 7x external 3.3V channel and 1x 5V channel
 3x internal channel
 Configurable input
 Single-ended
 Differential with predefined channel pair
 Contain 64 FIFO entries which is 16-bit width
 Multi-Channel DMA support
 Multi sampling trigger sources
 Software
 Timer
 1 low power voltage comparator for battery voltage measurement
 Conversion item control or another FIFO level to trigger wakeup circuit
1.4 Package Comparison
1.4.1 Peripherals
The peripherals of RTL872xD under different packages are shown in Table 1-9.
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
14

Product Overview
Table 1-9 Peripherals under different packages
| Item  | Peripheral  | Description  | QFN48      |            | QFN68      | QFN88      |
| ----- | ----------- | ------------ | ---------- | ---------- | ---------- | ---------- |
|       |             |              | RTL8720DN  | RTL8720DF  | RTL8721DM  | RTL8722DM  |
RTL8720DM
| UART  | HS_UART0     |                         | -   |    |    |    |
| ----- | ------------ | ----------------------- | --- | --- | --- | --- |
|       |              |                         |    |    |     |    |
|       | HS_USI_UART  |                         |     |     | -   |     |
|       | LP_UART1     | Low power mode wakeup   |    |    |    |    |
|       | LP_UART0     | LOGUART/low power mode  |    |    |    |    |
wakeup
| SPI  | HS_SPI0  | Maximum 50MHz,  | -   |    |    |    |
| ---- | -------- | --------------- | --- | --- | --- | --- |
Master/Slave
|     | HS_SPI1     | Maximum 25MHz, Master  |    |    |    |    |
| --- | ----------- | ---------------------- | --- | --- | --- | --- |
|     | HS_USI_SPI  | Maximum 25MHz,         |    |    |    |    |
Master/Slave
| RTC  | RTC_OUT       |                           |    |    |    |    |
| ---- | ------------- | ------------------------- | --- | --- | --- | --- |
|      | EXT_32K       |                           |    |    |    |    |
|      | LP_TIM4_TRIG  | Timer capture             | -   |    |    |    |
|      | LP_TIM5_TRIG  | Timer capture             | -   |    |    |    |
| IR   | IR            |                           |    |    |    |    |
| I2C  | LP_I2C        | Standard (up to 100Kbps)  |    |    |    |    |
Fast (up to 400Kbps)
|     |             |                           |    |    |     |    |
| --- | ----------- | ------------------------- | --- | --- | --- | --- |
|     | HS_USI_I2C  | Standard/fast/high speed  |     |     | -   |     |
mode (up to 3.33Mbps)
| SDIO  | SDIO 2.0  | Maximum 50MHz  | -   | -   | -   |    |
| ----- | --------- | -------------- | --- | --- | --- | --- |
Device
|      | SD HOST    | Maximum 50MHz  | -   |    | -   |    |
| ---- | ---------- | -------------- | --- | --- | --- | --- |
| PWM  | HS_PWM0 ~  |                | 8   | 13  | 11  | 17  |
17
|       | LP_PWM0 ~ 5  | Low power mode  | 4   | 6   | 6   | 6   |
| ----- | ------------ | --------------- | --- | --- | --- | --- |
| I2S   | I2S          |                 | -   |    |    |    |
| DMIC  | DMIC         |                 |    | -   |    |    |

| LCD  | LCD  | 8-bit/16-bit/RGB mode | -   | -   | -   |     |
| ---- | ---- | --------------------- | --- | --- | --- | --- |
MCU mode
LED mode
| Q-Decoder   | Q-Decoder     |                                  | -        |         |         |         |
| ----------- | ------------- | -------------------------------- | -------- | -------- | -------- | -------- |
| SGPIO       | SGPIO         |                                  |         | -        |         |         |
| Key-Scan    | Key-Scan      |                                  | 5x2/4x3  | 5x2/4x3  | 7x3/5x5  | 4x8/6x6  |
| Wake Pin    | Wake Pin      | Wakeup from deepsleep            | 7        | 7        | 10       | 12       |
| HS Timer    | HS_TIM4_TRIG  | Timer capture                    |         |         |         |         |
|             | HS_TIM5_TRIG  | Timer capture                    |         | -        |         |         |
| Analog Pin  | USB           | USB host or device (selectable)  |         |         |         |         |
|             | ADC           | 0 ~ 3.3V                         | x3       | x2       | x7       | x7       |
|             | VBAT_MEAS     | 0 ~ 5V                           | -        |         |         |         |
|             | Cap-Touch     |                                  | -        | -        | x4       | x4       |
Audio Output  Analog audio codec output  -  -  x2  x2 (Differential)/
|     |     |     |     |     | (Single-ended)  | x2 (Single-ended)  |
| --- | --- | --- | --- | --- | --------------- | ------------------ |
x2 (AUXIN)
Audio Input  Analog audio codec input  -  -  x2  x1 (Differential)/
|     |     |     |     |     | (Single-ended)  | x2 (Single-ended)  |
| --- | --- | --- | --- | --- | --------------- | ------------------ |
x2 (AUXIN)
1.4.2  Memory
The memory of RTL872xD under different packages are shown in Table 1-10.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
15

  RTL872xD Datasheet
Table 1-10 Memory under different packages
| Item            | QFN48      |            |            | QFN68      | QFN88      |
| --------------- | ---------- | ---------- | ---------- | ---------- | ---------- |
|                 | RTL8720DN  | RTL8720DM  | RTL8720DF  | RTL8721DM  | RTL8722DM  |
| Internal Flash  | -          | -          | 4M bytes   | -          | -          |
| PSRAM           | -          | 4M bytes   | -          | 4M bytes   | 4M bytes   |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
16

Package
2  Package
2.1  Package Types
There are three package types named QFN48, QFN68 and QFN88 in RTL872xD, the details are shown in Table 2-1.
Table 2-1 Package types
| Port Name  | QFN48      |            | QFN68      | QFN88      | Trap  |
| ---------- | ---------- | ---------- | ---------- | ---------- | ----- |
|            | RTL8720DN  | RTL8720DF  | RTL8721DM  | RTL8722DM  |       |
RTL8720DM
| PA[0]  |     |     |    |    |     |
| ------ | --- | --- | --- | --- | --- |
| PA[1]  |     |     |     |    |     |
|        |     |     |    |    |     |
PA[2]
PA[3]
| PA[4]    |     |     |    |    |                |
| -------- | --- | --- | --- | --- | -------------- |
| PA[5]    |     |     |     |    |                |
| PA[6]    |     |     |     |    |                |
| PA[7]    |    |    |    |    | UART_DOWNLOAD  |
| PA[8]    |    |    |    |    |                |
| PA[9]    |     |     |     |    |                |
| PA[10]   |     |     |     |    |                |
| PA[11]   |     |     |     |    |                |
| PA[12]   |    |    |    |    | ICFG0          |
|          |    |    |    |    |                |
| PA[13]   |     |     |     |     | ICFG1          |
| PA[14]   |    |    |    |    | ICFG2          |
| PA[15]1  |    |    |    |    | ICFG3          |
| PA[16]1  |    |    |    |    |                |
| PA[17]   |     |     |    |    |                |
| PA[18]   |     |     |    |    |                |
|          |     |     |    |    |                |
PA[19]
| PA[20]  |     |     |     |    |     |
| ------- | --- | --- | --- | --- | --- |
| PA[21]  |     |     |     |    |     |

PA[22]
| PA[23]  |     |     |     |    |     |
| ------- | --- | --- | --- | --- | --- |
| PA[24]  |     |     |     |    |     |
|         |    |    |    |    |     |
PA[25]
| PA[26]  |    |    |    |    |                  |
| ------- | --- | --- | --- | --- | ---------------- |
| PA[27]  |    |    |    |    | NORMAL_MODE_SEL  |
| PA[28]  |    |    |    |    |                  |
PA[29]
| PA[30]  |    |    |    |    | SPS_SEL  |
| ------- | --- | --- | --- | --- | -------- |
| PA[31]  |     |     |     |    |          |
| PB[0]   |     |     |     |    |          |
| PB[1]   |    |    |    |    |          |
| PB[2]   |    |     |    |    |          |
| PB[3]   |    |    |    |    |          |
|         |     |     |    |    |          |
PB[4]
| PB[5]  |     |     |    |    |     |
| ------ | --- | --- | --- | --- | --- |
| PB[6]  |     |     |    |    |     |
|        |     |     |    |    |     |
PB[7]
PB[8]
PB[9]
PB[10]
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
17

  RTL872xD Datasheet
PB[11]
| PB[12]  |     |     |     |    |
| ------- | --- | --- | --- | --- |
| PB[13]  |    |     |    |    |
| PB[14]  |    |     |    |    |
| PB[15]  |     |     |     |    |
| PB[16]  |    |     |    |    |
| PB[17]  |    |     |    |    |
|         |     |    |     |    |
PB[18]
| PB[19]  |     |    |     |    |
| ------- | --- | --- | --- | --- |
| PB[20]  |    |    |     |    |
|         |    |    |     |    |
PB[21]
| PB[22]  |     |    |    |    |
| ------- | --- | --- | --- | --- |
| PB[23]  |     |    |    |    |
PB[24]
PB[25]
| PB[26]  |     |     |    |     |
| ------- | --- | --- | --- | --- |
PB[27]
| PB[28]  |     |     |     |    |
| ------- | --- | --- | --- | --- |
| PB[29]  |     |     |    |    |

PB[30]
| PB[31]  |     |     |    |    |
| ------- | --- | --- | --- | --- |
1. For QFN48, PA[15] & PA[16] are in the High-Z state by default, and they do not affect each other. It is recommended to choose one of
PA[15] & PA[16] instead of using them at the same time.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
18

Package
2.1.1  QFN48
The QFN48 pin assignments for RTL8720DN/RTL8720DM are shown in Figure 2-1.
MARSP_81DV
|     |     | /LLP_3381AV | BSU_3381AV |     | EROC_11DV |
| --- | --- | ----------- | ---------- | --- | --------- |
LLP_11AV
|     |     |     | ]52[AP ]62[AP | ]82[AP ]03[AP ]72[AP |     |
| --- | --- | --- | ------------- | -------------------- | --- |
]3[BP ]1[BP ]2[BP
|            | 36 35 | 34 33 32 | 31 30 29 | 28 27 26 | 25               |
| ---------- | ----- | -------- | -------- | -------- | ---------------- |
| VD1833_PMU | 37    |          |          |          | 24 VD1833_GPIO   |
| GNDSPS     | 38    |          |          |          | 23 PA[15]/PA[16] |
| SW_LX      | 39    |          |          |          | 22 PA[14]        |
VD1833_SPS/
| VD1833_FLASH | 40  |     |     |     | 21 PA[13]      |
| ------------ | --- | --- | --- | --- | -------------- |
| PB[14]       | 41  |     |     |     | 20 PA[12]      |
| PB[13]       | 42  |     |     |     | 19 VR11_RF1    |
| PB[16]       | 43  |     |     |     | 18 VR1833_PA_G |
| PB[17]       | 44  |     |     |     | 17 RFOUT_G     |
RTL8720DN/RTL8720DM
| PB[20] | 45  |     |     |     | 16 RFIN_G |
| ------ | --- | --- | --- | --- | --------- |
LLLLLLL
| PB[21] | 46  |     |     |     | 15 RFGND |
| ------ | --- | --- | --- | --- | -------- |
GxxxA1
| VD11_CORE   | 47                      |                                   |            |                                | 14 RFIN_A/GND |
| ----------- | ----------------------- | --------------------------------- | ---------- | ------------------------------ | ------------- |
| VD1833_GPIO | 48 49 GND (Exposed Pad) |                                   |            |                                | 13 RFIO_A     |
|             | 1 2                     | 3 4 5                             | 6 7 8      | 9 10 11                        | 12            |
|             |                         |                                   | OX IX      |                                |               |
|             | NE_PIHC DXR_GOL_TRAU    | DXT_GOL_TRAU EFA_11AV LATX_3381AV | NYS_3381RV | NYS_11RV 2FR_11RV A_DAP_3381RV | A_AP_3381RV   |
Note: For #23, PA[15] & PA[16] are in the High-Z state by default, and they do not affect each other. It is
recommended to choose one of PA[15] & PA[16] instead of using them at the same time.
Figure 2-1 QFN48 pin assignments for RTL8720DN/RTL8720DM
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
19

  RTL872xD Datasheet
The QFN48 pin assignments for RTL8720DF is shown in Figure 2-2.
|     | MARSP_81DV | /LLP_3381AV | BSU_3381AV |     |           |
| --- | ---------- | ----------- | ---------- | --- | --------- |
|     |            | SAEM_TABV   |            |     | EROC_11DV |
LLP_11AV
|     |     |     | ]52[AP ]62[AP | ]82[AP ]03[AP ]72[AP |     |
| --- | --- | --- | ------------- | -------------------- | --- |
]3[BP ]1[BP
|            | 36 35 | 34 33 32 | 31 30 29 | 28 27 26 | 25               |
| ---------- | ----- | -------- | -------- | -------- | ---------------- |
| VD1833_PMU | 37    |          |          |          | 24 VD1833_GPIO   |
| GNDSPS     | 38    |          |          |          | 23 PA[15]/PA[16] |
| SW_LX      | 39    |          |          |          | 22 PA[14]        |
VD1833_SPS/
| VD1833_FLASH | 40  |     |     |     | 21 PA[13]      |
| ------------ | --- | --- | --- | --- | -------------- |
| PB[18]       | 41  |     |     |     | 20 PA[12]      |
| PB[19]       | 42  |     |     |     | 19 VR11_RF1    |
| PB[20]       | 43  |     |     |     | 18 VR1833_PA_G |
| PB[21]       | 44  |     |     |     | 17 RFOUT_G     |
RTL8720DF
| PB[22] | 45  |     |     |     | 16 RFIN_G |
| ------ | --- | --- | --- | --- | --------- |
LLLLLLL
| PB[23]/PB[24] | 46  |     |     |     | 15 RFGND |
| ------------- | --- | --- | --- | --- | -------- |
GxxxA1
| VD11_CORE   | 47  |     |     |     | 14 RFIN_A/GND |
| ----------- | --- | --- | --- | --- | ------------- |
| VD1833_GPIO | 48  |     |     |     | 13 RFIO_A     |
49 GND (Exposed Pad)
|     | 1 2                  | 3 4 5                             | 6 7 8            | 9 10 11                        | 12          |
| --- | -------------------- | --------------------------------- | ---------------- | ------------------------------ | ----------- |
|     | NE_PIHC DXR_GOL_TRAU | DXT_GOL_TRAU EFA_11AV LATX_3381AV | OX IX NYS_3381RV | NYS_11RV 2FR_11RV A_DAP_3381RV | A_AP_3381RV |
Note: For #23, PA[15] & PA[16] are in the High-Z state by default, and they do not affect each other. It is
recommended to choose one of PA[15] & PA[16] instead of using them at the same time.
Figure 2-2 QFN48 pin assignments for RTL8720DF
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
20

Package
2.1.2  QFN68
The QFN68 pin assignments for RTL8721DM are shown in Figure 2-3.
|     |            | ]7[BP/3YEK_HCUOT     | ]6[BP/2YEK_HCUOT ]5[BP/1YEK_HCUOT ]4[BP/0YEK_HCUOT |                                           |                                 |                      |
| --- | ---------- | -------------------- | -------------------------------------------------- | ----------------------------------------- | ------------------------------- | -------------------- |
|     |            |                      |                                                    |                                           | ]52[AP/1LOC_YEK ]62[AP/0LOC_YEK |                      |
|     |            |                      |                                                    | ]3[BP/6HC_CDA ]2[BP/5HC_CDA ]1[BP/4HC_CDA |                                 |                      |
|     | MARSP_81DV |                      |                                                    |                                           | ]82[AP/6MWP                     | ]03[AP/7MWP          |
|     |            | LLP_3381AV SAEM_TABV |                                                    |                                           |                                 | BSU_3381AV EROC_11DV |
LLP_11AV
|            | 51 50 | 49 48 47 | 46 45 44 | 43 42 41 | 40 39 38 | 37 36 35           |
| ---------- | ----- | -------- | -------- | -------- | -------- | ------------------ |
| VD1833_PMU | 52    |          |          |          |          | 34 VD1833_GPIO     |
| GNDSPS     | 53    |          |          |          |          | 33 SWD_DATA/PA[27] |
| SW_LX      | 54    |          |          |          |          | 32 PA[19]          |
VD1833_SPS/
| VD1833_FLASH | 55  |     |     |     |     | 31 PA[18]      |
| ------------ | --- | --- | --- | --- | --- | -------------- |
| PB[14]       | 56  |     |     |     |     | 30 PA[17]      |
| PB[13]       | 57  |     |     |     |     | 29 PA[16]      |
| PB[16]       | 58  |     |     |     |     | 28 PA[15]      |
| PB[17]       | 59  |     |     |     |     | 27 PA[14]      |
| PB[22]       | 60  |     |     |     |     | 26 PA[13]      |
| PB[23]       | 61  |     |     |     |     | 25 PA[12]      |
| VD11_CORE    | 62  |     |     |     |     | 24 VR11_RF1    |
| PB[26]       | 63  |     |     |     |     | 23 VR1833_PA_G |
RTL8721DM
| PB[29]    | 64  |                 |     |     |     | 22 RFOUT_G    |
| --------- | --- | --------------- | --- | --- | --- | ------------- |
| PB[31]    | 65  | LLLLLLL  GxxxA1 |     |     |     | 21 RFIN_G     |
| AVCC_DRIV | 66  |                 |     |     |     | 20 RFGND      |
|           | 67  |                 |     |     |     | 19 RFIN_A/GND |
AVCC
| AUDIO_GND | 68 69 GND (Exposed Pad)   |                         |                     |                         |                        | 18 RFIO_A                         |
| --------- | ------------------------- | ----------------------- | ------------------- | ----------------------- | ---------------------- | --------------------------------- |
|           | 1 2                       | 3 4 5                   | 6 7 8               | 9 10 11                 | 12 13 14               | 15 16 17                          |
|           | FERV_OIDUA ]0[AP/SAIB_CIM | ]4[AP ]2[AP OIPG_3381DV | NE_PIHC ]7[AP ]8[AP | EFA_11AV LATX_3381AV OX | IX NYS_3381RV NYS_11RV | 2FR_11RV A_DAP_3381RV A_AP_3381RV |
Figure 2-3 QFN68 pin assignment for RTL8721DM
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
21

  RTL872xD Datasheet
2.1.3  QFN88
The QFN88 pin assignments for RTL8722DM are shown in Figure 2-4.
]3[BP/KLC_DWS/6HC_CDA
|     |     | ]7[BP/3YEK_HCUOT ]6[BP/2YEK_HCUOT ]5[BP/1YEK_HCUOT | ]4[BP/0YEK_HCUOT |     | ]72[AP/ATAD_DWS |     |
| --- | --- | -------------------------------------------------- | ---------------- | --- | --------------- | --- |
]1[BP/4HC_CDA ]2[BP/5HC_CDA
|     | MARSP_81DV |                      |     | ]52[AP/MDSH BSU_3381AV  |     | OIPG_3381DV |
| --- | ---------- | -------------------- | --- | ----------------------- | --- | ----------- |
|     |            | LLP_3381AV SAEM_TABV |     | ]62[AP/PDSH ]82[AP/FERR |     | EROC_11DV   |
LLP_11AV
|     |     |     |     |     | ]03[AP ]13[AP | ]42[AP |
| --- | --- | --- | --- | --- | ------------- | ------ |
]0[BP
66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45
| VD1833_PMU  | 67  |     |     |     |     | 44 PA[23] |
| ----------- | --- | --- | --- | --- | --- | --------- |
| GNDSPS      | 68  |     |     |     |     | 43 PA[22] |
| SW_LX       | 69  |     |     |     |     | 42 PA[21] |
| VD1833_SPS/ | 70  |     |     |     |     | 41 PA[20] |
VD1833_FLASH
| PB[12]         | 71  |                 |     |     |     | 40 PA[19]      |
| -------------- | --- | --------------- | --- | --- | --- | -------------- |
| PB[14]         | 72  |                 |     |     |     | 39 PA[18]      |
| PB[13]         | 73  |                 |     |     |     | 38 PA[17]      |
| PB[15]         | 74  |                 |     |     |     | 37 PA[16]      |
| PB[17]         | 75  |                 |     |     |     | 36 PA[15]      |
| PB[16]         | 76  |                 |     |     |     | 35 PA[14]      |
| PB[18]         | 77  |                 |     |     |     | 34 PA[13]      |
| PB[19]         | 78  |                 |     |     |     | 33 PA[12]      |
| PB[20]         | 79  |                 |     |     |     | 32 PA[11]      |
| PB[21]         | 80  | RTL8722DM       |     |     |     | 31 PA[10]      |
| PB[22]         | 81  |                 |     |     |     | 30 PA[9]       |
| PB[23]         | 82  | LLLLLLL  GxxxA1 |     |     |     | 29 VR11_RF1    |
| VD11_CORE      | 83  |                 |     |     |     | 28 VR1833_PA_G |
| AOUTP_R/PB[29] | 84  |                 |     |     |     | 27 RFOUT_G     |
| AOUTN_R/PB[28] | 85  |                 |     |     |     | 26 RFIN_G      |
89 GND (Exposed Pad)
| AOUTN_L/PB[30] | 86  |     |     |     |     | 25 RFGND      |
| -------------- | --- | --- | --- | --- | --- | ------------- |
| AOUTP_L/PB[31] | 87  |     |     |     |     | 24 RFIN_A/GND |
| AVCC_DRIV      | 88  |     |     |     |     |               |
23 RFIO_A
|     | 1   | 2 3 4 5 6 7 | 8 9 10 | 11 12 13 14 15 | 16 17 18 19 | 20 21 22 |
| --- | --- | ----------- | ------ | -------------- | ----------- | -------- |
DNG_OIDUA CCVA FERV_OIDUA ]0[AP/SAIB_CIM ]4[AP/P_1CIM ]2[AP/P_2CIM ]1[AP/N_2CIM ]5[AP/L_NIXUA ]6[AP/R_NIXUA OIPG_3381DV NE_PIHC ]7[AP/DXT_GOL_TRAU ]8[AP/DXR_GOL_TRAU EFA_11AV LATX_3381AV OX IX NYS_3381RV NYS_11RV 2FR_11RV A_DAP_3381RV A_AP_3381RV
Figure 2-4 QFN88 pin assignments for RTL8722DM
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
22

Package
2.2  Low Power Pins
These pins can work and wakeup MCU under deepsleep mode, as Table 2-2 shows. All these pins are located at Key-Scan pins.
Table 2-2 Low power pins
Pin Name/GPIO  Ext32K  Key-Scan Row  Key-Scan Column  Wakeup
| PA[12]  |              | KEY_ROW0  |           | LGPIO[0]  |
| ------- | ------------ | --------- | --------- | --------- |
| PA[13]  |              | KEY_ROW1  |           | LGPIO[1]  |
| PA[14]  | RTC_OUT      | KEY_ROW2  |           | LGPIO[2]  |
| PA[15]  | RTC EXT_32K  | KEY_ROW3  | KEY_COL6  | LGPIO[3]  |
| PA[16]  |              | KEY_ROW4  | KEY_COL5  | LGPIO[0]  |
| PA[17]  |              | KEY_ROW6  | KEY_COL3  | LGPIO[1]  |
| PA[18]  | RTC_OUT      | KEY_ROW5  | KEY_COL4  | LGPIO[2]  |
| PA[19]  |              |           | KEY_COL2  | LGPIO[3]  |
| PA[20]  |              |           | KEY_COL7  | LGPIO[0]  |
| PA[21]  |              | KEY_ROW7  |           | LGPIO[1]  |
| PA[25]  |              |           | KEY_COL1  | LGPIO[2]  |
| PA[26]  |              |           | KEY_COL0  | LGPIO[3]  |
2.3  Pin Default Configuration
All pins are configured as GPIO without pull resistors except some special function like SWD or LOGUART. The pin default configuration is listed
in Table 2-3.
Table 2-3 Pin default configuration
| Pin Name  | Default Function            |     | Default PU/PD         |     |
| --------- | --------------------------- | --- | --------------------- | --- |
| PA[7]     | LOGUART                     |     | Internal UP           |     |
| PA[8]     | LOGUART                     |     | Internal UP           |     |
| PA[13]    | PA[13]                      |     | eFuse Pull Control 0  |     |
| PA[15]    | PA[15]                      |     | eFuse Pull Control 1  |     |
| PA[25]    | PA[25]                      |     | eFuse Pull Control 2  |     |
| PA[27]1   | SWD_DATA when eFuse enable  |     | Internal UP           |     |
| PA[28]    | PA[28]                      |     | eFuse Pull Control 3  |     |
| PA[30]    | PA[30]                      |     | External UP           |     |
| PB[1]     | PB[1]                       |     | eFuse Pull Control 4  |     |
| PB[3]1    | SWD_CLK when eFuse enable   |     | No pull               |     |
| PB[7]     | PB[7]                       |     | eFuse Pull Control 5  |     |
PB[18]2  SWD_CLK when eFuse enable, or SD_D2 when eFuse enable SDIO  No pull
PB[19]2  SWD_DATA when eFuse enable, or SD_D3 when eFuse enable SDIO  eFuse Pull Control 6
| PB[20]  | SD_CMD when eFuse enable SDIO  |     | No pull               |     |
| ------- | ------------------------------ | --- | --------------------- | --- |
| PB[21]  | SD_CLK when eFuse enable SDIO  |     | No pull               |     |
| PB[22]  | SD_D0 when eFuse enable SDIO   |     | eFuse Pull Control 7  |     |
| PB[23]  | SD_D1 when eFuse enable SDIO   |     | No pull               |     |
1. When bit[0] of logical eFuse 0x0e is ‘1’ by default, PA[27] and PB[3] are used for SWD function, and PA[27] is internal pull up.
2. When bit[0] of logical eFuse 0x0e is programmed to ‘0’, PB[18] and PB[19] are used for SWD function, and PB[19] is internal pull up.
2.4  Trap Pins
The trap pins are listed in Table 2-4.
Table 2-4 Trap pins
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
23

  RTL872xD Datasheet
| Pin Name  | Trap Function  | Description  |
| --------- | -------------- | ------------ |
PA[7]  UART_DOWNLOAD  Trigger ISP ROM code Flash download, low active.
| PA[12]  | ICFG0  | Realtek test mode  |
| ------- | ------ | ------------------ |
| PA[13]  | ICFG1  | Realtek test mode  |
| PA[14]  | ICFG2  | Realtek test mode  |
| PA[15]  | ICFG3  | Realtek test mode  |
PA[27]  NORMAL_MODE_SEL  It is not allowed to pull down when power on, otherwise:
 Realtek test mode will be selected.

Boot will fail.
When this pin is not pull-down, the state of PA[12] ~ PA[15] can be ignored.
PA[30]  SPS_SEL   1: Internal 1.1V regulator works at SPS mode
 0: Internal 1.1V regulator works at LDO mode
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
24

Analog Pin Descriptions
3  Analog Pin Descriptions
The signal type used in RTL872xD are shown in Table 3-1.
Table 3-1 Pin type description
| Symbol  | Type       |     |     |     | Symbol  Type   |     |
| ------- | ---------- | --- | --- | --- | -------------- | --- |
| I       | Input pin  |     |     |     | O  Output pin  |     |
P  Power pin  AH  Analog and digital hybrid programmable pin
| PI  | Power input pin  |     |     |     | PO  Power Output pin  |     |
| --- | ---------------- | --- | --- | --- | --------------------- | --- |
3.1  Power on Trap Pin
The power on trap pins in RTL872xD are shown in Table 3-2.
Table 3-2 Power on trap pins
| Symbol           |     | Type  QFN48  | QFN68  | QFN88  | Description         |     |
| ---------------- | --- | ------------ | ------ | ------ | ------------------- | --- |
| NORMAL_MODE_SEL  |     | I  26        | 33     | 48     | Shared with PA[27]  |     |
 1: Normal operation mode
 0: Enter into test/debug mode
| UART_DOWNLOAD  |     | I  3  | 7   | 12  | Shared with PA[7]  |     |
| -------------- | --- | ----- | --- | --- | ------------------ | --- |
 1: Boot from Flash
 0: Download image from UART
| SPS_SEL  |     | I  27  | 36  | 51  | Shared with PA[30]  |     |
| -------- | --- | ------ | --- | --- | ------------------- | --- |
 1: Internal 1.1V regulator works at SPS mode
 0: Internal 1.1V regulator works at LDO mode
3.2
RF Pins
The RF pins in RTL872xD are shown in Table 3-3.
Table 3-3 RF pins
| Symbol  |     | Type  | QFN48  | QFN68  | QFN88  | Description        |
| ------- | --- | ----- | ------ | ------ | ------ | ------------------ |
| RFGND   |     | P     | 15     | 20     | 25     | RF ground          |
| RFIO_A  |     | I/O   | 13     | 18     | 23     | WL 5GHz RF signal  |
RXIN_A/GND  I  14  19  24  WL 5GHz RF input signal or RF ground
| RFIN_G   |     | I   | 16  | 21  | 26  | WL/BT 2.4GHz RF input signal   |
| -------- | --- | --- | --- | --- | --- | ------------------------------ |
| RFOUT_G  |     | O   | 17  | 22  | 27  | WL/BT 2.4GHz RF output signal  |
3.3
Chip Enable Pin
The chip enable pin in RTL872xD is shown in Table 3-4.
Table 3-4 Chip enable pin
| Symbol   | Type  | QFN48  | QFN68  | QFN88  | Description  |     |
| -------- | ----- | ------ | ------ | ------ | ------------ | --- |
| CHIP_EN  | I     | 1      | 6      | 11     | Enable chip  |     |
 1: Enable chip
 0: Shut down chip
3.4
Power Pins
The power pins in RTL872xD are shown in Table 3-5.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
25

  RTL872xD Datasheet
Table 3-5 Power pins
| Symbol  | Type  | QFN48  | QFN68  | QFN88  | Description  |     |
| ------- | ----- | ------ | ------ | ------ | ------------ | --- |
VA1833_XTAL  P  5  10  15  1.8V/3.3V power for Crystal Oscillator
VA11_AFE  P  4  9  14  1.1V power for WL/BT Analog Front End
VR1833_SYN  P  8  13  18  1.8V/3.3V power for RF Synthesizer
| VR11_SYN  | P   | 9   | 14  | 19  | 1.1V power for RF Synthesizer  |     |
| --------- | --- | --- | --- | --- | ------------------------------ | --- |
VR1833_PA_A  P  12  17  22  1.8V/3.3V power for RF 5G Power amplifier
| VR1833_PAD_A  | P   | 11  | 16  | 21  | 1.8V/3.3V power for RF     |     |
| ------------- | --- | --- | --- | --- | -------------------------- | --- |
| VR11_RF2      | P   | 10  | 15  | 20  | 1.1V power for RF 5G path  |     |
VR1833_PA_G  P  18  23  28  1.8V/3.3V power for RF 2.4G Power amplifier
| VR11_RF1  | P   | 19  | 24  | 29  | 1.1V power for RF 2.4G path  |     |
| --------- | --- | --- | --- | --- | ---------------------------- | --- |
VD11_CORE  P  47, 25  62, 35  83, 46  1.1V power for digital core power
VD1833_SPS/VD1833 P  40  55  70  1.8V/3.3V power for Flash I/O power and internal regulator input
| _FLASH  |     |     |     |     | from 1.8V/3.3V to 1.1V  |     |
| ------- | --- | --- | --- | --- | ----------------------- | --- |
SW_LX  P  39  54  69  1.1V power output from Switching/Linear Regulator
GNDSPS  P  38  53  68  Ground for Switching/Linear Regulator
| VA1833_USB  | P   | 31  | 37  | 52  | 3.3V power for USB analog  |     |
| ----------- | --- | --- | --- | --- | -------------------------- | --- |
| VA1833_PLL  | P   | 31  | 49  | 64  | 1.8V/3.3V power for PLL    |     |
| VA11_PLL    | P   | 35  | 50  | 65  | 1.1V power for PLL         |     |
VD18_PSRAM  P  36  51  66  1.8V power output from internal Linear regulator for PSRAM
VD1833_PMU  P  37  52  67  1.8V/3.3V power for Power Management Unit
AVCC_DRIV  P  -  66  88  1.8V/3.3V power supply for internal audio codec LDO.
AVCC  P  -  67  2  AVCC output from internal audio codec LDO, add a 1uF cap as close as
possible.
AUDIO_VREF  P  -  1  3  Codec bandgap reference output, add a 4.7nF cap as close as possible.
| MIC_BIAS  | P   | -   | 2   | 4   | Microphone bias output  |     |
| --------- | --- | --- | --- | --- | ----------------------- | --- |
VD1833_GPIO  P  24, 48  5, 34  10, 45  1.8V/3.3V power for digital GPIO
| AUDIO_GND  | P   | -   | 68  | 1   | Audio ground  |     |
| ---------- | --- | --- | --- | --- | ------------- | --- |
3.5
XTAL Pins
The XTAL pins in RTL872xD are shown in Table 3-6.
Table 3-6 XTAL pins
| Symbol  | Type  | QFN48  | QFN68  | QFN88  | Description                              |     |
| ------- | ----- | ------ | ------ | ------ | ---------------------------------------- | --- |
| XI      | I     | 7      | 12     | 17     | Input of 40MHz Crystal Clock Reference   |     |
| XO      | O     | 6      | 11     | 16     | Output of 40MHz Crystal Clock Reference  |     |
3.6
ADC and Cap-Touch Pins
The ADC and Cap-Touch pins in RTL872xD are shown in Table 3-7.
Table 3-7 ADC and Cap-Touch pins
| Symbol  | Type  | QFN48      |            | QFN68      | QFN88      | Description  |
| ------- | ----- | ---------- | ---------- | ---------- | ---------- | ------------ |
|         |       | RTL8720DN  | RTL8720DF  | RTL8721DM  | RTL8722DM  |              |
RTL8720DM
ADC_0/TOUCH_KEY0  I  -  -  44  59  ADC or Cap-Touch input pin, 3.3V tolerance
ADC_1/TOUCH_KEY1  I  -  -  45  60  ADC or Cap-Touch input pin, 3.3V tolerance
ADC_2/TOUCH_KEY2  I  -  -  46  61  ADC or Cap-Touch input pin, 3.3V tolerance
ADC_3/TOUCH_KEY3  I  -  -  47  62  ADC or Cap-Touch input pin, 3.3V tolerance
| ADC_4  | I   | 33  | 33  | 41  | 57  | ADC input pin, 3.3V tolerance  |
| ------ | --- | --- | --- | --- | --- | ------------------------------ |
| ADC_5  | I   | 32  | -   | 42  | 56  | ADC input pin, 3.3V tolerance  |
| ADC_6  | I   | 34  | 34  | 43  | 58  | ADC input pin, 3.3V tolerance  |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
26

Analog Pin Descriptions
| VBAT_MEAS  | I   | -   | 32  | 48  | 63  | ADC input pin, 5V tolerance  |
| ---------- | --- | --- | --- | --- | --- | ---------------------------- |
3.7  USB Pins
The USB pins in RTL872xD are shown in Table 3-8.
Table 3-8 USB pins
| Symbol  | Type  | QFN48  | QFN68  | QFN88  | Description            |     |
| ------- | ----- | ------ | ------ | ------ | ---------------------- | --- |
| HSDP    | I/O   | 29     | 39     | 54     | USB differential bus   |     |
| HSDM    | I/O   | 30     | 40     | 55     | USB differential bus   |     |
RREF  I  28  38  53  External reference resistor for USB analog, 1% accuracy
3.8  Audio Codec Pins
The audio codec pins in RTL872xD are shown in Table 3-9.
Table 3-9 Audio codec pins
| Symbol  | Type  | QFN48  QFN68  | QFN88  | Description  |     |     |
| ------- | ----- | ------------- | ------ | ------------ | --- | --- |
MIC1_P  AH  -  3  5  MIC1 input positive pad. Used as main MIC in dual MIC application.
Programmable digital I/O, refer to pinmux table (UM0402 RTL872xD pinmux table).
MIC2_N  AH  -  -  7  MIC2 input negative pad. Used as 2nd MIC in dual MIC application.
Programmable digital I/O, refer to pinmux table.
MIC2_P  AH  -  4  6  MIC2 input positive pad. Used as 2nd MIC in dual MIC application.
Programmable digital I/O, refer to pinmux table.
AOUTN_R  AH  -  -  85  Right channel analog output negative pad.
Programmable digital I/O, refer to pinmux table.
AOUTP_R  AH  -  64  84  Right channel analog output positive pad.
Programmable digital I/O, refer to pinmux table.
AOUTN_L  AH  -  -  86  Left channel analog output negative pad.
Programmable digital I/O, refer to pinmux table.
AOUTP_L  AH  -  65  87  Left channel analog output positive pad.
Programmable digital I/O, refer to pinmux table.
| AUXIN_R  | AH  | -  -  | 9   | AUX input right channel pad.  |     |     |
| -------- | --- | ----- | --- | ----------------------------- | --- | --- |
Programmable digital I/O, refer to pinmux table.
| AUXIN_L  | AH  | -  -  | 8   | AUX input left channel pad.  |     |     |
| -------- | --- | ----- | --- | ---------------------------- | --- | --- |
Programmable digital I/O, refer to pinmux table.
AVCC_DRIV  PI  -  66  88  1.8V/3.3V power supply for internal audio codec LDO.
| MIC_BIAS  | PO  | -  2  | 4   | Microphone bias output  |     |     |
| --------- | --- | ----- | --- | ----------------------- | --- | --- |
AUDIO_VREF  PO  -  1  3  Codec bandgap reference output, add a 4.7nF cap as close as possible.
AVCC  PO  -  67  2  AVCC output from internal audio codec LDO, add a 1uF cap as close as possible.
| AUDIO_GND  | PO  | -  68  | 1   | Audio ground  |     |     |
| ---------- | --- | ------ | --- | ------------- | --- | --- |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
27

  RTL872xD Datasheet
4  Memory Organization
4.1  Introduction
The RTL872xD incorporates several distinct memory regions. Program memory, data memory, registers and I/O ports are organized within the
same linear 4Gbytes address space. The bytes are coded in memory in Little-Endian format.
The addressable memory space is divided into multiple main blocks, as shown in Table 4-1. All the memory areas that are not allocated to on-
chip memories and peripherals are considered “RSVD” (reserved).
Table 4-1 Address space main blocks
| Base Address  | Top Address  | Size  Function  |     | Description  |     |
| ------------- | ------------ | --------------- | --- | ------------ | --- |
0x0000_0000  0x0001_FFFF  128KB  KM0 ITCM ROM (actually 96KB)  32MB: KM0 Memory Address
| 0x0002_0000  | 0x0002_7FFF  | 32KB  KM0 DTCM ROM (actually 16KB)                  |     |     |     |
| ------------ | ------------ | --------------------------------------------------- | --- | --- | --- |
| 0x0002_8000  | 0x0007_FFFF  | 352KB  RSVD                                         |     |     |     |
| 0x0008_0000  | 0x0008_FFFF  | 64KB  KM0 SRAM                                      |     |     |     |
| 0x0009_0000  | 0x000B_FFFF  | 192KB  RSVD                                         |     |     |     |
| 0x000C_0000  | 0x000C_3FFF  | 16KB  Retention SRAM (actually 1KB) (the same port  |     |     |     |
with KM0 SRAM)
| 0x000C_4000  | 0x000F_FFFF  | 240KB  RSVD  |     |     |     |
| ------------ | ------------ | ------------ | --- | --- | --- |
| 0x0010_0000  | 0x01FF_FFFF  | 31MB  RSVD   |     |     |     |
0x0200_0000  0x07FF_FFFF  96MB  External PSRAM  224MB: External Memory Address
| 0x0800_0000  | 0x0FFF_FFFF  | 128MB  External Flash  |     |     |     |
| ------------ | ------------ | ---------------------- | --- | --- | --- |
0x1000_0000  0x1007_FFFF  512KB  KM4 SRAM  256MB: KM4 Memory Address
| 0x1008_0000  | 0x100D_FFFF  | 384KB  RSVD  |     |     |     |
| ------------ | ------------ | ------------ | --- | --- | --- |
0x100E_0000  0x100F_FFFF  128KB  Extension SRAM (the same port with KM4 SRAM)
| 0x1010_0000  | 0x101B_FFFF  | 768KB  KM4 ITCM ROM (actually 256KB)  |     |           |     |
| ------------ | ------------ | ------------------------------------- | --- | --------- | --- |
| 0x101C_0000  | 0x101D_7FFF  | 96KB  KM4 DTCM ROM                    |     |           |     |
| 0x101D_8000  | 0x101F_FFFF  | 160KB  RSVD                           |     |           |     |
| 0x1020_0000  | 0x1FFF_FFFF  | 254MB  RSVD                           |     |           |     |
| 0x2000_0000  | 0x3FFF_FFFF  | 512MB  RSVD                           |     | Reserved  |     |
0x4000_0000  0x47FF_FFFF  128MB  KM4 Peripherals  KM4 Peripherals Address
0x4800_0000  0x4FFF_FFFF  128MB  KM0 Peripherals  KM4 Peripherals Secure Address
0x5000_0000  0x57FF_FFFF  128MB  KM4 Peripherals Secure  KM0 Peripherals Address
| 0x5800_0000  | 0xDFFF_FFFF  | 2176MB  RSVD  |     | Reserved  |     |
| ------------ | ------------ | ------------- | --- | --------- | --- |
0xE000_0000  0xE0FF_FFFF  16MB  System PPB Device  RAM predefined
| 0xE100_0000  | 0xFFFF_FFFF  | 496MB  RSVD  |     | Reserved  |     |
| ------------ | ------------ | ------------ | --- | --------- | --- |
For the detailed mapping of available memory and register areas, refer to the following sections.
4.2  KM4 Memory
4.2.1  Memory Map and Register Boundary Addresses
Table 4-2 gives the boundary addresses of the peripherals available in the KM4 devices.
Table 4-2 KM4 register boundary addresses
| Port Name       |     | Security  | Base Address  | Top Address  | Size   |
| --------------- | --- | --------- | ------------- | ------------ | ------ |
| KM4_SRAM1       |     | IDAU      | 0x1000_0000   | 0x1003_FFFF  | 256KB  |
| KM4_SRAM2       |     | IDAU      | 0x1004_0000   | 0x1007_FFFF  | 256KB  |
| Extension SRAM  |     | IDAU      | 0x100E_0000   | 0x100F_FFFF  | 128KB  |
| PSRAM Memory    |     | IDAU      | 0x0200_0000   | 0x07FF_FFFF  | 96MB   |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
28

Memory Organization
| HS_SYSON         | Non-Secure  | 0x4000_0000  | 0x4000_0FFF  | 4KB  |
| ---------------- | ----------- | ------------ | ------------ | ---- |
|                  | Secure      | 0x5000_0000  | 0x5000_0FFF  | 4KB  |
| HS_TIM0 ~ 3/4/5  | Non-Secure  | 0x4000_2000  | 0x4000_2FFF  | 4KB  |
| HS_UART0         | Non-Secure  | 0x4000_4000  | 0x4000_4FFF  | 4KB  |
| HS_IPC           | Non-Secure  | 0x4000_6000  | 0x4000_6FFF  | 4KB  |
| HS_USI           | Non-Secure  | 0x4000_8000  | 0x4000_83FF  | 1KB  |
HS_UART1 (Bluetooth)  Non-Secure  0x4000_A000  0x4000_AFFF  4KB
| RXI300_KM4        | Non-Secure  | 0x4000_C000  | 0x4000_CFFF  | 4KB    |
| ----------------- | ----------- | ------------ | ------------ | ------ |
|                   | Secure      | 0x5000_C000  | 0x5000_CFFF  | 4KB    |
| HS_SPI1           | Non-Secure  | 0x4000_E000  | 0x4000_E7FF  | 2KB    |
| Audio Codec       | Non-Secure  | 0x4001_0000  | 0x4001_0FFF  | 4KB    |
| HS_IR             | Non-Secure  | 0x4001_2000  | 0x4001_2FFF  | 4KB    |
| PSRAM Controller  | Non-Secure  | 0x4001_4000  | 0x4001_4FFF  | 4KB    |
| I2S               | Non-Secure  | 0x4002_0000  | 0x4002_03FF  | 1KB    |
| Secure Engine     | Non-Secure  | 0x4002_2000  | 0x4002_5FFF  | 16KB   |
|                   | Secure      | 0x5002_2000  | 0x5002_5FFF  | 16KB   |
| SDIO Host         | Non-Secure  | 0x4002_6000  | 0x4002_9FFF  | 16KB   |
| HS_GDMA0          | Non-Secure  | 0x4002_A000  | 0x4002_BFFF  | 8KB    |
|                   | Secure      | 0x5002_A000  | 0x5002_BFFF  | 8KB    |
| SDIO Device       | Non-Secure  | 0x4002_C000  | 0x4002_FFFF  | 16KB   |
| USB               | Non-Secure  | 0x4004_0000  | 0x4006_FFFF  | 192KB  |
| LCD Controller    | Non-Secure  | 0x4007_0000  | 0x4007_4FFF  | 20KB   |
| HS_SPI0           | Non-Secure  | 0x4007_8000  | 0x4007_87FF  | 2KB    |
| Wi-Fi             | Non-Secure  | 0x4008_0000  | 0x400A_FFFF  | 192KB  |
| KM0 BRG           | Non-Secure  | 0x0008_0000  | 0x0008_FFFF  | 64KB   |
|                   |             | 0x000C_0000  | 0x000C_3FFF  | 16KB   |
|                   |             | 0x4800_0000  | 0x4803_FFFF  | 256KB  |
| Flash Controller  | Non-Secure  | 0x4808_0000  | 0x4808_0FFF  | 4KB    |
| Flash Memory      | IDAU        | 0x0800_0000  | 0x0FFF_FFFF  | 128MB  |
4.2.2  Embedded SRAM
The KM4 contains up to a total 512KB of contiguous, on-chip static RAM memory. This embedded SRAM can be accessed as bytes (8 bits), half-
words (16 bits) or full words (32 bits). It is divided into the following two blocks which can be accessed by both KM4 and KM0.
 KM4 SRAM1 (up to 256KB)

KM4 SRAM2 (up to 256KB)
Dividing SRAM into two slave ports allows user’s program to potentially obtain better performance. For example, simultaneous access to
SRAM1 by the CPU and by the system DMA controller does not result in any bus stalls for either master.
Generally speaking, the CPU reads or writes all peripheral data at some point, even when all such data is read from or sent to a peripheral by
DMA. So, minimizing stalls is likely to involve putting data to/from different peripherals in RAM on each port.
Alternatively, sequences of data from the same peripheral can be alternated between RAM on each port. This could be helpful if DMA fills or
empties a RAM buffer, and signals the CPU before proceeding on to a second buffer. The CPU then tends to access the data while the DMA is
using the other RAM.
In power domains, the entire SRAM is also divided into three blocks:
 SRAM_PD1 (up to 256KB)
 SRAM_PD2 (up to 128KB)
 SRAM_PD3 (up to 128KB)
Each block can be disabled or enabled individually in the Power Management Unit (PMU) block to save power, and the entire SRAM can also
keep power for quickly resuming from sleep mode when system enters sleep mode.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
29

  RTL872xD Datasheet
4.2.3  Extension SRAM
When Bluetooth is disabled, more 64KB SRAM will be extended. This SRAM can also be accessed by both KM4 and KM0, up to 50MHz*32 bits.
4.3  KM0 Memory
4.3.1  Memory Map and Register Boundary Addresses
Table 4-3 gives the boundary addresses of the peripherals available in the KM0 devices.
Table 4-3 KM0 register boundary addresses
| Port Name           | Base Address  | Top Address  | Size   |
| ------------------- | ------------- | ------------ | ------ |
| KM0_SRAM            | 0x0008_0000   | 0x0008_FFFF  | 64KB   |
| 1KB Retention SRAM  | 0x000C_0000   | 0x000C_3FFF  | 16KB   |
| KM4 BRG             | 0x1000_0000   | 0x1007_FFFF  | 512KB  |
|                     | 0x4000_0000   | 0x4007_FFFF  | 512KB  |
| Wi-Fi FW            | 0x4008_0000   | 0x400A_FFFF  | 192KB  |
| LP_SYSON            | 0x4800_0000   | 0x4800_0FFF  | 4KB    |
| LP_TIM0 ~ 3/4/5     | 0x4800_2000   | 0x4800_2FFF  | 4KB    |
| LP_RTC              | 0x4800_4000   | 0x4800_43FF  | 1KB    |
| LP_IPC              | 0x4800_6000   | 0x4800_63FF  | 1KB    |
| Key-Scan            | 0x4800_A000   | 0x4800_A3FF  | 1KB    |
| I2C0                | 0x4800_C000   | 0x4800_C3FF  | 1KB    |
| UART3               | 0x4800_E000   | 0x4800_E3FF  | 1KB    |
| LP_GDMA0            | 0x4801_0000   | 0x4801_07FF  | 2KB    |
| UART2 (LOGUART)     | 0x4801_2000   | 0x4801_23FF  | 1KB    |
| GPIOA/B             | 0x4801_4000   | 0x4801_47FF  | 2KB    |
| RXI300_KM0          | 0x4801_8000   | 0x4801_8FFF  | 4KB    |
| SGPIO               | 0x4801_A000   | 0x4801_AFFF  | 4KB    |
| Cap-Touch           | 0x4801_C000   | 0x4801_C7FF  | 2KB    |
| ADC                 | 0x4801_C800   | 0x4801_CBFF  | 1KB    |
| Comparator          | 0x4801_CC00   | 0x4801_CFFF  | 1KB    |
| Q-Decoder           | 0x4801_E000   | 0x4801_EFFF  | 4KB    |
| Flash Controller    | 0x4808_0000   | 0x4808_0FFF  | 4KB    |
| Flash Memory        | 0x0800_0000   | 0x0FFF_FFFF  | 128MB  |
4.3.2  Embedded SRAM
The KM0 features 64KB of system SRAM, the embedded SRAM can be accessed as bytes (8 bits), half-words (16 bits) or full words (32 bits).
This SRAM can be accessed by both KM4 and KM0.
4.4
Retention SRAM
The RTL872xD features 1KB of retention SRAM in order to allow saving data with minimal power usage during deepsleep mode.
This SRAM can be accessed by both KM4 and KM0.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
30

Memory Organization
4.5 SPI Flash Memory
The SPI Flash Controller manages CPU I-Code and D-Code accesses to the Flash memory. It implements the erase and program Flash memory
operations, and the read/write protection mechanisms. It accelerates code execution with a system of instruction prefetch and cache lines.
4.6 PSRAM
4MB 8IO DDR PSRAM is included in RTL872xD, up to 50MHz DDR.
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
31

RTL872xD Datasheet
5 Nested Vectored Interrupt Controller (NVIC)
5.1 Features
All interrupts including the core exceptions are managed by the NVIC. The nested vector interrupt controller NVIC includes the following
features:
 Nested Vectored Interrupt Controller that is an integral part of each CPU.
 Tightly coupled interrupt controller provides low interrupt latency.
 Controls system exceptions and peripheral interrupts.
 The NVIC of the KM4 supports:
 58 vectored interrupts.
 8 programmable interrupt priority levels with hardware priority level masking.
 Vector table offset register VTOR.
 The NVIC of the KM0 supports:
 32 vectored interrupts.
 4 programmable interrupt priority levels with hardware priority level masking.
 Vector table offset register VTOR.
 Support for NMI from any interrupt.
The NVIC and the processor core interface are closely coupled, which enables low latency interrupt processing and efficient processing of late
arriving interrupts.
5.2 NVIC Diagram
The diagram of NVIC is shown in Figure 5-1.
KM4 KM0
NVIC NVIC
3~13/15/16/29/30
0~2/14/
(32~63) (0~31)
17~28/31
KM4 Interrupt Sources KM0 Interrupt Sources
Figure 5-1 NVIC diagram
Most of KM0 interrupt signals are linked to KM4 NVIC at the same interrupt number like LOGUART, and other KM0 interrupt signals are not
linked to KM4 NVIC like WDG/RXI300/IPC.
This mechanism let KM4 use KM0’s peripheral like it is KM4’s peripheral. But if an interrupt signal is shared by KM0 and KM4, just only one CPU
can open this interrupt, if not, software will hang.
5.3 NVIC Table
Table 5-1 lists the interrupt sources for each peripheral function. Each peripheral device may have one or more interrupt lines to the Vectored
Interrupt Controller. Each line may represent more than one interrupt source. The interrupt number does not imply any interrupt priority.
The macro “configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY”, which is used to define the highest interrupt priority controlled by
FreeRTOS, is very important to interrupt. If the interrupt safe FreeRTOS API functions are called by the interrupt service routine, the priority
of it must not be higher than this macro. Do not call RTOS _ISR functions from any interrupt that has a higher priority than this macro.
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
32

Nested Vectored Interrupt Controller (NVIC)
Table 5-1 NVIC table
| IRQ  KM4 Name  | KM0 Name  | Description  |
| -------------- | --------- | ------------ |
| Reset          | Reset     | Reset        |
NMI  NMI  Non-maskable interrupts (external NMI input). The WDG is linked to the NMI
vector
Hard Fault  Hard Fault  All fault conditions if the corresponding fault handler is not enabled
MemManager Fault  MemManager Fault  Memory management fault; Memory Protection Unit (MPU) violation or access to
illegal locations
Bus Fault  Bus Fault  Bus error; occurs when Advanced High-Performance Bus (AHB) interface receives
an error response from a bus slave (also called pre-fetch abort if it is an
instruction fetch or data abort if it is a data access)
Usage Fault  Usage Fault  Exceptions resulting from program error or trying to access coprocessor (the
Cortex-M4 does not support a coprocessor)
| RSVD  | RSVD  | Reserved         |
| ----- | ----- | ---------------- |
| SVC   | SVC   | Supervisor Call  |
Debug Monitor  Debug Monitor  Debug monitor (breakpoints, watch-points, or external debug requests)
| RSVD     | RSVD     | Reserved               |
| -------- | -------- | ---------------------- |
| PendSV   | PendSV   | Pendable Service Call  |
| SYSTICK  | SYSTICK  | System Tick Timer      |
0  KM4_System_ISR  KM0_System_ISR   KM4: System ISR include wakeup event, like HS BT/ USB-dev/SDIO-
dev/UART0
 KM0: LS peripheral wakeup event, like GPIO/LUART/I2C0, etc.
1  WDG_ISR_H  WDG_ISR_L  KM4 & KM0 watchdog warning interrupt
2  RXI300_IRQ_H  RXI300_IRQ_L  KM4 & KM0 RXI300 platform interrupt
3  UART log  UART log  KM4 & KM0 LP_UART0 (LOGUART) interrupt
4  GPIOA  GPIOA  KM4 & KM0 GPIO PortA Interrupt (GPIO IP0 PORTA)
| 5  RTC        | RTC        | KM4 & KM0 RTC interrupt        |
| ------------- | ---------- | ------------------------------ |
| 6  I2C0       | I2C0       | KM4 & KM0 I2C0 interrupt       |
| 7  SPI_FLASH  | SPI_FLASH  | KM4 & KM0 SPI FLASH interrupt  |
8  GPIOB  GPIOB  KM4 & KM0 GPIO PortB Interrupt (GPIO IP1 PORTA)
| 9  LUART       | LUART      | KM4 & KM0 LP_UART1 interrupt   |
| -------------- | ---------- | ------------------------------ |
| 10  Key-Scan   | Key-Scan   | KM4 & KM0 Key-Scan interrupt   |
| 11  Cap-Touch  | Cap-Touch  | KM4 & KM0 Cap-Touch interrupt  |
| 12  BOR2       | BOR2       | KM4 & KM0 BOR Interrupt        |
| 13  SGPIO      | SGPIO      | KM4 & KM0 SGPIO interrupt      |
| 14  IPC_KM0    | IPC_KM4    | IPC interrupt:                 |
 KM0 IPC interrupt KM4
 KM4 IPC interrupt KM0
| 15  ADC       | ADC       | KM4 & KM0 ADC interrupt        |
| ------------- | --------- | ------------------------------ |
| 16  Q-Decoder | Q-Decoder | KM4 & KM0 Q-Decoder interrupt  |
| 17  HTimer0   | LTimer0   |  KM4 HTimer0 interrupt        |
 KM0 LTimer0 interrupt
| 18  HTimer1  | LTimer1  |  KM4 HTimer1 interrupt |
| ------------ | -------- | ----------------------- |
 KM0 LTimer1 interrupt
| 19  HTimer2  | LTimer2  |  KM4 HTimer2 interrupt |
| ------------ | -------- | ----------------------- |
 KM0 LTimer2 interrupt
| 20  HTimer3  | LTimer3  |  KM4 HTimer3 interrupt |
| ------------ | -------- | ----------------------- |
 KM0 LTimer3 interrupt
| 21  HTimer4  | LTimer4  |  KM4 HTimer4 interrupt |
| ------------ | -------- | ----------------------- |
 KM0 LTimer4 interrupt
| 22  HTimer5  | LTimer5  |  KM4 HTimer5 interrupt |
| ------------ | -------- | ----------------------- |

KM0 LTimer5 interrupt
| 23  LCDC  | LGDMA0_Channel0  |  KM4 LCDC interrupt |
| --------- | ---------------- | -------------------- |
 KM0 LGDMA0_Channel0 interrupt
| 24  USB  | LGDMA0_Channel1  |  KM4 USB interrupt |
| -------- | ---------------- | ------------------- |
 KM0 LGDMA0_Channel1 interrupt
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
33

  RTL872xD Datasheet
| 25  SDIO_DEV  | LGDMA0_Channel2  |  KM4 SDIO_DEV interrupt |
| ------------- | ---------------- | ------------------------ |
 KM0 LGDMA0_Channel2 interrupt
| 26  SD_HOST  | Wi-Fi FISR (0x134) +  |  KM4 SD_HOST interrupt              |
| ------------ | --------------------- | ------------------------------------ |
|              | FESR (0x124)          |  KM0 Wi-Fi FISR + FESR interrupt    |
| 27  IPSEC    | Wi-Fi FTSR (0x13C) +  |  KM4 SD_HOST interrupt              |
|              | mailbox               |  KM0 Wi-Fi FTSR + mailbox interrupt |
| 28  I2S0     | LGDMA0_Channel3       |  KM4 I2S0 interrupt                 |
 KM0 LGDMA0_Channel3 interrupt
29  PWR_DOWN  PWR_DOWN  KM4 & KM0 PWR_DOWN interrupt, this interrupt will be triggered when power
down pin push under power down interrupt mode
| 30  ADC_COMP  | ADC_COMP      | KM4 & KM0 ADC comparator interrupt  |
| ------------- | ------------- | ----------------------------------- |
| 31  WL_DMA    | KM4_WAKE_ISR  |  KM4 WL_DMA interrupt              |
 KM0: KM4 peripherals wake interrupt, the same as KM4_System_ISR
| 32  WL_PROTOCOL (0xB4)  | -   | KM4 WL_PROTOCOL interrupt          |
| ----------------------- | --- | ---------------------------------- |
| 33  PSRAMC              | -   | KM4 PSRAMC interrupt               |
| 34  UART0               | -   | KM4 HS_UART0 interrupt             |
| 35  UART1_BT            | -   | KM4 HS_UART1_BT interrupt          |
| 36  SPI0                | -   | KM4 HS_SPI0 interrupt              |
| 37  SPI1                | -   | KM4 HS_SPI1 interrupt              |
| 38  HUSI0               | -   | KM4 HS_USI0 interrupt              |
| 39  IR                  | -   | KM4 IR interrupt                   |
| 40  BT_SCORE_BOARD      | -   | KM4 BT_SCORE_BOARD interrupt       |
| 41  GDMA0_Channel0      | -   | KM4 HS_GDMA0_Channel0 interrupt    |
| 42  GDMA0_Channel1      | -   | KM4 HS_GDMA0_Channel1 interrupt    |
| 43  GDMA0_Channel2      | -   | KM4 HS_GDMA0_Channel2 interrupt    |
| 44  GDMA0_Channel3      | -   | KM4 HS_GDMA0_Channel3 interrupt    |
| 45  GDMA0_Channel4      | -   | KM4 HS_GDMA0_Channel4 interrupt    |
| 46  GDMA0_Channel5      | -   | KM4 HS_GDMA0_Channel5 interrupt    |
| 47  RSVD                | -   | Reserved                           |
| 48  RSVD                | -   | Reserved                           |
| 49  RSVD                | -   | Reserved                           |
| 50  IPSEC_S             | -   | KM4 HS_IPSEC TrustZone interrupt   |
| 51  RXI300_IRQ_S        | -   | KM4 HS_RXI300 TrustZone interrupt  |
52  GDMA0_Channel0_S  -  KM4 HS_GDMA0_Channel0 TrustZone interrupt
53  GDMA0_Channel1_S  -  KM4 HS_GDMA0_Channel1 TrustZone interrupt
54  GDMA0_Channel2_S  -  KM4 HS_GDMA0_Channel2 TrustZone  interrupt
55  GDMA0_Channel3_S  -  KM4 HS_GDMA0_Channel3 TrustZone interrupt
56  GDMA0_Channel4_S  -  KM4 HS_GDMA0_Channel4 TrustZone interrupt
57  GDMA0_Channel5_S  -  KM4 HS_GDMA0_Channel5 TrustZone interrupt
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
34

Radio Characteristics
6  Radio Characteristics
6.1  RF Block Diagram
The radio frequency (RF) block diagram of RTL872xD, including Wi-Fi and BLE modem, is given in Figure 6-1.
DAC
 Wi-Fi modem
IQ
TRSW
5G M
IQ
BLE modem
ADC
Diplexer
IQ
2G/BT
|     | M   | IQ  |     |     |     |
| --- | --- | --- | --- | --- | --- |
Figure 6-1 RF block diagram
6.2  Wi-Fi Radio Characteristics
The RF specifications of Wi-Fi are described in the tables below. These values are measured on the QFN68 board.
6.2.1  Wi-Fi 2.4GHz Band RF Receiver Specifications
Parameter  Description  Performance (1.8V)  Performance (3.3V)  Unit
|     |     | Min.  Typ.  | Max.  Min.  | Typ.  Max.  |     |
| --- | --- | ----------- | ----------- | ----------- | --- |
Frequency Range  Center channel frequency  2412  -  2484  2412  -  2484  MHz
| Rx Sensitivity  | 1Mbps CCK    | -  -98  | -  -  | -98 -  | dBm  |
| --------------- | ------------ | ------- | ----- | ------ | ---- |
|                 | 2Mbps CCK    | -  -96  | -  -  | -96 -  |      |
|                 | 5.5Mbps CCK  | -  -94  | -  -  | -94 -  |      |
|                 | 11Mbps CCK   | -  -91  | -  -  | -91 -  |      |
Rx Sensitivity  BPSK rate 1/2, 6Mbps OFDM  -  -96  -  -  -95 -  dBm
|     | BPSK rate 3/4, 9Mbps OFDM   | -  -94  | -  -  | -94 -  |     |
| --- | --------------------------- | ------- | ----- | ------ | --- |
|     | QPSK rate 1/2, 12Mbps OFDM  | -  -93  | -  -  | -93 -  |     |
|     | QPSK rate 3/4, 18Mbps OFDM  | -  -90  | -  -  | -90 -  |     |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
35

  RTL872xD Datasheet
|     | 16QAM rate 1/2, 24Mbps OFDM  | -  -87  | -  -  | -87 -  |     |
| --- | ---------------------------- | ------- | ----- | ------ | --- |
|     | 16QAM rate 3/4, 36Mbps OFDM  | -  -84  | -  -  | -84 -  |     |
|     | 64QAM rate 1/2, 48Mbps OFDM  | -  -79  | -  -  | -79 -  |     |
|     | 64QAM rate 3/4, 54Mbps OFDM  | -  -77  | -  -  | -77 -  |     |
Rx Sensitivity  MCS 0, BPSK rate 1/2  -  -95  -  -  -95 -  dBm
| BW = 20MHz   | MCS 1, QPSK rate 1/2  | -  -92  | -  -  | -92 -  |     |
| ------------ | --------------------- | ------- | ----- | ------ | --- |
| Mixed Mode   | MCS 2, QPSK rate 3/4  | -  -89  | -  -  | -89 -  |     |
800ns Guard Interval
|     | MCS 3, 16QAM rate 1/2  | -  -86  | -  -  | -86 -  |     |
| --- | ---------------------- | ------- | ----- | ------ | --- |
Non-STBC
|     | MCS 4, 16QAM rate 3/4  | -  -83  | -  -  | -83 -  |     |
| --- | ---------------------- | ------- | ----- | ------ | --- |
|     | MCS 5, 64QAM rate 2/3  | -  -78  | -  -  | -78 -  |     |
|     | MCS 6, 64QAM rate 3/4  | -  -77  | -  -  | -77 -  |     |
|     | MCS 7, 64QAM rate 5/6  | -  -75  | -  -  | -75 -  |     |
Rx Sensitivity  MCS 0, BPSK rate 1/2  -  -93  -  -  -93 -  dBm
BW = 40MHz
|              | MCS 1, QPSK rate 1/2  | -  -89  | -  -  | -89 -  |     |
| ------------ | --------------------- | ------- | ----- | ------ | --- |
| Mixed Mode   | MCS 2, QPSK rate 3/4  | -  -86  | -  -  | -86 -  |     |
800ns Guard Interval  MCS 3, 16QAM rate 1/2  -  -83  -  -  -83 -
Non-STBC
|                    | MCS 4, 16QAM rate 3/4        | -  -80  | -  -  | -80 -  |      |
| ------------------ | ---------------------------- | ------- | ----- | ------ | ---- |
|                    | MCS 5, 64QAM rate 2/3        | -  -75  | -  -  | -75 -  |      |
|                    | MCS 6, 64QAM rate 3/4        | -  -74  | -  -  | -74 -  |      |
|                    | MCS 7, 64QAM rate 5/6        | -  -72  | -  -  | -72 -  |      |
| Maximum Receive    | 6Mbps OFDM                   | -  0    | -  -  | 0  -   | dBm  |
| Level              | 54Mbps OFDM                  | -  0    | -  -  | 0  -   |      |
|                    | MCS 0                        | -  0    | -  -  | 0  -   |      |
|                    | MCS 7                        | -  0    | -  -  | 0  -   |      |
| Receive Adjacent   | 1Mbps CCK                    | -  43   | -  -  | 43  -  | dBm  |
| Channel Rejection  | 11Mbps CCK                   | -  40   | -  -  | 41  -  |      |
|                    | BPSK rate 1/2, 6Mbps OFDM    | -  40   | -  -  | 40  -  |      |
|                    | 64QAM rate 3/4, 54Mbps OFDM  | -  22   | -  -  | 22  -  |      |
|                    | HT20, MCS 0, BPSK rate 1/2   | -  39   | -  -  | 39  -  |      |
|                    | HT20, MCS 7, 64QAM rate 5/6  | -  20   | -  -  | 20  -  |      |
|                    | HT40, MCS 0, BPSK rate 1/2   | -  30   | -  -  | 29  -  |      |
|                    | HT40, MCS 7, 64QAM rate 5/6  | -  11   | -  -  | 10  -  |      |
6.2.2  Wi-Fi 2.4GHz Band RF Transmitter Specifications
Parameter  Description  Performance (1.8V)  Performance (3.3V)  Unit
|     |     | Min.  Typ.  | Max.  Min.  | Typ.  Max.  |     |
| --- | --- | ----------- | ----------- | ----------- | --- |
Frequency Range  Center channel frequency  2412  -  2484  2412  -  2484  MHz
| Output power with      | 1Mbps CCK   | -  14  | 15  -  | 20  21  | dBm  |
| ---------------------- | ----------- | ------ | ------ | ------- | ---- |
| spectral mask and EVM  | 11Mbps CCK  | -  12  | 15  -  | 18  21  |      |
compliance1  BPSK rate 1/2, 6Mbps OFDM  -  14  15  -  20  21
|         | 64QAM rate 3/4, 54Mbps OFDM  | -  11  | 12  -  | 17  18  |     |
| ------- | ---------------------------- | ------ | ------ | ------- | --- |
|         | HT20-MCS 0, BPSK rate 1/2    | -  13  | 14  -  | 19  20  |     |
|         | HT20-MCS 7, 64QAM rate 5/6   | -  10  | 11  -  | 16  17  |     |
|         | HT40-MCS 0, BPSK rate 1/2    | -  13  | 14  -  | 19  20  |     |
|         | HT40-MCS 7, 64QAM rate 5/6   | -  10  | 11  -  | 16  17  |     |
| Tx EVM  | BPSK rate 1/2, 6Mbps OFDM    | -  -   | -5 -   | -  -5   | dB  |
|         | 64QAM rate 3/4, 54Mbps OFDM  | -  -   | -25 -  | -  -25  |     |
|         | HT20-MCS 0, BPSK rate 1/2    | -  -   | -5 -   | -  -5   |     |
|         | HT20-MCS 7, 64QAM rate 5/6   | -  -   | -28 -  | -  -28  |     |
|         | HT40-MCS 0, BPSK rate 1/2    | -  -   | -5 -   | -  -5   |     |
|         | HT40-MCS 7, 64QAM rate 5/6   | -  -   | -28 -  | -  -28  |     |
Output power variation2  After do power trim at FT  -1.5 -  1.5  -1.5 -  1.5  dBm
| Carrier suppression  |     | -  -  | -30 -  | -  -30 | dBm  |
| -------------------- | --- | ----- | ------ | ------ | ---- |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
36

Radio Characteristics
Harmonic output power  2nd harmonic  -  -36  -34 -  -23  -21.9 dBm
| (IC port)  | 3rd harmonic  | -   | -26  -24 -  | -15  -14 |     |
| ---------- | ------------- | --- | ----------- | -------- | --- |
1. Power level is tested after DPK on.
2. VDD18 voltage is within ±5% of typical value.
6.2.3  Wi-Fi 5GHz Band RF Receiver Specifications
Parameter  Description  Performance (1.8V)  Performance (3.3V)  Unit
|     |     | Min.  Typ.  | Max.  Min.  | Typ.  Max.  |     |
| --- | --- | ----------- | ----------- | ----------- | --- |
Frequency Range  Center channel frequency  5180  -  5825  5180  -  5825  MHz
Rx Sensitivity  BPSK rate 1/2, 6Mbps OFDM  -  -94  -  -  -93 -  dBm
|     | BPSK rate 3/4, 9Mbps OFDM    | -  -93  | -  -  | -93 -  |     |
| --- | ---------------------------- | ------- | ----- | ------ | --- |
|     | QPSK rate 1/2, 12Mbps OFDM   | -  -92  | -  -  | -92 -  |     |
|     | QPSK rate 3/4, 18Mbps OFDM   | -  -89  | -  -  | -89 -  |     |
|     | 16QAM rate 1/2, 24Mbps OFDM  | -  -86  | -  -  | -86 -  |     |
|     | 16QAM rate 3/4, 36Mbps OFDM  | -  -83  | -  -  | -83 -  |     |
|     | 64QAM rate 1/2, 48Mbps OFDM  | -  -78  | -  -  | -78 -  |     |
|     | 64QAM rate 3/4, 54Mbps OFDM  | -  -76  | -  -  | -76 -  |     |
Rx Sensitivity  MCS 0, BPSK rate 1/2  -  -94  -  -  -93 -  dBm
| BW = 20MHz HT  | MCS 1, QPSK rate 1/2  | -  -91  | -  -  | -91 -  |     |
| -------------- | --------------------- | ------- | ----- | ------ | --- |
| Mixed Mode     | MCS 2, QPSK rate 3/4  | -  -88  | -  -  | -88 -  |     |
800ns Guard Interval  MCS 3, 16QAM rate 1/2  -  -85  -  -  -85 -
Non-STBC
|     | MCS 4, 16QAM rate 3/4  | -  -82  | -  -  | -82 -  |     |
| --- | ---------------------- | ------- | ----- | ------ | --- |
|     | MCS 5, 64QAM rate 2/3  | -  -77  | -  -  | -77 -  |     |
|     | MCS 6, 64QAM rate 3/4  | -  -76  | -  -  | -75 -  |     |
|     | MCS 7, 64QAM rate 5/6  | -  -74  | -  -  | -74 -  |     |
Rx Sensitivity  MCS 0, BPSK rate 1/2  -  -91  -  -  -91 -  dBm
| BW = 40MHz HT  | MCS 1, QPSK rate 1/2  | -  -88  | -  -  | -88 -  |     |
| -------------- | --------------------- | ------- | ----- | ------ | --- |
| Mixed Mode     | MCS 2, QPSK rate 3/4  | -  -85  | -  -  | -85 -  |     |
800ns Guard Interval  MCS 3, 16QAM rate 1/2  -  -82  -  -  -82 -
Non-STBC
|                  | MCS 4, 16QAM rate 3/4  | -  -79  | -  -  | -79 -  |      |
| ---------------- | ---------------------- | ------- | ----- | ------ | ---- |
|                  | MCS 5, 64QAM rate 2/3  | -  -74  | -  -  | -74 -  |      |
|                  | MCS 6, 64QAM rate 3/4  | -  -73  | -  -  | -73 -  |      |
|                  | MCS 7, 64QAM rate 5/6  | -  -71  | -  -  | -71 -  |      |
| Maximum Receive  | 6Mbps OFDM             | -  0    | -  -  | 0  -   | dBm  |
| Level            | 54Mbps OFDM            | -  0    | -  -  | 0  -   |      |
|                  | MCS 0                  | -  0    | -  -  | 0  -   |      |
|                  | MCS 7                  | -  0    | -  -  | 0  -   |      |
Receive Adjacent  BPSK rate 1/2, 6Mbps OFDM  -  21  -  -  21  -  dBm
Channel Rejection  64QAM rate 3/4, 54Mbps OFDM  -  12  -  -  11  -
|     | HT20, MCS 0, BPSK rate 1/2   | -  21  | -  -  | 19  -  |     |
| --- | ---------------------------- | ------ | ----- | ------ | --- |
|     | HT20, MCS 7, 64QAM rate 5/6  | -  7   | -  -  | 7  -   |     |
|     | HT40, MCS 0, BPSK rate 1/2   | -  30  | -  -  | 30  -  |     |
|     | HT40, MCS 7, 64QAM rate 5/6  | -  12  | -  -  | 13  -  |     |
6.2.4  Wi-Fi 5GHz Band RF Transmitter Specifications
Parameter  Description  Performance (1.8V)  Performance (3.3V)  Unit
|     |     | Min.  | Typ.  Max.  Min.  | Typ.  Max.  |     |
| --- | --- | ----- | ----------------- | ----------- | --- |
Frequency Range  Center channel frequency  5180  -  5825  5180  -  5825  MHz
Output power with  BPSK rate 1/2, 6Mbps OFDM  -  12  13  -  18  19  dBm
spectral mask and EVM  64QAM rate 3/4, 54Mbps OFDM  -  9  10  -  14  15
compliance1  HT20-MCS 0, BPSK rate 1/2  -  11  12  -  17  18
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
37

  RTL872xD Datasheet
|         | HT20-MCS 7, 64QAM rate 5/6   |     | -  8   | 9  -   | 13  14  |     |
| ------- | ---------------------------- | --- | ------ | ------ | ------- | --- |
|         | HT40-MCS 0, BPSK rate 1/2    |     | -  11  | 12  -  | 17  18  |     |
|         | HT40-MCS 7, 64QAM rate 5/6   |     | -  8   | 9  -   | 13  14  |     |
| Tx EVM  | BPSK rate 1/2, 6Mbps OFDM    |     | -  -   | -5 -   | -  -5   | dB  |
|         | 64QAM rate 3/4, 54Mbps OFDM  |     | -  -   | -25 -  | -  -25  |     |
|         | HT20-MCS 0, BPSK rate 1/2    |     | -  -   | -5 -   | -  -5   |     |
|         | HT20-MCS 7, 64QAM rate 5/6   |     | -  -   | -28 -  | -  -28  |     |
|         | HT40-MCS 0, BPSK rate 1/2    |     | -  -   | -5 -   | -  -5   |     |
|         | HT40-MCS 7, 64QAM rate 5/6   |     | -  -   | -28 -  | -  -28  |     |
Output power variation2  After doing power trim at FT  -1.5 -  1.5  -1.5 -  1.5  dBm
| Carrier suppression  |     |     | -  -  | -30 -  | -  -30 | dBm  |
| -------------------- | --- | --- | ----- | ------ | ------ | ---- |
1. Power level is tested after DPK on.
2. VDD18 voltage is within ±5% of typical value.
6.3  BT Radio Characteristics
The RF specifications of BT are described in the tables below. Both the transmitter specifications and the receiver specifications basically follow
the Bluetooth SIG specifications.
6.3.1  BT RF Transmitter Specifications
| Parameter        |     | Condition  |     | Min.  Typ.  | Max.  | Unit  |
| ---------------- | --- | ---------- | --- | ----------- | ----- | ----- |
| Frequency Range  |     |            |     | 2402        | 2480  | MHz   |
| Tx Output Power  |     | LE1M       |     | -10 4.5     |       | dBm   |
LE2M
| Modulation Characteristics (LE1M)  |     | ∆F1 Avg.                              |     | 225    | 275    | kHz  |
| ---------------------------------- | --- | ------------------------------------- | --- | ------ | ------ | ---- |
|                                    |     | ∆F2 Max.                              |     | 185    |        | kHz  |
|                                    |     | Modulation Index (∆F2 Avg./∆F1 Avg.)  |     | 0.8    |        |      |
| Modulation Characteristics (LE2M)  |     | ∆F1 Avg.                              |     | 450    | 550    | kHz  |
|                                    |     | ∆F2 Max.                              |     | 370    |        | kHz  |
|                                    |     | Modulation Index (∆F2 Avg./∆F1 Avg.)  |     | 0.8    |        |      |
| Modulation Characteristics         |     | ∆F1 Avg.                              |     | 247.5  | 252.5  | kHz  |
| Stable Modulation (LE1M)           |     | ∆F2 Max.                              |     | 185    |        | kHz  |
|                                    |     | Modulation Index (∆F2 Avg./∆F1 Avg.)  |     | 0.8    |        |      |
| Modulation Characteristics         |     | ∆F1 Avg.                              |     | 495    | 505    | kHz  |
| Stable Modulation (LE2M)           |     | ∆F2 Max.                              |     | 370    |        | kHz  |
|                                    |     | Modulation Index (∆F2 Avg./∆F1 Avg.)  |     | 0.8    |        |      |
6.3.2
BT RF Receiver Specifications
| Parameter        | Condition  | Min.   | Typ.  | Max.  | Unit  |     |
| ---------------- | ---------- | ------ | ----- | ----- | ----- | --- |
| Frequency Range  |            | 2402   | -     | 2480  | MHz   |     |
| Rx Sensitivity   | LE1M       | -100.9 | -     | -     | dBm   |     |
|                  | LE2M       | -96.8  | -     | -     |       |     |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
38

Electrical Characteristics
7  Electrical Characteristics
7.1  Temperature Limit Ratings
The temperature limit ratings are the parameter values or ranges which can cause permanent damage if exceeded.
Table 7-1 Temperature limit ratings
| Parameter            |     | Part Number        |     | Minimum  |     | Maximum  Unit  |
| -------------------- | --- | ------------------ | --- | -------- | --- | -------------- |
| Storage Temperature  |     | QFN48/QFN68/QFN88  |     | -55      |     | +125 C        |
Ambient Operating Temperature (T )  QFN48/QFN68/QFN88  -20 +85 C
a
|                         |      | QFN48 wide-range temperature  |     | -40 |     | +105     |
| ----------------------- | ---- | ----------------------------- | --- | --- | --- | -------- |
| Junction Temperature (T | j )  | QFN48/QFN68/QFN88             |     | -40 |     | +125 C  |
7.2  ESD Characteristicss
The Electro-Static discharge (ESD) characteristics of RTL872xD are listed in Table 7-2.
Table 7-2 ESD characteristics
| Reliability test        |     | Standards              | Test condition  |     | Result  |     |
| ----------------------- | --- | ---------------------- | --------------- | --- | ------- | --- |
| Human Body Model (HBM)  |     | JEDEC EIA/JESD22-A114  | ±2000V          |     | Pass    |     |
Charge Device Model (CDM)  JEDEC EIA/JESD22-C101  ±500V  Pass
7.3  DC Characteristics
The direct current (DC) characteristics of power supply and digital I/O pin are illustrated in the following sections.
7.3.1  Power Supply
The power supply DC characteristics of RTL872xD are shown in Table 7-3.
Table 7-3 Power supply DC characteristics
| Symbol  |     | Parameter  |     | Minimum  | Typical  | Maximum  Unit  |
| ------- | --- | ---------- | --- | -------- | -------- | -------------- |
VA1833_XTAL, VR1833_SYN, VR1833_PAD_A,  3.3V Supply Voltage  3.0  3.3  3.6  V
VR1833_PA_A, VR1833_PA_G, VA1833_USB,
VA1833_PLL, VD1833_PMU,
|     |     | 1.85V Supply Voltage  |     | 1.76  | 1.85  | 2.035  V  |
| --- | --- | --------------------- | --- | ----- | ----- | --------- |
VD1833_SPS/VD1833_FLASH, AVCC_DRIV
VD1833_GPIO  Digital I/O Supply Voltage  1.76  1.8 ~ 3.3  3.6  V
VA11_AFE, VA11_SYN,  VA11_RF1, VA11_RF2,  1.1V Core Supply Voltage  1.05  1.1  1.21  V
VD11_CORE, VA11_PLL
| IDD331  |     | 3.3V Rating Current (with internal  |     | -   | -   | 450  mA  |
| ------- | --- | ----------------------------------- | --- | --- | --- | -------- |
regulator and integrated CMOS PA)
| IDD181  |     | 1.85V Rating Current (with internal  |     |     |     | 800  mA  |
| ------- | --- | ------------------------------------ | --- | --- | --- | -------- |
regulator and integrated CMOS PA)
IDD_IO  I/O Rating Current (including VDD_IO)  -  -  200  mA
| IDD_IO_1833  |     | 1.85V or 3.3V I/O Rating Current  |     | -   | -   | 50  mA  |
| ------------ | --- | --------------------------------- | --- | --- | --- | ------- |
1. IDD33 is for 3.3V supply voltage, and IDD18 is for 1.85V supply voltage, to supply pins “VA1833_XTAL, VR1833_SYN, VR1833_PAD_A,
VR1833_PA_A, VR1833_PA_G, VA1833_USB, VA1833_PLL, VD1833_PMU, VD1833_SPS/VD1833_FLASH, AVCC_DRIV”.
There are some restrictions both for the application of 1.85V supply voltage and 3.3V supply voltage.
 On 1.85V supply voltage module, make sure to use wide-range voltage Flash; otherwise, the Flash cannot be accessed.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
39

  RTL872xD Datasheet
 On 3.3V supply voltage module, make sure your circuit satisfy the following two points; otherwise, you should refer to HDK to add a Reset
IC.
 The power-on voltage waveform is consistent with Figure 7-1.
 The power-off voltage can drop to under 0.3V finally.
VD33
3.0V
1.5V
t
Δt < 15ms
Figure 7-1 Power-on waveform
7.3.2  Digital I/O Pin
The digital I/O pin DC characteristics of RTL872xD are shown in Table 7-4 and Table 7-5, which illustrate 3.3V and 1.8V respectively.
Table 7-4 Digital I/O pin DC characteristics (3.3V)
Symbol  Parameter  Conditions  Minimum  Typical  Maximum  Unit
| V IH   | Input-High Voltage  |     | LVTTL  | 2.0  | -   | -    | V   |
| ------ | ------------------- | --- | ------ | ---- | --- | ---- | --- |
| V      | Input-Low Voltage   |     | LVTTL  | -    | -   | 0.8  | V   |
IL
| V   | Output-High Voltage  |     | LVTTL  | 2.4  | -   | -   | V   |
| --- | -------------------- | --- | ------ | ---- | --- | --- | --- |
OH
| V OL   | Output-Low Voltage          |     | LVTTL  | -     | -     | 0.4   | V   |
| ------ | --------------------------- | --- | ------ | ----- | ----- | ----- | --- |
| I      | Schmitt-trigger High Level  |     |        | 1.78  | 1.87  | 1.97  | V   |
T+
| I   | Schmitt-trigger Low Level  |     |     | 1.36  | 1.45  | 1.56  | V   |
| --- | -------------------------- | --- | --- | ----- | ----- | ----- | --- |
T-
I IL   Input-Leakage Current  V IN  = 3.3V or 0  -10 1  10  A
Table 7-5 Digital I/O pin DC characteristics (1.8V)
Symbol  Parameter  Conditions  Minimum  Typical  Maximum  Unit
| V   | Input-High Voltage  |     | CMOS  | 0.65 * Vcc  | -   | -   | V   |
| --- | ------------------- | --- | ----- | ----------- | --- | --- | --- |
IH
| V IL   | Input-Low Voltage    |     | CMOS  | -         | -   | 0.35 * Vcc  | V   |
| ------ | -------------------- | --- | ----- | --------- | --- | ----------- | --- |
| V      | Output-High Voltage  |     | CMOS  | Vcc-0.45  | -   | -           | V   |
OH
| V   | Output-Low Voltage  |     | CMOS  | -   | -   | 0.45  | V   |
| --- | ------------------- | --- | ----- | --- | --- | ----- | --- |
OL
| I T+   | Schmitt-trigger High Level  |     | -   | 1.02  | 1.09  | 1.14  | V   |
| ------ | --------------------------- | --- | --- | ----- | ----- | ----- | --- |
| I      | Schmitt-trigger Low Level   |     | -   | 0.67  | 0.73  | 0.87  | V   |
T-
| I   | Input-Leakage Current  |     | V = 1.8V or 0  | -10 | 1  | 10  | A  |
| --- | ---------------------- | --- | -------------- | --- | --- | --- | --- |
| IL  |                        |     | IN             |     |     |     |     |
7.4  ADC Characteristics
The ADC characteristics of RTL872xD are shown in Table 7-6.
Table 7-6 SAR ADC characteristics
| Parameter        |     | Conditions                     |     | Min.  | Typ.  | Max.  | Unit  |
| ---------------- | --- | ------------------------------ | --- | ----- | ----- | ----- | ----- |
| Temperature      |     |                                |     | ─40   | 25    | 125   | C    |
| Resolution       |     | Bypass mode                    |     |       | 12    |       | Bits  |
|                  |     | Resistor divider mode (1/3.3)  |     |       | 12    |       | Bits  |
| Clock Source     |     | From digital                   |     |       |       | 1000  | kHz   |
| DC Offset Error  |     | Cover VBAT=1.62~3.63V          |     |       | 2     |       | LSB   |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
40

Electrical Characteristics
Temperature=-40~125℃
| Gain Error  | Cover VBAT=1.62~3.63V  |     |     | 2   |     | LSB  |
| ----------- | ---------------------- | --- | --- | --- | --- | ---- |
Temperature=-40~125℃
| DNL  |     |     |     | ─1  |     | LSB  |
| ---- | --- | --- | --- | --- | --- | ---- |
| INL  |     |     |     | ±3  |     | LSB  |
Input Voltage Range  External channel (ch0 ~ ch7)  0  3.63  V
|                       | External channel 0 (VBAT)      |     | 0   |      | 5   | V    |
| --------------------- | ------------------------------ | --- | --- | ---- | --- | ---- |
| Input Impedance       | Bypass mode                    |     |     | 4    |     | MΩ   |
|                       | Resistor divider mode (1/3.3)  |     |     | 500  |     | kΩ   |
| Sampling Capacitance  | Bypass mode                    |     |     | 1.9  |     | pF   |
|                       | Resistor divider mode (1/3.3)  |     |     | 1.9  |     | pF   |
| Conversion Time       |                                |     |     | 1    |     | us   |
| Sampling Rate1        |                                |     |     |      | 1   | MHz  |
| Power Consumption     | 1 Msample/s                    |     |     | 190  |     | uA   |
1. Sampling rate depends on the output impedance of sensor and resistor divider, 8.4 * R OUT * 1.9 pF – 0.5 us < 1/f
S.
7.5  XTAL Oscillator
The RTL872xD has a built-in 40MHz crystal oscillation circuit to provide a stable, controllable system clock. With the built-in capacitor, the clock
offset can be fine-tuned in the mass production process. The maximum internal cap is 32pF typically, and we suggest following the Realtek
crystal design specification and QVL.
The external capacitor, C1 and C2, could be replaced by an internal capacitor, to reduce the BOM cost, minimize the PCB dimensions, and
provide flexibility for clock fine-tuning.
RTL872xD
|     | C = 0 – 32pF |     |     | C = 0 – 32pF |     |     |
| --- | ------------ | --- | --- | ------------ | --- | --- |
|     | int          |     |     | int          |     |     |
|     |              | C   |     | C            |     |     |
|     |              | 1   |     | 2            |     |     |
Figure 7-2 40MHz crystal specification suggestion
The specification of XTAL oscillation is listed in Table 7-7.
Table 7-7 Specification of crystal oscillator
| Parameter                              |     | Min.         | Typ.       | Max.  |     | Unit  |
| -------------------------------------- | --- | ------------ | ---------- | ----- | --- | ----- |
| Frequency                              |     | 40           |            |       |     | MHz   |
| Mode of oscillation                    |     | Fundamental  |            |       |     |       |
| Frequency tolerance (25°C ± 3°C)       |     | −10          |            | +10   |     | ppm   |
| Equivalent Series Resistance (RR)      |     |              | < 25       | 40    |     | Ω     |
| Temperature stability (−30°C ~ +85°C)  |     | −10          |            | +10   |     | ppm   |
| Load capacitance (CL)                  |     |              | 12         |       |     | pF    |
| Drive level (DL)                       |     |              | 150 ~ 200  | 300   |     | uW    |
| Shunt capacitance (C0)                 |     |              | < 0.88     | 2.0   |     | pF    |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
41

  RTL872xD Datasheet
7.6  Power Consumption
The specification of power consumption is listed in Table 7-8.
Table 7-8 Specification of power consumption
| Parameter  | Condition          | Typical  |     | Maximum  | Unit  |     |
| ---------- | ------------------ | -------- | --- | -------- | ----- | --- |
| Current    | In sleep mode      | 38       |     | 105      | uA    |     |
|            | In deepsleep mode  | 10       |     | 25       |       |     |
7.7  Power State and Power Sequence
The timing specification of the power sequence is listed in Table 7-9.
Table 7-9 Timing specification of power sequence
| Symbol  Parameter         |     |     | Minimum  | Typical  | Maximum  | Unit  |
| ------------------------- | --- | --- | -------- | -------- | -------- | ----- |
| T PRDY   VDDx ready time  |     |     | 0.6      | 0.6      | 1        | ms    |
T   Internal ring clock stable time after VDD1833 ready  1  -  -  ms
CLK
| T   LP core power ready time  |     |     | 1.5  | 1.5  | -   | ms  |
| ----------------------------- | --- | --- | ---- | ---- | --- | --- |
core
| T boot   HS MCU boot time  |     |     | 200  | 200  | -   | ms  |
| -------------------------- | --- | --- | ---- | ---- | --- | --- |
V   Shutdown occurs after CHIP_EN lower than this voltage  0  0  0.5 * VDDx  V
RST
T   The required time that CHIP_EN lower than V   1  1  -  ms
| RST |     | RST |     |     |     |     |
| --- | --- | --- | --- | --- | --- | --- |
VDDx is the supply power of VD1833.
7.7.1  Power on or Resuming from Deepsleep Sequence
The timing sequence of power on or resuming from deepsleep is given in Figure 7-3.
T
PRDY
VDDx > 1.8V
VDD_IO > 1.8V
CHIP_EN
T core
LP core voltage
T
CLK
RCO
HS core power
1.1V
T
|     | CPU boot time |     | boot |     |     |     |
| --- | ------------- | --- | ---- | --- | --- | --- |
Figure 7-3 Timing sequence of power on or resuming from deepsleep
| 7.7.2  Shutdown Sequence  |     |     |     |     |     |     |
| ------------------------- | --- | --- | --- | --- | --- | --- |
The timing sequence of shutdown is illustrated in Figure 7-4.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
42

Electrical Characteristics
VDDx
VDD_IO
V
RST
CHIP_EN
T
T CLK
RST
RCO
T
LP core power core
1.1V
T
boot
CPU available
Figure 7-4 Timing sequence of shutdown
7.8 I/O Port Characteristics
7.8.1 GPIO Sink and Source Current
Test condition: I/O output mode.
 Sink current: I/O output low
 Source current: I/O output high
current sensor
I/O
RTL872xD A
sink/source current
+
DC
power supply
GND
Figure 7-5 GPIO sink and source current test circuit
Table 7-10 GPIO sink and source current with 3.3V and 1.8V power supply
Pad type Pin name Driving strength (3.3V) Driving strength (1.8V)
PA PB Source current (mA) Sink current (mA) Source current (mA) Sink current (mA)
Normal1 PA[7] PB[8] 5.9/10.96/15.1/18.8 5.1/9.6/13.3/16.7 2.0/3.8/5.4/6.9 1.9/3.5/5.0/6.4
PA[8] PB[9]
PA[9] PB[10]
PA[10] PB[11]
PA[11] PB[12]
PA[22] PB[13]
PA[23] PB[14]
PA[24] PB[15]
PA[27] PB[16]
PA[28] PB[17]
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
43

RTL872xD Datasheet
SDIO1 PB[18] 4.2/7.9/11.2/14.2/19.2/23.3 3.6/6.9/9.9/12.6/17.5/22.1 1.3/2.6/3.7/4.8/6.8/8.5 1.3/2.4/3.5/4.6/6.5/8.3
PB[19]
PB[20]
PB[21]
PB[22]
PB[23]
PB[24]
PB[25]
Key-Scan1 PA[12] 11.0/18.9 9.7/16.8 3.9/7.1 3.6/6.7
PA[13]
PA[14]
PA[15]
PA[16]
PA[17]
PA[18]
PA[19]
PA[20]
PA[21]
PA[25]
PA[26]
Audio1 PA[0] PB[28] 6.1/13.9 6.1/15.3 2.1/5.4 2.2/6.2
PA[1] PB[29]
PA[2] PB[30]
PA[3] PB[31]
PA[4]
PA[5]
PA[6]
Cap-Touch2 PB[4] 7.8/14.5 7.2/13.6 3.0/5.8 2.6/5.0
PB[5]
PB[6]
PB[7]
I2C1 PA[29] PB[0] 10.9/18.6 9.4/16.3 3.9/7.2 3.5/6.5
PA[30] PB[26]
PA[31] PB[27]
PB[1]
PB[2]
PB[3]
1. For this type of GPIO pins with 3.3V power supply, external power supplies are set in 0.33V and 2.97V for testing sink current and source
current respectively. For this type of GPIO pins with 1.8V power supply, external power supplies are set in 0.18V and 1.62V for testing
sink current and source current respectively.
2. For this type of GPIO pins with 3.3V power supply, external power supplies are set in 0.4V and 2.4V for testing sink current and source
current respectively. For this type of GPIO pins with 1.8V power supply, external power supplies are set in 0.22V and 1.31V for testing
sink current and source current respectively.
7.8.2 GPIO Pull Low Restriction
When one of the GPIOs listed below needs to be pulled low by a resistor on PCB, the pull low resistor value must > 1K Ohm.
Port Name Schematic Resistor Value
PA[7] ~ PA[31] R > 1kΩ
GPIO
PB[0] ~ PB[3]
PB[8] ~ PB[17]
R R > 1kΩ
PB[26]
PB[27]
GND
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
44

Package Specifications
8  Package Specifications
8.1  QFN48
The QFN48 package specification of RTL872xD is shown in Figure 8-1 and Table 8-1.
Figure 8-1 QFN48 package specification
Table 8-1 QFN48 package specification
| Symbol  | Dimension (mm)  |          |          | Dimension (inch)  |          |          |
| ------- | --------------- | -------- | -------- | ----------------- | -------- | -------- |
|         | Minimum         | Nominal  | Maximum  | Minimum           | Nominal  | Maximum  |
| A       | 0.8             | 0.85     | 0.9      | 0.031             | 0.033    | 0.035    |
| A 1     | 0.00            | 0.02     | 0.05     | 0.000             | 0.001    | 0.002    |
| A       | 0.20REF         |          |          | 0.008REF          |          |          |
3
| b      | 0.15     | 0.20  | 0.25   | 0.006     | 0.008  | 0.010  |
| ------ | -------- | ----- | ------ | --------- | ------ | ------ |
| D      | 6.00BSC  |       |        | 0.236BSC  |        |        |
| E      | 6.00BSC  |       |        | 0.236BSC  |        |        |
| D2/E2  | 4.50     | 4.60  | 4.70   | 0.177     | 0.181  | 0.185  |
| e      | 0.40BSC  |       |        | 0.016BSC  |        |        |
| L      | 0.30     | 0.40  | 0.50   | 0.012     | 0.016  | 0.020  |
| K      | 0.20     | -     | -      | 0.008     | -      | -      |
| R      | 0.075    | -     | 0.125  | 0.003     | -      | 0.005  |
| aaa    | 0.10     |       |        | 0.004     |        |        |
| bbb    | 0.07     |       |        | 0.003     |        |        |
| ccc    | 0.10     |       |        | 0.004     |        |        |
| ddd    | 0.05     |       |        | 0.002     |        |        |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
45

  RTL872xD Datasheet
| eee  | 0.08  |     |     | 0.003  |     |     |
| ---- | ----- | --- | --- | ------ | --- | --- |
| fff  | 0.10  |     |     | 0.004  |     |     |
 Controlling dimension: millimeter (mm).
 Reference document: JEDEC MO-220.
8.2  QFN68
The QFN68 package specification of RTL872xD is shown in Figure 8-2 and Table 8-2.
Figure 8-2 QFN68 package specification
Table 8-2 QFN68 package specification
| Symbol  | Dimension (mm)  |          |          | Dimension (inch)  |          |          |
| ------- | --------------- | -------- | -------- | ----------------- | -------- | -------- |
|         | Minimum         | Nominal  | Maximum  | Minimum           | Nominal  | Maximum  |
| A       | 0.80            | 0.85     | 0.90     | 0.031             | 0.033    | 0.035    |
| A       | 0.00            | 0.02     | 0.05     | 0.000             | 0.001    | 0.002    |
1
| A 3   | 0.2 REF  |       |       | 0.008 REF  |        |        |
| ----- | -------- | ----- | ----- | ---------- | ------ | ------ |
| b     | 0.15     | 0.20  | 0.25  | 0.006      | 0.008  | 0.010  |
| D     | 7.90     | 8.00  | 8.10  | 0.311      | 0.315  | 0.319  |
| E     | 7.90     | 8.00  | 8.10  | 0.311      | 0.315  | 0.319  |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
46

Package Specifications
| D   | 5.80  | 5.90  | 6.00  | 0.205  | 0.209  | 0.213  |
| --- | ----- | ----- | ----- | ------ | ------ | ------ |
2
| E   | 5.35  | 5.45  | 5.55  | 0.217  | 0.220  | 0.224  |
| --- | ----- | ----- | ----- | ------ | ------ | ------ |
2
| e    | 0.40 BSC  |       |        | 0.016 BSC  |        |        |
| ---- | --------- | ----- | ------ | ---------- | ------ | ------ |
| L    | 0.30      | 0.40  | 0.50   | 0.012      | 0.016  | 0.020  |
| K    | 0.20      | -     | -      | 0.008      | -      | -      |
| R    | 0.075     | -     | 0.125  | 0.003      | -      | 0.005  |
| aaa  | 0.10      |       |        | 0.004      |        |        |
| bbb  | 0.07      |       |        | 0.003      |        |        |
| ccc  | 0.10      |       |        | 0.004      |        |        |
| ddd  | 0.05      |       |        | 0.002      |        |        |
| eee  | 0.08      |       |        | 0.003      |        |        |
| fff  | 0.10      |       |        | 0.004      |        |        |
 Controlling dimension: millimeter (mm).

Reference document: JEDEC MO-220.
8.3  QFN88
The QFN88 package specification of RTL872xD is shown in Figure 8-3 and Table 8-3.
Figure 8-3 QFN88 package specification
Table 8-3 QFN88 package specification
| Symbol  | Dimension (mm)  |          |          | Dimension (inch)  |          |          |
| ------- | --------------- | -------- | -------- | ----------------- | -------- | -------- |
|         | Minimum         | Nominal  | Maximum  | Minimum           | Nominal  | Maximum  |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
47

  RTL872xD Datasheet
| A   | 0.80  | 0.85  | 0.90  | 0.031  | 0.033  | 0.035  |
| --- | ----- | ----- | ----- | ------ | ------ | ------ |
| A   | 0.00  | 0.02  | 0.05  | 0.000  | 0.001  | 0.002  |
1
| A 3   | 0.20 REF  |        |        | 0.008 REF  |        |        |
| ----- | --------- | ------ | ------ | ---------- | ------ | ------ |
| b     | 0.15      | 0.20   | 0.25   | 0.006      | 0.008  | 0.010  |
| D     | 9.90      | 10.00  | 10.10  | 0.390      | 0.394  | 0.398  |
| E     | 9.90      | 10.00  | 10.10  | 0.390      | 0.394  | 0.398  |
| D     | 6.10      | 6.20   | 6.30   | 0.240      | 0.244  | 0.248  |
2
| E 2   | 5.83      | 5.93  | 6.03   | 0.230      | 0.233  | 0.237  |
| ----- | --------- | ----- | ------ | ---------- | ------ | ------ |
| e     | 0.40 BSC  |       |        | 0.016 BSC  |        |        |
| L     | 0.30      | 0.40  | 0.50   | 0.012      | 0.016  | 0.020  |
| K     | 0.20      | -     | -      | 0.008      | -      | -      |
| R     | 0.075     | -     | 0.125  | 0.003      | -      | 0.005  |
| aaa   | 0.10      |       |        | 0.004      |        |        |
| bbb   | 0.07      |       |        | 0.003      |        |        |
| ccc   | 0.10      |       |        | 0.004      |        |        |
| ddd   | 0.05      |       |        | 0.002      |        |        |
| eee   | 0.08      |       |        | 0.003      |        |        |
| fff   | 0.10      |       |        | 0.004      |        |        |
 Controlling dimension: millimeter (mm).
 Reference document: JEDEC MO-220.
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
48

List of Tables
List of Tables
Table 1-1 System and memory features .............................................................................................................................................. 9
Table 1-2 Wireless features ............................................................................................................................................................... 10
Table 1-3 Security features ................................................................................................................................................................ 10
Table 1-4 Communication interface features .................................................................................................................................... 10
Table 1-5 Audio features .................................................................................................................................................................... 12
Table 1-6 Timer features .................................................................................................................................................................... 13
Table 1-7 Human- machine interaction features ............................................................................................................................... 13
Table 1-8 Analog features .................................................................................................................................................................. 14
Table 1-9 Peripherals under different packages ................................................................................................................................ 15
Table 1-10 Memory under different packages .................................................................................................................................. 16
Table 2-1 Package types..................................................................................................................................................................... 17
Table 2-2 Low power pins .................................................................................................................................................................. 23
Table 2-3 Pin default configuration ................................................................................................................................................... 23
Table 2-4 Trap pins............................................................................................................................................................................. 23
Table 3-1 Pin type description ........................................................................................................................................................... 25
Table 3-2 Power on trap pins ............................................................................................................................................................. 25
Table 3-3 RF pins ................................................................................................................................................................................ 25
Table 3-4 Chip enable pin .................................................................................................................................................................. 25
Table 3-5 Power pins ......................................................................................................................................................................... 26
Table 3-6 XTAL pins ............................................................................................................................................................................ 26
Table 3-7 ADC and Cap-Touch pins .................................................................................................................................................... 26
Table 3-8 USB pins ............................................................................................................................................................................. 27
Table 3-9 Audio codec pins ................................................................................................................................................................ 27
Table 4-1 Address space main blocks ................................................................................................................................................ 28
Table 4-2 KM4 register boundary addresses ..................................................................................................................................... 28
Table 4-3 KM0 register boundary addresses ..................................................................................................................................... 30
Table 5-1 NVIC table .......................................................................................................................................................................... 33
Table 7-1 Temperature limit ratings .................................................................................................................................................. 39
Table 7-2 ESD characteristics ............................................................................................................................................................. 39
Table 7-3 Power supply DC characteristics ........................................................................................................................................ 39
Table 7-4 Digital I/O pin DC characteristics (3.3V) ............................................................................................................................. 40
Table 7-5 Digital I/O pin DC characteristics (1.8V) ............................................................................................................................. 40
Table 7-6 SAR ADC characteristics ..................................................................................................................................................... 40
Table 7-7 Specification of crystal oscillator ....................................................................................................................................... 41
Table 7-8 Specification of power consumption ................................................................................................................................. 42
Table 7-9 Timing specification of power sequence ........................................................................................................................... 42
Table 7-10 GPIO sink and source current with 3.3V and 1.8V power supply .................................................................................... 43
Table 8-1 QFN48 package specification ............................................................................................................................................. 45
Table 8-2 QFN68 package specification ............................................................................................................................................. 46
Table 8-3 QFN88 package specification ............................................................................................................................................. 47
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
49

RTL872xD Datasheet
List of Figures
Figure 1-1 System architecture ............................................................................................................................................................ 8
Figure 2-1 QFN48 pin assignments for RTL8720DN/RTL8720DM ...................................................................................................... 19
Figure 2-2 QFN48 pin assignments for RTL8720DF ............................................................................................................................ 20
Figure 2-3 QFN68 pin assignment for RTL8721DM ............................................................................................................................ 21
Figure 2-4 QFN88 pin assignments for RTL8722DM .......................................................................................................................... 22
Figure 5-1 NVIC diagram .................................................................................................................................................................... 32
Figure 6-1 RF block diagram ............................................................................................................................................................... 35
Figure 7-1 Power-on waveform ......................................................................................................................................................... 40
Figure 7-2 40MHz crystal specification suggestion ............................................................................................................................ 41
Figure 7-3 Timing sequence of power on or resuming from deepsleep ............................................................................................ 42
Figure 7-4 Timing sequence of shutdown .......................................................................................................................................... 43
Figure 7-5 GPIO sink and source current test circuit ......................................................................................................................... 43
Figure 8-1 QFN48 package specification ............................................................................................................................................ 45
Figure 8-2 QFN68 package specification ............................................................................................................................................ 46
Figure 8-3 QFN88 package specification ............................................................................................................................................ 47
Datasheet All information provided in this document is subject to legal disclaimers. © REALTEK 2021. All rights reserved.
50

Revision History
Revision History
| Date        | Version  | Description                      |
| ----------- | -------- | -------------------------------- |
| 2021-01-20  | v2.9     | Update the ordering information  |
2020-11-4  v2.8   Add the section: GPIO Sink and Source Current
 Add the section: ESD Characteristics
 Add the information of RTL8720DF (QFN48)
| 2020-08-17  | v2.7  |  Add the section: ADC Characteristics |
| ----------- | ----- | -------------------------------------- |
 Update the ordering information
| 2020-06-24  | v2.6  |  Update the ordering information |
| ----------- | ----- | --------------------------------- |

Update the chapter: Memory Organization
 Update the RF receiver specification
 Update the figure: System architecture
 Add the section: XTAL Oscillator
 Add the section: Power Consumption

Update the feature of UART
 Add some restrictions for Power Supply
| 2020-02-18  | v2.5  | Update the feature of USB  |
| ----------- | ----- | -------------------------- |
2020-02-13  v2.4  Add some description of PA[15] & PA[16] for QFN48
| 2020-01-13  | v2.3  | Update the features            |
| ----------- | ----- | ------------------------------ |
| 2019-12-25  | v2.2  |  Update the temperature limit |
 Update the part number and ordering information
| 2019-12-09  | v2.1  |  Update the chapter: Memory Organization |
| ----------- | ----- | ----------------------------------------- |
 Update the section: General Description
 Update the specifications of Wi-Fi 2.4GHz/5GHz RF transmitter output power
| 2019-09-20  | v2.0  |  Add the section: BT Radio Characteristics |
| ----------- | ----- | ------------------------------------------- |
 Update the value of Wi-Fi RF transmitter
| 2019-07-19  | v1.9  | Update the section: Package Types          |
| ----------- | ----- | ------------------------------------------ |
| 2019-06-03  | v1.8  |  Update the description of Flash features |

Update the BLE Tx power to 8dbm
 Update the table: Power supply DC characteristics
| 2019-04-10  | v1.7  |  Add the chapter: Radio Characteristics |
| ----------- | ----- | ---------------------------------------- |
 Update the table: Timing specification of power sequence
 Update the table: Power supply DC characteristics
 Add the note of interrupt priority
 Add the section: GPIO Pull Low Restriction
Update the pin assignments for
 QFN88
| 2019-02-21  | v1.6  |  Add RFGND pin to RF pins table |
| ----------- | ----- | -------------------------------- |
 Re-organize the section: DC Characteristics
2019-01-25  v1.5   Modify the pin name (pin49) of QFN88 package
 Update the memory address
| 2018-12-26  | v1.4  | Update the package specification of QFN68  |
| ----------- | ----- | ------------------------------------------ |
2018-12-20  v1.3   Update the peripherals under different packages
 Revise the incorrect pin number
| 2018-12-17  | v1.2  | Update the description of power pins    |
| ----------- | ----- | --------------------------------------- |
| 2018-12-13  | v1.1  |  Re-organize the structure of document |
 Update package types of RTL872xD and QFN68/QFN88 pin assignments
 Update the operating temperature limit
| 2018-10-18  | v1.0  | Add the description about AUDIO_GND  |
| ----------- | ----- | ------------------------------------ |
2018-10-09  v0.9   Add description about SDIO device time consuming

Unify and normalize the technical items and expression
| 2018-07-04  | v0.8  | Add pin default configuration when boot  |
| ----------- | ----- | ---------------------------------------- |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
51

  RTL872xD Datasheet
| 2018-07-04  | v0.7  | Change audio & Secure       |
| ----------- | ----- | --------------------------- |
| 2018-07-04  | v0.6  | Change system architecture  |
| 2018-07-03  | v0.5  | Update system architecture  |
| 2018-06-29  | v0.4  | Update power architecture   |
| 2018-06-28  | v0.3  | Add Peripherals             |
| 2018-06-27  | v0.2  | Add all features            |
| 2018-06-24  | v0.1  | Draft version               |
Datasheet     All information provided in this document is subject to legal disclaimers.     © REALTEK 2021. All rights reserved.
52