# 4\. Working with BIOS

## Battery Check

### Batteries

There are three DC power sources that may be present in the HP palmtop:

* Main battery.
* Backup battery.
* External power, such as the HP AC to 12V DC adapter.

#### General Information

The main or system battery consists of two 1.5 volt batteries in series. These batteries may be either NiCad or Alkaline, and feed the HP palmtop power supply in normal operation. The low main battery warning condition is nominally 2.2 (2.3 for NiCad) Volts. When the level reaches about 1.8 volts, the power supply can no longer reliably operate off of the main batteries, and an NMI Low Power Interrupt triggers a transition to Backup mode.

In Backup mode the system is shut down to conserve power and maintain RAM integrity. The power supply is supported in this state by the backup battery until it also is depleted. At this point RAM integrity will be compromised. Recovery from Backup mode (*) will always result in a warmstart. This means that the system memory is reset (but not the RAM Disk) and some of the work may be lost.

Because it is desirable to avoid warmstart, there is an intermediate state between the first low battery warnings and the time of the Low Power NMI that MAY be entered. This state, called Software Shutdown, will be entered when the battery voltage reaches 2.0 Volts (NiCad or Alkaline). In this state the unit will be shut down, and attempts to wake it up will result in a pair of hi pitched beeps. Recovery from this state (*) will not result in a warmstart provided that System Memory still appears to have integrity. It should be noted that Software Shutdown is not triggered by hardware, but is instead the result of periodic battery measurements that take place every 15 seconds or so. Abrupt power transitions such as loss of battery contact, or very heavy loads being applied may result in a direct transition from a low battery warning state to BackUp Mode without ever entering the Software Shutdown state.

Plug-in cards may (depending on their type) require a battery of their own. The HP palmtop power supply may also be fed directly from a plug-in AC adaptor. In this mode, the HP palmtop's power supply is supported completely by the adaptor - the main batteries, if present, are isolated and not used. HP palmtops also support in-unit charging of NiCad main batteries from the same AC adaptor.

(*) - By inserting the AC adaptor or installing fresh main batteries.

#### Some Limitations

The HP palmtop BIOS cannot differentiate between NiCad and Alkaline batteries; this information must be supplied to the BIOS by the application. Battery Voltage, especially NiCad, is not always a good indication of remaining battery capacity. There is no attempt to compensate for the internal resistance of the main batteries. As a result, the measured voltage of the main batteries will be slightly higher when the AC adaptor is plugged in (no load on the batteries).

### Battery Measurement

