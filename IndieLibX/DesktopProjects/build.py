#
# Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
#ProjectsCommon.py:
import os 
import os.path
import re
import string
import struct
import getopt
import sys
from xml.etree import ElementTree
from os.path import expanduser
import shutil
from mod_pbxproj import XcodeProject
import platform

g_iOS_VERSION='9.3'
g_MACOSX_VERSION='10.11'
g_iOS_SDK=g_iOS_VERSION
g_OSX_SDK=g_MACOSX_VERSION
g_XCODE='7.3'
CPLUSPLUS=''
test_OS=''
buildsln='AllLibs'
buildprj='a_02_IND_Surface'
buildtool='LNX'

def getOS(test=False):
	if not test_OS=='':
		return test_OS
	d=platform.dist()
	if d[0]=='Ubuntu':
		OS='Ubuntu'
	elif sys.platform=='darwin':
		OS='Darwin'
	elif os.path.exists('/etc/debian_version'):
		OS='Debian'
	elif os.name=='posix':
		OS='Linux'
	elif os.name=='nt':
		OS='Windows'
	#print 'OS='+OS
	#exit(0)
	return OS

def ossystem(cmd):
	r=0
	print cmd
	#return
	if test_OS=='':
    		r = 0
		os.system(cmd)
    		if not r==0:
			line='exit(%d)' % r
			print line
        		sys.exit(r)

ccc=0
g_node=0
drive,tail=os.path.splitdrive(os.path.expanduser("~"))

SDL2version=''
assimp3version=''
NDK=''

def make_executable(path):
	mode = os.stat(path).st_mode
	mode |= (mode & 0444) >> 2    # copy R bits to X
	os.chmod(path, mode)

def runscript(ascript, deletescript=True):

	if False:
		ossystem ("bash -c '"+ascript+"'")
	else:
	    	f=open('./_tmpbashscript_.sh','w')
	    	f.write(ascript)
	    	f.close()
		make_executable('./_tmpbashscript_.sh')
		ossystem ('./_tmpbashscript_.sh')
		if deletescript:
			ossystem ('rm -f ./_tmpbashscript_.sh')

def load(file):
    #print 'load:'+file
    xml_file = os.path.abspath(file)
    lines=[]
    if os.path.exists(file):
    	f=open(xml_file,'r')
    	lines=f.readlines()
    	f.close()
    lXmlAsString=string.join(lines,'')
    return lXmlAsString

def cnf():
	txt=''
	SDL2version=''
	assimp3version=''
	NDK=''

	txt=load('./Projects.xml')
    	p = re.compile(r"<NDK>(.*?)</NDK>",re.S)
	v=p.findall(txt)
	if len(v)==1:
  		NDK = v[0].strip()
    	p = re.compile(r"<assimp3version>(.*?)</assimp3version>",re.S)
	v=p.findall(txt)
	if len(v)==1:
  		assimp3version = v[0].strip()
    	p = re.compile(r"<SDL2version>(.*?)</SDL2version>",re.S)
	v=p.findall(txt)
	if len(v)==1:
  		SDL2version = v[0].strip()

	#print 'SDL2version   ='+SDL2version
	#print 'assimp3version='+assimp3version
	#print 'NDK           ='+NDK
	#exit(0)

	return SDL2version, assimp3version, NDK

def join(v, sep):
	if len(v)==0:
		return ''
	if len(v)==1:
		return v[0]
	return string.join(v, sep)

def raw_string(s):
    if isinstance(s, str):
        s = s.encode('string-escape')
    elif isinstance(s, unicode):
        s = s.encode('unicode-escape')
    return s

def txt2array(txt):
    r = re.compile(r'\s*', re.MULTILINE)
    l=r.split(txt)
    l2=[]
    for i in l:
        l2.append(i.strip())
    return l2

def itoh(i):
	r='%02X' % i
	return r

class Configuration:
    def __init__(self):

	self.maxerrors=1
	self.debug=True
	self.g='-g3'
	self.O='-O0'

	self.domain=''
	self.user=''

	self.androidurl=''
	self.SDLurl=''
	self.ADT=''
	self.ndk=''
	self.sdl2version=''
	self.assimp3version=''
	self.sdlimage2version=''
	self.OSXversion=''
	self.Debianversion=''
	self.Ubuntuversion=''

	self.pythonUbuntuversion=''
	self.pythonDebianversion=''
	self.pythonOSXversion=''

	#self.mscver='9.0'

	self.c=1
	self.Projectsxml='Projects.xml'
	self.Projectsxml2='./DesktopProjects/'+self.getProjectsxml()
	self.solutions=''
	self.projects=''
	if getOS()=='Windows':
		self.guid0='8BC9CEB8-8B4B-11D7-8D12-00A0C91BC9'
		#buildtool = 'NMAKE'
	else:
		self.guid0='EA72E90C-C91B-11DF-8FC4-539D12B3A9'
		#buildtool = 'LNX'
	self.solution='None'
	self.NDKplatform='19'

	self.NDKarmeabi=False
	self.NDKx86=False
	self.NDKmips=False

        self.text=''
	self.mainphoneproject='Test'
	self.package='com.test.Test'
	self.targetdir=''
	self.openal=False
	self.sdl2=False
	self.apps=True
	self.windowsphone=False
	self.ipad=False
	self.iPhoneSimulator=True
	self.mvc=False

	self.HAS_CGGL=False
	
	self.gles2=False

	self.CgGL_1_1=False
	self.GLSLGL_1_1=False

	self.d3d9=False
	self.d3d10=False
	self.d3d11=False

	self.INDIELIBROOT='..'
	self.win8=False
	self.win8app='DesktopApplication'
	self.WindowsKits=False
	self.cwd=''
	if os.getcwd().endswith('/DesktopProjects'):
		self.cwd=os.getcwd().replace('/DesktopProjects','')
	else:
		self.cwd=os.getcwd()

	self.arm6=False
	self.arm7=False
	self.i386=False
	self.arm64=False
	self.iOSmain='i386'
	
	self.ppp=struct.calcsize("P")
	#self.x86x64='x86'
	self.bitness=32
	#self.DXLIBDIR="x86"
	if 8 * self.ppp==64:
		#self.x86x64='x64'
		self.bitness=64
		#self.DXLIBDIR="x64"

	#buildtools='NDK|LNX|MW|PRO|CMake'
	if getOS()=='Windows':
		#buildtools='NDK|NMAKE|VC|MW|PRO|CMake'
		self.HAS_CGGL=True
		#buildtool='NMAKE'	
	elif getOS()=='Darwin':
		#buildtools='NDK|iOS|OSX|MW|PRO|CMake'
		self.LibDir='Mac64'
		#buildtool='OSX'
		self.HAS_CGGL=False
	elif getOS()=='Ubuntu':
		#buildtools='NDK|LNX|MW|PRO|CMake'
		self.LibDir='Linux%d' % self.bitness
		#buildtool='LNX'
		self.HAS_CGGL=True
	elif getOS()=='Debian':
		#buildtools='NDK|LNX|MW|PRO|CMake'
		self.LibDir='Linux%d' % self.bitness
		#buildtool='LNX'
		self.HAS_CGGL=False
	else:#if getOS()=='Linux':
		#buildtools='NDK|LNX|MW|PRO|CMake'
		self.LibDir='Linux%d' % self.bitness
		#buildtool='LNX'
		self.HAS_CGGL=True

	self.dxjune2010='/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/'
	pre='/windows'
	if getOS()=='Windows':
		self.dxjune2010=self.dxjune2010.replace('/','\\')
		pre=r''+drive+''
	self.d3d9 =os.path.exists(raw_string(pre+self.dxjune2010+'d3d9.h'))
	self.d3d10=os.path.exists(raw_string(pre+self.dxjune2010+'D3D10.h'))
	self.d3d11=os.path.exists(raw_string(pre+self.dxjune2010+'D3D11.h'))
	
	if True:
		self.win8=False
	elif getOS() in ['Ubuntu', 'Debian', 'Linux']:
		self.win8=os.path.exists(pre+'/Program Files (x86)/Windows Kits')
	elif getOS()=='Windows':
		self.win8=os.path.exists(r''+pre+r'\Program Files (x86)\Windows Kits')
	if self.win8:
		self.d3d9=False
		self.d3d10=False
		self.d3d11=True
		self.WindowsKits=True
	else:
		self.d3d9=True
		self.d3d10=False
		self.d3d11=False

    def usage(self, ln):
	ln2='Error in line %d' % ln
	print ln2

	OS='LNX'
	d=platform.dist()
	if d[0]=='Ubuntu':
		OS='LNX'
	elif sys.platform=='darwin':
		OS='LNX'
	elif os.path.exists('/etc/debian_version'):
		OS='LNX'
	elif os.name=='posix':
		OS='LNX'
	elif os.name=='nt':
		OS='WIN'
	#if OS=='LNX':
	print "Usage: python build.py --buildsln "+self.getsolutions()+" --buildtool [<LNX|OSX|iOS|NMAKE|MW|NDK>]"
	print "Usage: python build.py --buildprj "+self.getprojects()+" --buildtool [<LNX|OSX|iOS|NMAKE|MW|NDK>]"

    def settextfromxml(self,node):
	self.text=''
	for child_of_node in node:
		if child_of_node.tag=='solutions':
			slns=child_of_node.findall('.//solution')
			for sln in slns:
				name=sln.find('.//name').text.strip()
				if name.strip()==ccc.getsolution():
					#print 'XXX:'+ccc.getsolution()
					self.mainphoneproject=sln.find('.//mainphoneproject').text.strip()
					self.package=sln.find('.//package').text.strip()
					break
		elif child_of_node.tag=='MSCplatform':
			self.win8app=child_of_node.text
		elif child_of_node.tag=='iOSarm6':
			if child_of_node.text.strip()=='True':
				self.arm6=True
		elif child_of_node.tag=='iOSarm7':
			if child_of_node.text.strip()=='True':
				self.arm7=True
		elif child_of_node.tag=='iOSi386':
			if child_of_node.text.strip()=='True':
				self.i386=True
		elif child_of_node.tag=='iOSarm64':
			if child_of_node.text.strip()=='True':
				self.arm64=True

		elif child_of_node.tag=='iOSmain':
			self.iOSmain=child_of_node.text

		elif child_of_node.tag=='NDKplatform':
			self.NDKplatform=child_of_node.text

		elif child_of_node.tag=='NDKarmeabi':
			if child_of_node.text.strip()=='True':
				self.NDKarmeabi=True
		elif child_of_node.tag=='NDKx86':
			if child_of_node.text.strip()=='True':
				self.NDKx86=True
		elif child_of_node.tag=='NDKmips':
			if child_of_node.text.strip()=='True':
				self.NDKmips=True

		elif child_of_node.tag=='win8':
			if child_of_node.text.strip()=='True':
				self.win8=True
		elif child_of_node.tag=='D3D9':
			if child_of_node.text.strip()=='True':
				self.D3D9=True
		elif child_of_node.tag=='D3D10':
			if child_of_node.text.strip()=='True':
				self.D3D10=True
		elif child_of_node.tag=='D3D11':
			if child_of_node.text.strip()=='True':
				self.D3D11=True

		elif child_of_node.tag=='GLES2':
			if child_of_node.text.strip()=='True':
				ccc.setgles2(True)

		elif child_of_node.tag=='androidurl':
			self.androidurl=child_of_node.text
		elif child_of_node.tag=='SDLurl':
			self.SDLurl=child_of_node.text
		elif child_of_node.tag=='ADT':
			self.ADT=child_of_node.text
		elif child_of_node.tag=='assimp3version':
			assimp3version=child_of_node.text
		elif child_of_node.tag=='NDK':
			self.ndk=child_of_node.text
		elif child_of_node.tag=='SDL2version':
			self.sdl2version=child_of_node.text
		elif child_of_node.tag=='SDLimage2version':
			self.sdlimage2version=child_of_node.text
		elif child_of_node.tag=='OSXversion':
			self.OSXversion=child_of_node.text
		elif child_of_node.tag=='Debianversion':
			self.Debianversion=child_of_node.text
		elif child_of_node.tag=='Ubuntuversion':
			self.Ubuntuversion=child_of_node.text

		elif child_of_node.tag=='pythonOSXversion':
			self.pythonOSXversion=child_of_node.text
		elif child_of_node.tag=='pythonDebianversion':
			self.pythonDebianversion=child_of_node.text
		elif child_of_node.tag=='pythonUbuntuversion':
			self.pythonUbuntuversion=child_of_node.text

		elif child_of_node.tag=='domain':
			self.domain=child_of_node.text
		elif child_of_node.tag=='user':
			self.user=child_of_node.text

		elif child_of_node.tag=='max-errors':
			self.maxerrors=child_of_node.text

		elif child_of_node.tag=='debug':
			if child_of_node.text.strip()=='True':
				self.debug=True
		elif child_of_node.tag=='g':
			self.g=child_of_node.text
		elif child_of_node.tag=='O':
			self.O=child_of_node.text

		elif child_of_node.tag=='ipad':
			if child_of_node.text.strip()=='True':
				self.ipad=True

		#elif child_of_node.tag=='mscver':
		#	self.mscver=child_of_node.text

		elif child_of_node.tag=='OpenAL':
			if child_of_node.text.strip()=='True':
				self.openal=True

		elif child_of_node.tag=='Apps':
			if child_of_node.text.strip()=='False':
				self.apps=False

		elif child_of_node.tag=='SDL2':
			if child_of_node.text.strip()=='True':
				self.openal=True

		elif child_of_node.tag=='WindowsPhone':
			if child_of_node.text.strip()=='True':
				self.windowsphone=True

		elif child_of_node.tag=='iPhoneSimulator':
			if child_of_node.text.strip()=='False':
				self.iPhoneSimulator=False

		elif child_of_node.tag=='MVC':
			if child_of_node.text.strip()=='True':
				self.mvc=True

    def getiOSmain(self):
        return self.iOSmain

    def getmaxerrors(self):
        return self.maxerrors

    def getDebug(self):
        return self.debug

    def getG(self):
        return self.g

    def getO(self):
        return self.O

    def getipad(self):
        return self.ipad

    def getNDKplatform(self):
        return self.NDKplatform


    def getNDKarmeabi(self):
        return self.NDKarmeabi
    def getNDKx86(self):
        return self.NDKx86
    def getNDKmips(self):
        return self.NDKmips

    def getopenal(self):
        return self.openal

    def getwindowsphone(self):
        return self.windowsphone

    def getiPhoneSimulator(self):
        return self.iPhoneSimulator

    def getd3d9(self):
        return self.d3d9
    def getd3d10(self):
        return self.d3d10
    def getd3d11(self):
        return self.d3d11

    def getwin8(self):
        return self.win8

    def getwin8app(self):
        return self.win8app

    def getarm6(self):
        return self.arm6
    def getarm7(self):
        return self.arm7
    def geti386(self):
        return self.i386
    def getarm64(self):
        return self.arm64
    def getmainphoneproject(self):
        return self.mainphoneproject

    def gettargetdir(self):
        return self.targetdir
    def getpackage(self):
	return self.package
    def getbitness(self):
	return self.bitness
    #def getmscver(self):
    #	return self.mscver

    def getbuildtools(self):
	return ''
    def getsolution(self):
	return self.solution


    def getINDIELIBROOT(self):
	return self.INDIELIBROOT

    def getWindowsKits(self):
	return self.WindowsKits

    def getProjectsxml(self):
	return self.Projectsxml
    def getProjectsxml2(self):
	return self.Projectsxml2
    def setbuildtool(self, a):
	buildtool=a
    def getbuildtool(self):
	return buildtool
    def getsolutions(self):
	return self.solutions
    def setsolutions(self, a):
	self.solutions=a

    def getprojects(self):
	return self.projects
    def setprojects(self, a):
	self.projects=a

    def getguid0(self):
	return self.guid0
    def getC(self):
	r=self.c
	self.c=self.c+1
	return r
    def resetC(self):
	self.c=1

    def getURL(self):
	return "http://"+self.getDomain()
    def getandroidURL(self):
	return self.androidurl
    def getSDLURL(self):
	return self.SDLurl
    def getADT(self):
	return self.ADT
    def getSDLIMAGE2version(self):
	return self.sdlimage2version
    def getOSXversion(self):
	return self.OSXversion
    def getDebianversion(self):
	return self.Debianversion
    def getUbuntuversion(self):
	return self.Ubuntuversion

    def getpythonOSXversion(self):
	return self.pythonOSXversion
    def getpythonDebianversion(self):
	return self.pythonDebianversion
    def getpythonUbuntuversion(self):
	return self.pythonUbuntuversion

    def getDomain(self):
	return self.domain
    def getUser(self):
	return self.user

    def setgles2(self, a):
        self.gles2=a
    def getgles2(self):
        return self.gles2


    def getOpenAL(self):
        return self.openal

    def getApps(self):
        return self.apps

    def getSDL2(self):
        return self.sdl2

    def getWindowsPhone(self):
        return self.windowsphone

    def getiPhoneSimulator(self):
        return self.iPhoneSimulator

    def getMVC(self):
        return self.mvc


