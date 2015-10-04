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

@module_extension('python27')
def module_extensions_python27(ctx, kw, entry_prefix, platform, configuration):
	
	kw[entry_prefix + 'defines'] += ['USE_PYTHON_SCRIPTING']

	if platform == 'win_x86':
		kw[entry_prefix + 'includes'] += [ ctx.CreateRootRelativePath('Code/SDKs/python27/Win32/include') ]
		kw[entry_prefix + 'libpath'] += [ ctx.CreateRootRelativePath('Code/SDKs/Python27/Win32/libs') ]
	elif platform == 'win_x64':
		kw[entry_prefix + 'includes'] += [ ctx.CreateRootRelativePath('Code/SDKs/python27/x64/include') ]
		kw[entry_prefix + 'libpath'] += [ ctx.CreateRootRelativePath('Code/SDKs/Python27/x64/libs') ]
	else:
		return
		
	if not platform  == 'project_generator':
		kw[entry_prefix + 'features'] += [ 'copy_python27_binaries' ]

@feature('copy_python27_binaries')
def feature_copy_python27_binaries(self):
	bld 			= self.bld
	platform	= bld.env['PLATFORM']
	configuration = bld.env['CONFIGURATION']
		
	if platform  == 'project_generator':
		return
		
	if platform == ('win_x64'): # we don't have DLL's for x86 at the moment
		arch = ('x64' if platform == 'win_x64' else 'x86')

		bin_name = 'python27' # CryEngine uses python27.dll for all configurations
		output_folder = bld.get_output_folders(platform, configuration)[0]
		python_bin = bld.CreateRootRelativePath('Code/SDKs/Python27/x64/DLLs') + os.sep + bin_name
		
		self.create_task('copy_outputs', bld.root.make_node(python_bin + '.dll'), output_folder.make_node(bin_name + '.dll'))
		if os.path.isfile(python_bin + '.pdb'):
			self.create_task('copy_outputs', bld.root.make_node(python_bin + '.pdb'), output_folder.make_node(bin_name + '.pdb'))
		
	else:
		Logs.error('[ERROR] Python27 is not supported for plaform %s by WAF.' % platform)
