# 2\. BIOS Overview

This section describes the specification of the HP palmtop ROM Basic Input/Output System (BIOS). The BIOS provides the lowest level of software support for applications running on HP palmtops. The HP palmtop BIOS is designed to be completely compatible with IBM's new version of the PC-XT. In addition to the PC-XT BIOS functions, the HP palmtop BIOS includes a small number of BIOS functions which are compatible with the IBM-AT and a number of additional non-standard functions to support features that are unique to the HP palmtop.

The BIOS is based upon an XT-level BIOS source code obtained from Phoenix Software Associates (PSA). The code HP purchased from PSA was written to be compatible with the IBM PC-XT of pre-April 1986 vintage, that is, before the introduction of a new version of the XT with the enhanced keyboard. To our knowledge, the PSA XT code received by HP was written by Phoenix without infringing on any of IBM's copyrights to the XT BIOS code. In the same spirit, HP has added and modified the code without copyright infringement.

## HP Palmtop Hardware set

The HP palmtop is a Personal Computer with built-in features that provide industry standard spread-sheet processing, and several Personal Information Management (PIM) applications including a full featured financial calculator. It features an 80186 processor and a hardware set that is moderately compatible with an IBM XT. The HP palmtop has 2 MB of ROM and 1 or 2 MB of RAM built-in, and 1 card port to support PCMCIA plug-in cards.

The LCD display has a resolution of 640x200 dots and supports both MDA and CGA compatible display operation except that "shades of grey" emulate "color." Several different-sized fonts are used to optimize the use of the display, yet retain CGA compatibility.

Several types of graphics modes, including 640x200 Black & White, 320x200 with 4-shade grey scale, and an HP 95LX-compatible 240x128 Black & White (centered in a 320x200 display), are supported. In addition, some non-standard hardware support is provided for rapid display of textual information in Graphics mode. Several different-sized fonts are used by the built-in applications for display of text in Graphics mode.

HP palmtops have improved hardware support (over the HP 95LX) for serial I/O with a 9-pin connection and 16450 compatible UART.

Some differences between HP palmtop and XT hardware are listed below:

* The HP palmtop CGA compatible video mode does not have color.
* The HP palmtop MDA compatible video mode is a 40x16 zoom mode into an 80x25 page.
* The HP palmtop supports "soft fonts" and has BitBlt hardware.
* The HP palmtop has no mechanical disk. Instead, there is a built-in RAM disk and a built-in ROM disk.
* The HP palmtop has a different keyboard layout from the IBM-XT.
* The HP palmtop keyboard management is different from the IBM-XT. Keyboard scans are implemented by an 8048 microcontroller in an XT, while the HP palmtop emulates this action in software.
* The HP palmtop supports plug-in ROMs.
* The HP palmtop supports plug-in RAMs. All memory in plug-in RAM and part of the built-in memory is used as RAM disk.
* The HP palmtop is switched on or off under software control. This is compared to an XT which is switched on or off by a hardware switch that controls power to the entire machine.
* The HP palmtop implements LCD contrast adjustment under software control. This is fundamentally different from XT brightness control, which is done with a potentiometer adjustment.
* The HP palmtop does not support a parallel printer. However, it does support a serial printer which uses XON/XOFF flow control.
* The HP palmtop supports only one serial port UART. However the serial channel can be directed either to the IR or wired serial port.
* There is no parity bit in HP palmtop RAM. The NMI parity error detection in the IBM XT is not present in the HP palmtop.
* The HP palmtop hardware interrupt set is not identical in function to an XT.
* Some XT compatibles have Real Time Clock services. The HP palmtop provides a set of Real Time Clock Time/Date/Alarm services supported in hardware by a 26-bit, 1 Hz timer.

## Changes to PSA Code

This list reflects some of the major changes to the PSA Code to support the BIOS on the HP palmtop. Effort has been made to maintain PC and HP 95LX compatibility in the user interface (inputs,outputs, and resource utilization) for BIOS routines that provide identical functionality in other HP palmtops. A number of additional non-standard BIOS services have been added to support features unique to HP palmtops.

* **Reset** Power On code is changed so that turning the machine on causes the machine to return to the application that was running before the system was powered down.

* **Int 02h** Nonmaskable interrupt (NMI).

  This interrupt is invoked in the IBM-XT when a RAM parity error occurs. It is invoked on the HP palmtop by the Low Power interrupt (LPI) or the Module Pulled interrupt (MPI).

* **Int 05h** Print Screen interrupt.

  The HP palmtop Print Screen service prints the contents of the active display page.

* **Int 06h** General Purpose hook.

  On the HP 95LX this hook was used for Low Power processing only. On any other HP palmtop, this hook is provided in several additional locations to permit applications to intervene in certain processing.

* **Int 07h** The HP palmtop BIOS installs a special Int 07h handler on power-up.

  Certain DOS applications do not use the equipment check to determine whether or not a numeric coprocessor is present. Instead they attempt to execute certain coprocessor instructions. Because of the configuration of the embedded 80186 CPU in HP palmtops, attempted execution of numeric coprocessor instructions will cause ESC Interrupts (Int 07h) to be generated. Without the default Int 07h handler supplied by the BIOS, numeric coprocessor instructions will "hang". The default Int 07h handler supplied by the BIOS "decodes" numeric coprocessor instructions, causing them to be skipped. This enables most applications to successfully determine that a numeric coprocessor is not present.

* **Int 08h** IRQ0 - Timer Hardware service.

  The Timer0 service was modified to add display window control via the menu-arrow key sequences.

* **Int 09h** IRQ1 - Keyboard interrupt.

  Int 09 was modified to support Fn key translations, and Mute key translations.

* **Int 0Ah** IRQ2 - Hornet Miscellaneous interrupt.

  Display Cursor Update, RTC, Timer1, RX, Ring Detect, Keyboard, IR, and several GPIO lines can be the source for this hardware interrupt. This is a reserved interrupt in the IBM XT.

* **Int 0Bh** IRQ3 - Unused (GPIO[24:26] Pads if properly configured).

  This is the COM2 interrupt in the IBM-XT.

* **Int 0Ch** IRQ4 - UART interrupt.

  This is the COM1 interrupt in the IBM-XT.

* **Int 0Dh** IRQ5 - Unused (GPIO[24:26] Pads if properly configured)

  This is the fixed disk interrupt in the IBM-XT.

* **Int 0Eh** IRQ6 - Ready/Change (GPIO[24]) interrupt.

  This is the diskette interrupt in the IBM-XT.

* **Int 0Fh** IRQ7 - Unused (GPIO[24:26] Pads if properly configured).

  This is the LPT1 (printer) interrupt in the IBM-XT.

* **Int 10h** Video services.

  MDA and CGA functions were modified for the "Hornet" hardware IC. Several non-standard services added.

* **Int 13h** Disk services.

  Modified to work with a RAM disk.

* **Int 14h** Serial Port services.

  Original PSA serial port services were modified to support the new 9-pin connection provided by the Hornet chip.

