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

VERSION		= "201809180"


$addr		= 0x00000000;
$datalen	= 32
$strHeader	= nil
#strHeader	= "data"
$fNoend		= false
$addrStart	= 0x00000000
$addrExec	= 0x00000000

def GetParam
  opt = OptionParser.new

  opt.on('--noend')          { |v| $fNoend		= true}
  opt.on('--header=VALUE')   { |v| $strHeader		= v}
  opt.on('--start-addr=VAL') { |v| $addrStart		= v.hex}
  opt.on('--start-exec=VAL') { |v| $addrExec		= v.hex}
  opt.on('-v')               { print VERSION+"\n"; exit }

  opt.parse!(ARGV)

  rescue
#  ShowHelp()
  exit(1)
end


def Output(num, addr, str)
  sum  = (addr >>  8) & 0xff
  sum +=  addr        & 0xff
  if(num == 0 || num == 1 || num == 9) then
    lenCtrl = 1+2
    format = "S%d%02X%04X"
  elsif(num == 2 || num == 8) then
    lenCtrl = 1+3
    format = "S%d%02X%06X"
    sum += (addr >> 16) & 0xff
  else  #if(num == 3 || num == 7) then
    lenCtrl = 1+4
    format = "S%d%02X%08X"
    sum += (addr >> 24) & 0xff
    sum += (addr >> 16) & 0xff
  end
  sum += str.length+lenCtrl
  printf(format, num, str.length+lenCtrl, addr);

  str.length.times do |i|
    c = str[i]
    printf("%02X", c.ord)
    sum += c.ord
  end
  addr += str.length

  printf("%02X\n", ~sum & 0xff)
end


def PrintVersion
  print $version
end


def main
  #STDIN.binmode

  GetParam()

  if($strHeader != nil) then
    output(0, 0x00000000, $strHeader)
  end

  $addr = $addrStart;
  while str = STDIN.read($datalen)
    Output(3, $addr, str)
    $addr += $datalen
  end

  if($fNoend != true) then
    Output(7, $addrExec, "")
  end
end

main()