class Solution:
    def __init__(self):
	self.name=''
        self.v=[]
        self.text=''
    def getname(self):
        return self.name
    def getmembers(self):
        return self.v
    def settextfromxml(self,node):
        self.text=''
	for child_of_node in node:
		if child_of_node.tag=='modules':
			self.v=txt2array(child_of_node.text)
		elif child_of_node.tag=='name':
			self.name=child_of_node.text.strip()

class Project:
    def __init__(self):
	self.name=''
        self.text=''
        self.srcs=[]
        self.srcsc=[]
        self.srcscpp=[]
        self.incs=[]
        self.flags=[]
        self.libs=[]
        self.sharedlibs=[]
        self.srcdir=''
        self.exedir='bin'
	self.type='static_library'
	buildtools=[]
	#buildtool = 'LNX'
	#self.gles2=False
	self.characterset=False

    def getsrcdir(self):
        return self.srcdir

    def getbuildtools(self):
        return buildtools

    def getbuildtool(self):
        return buildtool

    def getcharacterset(self):
        return self.characterset

    def setcharacterset(self, a):
        self.characterset=a

    def getuc(self):
        return self.characterset

    def setuc(self, a):
        self.characterset=a

    #def setgles2(self, a):
    #    self.gles2=a
    def getname(self):
        return self.name
    def setname(self,a):
        self.name=a.strip()
    def settextfromxml(self,node,libs, abuildtool):
	buildtool=abuildtool.strip()
        self.text=''
	self.libs=list(reversed(libs))
	for child_of_node in node:
		if child_of_node.tag=='name':
			self.name=child_of_node.text.strip()
		elif child_of_node.tag=='platform':
			if not child_of_node.text.strip()=='PHONE':
				#buildtools.append('NDK')
				#buildtools.append('iOS')
				#buildtools.append('WP')
				#buildtool='PHONE'
				#else:
				#buildtools=txt2array(child_of_node.text)
				buildtool=child_of_node.text.strip()
			#if(not len(buildtools)):
			#	buildtools.append(abuildtool)
			#	buildtool=abuildtool.strip()
		elif child_of_node.tag=='flags':
			self.flags=txt2array(child_of_node.text)
		elif child_of_node.tag=='include_directories':
			self.incs=txt2array(child_of_node.text)
		elif child_of_node.tag=='src_directory':
			self.srcdir=child_of_node.text
		elif child_of_node.tag=='src_files':
			r = re.compile(r'\s*', re.MULTILINE)
			l=r.split(child_of_node.text)
			self.srcs=[]
			for i in l:
				if i.strip()=='':
					continue
				if i.strip().startswith('#'):
					continue
				if (not buildtool=='iOS'):
					# and (not buildtool=='OSX'):
					if (i.strip().endswith('.m')):
						continue
					if (i.strip().endswith('.mm')):
						continue
				self.srcs.append(self.srcdir+'/'+i.strip())
				cwd=''
				if os.getcwd().endswith('/DesktopProjects'):
					cwd=os.getcwd().replace('/DesktopProjects','')
				else:
					cwd=os.getcwd()
				ii=self.srcdir+'/'+os.path.abspath(i).replace(cwd+'/DesktopProjects/','').strip()
				#print ii
				if os.name=='nt':
					
					ii=ii.replace('/C:\\IndieLibX\\DesktopProjects\\','/')
					ii=ii.replace('/C:\\IndieLibX\\DesktopProjects','')
					ii=ii.replace('\\','/')
					#print ii

				main,ext=os.path.splitext(ii)
				if ext.strip()=='.cpp':
					self.srcscpp.append(ii)
				if ext.strip()=='.c':
					self.srcsc.append(ii)

				p=i.rfind('/')
				if self.srcdir=='':
					self.srcdir=i[0:p]
			#exit(0)
		elif child_of_node.tag=='construct':
			self.type=child_of_node.text
		elif child_of_node.tag=='exe_directory':
			self.exedir=child_of_node.text
    def gettext(self):
        return self.text
    def settype(self,a):
        self.type=a
    def gettype(self):
        return self.type
    def getexedir(self):
        return self.exedir
    def setincs(self,a):
        self.incs=txt2array(a)
    def getincs(self):
        return self.incs
    def setsrcs(self,a):
        self.srcs=txt2array(a)
	new=[]
	for i in self.srcs:
		new.append(os.path.abspath(i).replace(cwd+'/','./'))
	self.srcs=new
    def getsrcs(self):
        return self.srcs
    def getsources(self):
    	return string.join(self.srcs,' ')

    def getcsrcsv(self):
	return self.srcsc
    def getcppsrcsv(self):
	return self.srcscpp
    def setlibs(self,a):
        self.libs=txt2array(a)
    def setsharedlibs(self,a):
        self.sharedlibs=txt2array(a)
    def getlibs(self):
        return self.libs
    def getsharedlibs(self):
        return self.sharedlibs
    def setflags(self,a):
        self.flags=txt2array(a)
    def getflags(self):
        return self.flags
    def getguid(self):
	return ccc.getguid0()+itoh(ccc.getC())

def parse(sln, abuildtool, gles2=False):
	c=0
	s=0
	pv=[]
	libs=[]
	c=Configuration()
	c.settextfromxml(g_node)
	for node in g_node.findall('.//solution'):
		s0=Solution()
		s0.settextfromxml(node)
		#print s0.getname()
		#print sln.strip()
		if s0.getname()==sln.strip():
			s=s0
			print 'build solution='+s0.getname()
			break
	if s==0:
		print 'build solution '+sln.strip()+' not found!'
		exit(0)

	sv=s.getmembers()
	for i in sv:
		if i.startswith("#"):
			continue
		for node in g_node.findall('.//project'):

			if (not (buildtool==abuildtool.strip())):
				continue
			p=Project()
			p.settextfromxml(node,libs, abuildtool)
			if buildtool in ['LNX','OSX','NMAKE','NDK','iOS','OSX','MW']:
				if p.getname() in ['SDL2','OpenAL_x']:
					continue

			if not p.getbuildtool() == 'PHONE':
				if not p.getbuildtool() == abuildtool.strip():
					continue
			else:
				if not abuildtool.strip() in ['NDK','iOS','WP']:
					continue

			if p.getname()==i.strip():
				if not p.gettype()=='application':
					libs.append(p.getname())
				if gles2:
					ccc.setgles2(True)			
				pv.append(p)
			
					
	return c,s,pv

