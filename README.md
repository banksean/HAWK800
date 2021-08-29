# HAWK800 Controller

Hardware MIDI controller for the Korg EX-800 with the [HAWK-800 mod](https://www.hawk800.com/) installed.

## Background

The HAWK-800 adds a bunch of new MIDI CC messages to control various 
parameters in realtime, but they way it implements these messages
makes it difficult to use a typical generic MIDI controller's CC support.

There are three different methods of sending CC messages that it will
accept, but the simplest for our purposes is one that sends two
CC messages per paraemter. The first CC message specifies the parameter
you want to change, and the second CC message sets the value for that
parameter.

## Problem

I haven't found a cheap hardware controller that can send multiple CC messages
per knob wiggle.  Maybe there is one, IDK. 

The purpose of this project is to provide a hardware interface that
sends CC messages using this HAWK-800 specific method.

## Hardware

I built a prototype using an Arduino UNO, a cheapo MIDI shield, six
potentiometers (B10k I think), a cardboard box and some knobs.

This repo contains the code for the Arduino part.

## Connections

It works sort of like a hardware MIDI proxy. Instead of plugging your
controller keyboard's MIDI OUT directly into the EX-800's MIDI IN, 
plug this box in between the two.

Keyboard MIDI OUT -> MIDI IN on this box, MIDI OUT -> MIDI IN on EX-800

Then you can play the keys on a proper keybard controller, and use this
arduino box to wiggle the HAWK-800 parameters at the same time.

## Rando bits that might be useful for other projects.

I slapped together a [ccmsgs.csv](./ccmsgs.csv) file that lists all of the
parameters and the control number for each in a machine-readable format. 
I got tired of looking this stuff up in the HAWK800 pdf files so I
automated that task with some code-gen.

To generate a .h file for all of the CC control numbers:

```
go run src/go/gen.go > whatever.h
```

This reads the ccmsgs.csv file and generates .h for named symbols. This
is how I generated the CCMessages.h file in the Arduino project.

