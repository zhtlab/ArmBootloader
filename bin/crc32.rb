#!/usr/bin/ruby
# -*- coding: utf-8 -*-
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

require         "zlib"
require         'rubygems'
require         'optparse'

VERSION        = "201809180"
MAX_BUF_SIZE   = 8192

$fBin          = false;
$fEndianBig    = false;
$fEndianLittle = false;


def GetParam
  opt = OptionParser.new

  opt.on('--bin')          { |v| $fBin          = true}
  opt.on('--big')          { |v| $fEndianBig    = true}
  opt.on('--little')       { |v| $fEndianLittle = true}
  opt.on('-v')             { print VERSION+"\n"; exit }

  opt.parse!(ARGV)

  rescue
#  ShowHelp()
  exit(1)
end


def GetCrc(filename)
  ret_crc32 = 0

  buf = [""].pack('a8192')
  File.open( filename, 'rb' ) do |fp|
    while fp.read(MAX_BUF_SIZE, buf)
      ret_crc32 = Zlib.crc32(buf, ret_crc32)
    end
  end

  ret_crc32
end


def main
  filename = ARGV[0]

  GetParam()
  crc = GetCrc(filename)

  if($fBin == true) then
    if($fEndianBig == true) then
      putc((crc >> 24) & 0xff)
      putc((crc >> 16) & 0xff)
      putc((crc >>  8) & 0xff)
      putc((crc >>  0) & 0xff)
    else
      putc((crc >>  0) & 0xff)
      putc((crc >>  8) & 0xff)
      putc((crc >> 16) & 0xff)
      putc((crc >> 24) & 0xff)
    end

  else
    printf("%08x", crc)
  end
end

main()