def install():
	install='''#!/bin/bash
#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#

CURDIR=`pwd -P`

if [[ $OSTYPE == darwin* ]]; then
	XCODE=`which xcodebuild`
	if [ -z "$XCODE" ]; then
		echo "Could not find Xcode. Install Xcode:"
		exit 1
	fi
	BREW=`which brew`
	if [ -z "$BREW" ]; then
		echo "Could not find brew. Install brew:"
		echo "ruby -e \\\"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)\\\""
		exit 1
	fi
else
	APTGET=`which apt-get`
	if [ -z "$APTGET" ]; then
		echo "Could not find apt-get."
		exit 1
	fi
fi

SUDO=sudo
if [ -f "/etc/debian_version" ]; then
	SUDO=
fi

CURL=`which curl`
if [ -z "$CURL" ];then
	echo "Could not find curl. Install curl:"
	if [[ $OSTYPE == darwin* ]]; then
		echo "brew install curl"
	fi
	if [[ $OSTYPE == linux* ]]; then
		echo "$SUDO apt-get -y install curl"
	fi
	exit 1
fi

url="'''+ccc.getURL()+'''"
androidurl="'''+ccc.getandroidURL()+'''"
adt="'''+ccc.getADT()+'''"
ndk="'''+NDK+'''"
sdl2version="'''+SDL2version+'''"
sdlimage2version="'''+ccc.getSDLIMAGE2version()+'''"
OSXversion="'''+ccc.getOSXversion()+'''"
Debianversion="'''+ccc.getDebianversion()+'''"
Ubuntuversion="'''+ccc.getUbuntuversion()+'''"

OS="Windows"
OK="N"
if [ -f "/etc/lsb-release" ]; then
	DINFO=$(cat /etc/lsb-release | grep DISTRIB_ID | sed s/.*=//)
	if [ $DINFO == "Ubuntu" ]; then
		OS="Ubuntu"
		sr=$(lsb_release -sr)
		if [[ "$sr" == "$Ubuntuversion" ]]; then
			OK="Y"
		fi
	fi
elif [ -f "/etc/debian_version" ]; then
	OS="Debian"
	sr=$(lsb_release -sr)
	if [[ "$sr" == "$Debianversion" ]]; then
		OK="Y"
	fi
elif [ -d "/Applications/Xcode.app" ]; then
	OS="Darwin"
	sr=$(sw_vers -productVersion)
	if [[ "$sr" == "$OSXversion" ]]; then
		OK="Y"
	fi
elif [ -d "/Applications/Xcode-Beta.app" ]; then
	OS="Darwin"
	sr=$(sw_vers -productVersion)
	if [[ "$sr" == "$OSXversion" ]]; then
		OK="Y"
	fi
fi

bitness="x86"
if [[ "$arch" == "x86_64" ]]; then
	bitness="x86_64"
elif [[ "$arch" == "i386" ]]; then
	bitness="x86_64"
fi

INC="/usr/include"
if [[ $OSTYPE == darwin* ]]; then
	INC="/usr/local/include"
fi

clean()
{
	cd $CURDIR
	if [[ $OSTYPE == darwin* ]]; then
		if [ -e ./$ndk-darwin-$bitness.tar ]; then
			rm -f ./$ndk-darwin-$bitness.tar
			rm -f ./$ndk-darwin-$bitness.tar.bz2
			rm -f ./adt-bundle-mac-$bitness-$adt.zip
		fi
	fi
	if [[ $OSTYPE == linux* ]]; then
		if [ -e ./$ndk-linux-$bitness.tar ]; then
			rm -f ./$ndk-linux-$bitness.tar
			rm -f ./$ndk-linux-$bitness.tar.bz2
			rm -f ./adt-bundle-linux-$bitness-$adt.zip
		fi
	fi
	rm -f ./SDL2-$sdl2version.tar
	rm -f ./SDL2_image-$sdlimage2version.tar
	rm -f ./SDL2-$sdl2version.tar.gz
	rm -f ./SDL2_image-$sdlimage2version.tar.gz
	rm -f ./IndieLibX.zip
	rm -f ./IndieLib_resources.zip
	rm -f ./IndieLibX_AllLibs_LNX64.zip
	rm -f ./IndieLibX_AllLibs_LNX32.zip
	rm -f ./IndieLibX_AllLibs_OSX64.zip
	rm -f ./IndieLibX_AllLibs_LNX64.zip
	rm -f ./IndieLibX_AllLibs_LNX32.zip
	rm -f ./IndieLibX_AllLibs_OSX64.zip
}

install()
{
cd $CURDIR

YmdHMS=$(date +%Y-%m-%d-%H-%M-%S)

if [ "IndieLibX" == "IndieLibX" ]; then
	if [[ $OSTYPE == darwin* ]]; then		
		if [ -d ./IndieLib_resources ]; then
			mv -f ./IndieLib_resources "./IndieLib_resources_"$YmdHMS
		fi
		if [ -d ./IndieLibX ]; then
			mv -f ./IndieLibX "./IndieLibX_"$YmdHMS
		fi
		rm -fr $HOME/Documents/IndieLib_resources
	fi
	if [[ $OSTYPE == linux* ]]; then		
		if [ -f ./IndieLib_resources ]; then
			mv -f ./IndieLib_resources "./IndieLib_resources_"$YmdHMS
		fi
		if [ -f ./IndieLibX ]; then
			mv -f ./IndieLibX "./IndieLibX_"$YmdHMS
		fi
		rm -fr $HOME/Asiakirjat/IndieLib_resources
		rm -fr $HOME/Documents/IndieLib_resources
	fi
	#mv -f ./$ndk "./$ndk_"$YmdHMS
	#mv -f ./adt-bundle "./adt-bundle_"$YmdHMS
	clean
fi

if [[ $OSTYPE == darwin* ]]; then
	brew install cvs git subversion mercurial gcc valgrind cmake python bison flex libtool autoconf swig
	#brew install android-sdk android-ndk
	#brew install ant
	#brew install sdl2 sdl2_image
	brew tap caskroom/cask
	brew unlink brew-cask
	brew install brew-cask
	brew cask install java xquartz
	brew install wine
	brew install ios-sim
	#brew install node
	#npm install -g ios-deploy
	#sudo gem install cocoapods
	#sudo gem install xcodeproj
	sudo xcodebuild -license
fi
if [[ $OSTYPE == linux* ]]; then
	$SUDO apt-get update
	$SUDO apt-get upgrade
	$SUDO apt-get dist-upgrade
	$SUDO apt-get -y install gedit cvs git subversion mercurial gcc g++ build-essential valgrind filezilla cmake rar libsdl1.2-dev libsdl-image1.2-dev python bison flex libtool autoconf swig mypaint pencil blender libjpeg-dev libpng12-dev libdevil-dev libassimp3 libtinyxml-dev libsdl2-dev libsdl2-image-dev openjdk-8-jdk mono-mcs lmms libusb-dev asunder
	#$SUDO gsettings set org.gnome.desktop.screensaver lock-enabled false
fi

if [ ! -f $INC/SDL2/SDL.h ]; then
	rm -fr ./SDL2-$sdl2version
	$CURL -O $sdlurl/release/SDL2-$sdl2version.tar.gz
	gunzip ./SDL2-$sdl2version.tar.gz
	tar -xf ./SDL2-$sdl2version.tar
	cd ./SDL2-$sdl2version
	./configure
	make
	$SUDO make install
	cd ..
	rm -fr ./SDL2-$sdl2version
fi
if [ ! -f $INC/SDL2/SDL_image.h ]; then
	rm -fr ./SDL2_image-$sdlimage2version
	$CURL -O $sdlurl/projects/SDL_image/release/SDL2_image-$sdlimage2version.tar.gz
	gunzip ./SDL2_image-$sdlimage2version.tar.gz
	tar -xf ./SDL2_image-$sdlimage2version.tar
	cd ./SDL2_image-$sdlimage2version
	./configure
	make
	$SUDO make install
	cd ..
	rm -fr ./SDL2_image-$sdlimage2version
fi

if [ "IndieLibX" == "IndieLibX" ]; then
	$CURL -O $url/IndieLibX.zip
	unzip -x ./IndieLibX.zip #-d ./IndieLibX
	#find ./IndieLibX -type d -exec chmod a+x {} \;
	#find ./IndieLibX -type d -exec chmod a+r {} \;
	#find ./IndieLibX -type f -exec chmod a+r {} \;
	$CURL -O $url/IndieLib_resources.zip
	unzip -x ./IndieLib_resources.zip #-d ./IndieLib_resources
	#find ./IndieLib_resources -type d -exec chmod a+x {} \;
	#find ./IndieLib_resources -type d -exec chmod a+r {} \;
	#find ./IndieLib_resources -type f -exec chmod a+r {} \;
	if [[ $OSTYPE == darwin* ]]; then
		cp -fr ./IndieLib_resources ./Documents
	fi
	if [[ $OSTYPE == linux* ]]; then
	if [ -f $HOME/Asiakirjat ]; then
		cp -fr ./IndieLib_resources $HOME/Asiakirjat
	fi
	if [ -f $HOME/Documents ]; then
		cp -fr ./IndieLib_resources $HOME/Documents
	fi
	fi

	if [[ $OSTYPE == linux* ]]; then
	if [[ "$bitness" == "x86_64" ]]; then
		rm -fr ./IndieLibX_AllLibs_LNX64
		$CURL -O $url/IndieLibX_AllLibs_LNX64.zip
		unzip -x ./IndieLibX_AllLibs_LNX64.zip
		$SUDO mv ./IndieLibX_AllLibs_LNX64/lib*.a /usr/local/lib
		rm -fr ./IndieLibX_AllLibs_LNX64
	else
		rm -fr ./IndieLibX_AllLibs_LNX32
		$CURL -O $url/IndieLibX_AllLibs_LNX32.zip
		unzip -x ./IndieLibX_AllLibs_LNX32.zip
		$SUDO mv ./IndieLibX_AllLibs_LNX32/lib*.a /usr/local/lib
		rm -fr ./IndieLibX_AllLibs_LNX32
	fi
	fi
	if [[ $OSTYPE == darwin* ]]; then
		rm -fr ./IndieLibX_AllLibs_OSX64
		$CURL -O $url/IndieLibX_AllLibs_OSX64.zip
		unzip -x ./IndieLibX_AllLibs_OSX64.zip
		$SUDO mv ./IndieLibX_AllLibs_OSX64/lib*.a /usr/local/lib
		rm -fr ./IndieLibX_AllLibs_OSX64
	fi
fi

if [[ $OSTYPE == darwin* ]]; then
	#echo "export ANDROID_SDK=/usr/local/Cellar/android-sdk/r17" >> .bash_profile
	#echo "export ANDROID_NDK=/usr/local/Cellar/android-ndk/r7b" >> .bash_profile
	if [ ! -d ./$ndk ]; then
		$CURL -O $androidurl/ndk/$ndk-darwin-$bitness.tar.bz2
		bunzip2 -d ./$ndk-darwin-$bitness.tar.bz2
		tar -xf ./$ndk-darwin-$bitness.tar
		#mv ./$ndk-darwin-$bitness ./$ndk
		#find ./$ndk -type d -exec chmod a+x {} \;
		#find ./$ndk -type d -exec chmod a+r {} \;
		#find ./$ndk -type f -exec chmod a+r {} \;
		echo "export NDK=$ndk" >> .bash_profile
		echo "export ANDROID_NDK=$CURDIR/$ndk" >> .bash_profile
		echo "export PATH=$ANDROID_NDK:$PATH" >> .bash_profile
		source ~/.bash_profile
	fi
	if [ ! -d ./adt-bundle ]; then
		$CURL -O $androidurl/adt/adt-bundle-mac-$bitness-$adt.zip
		unzip -x ./adt-bundle-mac-$bitness-$adt.zip  #-d ./adt-bundle
		mv ./adt-bundle-mac-$bitness-$adt ./adt-bundle
		#find ./adt-bundle -type d -exec chmod a+x {} \;
		#find ./adt-bundle -type d -exec chmod a+r {} \;
		#find ./adt-bundle -type f -exec chmod a+r {} \;
		echo "export ANDROID_SDK=$CURDIR/adt-bundle/sdk" >> .bash_profile
		echo "export PATH=$ANDROID_SDK:$ANDROID_SDK/tools:$ANDROID_SDK/platform-tools:$PATH" >> .bash_profile
		source ~/.bash_profile
	fi
fi

if [[ $OSTYPE == linux* ]]; then
	if [ ! -f ./$ndk ]; then
		$CURL -O $androidurl/ndk/$ndk-linux-$bitness.tar.bz2
		bunzip2 -d ./$ndk-linux-$bitness.tar.bz2
		tar -xf ./$ndk-linux-$bitness.tar
		#mv ./$ndk-linux-$bitness ./$ndk
		#find ./$ndk -type d -exec chmod a+x {} \;
		#find ./$ndk -type d -exec chmod a+r {} \;
		#find ./$ndk -type f -exec chmod a+r {} \;
		echo "export NDK=$ndk" >> .bashrc
		echo "export ANDROID_NDK=$CURDIR/$ndk" >> .bashrc
		echo "export PATH=$ANDROID_NDK:$PATH" >> .bashrc
		source ~/.bashrc
	fi
	if [ ! -f ./adt-bundle ]; then
		$CURL -O $androidurl/adt/adt-bundle-linux-$bitness-$adt.zip
		unzip -x ./adt-bundle-linux-$bitness-$adt.zip #-d ./adt-bundle
		mv ./adt-bundle-linux-$bitness-$adt ./adt-bundle
		#find ./adt-bundle -type d -exec chmod a+x {} \;
		#find ./adt-bundle -type d -exec chmod a+r {} \;
		#find ./adt-bundle -type f -exec chmod a+r {} \;
		echo "export ANDROID_SDK=$CURDIR/adt-bundle/sdk" >> .bashrc
		echo "export PATH=$ANDROID_SDK:$ANDROID_SDK/tools:$ANDROID_SDK/platform-tools:$PATH" >> .bashrc
		source ~/.bashrc
	fi
fi

echo "#"
echo "#"
echo "#"
echo "# Open Android SDK Manager window and mark checkbox Android SDK Build-tools 19.1 to be installed."
echo "#"
echo "#"
echo "#"
#ANDROID=`which android`
ANDROID='$CURDIR/adt-bundle/sdk/tools/android'
if [ "X" == "Y" ]; then
	$ANDROID update sdk --no-ui
	$ANDROID update sdk --filter tools,platform-tools,build-tools-19.1.0
else
	$ANDROID
fi
if [ -n $ANDROID ]; then
	emulator_images=`$ANDROID list avds | grep "Name:" | cut -f 2 -d ":"`
	emulator_list=($emulator_images)
	if [[ " ${emulator_list[@]} " =~ "avd19" ]]; then
		echo "avd19 found."
	else
		$ANDROID create avd -n avd19 -t 1
	fi
fi

CURDIR0="$CURDIR/IndieLibX/DesktopProjects"
SDLPATH="$CURDIR/IndieLibX/Libraries/SDL2-$sdl2version"

clean
}

if [[ $OSTYPE == linux* ]]; then
if [ ! -f $INC/GL/gl.h ]; then
	echo "Could not find $INC/GL/gl.h. Install mesa-common-dev:"
	echo "$SUDO apt-get -y install mesa-common-dev"
	exit 1
fi
if [ ! -f $INC/GL/glu.h ]; then
	echo "Could not find $INC/GL/glu.h. Install libglu1-mesa-dev:"
	echo "$SUDO apt-get -y install libglu1-mesa-dev"
	exit 1
fi
if [ ! -f /usr/include/Cg/cg.h ]; then
	echo "Could not find /usr/include/Cg/cg.h. Install nvidia-cg-toolkit:"
	echo "$SUDO apt-get -y install nvidia-cg-toolkit"
	exit 1
fi
fi

if [[ "$OK" != "Y" ]]; then
	yn="Y"
	echo "This system is not one of: Ubuntu $Ubuntuversion, Debian $Debianversion or OSX $OSXversion ."
	echo "Continue at your own risk."
	while true; do
    		read -p "Do you wish to install IndieLibX [YyNn]?" yn
    		case $yn in 
        		[Yy]* ) install; break;;
        		[Nn]* ) exit 0;;
        		* ) echo "Please answer yes or no.";;
    		esac
	done
else
	install;
fi
'''
	fn='../install.sh'
	output = open(fn,'w')		
	output.write(install)
	output.close()
	make_executable(fn)

def getpackagesources(target, node):
	return ''
	slns=node.findall('.//solution')
	for sln in slns:
		name=sln.find('.//name').text.strip()
		if name.strip()==target.strip():
			configuration, solutions, projects=parse(sln, buildtool)
		
			for i in projects:
				if i.getname().strip()==ccc.getmainphoneproject().strip():
					sources=i.getsources()
					package=ccc.getpackage()
					return package+' '+sources
				else:
					continue
	return ''

def checktarget(target, node):	
	slns=node.findall('.//solution')
	for sln in slns:
		name=sln.find('.//name').text.strip()
		if name.strip()==target.strip():
			return True
	return False

def getslns(node):	
	slns=node.findall('.//solution')
	#print 'slns='+slns
	#exit(0)
	r=''
	c=1
	for sln in slns:
		if c==1:
			r=sln.find('.//name').text.strip()
		else:
			r+='|'+sln.find('.//name').text.strip()
		c=c+1	
	return r

def getprjs(node):	
	slns=node.findall('.//project')
	#print 'slns='+slns
	#exit(0)
	r=''
	c=1
	for sln in slns:
		if c==1:
			r=sln.find('.//name').text.strip()
		else:
			r+='|'+sln.find('.//name').text.strip()
		c=c+1	
	return r

#ARGV = sys.argv
#if '-mingw32' in ARGV:

def getflagsincs(a):
	v=[]

	for i in a.getflags():
		v.append('-D'+i)

	v.append('-I'+ccc.getINDIELIBROOT()+'/Libraries/BoostWorkaround')
	for i in a.getincs():
	    	v.append('-I'+ccc.getINDIELIBROOT()+'/'+i)
	    
	aflagsincs=join(v, ' ')
	return aflagsincs

def test2():
	if not sys.platform=='darwin':
		return
	Darwinversions='''#!/bin/bash

if [ "$OS" == "Darwin" ]; then

	if [ -z ${SDK_NAME} ]; then
		UFW_GREP_RESULT=$(xcodebuild -showsdks | grep -o "iphoneos.*$")
		while read -r line; do
		iOS_VERSION="${line}"
		done <<< "${UFW_GREP_RESULT}"
		UFW_GREP_RESULT2=$(xcodebuild -showsdks | grep -o "macosx.*$")
		while read -r line; do
		MACOSX_VERSION="${line}"
		done <<< "${UFW_GREP_RESULT2}"

	fi

	iOS_VERSION=$(echo "${iOS_VERSION}" | grep -o "[0-9].*$")
	MACOSX_VERSION=$(echo "${MACOSX_VERSION}" | grep -o "[0-9].*$")

	readonly iOS_SDK=$(xcodebuild -showsdks | grep iphoneos | sort | tail -n 1 | awk '{print substr($NF, 9)}')
	readonly OSX_SDK=$(xcodebuild -showsdks | grep macosx | sort | tail -n 1 | awk '{print substr($NF, 7)}')
	readonly XCODE=$(xcodebuild -version | grep Xcode | cut -d " " -f2)

	export iOS_VERSION
	export MACOSX_VERSION
	export iOS_SDK
	export OSX_SDK
	export XCODE

	echo "iOS_VERSION=$iOS_VERSION" > ./_tmpbashscript_.txt
	echo "MACOSX_VERSION=$MACOSX_VERSION" >> ./_tmpbashscript_.txt
	echo "iOS_SDK=$iOS_SDK" >> ./_tmpbashscript_.txt
	echo "OSX_SDK=$OSX_SDK" >> ./_tmpbashscript_.txt
	echo "XCODE=$XCODE" >> ./_tmpbashscript_.txt
else
	echo "iOS_VERSION=9.3" > ./_tmpbashscript_.txt
	echo "MACOSX_VERSION=10.11" >> ./_tmpbashscript_.txt
	echo "iOS_SDK=9.3" >> ./_tmpbashscript_.txt
	echo "OSX_SDK=10.11" >> ./_tmpbashscript_.txt
	echo "XCODE=?" >> ./_tmpbashscript_.txt
fi
	'''

	runscript(Darwinversions, False)

	txt=load('./_tmpbashscript_.txt')
    	p = re.compile(r"\biOS_VERSION=(.*?)$",re.S)
	v=p.findall(txt)
	if len(v)==1:
  		g_iOS_VERSION = v[0].strip()
		g_iOS_SDK = g_iOS_VERSION
    	p = re.compile(r"\bMACOSX_VERSION=(.*?)$",re.S)
	v=p.findall(txt)
	if len(v)==1:
  		g_MACOSX_VERSION = v[0].strip()
		g_MACOSX_SDK=g_MACOSX_VERSION
    	p = re.compile(r"\bXCODE=(.*?)$",re.S)
	v=p.findall(txt)
	if len(v)==1:
  		g_XCODE = v[0].strip()
	ossystem('rm -r ./_tmpbashscript_.txt')