* **Int 15h** System services (Obsolete cassette control in XT).

  Most of Int 15h has been modified.

* **Int 16h** Keyboard services.

  This interrupt was modified to trap an [ON] key press while machine is running. Several hot-key sequences were added for display tuning control. Int 16h also invokes light sleep code as a power-saving feature.

* **Int 17h** Printer services.

  This interrupt was modified to work with a serial printer. The HP palmtop observes XON/XOFF flow control.

* **Int 19h** HP palmtops boot DOS from ROM.

* **Int 1Ah** Time of Day services.

  Support of real time clock, including the capability of setting an alarm, was added. The alarm is capable of turning on power to the unit. Nonstandard functions were added to set alarms to future date (avoids shelf storage turn-on for system time updates).

* **Int 1Eh** Set to a dummy IRET.

  This is the disk parameter table in the IBM-XT.

* **Int 1Fh** Graphics Character Table pointer.

  This points to the last half (chars 80h - FFh) of the default font in the currently active code page font (std 8x8, byte wide).

**The following PSA interrupt handlers were not modified:**

* **Int 11h** Equipment Check service.
* **Int 12h** Get Memory Size service.
* **Int 14h** Serial Port service.
* **Int 1Bh** Keyboard Break Default handler.
* **Int 1Ch** User Timer Tick hook.
* **Int 1Dh** Video Parameter table

## BIOS RAM Definition

This section describes how memory is organized and used by the BIOS.

Table 1: CPU Address Space

Description               | Base
--------------------------|----------
28KB hard mapped BIOS ROM | F9000h
36KB hard mapped ROM      | F0000h
4 - 16KB Banks            | E0000h
4 - 16KB Banks            | D0000h
1 - 64KB Bank             | C0000h
16KB hard mapped ROM      | BC000h
CGA Frame Buffer          | B8000h
16KB hard mapped ROM      | B4000h
MDA Frame Buffer          | B0000h
64KB hard mapped OS Fcns  | A0000h
OS Data                   | Variable*
Application Program Area  | Variable*
DOS                       | Variable*
DOS Data Area             | 00600h
BIOS Data Area            | 00400h
Interrupt Vectors         | 00000h

* The size of the Disk Operating System area varies since optional drivers and buffers may occupy variable amounts of RAM.

## HP100LX/200LX Memory Map

![HP 100LX/200LX Memory Map](image/__dg0201.gif)

NRCE, and NRAS [0] through NRAS[3] are separate chips. They are accessed by mapping portions of them into memory banks starting at C000 (the areas in gray). Each chip has its own ID; you need to provide the appropriate chip ID to the Hornet mapping registers. See INT 63H for more details.

The PCMCIA card in the slot is accessed as NCS [0]. For a RAM card, it is mapped into bank memory and manipulated by the DOS disk drivers (the same as the rest of system RAM). For other cards, only the Attribute memory is mapped into real address space. Details on accessing the rest of the card via I/O addresses varies on a card per card basis.

Memory Address Ranges
Table 1:

Address Range | Description                                      | Ksize
--------------|--------------------------------------------------|-----:
000000-009000 | 36K of SysMgr permap at F000:0000                |    36
009000-020000 | 28K BIOS and 64K DOS/CardDrv                     |    28
020000-024000 | 16K of SysMgr permap at BC00:0000                |    16
024000-100000 | CB$HIPS.IMG (HP B400:0000 and ROMDISK)           |   880
100000-120000 | Compressed initialized data for all applications |   128
120000-13C000 | CDB.EXE (Kitchen Sink Database)                  |   112
140000-168000 | CAPPTSBK.EXE and LLRAS.EXE                       |   160
168000-17C000 | .RI files                                        |    80
180000-1A0000 | HPCALC, STOPWATCH, MACROS, DOSTASK               |   128
1A0000-1C8000 | 123, ALRMAPP                                     |   160
1C8000-1E0000 | FILER, APPMGR                                    |    96
1E0000-1E4000 | SysMgr OVL8                                      |    16
1E4000-1E8000 | SysMgr OVL7                                      |    16
1E8000-1EC000 | SysMgr OVL6                                      |    16
1EC000-1F0000 | SysMgr OVLS                                      |    16
1F0000-1F4000 | SysMgr OVL4                                      |    16
1E4000-1E8000 | SysMgr OVL3                                      |    16
1F8000-1FC000 | SysMgr OVL2                                      |    16
1FC000-200000 | SysMgr OVL1                                      |    16
200000-200000 | QHPO.EXE                                         |    48
200000-20D000 | QHPLOADR.EXE                                     |     4
210000-220000 | QHPX1.XIP                                        |    64
220000-230000 | QHPX2.XIP                                        |    64
230000-240000 | QHPX4.XIP                                        |    64
240000-24C000 | QHPX3.XIP                                        |    48
24C000-258000 | QHPX5.XIP                                        |    48
260000-280000 | DATACOMM, SETUP, MEMO, TASKO                     |   128

## Interrupt Vector Table

The interrupt vector table is in the address range from 0:0000h through 0:3ffh. Vectors used by BIOS are initialized by the BIOS initialization code. Each vector consists of a segment and an offset (4 bytes).

The interrupt vector assignments, functions, and types are listed in the following table. There are four types of interrupts: ISR, service, hook, and table.

* An ISR is a interrupt service routine invoked by the processor or one of its peripherals. These routines should not be called from applications since unpredictable results may occur. Any ISR handler may be taken over (re-written), but this is recommended only as a last resort. In some cases, this can be quite simple and safe. In others, hardware details (eg; timing, current limitations, etc.) can make the task tricky. In some cases, if taking over the ISR handler is not done with care, it is even possible to damage the hardware.
* A service is an application program callable interrupt. Such interrupts provide functions that an application can call by using the appropriate Int instruction.
* A hook is an interrupt service routine provided for applications to optionally take over. Generally, the HP palmtop hook is invoked by a service or ISR, and its default action is a NOP. An application writes a handler for the hook, and sets the Interrupt Ptr to point to the handler. Thus when the service or ISR is invoked, the application gets a chance (thru the hook) to modify the action of the service or ISR.
* A table is a pointer to a table of data bytes.

