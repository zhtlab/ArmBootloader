#!/usr/bin/ruby
#
# Copyright (c) 2010-2018 zhtlab
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#


require         'rubygems'
require         'optparse'

VERSION         = "201809180"

$infile         = nil;
$d              = STDIN
$offset         = 0;
$offsetSum      = 0;
$transferedCnt  = 0;


def ShowHelp
  print "srec address converter (S1,S2 are converted to S3)\n";
  print "  options:\n";
  print "    --off=[offset]:  offset\n";
  print "    -s, --noend:     strip end of file\n";
  print "    -v:              version\n";
  print "    -h:              help\n";
  exit(0);
end


def GetParam
  opt = OptionParser.new

  opt.on('-f VAL')           { |v| $infile      = v}
  opt.on('--noend')          { |v| $fNoend      = true}
  opt.on('-s')               { |v| $fNoend      = true}
  opt.on('--off=VALUE')      { |v| $offset      = v.hex}
  opt.on('-v')               { print VERSION+"\n"; exit }
  opt.on('-h')               { ShowHelp(); exit }

  opt.parse!(ARGV)

  rescue
    ShowHelp()
  exit(1)
end


def Output(mode, len, addr, data, sum)
  if(mode >= 1 && mode <= 3)
    addr += $offset
    sum  -= $offsetSum
    sum  &= 0xff
  end
#  printf("%d %02X %08X [%s] %02X\n", mode, len, addr, data, sum)
  printf("S%d%02X%08X%s%02X\n", mode, len, addr, data, sum)
end


def Convert(str)
  str.match(/S(.)(..)(........)(.*)(..)/)
  mode=$1.to_i
  len =$2.hex
  addr=$3.hex
  data=$4
  sum =$5.hex

  if(mode == 0)
    print "#{str}\n";
  elsif(mode == 1)
    len  += 2;
    mode  = 3;
    addr += $transferedCnt << 16
    Output(mode, len, addr, data, sum)
  elsif(mode == 2)
    len  += 1;
    mode  = 3;
    addr += $transferedCnt << 8
    Output(mode, len, addr, data, sum)
  elsif(mode == 3)
    Output(mode, len, addr, data, sum)
#  elsif(mode >= 7)
#    print "S70500000000FA\n"
  end
end


def main

  GetParam()

  if($infile != nil)
    $d = File.open($infile, "r")
  end

  $offsetSum  = ($offset >> 24) & 0xff
  $offsetSum += ($offset >> 16) & 0xff
  $offsetSum += ($offset >>  8) & 0xff
  $offsetSum += ($offset      ) & 0xff

  $d.each_line do |str|
    str.chomp!
    Convert(str)
  end

  if($fNoend != true) then
    Output(7, 5, 0, "", 0xfa);
  end
end

main()
