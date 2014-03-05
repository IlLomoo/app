/* This file is automatically generated.  DO NOT EDIT!
   Instead, edit gen-tld-tables.pl and re-run.  */

#include "idn_framework.h"




#include "tld.h"

/* TLD fr */
static const Tld_table_element _tld_fr_valid[] =
{
  { 0x2d,  0x2d },
  { 0x30,  0x39 },
  { 0x61,  0x7a },
  { 0xe0,  0xe0 },
  { 0xe2,  0xe2 },
  { 0xe6,  0xeb },
  { 0xee,  0xef },
  { 0xf4,  0xf4 },
  { 0xf9,  0xf9 },
  { 0xfb,  0xfc },
  { 0xff,  0xff },
  { 0x153,  0x153 }
};

static const Tld_table _tld_fr =
{
  "fr",
  "1.0",
  12,
  &_tld_fr_valid[0]
};

/* TLD no */
static const Tld_table_element _tld_no_valid[] =
{
  { 0xe0,  0xe1 },
  { 0xe4,  0xea },
  { 0xf1,  0xf4 },
  { 0xf6,  0xf6 },
  { 0xf8,  0xf8 },
  { 0xfc,  0xfc },
  { 0x10d,  0x10d },
  { 0x111,  0x111 },
  { 0x144,  0x144 },
  { 0x14b,  0x14b },
  { 0x161,  0x161 },
  { 0x167,  0x167 },
  { 0x17e,  0x17e }
};

static const Tld_table _tld_no =
{
  "no",
  "1.0",
  13,
  &_tld_no_valid[0]
};

/* Main array */
const Tld_table * _tld_tables[] =
{
  &_tld_fr,
  &_tld_no,
  NULL
};