INTERRUPT | ADDRESS RANGE (HEX) | FUNCTION                              | TYPE
----------|---------------------|---------------------------------------|--------
00h       | 000-003             | Divide by Zero                        | ISR
01h       | 004-007             | Single Step                           | ISR
02h       | 008-00B             | NMI Interrupt                         | ISR
03h       | 00C-00F             | Breakpoint                            | ISR
04h       | 010-013             | Arithmetic Overflow                   | ISR
05h       | 014-017             | Print Screen                          | SERVICE
06h       | 018-01B             | BIOS Hook                             | HOOK
07h       | 01C-01F             | ESC Opcode Trap                       | ISR
08h       | 020-023             | IRQ0, Timer0 Interrupt                | ISR
09h       | 024-027             | IRQ1, PC Keyboard Interrupt           | ISR
0Ah       | 028-02B             | IRQ2, Hornet Misc. Interrupt          | ISR
0Bh       | 02C-02F             | IRQ3, Unused (GPIO) Interrupt         | ISR
0Ch       | 030-033             | IRQ4, UART (COM1) Interrupt           | ISR
0Dh       | 034-037             | IRQ5, Unused (GPIO) Interrupt         | ISR
0Eh       | 038-03B             | IRQ6, Ready/Change Interrupt          | ISR
0Fh       | 03C-03F             | IRQ7, Unused (GPIO) Interrupt         | ISR
10h       | 040-043             | Video Services                        | SERVICE
11h       | 044-047             | Equipment Check                       | SERVICE
12h       | 048-04B             | Memory Size                           | SERVICE
13h       | 04C-04F             | Flexible Disk Services                | SERVICE
14h       | 050-053             | Serial Port Services                  | SERVICE
15h       | 054-057             | System Functions                      | SERVICE
16h       | 058-05B             | Keyboard Services                     | SERVICE
17h       | 05C-05F             | Dummy Return                          | --
18h       | 060-063             | Reserved                              | --
19h       | 064-067             | Boot                                  | SERVICE
1Ah       | 068-06B             | Time-of-Day Services                  | SERVICE
1Bh       | 06C-06F             | Keyboard Break                        | HOOK
1Ch       | 070-073             | User Timer Tick                       | HOOK
1Dh       | 074-077             | Video Parameter Table Pointer         | Table
1Eh       | 078-07B             | Flexible Disk Parameter Table Pointer | Table
1Fh       | 07C-07F             | Graphics Character Table Pointer      | Table
20h-3Fh   | 080-0FF             | Reserved for DOS                      | --
40h-49h   | 100-127             | Reserved                              | --
4Ah       | 128-12B             | User Alarm Interrupt                  | HOOK
4Bh-5Eh   | 12C-178             | Reserved                              | --
5Fh       | 17C-17F             | Low level graphics                    | SERVICE
60h-61h   | 180-187             | System Manager Interrupts             | SERVICE
62h       | 188-18B             | Reserved                              | --
63h       | 18C-18F             | XIP Services                          | SERVICE
64h-6Fh   | 190-1BF             | Reserved                              | --
70h       | 1C0-1C3             | Reserved                              | --
71h-F0h   | 1C4-3C3             | Reserved                              | --
F1h-FFh   | 3C4-3FF             | Not Used                              | --

Most HP palmtop interrupts have the same function as the corresponding PSA interrupt. Interrupts with different functions from the PSA BIOS definition are shown in the following table.

INT (HEX) | PALMTOP FUNCTION | IBM-XT FUNCTION   | TYPE         | IRQ
----------|------------------|-------------------|--------------|----
02h       | Very Low Batt    | RAM Parity Errors | Non-Maskable | NMI
06h       | BIOS Hook        | Reserved          | HOOK         |
09h       | Keyboard         | Keyboard          | ISR*         | 1
0Ah       | Hornet Misc      | Reserved          | ISR          | 2
0Bh       | GPIO[24:26]      | COM2              | ISR          | 3
0Dh       | GPIO[24:26]      | Hard Disk         | ISR          | 5
0Eh       | GPIO[24:26]      | Floppy Disk       | ISR          | 6
0Fh       | GPIO[24:26]      | LPT               | ISR          | 7

Int 09h is a hardware interrupt in the IBM XT, but is invoked by software in the HP palmtop. After an IRQ2 (Int 0Ah) Keyboard interrupt, the software scans and debounces the keyboard and writes the keycode to the keycode register (60h). The software then invokes INT 09h indirectly by writing to a special Hornet register.

## BIOS Data Area

The BIOS data area is initialized and used by the BIOS code. HP palmtop RAM definitions are based on the PSA XT BIOS definitions. There is effort to be compatible with the HP 95LX usage, which is also based on the PSA definition. Avoid writing software that depends directly on the contents of this RAM.

### BIOS Data Area Definitions

