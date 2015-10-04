#############################################################################
## Crytek Source File
## Copyright (C) 2015, Crytek Studios
##
## Creator: Nico Moss
## Date: July 03, 2015
## Description: WAF  based build system
#############################################################################

import os
from waflib import Logs
from waflib.TaskGen import feature
from waflib.CryModuleExtension import module_extension

@module_extension('sdl2')
def module_extensions_sdl2(ctx, kw, entry_prefix, platform, configuration):

	if platform.startswith('win'):
		kw[entry_prefix + 'includes'] += [ ctx.CreateRootRelativePath('Code/SDKs/SDL2/include/win')]
		kw[entry_prefix + 'lib'] += [ 'SDL2' ]		
		arch = ('x64' if platform == 'win_x64' else 'x86')		
		kw[entry_prefix + 'libpath'] += [ ctx.CreateRootRelativePath('Code/SDKs/SDL2/lib') + os.sep + arch]
	elif platform.startswith('linux'):
		kw[entry_prefix + 'includes'] += [ ctx.CreateRootRelativePath('Code/SDKs/SDL2/include/linux')]
	else:
		return
		
	if not platform  == 'project_generator':
		kw[entry_prefix + 'features'] += [ 'copy_sdl2_binaries' ]

@feature('copy_sdl2_binaries')
def feature_copy_sdl2_binaries(self):
	bld 			= self.bld
	platform	= bld.env['PLATFORM']
	configuration = bld.env['CONFIGURATION']
		
	if platform  == 'project_generator':
		return
		
		
	output_folder = bld.get_output_folders(platform, configuration)[0]			
	
	if platform.startswith('win'):
		arch = ('x64' if platform == 'win_x64' else 'x86')
		sdl_binary_name = 'SDL2'
		extention = '.dll'		
		sdl_bin = bld.CreateRootRelativePath('Code/SDKs/SDL2/lib') + os.sep + arch + os.sep + sdl_binary_name
		self.create_task('copy_outputs', bld.root.make_node(sdl_bin + extention), output_folder.make_node(sdl_binary_name + extention))
	elif platform.startswith('linux'):
		pass #install libsdl2-dev on system
	else:
		Logs.error('[ERROR] SDL is not supported on plaform by WAF: %s' % platform)
	
	
############################################################################################################

@module_extension('sdl_mixer')
def module_extensions_sdl_mixer(ctx, kw, entry_prefix, platform, configuration):

	if platform.startswith('win'):
		kw[entry_prefix + 'includes'] += [ ctx.CreateRootRelativePath('Code/SDKs/Audio/SDL_mixer/include') ]
		kw[entry_prefix + 'lib'] += [ 'SDL2_mixer' ]		
		arch = ('x64' if platform == 'win_x64' else 'x86')		
		kw[entry_prefix + 'libpath'] += [ ctx.CreateRootRelativePath('Code/SDKs/Audio/SDL_mixer/lib') + os.sep + arch ]
	elif platform.startswith('linux'):
		kw[entry_prefix + 'includes'] += [ ctx.CreateRootRelativePath('Code/SDKs/Audio/SDL_mixer/include') ]
		Logs.warn('[WARNING] SDL_Mixer binaries missing')
	else:
		Logs.error('[ERROR] SDL_Mixer is not supported for plaform %s by WAF' % platform)
		
	if not platform  == 'project_generator':
		kw[entry_prefix + 'features'] += [ 'copy_sdl2_mixer_binaries' ]

@feature('copy_sdl2_mixer_binaries')
def feature_copy_sdl_mixer_binaries(self):
	bld 			= self.bld
	platform	= bld.env['PLATFORM']
	configuration = bld.env['CONFIGURATION']
		
	if platform  == 'project_generator':
		return
		
	sdl_mixer_binaries = [ 'SDL2_mixer', 'libogg-0', 'libvorbis-0', 'libvorbisfile-3']
	output_folder = bld.get_output_folders(platform, configuration)[0]
	
	if platform.startswith('win'):
		arch = ('x64' if platform == 'win_x64' else 'x86')
		extention = '.dll'

		# Copy SDL mixer
		sdl_mixer_base = bld.CreateRootRelativePath('Code/SDKs/Audio/SDL_mixer/lib') + os.sep  + arch + os.sep
		for bin_name in sdl_mixer_binaries:
			sdl_mixer_bin = sdl_mixer_base +  bin_name
			self.create_task('copy_outputs', bld.root.make_node(sdl_mixer_bin + extention), output_folder.make_node(bin_name + extention))
	elif platform.startswith('linux'):
		pass
	else:
		Logs.error('[ERROR] SDL_Mixer is not supported for plaform %s by WAF.'  % platform)