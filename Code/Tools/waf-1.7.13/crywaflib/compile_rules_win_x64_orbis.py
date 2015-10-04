#############################################################################
## Crytek Source File
## Copyright (C) 2013, Crytek Studios
##
## Creator: Christopher Bolte
## Date: Oct 31, 2013
## Description: WAF  based build system
#############################################################################
from waflib.Configure import conf
from waflib import Logs
import os

@conf
def check_win_x64_orbis_installed(conf):
	"""
	Check compiler is actually installed on executing machine
	"""
	if not conf.is_option_true('auto_detect_compiler'):
		orbis_sdk_folder = conf.CreateRootRelativePath('Code/SDKs/Orbis')
		
		if not os.path.exists(orbis_sdk_folder):
			Logs.warn('[WARNING] Could not find ORBIS SDK toolchain path: %s' % orbis_sdk_folder)
			return False		
	else:
		# Orbis stored the values in the env
		if not 'SCE_ORBIS_SDK_DIR' in os.environ:
			Logs.warn('[WARNING] Could not find environment variable "SCE_ORBIS_SDK_DIR". Please verify your Orbis SDK installation')
			return False
		
	return True
	
@conf
def load_win_x64_orbis_common_settings(conf):
	"""
	Setup all compiler and linker settings shared over all win_x64_orbis configurations
	"""
	pass
	
@conf
def load_debug_win_x64_orbis_settings(conf):
	"""
	Setup all compiler and linker settings shared over all win_x64_orbis configurations for
	the 'debug' configuration
	"""
	v = conf.env
	conf.load_win_x64_orbis_common_settings()
	
	# Load addional shared settings
	conf.load_debug_cryengine_settings()
	conf.load_debug_clang_settings()
	conf.load_debug_orbis_settings()	
	
@conf
def load_profile_win_x64_orbis_settings(conf):
	"""
	Setup all compiler and linker settings shared over all win_x64_orbis configurations for
	the 'profile' configuration
	"""
	v = conf.env
	conf.load_win_x64_orbis_common_settings()
	
	# Load addional shared settings
	conf.load_profile_cryengine_settings()
	conf.load_profile_clang_settings()
	conf.load_profile_orbis_settings()
	
@conf
def load_performance_win_x64_orbis_settings(conf):
	"""
	Setup all compiler and linker settings shared over all win_x64_orbis configurations for
	the 'performance' configuration
	"""
	v = conf.env
	conf.load_win_x64_orbis_common_settings()
	
	# Load addional shared settings
	conf.load_performance_cryengine_settings()
	conf.load_performance_clang_settings()
	conf.load_performance_orbis_settings()
	
@conf
def load_release_win_x64_orbis_settings(conf):
	"""
	Setup all compiler and linker settings shared over all win_x64_orbis configurations for
	the 'release' configuration
	"""
	v = conf.env
	conf.load_win_x64_orbis_common_settings()
	
	# Load addional shared settings
	conf.load_release_cryengine_settings()
	conf.load_release_clang_settings()
	conf.load_release_orbis_settings()
	