def NDKInstall(apackage):
	androidbuild='''#!/bin/bash
source ../make.cnf

if [ -a ../count.txt ]; then
	num=$(<../count.txt)
	num2=$((num + 1)) 
	echo "$num2" > ../count.txt
else
	echo "1" > ../count.txt
fi
#echo "num:$num"

assets()
{
	$ADB shell rm -fr /mnt/sdcard/IndieLib_resources
 	$ADB shell mkdir /mnt/sdcard/IndieLib_resources
 	mkdir -p $BUILDPATH/assets/IndieLib_resources
 	#cp -fr $CURDIR/../../../../IndieLib_resources $BUILDPATH/assets/.

	arr=("IndieLib-Animation-Collision" "IndieLib-Blend" "IndieLib-Fading" "IndieLib-FloatingText" "IndieLib-ParallaxScroll" "IndieLib-TankEnemies" "IndieLib-TankEnemiesParallax" "IndieLib-TankFollowsMouseFires" "IndieLib-Transitions" "RadeonTerrainDemo" "NatureScene" "testspine2" "ParticleVS")

	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/'''+ccc.getmainphoneproject()+''' $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/'''+ccc.getmainphoneproject()+''' /mnt/sdcard/IndieLib_resources/.
	fi

	arr=("a_08_Collisions" "a_16_IND_Timer" "a_02_IND_Surface" "b_01_IND_Surface_Grids" "a_03_IND_Image" "b_02_Blitting_2d_Directly" "a_04_IND_Animation" "a_11_Animated_Tile_Scrolling" "a_05_IND_Font" "a_13_2d_Camera" "b_04_Several_ViewPorts" "a_06_Primitives" "c_01_Alien_BenchMark" "a_07_IND_Input" "a_15_Parallax_Scrolling" "Texture" "Blending")

	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/'''+ccc.getmainphoneproject()+''' $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/'''+ccc.getmainphoneproject()+''' /mnt/sdcard/IndieLib_resources/.
	fi

	arr=("3DAnimation" "b_03_Blitting_3d_Directly" "a_09_IND_3dMesh")
	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/skeleton $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/skeleton /mnt/sdcard/IndieLib_resources/skeleton
	fi

	arr=("a_10_IND_Light" "a_14_3d_Camera")
	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		cp -fr "$CURDIR/../../../../IndieLib_resources/trex dx" $BUILDPATH/assets/IndieLib_resources/.
		$ADB push "$CURDIR/../../../../IndieLib_resources/trex dx" "/mnt/sdcard/IndieLib_resources/trex dx"
	fi
	arr=("Test" "SimpleTexture")
	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		cp -f $CURDIR/../../../../IndieLib_resources/test.bmp $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/test.bmp /mnt/sdcard/IndieLib_resources/.
	fi
	if [ "'''+ccc.getmainphoneproject()+'''" == "2DAnimation" ]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/2DAnimation $BUILDPATH/assets/IndieLib_resources/.
		cp -fr $CURDIR/../../../../IndieLib_resources/IndieLib-Animation-Collision $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/2DAnimation /mnt/sdcard/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/IndieLib-Animation-Collision /mnt/sdcard/IndieLib_resources/.
	fi

 	if [ "X" == "Y" ]; then
 	#chmod -R a+r $BUILDPATH/assets
	find $BUILDPATH/assets -type d -exec chmod a+x {} \;
	find $BUILDPATH/assets -type d -exec chmod a+r {} \;
	find $BUILDPATH/assets -type f -exec chmod a+r {} \;
	find $BUILDPATH/assets -type d -exec chmod a+w {} \;
	find $BUILDPATH/assets -type f -exec chmod a+w {} \;
	fi
}

NDK1()
{
#echo "mainphoneproject2:'''+ccc.getmainphoneproject()+'''"
#exit 0

ln -s $CURDIR/../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK $BUILDPATH/jni

ln -s $CURDIR/../Samples $BUILDPATH/jni
ln -s $CURDIR/../Dependencies $BUILDPATH/jni
ln -s $CURDIR/../../$NDK $BUILDPATH/jni

mkdir -p $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/otl $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/hlsl2glslfork $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/assimp-3.2 $BUILDPATH/jni/Libraries

ln -s $CURDIR/../Libraries/BoostWorkaround $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/libzip $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/spine-2.3 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/lua-5.3.2 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/STX $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/Effects11 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/MeshRenderer2 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/Framework3 $BUILDPATH/jni/Libraries
#ln -s $CURDIR/../Libraries/MVC $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/tinyxml $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/FW3 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/NSGUI $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/openal-soft-1.15.1 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/IndieLib $BUILDPATH/jni/Libraries
ln -s $CURDIR/../Libraries/Image3 $BUILDPATH/jni/Libraries

}

CURDIR0=`pwd -P`
CURDIR="$CURDIR0/../Libraries/MVC/NDK"

APPPATH="$CURDIR0/../Libraries/MVC/NDK"

NDKBUILD=`which ndk-build`
if [ -z "$NDKBUILD" ];then
    echo "Could not find the ndk-build utility, install Android's NDK and add it to the path"
    exit 1
fi

ANDROID=`which android`
if [ -z "$ANDROID" ];then
    echo "Could not find the android utility, install Android's SDK and add it to the path"
    exit 1
fi

ANT=`which ant`
if [ -z "$ANT" ];then
    echo "Could not find the ant utility, install ant and add it to the path"
    exit 1
fi

ADB=`which adb`
if [ -z "$ADB" ];then
    echo "Could not find the adb utility, install Android's SDK and add it to the path"
    exit 1
fi

NCPUS="1"
case "$OSTYPE" in
    darwin*)
        NCPU=`sysctl -n hw.ncpu`
        ;; 
    linux*)
        if [ -n `which nproc` ]; then
            NCPUS=`nproc`
        fi  
        ;;
  *);;
esac

APP="'''+apackage+'''"
APPARR=(${APP//./ })
BUILDPATH="$APPPATH"

export NDK_PROJECT_PATH="$BUILDPATH"

CURDIR1=`pwd -P`

mkdir -p $APPPATH/jni/MVC
mkdir -p $APPPATH/libs/x86

# ../build/com.yourcompany.yourapp/libs/armeabi-v7a/libmain.so
# ../build/com.yourcompany.yourapp/libs/x86/libmain.so
# ../build/com.yourcompany.yourapp/libs/armeabi/libmain.so

#echo "cp $CURDIR0/../bin/lib*.so $APPPATH/libs/x86"
cp $CURDIR0/../bin/lib*.so $APPPATH/libs/x86

cp $CURDIR0/NDK_gl_code2/AndroidManifest.xml $APPPATH
cp $CURDIR0/NDK_gl_code2/Android.mk $APPPATH/jni
cp $CURDIR0/NDK_gl_code2/Application.mk $APPPATH/jni

cp $CURDIR0/NDK_gl_code2/android_fopen.h $APPPATH/jni/MVC

cp $CURDIR0/NDK_gl_code2/hello-gl2/default.properties $APPPATH
cp -fr $CURDIR0/NDK_gl_code2/hello-gl2/res $APPPATH  
cp -fr $CURDIR0/NDK_gl_code2/hello-gl2/src $APPPATH

cd "$CURDIR"

NDK1;
assets;

rm -fr $APPPATH/jni/SDL2

cat > "$APPPATH/jni/Android.mk" <<HERE1
include \$(call all-subdir-makefiles)
HERE1

sed -i.bu "s|>GL2JNI<|>GL2JNI_'''+ccc.getmainphoneproject()+'''_$num<|g" $APPPATH/res/values/strings.xml
rm -f $APPPATH/res/values/strings.xml.bu

cd $BUILDPATH
$ANDROID update project --path . --target "android-19" --subprojects
	'''

	runscript(androidbuild)
	
	androidbuild3='''#!/bin/bash
source ../make.cnf

CURDIR0=`pwd -P`
CURDIR="$CURDIR0/../Libraries/MVC/NDK"
cd "$CURDIR"
#SDLPATH="$( cd "$(dirname "$0")/.." ; pwd -P )"
SDLPATH="$CURDIR0/../Libraries/MVC/NDK"

APP="'''+apackage+'''"
APP="GL2JNIActivity"
BUILDPATH="$SDLPATH"

export NDK_PROJECT_PATH="$BUILDPATH"

APK="$BUILDPATH/bin/$APP-debug.apk"

NDKBUILD=`which ndk-build`
if [ -z "$NDKBUILD" ];then
    echo "Could not find the ndk-build utility, install Android's NDK and add it to the path."
    exit 1
fi

ANT=`which ant`
if [ -z "$ANT" ];then
    echo "Could not find the ant utility, install ant and add it to the path."
    exit 1
fi

NCPUS="1"
case "$OSTYPE" in
    darwin*)
        NCPU=`sysctl -n hw.ncpu`
        ;; 
    linux*)
        if [ -n `which nproc` ]; then
            NCPUS=`nproc`
        fi  
        ;;
  *);;
esac

cd $NDK_PROJECT_PATH
$NDKBUILD V=1 -j $NCPUS $NDKARGS | tee $CURDIR0/../NDKbuild.log
#$NDKBUILD V=1 -j $NCPUS $NDKARGS | tee $CURDIR0/../NDKbuild.log
$ANT debug

cd $CURDIR

APK="$BUILDPATH/bin/$APP-debug.apk"

if [ -f "$APK" ]; then
    echo "Your APK is ready at $APK"
    echo "To install to your device: "
    cd  $BUILDPATH
    $ANT debug install
    exit 0
fi

echo "There was an error building the APK"
exit 1
	'''

	runscript(androidbuild3)