```
ADDRESS  NAME     LENGTH  DESCRIPTION
                  (BYTES)
==========================================================================================
40h:00h  EIADRTBL   8     I/O address of up to 4 serial communications ports
------------------------------------------------------------------------------------------
40h:08h  LPADRTBL   6     I/O address of up to 3 parallel ports (All 00's in HP palmtop).
------------------------------------------------------------------------------------------
40h:0Eh  EXTDAT     2     Segment address of extended data area (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:10h  DEVFLG     2     Equipment variable.
                          BIT     DEFINITION
                          15-14   Number of printer adapters
                          13-12   Reserved
                          11-9    Number of RS-232 Adapters
                           8      Reserved
                          7-6     Number of disk drives where
                                   00b=1 drive
                                   01b=2 drives
                          5-4     Initial video mode
                                   00b=Reserved
                                   01b=40x25 Color
                                   10b=80x25 Color
                                   11b=80x25 Black & White
                           3      Reserved
                           2      1 if Pointing device installed
                           1      1 if Math coprocessor installed
                           0      1 if Disk installed
------------------------------------------------------------------------------------------
40h:12h  MANTST     1     reserved for Manufacturer Test (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:13h  MEMSIZE    2     installed memory in Kilobytes
------------------------------------------------------------------------------------------
40h:15h  BASMEM     2     reserved (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:17h  SHFLGS     1     Keyboard flag 1
                          BIT     DEFINITION
                           7      1=Insert active
                           6      1=Caps Lock active
                           5      1=Num Lock active
                           4      1=Scroll Lock active
                           3      1=Alt pressed
                           2      1=Ctrl pressed
                           1      1=Left shift presse
                           0      1=Right shift pressed
------------------------------------------------------------------------------------------
40h:18h  SHFLGS2    1     Keyboard flag 2
                          BIT     DEFINITION
                           7      1=Insert pressed
                           6      1=Caps Lock pressed
                           5      1=Num Lock pressed
                           4      1=Scroll Lock Pressed
                           3      1=Ctrl-Num Lock active (Pause)
                           2      1=Sys Req pressed (Unused in HP palmtop)
                           1      1=Left Alt pressed
                           0      1=Left Ctrl pressed
------------------------------------------------------------------------------------------
40h:19h  ALTDATA    1     Alt-key, keypad buffer
------------------------------------------------------------------------------------------
40h:1Ah  KBGET      2     Key buffer read pointer (relative to Segment 0040h)
------------------------------------------------------------------------------------------
40h:1Ch  KBPUT      2     Key buffer write pointer (relative to Segment 0040h)
------------------------------------------------------------------------------------------
40h:1Eh  KBUFR      32    Key buffer (16 words, 15 keys maximum)
------------------------------------------------------------------------------------------
40h:3Eh  DRVSTAT    1     Floppy recalibrate status (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:3Fh  FDMOTS     1     Floppy disk motor status (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:40h  FDTIMO     1     Floppy motor time-out count (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:41h  ERRSTAT    1     Disk Status return code (unused in HP palmtop)
                          BIT     DEFINITION
                           7      1=Drive not ready
                           6      1=seek error occurred
                           5      1=disk ctrlr failed
                          4-0     Error codes, where:
                                   0h=No error
                                   1h=Illegal function was requested
                                   2h=Address mark not found
                                   3h=Write protect error
                                   4h=Sector not found
                                   6h=Drive door was opened
                                   8h=DMA overrun error (unused in palmtop)
                                   9h=DMA boundary error (unused in palmtop)
                                   Ch=Media type unknown
                                  10h=CRC failed on disk read
------------------------------------------------------------------------------------------
40h:42h  DSKST      7     Floppy controller status & cmd bytes (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:49h  CRTMODE    1     Video mode setting
------------------------------------------------------------------------------------------
40h:4Ah  SCRNWID    2     Number of character columns on screen
------------------------------------------------------------------------------------------
40h:4Ch  SCRNLEN    2     Current page size (bytes - 2K,4K, or 16K)
------------------------------------------------------------------------------------------
40h:4Eh  PAGADDR    2     Page offset in bytes (=SCRNLEN*DSPYPAG)
------------------------------------------------------------------------------------------
40h:50h  CURCOOR    16    Cursor coordinates for 8 pages, with two bytes for each page
                          The first byte of each pair is the column, the second byte is
                          the row.  (0,0) is the upper left corner of the screen.
------------------------------------------------------------------------------------------
40h:60h  CURTYPE    2     Cursor size: byte1 = end scan line, byte2 = start scan line
------------------------------------------------------------------------------------------
40h:62h  DSPYPAG    1     Current display page number
------------------------------------------------------------------------------------------
40h:63h  CRTADDR    2     Base I/O address of Video controller (03b4h/03d4h)
------------------------------------------------------------------------------------------
40h:65h  MSRCOPY    1     Display controller mode register (copy)
------------------------------------------------------------------------------------------
40h:66h  PALETTE    1     Display controller palette (copy)
------------------------------------------------------------------------------------------
40h:67h  ///////    4     (Reserved in HP palmtop)
------------------------------------------------------------------------------------------
40h:6Bh  INTLST     1     Last Interrupt
------------------------------------------------------------------------------------------
40h:6Ch  LOTIME     2     Timer count: ticks since midnight (LSW)
------------------------------------------------------------------------------------------
40h:6Eh  HITIME     2     Timer count: ticks since midnight (MSW)
------------------------------------------------------------------------------------------
40h:70h  HOUR24     1     24 hour rollover flag
------------------------------------------------------------------------------------------
40h:71h  BRKFLG     1     Ctrl-Brk flag (bit 7=1 : <Ctrl><Break> pressed)
------------------------------------------------------------------------------------------
40h:72h  RSTFLG     2     Warmstart flag: 1234h means warmstart
------------------------------------------------------------------------------------------
40h:74h  HDSTAT     1     Hard disk status (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:75h  HDNUMB     1     Number of hard drives (0 in HP palmtop)
------------------------------------------------------------------------------------------
40h:76h  HDCTRL     1     Hard disk control byte copy (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:77h  HDPORT     1     Hard disk controller port offset (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:78h  LPTOTBL    3     Parallel printer time-out table, Ports 0-2
------------------------------------------------------------------------------------------
40h:7Bh  HALTVAL    1     Halt Value (Event timeout)
------------------------------------------------------------------------------------------
40h:7Ch  EIATOTBL   4     Serial port time-out table, Ports 0-3\.
                          Only port 0 is built-in to the HP palmtop.
------------------------------------------------------------------------------------------
40h:80h  KBXTGET    2     Offset to Key Buffer start (40h:[KBXTGET])
------------------------------------------------------------------------------------------
40h:82h  KBXTPUT    2     Offset to 1st byte after Key buffer (40h:[KBXTPUT])
------------------------------------------------------------------------------------------
40h:84h  CRTROW     1     Number of video rows -1 (EGA function used in HP palmtop)
------------------------------------------------------------------------------------------
40h:85h  CHRSIZ     2     Character height EGA mode(unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:87h  EGAINF     1     EGA Info Video control bits (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:88h  EGAIN2     1     EGA Info2 switch data (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:89h  ///////    2     (Reserved in HP palmtop)
------------------------------------------------------------------------------------------
40h:8Bh  FDRATE     1     Last floppy data rate selected (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:8Ch  HDSTRG     1     Hard Disk Controller status copy (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:8Dh  HDERRG     1     Hard disk error status copy (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:8Eh  HDINTR     1     Hard Disk Interrupt flag (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:8Fh  HDCFLG     1     Hard Disk Controller flag (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:90h  KYBSET     2     Keyboard Set flag
------------------------------------------------------------------------------------------
40h:92h  LANG       1     Language Set flag
------------------------------------------------------------------------------------------
40h:93h  WINFLG     1     Windowing Enable flag- Cursor and Kybd
                                     CURSOR     KBD
                          VALUE     ENABLED   ENABLED
                           00h        Yes       Yes
                           01h        Yes       No
                           02h        No        Yes
                           03h        No        No
------------------------------------------------------------------------------------------
40h:94h  WkeUpSrc   1     Wake Up source, where:
                          BIT     DEFINITION
                           0      1 = Keyboard wakeup
                           1      1 = UART wakeup
                           2      1 = RTC wakeup
                           3      1 = GPIO wakeup
                           0-3    0h = System Reset
------------------------------------------------------------------------------------------
40h:95h  T1_State   1     Timer1 state (how being used)
                          0 = Polling (Periodic Services)
                          1 = Kbd Debounce
                          2 = Push Scan Codes
                          3 = Send (to Int 09h) Scan Codes
------------------------------------------------------------------------------------------
40h:96h  KBDSTAT1   1     Kbd Status Byte 1 (Unused in HP palmtop):
                          BIT     DEFINITION
                           7      1=Read ID in progress
                           6      1=Last code was first ID
                           5      1=Forced NumLock
                           4      1=101/102 kbd present
                           3      1=Right Alt Active
                           2      1=Right Ctrl Active
                           1      1=Last code was E0h
                           0      1=Last code was E1h
------------------------------------------------------------------------------------------
40h:97h  KBDSTAT2   1     Kbd Status Byte 2 (Unused in HP palmtop):
                          BIT     DEFINITION
                           7      1=Error Flag for Kbd command
                           6      1=LED update in progress
                           5      1=RESEND received from Kbd
                           4      1=ACK received from Kbd
                           3      1=Reserved
                           2      1=Caps Lock LED Status
                           1      1=Num Lock LED Status
                           0      1=Scroll Lock LED Status
------------------------------------------------------------------------------------------
40h:98h  NoBeep     1     beep suppression for BIOS Messages:
                          BIT     DEFINITION
                           7      1=Msg# 7
                           6      1=Msg# 6
                           5      1=Msg# 5
                           4      1=Msg# 4
                           3      1=Msg# 3
                           2      1=Msg# 2
                           1      1=Msg# 1
                           0      1=Msg# 0
------------------------------------------------------------------------------------------
40h:99h  ///////    1     (Reserved in HP palmtop)
------------------------------------------------------------------------------------------
40h:9Ah  BATsys     1     Last system battery measurement
------------------------------------------------------------------------------------------
40h:9Bh  BATbkup    1     Last back-up battery measurement
------------------------------------------------------------------------------------------
40h:9Ch  BATref     1     Last reference battery measurement
------------------------------------------------------------------------------------------
40h:9Dh  KYBFLG     2     Keyboard type
------------------------------------------------------------------------------------------
40h:9Fh  CRTZOOM    1     Extension of CRTMODE (0-7, 20h,21h, 80h-85h)
------------------------------------------------------------------------------------------
40h:A0h  WTACTF     1     Wait Active flag (unused in HP palmtop)
------------------------------------------------------------------------------------------
40h:A1h  TimOutCnt  2     Number of timer ticks until display timeout
------------------------------------------------------------------------------------------
40h:A3h  TimOutVal  2     Display Time Out reset value
------------------------------------------------------------------------------------------
40h:A5h  PrtStatus  1     Printer Status flag
------------------------------------------------------------------------------------------
40h:A6h  CurFlag    1     Cursor Movement flag
------------------------------------------------------------------------------------------
40h:A7h  BATTFL3    1     Battery flags, Byte 3
                          BIT     DEFINITION
                           7      1 = xxxx (/// RESERVED ///)
                           6      1 = VLBAK (Very Low Main Batt Acknowledge)
                           5      1 = xxxx (/// RESERVED ///)
                           4      1 = xxxx (/// RESERVED ///)
                           3      1 = xxxx (/// RESERVED ///)
                           2      1 = xxxx (/// RESERVED ///)
                           1      1 = xxxx (/// RESERVED ///)
                           0      1 = xxxx (/// RESERVED ///)
------------------------------------------------------------------------------------------
40h:A8h  CPUCKSUM   2     CPU register checksum
------------------------------------------------------------------------------------------
40h:AAh  URAMCKSUM  2     User RAM checksum
------------------------------------------------------------------------------------------
40h:ACh  IOCKSUM    2     Hornet register checksum
------------------------------------------------------------------------------------------
40h:AEh  SS_Save    2     Stack Segment register save location
------------------------------------------------------------------------------------------
40h:B0h  SP_Save    2     Stack Pointer register save location
------------------------------------------------------------------------------------------
40h:B2h  DAYCNTR    2     Day Counter
                          This word contains the count of times the software clock at
                          40h:06Ch has been set to zero.
------------------------------------------------------------------------------------------
40h:B4h  RTC_Cent   1     RTC century value in bcd
------------------------------------------------------------------------------------------
40h:B5h  RTC_Yr     1     RTC year value in bcd
------------------------------------------------------------------------------------------
40h:B6h  RTC_Mon    1     RTC month value in bcd
------------------------------------------------------------------------------------------
40h:B7h  RTC_Day    1     RTC day of month value in bcd
------------------------------------------------------------------------------------------
40h:B8h  RTC_Hour   1     RTC hour value in bcd
------------------------------------------------------------------------------------------
40h:B9h  RTC_Min    1     RTC minute value in bcd
------------------------------------------------------------------------------------------
40h:BAh  RTC_Sec    1     RTC second value in bcd
------------------------------------------------------------------------------------------
40h:BBh  RTC_DayLt  1     RTC daylight savings time flag
------------------------------------------------------------------------------------------
40h:BCh  ALM_Cent   1     RTC ALARM century value in bcd
------------------------------------------------------------------------------------------
40h:BDh  ALM_Yr     1     RTC ALARM year value in bcd
------------------------------------------------------------------------------------------
40h:BEh  ALM_Mon    1     RTC ALARM month value in bcd
------------------------------------------------------------------------------------------
40h:BFh  ALM_Day    1     RTC ALARM day value in bcd
------------------------------------------------------------------------------------------
40h:C0h  ALM_Hour   1     RTC ALARM hour value in bcd
------------------------------------------------------------------------------------------
40h:C1h  ALM_Min    1     RTC ALARM minute value value in bcd
------------------------------------------------------------------------------------------
40h:C2h  ALM_Sec    1     RTC ALARM second value value in bcd
------------------------------------------------------------------------------------------
40h:C3h  ALM_Stat   1     RTC ALARM status; where:
                          BIT     DEFINITION
                           7      1 = /// RESERVED ///
                           6      1 = /// RESERVED ///
                           5      1 = /// RESERVED ///
                           4      1 = /// RESERVED ///
                           3      1 = /// RESERVED ///
                           2      1 = Alarm already serviced.
                           1      1 = Alarm Enabled to pwr on.
                           0      1 = Alarm Enabled (no pwr on)
------------------------------------------------------------------------------------------
40h:C4h  TMRVAL     4     RTC timer value at last update of RTC Variables.
------------------------------------------------------------------------------------------
40h:C8h  NRAS0_Size 1     NRAS0 RAM physical device size
                          0 = No device, else
                          BIT     DEFINITION
                           7      1=32M (128)
                           6      1=16M (64)
                           5      1=8M (32)
                           4      1=4M (16)
                           3      1=2M (8)
                           2      1=1M (4)
                           1      1=512K (2)
                           0      1=256K (1)
------------------------------------------------------------------------------------------
40h:C9h  NRAS1_Size 1     NRAS1 RAM physical device size
                          This is the same format as NRAS0_Size.
------------------------------------------------------------------------------------------
40h:CAh  NRAS2_Size 1     NRAS2 RAM physical device size
                          This is the same format as NRAS0_Size.
------------------------------------------------------------------------------------------
40h:CBh  NRAS3_Size 1     NRAS3 RAM physical device size
                          This is the same format as NRAS0_Size.
------------------------------------------------------------------------------------------
40h:CCh  XIPFL      1     Port Locked & System Manager Media Changed flags
                          BIT     DEFINITION
                           7      1 = Port 1 locked
                           6      1 = Port 0 locked
                           5      1 = /// RESERVED ///
                           4      1 = /// RESERVED ///
                           3      1 = /// RESERVED ///
                           2      1 = /// RESERVED ///
                           1      1 = Port 1 media changed
                           0      1 = Port 0 media changed
------------------------------------------------------------------------------------------
40h:CDh  SysFlags   1     System flags
                           bit 7 Time/Date Change flag (Int 15h, Fcns 03h,05h)
                           bit 6 Graphics Cursor Service in Progress
                           bit 5 Zoom Disabled
                           bit 4 Charger-Poll in Progress
                           bit 3 Battery-Poll in Progress
                           bit 2 DAC In-Use
                           bit 1 Tmp Msg in Display
                           bit 0 /// RESERVED ///
------------------------------------------------------------------------------------------
40h:CEh  NumPadFlags 2    Bits when set indicate a NumPad "digit" MAKE Scan Code has
                          been pushed into the Scan Code buffer and we are awaiting
                          keyup to send the corresponding BREAK.
                           bit 13 [MENU] [0]
                           bit 12 [MENU] [+]
                           bit 11 [MENU] [.]
                           bit 10 [MENU] [4]
                           bit  9 [MENU] [*]
                           bit  8 [MENU] [5]
                           bit  7 [MENU] [6]
                           bit  6 [MENU] [1]
                           bit  5 [MENU] [-]
                           bit  4 [MENU] [2]
                           bit  3 [MENU] [3]
                           bit  2 [MENU] [7]
                           bit  1 [MENU] [8]
                           bit  0 [MENU] [9]
------------------------------------------------------------------------------------------
40h:D0h  BATCKRATE  2     Battery service rate (55 msec units).
------------------------------------------------------------------------------------------
40h:D2h  BATCKCNT   2     Counts before battery service (55 msec units).
------------------------------------------------------------------------------------------
40h:D4h  BATTFL1    1     Battery flags, Byte 1
                          BIT     DEFINITION
                           7      1 = TYPE (0=Alkaline, 1=NiCad)
                           6      1 = NOAC (Pwr Source: 1 = Batt, 0 = AC_Adaptor)
                           5      1 = FCHG (Charge Speed = Fast)
                           4      1 = ECHG (Charging In Progress)
                           3      1 = UCHG (Charging User Enabled)
                           2      1 = CARD (Low Card Battery)
                           1      1 = BKUP (Low BackUp Battery)
                           0      1 = MAIN (Low Main Battery)
------------------------------------------------------------------------------------------
40h:D5h  BATTFL2    1     Battery flags, Byte 2
                          BIT     DEFINITION
                           7      1 = OVDT (Over Voltage Detected)
                           6      1 = VLBI (Very Low Batt - Software ShutDn)
                           5      1 = NIFL (Bits 54: 01=LOW, 00=OK, 1x=FULL)
                           4      1 = NILO (Bits 54 = NiCad Level)
                           3      1 = /// Reserved ///
                           2      1 = CDAK (Low Card Batt Acknowdledged)
                           1      1 = BPAK (Low BackUp Batt Acknowledged)
                           0      1 = MNAK (Low Main Batt Acknowledged)
------------------------------------------------------------------------------------------
40h:D6h  LteSlpFL   1     Light Sleep flag
                          BIT     DEFINITION
                           0      1 = Avoid lite sleep in Int 16h, Fcns{01h,11h}.
                           1      1 = Currently in lite sleep.
------------------------------------------------------------------------------------------
40h:D7h  KBDBM      10    Current Keyboard BIT MAP, last bitmap collected by INT 0Ah.
------------------------------------------------------------------------------------------
40h:E1h  TMPBM      10    Keyboard work area
------------------------------------------------------------------------------------------
40h:EBh  AnnFlags   1     Indicates presence of annunciators in display
                          BIT     DEFINITION
                           7      1 = Annun Display Disabled
                           6      1 = Right Corner Location
                           5      1 = /// RESERVED ///
                           4      1 = /// RESERVED ///
                           3      1 = /// RESERVED ///
                           2      1 = Caps Annunciator present
                           1      1 = Fn Annunciator present
                           0      1 = Shift Annunciator present
------------------------------------------------------------------------------------------
40h:ECh  LPI_Flag   2     Low Power NMI Interrupt flag (BackUp mode
------------------------------------------------------------------------------------------
40h:EEh  OutCopy    1     Keyboard output register copy
------------------------------------------------------------------------------------------
40h:EFh  CodePage   2     Current code page (437h,850h,852h,860h,863h,865h,..)
------------------------------------------------------------------------------------------
40h:F1h  KBDFLGS    1     Keyboard flags maintained and used during scan code creation
                          (before being sent to Int 09h) to maintain proper shift status.
                           bit 7     /// RESERVED ///
                           bit 6     /// RESERVED ///
                           bit 5     /// RESERVED ///
                           bit 4     FnFL - Fn Active
                           bit 3     ALTSHF - Alt Active
                           bit 2     CTRLSHF - Control Active
                           bit 1     LEFTSHF - Left Shift Active
                           bit 0     RGHTSHF - Right Shift Active
------------------------------------------------------------------------------------------
40h:F2h  FnFlags    2     Bits when set indicate a Fn MAKE Scan Code has been pushed
                          into the Scan Code buffer and we are awaiting keyup to send the
                          corresponding BREAK.
                           bit 15     /// RESERVED ///
                           bit 14     [Fn][+] = Paste
                           bit 13     [Fn][=] = Copy
                           bit 12     [Fn][RDOT] = Cut
                           bit 11     [Fn][LDOT] = Time
                           bit 10     [Fn][COMMA] = Date
                           bit  9     [Fn][SPACE] = Zoom
                           bit  8     [Fn][2] = F12
                           bit  7     [Fn][1] = F11
                           bit  6     [Fn][BS] = SCRL
                           bit  5     [Fn][0] = PrtSc
                           bit  4     [Fn][DEL] = INS
                           bit  3     [Fn][DnArrow] = PgDn
                           bit  2     [Fn][LtArrow] = End
                           bit  1     [Fn][RtArrow] = Home
                           bit  0     [Fn][UpArrow] = PgUp
------------------------------------------------------------------------------------------
40h:F4h  SysFlags2  2     System flags Byte 2
                           bit 7     /// RESERVED ///
                           bit 6     /// RESERVED ///
                           bit 5     NIFLC (NIFL at Charge Start - See BATTFL2)
                           bit 4     NILOC (NILO at Charge Start - See BATTFL2)
                           bit 3     /// RESERVED ///
                           bit 2     /// RESERVED ///
                           bit 1     /// RESERVED ///
                           bit 0     /// RESERVED ///
------------------------------------------------------------------------------------------
40h:F5h  Debounce   1     Software Keyboard Debounce state
------------------------------------------------------------------------------------------
40h:F6h  LASTIRQ2   2     Rotating IRQ2 Priority
------------------------------------------------------------------------------------------
40h:F8h  LASTKY     1     Last key pressed
------------------------------------------------------------------------------------------
40h:F9h  RPTCNT     1     Key repeat counter
------------------------------------------------------------------------------------------
40h:FAh  MISCFL     1     Miscellaneous Keyboard flags
                          BIT     DEFINITION
                           7      1=ClrMuteEn (Ready to clear Mute function)
                           6      1=MenuDown (Windowing in zoomed text)
                           5      1=ClrFnEn (Ready to clear Fn Key function)
                           4      1=FnFL (Fn Key function Active)
                           3      1=SH2NORM (HP palmtop shifted, PC unshifted)
                           2      1=NORM2SH (HP palmtop unshifted, PC shifted)
                           1      1=ClrShEn (Ready to send Shift Break)
                           0      1=OnKySeq (On key sequence active)
------------------------------------------------------------------------------------------
40h:FBh  MUTEFL     1     Mute key flags
------------------------------------------------------------------------------------------
40h:FCh  KYDELAY    1     Key repeat start delay (in 33 msec tick units)
------------------------------------------------------------------------------------------
40h:FDh  TYPMATC    1     Typematic key repeat rate (in 33 msec tick units)
------------------------------------------------------------------------------------------
40h:FEh  RESETID    2     Power-Up ID flags
------------------------------------------------------------------------------------------
```

