/*
    This file is part of RoboJournal.
    Copyright (c) 2013 by Will Kraft <pwizard@gmail.com>.
    MADE IN USA

    RoboJournal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RoboJournal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with RoboJournal.  If not, see <http://www.gnu.org/licenses/>.
  */

/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Hunspell, based on MySpell.
 *
 * The Initial Developers of the Original Code are
 * Kevin Hendricks (MySpell) and Laszlo Nemeth (Hunspell).
 * Portions created by the Initial Developers are Copyright (C) 2002-2005
 * the Initial Developers. All Rights Reserved.
 *
 * Contributor(s):
 * David Einstein
 * Davide Prina
 * Giuseppe Modugno
 * Gianluca Turconi
 * Simon Brouwer
 * Noll Janos
 * Biro Arpad
 * Goldman Eleonora
 * Sarlos Tamas
 * Bencsath Boldizsar
 * Halacsy Peter
 * Dvornik Laszlo
 * Gefferth Andras
 * Nagy Viktor
 * Varga Daniel
 * Chris Halls
 * Rene Engelhard
 * Bram Moolenaar
 * Dafydd Jones
 * Harri Pitkanen
 * Andras Timar
 * Tor Lillqvist
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/* hunzip: file decompression for sorted dictionaries with optional encryption,
 * algorithm: prefix-suffix encoding and 16-bit Huffman encoding */

#ifndef _HUNZIP_HXX_
#define _HUNZIP_HXX_

#define BUFSIZE  65536
#define HZIP_EXTENSION ".hz"

#define MSG_OPEN   "error: %s: cannot open\n"
#define MSG_FORMAT "error: %s: not in hzip format\n"
#define MSG_MEMORY "error: %s: missing memory\n"
#define MSG_KEY    "error: %s: missing or bad password\n"

struct bit {
    unsigned char c[2];
    int v[2];
};

class Hunzip
{

protected:
    char * filename;
    FILE * fin;
    int bufsiz, lastbit, inc, inbits, outc;
    struct bit * dec;        // code table
    char in[BUFSIZE];        // input buffer
    char out[BUFSIZE + 1];   // Huffman-decoded buffer
    char line[BUFSIZE + 50]; // decoded line
    int getcode(const char * key);
    int getbuf();
    int fail(const char * err, const char * par);
    
public:   
    Hunzip(const char * filename, const char * key = NULL);
    ~Hunzip();
    const char * getline();
};

#endif