def do_cmd(a, afile='', files=[], cmd='', alink=False, aarch='i386'):

	#print afile
	file,ext=os.path.splitext(afile)
	cmd2=cmd

	CFLAGS=''
	COBJFLAGS = ' -w -fnested-functions'
	CXXFLAGS = ''
	CPPFLAGS = ''
	LDFLAGS=''

	LIBDIR='../../IndieLibX_Libraries/IndieLibX_AllLibs_LNX32'
	PIC3 = ''
	if buildtool in ['NDK']:
		PIC3 = '-fPIC'
	PTHREADS = ''
	CGLIBS = ''
	if buildtool in ['LNX','OSX']:
		if os.path.exists('/usr/include/pthread.h'):
			PTHREADS = '-pthread'

		if os.path.exists('/usr/include/Cg/cg.h'):
			CGLIBS = '-lCg -lCgGL'

	iOS_SDK=g_iOS_VERSION
	OSX_SDK=g_MACOSX_VERSION

	if not ccc.getDebug():
		CFLAGS += ' -DNDEBUG'
	else:	
		CFLAGS += ' -D_DEBUG -DDEBUG'
	str=' -fmax-errors=%d $(PIC3)' % ccc.getmaxerrors()

	if buildtool in ['iOS','OSX']:
		str+=' -fvisibility=hidden -fvisibility-inlines-hidden'

	if not buildtool=='NDK':
		CFLAGS +=' -w '+ccc.getG()+' '+ccc.getO()+str
		LDFLAGS +=' '+ccc.getG()+' '+ccc.getO()
	else:
		CFLAGS +=' -w '+str

	if (buildtool=='LNX') and (getOS()=='Debian'):
		CFLAGS += ' -DOS_Debian'
	if(getOS() in ['Ubuntu', 'Debian', 'Linux']) and (buildtool=='LNX'):
		CXXFLAGS += ' -std=gnu++0x -fexceptions -fnon-call-exceptions'
		CPPFLAGS += ' -std=gnu++0x -fexceptions -fnon-call-exceptions'
	
	elif buildtool=='NDK':
		CFLAGS += ' -fexceptions -fpermissive'
		LIBDIR='../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK'
	elif buildtool=='iOS':
		CFLAGS += ' -Wno-write-strings'
		CFLAGS += ' -Wno-unused-value'
		CFLAGS += ' -Wno-c++11-narrowing'
	#else:
	#	CXXFLAGS = CFLAGS
	#	CPPFLAGS = CFLAGS

	if buildtool in ['iOS','NDK']:
		CFLAGS += ' -DUSE_GLES_SHADERS'
	else:
		CFLAGS += ' -DUSE_HLSL_SHADERS'
	
	if (buildtool=='NMAKE') and ccc.getwin8():
		if ccc.getwin8app()=='DesktopApplication':
			CFLAGS+=' /D\"WIN8_APP\"'
		else:
			CFLAGS+=' /D\"WIN_PHONE_APP\"'

	if((getOS()=='Windows') and (buildtool=='NMAKE')):

		#cmd2=cmd2.replace('$(FLAGS)',CFLAGS)
		
		CFLAGS+=' /D\"_MSC_VER=1600\"'
		if ccc.getbitness()==32: 
			CFLAGS+=' /D\"WIN32\"' 
			CFLAGS+=' /D\"_WIN32\"' 
			CFLAGS+=' /D\"__WIN32__\"'
			CFLAGS+=' /D\"__32_BIT__\"'
		elif ccc.getbitness()==64: 
			CFLAGS+=' /D\"WIN64\"' 
			CFLAGS+=' /D\"_WIN64\"' 
			CFLAGS+=' /D\"__WIN64__\"'
			CFLAGS+=' /D\"__64_BIT__\"'
	
	    	if ccc.getDebug():
			CFLAGS+=' /D\"DEBUG\"' 
			CFLAGS+=' /D\"_DEBUG\"'
		else:
			CFLAGS+=' /D\"NDEBUG\"' 

		if ccc.getgles2():
			CFLAGS+=' /D\"USE_GLES_SHADERS\"'
		else:
			CFLAGS+=' /D\"USE_HLSL_SHADERS\"'

		if ccc.getd3d11(): 
			CFLAGS+=' /D\"D3D11\"'
		if ccc.getd3d10(): 
			CFLAGS+=' /D\"D3D10\"'
		if ccc.getd3d9(): 
			CFLAGS+=' /D\"D3D9\"'
		if ccc.getWindowsKits(): 
			CFLAGS+=' /D\"WindowsKits\"'
		CFLAGS+=' /D\"_HAS_ITERATOR_DEBUGGING=0\"'
		CFLAGS+=' /D\"_SECURE_SCL=0\"'
		CFLAGS+=' /D\"_SCL_SECURE_NO_WARNINGS\"'
		CFLAGS+=' /D\"_CRT_SECURE_NO_WARNINGS\"'
		CFLAGS+=' /D\"PROFILE\"'
		CFLAGS+=' /D\"D3DXFX_LARGEADDRESS_HANDLE\"'
	
		if ccc.getwin8():
			if ccc.getwin8app()=='DesktopApplication':
				CFLAGS+=' /D\"WIN8_APP\"'
			else:
				CFLAGS+=' /D\"WIN_PHONE_APP\"'
		#print a
	        if len(a.getflags())>0:
			CFLAGS+=' /D\"'+string.join(a.getflags(), '\" /D\"')
	
		if a.getuc()==True:
			CFLAGS+=' /D\"_UNICODE -DUNICODE\"'
		else:
			CFLAGS+=' /D\"_MBCS\"'
		
		v=[]
		for i in a.getincs():
			x=i.replace('/','\\')
			v.append(x)
	        r=''
	        if len(v)>0:
	        	if ccc.getwin8():
				CFLAGS+=' /I\".\\..\\Program Files (x86)\\Windows Kits\\8.1\\Include\\shared\" '
				CFLAGS+=' /I\".\\..\\Program Files (x86)\\Windows Kits\\8.1\\Include\\um\" '
				#CFLAGS+=' /I\".\\..\\Program Files (x86)\\Windows Phone Kits\\8.1\\Include\" '
	            	else:
		    		#CFLAGS+=' /I\".\\..\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Include\" '
		    		CFLAGS+=' /I\".\\..\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\" '
	            	CFLAGS+=' /I\".\\Libraries\\SDL2-'+SDL2version+'\\include\" '
		    	CFLAGS+=' /I\".\\Dependencies\\Cg\\include\" '
		    	if ccc.getgles2():
				CFLAGS+=' /I\".\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\include\" '
		    	else:
		    		CFLAGS+=' /I\".\\Dependencies\\OpenGL\\include\" '
		    	CFLAGS+=' /I\".\\Libraries\\BoostWorkaround\" '
		    	CFLAGS+=' /I\".\\Libraries\\openal-soft-1.15.1\\include\" '
	            	CFLAGS+=' /I\".\\'+string.join(v,'\" /I\"'+ccc.getINDIELIBROOT()+'\\')+'\" '

			
			v=[]
			r=' '
			for i in a.getlibs():
				v.append(i)
			for i in a.getsharedlibs():
				v.append(i)
			r+=join(v, '.lib ')+'.lib '
	
			x='SDL2.lib GDI32.lib ole32.lib Winmm.lib Ws2_32.lib Advapi32.lib User32.lib ComCtl32.lib cg.lib cgGL.lib cgD3D9.lib cgD3D10.lib cgD3D11.lib '
			x+='d3dcompiler.lib d3d11.lib dxguid.lib '
	
			if (not ccc.getwin8()) and (not ccc.getwindowsphone()):
				x+='d3d9.lib d3dx9.lib d3d10.lib d3d10_1.lib d3dx10d.lib d3dx10.lib d3dx11d.lib d3dx11.lib '
	
			if ccc.getwindowsphone():
				x+='d2d1.lib d3d11.lib dxgi.lib dwrite.lib WindowsPhoneCore.lib RuntimeObject.lib PhoneAppModelHost.lib '
			#elif mscver=='9.0' or mscver=='10.0':
			elif ccc.getwin8():
				x+='d2d1.lib d3d11.lib dxgi.lib dwrite.lib windowscodecs.lib '
	
			x+='OpenAL32.lib OPENGL32.lib '
			if ccc.getgles2():
				x+='libGLESv2.lib libEGL.lib '
			else:
				x+='glut32.lib GLU32.lib '
	
			r+=x
	
			r=r.replace('.lib .lib','.lib')
			r=r.replace('hlsl2glsl.lib','')	
			r=r.replace(' hlsl2glsl ',' ')	
			r2=r
	
			r=''
			if ccc.getwin8():
				r+=' /LIBPATH:\".\\..\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x86\"'
			else:
				r+=' /LIBPATH:\".\\..\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\"'
				r+=' /LIBPATH:\".\\..\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\"'
				
			r+=' /LIBPATH:\"'+ccc.getINDIELIBROOT()+'\\lib\"'
			r+=' /LIBPATH:\"'+os.path.expanduser("~")+'\\Documents\\lib\"'
			r+=' /LIBPATH:\"..\\..\\IndieLibX_Libraries\\IndieLibX_AllLibs\"'
	
			if ccc.getbitness()==64:
				r+=' /LIBPATH:\".\\Dependencies\\SDL2-'+SDL2version+'\\lib\\x64\"'
				r+=' /LIBPATH:\".\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win64\"'
			else:
				r+=' /LIBPATH:\".\\Dependencies\\SDL2-'+SDL2version+'\\lib\\x86\"'
				r+=' /LIBPATH:\".\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win32\"'
	
			r+=' /LIBPATH:\".\\Dependencies\\Cg\\lib\"'
		
			if ccc.getgles2():
				r+=' /LIBPATH:\".\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\lib\"'
			else:
				r+=' /LIBPATH:\".\\Dependencies\\OpenGL\\lib\"'
	
			r+=r2
		
			r=r.replace('.lib .lib','.lib')	
	
			alibs=r	

			# mkf+='OBJ_FILES = $(CPPSRCS:.cpp=.obj) $(CSRCS:.c=.obj)\n'
	
			cmd2=cmd2.replace('$(LIBRARIES)', alibs)

	else:
		if (buildtool=='NDK'):

			# x='-frtti -fexceptions'

			CFLAGS+=' -DANDROID'
			CFLAGS+=' -fno-strict-aliasing -finline-functions -ffunction-sections -funwind-tables -fmessage-length=0 -fno-inline-functions-called-once -fgcse-after-reload -frerun-cse-after-loop -frename-registers -no-canonical-prefixes -fomit-frame-pointer -funswitch-loops -finline-limit=300'
			CFLAGS+=' -O2 -g -Wa,--noexecstack -Wformat -Werror=format-security -fno-exceptions -fno-rtti'
			# pre='../../android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-'
			pre='../../android-ndk-r9d/toolchains/x86-4.6/prebuilt/linux-x86/bin/i686-linux-android-'
			post=' --sysroot=../../android-ndk-r9d/platforms/android-19/arch-x86'

	    		cmd2=cmd2.replace('$(GPP)', pre+'g++'+post)
    			cmd2=cmd2.replace('$(GCC)', pre+'gcc'+post)
    			cmd2=cmd2.replace('$(CC)', pre+'cc'+post)
	    		cmd2=cmd2.replace('$(AR)', pre+'ar'+post)
    			cmd2=cmd2.replace('$(LD)', pre+'ld'+post)
    			cmd2=cmd2.replace('$(RANLIB)', pre+'ranlib'+post)

			CFLAGS += ' -I../../android-ndk-r9d/sources/cxx-stl/stlport/stlport'
			CFLAGS += ' -I../../android-ndk-r9d/sources/cxx-stl/gabi++/include'
			
			CFLAGS += ' -I'+ccc.getINDIELIBROOT()+'/Libraries/MVC'
			CFLAGS += ' -I'+ccc.getINDIELIBROOT()+'/Libraries/SDL2-'+SDL2version
			CFLAGS += ' -I'+ccc.getINDIELIBROOT()+'/Libraries/MVC/NDK/jni'
			CFLAGS += ' -I'+ccc.getINDIELIBROOT()+'/Libraries/BoostWorkaround' 
			CFLAGS += ' -I'+ccc.getINDIELIBROOT()+'/DesktopProjects/NDK_gl_code2'

			CFLAGS += ' -include "android_fopen.h"'
			#CFLAGS += ' -isysroot ../../android-ndk-r9d/platforms/android-19/arch-x86'

			#print a.getname()+' '+a.gettype()
			#if not a.gettype()=='static_library':
			#	#LDFLAGS += ' -isysroot ../../android-ndk-r9d/platforms/android-19/arch-x86'
			#	#LDFLAGS += ' -ldl -lOpenSLES -lGLESv1_CM -lGLESv2 -lGLESv3 -lEGL -llog -landroid -lstdc++ -lc -lm -Wl,--undefined=Java_org_libsdl_app_SDLActivity_nativeInit'
				
			cmd2=cmd2.replace('$(FRAMEWORKS)','') 
			
		elif(getOS() in ['Ubuntu', 'Debian', 'Linux']) and (buildtool=='LNX'):
			CFLAGS+=' -DLINUX -D_LINUX'
	    		cmd2=cmd2.replace('$(GPP)','/usr/bin/g++')
    			cmd2=cmd2.replace('$(GCC)','/usr/bin/gcc')
    			cmd2=cmd2.replace('$(CC)','/usr/bin/cc')
	    		cmd2=cmd2.replace('$(AR)','/usr/bin/ar')
    			cmd2=cmd2.replace('$(LD)','/usr/bin/ld')
    			cmd2=cmd2.replace('$(RANLIB)','/usr/bin/ranlib')
			cmd2 = cmd2.replace('$(FRAMEWORKS)','')

		elif(getOS()=='Darwin') and (buildtool=='OSX'):
			CFLAGS+=' -D__APPLE__'
   	 		cmd2=cmd2.replace('$(GPP)','xcrun --sdk macosx'+g_MACOSX_VERSION+' g++ $(ARCH)')
   	 		cmd2=cmd2.replace('$(GCC)','xcrun --sdk macosx'+g_MACOSX_VERSION+' gcc $(ARCH)')
   	 		cmd2=cmd2.replace('$(CC)','xcrun --sdk macosx'+g_MACOSX_VERSION+' cc $(ARCH)')
   	 		cmd2=cmd2.replace('$(AR)','xcrun --sdk macosx'+g_MACOSX_VERSION+' ar')
   	 		cmd2=cmd2.replace('$(LD)','xcrun --sdk macosx'+g_MACOSX_VERSION+' ld')
   	 		cmd2=cmd2.replace('$(RANLIB)','xcrun --sdk macosx'+g_MACOSX_VERSION+' ranlib')

			CFLAGS += ' -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX'+g_MACOSX_VERSION+'.sdk'
			LDFLAGS += ' -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX'+g_MACOSX_VERSION+'.sdk'
			LDFLAGS += ' -L/usr/local/lib -L/usr/X11R6/lib'

			if ccc.getbitness()==64:
				cmd2=cmd2.replace('$(ARCH)','-arch x86_64 -mmacosx-version-min=10.6')
			else:			
				cmd2=cmd2.replace('$(ARCH)','-arch i386 -mmacosx-version-min=10.6')
			LIBDIR='../../IndieLibX_Libraries/IndieLibX_AllLibs_OSX64'

		elif(getOS()=='Darwin') and (buildtool=='iOS'):
			iPhone=''
			iphone=''
			arch=aarch
			
			iPhone='iPhoneOS'
			iphone='iphoneos'
			CFLAGS_ = ' -DOS_IPHONE'
			if arch=='i386':
				iPhone='iPhoneSimulator'
				iphone='iphonesimulator'
				CFLAGS_=' -DIPHONE_SIMULATOR'
			CFLAGS += CFLAGS_

   	 		cmd2=cmd2.replace('$(GPP)','xcrun --sdk '+iphone+g_iOS_VERSION+' g++ $(IOSARCH) -std=gnu++0x')
   	 		cmd2=cmd2.replace('$(GCC)','xcrun --sdk '+iphone+g_iOS_VERSION+' gcc $(IOSARCH)')
   	 		cmd2=cmd2.replace('$(CC)','xcrun --sdk '+iphone+g_iOS_VERSION+' cc $(IOSARCH)')
   	 		cmd2=cmd2.replace('$(AR)','xcrun --sdk '+iphone+g_iOS_VERSION+' ar')
   	 		cmd2=cmd2.replace('$(LD)','xcrun --sdk '+iphone+g_iOS_VERSION+' ld')
   	 		cmd2=cmd2.replace('$(RANLIB)','xcrun --sdk '+iphone+g_iOS_VERSION+' ranlib')
			
			cmd2=cmd2.replace('$(IOSARCH)','-arch '+arch+' -miphoneos-version-min=7.0')

			DEV = '/Applications/Xcode.app/Contents/Developer/Platforms/'+iPhone+'.platform/Developer'
			CFLAGS += ' -stdlib=libc++'
			CFLAGS += ' -isysroot "'+DEV+'/SDKs/'+iPhone+g_iOS_VERSION+'.sdk"'
			LDFLAGS += ' -isysroot "'+DEV+'/SDKs/'+iPhone+g_iOS_VERSION+'.sdk"'

		elif buildtool=='MW':			
			cmd2 = cmd2.replace('$(FRAMEWORKS)','')

			CXXFLAGS += ' -std=gnu++0x -fexceptions -fnon-call-exceptions'
			CPPFLAGS += ' -std=gnu++0x -fexceptions -fnon-call-exceptions'
			CFLAGS += ' -Wno-enum-compare'
			CFLAGS += ' -Wno-comment'
	
			CFLAGS+=' -D_MSC_VER=1600'
			if ccc.getbitness()==32: 
				CFLAGS+=' -DWIN32' 
				CFLAGS+=' -D_WIN32' 
				CFLAGS+=' -D__WIN32__'
				CFLAGS+=' -D__32_BIT__'
				CFLAGS+=' -D__MINGW32__'
			elif ccc.getbitness()==64: 
				CFLAGS+=' -DWIN64' 
				CFLAGS+=' -D_WIN64' 
				CFLAGS+=' -D__WIN64__'
				CFLAGS+=' -D__64_BIT__'
				CFLAGS+=' -D__MINGW64__'

   	 		cmd2=cmd2.replace('$(GPP)','/usr/bin/i686-w64-mingw32-g++ $(ARCH)')
   	 		cmd2=cmd2.replace('$(GCC)','/usr/bin/i686-w64-mingw32-gcc $(ARCH)')
   			cmd2=cmd2.replace('$(CC)','/usr/bin/i686-w64-mingw32-gcc $(ARCH)')
   	 		cmd2=cmd2.replace('$(AR)','/usr/bin/i686-w64-mingw32-ar')
   	 		cmd2=cmd2.replace('$(LD)','/usr/bin/i686-w64-mingw32-ld')
   	 		cmd2=cmd2.replace('$(RANLIB)','/usr/bin/i686-w64-mingw32-ranlib')

	cmd2=cmd2.replace('$(LIBDIR)',LIBDIR)
	cmd2=cmd2.replace('$(ARCH)','')

	if a.gettype()=='application':
		if getOS()=='Darwin':
			cmd2=cmd2.replace('$(FILEEXT)','.app')
			cmd2=cmd2.replace('$(TARGET)',ccc.getINDIELIBROOT()+'/'+a.getexedir()+'/'+a.getname()+'.app/Contents/MacOS/'+a.getname()+'')
		else:
			cmd2=cmd2.replace('$(FILEEXT)','.bin')
			cmd2=cmd2.replace('$(TARGET)',ccc.getINDIELIBROOT()+'/'+a.getexedir()+'/'+a.getname()+'.bin')
	
	elif a.gettype()=='static_library':
		cmd2=cmd2.replace('$(FILEEXT)','.a')
		cmd2=cmd2.replace('$(TARGET)',ccc.getINDIELIBROOT()+'/lib/lib'+a.getname()+'.a')
	elif a.gettype()=='shared_library':
		cmd2=cmd2.replace('$(FILEEXT)','.so')
		PIC3 = '-fPIC'
		if getOS()=='Darwin':
			cmd2=cmd2.replace('$(TARGET)',ccc.getINDIELIBROOT()+'/'+a.getexedir()+'/'+a.getname()+'.app/Contents/MacOS/lib'+a.getname()+'.so')
		else:
			cmd2=cmd2.replace('$(TARGET)',ccc.getINDIELIBROOT()+'/'+a.getexedir()+'/lib'+a.getname()+'.so')

	if(getOS()=="Debian"):
		txt='Linux%d' % ccc.getbitness()
		cmd2=cmd2.replace('$(LibDir)','Linux'+txt)
		if ccc.getgles2():
			cmd2=cmd2.replace('$(OPENGL)','-lGLESv2 -lGL -lGLU')
		else:
			cmd2=cmd2.replace('$(OPENGL)','-lGL -lGLU')
		cmd2=cmd2.replace('$(SDL)','-lSDL2 -lSDL2_image -lopenal')
		cmd2=cmd2.replace('$(X11)','-lX11')
	elif(getOS()=="Darwin") and (buildtool=='OSX'):
		CPLUSPLUS=' -D__cplusplus '
		cmd2=cmd2.replace('$(LibDir)','Mac64')
		cmd2=cmd2.replace('$(SDL)','-lSDL2 -lSDL2_image -lX11')
		cmd2=cmd2.replace('$(FRAMEWORKS)','-framework OpenGL -framework OpenAL')
		cmd2=cmd2.replace('$(OPENGL)','')
		cmd2=cmd2.replace('$(X11)','')
	elif(getOS()=="Darwin") and (buildtool=='iOS'):
		cmd2=cmd2.replace('$(FRAMEWORKS)','-framework CoreMotion -framework GameController -framework OpenGLES -framework UIKit -framework CoreGraphics -framework OpenAL -framework CoreAudio -framework AudioToolbox -framework QuartzCore -framework Foundation -framework CoreFoundation')
		cmd2=cmd2.replace('$(OPENGL)','')
		cmd2=cmd2.replace('$(SDL)','')
		cmd2=cmd2.replace('$(X11)','')

	elif not buildtool=='NDK':
		if ccc.getgles2():
			cmd2=cmd2.replace('$(OPENGL)','-lGLESv2 -lGL -lGLU')
		else:
			cmd2=cmd2.replace('$(OPENGL)','-lGL -lGLU')
		cmd2=cmd2.replace('$(SDL)','-lSDL2 -lSDL2_image -lopenal')
		cmd2=cmd2.replace('$(X11)','-lX11')
		txt='Linux%d' % ccc.getbitness()
		cmd2=cmd2.replace('$(LibDir)',txt)

	elif buildtool=='NDK':
		cmd2=cmd2.replace('$(OPENGL)','')
		cmd2=cmd2.replace('$(SDL)','')
		cmd2=cmd2.replace('$(X11)','')
		txt='Linux%d' % ccc.getbitness()
		cmd2=cmd2.replace('$(LibDir)',txt)

	cmd2=cmd2.replace('$(DEL)','/bin/rm -fr')
	cmd2=cmd2.replace('$(NAME)',a.getname())

	#ccc.getG()ccc.getO()
	if not buildtool=='NMAKE':
		CFLAGS +=' '+getflagsincs(a)
		if (buildtool=='NDK'):
			CFLAGS +=' -frtti -fexceptions'
	

	COBJFLAGS += CFLAGS
	CPPFLAGS += CFLAGS
	CXXFLAGS += CFLAGS
    	cmd2=cmd2.replace('$(CFLAGS)', CFLAGS)
    	cmd2=cmd2.replace('$(CPPFLAGS)', CPPFLAGS)
    	cmd2=cmd2.replace('$(CXXFLAGS)', CXXFLAGS)
    	cmd2=cmd2.replace('$(COBJFLAGS)', COBJFLAGS)
    	cmd2=cmd2.replace('$(LDFLAGS)', LDFLAGS)

	if False:
		print 'CFLAGS='+CFLAGS
		print 'COBJFLAGS='+COBJFLAGS
		print 'CXXFLAGS='+CXXFLAGS
		print 'CPPFLAGS='+CPPFLAGS
		print 'LDFLAGS='+LDFLAGS
		exit(0)

	if((getOS()=='Windows') and (buildtool=='NMAKE')):
		cmd2=cmd2.replace('$(FLAGS)',CFLAGS)

	if(getOS()=='Darwin'):
		cmd2 = cmd2.replace('$(iOS_VERSION)', g_iOS_VERSION)
		cmd2 = cmd2.replace('$(MACOSX_VERSION)', g_MACOSX_VERSION)
		cmd2 = cmd2.replace('$(iOS_SDK)', g_iOS_SDK)
		cmd2 = cmd2.replace('$(OSX_SDK)', g_OSX_SDK)
		cmd2 = cmd2.replace('$(XCODE)', g_XCODE)

	cmd2 = cmd2.replace('$(CGLIBS)',CGLIBS)
	cmd2 = cmd2.replace('$(PTHREADS)',PTHREADS)
	cmd2=cmd2.replace('$(PIC3)',PIC3)

	#if not file=='':
	#	cmd2=cmd2.replace('$@', ccc.getINDIELIBROOT()+'/'+file+'.o')
	#	cmd2=cmd2.replace('$<', ccc.getINDIELIBROOT()+'/'+file+ext)

	cmd2 = ' '.join(cmd2.split())
	cmd2=cmd2.replace('//','/')

	ossystem(cmd2)
    	return