## Other BIOS Data Area Definitions

```
ADDRESS  NAME     LENGTH  DESCRIPTION
                  (BYTES)
==========================================================================================
50h:00h  PRTFLG     1     Print screen status byte (=40h:100h)
                           00h=No Print Screen activity
                           01h=Print Screen operation in progress
                           ffh=Previous Print Screen operation failed.
------------------------------------------------------------------------------------------
50h:01h  BRKFLG     1     Break Flag
------------------------------------------------------------------------------------------
50h:02h  ??????     2     (??????)
------------------------------------------------------------------------------------------
50h:04h  ??????     1     Single drive status (Drive A or B)
------------------------------------------------------------------------------------------
50h:05h  ??????     12    (??????)
------------------------------------------------------------------------------------------
50h:10h  ??????     2     BASIC's Segment Address store
------------------------------------------------------------------------------------------
50h:12h  ??????     2     BASIC's Timer Interrupt Vector offset
------------------------------------------------------------------------------------------
50h:14h  ??????     2     BASIC's Timer Interrupt Vector segment
------------------------------------------------------------------------------------------
50h:16h  ??????     2     BASIC's CTRL-Break Interrupt Vector offset
------------------------------------------------------------------------------------------
50h:18h  ??????     2     BASIC's CTRL-Break Interrupt Vector segment
------------------------------------------------------------------------------------------
50h:1Ah  ??????     2     BASIC's Disk Error Interrupt Vector offset
------------------------------------------------------------------------------------------
50h:1Ch  ??????     2     BASIC's Disk Error Interrupt Vector segment
------------------------------------------------------------------------------------------
50h:1Eh  ??????     4     (??????)
------------------------------------------------------------------------------------------
50h:22h  DSKPRM     11    Disk Parameter table
------------------------------------------------------------------------------------------
50h:2Dh  ??????     1     (??????)
------------------------------------------------------------------------------------------
50h:2Eh  ??????     4     IBM wants 4 zeros here.
------------------------------------------------------------------------------------------
```

