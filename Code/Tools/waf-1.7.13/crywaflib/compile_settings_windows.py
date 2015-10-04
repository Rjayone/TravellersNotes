#############################################################################
## Crytek Source File
## Copyright (C) 2013, Crytek Studios
##
## Creator: Christopher Bolte
## Date: Oct 31, 2013
## Description: WAF  based build system
#############################################################################
from waflib.Configure import conf

@conf
def load_windows_common_settings(conf):
	"""
	Setup all compiler and linker settings shared over all windows configurations
	"""
	v = conf.env
	
	# Configure manifest tool
	v.MSVC_MANIFEST = True
	
	# Setup default libraries to always link
	v['LIB'] += [ 'User32', 'Advapi32', 'Ntdll' ]
			
	# Ensure WINAPI version is consistent everywhere
	v['DEFINES'] += [ '_WIN32_WINNT=0x0600', 'NTDDI_VERSION=0x06000000', 'WINVER=0x0600' ]
	
	# Load Resource Compiler Tool
	conf.load_rc_tool()
	
@conf
def load_debug_windows_settings(conf):
	"""
	Setup all compiler and linker settings shared over all windows configurations for
	the 'debug' configuration
	"""
	v = conf.env
	conf.load_windows_common_settings()
	
@conf
def load_profile_windows_settings(conf):
	"""
	Setup all compiler and linker settings shared over all windows configurations for
	the 'debug' configuration
	"""
	v = conf.env
	conf.load_windows_common_settings()
	
@conf
def load_performance_windows_settings(conf):
	"""
	Setup all compiler and linker settings shared over all windows configurations for
	the 'debug' configuration
	"""
	v = conf.env
	conf.load_windows_common_settings()
	
@conf
def load_release_windows_settings(conf):
	"""
	Setup all compiler and linker settings shared over all windows configurations for
	the 'debug' configuration
	"""
	v = conf.env
	conf.load_windows_common_settings()