def do_cmd2(cmd2):
	ossystem(cmd2)
    	return

def bootstrap(aproject, aarch='i386'):
	#print 'bootstrap:'+aproject.getname()

	objs=[]
	delfiles=[]
	files=[]
	srcs=aproject.getsrcs()

	LIBS = ' -lstdc++ -lm'
	if buildtool=='NDK':
		LIBS = ' -ldl -lOpenSLES -lGLESv1_CM -lGLESv2 -lGLESv3 -lEGL -llog -landroid -lstdc++ -lc -lm'

	for i in range(0,len(srcs)):
		head,tail=os.path.split(srcs[i])
		main,ext=os.path.splitext(srcs[i])
		file=ccc.getINDIELIBROOT()+'/'+main
		if (getOS()=='Windows') and (buildtool=='NMAKE'):
			objs.append(file+'.obj')
		else:
			objs.append(file+'.o')
		delfiles.append(file+ext+'~')
		files.append(file+ext)

		narrowing=''
		if((getOS()=='Darwin') and (buildtool=='OSX')):
			narrowing='-std=c++11 -Wno-c++11-narrowing'

		if((getOS()=='Windows') and (buildtool=='NMAKE')):
			
			do_cmd(aproject, srcs[i], [], "cl.exe $(FLAGS) /Od /Zi /EHsc /MDd /Fo\""+ccc.getINDIELIBROOT()+head.replace('/','\\')+"\\\\\" /c \""+(ccc.getINDIELIBROOT()+srcs[i]).replace('/','\\')+"\"")
		else:
			if srcs[i].strip().endswith('.c'):
				do_cmd(aproject, srcs[i], [], "$(GCC) $(CFLAGS) -o "+objs[i]+" -c "+files[i])
			elif (srcs[i].strip().endswith('.cc')):
				do_cmd(aproject, srcs[i], [], "$(GPP) "+narrowing+" -fexceptions -fnon-call-exceptions "+CPLUSPLUS+"$(CPPFLAGS) -o "+objs[i]+" -c "+files[i])
			elif (srcs[i].strip().endswith('.cpp')):
				do_cmd(aproject, srcs[i], [], "$(GPP) "+narrowing+" -fexceptions -fnon-call-exceptions "+CPLUSPLUS+"$(CPPFLAGS) -o "+objs[i]+" -c "+files[i])
			elif (srcs[i].strip().endswith('.cxx')):
				do_cmd(aproject, srcs[i], [], "$(GPP) "+narrowing+" -fexceptions -fnon-call-exceptions "+CPLUSPLUS+"$(CXXFLAGS) -o "+objs[i]+" -c "+files[i])
			elif ((getOS()=='Darwin') and srcs[i].strip().endswith('.m')):
				do_cmd(aproject, srcs[i], [], "$(GCC) $(COBJFLAGS) -fnested-functions "+objs[i]+" -c "+files[i])
			elif ((getOS()=='Darwin') and srcs[i].strip().endswith('.mm')):
				do_cmd(aproject, srcs[i], [], "$(GCC) $(COBJFLAGS) -fnested-functions "+objs[i]+" -c "+files[i])

	if (getOS()=='Windows') and (buildtool=='NMAKE'):
		objsstr=join(objs,' ')
		if aproject.gettype()=='static_library':
			do_cmd(aproject, '', files, 'lib.exe /out:$(NAME).lib '+objsstr+' $(LIBRARIES)')
		elif aproject.gettype()=='shared_library':
			do_cmd(aproject, '', files, 'link.exe /DLL /out:$(NAME).dll /debug '+objsstr+' $(LIBRARIES)')
		else:
	   		do_cmd(aproject, '', files, 'link.exe /out:$(NAME).exe /debug '+objsstr+' $(LIBRARIES)')
	else:

		v=[]
		v.append('-L'+ccc.getINDIELIBROOT()+'/lib')
		if buildtool in ['LNX','OSX']:
			v.append('-L/usr/local/lib')
		elif buildtool in ['NDK']:
			v.append('-L../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK')
			#v.append('-L../../android-ndk-r9d/platforms/android-19/arch-x86/usr/lib')

		elif buildtool in ['iOS']:
			v.append('-L../../IndieLibX_Libraries/IndieLibX_AllLibs_iOS')
		for i in aproject.getlibs():
			v.append('-l'+i)
		for i in aproject.getsharedlibs():
		        v.append('-l'+i)
		alibs=join(v, ' ')

		#do_link(aproject, '', files, 'cmd, alink=True)
		objsstr=join(objs,' ')

		if aproject.gettype()=='static_library':
			do_cmd(aproject, '', files, '$(AR) -cr $(TARGET) '+objsstr)
			do_cmd(aproject, '', files, '$(RANLIB) $(TARGET)')
		elif aproject.gettype()=='shared_library' or ((buildtool=='NDK') and (not aproject.gettype()=='static_library')):
			if False: # buildtool=='NDK':
				do_cmd(aproject, '', files, '$(GPP) -Wl,-soname,lib'+aproject.getname()+'.so -shared '+objsstr+' -lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now '+alibs+' $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREADS) $(X11) '+LIBS+' -o ../bin/lib'+aproject.getname()+'.so')
			else:
				do_cmd(aproject, '', files, '$(GCC) -shared $(LDFLAGS) '+objsstr+' '+alibs+' $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREADS) $(X11) '+LIBS+' -o ../bin/lib'+aproject.getname()+'.so')
		else:
			do_cmd(aproject, '', files, '$(GCC) $(LDFLAGS) '+objsstr+' '+alibs+' $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREADS) $(X11) '+LIBS+' -o $(TARGET)')

	if getOS() in ['Ubuntu', 'Debian', 'Linux', 'Darwin']:
		do_cmd2('/bin/rm -f '+string.join(delfiles, ' '))
		do_cmd2('/bin/rm -f '+objsstr)
	elif (getOS()=='Windows') and (buildtool=='NMAKE'):
		do_cmd2('DEL /S /Q '+objsstr)

	return

def bootstrapNDK_(buildsln, buildprj, buildtool, apackage):
	NDK_gl_code2='''#!/bin/bash
#
# Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
source ../make.cnf

PYTHON=`which python`
if [ -z "$PYTHON" ]; then
	echo "Could not find python."
	exit 1
fi

OS='Windows'
platformsArray=( NDK NMAKE VC MW PRO CMake )
makesystem="NMAKE"
if [ -f "/etc/lsb-release" ]; then
	DINFO=$(cat /etc/lsb-release | grep DISTRIB_ID | sed s/.*=//)
	if [ $DINFO == "Ubuntu" ]; then
		OS='Ubuntu'
		platformsArray=( NDK LNX MW PRO CMake )
		makesystem="LNX"
	fi
elif [ -f "/etc/debian_version" ]; then
	OS='Debian'
	platformsArray=( NDK LNX MW PRO CMake )
	makesystem="LNX"
elif [ -d "/Applications/Xcode.app" ]; then
	OS='Darwin'
	platformsArray=( NDK LNX iOS OSX MW PRO CMake )
	makesystem='OSX'
elif [ -d "/Applications/Xcode-Beta.app" ]; then
	OS='Darwin'
	platformsArray=( NDK LNX iOS OSX MW PRO CMake )
	makesystem='OSX'
fi

Bit32p64=''
if [ $(uname -m) == "i686" ]; then
	Bit32p64='i686'
fi

export OS
export Bit32p64

solution='''+buildsln+'''
project='''+buildprj+'''
buildtool='''+buildtool+'''
package='''+apackage+'''

NDK_gl_code2()
{
	JNI="./Libraries/MVC/NDK/build/$package/jni"
	mkdir -p $JNI
	echo "#2a:$PYTHON Projects.py --solution '''+buildsln+''' --buildtool NDK"
	$PYTHON Projects.py --solution '''+buildsln+''' --buildtool NDK
	echo "#2b:$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject '''+buildprj+''' --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19"
	$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject '''+buildprj+''' --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19
	cd ./NDK_gl_code2
	./androidbuild.sh $package ../test/testgles2.c
	cd ..
	rm -fr $JNI/src
	cd ./NDK_gl_code2
	./androidbuild3.sh $package
	cd ..
}

NDK_gl_code2_()
{
	JNI="./Libraries/MVC/NDK/build/$package/jni"
	mkdir -p $JNI
	echo "#2a:$PYTHON Projects.py --solution '''+buildsln+''' --buildtool NDK"
	$PYTHON Projects.py --solution '''+buildsln+''' --buildtool NDK
	echo "#2b:$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject '''+buildprj+''' --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19"
	$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject '''+buildprj+''' --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19
	cd ./NDK_gl_code2
	./androidbuild.sh $package ../test/testgles2.c
	cd ..
	rm -fr $JNI/src
	cd ./NDK_gl_code2
	./androidbuild3.sh $package
	cd ..
}

NDK_gl_code2_
	'''
	runscript(NDK_gl_code2)

def exists(aproject):

	if buildtool in ['iOS','NMAKE','MW']:
		return False

	#if buildprj=='None':
	f1='/usr/lib/lib'+aproject.getname()
	f2='../../IndieLibX_Libraries/IndieLibX_AllLibs_LNX32/lib'+aproject.getname()
	if ccc.getbitness()==64:
		f2='../../IndieLibX_Libraries/IndieLibX_AllLibs_'+buildtool+'64/lib'+aproject.getname()

	if buildtool=='NDK':
		f1='../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK/lib'+aproject.getname()
		f2='../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK/lib'+aproject.getname()

	if buildprj=='None':
		if aproject.gettype()=='static_library':
			if os.path.isfile(f1+'.a') or os.path.isfile(f2+'.a'):
				print 'Passing static library: '+aproject.getname()
				return True
		elif aproject.gettype()=='shared_library':
			if os.path.isfile(f1+'.so') or os.path.isfile(f2+'.so'):
				print 'Passing dynamic library: '+aproject.getname()
				return True
	else:
		if not buildprj==aproject.getname():
			return False
	return False

def rrr(v):
	f=open('../run.sh','w')
	f.write('')
	ff=open('../run.bat','w')
	ff.write('')
	cc=0
	for i in v:
	        if i.gettype()=='application':
			filename=i.getname()
			#print 'XXX:'+filename
	        	exedir=i.getexedir()
	        	vv=exedir.split('/')
	        	ret='..'
	        	cret='..'
	        	c=0
			cexedir=exedir.replace('/','\\\\')
	        	for ii in vv:
	        		if c>0:
	        			ret=ret+'/..'
	        	   		cret=cret+'\\..'
	        		c=c+1
			
			Linuxfn='cd ./'+exedir+';./'+i.getname()+'.bin;cd '+ret+''
			if sys.platform=='darwin':
				Linuxfn='cd ./bin/'+i.getname()+'.app/Contents/MacOS;./'+i.getname()+';cd ../../../..'

			if not cc==0:
				if getOS()=='Darwin':
					f.write(' &&\n'+Linuxfn)
				else:
					f.write(' &&\n'+Linuxfn)
			else:
	        	    	if getOS()=='Darwin':
					f.write(Linuxfn) 
				else:
					f.write(Linuxfn)  
	        	ff.write('cd '+cexedir+'\ncall '+i.getname()+'.exe\ncd '+cret+'\n') 
	    		cc=cc+1
	f.close()
	ff.close()
	make_executable('../run.sh')

