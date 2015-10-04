#!/usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2006-2010 (ita)
# Christopher Bolte: Created a copy to easier adjustment to crytek specific changes

from waflib.TaskGen import feature, after_method,before_method, extension
from waflib.Task import Task,RUN_ME
from waflib.Tools import c_preproc, cxx
from waflib import TaskGen
from waflib import Task
from waflib import Logs
import os, re

try:
	from xml.sax import make_parser
	from xml.sax.handler import ContentHandler
except ImportError:
	xml_sax = False
	ContentHandler = object
else:
	xml_sax = True

# TODO: This should be moved to a global configuration file at some point
QT_ROOT = 'Code/SDKs/Qt'
QT_MAJOR_VERSION = "5"
QT_MINOR_VERSION = "5"

qt_modules = [
	'Core', 
	'Gui',
	'OpenGL',
	'Widgets'
]

qt_core_binaries = [
	'icudt', 
	'icuin', 
	'icuuc', 
	'libGLESv2'
]

qt_plugins = {
	'platforms': [
		'qminimal',
		'qwindows',
	]
}

def get_qt_root_path(self):
	# Uses the current MSVC version to find the correct path to binaries/libraries/plugins of QT
	msvc_version = self.bld.env['MSVC_VERSION']
	if isinstance(msvc_version, basestring):
		msvc_version = int(float(msvc_version))
	else:
		msvc_version = 11

	if msvc_version == 11:
		qt_base = 'msvc2012_64'
	elif msvc_version == 12:
		qt_base = 'msvc2013_64'
	elif msvc_version == 14:
		qt_base = 'msvc2015_64'
	else:
		self.fatal('[ERROR] Unable to find QT build to use for MSVC %d' % msvc_version)

	qt_version = QT_MAJOR_VERSION + '.' + QT_MINOR_VERSION
	qt_root = os.path.join(QT_ROOT, qt_version, qt_base)
	return self.bld.CreateRootRelativePath(qt_root)

def collect_qt_properties(self):

	# QT specific settings, should be better to move to configure step
	qt_root = get_qt_root_path(self)
	qt_includes_folder = os.path.join(qt_root, 'include')
	qt_libs_folder = os.path.join(qt_root, 'lib')
	qt_bin_folder = os.path.join(qt_root, 'bin')

	defines = [ 'QT_GUI_LIB', 'QT_NO_EMIT', 'QT_WIDGETS_LIB' ]
	includes = [ qt_includes_folder ]
	libpath = [ qt_libs_folder ]	
	
	if hasattr(self.env['CONFIG_OVERWRITES'], self.target):
		configuration = self.env['CONFIG_OVERWRITES'][self.target]
	else:
		configuration =  self.env['CONFIGURATION']
		
	# Collect module libs	
	libs = []
	debug_postfix = 'd' if configuration == 'debug' else ''	
	qt_modules_include_path_base = qt_includes_folder + '/Qt'
	qt_lib_prefix = 'Qt' + QT_MAJOR_VERSION	
	for module in qt_modules:	
		# Collect include path
		includes += [qt_modules_include_path_base + module] # e.g. .../include/QtNetwork		
		
		# Collect lib and binary
		lib_name = '%s%s%s' % (qt_lib_prefix, module, debug_postfix)
		libs += [lib_name]

	# Save core binaries
	# Note: icudt has special treatment since it's shared between release and debug configurations (as of v55 at least)
	binaries = []
	for binary in qt_core_binaries:
		binaries += ['%s%s' % (binary, debug_postfix if binary != 'icudt' else '')]
	
	return (defines, includes, libpath, libs, binaries)
	
