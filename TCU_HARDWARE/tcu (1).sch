EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:tcu-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATmega32U4 U?
U 1 1 54A36652
P 5200 3650
F 0 "U?" H 4300 5350 60  0000 C CNN
F 1 "ATmega32U4" H 4550 2100 60  0000 C CNN
F 2 "" H 6400 4750 60  0000 C CNN
F 3 "" H 6400 4750 60  0000 C CNN
	1    5200 3650
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X03 P?
U 1 1 54A366D3
P 4450 6900
F 0 "P?" H 4450 7100 50  0000 C CNN
F 1 "CONN_02X03" H 4450 6700 50  0000 C CNN
F 2 "" H 4450 5700 60  0000 C CNN
F 3 "" H 4450 5700 60  0000 C CNN
	1    4450 6900
	1    0    0    -1  
$EndComp
Text Label 3600 2800 0    60   ~ 0
UVCC
Text Label 3600 2100 0    60   ~ 0
UVCC
Wire Wire Line
	3600 2800 3950 2800
Wire Wire Line
	3950 2100 3600 2100
Wire Wire Line
	3950 2200 3600 2200
Wire Wire Line
	3950 2600 3850 2600
Wire Wire Line
	3850 2600 3850 2200
Connection ~ 3850 2200
Wire Wire Line
	3950 2400 3850 2400
Connection ~ 3850 2400
Wire Wire Line
	3950 2300 3850 2300
Connection ~ 3850 2300
Text Label 3600 2200 0    60   ~ 0
VCC
Wire Wire Line
	3950 2900 3600 2900
Wire Wire Line
	3950 3000 3600 3000
Wire Wire Line
	3300 3100 3950 3100
Wire Wire Line
	3950 3200 3600 3200
Wire Wire Line
	3600 3200 3600 3250
Wire Wire Line
	3950 4700 3900 4700
Wire Wire Line
	3900 4700 3900 5100
Wire Wire Line
	3950 5000 3900 5000
Connection ~ 3900 5000
Wire Wire Line
	3950 4900 3900 4900
Connection ~ 3900 4900
Wire Wire Line
	3950 4800 3900 4800
Connection ~ 3900 4800
$Comp
L GND #PWR?
U 1 1 54ACA6C8
P 3600 3250
F 0 "#PWR?" H 3600 3250 30  0001 C CNN
F 1 "GND" H 3600 3180 30  0001 C CNN
F 2 "" H 3600 3250 60  0000 C CNN
F 3 "" H 3600 3250 60  0000 C CNN
	1    3600 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 54ACA6E6
P 3900 5100
F 0 "#PWR?" H 3900 5100 30  0001 C CNN
F 1 "GND" H 3900 5030 30  0001 C CNN
F 2 "" H 3900 5100 60  0000 C CNN
F 3 "" H 3900 5100 60  0000 C CNN
	1    3900 5100
	1    0    0    -1  
$EndComp
Text Label 3600 2900 0    60   ~ 0
D+
Text Label 3600 3000 0    60   ~ 0
D-
$EndSCHEMATC