def cnf2(accc):
	iOS='iPhoneOS'
	if accc.getiPhoneSimulator():
		iOS='iPhoneSimulator'

	NDKarmeabi='False'
	if accc.getNDKarmeabi():
		NDKarmeabi='True'
	NDKx86='False'
	if accc.getNDKx86():
		NDKx86='True'
	NDKmips='False'
	if accc.getNDKmips():
		NDKmips='True'

	if(getOS()=='Windows'):
		outtext='''rem This file is automatically generated by build.py.
rem Do not modify this file -- YOUR CHANGES WILL BE ERASED!
set iOSplatform='''+iOS+'''
set NDKplatform='''+accc.getNDKplatform()+'''
set NDKarmeabi='''+NDKarmeabi+'''
set NDKx86='''+NDKx86+'''
set NDKmips='''+NDKmips+'''
set solutions=\"'''+accc.getsolutions()+'''\"
set projects=\"'''+accc.getprojects()+'''\"
set platforms=\"'''+accc.getbuildtools()+'''\"
set solution='''+buildsln+'''
set mainphoneproject='''+accc.getmainphoneproject()+'''
set package='''+accc.getpackage()+'''
set domain=\"'''+accc.getDomain()+'''\"
set androidurl=\"'''+accc.getandroidURL()+'''\"
set sdlurl=\"'''+accc.getSDLURL()+'''\"
set adt=\"'''+accc.getADT()+'''\"
set assimp3version=\"'''+assimp3version+'''\"
set ndk=\"'''+NDK+'''\"
set sdl2version='''+SDL2version+'''
set sdlimage2version=\"'''+accc.getSDLIMAGE2version()+'''\"
set OSXversion=\"'''+accc.getOSXversion()+'''\"
set Debianversion=\"'''+accc.getDebianversion()+'''\"
set Ubuntuversion=\"'''+accc.getUbuntuversion()+'''\"
set pythonUbuntuversion=\"'''+accc.getpythonUbuntuversion()+'''\"
set pythonDebianversion=\"'''+accc.getpythonDebianversion()+'''\"
set pythonOSXversion=\"'''+accc.getpythonOSXversion()+'''\"'''
		output = open('../cnf.bat','w')		
		output.write(outtext)
		output.close()
	else:
		#print '#:62'
		outtext='''# This file is automatically generated by build.py.
# Do not modify this file -- YOUR CHANGES WILL BE ERASED!
iOSplatform='''+iOS+'''
NDKplatform='''+accc.getNDKplatform()+'''
NDKarmeabi='''+NDKarmeabi+'''
NDKx86='''+NDKx86+'''
NDKmips='''+NDKmips+'''
solutions=\"'''+accc.getsolutions()+'''\"
projects=\"'''+accc.getprojects()+'''\"
platforms=\"'''+accc.getbuildtools()+'''\"
solution='''+buildsln+'''
mainphoneproject='''+accc.getmainphoneproject()+'''
package='''+accc.getpackage()+'''
domain=\"'''+accc.getDomain()+'''\"
androidurl=\"'''+accc.getandroidURL()+'''\"
sdlurl=\"'''+accc.getSDLURL()+'''\"
adt=\"'''+accc.getADT()+'''\"
assimp3version=\"'''+assimp3version+'''\"
ndk=\"'''+NDK+'''\"
sdl2version='''+SDL2version+'''
sdlimage2version=\"'''+accc.getSDLIMAGE2version()+'''\"
OSXversion=\"'''+accc.getOSXversion()+'''\"
Debianversion=\"'''+accc.getDebianversion()+'''\"
Ubuntuversion=\"'''+accc.getUbuntuversion()+'''\"
pythonUbuntuversion=\"'''+accc.getpythonUbuntuversion()+'''\"
pythonDebianversion=\"'''+accc.getpythonDebianversion()+'''\"
pythonOSXversion=\"'''+accc.getpythonOSXversion()+'''\"'''
		output = open('../make.cnf','w')		
		output.write(outtext)
		output.close()

import subprocess

def bootstrapiOS(a):
	if a.gettype()=='static_library':
		if True:
			bootstrap(a, 'i386')
			return
		v=[]
		if ccc.getarm6():
			v.append('armv6')
		if ccc.getarm7():
			v.append('armv7')
		if ccc.geti386():
			v.append('i386')
		if ccc.getarm64():
			v.append('arm64')
		print v
		for arch in v:
			bootstrap(a, arch)
		v=[]
		txt1='Release'
		v.append(a.getname())
		if ccc.getarm6():
			v.append('y')
		else:
			v.append('n')
		if ccc.getarm7():
			v.append('y')
		else:
			v.append('n')
		if ccc.geti386():
			v.append('y')
		else:
			v.append('n')
		if ccc.getarm64():
			v.append('y')
		else:
			v.append('n')
		if ccc.getDebug():
			txt1='Debug'
			v.append('y')
		else:
			v.append('n')
		if ccc.getipad():
			v.append('y')
		else:
			v.append('n')

		print v
		createframework='''#!/bin/bash

createInfoplist() {

   if [ -n "$1" ]
   then
	FRAMEWORK_DIR=$1
   fi

   if [ -n "$2" ]
   then
	FRAMEWORK_NAME=$2
   fi

    #$FRAMEWORK_DIR/Resources/Info.plist
    if [ 1 ] ; then
        cat > "$FRAMEWORK_DIR/Resources/Info.plist" <<HERE1
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"\
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
        <key>CFBundleDevelopmentRegion</key>
        <string>English</string>
        <key>CFBundleDisplayName</key>
        <string>$FRAMEWORK_NAME</string>
        <key>CFBundleExecutable</key>
        <string>$FRAMEWORK_NAME</string>
        <key>CFBundleIconFile</key>
        <string>Icon.png</string>
        <key>CFBundleIdentifier</key>
        <string>com.example.$FRAMEWORK_NAME</string>
        <key>CFBundleInfoDictionaryVersion</key>
        <string>6.0</string>
        <key>CFBundleName</key>
        <string>$FRAMEWORK_NAME</string>
        <key>CFBundlePackageType</key>
        <string>APPL</string>
        <key>CFBundleSignature</key>
        <string>????</string>
        <key>CFBundleShortVersionString</key>
        <string>1.0</string>
        <key>CFBundleVersion</key>
        <string>1.0.0</string>
        <key>UIStatusBarStyle</key>
        <string>UIStatusBarStyleBlackOpaque</string>
        <key>LSRequiresIPhoneOS</key>
        <true/>
</dict>
</plist>
HERE1

    fi

    # Add device specifications to Info.plist (normally done by Xcode).
    # Without these, Instruments reports the app as AWOL.
    #
    iphonesimulator="iphonesimulator"
    iPhoneSimulator="iPhoneSimulator"
    if [ "$i386" != "y" ]; then
	iphonesimulator="iphoneos"
	iPhoneSimulator="iPhoneOS"
    fi
    FAMILY2=1 # phone
    if [ "$FAMILY" == "y" ]; then
    	FAMILY2=2 # pad
    fi
    python -c '
import plistlib
import sys
p = plistlib.readPlist(sys.argv[1])
p["CFBundleSupportedPlatforms"] = ["'+$iPhoneSimulator+'"]
p["DTPlatformName"] = "'+$iphonesimulator+'"
p["DTSDKName"] = "'+$iphonesimulator+'6.0"
p["UIDeviceFamily"] = ['$FAMILY2']
plistlib.writePlist(p, sys.argv[1])
' "$FRAMEWORK_DIR/Resources/Info.plist"

} 

name=''
arm6=n
arm7=n
i386=n
arm64=n
debug=n
FAMILY=n
dir=../lib

name='''+v[0]+'''
arm6='''+v[1]+'''
arm7='''+v[2]+'''
i386='''+v[3]+'''
arm64='''+v[4]+'''
debug='''+v[5]+'''
FAMILY='''+v[6]+'''

    # Define these to suit your nefarious purposes  
                     FRAMEWORK_NAME=$name  
                  FRAMEWORK_VERSION=A  
          FRAMEWORK_CURRENT_VERSION=1  
    FRAMEWORK_COMPATIBILITY_VERSION=1  
                         BUILD_TYPE='''+txt1+'''

    if [ "$debug" != "y" ]; then
	BUILD_TYPE=Release
    fi

    # Check that this is what your static libraries  
    # are called 
    FRAMEWORK_INPUT_ARM_FILES=  
    FRAMEWORK_INPUT_ARM7_FILES=
    FRAMEWORK_INPUT_I386_FILES=
    FRAMEWORK_INPUT_ARM64_FILES=

    if [ "$arm6" != "n" ]; then
	FRAMEWORK_INPUT_ARM_FILES="-arch armv6 $dir/lib$FRAMEWORK_NAME-armv6-$BUILD_TYPE.a"
    fi
    if [ "$arm7" != "n" ]; then
	FRAMEWORK_INPUT_ARM7_FILES="-arch armv7 $dir/lib$FRAMEWORK_NAME-armv7-$BUILD_TYPE.a"
    fi
    if [ "$i386" != "n" ]; then
	FRAMEWORK_INPUT_I386_FILES="-arch i386 $dir/lib$FRAMEWORK_NAME-i386-$BUILD_TYPE.a"
    fi
    if [ "$arm64" != "n" ]; then
	FRAMEWORK_INPUT_ARM64_FILES="-arch arm64 $dir/lib$FRAMEWORK_NAME-arm64-$BUILD_TYPE.a"
    fi
 
    # Where we'll put the build framework.  
    # The script presumes we're in the project root  
    # directory. Xcode builds in "build" by default  
    FRAMEWORK_BUILD_PATH="$dir"  
 
    # Clean any existing framework that might be there  
    # already  
    echo "Framework: Cleaning framework..."  
    [ -d "$FRAMEWORK_BUILD_PATH" ] && rm -rf "$FRAMEWORK_BUILD_PATH/$FRAMEWORK_NAME.framework"  
 
    # This is the full name of the framework we'll  
    # build  
    FRAMEWORK_DIR=$FRAMEWORK_BUILD_PATH/$FRAMEWORK_NAME.framework 
 
    # Build the canonical Framework bundle directory  
    # structure  
    echo "Framework: Setting up directories..."  
    mkdir -p $FRAMEWORK_DIR  
    mkdir -p $FRAMEWORK_DIR/Versions  
    mkdir -p $FRAMEWORK_DIR/Versions/$FRAMEWORK_VERSION
    mkdir -p $FRAMEWORK_DIR/Versions/$FRAMEWORK_VERSION/Resources
    mkdir -p $FRAMEWORK_DIR/Versions/$FRAMEWORK_VERSION/Headers
  
    echo "Framework: Creating symlinks..."  
    ln -s $FRAMEWORK_VERSION $FRAMEWORK_DIR/Versions/Current
    ln -s Versions/Current/Headers $FRAMEWORK_DIR/Headers
    ln -s Versions/Current/Resources $FRAMEWORK_DIR/Resources
    ln -s Versions/Current/$FRAMEWORK_NAME $FRAMEWORK_DIR/$FRAMEWORK_NAME

    # The trick for creating a fully usable library is  
    # to use lipo to glue the different library  
    # versions together into one file. When an  
    # application is linked to this library, the  
    # linker will extract the appropriate platform  
    # version and use that.  
    # The library file is given the same name as the  
    # framework with no .a extension.  
    echo "Framework: Creating library..."
    #lipo -create libdevice.a libsimulator.a -output libcombined.a
    # lipo -output $dir/libOmnitureAppMeasurement-fat.a -create -arch armv6 libOmnitureAppMeasurement-iPhoneDevice.a -arch l386 libOmnitureAppMeasurement-iPhoneSimulator_4_0_GM.a
	
    echo "lipo -output libcombined.a -create $FRAMEWORK_INPUT_ARM_FILES $FRAMEWORK_INPUT_ARM7_FILES $FRAMEWORK_INPUT_I386_FILES $FRAMEWORK_INPUT_ARM64_FILES"
    if [[ $OSTYPE == darwin* ]]; then
	lipo -output libcombined.a -create $FRAMEWORK_INPUT_ARM_FILES $FRAMEWORK_INPUT_ARM7_FILES $FRAMEWORK_INPUT_I386_FILES $FRAMEWORK_INPUT_ARM64_FILES
	if [ -f ./libcombined.a ]; then
		mv ./libcombined.a $FRAMEWORK_DIR/Versions/Current/$FRAMEWORK_NAME
	fi
    fi

    # Now copy the final assets over: your library  
    # header files and the plist file  
    echo "Framework: Copying assets into current version..."  
    #cp Include/$FRAMEWORK_NAME/* $FRAMEWORK_DIR/Headers/  
    #cp Resources/Framework.plist $FRAMEWORK_DIR/Resources/Info.plist
    if [ 0 ] ; then
    tee $FRAMEWORK_DIR/Resources/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleExecutable</key>
	<string>$FRAMEWORK_NAME</string>
	<key>CFBundleIdentifier</key>
	<string>dev.mxcl.x</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>$FRAMEWORK_NAME</string>
	<key>CFBundlePackageType</key>
	<string>FMWK</string>
	<key>CFBundleShortVersionString</key>
	<string>1.0.7</string>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>CFBundleVersion</key>
	<string>$FRAMEWORK_CURRENT_VERSION</string>
	<key>NSPrincipalClass</key>
	<string></string>
</dict>
</plist>
EOF
	else
		createInfoplist FRAMEWORK_DIR FRAMEWORK_NAME;
	fi

		'''

		runscript(createframework)

	elif a.gettype()=='application':
		bootstrap(a, 'i386')
		dev='iPhone-6'
		if ccc.getipad():
			dev='iPad-2'
		appbundle='''#!/bin/bash

source ../make.cnf

CURDIR=`pwd -P`
echo "$CURDIR"

assets()
{

   if [ -n "$1" ]
   then
	DIR=$1
   else
	DIR=$CURDIR/../../../..
   fi

	TARGET_DIR=../bin/'''+ccc.getmainphoneproject()+'''.app/Documents/IndieLib_resources

	mkdir -p $TARGET_DIR
 	#cp -fr $DIR/IndieLib_resources $BUILDPATH/assets/.

	arr=("IndieLib-Animation-Collision" "IndieLib-Blend" "IndieLib-Fading" "IndieLib-FloatingText" "IndieLib-ParallaxScroll" "IndieLib-TankEnemies" "IndieLib-TankEnemiesParallax" "IndieLib-TankFollowsMouseFires" "IndieLib-Transitions" "RadeonTerrainDemo" "NatureScene" "testspine2" "ParticleVS")

	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		echo "cp -fr $DIR/IndieLib_resources/'''+ccc.getmainphoneproject()+''' $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/'''+ccc.getmainphoneproject()+''' $TARGET_DIR/.
	fi

	arr=("a_08_Collisions" "a_16_IND_Timer" "a_02_IND_Surface" "b_01_IND_Surface_Grids" "a_03_IND_Image" "b_02_Blitting_2d_Directly" "a_04_IND_Animation" "a_11_Animated_Tile_Scrolling" "a_05_IND_Font" "a_13_2d_Camera" "b_04_Several_ViewPorts" "a_06_Primitives" "c_01_Alien_BenchMark" "a_07_IND_Input" "a_15_Parallax_Scrolling" "Texture" "Blending")

	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		echo "cp -fr $DIR/IndieLib_resources/'''+ccc.getmainphoneproject()+''' $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/'''+ccc.getmainphoneproject()+''' $TARGET_DIR/.
	fi

	arr=("3DAnimation" "b_03_Blitting_3d_Directly" "a_09_IND_3dMesh")
	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		echo "cp -fr $DIR/IndieLib_resources/skeleton $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/skeleton $TARGET_DIR/.
	fi

	arr=("a_10_IND_Light" "a_14_3d_Camera")
	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		echo "cp -fr "$DIR/IndieLib_resources/trex dx" $TARGET_DIR/."
		cp -fr "$DIR/IndieLib_resources/trex dx" $TARGET_DIR/.
	fi
	arr=("Test" "SimpleTexture")
	if [[ " ${arr[@]} " =~ " '''+ccc.getmainphoneproject()+''' " ]]; then
		echo "cp -f $DIR/IndieLib_resources/test.bmp $TARGET_DIR/."
		cp -f $DIR/IndieLib_resources/test.bmp $TARGET_DIR/.
	fi
	if [ "'''+ccc.getmainphoneproject()+'''" == "2DAnimation" ]; then
		echo "cp -fr $DIR/IndieLib_resources/2DAnimation $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/2DAnimation $TARGET_DIR/.
		echo "cp -fr $DIR/IndieLib_resources/IndieLib-Animation-Collision $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/IndieLib-Animation-Collision $TARGET_DIR/.
	fi
}

cp ./iOS/Info.plist '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/'''+a.getname()+'''.app
sed -i.bu "s|>test<|>'''+a.getname()+'''<|g" '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/'''+a.getname()+'''.app/Info.plist
sed -i.bu "s|>iOSAppFromScratch<|>'''+a.getname()+'''<|g" '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/'''+a.getname()+'''.app/Info.plist
sed -i.bu "s|>com.seancrazy.iOSApp<|>'''+ccc.getpackage()+'''<|g" '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/'''+a.getname()+'''.app/Info.plist
# python -c '\\nimport plistlib\\nimport sys\\np = plistlib.readPlist(sys.argv[1])\\np["CFBundleSupportedPlatforms"] = ["iPhoneSimulator"]\\np["DTPlatformName"] = "iphonesimulator"\\np["DTSDKName"] = "iphonesimulator6.0"\\np["UIDeviceFamily"] = ['$FAMILY']\\nplistlib.writePlist(p, sys.argv[1])' "'''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/'''+a.getname()+'''.app/Info.plist"
touch '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/'''+a.getname()+'''.app/PkgInfo
assets
ios-sim launch '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/'''+a.getname()+'''.app --devicetypeid com.apple.CoreSimulator.SimDeviceType.'''+dev+'''
		'''
		runscript(appbundle)