@feature('qt')
@before_method('apply_incpaths')
def apply_qt_flags(self):
	(defines, includes, libpath, lib, binaries) = collect_qt_properties(self)	
			
	# Add dependencies for moc_*.cpp files
	for t in getattr(self, 'compiled_tasks', []):
		for file in getattr(self, 'qt_to_moc_files', []):
			moc_input_file = self.path.make_node(file)
			input_file = t.inputs[0]
			
			base_name = os.path.basename(file)
			dir_name = os.path.dirname(file)
						
			moc_base_output_folder = self.bld.get_bintemp_folder_node().make_node('moc_files').make_node(self.target).make_node(self.bld.env['PLATFORM']+'_'+self.bld.env['CONFIGURATION'])
			moc_output_folder = moc_base_output_folder.make_node(dir_name)	
			moc_file_name = moc_output_folder.make_node('moc_' + base_name.replace('.h','.cpp'))
			
			# Add dependency for moc file
			t.dep_nodes.append(moc_file_name)
			
			# Also add include path for moc file
			if not moc_base_output_folder.abspath() in includes:
				includes.append( moc_base_output_folder.abspath() )
			if not os.path.dirname(moc_base_output_folder.abspath()) in includes:
				includes.append( os.path.dirname(moc_base_output_folder.abspath()) )
		
	# Add defines and include path for QT
	self.env['DEFINES'] += defines
	if not hasattr(self, 'includes'):
		self.includes = []
	self.includes = includes + self.includes 
	
	# Add Linker Flags for QT	      
	self.env['LIB'] += lib
	self.env['LIBPATH'] += libpath

@feature('qt')
@after_method('apply_link')
def create_manual_moc_tasks(self):	
	if self.env['PLATFORM'] == 'project_generator':
		return # Dont create tasks for msvs
		
	for file in getattr(self, 'qt_to_moc_files', []):
		base_name = os.path.basename(file)
		dir_name = os.path.dirname(file)
		moc_input_file = self.path.make_node(file)
				
		moc_output_folder = self.bld.get_bintemp_folder_node().make_node('moc_files').make_node(self.target).make_node(self.bld.env['PLATFORM']+'_'+self.bld.env['CONFIGURATION']).make_node(dir_name)			
		moc_output_folder.mkdir()
		moc_file_name = moc_output_folder.make_node('moc_' + base_name.replace('.h','.cpp'))
		
		# Create Moc Task		
		tsk = self.create_task('qt_moc', moc_input_file, moc_file_name ) 
		tsk.env.append_value( 'MOC', os.path.join(get_qt_root_path(self), 'bin', 'moc.exe') )
		tsk.env.append_value( 'EXTRA_FLAGS', [])
		
		# Collect manual mocced files to prevent moccing those automatically as well
		if not hasattr(self, 'manually_mocced_files'):
			self.manually_mocced_files = set()
		self.manually_mocced_files.add( moc_input_file.abspath().lower() )
		

class qt_moc(Task.Task):	
	color =  'GREEN'	
	run_str = '${MOC} ${EXTRA_FLAGS} -o ${TGT} ${SRC}'	
	
	def scan(self):
		"""
		Re-use the C/C++ scanner, but remove the moc files from the dependencies
		since the .cpp file already depends on all the headers
		"""		
		return c_preproc.scan(self)
		
	def runnable_status(self):
		if super(qt_moc, self).runnable_status() == -1:
			return -1
				
		src = self.inputs[0].abspath()
		tgt = self.outputs[0].abspath()
		
		# If there any target file is missing, we have to copy
		try:
			stat_tgt = os.stat(tgt)
		except OSError:	
			return RUN_ME
					
		# Now compare both file stats
		try:
			stat_src = os.stat(src)				
		except OSError:
			pass
		else:
			# same size and identical timestamps -> make no copy
			if stat_src.st_mtime >= stat_tgt.st_mtime + 2:			
				return RUN_ME
				
		# Everything fine, we can skip this task		
		return -2 # SKIP_ME
		
@extension('.ui')
def create_uic_task(self, node):
	"hook for uic tasks"
	uictask = self.create_task('ui4', node)
	uictask.outputs = [self.path.find_or_declare('ui_%s.h' % node.name[:-3])]
	uictask.env.append_value( 'QT_UIC', os.path.join(get_qt_root_path(self), 'bin', 'uic.exe') )
	
	
class ui4(Task.Task):
	"""
	Process *.ui* files
	"""
	color   = 'BLUE'
	run_str = '${QT_UIC} ${SRC} -o ${TGT}'
	ext_out = ['.h']
	
	
