# ncaster [![Build Status](https://travis-ci.org/io12/raycaster.svg?branch=master)](https://travis-ci.org/io12/raycaster)

just a basic raycaster in ncurses

![screenshot](https://raw.githubusercontent.com/io12/raycaster/master/screenshot.png)

This project is dead. Don't expect updates.

## Usage

Read a map from a file

`./ncaster.out [file]`

Generate a random [colored] map

`./ncaster.out -r[c]`

Write a random [colored] map to a file

`./ncaster.out -r[c]o [file]`

## Keybindings

Arrow keys to move and turn

`q`: quit

`x`: save and quit (not added yet)

`1`: increase the FOV

`2`: decrease the FOV

`3`: toggle the HUD

`4`: toggle the cross-hairs

`5`: toggle the lantern

## Map file syntax

In addition to the explanation below, you can look at `example.map` to see an example of a map file.

### First line

The first line must contain two doubles to store the starting player coordinates.
These doubles must be separated by a non-digit, non-period, non-newline character.

### Map cells

The following lines are just a two dimensional array of digits from 0-7.
These digits represent cells on the map and their value represents their state.
The meaning of the values of the cells is listed below.

#### Map cell states

`0`: empty, no wall

`1`: white wall

`2`: red wall

`3`: green wall

`4`: yellow wall

`5`: blue wall

`6`: magenta wall

`7`: cyan wall

Notice how the colors resemble the color palette of a 3 bit color terminal.