if __name__ == "__main__":

	test2()

	#print '#:1'

	SDL2version, assimp3version, NDK= cnf()

	#print '#:11'
	ccc=Configuration()

	#print '#:12'

	if(getOS()=='Windows'):
		ccc.setbuildtool ('NMAKE')
	else:
		ccc.setbuildtool ('LNX')

	#print '#:13'

	buildsln='AllLibs'
	buildprj='a_02_IND_Surface'
	buildtool='LNX'
	package='com.yourcompany.yourapp'

	#print '#:2'

	try:
		options, remainder = getopt.getopt(sys.argv[1:], '', [	'buildsln=',
	                                                         	'buildprj=',
									'buildtool='
									])
		for opt, arg in options:

			if opt=='--buildsln':
				buildsln = arg
				buildprj = 'None'

			elif opt=='--buildprj':
				solution = 'AllLibs'
				buildprj = arg

			if opt=='--buildtool':
				buildtool=arg
				if os.name=='posix':
					if arg in ['OSX','iOS']:
						test_OS='Darwin'
					elif arg in ['NMAKE','VC']:
						test_OS='Windows'
				elif os.name=='nt':
					if arg in ['OSX','iOS']:
						test_OS='Darwin'
					elif arg in ['LNX']:
						test_OS='Ubuntu'

				elif sys.platform=='darwin':
					if arg in ['LNX']:
						test_OS='Ubuntu'
					elif arg in ['NMAKE','VC']:
						test_OS='Windows'
			 
	except getopt.GetoptError as err:
		print str(err)
		ccc.usage(6)
		sys.exit(2)

	#test_OS='Ubuntu'

	#print '#:3c'
	#print 'buildproject:'+buildprj
	#print '#:3d'

	xmlfromstring=True
	g_node = None

	#print '#:4'
	if xmlfromstring:
		txt=load(ccc.getProjectsxml())
		SDL2version, assimp3version, NDK=cnf()

		#print 'SDL2version   ='+SDL2version
		#print 'assimp3version='+assimp3version
		#print 'NDK           ='+NDK
		#exit(0)

		txt=txt.replace('{assimp3version}',assimp3version)
		txt=txt.replace('{SDL2version}',SDL2version)
		txt=txt.replace('{NDK}',NDK)

		#if not buildproject=='None':
		#	ccc.setbuildproject(buildproject)

		g_node = ElementTree.fromstring(txt)
		#xxx()
	else:
		with open(ccc.getProjectsxml(), 'rt') as f:
			tree = ElementTree.parse(f)
			g_node=tree.getroot()

	#print '#:5'

	packagesources=getpackagesources(buildsln, g_node)
	ccc.setsolutions(getslns(g_node))
	ccc.setprojects(getprjs(g_node))
	#ccc.settextfromxml(g_node)
	install()

	#print '#:6'
	
	#ccc=Configuration()
	#ccc.settextfromxml(g_node)

	#print '#:7'
	#print 'buildtool:'+buildtool
	#print 'OS:'+getOS()
	#print 'solution:'+buildsln
	#print 'buildproject:'+buildprj
	#def parse(sln, abuildtool, gles2=False):
	configuration, solutions, projects=parse(buildsln, buildtool)
	#print '#:8'

	for i in projects:

		if i.getname()=='SDL2':
			if i.gettype()=='static_library':
				print 'Passing static library: '+i.getname()
			elif i.gettype()=='shared_library':
				print 'Passing dynamic library: '+i.getname()
			continue

		if buildprj=='None':
			if exists(i):
				continue
		else:
			if not buildprj==i.getname():
				if i.gettype()=='static_library':
					print 'Passing static library: '+i.getname()
				elif i.gettype()=='shared_library':
					print 'Passing dynamic library: '+i.getname()
				continue

		if(getOS() in ['Ubuntu', 'Debian', 'Linux']) and (buildtool=='LNX'):
			bootstrap(i)
		elif(getOS()=='Darwin') and (buildtool=='OSX'):
			bootstrap(i)
		#elif((sys.platform=='darwin') and (buildtool=='OSX')):
		#	p=pbxproj(i)
		elif((getOS()=='Darwin') and (buildtool=='iOS')):
			bootstrapiOS(i)
		elif(buildtool=='MW'):
			bootstrap(i)
		elif((getOS()=='Windows') and (buildtool=='NMAKE')):
			bootstrap(i)
		#elif((getOS()=='Windows') and (buildtool=='VC')):
		#	bootstrapVC(i)
		elif(buildtool=='NDK'):
			bootstrap(i)
			#bootstrapNDK(buildsln, buildprj, buildtool, package)
		#elif(buildtool=='CMake'):
		#	bootstrapCMake(i)
		#elif((getOS()=='Windows') and (buildtool=='BAT')):
		#	bootstrapBAT(i)   
		#elif(buildtool=='PRO'):
		#	bootstrapPRO(i)

	if not test_OS=='':
		exit(0)

	if(buildtool=='NDK'):
		NDKInstall(package)

	exit(0)
	if(buildtool in ['LNX','OSX','NMAKE']):
		rrr(projects)
		cnf2(ccc)

	#os.system("start C:/thepathyouwant/file")

	#print '#:9'

	x='''
[armeabi] SharedLibrary  : libSDL2.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-g++ -Wl,-soname,libSDL2.so -shared --sysroot=/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm $(OBJS) -lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now  -L/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm/usr/lib -ldl -lGLESv1_CM -lGLESv2 -llog -landroid -lc -lm -o ./obj/local/armeabi/libSDL2.so

[armeabi-v7a] SharedLibrary  : libSDL2.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-g++ -Wl,-soname,libSDL2.so -shared --sysroot=/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm $(OBJS) -lgcc -no-canonical-prefixes -march=armv7-a -Wl,--fix-cortex-a8  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now  -L/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm/usr/lib -ldl -lGLESv1_CM -lGLESv2 -llog -landroid -lc -lm -o ./obj/local/armeabi-v7a/libSDL2.so

[x86] SharedLibrary  : libSDL2.so
/home/tronty/android-ndk-r9d/toolchains/x86-4.6/prebuilt/linux-x86/bin/i686-linux-android-g++ -Wl,-soname,libSDL2.so -shared --sysroot=/home/tronty/android-ndk-r9d/platforms/android-19/arch-x86 $(OBJS) -lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now  -L/home/tronty/android-ndk-r9d/platforms/android-19/arch-x86/usr/lib -ldl -lGLESv1_CM -lGLESv2 -llog -landroid -lc -lm -o ./obj/local/x86/libSDL2.so

[armeabi] Install        : libSDL2.so => libs/armeabi/libSDL2.so
install -p ./obj/local/armeabi/libSDL2.so ./libs/armeabi/libSDL2.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-strip --strip-unneeded  ./libs/armeabi/libSDL2.so
[armeabi] SharedLibrary  : libmain.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-g++ -Wl,-soname,libmain.so -shared --sysroot=/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm ./obj/local/armeabi/objs/main/__/SDL/src/main/android/SDL_android_main.o ./obj/local/armeabi/objs/main/testgles2.o -lgcc ./obj/local/armeabi/libSDL2.so -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now  -L/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm/usr/lib -lGLESv1_CM -lGLESv2 -llog -lc -lm -o ./obj/local/armeabi/libmain.so
[armeabi-v7a] Install        : libSDL2.so => libs/armeabi-v7a/libSDL2.so
install -p ./obj/local/armeabi-v7a/libSDL2.so ./libs/armeabi-v7a/libSDL2.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-strip --strip-unneeded  ./libs/armeabi-v7a/libSDL2.so
[armeabi-v7a] SharedLibrary  : libmain.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-g++ -Wl,-soname,libmain.so -shared --sysroot=/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm ./obj/local/armeabi-v7a/objs/main/__/SDL/src/main/android/SDL_android_main.o ./obj/local/armeabi-v7a/objs/main/testgles2.o -lgcc ./obj/local/armeabi-v7a/libSDL2.so -no-canonical-prefixes -march=armv7-a -Wl,--fix-cortex-a8  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now  -L/home/tronty/android-ndk-r9d/platforms/android-19/arch-arm/usr/lib -lGLESv1_CM -lGLESv2 -llog -lc -lm -o ./obj/local/armeabi-v7a/libmain.so
[armeabi] Install        : libmain.so => libs/armeabi/libmain.so
install -p ./obj/local/armeabi/libmain.so ./libs/armeabi/libmain.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-strip --strip-unneeded  ./libs/armeabi/libmain.so
[armeabi-v7a] Install        : libmain.so => libs/armeabi-v7a/libmain.so
install -p ./obj/local/armeabi-v7a/libmain.so ./libs/armeabi-v7a/libmain.so
/home/tronty/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86/bin/arm-linux-androideabi-strip --strip-unneeded  ./libs/armeabi-v7a/libmain.so
[x86] Install        : libSDL2.so => libs/x86/libSDL2.so
install -p ./obj/local/x86/libSDL2.so ./libs/x86/libSDL2.so
[x86] SharedLibrary  : libmain.so
/home/tronty/android-ndk-r9d/toolchains/x86-4.6/prebuilt/linux-x86/bin/i686-linux-android-g++ -Wl,-soname,libmain.so -shared --sysroot=/home/tronty/android-ndk-r9d/platforms/android-19/arch-x86 ./obj/local/x86/objs/main/__/SDL/src/main/android/SDL_android_main.o ./obj/local/x86/objs/main/testgles2.o -lgcc ./obj/local/x86/libSDL2.so -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now  -L/home/tronty/android-ndk-r9d/platforms/android-19/arch-x86/usr/lib -lGLESv1_CM -lGLESv2 -llog -lc -lm -o ./obj/local/x86/libmain.so



tronty@ubuntu16:~/IndieLibX$ ./make.sh buildsln Test NDK
#4:/usr/bin/python build.py --buildsln Test --buildtool NDK
/home/tronty/android-ndk-r9d/toolchains/x86-4.6/prebuilt/linux-x86/bin/../lib/gcc/i686-linux-android/4.6/../../../../i686-linux-android/bin/ld: fatal error: lib/libSimpleTexture.so: open: No such file or directory
collect2: ld returned 1 exit status




tronty@ubuntu16:~/IndieLibX$ ./make.sh buildsln Test NDK
#4:/usr/bin/python build.py --buildsln Test --buildtool NDK
build solution=Test
Passing static library: hlsl2glsl
Passing static library: Image3
Passing static library: STX
Passing static library: Framework3

../../android-ndk-r9d/toolchains/x86-4.6/prebuilt/linux-x86/bin/i686-linux-android-g++ --sysroot=../../android-ndk-r9d/platforms/android-19/arch-x86 -fexceptions -fnon-call-exceptions -D_DEBUG -DDEBUG -w -fmax-errors=1 -fPIC -fexceptions -fpermissive -DUSE_GLES_SHADERS -DANDROID -fno-strict-aliasing -finline-functions -ffunction-sections -funwind-tables -fmessage-length=0 -fno-inline-functions-called-once -fgcse-after-reload -frerun-cse-after-loop -frename-registers -no-canonical-prefixes -fomit-frame-pointer -funswitch-loops -finline-limit=300 -O2 -g -Wa,--noexecstack -Wformat -Werror=format-security -fno-exceptions -fno-rtti -I../../android-ndk-r9d/sources/cxx-stl/stlport/stlport -I../../android-ndk-r9d/sources/cxx-stl/gabi++/include -I../Libraries/MVC -I../Libraries/SDL2-2.0.4 -I../Libraries/MVC/NDK/jni -I../Libraries/BoostWorkaround -I../DesktopProjects/NDK_gl_code2 -include "android_fopen.h" -I../Libraries/BoostWorkaround -I../Libraries/Image3 -I../Libraries/STX/include -I../Libraries/Framework3/include -I../Libraries/IndieLib/IndieLib/Common -I../Libraries/hlsl2glslfork/include -I../Libraries/otl/include -I../Libraries/assimp-3.2/include -I../Libraries/assimp-3.2/code -I../Libraries/MeshRenderer2 -I../Samples/SimpleTexture -I../Libraries/STX/include -I../Libraries/Framework3/include -frtti -fexceptions -o ../Samples/SimpleTexture/SimpleTexture.o -c ../Samples/SimpleTexture/SimpleTexture.cpp

../../android-ndk-r9d/toolchains/x86-4.6/prebuilt/linux-x86/bin/i686-linux-android-g++ --sysroot=../../android-ndk-r9d/platforms/android-19/arch-x86 -Wl,-soname,libSimpleTexture.so -shared ../Samples/SimpleTexture/SimpleTexture.o -lgcc -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -L../lib -L../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK -lFramework3 -lSTX -lImage3 -lhlsl2glsl -ldl -lOpenSLES -lGLESv1_CM -lGLESv2 -lGLESv3 -lEGL -llog -landroid -lstdc++ -lc -lm -lm -o ../lib/libSimpleTexture.so
/bin/rm -f ..//Samples/SimpleTexture/SimpleTexture.cpp~
/bin/rm -f ..//Samples/SimpleTexture/SimpleTexture.o
tronty@ubuntu16:~/IndieLibX$ 


	'''