HP palmtop hardware provides a 3-channel 6-bit DAC and a comparator. This provides the means to implement a 6-bit, 3 channel ADC through software successive approximation. The three input channels are dedicated to the Main, Backup, and Reference Voltage sources. The Reference voltage measurement is used by the software to adjust the main and backup battery measurements. Periodic main, backup, and card battery measurements are scheduled during normal operation using Timer1 (IRQ2). BIOS Power services provides applications the ability to measure the Main and BackUp batteries, enable battery charging, etc. The measurement of plug-in card batteries is direct (doesn't use the ADC).

### Recharge

In-unit charging of NiCad main batteries is supported through the AC adaptor. Charging commences when three conditions are met:

* The adaptor is plugged in.
* Charging is enabled.
* NiCad batteries are specified.

Charging stops when any one of the conditions becomes false or if the hardware detects overvoltage. Overvoltage can sometimes occur if an attempt is made to charge Alkaline batteries. Charging occurs at the "fast" rate for the first six hours, followed by charging at the "slow" rate thereafter (until stopped). The transition from fast to SLOW charging occurs quietly without a BIOS message.

### Battery Type

It will be necessary for the application to specify (through a BIOS service) the type of the installed batteries (NiCad or Alkaline; the default is Alkaline). This is necessary for 2 reasons:

1. The HP palmtop should ever be used to recharge Alkaline batteries.
2. NiCad and Alkaline batteries have quite different characteristics.

Reliable end-of-battery-life processing (Warnings, Software Shutdown, etc.) depend upon knowing the type of main batteries installed.

### Battery Messages

The battery related BIOS messages are shown below:

* MAIN BATTERY LOW - Press ESC.
* MAIN BATT VERY LOW - Press ESC.
* BKUP BATTERY LOW - Press ESC.
* CARD BATTERY LOW - Press ESC.
* BATTERY CHARGING STARTED ...
* BATTERY CHARGING STOPPED ...
* BATTERY ERROR - Press ESC.

The BIOS software provides some "hysteresis" in the display of the low battery messages to avoid the appearance of unreliability for battery levels near the thresholds. This means that once a low battery condition is detected, a significant voltage recovery is required to make the condition go away. The message(s) are displayed briefly, then the original contents of display memory are restored. The battery warnings will continue at the periodic measurement rate (15 seconds) until fresh batteries are installed, or the message is acknowledged by pressing the [ESC] key while the message is in the display. If acknowledged, subsequent warnings will be suppressed until the unit is awakened from deep sleep, or reset. The accompanying warning beep will only happen on the first display of the message, but will reoccur if there is any keyboard activity.

The charging messages will appear to confirm charging transitions. These messages will appear only once, and will appear briefly in the display in the same manner as the low battery warnings. As with the low battery warnings, acknowledging the message by pressing the [ESC] key will cause the underlying display data to be restored more quickly.

The Battery Error message occurs as the result of detection of overvoltage. It will occur if an attempt is made to recharge the main batteries when there are no batteries installed. It may also occur if an attempt is made to recharge Alkaline batteries. If this message occurs, charging will immediately be disabled. The message will continue to appear at the periodic battery measurement rate until acknowledged by pressing the [ESC] key.

## Power Management

The HP palmtop has several features to extend battery life and maintain system RAM. For a discussion of the features designed to protect system RAM integrity, see "Battery Check" on [page 4-142](#battery-check). There are three states that the HP palmtop enters regularly during normal operation:

* Run state (CPU running, display on).
* Light sleep state (CPU idle, display on).
* Deep sleep state (CPU idle, display off, other devices off).

In the run state the unit is doing useful work. This may or may not result in display changes. For example, the unit will typically be placed in the run state 18 or more times each second due to Timer0 Interrupts without any noticeable effect in the display.

Most units that are turned on, stay in the light sleep state a good percentage of time. When in this state, the unit will use less power than in the run state. This state is entered from the BIOS Int 15h, Function 41xxh (Wait-For-Event) service. Some of the Int 16h services invoke the Wait-For-Event function and will enter the light sleep state. TIMER0, TIMER1, KBD, and RTC Interrupts are typical events that cause transition from the light sleep to the run state.

When a unit is placed in the deep sleep state it will use significantly less power than one in the run or light sleep states. When the unit is placed in this state the CPU is reset (register contents lost). When "awakened" from this state, execution starts at the reset vector. Under normal circumstances the BIOS makes this transparent to the application, and execution is eventually returned to the code that caused the deep sleep state to be entered. The deep sleep state may be entered in one of several ways:

* Power Off.
* Keyboard turn-off ([ON] key detected inside Int 16h).
* Auto-timeout (3 minutes with no activity).
* Int 15h, Function 42h BIOS call.
* Software Shutdown (low batteries).
* Backup mode (NMI Interrupt, very low power response)

### Normal Power Up Behavior

The code that handles power on is accessed by the reset vector (0FFFFh:0000h). This is invoked when the ON key is pressed (and other deep sleep wakeup sources) while the machine is off.

### Warmstart Behavior

Warmstart is normally invoked by [CTRL][ALT][DEL]. It is also invoked if the user RAM is found to be corrupted during normal power on initialization. It initializes user RAM (but not the built-in or plug-in RAM Disks), then invokes Int 19h bootstrap loader.

### Coldstart Behavior

Coldstart is invoked by [LSHFT][CTRL][ON] or, if the built-in RAM disk is found to be corrupted during normal power on. It initializes the user RAM and built-in RAM disk, but not the plug-in RAM disk. The user is prompted to specify whether or not to reformat the built-in RAM disk. After all initializations are done, coldstart invokes Int 19h bootstrap loader.

### Deep Sleep Wakeup Sources

The RTC, [ON] key, RX, and RING Interrupts are typical events that will "awaken" a unit from deep sleep, causing a transition to the RUN State. The CPU will not begin processing instructions for between 35 and 44 milliseconds after the actual wakeup event occurs. For serial RX wakeups at any reasonable baud rate, data will surely be lost due to overrun errors. In addition, the software requires about 750 microseconds to arrive at the code that begins to monitor the serial port. These delays must be considered for serial code designed to be capable of waking up the HP palmtop from deep sleep. A true serial RING wakeup from the deep sleep state requires RING to be asserted for about 0.4 seconds. A genuine RX wakeup requires four valid characters to be received before two data errors are detected. Note that one of these data errors will almost certainly be the guaranteed overrun error.

### Normal Deep Sleep Entry

Under normal circumstances the deep sleep state is entered in a systematic manner which includes steps to ensure system integrity when the unit is awakened and placed in the run state. For example, checksums are computed for portions of user RAM, hardware registers, and CPU Registers. These values are checked for integrity when the unit is awakened, and assuming valid, the unit will eventually continue execution from the code that requested the deep sleep. The events which require the deep sleep state to be entered do not invoke the deep sleep code directly (exception NMI Low Power). Instead, these events "request" entry to the deep sleep state. Operating System code responsible for plug-in card management detects the request (TIMER0 tick) and causes the deep sleep state to be entered after orderly shutdown of plug-in card operations.

### NMI Deep Sleep Entry

The entry into deep sleep due to the NMI Low Power interrupt is an exception to the orderly process described above. In this case there is very little timing margin available to get the unit turned off before RAM integrity is compromised. There is insufficient time to compute checksums, save the CPU registers, wait for a Timer0 tick, etc. The unit is placed in the deep sleep state by the Low Power handler as quickly as possible. If the NMI Low Power Interrupt does occur, the deep sleep wakeup response (after installation of a fresh set of batteries or the AC adaptor) is a Warmstart. Plug-in card access can utilize considerable power (depending on card type). Because it is desirable to avoid WarmStart, special steps are taken by the plug-in card management code in an effort to avoid the NMI Low Power Interrupt during card access. If the main batteries are marginal, these steps can result in the unit being placed immediately in the software shutdown state.

### Software Shutdown Deep Sleep Entry

The entry into deep sleep due to software shutdown is similar to that due to normal turn off. However, normal wakeup is inhibited until a fresh set of batteries or the AC adaptor has been installed.

## BIOS ID Block

The BIOS ROM contains a block of information which encodes the identification of the machine, and indicates the date the BIOS was created. The BIOS ID Block begins at FFFF:5.

| Address | Description                                      |
|---------|--------------------------------------------------|
| FFFF:5  | Date of BIOS release (format: MM/DD/YY; 8 bytes) |
| FFFF:D  | 00h (Reserved)                                   |
| FFFF:E  | FEh (System model ID)                            |
| FFFF:F  | 00h (CkSum - Unused)                             |

## Keyboard Overview

In a PC with hardware keyscan support, typically the Int 09h routine is invoked directly by the hardware to process (translate) the scan code that it has produced in response to the "Make" or "Break" keyboard interrupt. The HP palmtop does not have this level of hardware support for the keyboard. Instead much of the work is performed by software. The goal is to make the lack of this resource transparent to applications that run on HP palmtop.

In the HP palmtop, the hardware provides the first keyboard "Make" IRQ2 interrupt that is serviced by Int 0Ah (Hornet Specific Interrupts). Keyboard "Breaks" or subsequent key presses while 1 or more keys are depressed do not produce an interrupt (except the hardware provides an interrupt on all keys up). The HP palmtop BIOS deals with this situation by scheduling periodic keyboard scans (TIMER1: Int 0Ah interrupts) to detect changes in the keyboard state. This cooperative effort between the KBD and TIMER1 interrupts eventually (key debounce, scan code creation, et al.) produces a produces a 1-byte scan code that is placed in I/O port 60h. A hardware Int 09h is then invoked by software writing a bit in a specific hardware register. This completes the software emulation process for hardware keyscans.

It is now the responsibility of the Int 09h code to take the scancode and produce something meaningful to the application (e.g.; ASCII KeyCode), update necessary keyboard information (that is, shift status), or convert certain scan codes into ISR internal functions (e.g.; [CTL] [ALT] [DEL]). When KeyCodes are produced, they are in a 2-byte format and placed in a a 16-word key buffer. This provides the interface to the services provided by Int 16h.

The application's interface to the keyboard is often at the Int 16h level. The services provided by the Int 16h functions permit the application to determine if a key is available (ready for processing), remove the keycode from the key buffer, write a key code into the key buffer, etc. In addition, many of the functions feature a low power mode (light sleep) while waiting for an event.

In summary, the keyboard "players" are

* Int 0Ah: KBD Interrupt: Initial key "Make" Interrupt.
* Int 0Ah: TIMER1 Interrupt: Keyboard Scans/Debounce/Create ScanCode/Invoke Int 09h
* Int 09h: Invoked by Int 0Ah to process ScanCodes.
* Int 16h: Provides keyboard services at the application level.

More detail on the players is provided in "Int 09h: Keyboard Translate Interrupt" on [page 3-31](ch3#Int-09h:-keyboard-translate-interrupt).
