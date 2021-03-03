#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import ctypes
import sys


class PackInfo(ctypes.Structure):
    _fields_ = [
        ("pkg_type", ctypes.c_int),
        ("is_success", ctypes.c_bool)
    ]


class InstallerExtractor(object):
    def __init__(self):
        abs_dir = os.path.dirname(os.path.abspath(__file__))
        self.abs_dir = abs_dir
        self.lib_path = os.path.join(self.abs_dir, "instalextract.so")
        self.lib_ext = ctypes.cdll.LoadLibrary(self.lib_path)

    def get_comress_type(self, pkg_path):
        pack_info = PackInfo()
        target_path = ctypes.c_char_p()
        target_path.value = pkg_path
        self.lib_ext.InstallerTypeDetect(target_path, ctypes.byref(pack_info))
        return pack_info
        # return pack_info.is_success, pack_info.pkg_type

    def extract_pkg(self, pkg_path, pack_info):
        target_path = ctypes.c_char_p()
        target_path.value = pkg_path
        self.lib_ext.InstallerExtract(target_path, "./", ctypes.byref(pack_info))


if __name__ == '__main__':
    #pak_path = "/root/lizewen/sample-file/ac.zip"
    pak_path = sys.argv[1]
    tool = InstallerExtractor()
    pack_info = tool.get_comress_type(pak_path)
    #_success, pkg_type = tool.get_comress_type(pak_path)
    print pack_info.is_success, pack_info.pkg_type
    tool.extract_pkg(pak_path,pack_info)
    # installer_extractor = InstallerExtractor()
	# installer_extractor.lib_ext.InstallerTest()
    #
	# pack_info = PackInfo()
	# target_path = ctypes.c_char_p()
	# target_path.value = "/data/wangteng/test1/003259ce4bfedc3f58f9be0dfdc9f65e"
	# #target_path = bytes("/data/wangteng/test")
	# installer_extractor.lib_ext.InstallerTypeDetect(target_path, ctypes.byref(pack_info))
	# print pack_info.pkg_type
	# print pack_info.is_success
	# print target_path.value
	# installer_extractor.lib_ext.InstallerExtract(target_path, 0, ctypes.byref(pack_info))
	# print pack_info.type
	# print pack_info.success
	
