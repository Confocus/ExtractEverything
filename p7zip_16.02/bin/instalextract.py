#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import ctypes
import sys

class InstallerExtractor(object):
	def __init__(self):
		abs_dir = os.path.dirname(os.path.abspath(__file__))
		self.abs_dir = abs_dir
		self.lib_path = os.path.join(self.abs_dir, "instalextract.so")
		self.lib_ext = ctypes.cdll.LoadLibrary(self.lib_path)
		
		
class PackInfo(ctypes.Structure):
	_fields_ = [
		("type", ctypes.c_int),
		("success", ctypes.c_bool)
	]

if __name__ == '__main__':
	installer_extractor = InstallerExtractor()
	installer_extractor.lib_ext.InstallerTest()

	pack_info = PackInfo()
	target_path = ctypes.c_char_p()
	target_path.value = "/data/wangteng/test/34b336ce63fdf6316784a27ee446d0c1"
	instaltype = ctypes.c_int(-1)
	output_path = ctypes.c_char_p()
	output_path.value = "/data/wangteng/extracted"
	#target_path = bytes("/data/wangteng/test")
	installer_extractor.lib_ext.InstallerTypeDetect(target_path, ctypes.pointer(instaltype))
	print instaltype.value
	#print pack_info.type
	#print pack_info.success
	#print target_path.value
	installer_extractor.lib_ext.InstallerExtract(target_path, output_path, ctypes.byref(pack_info))
	print pack_info.type
	print pack_info.success