## Extended BIOS Data Area Definitions

```
ADDRESS      NAME      LENGTH  DESCRIPTION
                       (BYTES)
==========================================================================================
9000h:F733h  ScanPutPtr   1    Put Ptr (offset into ScanBuff).
------------------------------------------------------------------------------------------
9000h:F734h  ScanGetPtr   1    Get Ptr (offset into ScanBuff).
------------------------------------------------------------------------------------------
9000h:F735h  ScanBuff     33   Room for 32 scancodes (1 less than ScanBuff Size) buffer
                               for Byte scancodes that will be sent to Int 09h.
------------------------------------------------------------------------------------------
9000h:F756h  ///////      22   Reserved in HP palmtops.
------------------------------------------------------------------------------------------
9000h:F76Ch  MailBox      16   MailBox (8 words - See Int 15h, Fcn 51h).
------------------------------------------------------------------------------------------
9000h:F77Ch  AnnSave      48   Save Area for display underlying annunciators.
------------------------------------------------------------------------------------------
9000h:F7ACh  Irq2VecTbl   64   Vector table for the 16 potential IRQ2 Int sources.
                               OFFSET     IRQ2 SOURCE
                               00h        F7ACh, DCI
                               04h        F7B0h, RTC
                               08h        F7B4h, TIMER 1
                               0Ch        F7B8h, RX
                               10h        F7BCh, RING
                               14h        F7C0h, KBD
                               18h        F7C4h, IR
                               1Ch        F7C8h, Gpio[10]
                               20h        F7CCh, Gpio[11]
                               24h        F7D0h, Gpio[12]
                               28h        F7D4h, Gpio[13]
                               2Ch        F7D8h, Gpio[06]
                               30h        F7DCh, Gpio[07]
                               34h        F7E0h, Gpio[24]
                               38h        F7E4h, Gpio[25]
                               3Ch        F7E8h, Gpio[26]
------------------------------------------------------------------------------------------
9000h:F7ECh  NmiVecTbl    12   NMI vector table (GPIO[24;26] sources only)
                               OFFSET     GPIO SOURCE
                               1Ch        F7ECh, Gpio[24]
                               20h        F7F0h, Gpio[25]
                               24h        F7F4h, Gpio[26]
------------------------------------------------------------------------------------------
9000h:F7F8h  Far_DpSleep? 4    Far address of DPSLEEP?  (APM Usage)
------------------------------------------------------------------------------------------
9000h:F7FCh  Far_ShutDown 4    Far address of ShutDown (APM Usage)
------------------------------------------------------------------------------------------
9000h:F800h  BMSP         2    Bitmap Stack pointer

Values: BMSTK-10, BMSTK, .  .  .
------------------------------------------------------------------------------------------
9000h:F802h  BMSTK        70   The 7-level, 10-byte wide BitMap Stack supports variable
                               sized, RAM resident, fonts, and symbol bitmaps and grows
                               toward High Memory.
------------------------------------------------------------------------------------------
9000h:F848h  ChgCntLow    2    Charge timer (55 msec Timer1 Ticks, LSW)
------------------------------------------------------------------------------------------
9000h:F84Ah  ChgCntHi     2    Charge timer (55 msec Timer1 Ticks, MSW)
------------------------------------------------------------------------------------------
9000h:F84Ch  FntReg       2    value for Hornet's Font Control registers
                               This is constant, but hardware configuration dependent.
------------------------------------------------------------------------------------------
9000h:F84Eh  DspReg       2    value for Hornet's Display Control registers
                               This is constant, but hardware configuration dependent.
------------------------------------------------------------------------------------------
9000h:F850h  ///////      4    Reserved in HP palmtops
------------------------------------------------------------------------------------------
9000h:F854h  BAT_Start    1    The digital value corresponding to the leftmost cell
                               (cell -5) in the measurement histogram. Cells are
                               numbered: -5, -4, ...,0,...  4, 5.
------------------------------------------------------------------------------------------
9000h:F855h  BAT_Hist     11   Battery histogram with eleven -27.46 mVolt wide cells.
                               This gives a horizontal range of 0.302 Volts generally
                               centered on the first reading. In any case, the value
                               in BAT_Start gives the digital value (0-63) associated
                               with the leftmost cell (cell -5).
------------------------------------------------------------------------------------------
9000h:F860h  BAT_OutRng   1    Out-Of-Range cell for battery histogram
                               Once the 1st measurement determines placement of the
                               histogram, values out of range will be added to this bucket
------------------------------------------------------------------------------------------
9000h:F861h  GC_Rate      2    Graphics cursor display rate (55 msec units)
------------------------------------------------------------------------------------------
9000h:F863h  GC_Count     2    Counts before blink (55 msec units)
------------------------------------------------------------------------------------------
9000h:F865h  GC_Status    1    Graphics cursor status
                               BIT     DEFINITION
                                0      1 = Cursor On (Enabled)
                                1      1 = Blink Enabled
                                2      1 = Cursor State (Visible=1)
------------------------------------------------------------------------------------------
9000h:F866h  GC_X         2    Graphics cursor screen destination X coord (0-639)
------------------------------------------------------------------------------------------
9000h:F868h  GC_Y         2    Graphics cursor screen destination Y coord (0-199)
------------------------------------------------------------------------------------------
9000h:F86Ah  GC_HotspotX  1    Graphics cursor (origin) X coordinate (0-15)
                               Normally 0 (upper left corner of 16x32 cell).
------------------------------------------------------------------------------------------
9000h:F86Bh  GC_HotspotY  1    Graphics cursor (origin) Y coordinate (0-31)
                               Normally 0 (upper left corner of 16x32 cell).
------------------------------------------------------------------------------------------
9000h:F86Ch  GC_And       64   Bitmap of 16x32 cursor to be logically ANDed into
                               the display
------------------------------------------------------------------------------------------
9000h:F8ACh  GC_Xor       64   Bitmap of 16x32 cursor to be logically XORed into
                               the display
------------------------------------------------------------------------------------------
9000h:F8ECh  GC_Save      64   Stash for 16x32 display area for cursor overwrite
------------------------------------------------------------------------------------------
9000h:F92Ch  GC_Disp      64   Bitmap of 16x32 cursor to be displayed
                               This bitmap is built by the BIOS each time the cursor is
                               moved or redefined.  The value is: GC_Disp = (GC_Save
                               AND GC_And) XOR GC_Or.
------------------------------------------------------------------------------------------
9000h:F96Ch  BiosMsgBuf   650  (40x(10x13))/8 = 650 byte buffer for upper left corner of
                               display This buffer is used by BIOS for msg display.
------------------------------------------------------------------------------------------
9000h:FBF6h  ATS_LABEL    9    Label identifying key pressed (eg: Menu Label)
                               This label is used to make ATS test results more readable.
------------------------------------------------------------------------------------------
9000h:FBFFh  ATS_FLAG     1    Length of label stored at ATS_LABEL
                               The value is also used as a SysMgr/ATS handshake for
                               label processing.
------------------------------------------------------------------------------------------
9000h:FC00h  StdFontHi    1024 Last Half (Chrs 128-255) of standard byte-wide 8x8 font
                               (code page support in graphics)
------------------------------------------------------------------------------------------
```

