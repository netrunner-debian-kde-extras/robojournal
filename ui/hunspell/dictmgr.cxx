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

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>

#include "ui/hunspell/dictmgr.hxx"

using namespace std;

DictMgr::DictMgr(const char * dictpath, const char * etype) 
{
  // load list of etype entries
  numdict = 0;
  pdentry = (dictentry *)malloc(MAXDICTIONARIES*sizeof(struct dictentry));
  if (pdentry) {
     if (parse_file(dictpath, etype)) {
        numdict = 0;
        // no dictionary.lst found is okay
     }
  } else {
     numdict = 0;
  }
}


DictMgr::~DictMgr() 
{
  dictentry * pdict = NULL;
  if (pdentry) {
     pdict = pdentry;
     for (int i=0;i<numdict;i++) {
        if (pdict->lang) {
            free(pdict->lang);
            pdict->lang = NULL;
        }
        if (pdict->region) {
            free(pdict->region);
            pdict->region=NULL;
        }
        if (pdict->filename) {
            free(pdict->filename);
            pdict->filename = NULL;
        }
        pdict++;
     }
     free(pdentry);
     pdentry = NULL;
     pdict = NULL;
  }
  numdict = 0;
}


// read in list of etype entries and build up structure to describe them
int  DictMgr::parse_file(const char * dictpath, const char * etype)
{

    int i;
    char line[MAXDICTENTRYLEN+1];
    dictentry * pdict = pdentry;

    // open the dictionary list file
    FILE * dictlst;
    dictlst = fopen(dictpath,"r");
    if (!dictlst) {
      return 1;
    }

    // step one is to parse the dictionary list building up the 
    // descriptive structures

    // read in each line ignoring any that dont start with etype
    while (fgets(line,MAXDICTENTRYLEN,dictlst)) {
       mychomp(line);

       /* parse in a dictionary entry */
       if (strncmp(line,etype,4) == 0) {
          if (numdict < MAXDICTIONARIES) {
             char * tp = line;
             char * piece;
             i = 0;
             while ((piece=mystrsep(&tp,' '))) {
                if (*piece != '\0') {
                    switch(i) {
                       case 0: break;
                       case 1: pdict->lang = mystrdup(piece); break;
                       case 2: if (strcmp (piece, "ANY") == 0)
                                 pdict->region = mystrdup("");
                               else
                                 pdict->region = mystrdup(piece);
                               break;
                       case 3: pdict->filename = mystrdup(piece); break;
                       default: break;
                    }
                    i++;
                }
                free(piece);
             }
             if (i == 4) {
                 numdict++;
                 pdict++;
             } else {
                 fprintf(stderr,"dictionary list corruption in line \"%s\"\n",line);
                 fflush(stderr);
             }
          }
       }
    }
    fclose(dictlst);
    return 0;
}

// return text encoding of dictionary
int DictMgr::get_list(dictentry ** ppentry)
{
  *ppentry = pdentry;
  return numdict;
}



// strip strings into token based on single char delimiter
// acts like strsep() but only uses a delim char and not 
// a delim string

char * DictMgr::mystrsep(char ** stringp, const char delim)
{
  char * rv = NULL;
  char * mp = *stringp;
  int n = strlen(mp);
  if (n > 0) {
     char * dp = (char *)memchr(mp,(int)((unsigned char)delim),n);
     if (dp) {
        *stringp = dp+1;
        int nc = (int)((unsigned long)dp - (unsigned long)mp); 
        rv = (char *) malloc(nc+1);
        if (rv) {
           memcpy(rv,mp,nc);
           *(rv+nc) = '\0';
           return rv;
        }
     } else {
       rv = (char *) malloc(n+1);
       if (rv) {
          memcpy(rv, mp, n);
          *(rv+n) = '\0';
          *stringp = mp + n;
          return rv;
       }
     }
  }
  return NULL;
}


// replaces strdup with ansi version
char * DictMgr::mystrdup(const char * s)
{
  char * d = NULL;
  if (s) {
     int sl = strlen(s);
     d = (char *) malloc(((sl+1) * sizeof(char)));
     if (d) memcpy(d,s,((sl+1)*sizeof(char)));
  }
  return d;
}


// remove cross-platform text line end characters
void DictMgr:: mychomp(char * s)
{
  int k = strlen(s);
  if ((k > 0) && ((*(s+k-1)=='\r') || (*(s+k-1)=='\n'))) *(s+k-1) = '\0';
  if ((k > 1) && (*(s+k-2) == '\r')) *(s+k-2) = '\0';
}