class XMLHandler(ContentHandler):
	"""
	Parser for *.qrc* files
	"""
	def __init__(self):
		self.buf = []
		self.files = []
	def startElement(self, name, attrs):
		if name == 'file':
			self.buf = []
	def endElement(self, name):
		if name == 'file':
			self.files.append(str(''.join(self.buf)))
	def characters(self, cars):
		self.buf.append(cars)
	
	
@extension('.qrc')
def create_rcc_task(self, node):
	"Create rcc and cxx tasks for *.qrc* files"
	rcnode = node.change_ext('_rc.cpp')
	rcctask = self.create_task('rcc', node, rcnode)
	rcctask.disable_pch = True
	rcctask.env.append_value( 'QT_RCC', os.path.join(get_qt_root_path(self), 'bin', 'rcc.exe') )
	cpptask = self.create_task('cxx', rcnode, rcnode.change_ext('.o'))
	cpptask.disable_pch = True
	try:
		self.compiled_tasks.append(cpptask)
	except AttributeError:
		self.compiled_tasks = [cpptask]
	return cpptask	
	
class rcc(Task.Task):
	"""
	Process *.qrc* files
	"""
	color   = 'BLUE'
	run_str = '${QT_RCC} -name ${SRC[0].name} ${SRC[0].abspath()} ${RCC_ST} -o ${TGT}'
	ext_out = ['.h']

	def scan(self):
		"""Parse the *.qrc* files"""
		node = self.inputs[0]

		if not xml_sax:
			Logs.error('no xml support was found, the rcc dependencies will be incomplete!')
			return ([], [])

		parser = make_parser()
		curHandler = XMLHandler()
		parser.setContentHandler(curHandler)
		fi = open(self.inputs[0].abspath(), 'r')
		try:
			parser.parse(fi)
		finally:
			fi.close()

		nodes = []
		names = []
		root = self.inputs[0].parent
		for x in curHandler.files:
			nd = root.find_resource(x)
			if nd: nodes.append(nd)
			else: names.append(x)
		return (nodes, names)
		
# Automatic Moccing support
from waflib.Tools import c_preproc
import re, sys, os, string, traceback
re_inc = re.compile(
	'^[ \t]*(#|%:)[ \t]*(include)[ \t]*[<"](.*)[>"]\r*$',
	re.IGNORECASE | re.MULTILINE)
	
@TaskGen.taskgen_method
def parse_for_moccing_files(self, tsk):
	""" Util function to check all includes, which are part of the current project for Q_OBJECT string, which indicates the need for mocing """			

	tg = self
	def lines_includes(self, node):	
		lower_case_name = node.abspath().lower()
		if not lower_case_name in tg.task_generator_files:
			return []	
		code = node.read()

		if 'Q_OBJECT' in code:
			self.need_moccing_files.add(lower_case_name)
			
		code = c_preproc.re_nl.sub('', code)
		code = c_preproc.re_cpp.sub(c_preproc.repl, code)
		return [(m.group(2), m.group(3)) for m in re.finditer(re_inc, code)]
			
	class qt_parser(c_preproc.c_parser):		
		def addlines(self, node):
			if node in self.nodes[:-1]:
				return
			self.currentnode_stack.append(node.parent)
			self.lines = lines_includes(self,node) + [(c_preproc.POPFILE, '')] +  self.lines
		
		def tryfind(self, filename):
			""" custom tryfind, based on c_preproc but restricted to include only task generator files"""
			self.curfile = filename

			# for msvc it should be a for loop on the whole stack
			found = self.cached_find_resource(self.currentnode_stack[-1], filename)

			for n in self.nodepaths:
				if found:
					break
				found = self.cached_find_resource(n, filename)

			if found and found.abspath().lower() in tg.task_generator_files:
				# TODO the duplicates do not increase the no-op build times too much, but they may be worth removing
				self.nodes.append(found)
				if filename[-4:] != '.moc':
					self.addlines(found)
			else:
				if not filename in self.names:
					self.names.append(filename)
			return found
		
		def start(self, node, env):
			self.need_moccing_files = set()
			self.addlines(node)
			while self.lines:
				(x, y) = self.lines.pop(0)
				if x == c_preproc.POPFILE:
					self.currentnode_stack.pop()
					continue
				self.tryfind(y)

	if getattr(tsk, 'moc_parsing_done', False):		
		return
	tsk.moc_parsing_done = True
	
	# Setup custom parser
	nodepaths = [x for x in self.includes_nodes if x.is_child_of(x.ctx.srcnode) or x.is_child_of(x.ctx.bldnode)]
	
	qt_parser = qt_parser(nodepaths)	
	qt_parser.start(tsk.inputs[0], tsk.env)
	
	ctx = tg.bld
	if not hasattr(ctx, 'need_moccing_files'):
		ctx.need_moccing_files = set()
	ctx.need_moccing_files |= qt_parser.need_moccing_files	
			