## Compatibility Issues

### Bios Special Compatible Subroutine

The following code fragment must be present at the address indicated for compatibility with the Industry Standard.

```
F000:E00D 20 49 42        AND [BX+DI+42],CL    ;db ' ', 'I', 'B', 'M'
F000:E010 4D              DEC BP
F000:E011 C3              RET
```

### Compatibility Addresses

The table below shows the compatible entry points and data table addresses which the 100LX/200LX BIOS system supports.

Int    | ROM Entry | Type |   Function
-------|-----------|------|---------------------------------------------------------------------------------------
--     | F000:E05B | code | Reset
02     | F000:E2C3 | code | Nonmaskable Interrupt
19     | F000:E6F2 | code | Boot
--     | F000:E729 | data | Baud rate divisor table
14     | F000:E739 | code | Serial
16     | F000:E82E | code | Keyboard
09     | F000:E987 | code | Int 09h (Kbd ISR)
13     | F000:EC59 | code | Int 13h (Disk Services)
0E     | F000:EF57 | code | Disk ISR (Not used in 100LX/200LX)
--     | F000:EFC7 | data | Disk parameter table (Not used in 100LX/200LX)
17     | F000:EFD2 | code | Int 17h (Printer Services)
10     | F000:F065 | code | Int 10h (Video Services)
1D     | F000:F0A4 | data | Video parameter table
12     | F000:F841 | code | Int 12h (Memory Size)
11     | F000:F84D | code | Int 11h (Equipment Check)
15     | F000:F859 | code | Int 15h (System Services)
--     | F000:FA6E | data | 8x8 Character table (lower 128 chars only)
1A     | F000:FE6E | code | Int 1Ah (Time and Date Services)
08     | F000:FEA5 | code | Int 08h (Timer0 ISR)
--     | F000:FEF3 | data | Interrupt Vector Table
--     | F000:FF23 | data | Unexpected Interrupt Handler (handles interrupts not specifically handled by the BIOS.
--     | F000:FF53 | code | Dummy IRET
05     | F000:FF54 | code | Int 05h (Print Screen)
--     | F000:FFF0 | code | Hardware reset point
--     | F000:FFF5 | data | BIOS date stamp
--     | F000:FFFE | data | Hardware ID byte

**Notes:**

* In the HP100LX/200LX, the Int 19h (Bootstrap Load) jumps to F000h:E05Bh
* The BIOS uses the "Interrupt Vector Table" to initialize the interrupt vectors for Int 08h through Int 1Eh on power-up. Some of these are then chained by DOS.

## BIOS Messages

The following is a list of messages displayed by the BIOS. A few of these messages will not be localized to foreign languages.

### Messages which will be localized to foreign languages

* "MAIN BATTERY LOW - Press ESC"
* "BKUP BATTERY LOW - Press ESC"
* "CARD BATTERY LOW - Press ESC"
* "BATTERY CHARGING STARTED..."
* "BATTERY CHARGING STOPPED..."
* "*BATTERY ERROR* - Press ESC"
* "MAIN BATT VERY LOW - Press ESC"
* CR, LF, "Initialize RAM Disk? Enter Y or N:"
* CR, LF, "Initializing RAM Disk", CR, LF
* "Exiting Backup Mode", CR, LF
* CR, LF, "All files on drive C: will be erased!", CR, LF,
* "Continue? Enter Y or N:"
* CR, LF, "Card changed in locked port!", CR, LF, "Recovery requires warm start..."

### Messages which will not be localized

* "Laplink Remote Access (c) Copyright 1993-94",CR,LF, "Traveling Software, Inc, All Rights Reserved",CR,LF
* "Copyright (C) Intuit, Inc. 1993,1994",CR,LF
* "Copyright (C) 1991, 1994 Lotus Development Corp.", CR, LF, " All Rights Reserved",CR,LF
* "Copyright (C) Hewlett-Packard 1990,1994", CR, LF
* "Copyright 1984, 1985 Phoenix Software Associates Ltd", CR,LF
* "Version XXXXXXXX",CR,LF
