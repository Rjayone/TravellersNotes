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
from waflib.TaskGen import feature, after_method
from waflib.CryModuleExtension import module_extension
from waflib.TaskGen import taskgen_method

eaas_shipped_modules = ['CryCommon', 'CryAction', 'CryEntitySystem', 'CryInput', 'CryScriptSystem', 'CrySoundSystem' ]

@module_extension('eaas')
def module_extensions_eaas(ctx, kw, entry_prefix, platform, configuration):	
	kw[entry_prefix + 'defines'] += [ 'IS_EAAS' ]
			
	if not platform  == 'project_generator' and not kw['target'] in eaas_shipped_modules:
		kw[entry_prefix + 'features'] += [ 'copy_eaas_binaries' ]

@feature('copy_eaas_binaries')
@after_method('set_pdb_flags')
@after_method('apply_incpaths')
@after_method('apply_map_file')
def feature_copy_eaas_binaries(self):
	bld 			= self.bld
	platform	= bld.env['PLATFORM']
	configuration = bld.env['CONFIGURATION']
		
	if platform  == 'project_generator' or not bld.options.project_spec: # only support build that have a project spec
		return
		
	if not getattr(self, 'link_task', None):
		return	
		
	if self._type == 'stlib': # Do not copy static libs
		return
		
	eaas_bin_folder_node = bld.root.make_node(bld.CreateRootRelativePath('bin_eaas/%s_%s' % (platform , configuration)))
	eaas_bin_folder_path = eaas_bin_folder_node.abspath()
	eaas_bin_folder_path_length = len(eaas_bin_folder_path)
	output_folders = self.bld.get_output_folders(platform, configuration)
		
	if platform.startswith('win') or platform.startswith('linux'):		
		# Copy binaries to bin folder (including lib files) 
		if bld.is_option_true('generate_product'):	
			for src in self.link_task.outputs:				
				if not src.abspath().endswith('.manifest'):
					self.copy_files(src, [eaas_bin_folder_node])
		else:
			# Copy all files in folder to final output directory
			for dirName, subdirList, fileList in os.walk(eaas_bin_folder_path, followlinks=True):
				rel_path = dirName[eaas_bin_folder_path_length:]
				for output_node in output_folders:
					for file_name in fileList:
						self.create_task('copy_outputs', self.bld.root.make_node(dirName + os.sep + file_name), output_node.make_node(rel_path + os.sep + file_name)) 