@TaskGen.taskgen_method
def create_moc_task(self, tsk, to_moc_file):
	""" Create a moc task (and a cpp task to compile it), and adjust the dependencies """	
	generator 				= self
	bld 							= generator.bld
	to_moc_file				= to_moc_file.lower()
	to_moc_file_node 	= bld.root.make_node(to_moc_file)

	#Logs.info('[INFO] Moc`ing file %s' % to_moc_file)
	
	# Create moc and cxx task
	cxx_node	= to_moc_file_node.parent.get_bld().make_node(to_moc_file_node.name.replace('.', '_') + '_%d_moc.cpp' % generator.idx)
	moc_tsk 	= generator.create_task('qt_moc', to_moc_file_node, cxx_node)
	moc_tsk.env.append_value( 'MOC', os.path.join(get_qt_root_path(self), 'bin', 'moc.exe') )
	extra_flags = ''

	if hasattr(self, 'pch_name'):
		extra_flags = '-b%s' % self.pch_name.replace('.cpp', '.h')
	if hasattr(self, 'qt_moc_additional_includes'): #overwrite -b is moc only allows one
		extra_flags = '-b%s' % self.qt_moc_additional_includes
		
	moc_tsk.env.append_value( 'EXTRA_FLAGS', extra_flags)
	
	if to_moc_file_node.name.endswith('.cpp'):	
		gen = bld.producer
		gen.outstanding.insert(0, moc_tsk)
		gen.total += 1
		tsk.set_run_after(moc_tsk)
	else:						
		cxxtsk = generator.create_task('cxx', moc_tsk.outputs, cxx_node.change_ext('.obj'))
		cxxtsk.set_run_after(moc_tsk)
		
		if self. bld.is_option_true('generate_debug_info'):	
			# Compute PDB file path
			pdb_folder = self.path.get_bld()	
			msvc_version = 'vc' + self.env['MSVC_VERSION'].replace('.', '')
			pdb_file = pdb_folder.make_node(str(self.idx) + '.' + msvc_version + '.pdb')
			pdb_cxxflag = '/Fd' + pdb_file.abspath() + ''
			cxxtsk.env.append_value('CXXFLAGS', pdb_cxxflag) 
			cxxtsk.env.append_value('CFLAGS', pdb_cxxflag) 
			
		try:
			tsk.more_tasks.extend([moc_tsk, cxxtsk])
		except AttributeError:
			tsk.more_tasks = [moc_tsk, cxxtsk]
    
		try:
			link = generator.link_task
		except:
			pass
		else:
			link.set_run_after(cxxtsk)
			link.inputs.extend(cxxtsk.outputs)
			link.inputs.sort(key=lambda x: x.abspath())		
			
