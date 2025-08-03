# Beer Tracker

This repository contains the PCB, Enclosure and Code for the Beer Tracker.

The Beer Tracker allows a group of people to keep track of
various things (mainly beer).

## What does it do?

Each person gets a tracker. If a person logs a drink then the
total amount will be synced between all peers (if in reach).
If a peer is out of reach, it will be synced later. (It can
also sync via multiple hops: eg A syncs data to B and B syncs
As data to C).

The device also allows the user to see the current counts of
the other peers.

## Why?

This is a really good question...

It all started during a music festival, where we tracked our
beer consumption with a tally sheet. This worked great, as
long as we all are together in a group. But some people have
different music taste and hence the group was split and the
tracking failed. This major problem needed to be addressed. A
smart way to solve it would be to just you a tracking app, or
to write our own app if we are really fancy. But those
solutions aren't enough overengineered!!!. Hence we created
this project.

## Structure

- `code` - source code for the ESP32
- `enclosure` - STL files and FreeCAD files of the case
- `kicad` - schematic + pcb layout


## Images

TODO