'''
Register a task subclass that has hooks for running our custom
dependency calculations to discover QT moccing tasks
'''
def wrap_compiled_task(classname):
	derived_class = type(classname, (Task.classes[classname],), {})		

	def runnable_status(self):
		ret = super(derived_class, self).runnable_status()
		if not 'qt' in self.generator.features:
			return ret
				
		if ret != Task.ASK_LATER and not getattr(self, 'moc_done', None):
        
			generator = self.generator
			bld				= generator.bld
				
			# make sure all sets exist
			if not hasattr(generator ,'moc_tasks_created'):
				generator.moc_tasks_created = set()
			if not hasattr(bld ,'need_moccing_files'):
				bld.need_moccing_files = set()	
			if not hasattr(generator, 'manually_mocced_files'):
				generator.manually_mocced_files = set()
					
			# If this task is executed, we must check if, since it is possible that someone
			# has added a new include which was not in our previous includes
			if ret == Task.RUN_ME: 
				self.generator.parse_for_moccing_files(self)
				
			# Find tasks which must be mocced, we have three sets (each set stored the lower cases abspath)
			# A: The global set of all files which must be mocced (stored in the context, as we else run into performance problems with node caching)
			# B: The set of files which belong to this task generator, to not moc files which we include form another DLL
			# C: The set of files which are already mocced, since we generate those tasks during dependency checking, we can always find new candidates
			# D: All files which were manually mocced already
			
			# Find all to be mocced files which are in this task generator (intersect A and B)
			should_be_mocced = bld.need_moccing_files & generator.task_generator_files
			
			# Remove all files which are already manually mocced
			should_be_mocced -= generator.manually_mocced_files
			
			# Now find all elements which are in the intersection but not in the already mocced list
			must_be_mocced = should_be_mocced - generator.moc_tasks_created
			
			for file in must_be_mocced:
				generator.moc_tasks_created.add(file)
				generator.create_moc_task(self, file)
				
			self.moc_done = True
				
		for t in self.run_after:
				if not t.hasrun:
						return Task.ASK_LATER
		
		return ret		
			
	derived_class.runnable_status = runnable_status
		
for k in ('c', 'cxx', 'pch_msvc'):
	wrap_compiled_task(k)		
	
@feature('copy_qt_binaries')
def copy_qt_binaries(self):

	if self.env['PLATFORM'] == 'project_generator':
		return

	if not 'msvc' in (self.env.CC_NAME, self.env.CXX_NAME) or self.env['MSVC_TARGETS'][0] != 'x64':
		return
		
	if hasattr(self.env['CONFIG_OVERWRITES'], self.target):
		configuration = self.env['CONFIG_OVERWRITES'][self.target]
	else:
		configuration =  self.env['CONFIGURATION']
	
	bld	= self.bld
	platform	= bld.env['PLATFORM']
	output_folder = bld.get_output_folders(platform, configuration)[0]
	qt_root_path = get_qt_root_path(self)
	qt_root_node = bld.root.make_node(qt_root_path)
	
	# Copy qt plugins	
	debug_postfix = 'd' if configuration == 'debug' else ''
	for qt_plugin in qt_plugins.keys():
		qt_plugin_node = qt_root_node.make_node('/plugins/' + qt_plugin)		
		output_folder_plugin = output_folder.make_node(qt_plugin)
		for plugin_name in qt_plugins[qt_plugin]:
			file = plugin_name + debug_postfix + '.dll'
			self.create_task('copy_outputs', qt_plugin_node.make_node(file), output_folder_plugin.make_node(file))
			pdb = plugin_name + debug_postfix + '.pdb'
			if os.path.isfile(os.path.join(qt_root_path, 'plugins', qt_plugin, pdb)):
				self.create_task('copy_outputs', qt_plugin_node.make_node(pdb), output_folder_plugin.make_node(pdb))

	# Copy QT core binaries	
	(defines, includes, libpath, libs, binaries) = collect_qt_properties(self)
	qt_binaries = libs + binaries
	
	qt_bin_node = qt_root_node.make_node('bin')
	for file_name in os.listdir(qt_bin_node.abspath()):
		file_base, file_exension = os.path.splitext(file_name)
		
		# Special case qt icu binaries which carry version number in name
		# e.g. icutest53.dll
		# Note: we can't just strip the last few characters since there may be a 'd' affix for debug libraries
		if file_base.startswith('icu'):
			file_base = ''.join(c for c in file_base if not c.isdigit())

		pdb_name = file_base + '.pdb'
		pdb_path = os.path.join(qt_root_path, 'bin', pdb_name)
			
		# Copy binaries
		if file_base in qt_binaries:
			self.create_task('copy_outputs', qt_bin_node.make_node(file_name), output_folder.make_node(file_name))
			if os.path.isfile(pdb_path):
				self.create_task('copy_outputs', qt_bin_node.make_node(pdb_name), output_folder.make_node(pdb_name))
