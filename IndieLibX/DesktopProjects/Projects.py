#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
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

ccc=0
g_node=0
drive,tail=os.path.splitdrive(os.path.expanduser("~"))

SDL2version=''
assimp3version=''
NDK=''

def load(file):
    xml_file = os.path.abspath(file)
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

def getOS(test=False):
	if test:
		return 'Darwin'
		return 'Windows'
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

def make_executable(path):
	mode = os.stat(path).st_mode
	mode |= (mode & 0444) >> 2    # copy R bits to X
	os.chmod(path, mode)

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
				f.write(' &&\n'+Linuxfn)
			else:
	        	    	f.write(Linuxfn) 
	        	ff.write('cd '+cexedir+'\ncall '+i.getname()+'.exe\ncd '+cret+'\n') 
	    		cc=cc+1
	f.close()
	ff.close()
	x='''
	if getOS()=='Windows':
		os.system('call ../run.bat')
	elif getOS() in ['Ubuntu', 'Debian', 'Linux']:
		make_executable('../run.sh')
		os.system('../run.sh')
	'''

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
        self.buildproject='None'

	self.maxerrors=1
	self.debug=False
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
		self.buildtool = 'NMAKE'
	else:
		self.guid0='EA72E90C-C91B-11DF-8FC4-539D12B3A9'
		self.buildtool = 'LNX'
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

	self.INDIELIBROOT='../../..'
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

	self.buildtools='NDK|LNX|MW|PRO|CMake'
	if getOS()=='Windows':
		self.buildtools='NDK|NMAKE|VC|MW|PRO|CMake'
		self.HAS_CGGL=True
		self.buildtool='NMAKE'	
	elif getOS()=='Darwin':
		self.buildtools='NDK|iOS|OSX|MW|PRO|CMake'
		self.LibDir='Mac64'
		self.buildtool='OSX'
		self.HAS_CGGL=False
	elif getOS()=='Ubuntu':
		self.buildtools='NDK|LNX|MW|PRO|CMake'
		self.LibDir='Linux%d' % self.bitness
		self.buildtool='LNX'
		self.HAS_CGGL=True
	elif getOS()=='Debian':
		self.buildtools='NDK|LNX|MW|PRO|CMake'
		self.LibDir='Linux%d' % self.bitness
		self.buildtool='LNX'
		self.HAS_CGGL=False
	else:#if getOS()=='Linux':
		self.buildtools='NDK|LNX|MW|PRO|CMake'
		self.LibDir='Linux%d' % self.bitness
		self.buildtool='LNX'
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

	try:
		options, remainder = getopt.getopt(sys.argv[1:], '', [	'solution=',
	                                                         	'mainphoneproject=',
									'package=',
	                                                         	'OpenAL=',
	                                                         	'SDL2=',
							        	'iOS=',
									'apps=',
									'buildtool=',
									'TargetDirectory=',
									'NDKPlatform=',
									'iPhoneSimulator=',
									'GLES2=',
	                                                         	'WindowsPhone=',
									'MVC=',
									'buildproject='
									])
		for opt, arg in options:
		
			if opt=='--buildtool':
	        		self.buildtool = arg
				if self.buildtool=='NDK':
					self.INDIELIBROOT='..'
				elif self.buildtool=='NMAKE':
					self.INDIELIBROOT='.'
				elif self.buildtool=='BAT':
					self.INDIELIBROOT='..'
			elif opt=='--solution':
				if not arg == 'None':
	        			self.solution = arg.strip()
				else:
					if getOS()=='Windows':
						txt = load('../cnf.bat')
						p = re.compile(r"set solution=(\w+)",re.S)
						v=p.findall(txt)
						if len(v)==1:
  							self.solution = v[0].strip()
					else:
						txt = load('../make.cnf')
						p = re.compile(r"solution=(\w+)",re.S)
						v=p.findall(txt)
						if len(v)==1:
  							self.solution = v[0].strip()

			elif opt=='--buildproject':
				self.buildproject = arg

			elif opt=='--mainphoneproject':
	        		self.mainphoneproject = arg
			elif opt=='--package':
	        		self.package = arg
			elif opt=='--OpenAL':
	        		self.openal = arg
			elif opt=='--SDL2':
	        		self.sdl2 = arg
			elif opt=='--apps':
	        		self.apps = arg
			elif opt=='--WindowsPhone':
	        		self.windowsphone = arg
			#elif opt=='--GLES2':
	        	#	self.gles2 = arg
			elif opt=='--iPhoneSimulator':
	        		self.apps = arg
			elif opt=='--MVC':
	        		self.mvc = arg
			elif opt=='--TargetDirectory':
	        		self.targetdir = arg
			elif opt=='--NDKPlatform':
	        		self.NDKplatform = arg
			elif opt=='--MVC':
	        		mvc = arg
	except getopt.GetoptError as err:
		#print str(err)
		#self.usage()
		sys.exit(2)
	if self.openal=="True":
		self.openal=True
	else:
		self.openal=False
	
	if self.apps=="True":
		self.apps=True
	else:
		self.apps=False
	
	if self.sdl2=="True":
		self.sdl2=True
	else:
		self.sdl2=False
	if self.windowsphone=="True":
		self.windowsphone=True
	else:
		self.windowsphone=False
	
	#if self.gles2=="True":
	#	self.gles2=True
	#else:
	#	self.gles2=False

	if self.iPhoneSimulator=="True":
		self.iPhoneSimulator=True
	else:
		self.iPhoneSimulator=False
	
	if self.mvc=="True":
		self.mvc=True
	else:
		self.mvc=False

    def usage(self):
	print "python Projects.py --buildtool <"+ccc.getbuildtools()+"> --solution <"+ccc.getsolutions()+"> --mainphoneproject <string> --package <string> --WindowsPhone <True|False> --iOS <True|False> --OpenAL <True|False> --SDL2 <True|False> --GLES2 <True|False> --apps <True|False> --iPhoneSimulator <True|False> --MVC <True|False> --TargetDirectory <string> --NDKPlatform <string>"

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

    def setbuildproject(self, abuildproject):
	self.buildproject=abuildproject

    def getbuildproject(self):
	return self.buildproject

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

    def getbuildtool(self):
        return self.buildtool
    def getOS(self):
        return getOS()
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
	return self.buildtools
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
	self.buildtool=a
    def getbuildtool(self):
	return self.buildtool
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
	self.buildtools=[]
	self.buildtool = 'LNX'
	#self.gles2=False
	self.characterset=False

    def getsrcdir(self):
        return self.srcdir

    def getbuildtools(self):
        return self.buildtools

    def getbuildtool(self):
        return self.buildtool

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
	self.buildtool=abuildtool.strip()
        self.text=''
	self.libs=list(reversed(libs))
	for child_of_node in node:
		if child_of_node.tag=='name':
			self.name=child_of_node.text.strip()
		elif child_of_node.tag=='platform':
			if(child_of_node.text.strip()=='PHONE'):
				self.buildtools.append('NDK')
				self.buildtools.append('iOS')
				self.buildtools.append('WP')
				self.buildtool='PHONE'
			else:
				self.buildtools=txt2array(child_of_node.text)
				self.buildtool=child_of_node.text.strip()
			if(not len(self.buildtools)):
				self.buildtools.append(abuildtool)
				self.buildtool=abuildtool.strip()
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
				if (not ccc.getbuildtool()=='iOS'):
					# and (not ccc.getbuildtool()=='OSX'):
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

			if (not (ccc.getbuildtool()==abuildtool.strip())):
				continue
			p=Project()
			p.settextfromxml(node,libs, abuildtool)
			if ccc.getbuildtool() in ['LNX','OSX','MSC']:
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

#ProjectsPMiOS.py:
class ProjectPMiOS:
    	def __init__(self):
		return

	def project2CPiOS(self, f, a):
		flags=''
		files=''
		incs=''
		if len(a.getflags()):
			flags=',\n\t\t\t\"'+string.join(a.getflags(), '\"\n\t\t\t\t, \"')+'\"'
		if len(a.getsrcs()):
			files='\t\"../../'+string.join(a.getsrcs(), '\"\n\t\t\t\t, \"../../')+'\"'
		if len(a.getincs()):
			incs='\t\"../../'+string.join(a.getincs(), '\"\n\t\t\t\t, \"../../')+'\"'

		files=files.replace('//','/')
		incs=incs.replace('//','/')

		#'''+ccc.getg()+''' '''+ccc.getO()+'''

		txt=''
		if a.gettype()=='application':
			txt='''
\tproject "'''+a.getname()+'''"
\t\tkind "ConsoleApp"

\t\tconfiguration "Debug"
\t\t\tdefines { "DEBUG",
\t\t\t\t"_DEBUG"'''+flags+''' }
\t\t\tflags { "Symbols" }
\t\t\ttargetdir "build/Debug"

\t\tconfiguration "Release"
\t\t\tdefines { "NDEBUG"'''+flags+''' }
\t\t\tflags { "Optimize" }
\t\t\ttargetdir "build/Release"

\t\tfiles { '''+files+''' }
\t\tfiles { "app/en.lproj" }

\t\tincludedirs { '''+incs+''' }
\t\t//links { "lib" }

\t\tlinkoptions
\t\t\t{ "-framework CoreMotion"
\t\t\t, "-framework GameController"
\t\t\t, "-framework OpenGLES"
\t\t\t, "-framework UIKit"
\t\t\t, "-framework CoreGraphics"
\t\t\t, "-framework OpenAL"
\t\t\t, "-framework CoreAudio"
\t\t\t, "-framework AudioToolbox"
\t\t\t, "-framework QuartzCore"
\t\t\t, "-framework Foundation"
\t\t\t, "-lstdc++"
\t\t\t, "-lc++"
\t\t\t, "-lm"
\t\t\t, "-lc"
\t\t\t, "-ObjC"
\t\t\t}
'''
	    	elif a.gettype()=='static_library':
			txt='''
\tproject "'''+a.getname()+'''"
\t\tkind "StaticLib"

\t\tconfiguration "Debug"
\t\t\tdefines { "DEBUG",
\t\t\t\t"_DEBUG"'''+flags+''' }
\t\t\tflags { "Symbols" }
\t\t\ttargetdir "build/Debug"

\t\tconfiguration "Release"
\t\t\tdefines { "NDEBUG"'''+flags+''' }
\t\t\tflags { "Optimize" }
\t\t\ttargetdir "build/Release"

\t\tfiles { '''+files+''' }
\t\tincludedirs { '''+incs+''' }
'''
	    	elif a.gettype()=='shared_library':
			txt='''
\tproject "'''+a.getname()+'''"
\t\tkind "DynamicLib"

\t\tconfiguration "Debug"
\t\t\tdefines { "DEBUG",
\t\t\t\t"_DEBUG"'''+flags+''' }
\t\t\tflags { "Symbols" }
\t\t\ttargetdir "build/Debug"

\t\tconfiguration "Release"
\t\t\tdefines { "NDEBUG"'''+flags+''' }
\t\t\tflags { "Optimize" }
\t\t\ttargetdir "build/Release"

\t\tfiles { '''+files+''' }
\t\tincludedirs { '''+incs+''' }
'''

	    	f.write(txt)

	def run(self):
	   	if not os.path.exists('./projectsX'):
			os.makedirs('./projectsX')
		configuration, solutions, projects=parse(ccc.getsolution(), 'iOS')
		f=open('./projectsX/premake4.lua','w')
		txt='''
solution "'''+ccc.getsolution()+'''"
\tlanguage "C++"
\tconfigurations { "Debug", "Release" }
\tflags { "ExtraWarnings", "StaticRuntime" }

\t-- if you used Homebrew to install boost to these locations...
\tif _ACTION == "xcode3" then
\t\tincludedirs { "/usr/local/include" }
\t\tlibdirs { "/usr/local/lib" }
\tend

		'''
	    	f.write(txt)
		for i in projects:
			self.project2CPiOS(f, i)
		f.close

		txt='''# Usage:
# In premake4.lua use StaticLib for libraries; ConsoleApp for applications.
# Then run `rake xcode && rake xcodebuild`

ENV_DEFAULTS = {
    'CONFIGURATION' => "Debug",
    'SDK' => "iphonesimulator",
}

SDK_TO_ARCH = {
    "iphonesimulator" => "i386",
    "iphoneos" => "armv7",
}

# These projects will be converted to iOS applications. These 
# should be the ones you specified ConsoleApp in premake4.lua
APP_PROJECTS = %w(app1 app2)
APP_XCODEPROJS = APP_PROJECTS.map { |x| "#{x}.xcodeproj" }

ENV_DEFAULTS.each {|k, v| ENV[k] = v if !ENV[k] }

def xcode_to_ios pbxproj
    lines = IO.readlines(pbxproj).select do |l|
        not l =~ /PREBINDING =/ and not l =~ /GCC_ENABLE_FIX_AND_CONTINUE =/
    end.map do |l|
        l.
            gsub("$(NATIVE_ARCH_ACTUAL)", "$(ARCHS_STANDARD_32_BIT)").
            gsub("objectVersion = 45", "objectVersion = 46")
    end.map do |l|
        l =~ /PRODUCT_NAME =/ ?  [l,
            %(CODE_SIGN_IDENTITY = "iPhone Developer";),
            %(PROVISIONING_PROFILE = "";),
            %(SDKROOT = iphoneos;)] : l
    end.flatten
    open(pbxproj, 'w') {|f| f.puts lines }
end

def xcode_to_app pbxproj
    lines = IO.readlines(pbxproj).map do |l|
        l.gsub("com.apple.product-type.tool", "com.apple.product-type.application")
    end
    open(pbxproj, 'w') {|f| f.puts lines }
end

def run cmd
    puts cmd
    system cmd
end

desc "Create Xcode build files."
task :xcode do
    run "premake4 clean"
    run "premake4 xcode3"
    print "#{xc}/project.pbxproj"
    Dir["*.xcodeproj"].each { |xc| xcode_to_ios "#{xc}/project.pbxproj" }
    APP_XCODEPROJS.each { |xc| xcode_to_app "#{xc}/project.pbxproj" }
end

desc "Run xcodebuild command. See ENV_DEFAULTS for a list."
task :xcodebuild do
    APP_XCODEPROJS.each do |xc|
        run(%(xcodebuild -project #{xc} -configuration #{ENV['CONFIGURATION']} ) +
            %(-sdk #{ENV['SDK']} -arch #{SDK_TO_ARCH[ENV['SDK']]}))
    end
end
		'''

	    	f=open('./projectsX/Rakefile','w')
	    	f.write(txt)
	    	f.close()

#ProjectsCPiOS.py:
class ProjectCPiOS:
    	def __init__(self):
		return

	def project2CPiOS(self, a):

		v=[]
		for i in a.getlibs():
			if i in ['SDL2']:
				continue
	            	v.append(i)
	        for i in a.getsharedlibs():
			if i in ['SDL2']:
				continue
	            	v.append(i)
	        frameworks=string.join(v, '\', \'')

		txt='''Pod::Spec.new do |spec|
  spec.name         = \''''+a.getname()+'''\'
  spec.platform     = :ios, \'7.0\'
  spec.source_files = \'../../'''+a.getsrcdir()+'''/*.{m,mm,c,cxx,cpp}\'
  spec.frameworks   = \''''+frameworks+'''\'
end
		'''

	    	f=open('./projectsX/'+a.getname()+'.podspec','w')
	    	f.write(txt)
	    	f.close()

	def run(self):
	   	if not os.path.exists('./projectsX'):
			os.makedirs('./projectsX')

		configuration, solutions, projects=parse(ccc.getsolution(), 'iOS')
		for i in projects:
			self.project2CPiOS(i)

		f=open('./projectsX/Podfile','w')
		if False:
	    		f.write('target :'+ccc.getmainphoneproject()+' do\n')
			for i in projects:
				if i.getname() in ['SDL2']:
					continue
				f.write('\tpod \''+i.getname()+'\', :path => \'.\'\n')
			f.write('end\n')
		else:
	    		f.write('platform :ios, \'7.0\'\n')
			for i in projects:
				if i.getname() in ['SDL2']:
					continue
				f.write('\tpod \''+i.getname()+'\', :path => \'.\'\n')
		
	    	f.close()

#ProjectsLNX.py:
class ProjectLNX:
    	def __init__(self):
		return

	def project2gcc(self, a):
	
	    asrcs=' '+ccc.getINDIELIBROOT()+'/'+string.join(a.getsrcs(),' \\\n\t'+ccc.getINDIELIBROOT()+'/')+'\n'
	
	    v=[]
	    v.append('-fmax-errors='''+ccc.getmaxerrors())

	    if getOS()=='Darwin':
	    	v.append('-fvisibility=hidden')
		v.append('-fvisibility-inlines-hidden')

	    if not ccc.getDebug():
	    	v.append('-DNDEBUG')
	    else:
	    	v.append('-D_DEBUG')
	    	v.append('-DDEBUG')

	    if ccc.getgles2():
		v.append('-DUSE_GLES_SHADERS')
	    else:
		v.append('-DUSE_HLSL_SHADERS')

	    for i in a.getflags():
		v.append('-D'+i)

	    v.append('-I'+ccc.getINDIELIBROOT()+'/Libraries/BoostWorkaround')
	    for i in a.getincs():
	    	v.append('-I'+ccc.getINDIELIBROOT()+'/'+i)
	    
	    aflagsincs=join(v, ' ')
	
	    v=[]
	    v.append('-L'+ccc.getINDIELIBROOT()+'/lib')
	    v.append('-L/usr/local/lib')
	    for i in a.getlibs():
	    	v.append('-l'+i)
	    for i in a.getsharedlibs():
	        v.append('-l'+i)
	    alibs=join(v, ' ')

	    # $(LDFLAGS)
	    # $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREAD) $(X11)

	    txt0='''NAME = '''+a.getname()+'''
FILEEXT = \"<FILEEXT>\"

ifeq ($(FILEEXT), ".so")
ifeq ("Darwin",$(OS))
TARGET = '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Contents/MacOS/lib$(NAME).so
else
TARGET = '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/lib$(NAME).so
endif
else ifeq ($(FILEEXT), ".a")
TARGET = '''+ccc.getINDIELIBROOT()+'''/lib/lib$(NAME).a
else ifeq ($(FILEEXT), ".app")
TARGET = '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Contents/MacOS/$(NAME)
else ifeq ($(FILEEXT), ".bin")
TARGET = '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).bin
endif

MNAME := $(shell uname -m | tr "A-Z" "a-z")
ifeq ($(MNAME),i686)
BITNESS = "32"
LIBDIR=../../../../IndieLibX_Libraries/IndieLibX_AllLibs_LNX32
else
BITNESS = "64"
ifeq ("Darwin",$(OS))
LIBDIR=../../../../IndieLibX_Libraries/IndieLibX_AllLibs_OSX64
else
LIBDIR=../../../../IndieLibX_Libraries/IndieLibX_AllLibs_LNX64
endif
endif

LDFLAGS += -L$(LIBDIR)

ifeq ("Debian",$(OS))
	export LibDir="Linux$(BITNESS)"
ifeq ("True",$(USE_GLES_SHADERS))
	export OPENGL = -lGLESv2 -lGL -lGLU
else
	export OPENGL = -lGL -lGLU
endif
	export SDL = -lSDL2 -lSDL2_image -lopenal
	export X11 = -lX11
	CFLAGS += -DOS_Debian
else ifeq ("Darwin",$(OS))
	CFLAGS += -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$(MACOSX_VERSION).sdk
	LDFLAGS += -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$(MACOSX_VERSION).sdk
	
	CFLAGS += -I/usr/local/include -I/usr/X11R6/include
	LDFLAGS += -L/usr/local/lib -L/usr/X11R6/lib
	export CPLUSPLUS = -D__cplusplus
	export LibDir="Mac64"
	export SDL = -lSDL2 -lSDL2_image -lX11
	export FRAMEWORKS = -framework OpenGL -framework OpenAL
else
	export CGLIBS = -lCg -lCgGL
	#export CFLAGS += -DHAS_CGGL
ifeq ("True",$(USE_GLES_SHADERS))
	export OPENGL = -lGLESv2 -lGL -lGLU
else
	export OPENGL = -lGL -lGLU
endif
	export SDL = -lSDL2 -lSDL2_image -lopenal
	export X11 = -lX11 
	#-L/usr/lib/i386-linux-gnu -lusb -ludev
	export LibDir="Linux$(BITNESS)"
endif

CXXFLAGS += -std=gnu++0x
CPPFLAGS += -std=gnu++0x

ifneq ("Darwin",$(OS))
CFLAGS += -Wno-enum-compare
CFLAGS += -rdynamic -D_LINUX -DLINUX -D__linux__
endif

ifeq ($(OS), "Darwin")
ifeq ($(MNAME),i686)
ARCH = -arch i386 -mmacosx-version-min=10.6
else
ARCH = -arch x86_64 -mmacosx-version-min=10.6
endif
endif

ifeq ("Darwin",$(OS))
NARROWING = -std=c++11 -Wno-c++11-narrowing
else
NARROWING = 
endif
	
ifeq ($(OS), "Darwin")
export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer
# xcrun clang -arch i386 -arch x86_64 -stdlib=libc++ $(NARROWING) -mmacosx-version-min=10.8
export CC  = xcrun --sdk macosx$(MACOSX_VERSION) cc $(ARCH)
export GCC = xcrun --sdk macosx$(MACOSX_VERSION) gcc $(ARCH)
export GPP = xcrun --sdk macosx$(MACOSX_VERSION) g++ $(ARCH)
export AR  = xcrun --sdk macosx$(MACOSX_VERSION) ar
export LD  = xcrun --sdk macosx$(MACOSX_VERSION) ld
export RANLIB = xcrun --sdk macosx$(MACOSX_VERSION) ranlib
else
export CC  = /usr/bin/cc
export GCC = /usr/bin/gcc
export GPP = /usr/bin/g++
export AR  = /usr/bin/ar
export LD  = /usr/bin/ld
export RANLIB = /usr/bin/ranlib
endif
	
DEL=/bin/rm -fr
	
ifeq ($(OS), "Darwin")
ifneq ($(MNAME),i686)
CFLAGS += -Dx86_64
endif
endif
	
CFLAGS += --trigraphs -Wno-comment -Wno-format -Wno-write-strings 
	
ifeq ($(FILEEXT), ".so")
PIC3 = -fPIC
endif
	
COBJFLAGS = '''+aflagsincs+''' -w
CFLAGS += $(PIC3) -fpermissive $(COBJFLAGS)
	
CXXFLAGS += $(CFLAGS)
CPPFLAGS += $(CFLAGS)
	
SRCS = '''+asrcs+'''
	
%.o: %.c
	$(GCC) '''+ccc.getG()+''' '''+ccc.getO()+''' $(CFLAGS) -o $@ -c $<
	
%.o: %.cc
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' $(NARROWING) -fexceptions -fnon-call-exceptions $(CPLUSPLUS) $(CPPFLAGS) -o $@ -c $<
	
%.o: %.cpp
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' $(NARROWING) -fexceptions -fnon-call-exceptions $(CPLUSPLUS) $(CPPFLAGS) -o $@ -c $<
	
%.o: %.cxx
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' $(NARROWING) -fexceptions -fnon-call-exceptions $(CPLUSPLUS) $(CXXFLAGS) -o $@ -c $<
	
ifeq ($(OS), "Darwin")
%.o: %.m
	$(GCC) '''+ccc.getG()+''' '''+ccc.getO()+''' $(COBJFLAGS) -fnested-functions $@ -c $<
else
%.o: %.m
	echo $@ not included in Linux build
endif
	
ifeq ($(OS), "Darwin")
OBJS=$(patsubst %.m,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.cpp %.cxx %.cc %c %.m ,$(SRCS)))))))
FILES=$(patsubst %.m,%.m~,$(patsubst %.cxx,%.cxx~,$(patsubst %.cpp,%.cpp~,$(patsubst %.cc,%.cc~,$(patsubst %.c,%.c~,$(filter %.c %.cc %.cpp %.cxx %.m,$(SRCS)))))))
else
OBJS=$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.cpp %.cxx %.cc %c ,$(SRCS))))))
FILES=$(patsubst %.cxx,%.cxx~,$(patsubst %.cpp,%.cpp~,$(patsubst %.cc,%.cc~,$(patsubst %.c,%.c~,$(filter %.c %.cc %.cpp %.cxx ,$(SRCS))))))
endif
	
mkappdir:
ifeq ($(FILEEXT), ".app")
	mkdir -p '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Contents/MacOS/Documents
	cp -fr '''+ccc.getINDIELIBROOT()+'''/../IndieLib_resources '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Contents/MacOS/Documents/.
endif
	
ifeq ($(FILEEXT), ".so")
$(TARGET): mkappdir $(OBJS)
	$(GCC) -shared $(LDFLAGS) '''+ccc.getG()+''' '''+ccc.getO()+''' $(OBJS) '''+alibs+''' $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREAD) $(X11) -lstdc++ -lm -o lib/lib$(NAME).so
else ifeq ($(FILEEXT), ".a")
$(TARGET): $(OBJS)
	$(AR) -cr $@ $^
	$(RANLIB) $(TARGET)
else
$(TARGET): mkappdir $(OBJS)
	$(GCC) $(LDFLAGS) '''+ccc.getG()+''' '''+ccc.getO()+''' $(OBJS) '''+alibs+''' $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREAD) $(X11) -lstdc++ -lm -o $@
endif
	
ifeq ($(FILEEXT), ".app")
AppBundle: $(TARGET)
		
all: AppBundle
	#sudo cp -fr '''+ccc.getINDIELIBROOT()+'''/lib/lib*.a /usr/local/lib
clean:
	$(DEL) $(OBJS) $(FILES) $(TARGET)
	$(DEL) '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app
		
else
all: $(TARGET)
	#sudo cp -fr '''+ccc.getINDIELIBROOT()+'''/lib/lib*.a /usr/local/lib
clean:
	$(DEL) $(OBJS) $(FILES) $(TARGET)
endif
	    '''

	    if ccc.getgles2():
		txt0='USE_GLES_SHADERS = "True"\n'+txt0
	
	    if a.gettype()=='application':
		if getOS()=='Darwin':
			txt0=txt0.replace('<FILEEXT>','.app')
		else:
			txt0=txt0.replace('<FILEEXT>','.bin')
	
	    elif a.gettype()=='static_library':
		txt0=txt0.replace('<FILEEXT>','.a')
	    elif a.gettype()=='shared_library':
		txt0=txt0.replace('<FILEEXT>','.so')

	    #txt0=re.sub(r'\s+', ' ', txt0)
	
	    txt0=txt0.replace('//','/')
	    fn='./projectsX/'+a.getname()
	    if not os.path.exists(fn):
		os.makedirs(fn)
	    print fn+'/Makefile'
	    f=open(fn+'/Makefile','w')
	    f.write(txt0)
	    f.close()
	   
	    x='''
		NAME=test
		CC = /usr/bin/gcc
		SRCS=
		FRAMEWORKS:= -framework OpenGL -framework SDL -framework OpenAL
		LIBRARIES:=  -lstdc++ -lm -lobjc
		
		all: $(SRCS)
			$(GCC)  \
				'''+ccc.getG()+''' '''+ccc.getO()+''' $(FRAMEWORKS) $(LIBRARIES) $(SRCS) ./$(NAME).app/Contents/MacOS/$(NAME)
			./$(NAME).app/Contents/MacOS/$(NAME)
	    '''#-fobjc-abi-version=2
	
	def pmain(self, projects):
	    f=open('projectsX/Makefile','w')
	    f.write('\ncopy:\n\tcp -f ./Dependencies/lib/$(LibDir)/libGCC/*.* ./lib/$(LibDir)/.\n')
	    f.write('\nall:\n')
	    for i in projects:
		f.write('\tmake -C '+i.getname()+' all\n')
	        
	    f.write('\nclean:\n')
	    for i in projects:
		f.write('\tmake -C '+i.getname()+' clean\n')
	  
	    f.close()

	def run(self):
		#print self.__class__.__name__
		#exit(0)
		if getOS()=='Darwin':
			configuration, solutions, projects=parse(ccc.getsolution(), 'OSX')
		else:
			configuration, solutions, projects=parse(ccc.getsolution(), 'LNX')
	        v=[]
		for i in projects:
	
			if i.getname() in ['SDL2','OpenAL_x']:
				continue

			LIBDIR=''
			if ccc.getbitness()==32:
				LIBDIR='../../../../IndieLibX_Libraries/IndieLibX_AllLibs_LNX32'
			else:
				if getOS()=='Darwin':
					LIBDIR='../../../../IndieLibX_Libraries/IndieLibX_AllLibs_OSX64'
				else:
					LIBDIR='../../../../IndieLibX_Libraries/IndieLibX_AllLibs_LNX64'
	
			f1='/usr/local/lib/lib'+i.getname()
			f2=f1
			f3=LIBDIR+'/lib'+i.getname()
			if ccc.getbuildproject()=='None':
				if i.gettype()=='static_library':
					if os.path.isfile(f1+'.a') or os.path.isfile(f2+'.a') or os.path.isfile(f3+'.a'):
						print 'Passing static library: '+i.getname()
						continue
				elif i.gettype()=='shared_library':
					if os.path.isfile(f1+'.so') or os.path.isfile(f2+'.so') or os.path.isfile(f3+'.so'):
						print 'Passing dynamic library: '+i.getname()
						continue
			else:
				if not ccc.getbuildproject()==i.getname():
						continue
			self.project2gcc(i)
			v.append(i)
	
		self.pmain(v)
		rrr(v)		
	
#ProjectsMW.py:
class ProjectMW:
	def __init__(self):
		return
	
	def project2gcc(self, a):
	
	    asrcs=' '+ccc.getINDIELIBROOT()+'/'+string.join(a.getsrcs(),' \\\n\t'+ccc.getINDIELIBROOT()+'/')+'\n'
	    v=[]
	    v.append("-fmax-errors="+ccc.getmaxerrors()) 
	    v.append("-D_MSC_VER=1600")
	    if ccc.getbitness()==32: 
		v.append("-DWIN32") 
		v.append("-D_WIN32") 
		v.append("-D__WIN32__")
		v.append("-D__32_BIT__")
	    elif ccc.getbitness()==64: 
		v.append("-DWIN64") 
		v.append("-D_WIN64") 
		v.append("-D__WIN64__")
		v.append("-D__64_BIT__")
	    if not ccc.getDebug():
	    	v.append("-DNDEBUG")
	    else:
	    	v.append("-DDEBUG") 
	    	v.append("-D_DEBUG")
	    if ccc.getgles2():
		v.append("-DUSE_GLES_SHADERS")
	    else:
	    	v.append("-DUSE_HLSL_SHADERS")
	    if ccc.getd3d11(): 
		v.append("-DD3D11")
	    if ccc.getd3d10(): 
		v.append("-DD3D10")
	    if ccc.getd3d9(): 
		v.append("-DD3D9")
	    if ccc.getWindowsKits(): 
		v.append("-DWindowsKits")
	    v.append("-D_SCL_SECURE_NO_WARNINGS")
	    v.append("-D_CRT_SECURE_NO_WARNINGS")
	    v.append("-DPROFILE")
	    v.append("-DD3DXFX_LARGEADDRESS_HANDLE")
	
	    if ccc.getwin8():
		if ccc.getwin8app()=='DesktopApplication':
			v.append("-DWIN8_APP")
		else:
			v.append("-DWIN_PHONE_APP")
	
	    if a.getuc()==True:
		v.append("-D_UNICODE")
		v.append("-DUNICODE")
	    else:
		v.append("-D_MBCS")

	    v.append('-I'+ccc.getINDIELIBROOT()+'/Libraries/BoostWorkaround')
	    for i in a.getincs():
	    	v.append('-I'+ccc.getINDIELIBROOT()+'/'+i)
	
	    aflagsincs=''
	    if len(v)>0:
		aflagsincs=join(v, ' ')

	    v=[]
	    v.append('-L'+ccc.getINDIELIBROOT()+'/lib')
	    v.append('-L/usr/local/lib')
	    for i in a.getlibs():
	    	v.append('-l'+i)
	    for i in a.getsharedlibs():
	        v.append('-l'+i)
	    alibs=join(v, ' ')
	
	    txt0='''NAME = '''+a.getname()+'''
FILEEXT = \"<FILEEXT>\"
	
ifeq ($(FILEEXT), ".dll")
TARGET = '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).dll
else ifeq ($(FILEEXT), ".lib")
TARGET = '''+ccc.getINDIELIBROOT()+'''/lib/lib$(NAME).lib
else ifeq ($(FILEEXT), ".exe")
TARGET = '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).exe
endif

ifneq ("Darwin",$(OS))	
ifneq ("Debian",$(OS))
ifeq ("True",$(USE_GLES_SHADERS))
	OPENGL = -lGLESv2 -lGL -lGLU
else
	OPENGL = -lGL -lGLU
endif
	SDL = 
	X11 =
endif
endif
	
CXXFLAGS += -std=gnu++0x
CPPFLAGS += -std=gnu++0x
CFLAGS += -Wno-enum-compare
CFLAGS += -Wno-comment
	
export CC  = /usr/bin/i686-w64-mingw32-gcc $(ARCH)
export GCC = /usr/bin/i686-w64-mingw32-gcc $(ARCH)
export GPP = /usr/bin/i686-w64-mingw32-g++ $(ARCH)
export AR  = /usr/bin/i686-w64-mingw32-ar
export LD  = /usr/bin/i686-w64-mingw32-ld
export RANLIB = /usr/bin/i686-w64-mingw32-ranlib
	
DEL=/bin/rm -fr
	
CFLAGS += --trigraphs -Wno-format -Wno-write-strings 
	
ifeq ($(FILEEXT), ".dll")
PIC3 = -fPIC
endif
	
COBJFLAGS = -w '''+aflagsincs+'''
CFLAGS += $(PIC3) -fpermissive $(COBJFLAGS)

CXXFLAGS += $(CFLAGS)
CPPFLAGS += $(CFLAGS)
	
SRCS = '''+asrcs+'''
	
%.o: %.c
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' $(CFLAGS) -o $@ -c $<
	
%.o: %.cc
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' -fexceptions -fnon-call-exceptions $(CPLUSPLUS) $(CPPFLAGS) -o $@ -c $<
	
%.o: %.cpp
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' -fexceptions -fnon-call-exceptions $(CPLUSPLUS) $(CPPFLAGS) -o $@ -c $<
	
%.o: %.cxx
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' -fexceptions -fnon-call-exceptions $(CPLUSPLUS) $(CXXFLAGS) -o $@ -c $<
	
%.o: %.m
	echo $@ not included in Linux build
	
OBJS=$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.cpp %.cxx %.cc %c ,$(SRCS))))))
FILES=$(patsubst %.cxx,%.cxx~,$(patsubst %.cpp,%.cpp~,$(patsubst %.cc,%.cc~,$(patsubst %.c,%.c~,$(filter %.c %.cc %.cpp %.cxx ,$(SRCS))))))
	
ifeq ($(FILEEXT), ".dll")
$(TARGET): $(OBJS)
	$(GCC) -shared  $(LDFLAGS) '''+ccc.getG()+''' '''+ccc.getO()+''' $(OBJS) '''+alibs+''' $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREAD) $(X11) -lstdc++ -lm -o lib/lib$(NAME).dll
else ifeq ($(FILEEXT), ".lib")
$(TARGET): $(OBJS)
	$(AR) -cr $@ $^
	$(RANLIB) $(TARGET)
else
$(TARGET): $(OBJS)
	$(GCC) $(LDFLAGS) '''+ccc.getG()+''' '''+ccc.getO()+''' $(OBJS) '''+alibs+''' $(FRAMEWORKS) $(OPENGL) $(SDL) $(CGLIBS) $(PTHREAD) $(X11) -lstdc++ -lm -o $@
endif
	
all: $(TARGET)
clean:
	$(DEL) $(OBJS) $(FILES) $(TARGET)
	
	    '''

	    if ccc.getgles2():
		txt0='USE_GLES_SHADERS = "True"\n'+txt0
	
	    if a.gettype()=='application':
		txt0=txt0.replace('<FILEEXT>','.exe')
	
	    elif a.gettype()=='static_library':
		txt0=txt0.replace('<FILEEXT>','.lib')
	    elif a.gettype()=='shared_library':
		txt0=txt0.replace('<FILEEXT>','.dll')
	
	    txt0=txt0.replace('//','/')
	    #txt0=re.sub(r'\s+', ' ', txt0)

	    fn='./projectsX/'+a.getname()
	    if not os.path.exists(fn):
		os.makedirs(fn)
	    f=open(fn+'/Makefile.mw','w')
	    f.write(txt0)
	    f.close()
	
	def pmain(self, projects):
	    f=open('projectsX/Makefile.mw','w')
	    f.write('\ncopy:\n\tcp -f ./Dependencies/lib/$(LibDir)/libGCC/*.* ./lib/$(LibDir)/.\n')
	    f.write('\nall:\n')
	    for i in projects:
		f.write('\tmake -f Makefile.mw -C '+i.getname()+' all\n')

	        
	    f.write('\nclean:\n')
	    for i in projects:
		f.write('\tmake -f Makefile.mw -C '+i.getname()+' clean\n')
	        	
	    f.close()
	
	def run(self):
		#print self.__class__.__name__
		#exit(0)
		configuration, solutions, projects0=parse(ccc.getsolution(), 'MW')
		projects=[]
		for i in projects0:
			if i.getname() in ['SDL2','OpenAL_x']:
				continue
			if ccc.getbuildproject()=='None':
				if i.gettype()=='static_library':
					if os.path.isfile(os.path.expanduser("~")+'\\Asiakirjat\\lib\\'+i.getname()+'.lib'):
						print 'Passing '+i.getname()
						continue
				elif i.gettype()=='shared_library':
					if os.path.isfile(os.path.expanduser("~")+'\\Asiakirjat\\lib\\'+i.getname()+'.dll'):
						print 'Passing '+i.getname()
						continue
			else:
				if not ccc.getbuildproject()==i.getname():
						continue
			self.project2gcc(i)
			projects.append(i)
		self.pmain(projects)
	
#ProjectsNMAKE.py:
class ProjectNMAKE:
	def __init__(self):
		return
	
	def mkx(self, a,name,type):
		windowsphone=ccc.getwindowsphone()
		acsrcsv=a.getcsrcsv()
		acppsrcsv=a.getcppsrcsv()
		r=''
		if len(acppsrcsv)>0:
	        	r+='\t\t.\\'+string.join(acppsrcsv,' \\\n\t\t\t.\\')
		r=r.replace('/','\\')
		acppsrcs=r
		r=''
		if len(acsrcsv)>0:
	        	r+='\t\t.\\'+string.join(acsrcsv  ,' \\\n\t\t\t.\\')
		r=r.replace('/','\\')
		acsrcs=r
	
		r='\n\t\t'
		v=[]
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
			r+='/LIBPATH:\".\\..\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x86\" \\\n'
		else:
			r+='/LIBPATH:\".\\..\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\" \\\n'
			r+='\t\t/LIBPATH:\".\\..\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\" \\\n'
				
		r+='\t\t/LIBPATH:\"'+ccc.getINDIELIBROOT()+'\\lib\" \\\n'
		r+='\t\t/LIBPATH:\"'+os.path.expanduser("~")+'\\Documents\\lib\" \\\n'
	
		if ccc.getbitness()==64:
			r+='\t\t/LIBPATH:\".\\Dependencies\\SDL2-'+SDL2version+'\\lib\\x64\" \\\n'
			r+='\t\t/LIBPATH:\".\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win64\" \\\n'
		else:
			r+='\t\t/LIBPATH:\".\\Dependencies\\SDL2-'+SDL2version+'\\lib\\x86\" \\\n'
			r+='\t\t/LIBPATH:\".\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win32\" \\\n'
	
		r+='\t\t/LIBPATH:\".\\Dependencies\\Cg\\lib\" \\\n'
	
		if ccc.getgles2():
			r+='\t\t/LIBPATH:\".\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\lib\" \\'
		else:
			r+='\t\t/LIBPATH:\".\\Dependencies\\OpenGL\\lib\" \\'
	
		r+=r2
		
		r=r.replace('.lib .lib','.lib')	
	
	
	
		alibs=r
		v=[]
		for i in a.getincs():
			x=i.replace('/','\\')
			v.append(x)
	        r=''
	        if len(v)>0:
	            if ccc.getwin8():
			r+='\t/I\".\\..\\Program Files (x86)\\Windows Kits\\8.1\\Include\\shared\" \\'
			r+='\n\t\t\t/I\".\\..\\Program Files (x86)\\Windows Kits\\8.1\\Include\\um\" \\'
			#r+='\n\t\t\t/I\".\\..\\Program Files (x86)\\Windows Phone Kits\\8.1\\Include\" \\'
	            else:
		    	#r+='\n\t\t\t/I\".\\..\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Include\" \\'
		    	r+='\t/I\".\\..\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\" \\'
	
	            r+='\n\t\t\t/I\".\\Libraries\\SDL2-'+SDL2version+'\\include\" \\'
		    r+='\n\t\t\t/I\".\\Dependencies\\Cg\\include\" \\'
	
		    if ccc.getgles2():
			r+='\n\t\t\t/I\".\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\include\" \\'
		    else:
		    	r+='\n\t\t\t/I\".\\Dependencies\\OpenGL\\include\" \\'
			
		    r+='\n\t\t\t/I\".\\Libraries\\BoostWorkaround\" \\'
	
		    r+='\n\t\t\t/I\".\\Libraries\\openal-soft-1.15.1\\include\" \\'
	
	            r+='\n\t\t\t/I\".\\'+string.join(v,'\" \\\n\t\t\t/I\"'+ccc.getINDIELIBROOT()+'\\')+'\" '
	        else:
	            r+=''
		aincs=r
	        r=''
	        r+='\t/D"_MSC_VER=1600" \\\n'
		if ccc.getbitness()==32: 
			r+='\t\t\t/D"WIN32" \\\n' 
			r+='\t\t\t/D"_WIN32" \\\n' 
			r+='\t\t\t/D"__WIN32__" \\\n'
			r+='\t\t\t/D"__32_BIT__" \\\n'
		elif ccc.getbitness()==64: 
			r+='\t\t\t/D"WIN64" \\\n' 
			r+='\t\t\t/D"_WIN64" \\\n' 
			r+='\t\t\t/D"__WIN64__" \\\n'
			r+='\t\t\t/D"__64_BIT__" \\\n'
		
	    	if not ccc.getDebug():
			r+='\t\t\t/D"NDEBUG" \\\n' 
	    	else:
			r+='\t\t\t/D"DEBUG" \\\n' 
			r+='\t\t\t/D"_DEBUG" \\\n'

		if ccc.getgles2():
			r+='\t\t\t/D"USE_GLES_SHADERS" \\\n'
		else:
			r+='\t\t\t/D"USE_HLSL_SHADERS" \\\n'
		if ccc.getd3d11(): 
			r+='\t\t\t/D"D3D11" \\\n'
		if ccc.getd3d10(): 
			r+='\t\t\t/D"D3D10" \\\n'
		if ccc.getd3d9(): 
			r+='\t\t\t/D"D3D9" \\\n'
		if ccc.getWindowsKits(): 
			r+='\t\t\t/D"WindowsKits" \\\n'
		r+='\t\t\t/D"_HAS_ITERATOR_DEBUGGING=0" \\\n'
		r+='\t\t\t/D"_SECURE_SCL=0" \\\n'
		r+='\t\t\t/D"_SCL_SECURE_NO_WARNINGS" \\\n'
		r+='\t\t\t/D"_CRT_SECURE_NO_WARNINGS" \\\n'
		r+='\t\t\t/D"PROFILE" \\\n'
		r+='\t\t\t/D"D3DXFX_LARGEADDRESS_HANDLE" \\\n'
	
		if ccc.getwin8():
			if ccc.getwin8app()=='DesktopApplication':
				r+='\t\t\t/D"WIN8_APP" \\\n'
			else:
				r+='\t\t\t/D"WIN_PHONE_APP" \\\n'
	
	        if len(a.getflags())>0:
		    r+='\t\t\t/D\"'+string.join(a.getflags(), '\" \\\n\t\t\t/D\"')+'\" \\\n'
	
		if a.getuc()==True:
			r+='\t\t\t/D"_UNICODE" \\\n\t\t\t/D"UNICODE" \n'
		else:
			r+='\t\t\t/D"_MBCS" \n'
	
	        if r==' /D\"\"':
	            r=''
		aflags=r
	
		mkf ='TARGET_NAME\t=\t'
		if type=="exe":
	   		mkf+='.\\'+a.getexedir().replace('/','\\')+'\\'
	   	elif type=="lib":
	   		mkf+='.\\lib\\'
	   	elif type=="dll":
			mkf+='.\\'+a.getexedir().replace('/','\\')+'\\'
		mkf+=name+'.'+type+'\n'
	
		mkf+='VC = Microsoft Visual Studio\n'
		mkf+='DX_SDK = Microsoft DirectX SDK (June 2010)\n'
		mkf+='PLAT_SDK = Microsoft Platform SDK for Windows Server 2003 R2\n'
	
		mkf+='DIR_INCLUDE\t= '+aincs+'\n'
		
		if type=="exe":
	   		mkf+='LIBRARIES\t= \\\n\t\t'+alibs+'\n'
		else:
	   		mkf+='LIBRARIES\t=\n'
		mkf+='FLAGS\t\t='+aflags
		mkf+='DIR_BIN\t\t= .\\bin\n'
		mkf+='DIR_BIN_X86\t= $(DIR_BIN)\n'
		mkf+='DIR_LIB\t\t= .\\lib\n'
		mkf+='DIR_LIB_X86\t= $(DIR_LIB)\n'
	
		for x in range(0,len(acsrcsv)):
			src='.\\'+acsrcsv[x].replace('\\/','/')
			head,tail=os.path.split(src)
			#print 'src ='+src
			#print 'head='+head
			#print 'tail='+tail
			obj=src.replace('.c','.obj')
			src=src.replace('/','\\')
			obj=obj.replace('/','\\')
			mkf+='\n'+obj+': '+src+'\n'		
			mkf+='\tcl.exe $(DIR_INCLUDE) $(FLAGS) /Od /Zi /EHsc /MDd /Fo\"'+head.replace('/','\\')+'\\\\\" /c \"'+src+'\"\n'
		for x in range(0,len(acppsrcsv)):
			src='.\\'+acppsrcsv[x].replace('\\/','/')
			head,tail=os.path.split(src)
			#print 'src ='+src
			#print 'head='+head
			#print 'tail='+tail
			obj=src.replace('.cpp','.obj')
			src=src.replace('/','\\')
			obj=obj.replace('/','\\')
			mkf+='\n'+obj+': '+src+'\n'	
			mkf+='\tcl.exe $(DIR_INCLUDE) $(FLAGS) /Od /Zi /EHsc /MDd /Fo\"'+head.replace('/','\\')+'\\\\\" /c \"'+src+'\"\n'
	
		mkf+='\nCPPSRCS\t= '+acppsrcs+'\n'
	
		mkf+='CSRCS\t= '+acsrcs+'\n\n'
	
		mkf+='OBJ_FILES = $(CPPSRCS:.cpp=.obj) $(CSRCS:.c=.obj)\n'
	
		mkf+='\n$(TARGET_NAME): $(OBJ_FILES)\n'
		
		if type=="exe":
	   		mkf+='\tlink.exe /out:$(TARGET_NAME) /debug '# /COD ???
	   	elif type=="lib":
	   		mkf+='\tlib.exe /out:$(TARGET_NAME) '# /COD ???
	   	elif type=="dll":
	   		mkf+='\tlink.exe /DLL /out:$(TARGET_NAME) /debug '# /COD ???
		mkf+='$(OBJ_FILES) $(LIBRARIES)\n'
	
		mkf+='\nclean:\n\t@echo clean'
		#if type=="exe":
	   	#	mkf+='\n\tdel $(TARGET_NAME)'
	   	#elif type=="dll":
	   	#	mkf+='\n\tdel $(TARGET_NAME)'
	   	#elif type=="lib":
	   	#	mkf+='\n\tdel $(TARGET_NAME)'
	   	mkf+='\n\tdel $(OBJ_FILES)\n\n'
		mkf+='# create directories and build application\n'
		mkf+='all:\t$(TARGET_NAME)\n\n'
		#mkf+='\t\tcopy '+drive+'\\IndieLibX\\lib\\*.lib %USERPROFILE%\\Documents\\lib\n\n'
	
		mkf=mkf.replace('\\\\Libraries\\','\\Libraries\\')
		mkf=mkf.replace('\\\\Samples\\','\\Samples\\')
	
		mkf=mkf.replace('.\\'+drive+'\\IndieLibX\\','.\\')
	
		mkf=mkf.replace('.\\\\'+drive+'\\IndieLibX\\','.\\')
	
		mkf=mkf.replace('.\\'+drive+'\\Libraries\\','.\\Libraries\\')
	
		mkf=mkf.replace('.\\'+drive+'\\Samples\\','.\\Samples\\')
	
		mkf=mkf.replace('DesktopProjects\\Libraries\\','Libraries\\')	
		
		mkf=mkf.replace('\\'+drive+'\\IndieLibX\\DesktopProjects\\','.\\')
		
		mkf=mkf.replace('.\\DesktopProjects\\','.\\')

		#mkf=re.sub(r'\b\.\\\b', r'\\', mkf)

		#mkf=re.sub(r'\b\.\\', r'\\', mkf)

		if not os.path.exists('projectsX/'+name):
			os.makedirs('projectsX/'+name)
	    	fn='projectsX/'+name+'/Makefile.msc'
	    	f=open(fn,'w')
		f.write(mkf)
		f.close()
	
	def project2gcc(self, a):
	    #raw_input('ProjectsLNX1.run')
	    if a.gettype()=='application':
	        self.mkx(a,a.getname(),"exe")
	    elif a.gettype()=='shared_library':
	        self.mkx(a,a.getname(),"dll")
	    else:
	        self.mkx(a,a.getname(),"lib")
	    #raw_input('ProjectsLNX1.run')
	
	def pmain(self, projects):
	    f=open('projectsX/Makefile.msc','w')
	    
	    #nmake='wine \'/windows/Program Files'+x86+'/Microsoft Visual Studio '+mscver+'/VC/bin/nmake.exe\''
	    nmake='nmake'
	    f.write('\nall:\n')
	    for i in projects:
		if i.gettype()=='static_library':
			if os.path.isfile(os.path.expanduser("~")+'\\Documents\\lib\\'+i.getname()+'.lib'):
				continue
		if i.getname()=='hlsl2glsl':
			continue
		f.write('\t'+nmake+' /F DesktopProjects/projectsX/'+i.getname()+'/Makefile.msc all\n')
	        
	    f.write('\nclean:\n')
	    for i in projects:
		if i.gettype()=='static_library':
			if os.path.isfile(os.path.expanduser("~")+'\\Documents\\lib\\'+i.getname()+'.lib'):
				continue
		if i.getname()=='hlsl2glsl':
			continue
	    	f.write('\t'+nmake+' /F DesktopProjects/projectsX/'+i.getname()+'/Makefile.msc clean\n')
	    f.close()

	def run(self):
		#print self.__class__.__name__
		#exit(0)
		configuration, solutions, projects0=parse(ccc.getsolution(), 'NMAKE')
		projects=[]
		for i in projects0:
	
			if i.getname() in ['SDL2','OpenAL_x']:
				continue
			if ccc.getbuildproject()=='None':
				if i.gettype()=='static_library':
					if os.path.isfile(os.path.expanduser("~")+'\\Documents\\lib\\'+i.getname()+'.lib'):
						print 'Passing '+i.getname()
						continue
				elif i.gettype()=='shared_library':
					if os.path.isfile(os.path.expanduser("~")+'\\Documents\\lib\\'+i.getname()+'.dll'):
						print 'Passing '+i.getname()
						continue
			else:
				if not ccc.getbuildproject()==i.getname():
						continue
			self.project2gcc(i)
			projects.append(i)
		self.pmain(projects)
		rrr(projects)

#ProjectsVC.py:
class ProjectVC:
	x='''
	 <Tool
					Name="VCCLCompilerTool"
					Optimization="0"

	 /Od (Disable (Debug))
	 /O1
	 /O2 (Minimize Size, Maximize Speed)
	 /Ox (Full Optimization)
	'''
	def __init__(self):
	
		self.ilr=os.getcwd()#.replace('/DesktopProjects','')
		if self.ilr.endswith('/DesktopProjects'):
			self.ilr=self.ilr[0:len(self.ilr)-9]
		self.ilr=self.ilr.split('/')
		self.ilr=self.ilr[len(self.ilr)-1]
		
		self.sln1='''Microsoft Visual Studio Solution File, Format Version 10.00
	# Visual C++ Express 2008'''
		self.sln2='''Global
		GlobalSection(SolutionConfigurationPlatforms) = preSolution
			Debug|Win32 = Debug|Win32
			Profile|Win32 = Profile|Win32
			Release|Win32 = Release|Win32
		EndGlobalSection
		GlobalSection(ProjectConfigurationPlatforms) = postSolution
			{D3D09108-96D0-4629-88B8-122C0256058C}.Debug|Win32.ActiveCfg = Debug|Win32
			{D3D09108-96D0-4629-88B8-122C0256058C}.Debug|Win32.Build.0 = Debug|Win32
			{D3D09108-96D0-4629-88B8-122C0256058C}.Profile|Win32.ActiveCfg = Profile|Win32
			{D3D09108-96D0-4629-88B8-122C0256058C}.Profile|Win32.Build.0 = Profile|Win32
			{D3D09108-96D0-4629-88B8-122C0256058C}.Release|Win32.ActiveCfg = Release|Win32
			{D3D09108-96D0-4629-88B8-122C0256058C}.Release|Win32.Build.0 = Release|Win32
			{36381A1F-6F6C-4068-80B7-34C1A576F8BD}.Debug|Win32.ActiveCfg = Debug|Win32
			{36381A1F-6F6C-4068-80B7-34C1A576F8BD}.Debug|Win32.Build.0 = Debug|Win32
			{36381A1F-6F6C-4068-80B7-34C1A576F8BD}.Profile|Win32.ActiveCfg = Profile|Win32
			{36381A1F-6F6C-4068-80B7-34C1A576F8BD}.Profile|Win32.Build.0 = Profile|Win32
			{36381A1F-6F6C-4068-80B7-34C1A576F8BD}.Release|Win32.ActiveCfg = Release|Win32
			{36381A1F-6F6C-4068-80B7-34C1A576F8BD}.Release|Win32.Build.0 = Release|Win32
			{D6F713CD-3103-4F68-8251-9899970594DB}.Debug|Win32.ActiveCfg = Debug|Win32
			{D6F713CD-3103-4F68-8251-9899970594DB}.Debug|Win32.Build.0 = Debug|Win32
			{D6F713CD-3103-4F68-8251-9899970594DB}.Profile|Win32.ActiveCfg = Profile|Win32
			{D6F713CD-3103-4F68-8251-9899970594DB}.Profile|Win32.Build.0 = Profile|Win32
			{D6F713CD-3103-4F68-8251-9899970594DB}.Release|Win32.ActiveCfg = Release|Win32
			{D6F713CD-3103-4F68-8251-9899970594DB}.Release|Win32.Build.0 = Release|Win32
		EndGlobalSection
		GlobalSection(SolutionProperties) = preSolution
			HideSolutionNode = FALSE
		EndGlobalSection
	EndGlobal'''
	
		self.vcprojexe='''<?xml version="1.0" encoding="Windows-1252"?>
	<VisualStudioProject
		ProjectType="Visual C++"
		Version="9,00"
		Name="EmptyProject"
		ProjectGUID="{}"
		RootNamespace="EmptyProject"
		Keyword="Win32Proj"
		TargetFrameworkVersion="131072"
		>
		<Platforms>
			<Platform
				Name="Win32"
			\\>
			<Platform
				Name="x64"
			\\>
		<\\Platforms>
		<ToolFiles>
		<\\ToolFiles>
		<Configurations>
			<Configuration
				Name="Debug|Win32"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="0"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					MinimalRebuild="true"
					ExceptionHandling="1"
					BasicRuntimeChecks="3"
					RuntimeLibrary="3"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="4"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalLibraryDirectories=""
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="2"
					GenerateManifest="true"
					UACExecutionLevel="0"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="1"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Release|Win32"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					ExceptionHandling="1"
					RuntimeLibrary="2"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalLibraryDirectories=""
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					GenerateManifest="true"
					UACExecutionLevel="0"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="1"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Profile|Win32"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					ExceptionHandling="1"
					RuntimeLibrary="2"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					UACExecutionLevel="0"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="1"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Debug|x64"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="0"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					MinimalRebuild="true"
					BasicRuntimeChecks="3"
					RuntimeLibrary="3"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalLibraryDirectories=""
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="2"
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Release|x64"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					RuntimeLibrary="2"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Profile|x64"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					RuntimeLibrary="2"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
		<\\Configurations>
		<References>
		<\\References>
		<Files><\\Files>
		<Globals>
		<\\Globals>
	<\\VisualStudioProject>'''
	
		self.vcprojdll='''<?xml version="1.0" encoding="Windows-1252"?>
	<VisualStudioProject
		ProjectType="Visual C++"
		Version="9,00"
		Name="EmptyProject"
		ProjectGUID="{}"
		RootNamespace="EmptyProject"
		Keyword="Win32Proj"
		TargetFrameworkVersion="131072"
		>
		<Platforms>
			<Platform
				Name="Win32"
			\\>
			<Platform
				Name="x64"
			\\>
		<\\Platforms>
		<ToolFiles>
		<\\ToolFiles>
		<Configurations>
			<Configuration
				Name="Debug|Win32"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="2"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="0"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					MinimalRebuild="true"
					ExceptionHandling="1"
					BasicRuntimeChecks="3"
					RuntimeLibrary="3"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="4"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalLibraryDirectories=""
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="2"
					GenerateManifest="true"
					UACExecutionLevel="0"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="1"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Release|Win32"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="2"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					ExceptionHandling="1"
					RuntimeLibrary="2"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					UACExecutionLevel="0"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="1"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Profile|Win32"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="2"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					ExceptionHandling="1"
					RuntimeLibrary="2"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					UACExecutionLevel="0"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="1"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Debug|x64"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="0"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					MinimalRebuild="true"
					BasicRuntimeChecks="3"
					RuntimeLibrary="3"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalLibraryDirectories=""
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="2"
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Release|x64"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					RuntimeLibrary="2"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Profile|x64"
				OutputDirectory="..\\..\\..\\bin"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					RuntimeLibrary="2"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
		<\\Configurations>
		<References>
		<\\References>
		<Files><\\Files>
		<Globals>
		<\\Globals>
	<\\VisualStudioProject>'''
	
		self.vcprojlib='''<?xml version="1.0" encoding="Windows-1252"?>
	<VisualStudioProject
		ProjectType="Visual C++"
		Version="9,00"
		Name="EmptyProject"
		ProjectGUID="{}"
		RootNamespace="EmptyProject"
		Keyword="Win32Proj"
		TargetFrameworkVersion="131072"
		>
		<Platforms>
			<Platform
				Name="Win32"
			\\>
			<Platform
				Name="x64"
			\\>
		<\\Platforms>
		<ToolFiles>
		<\\ToolFiles>
		<Configurations>
			<Configuration
				Name="Debug|Win32"
				OutputDirectory="..\\..\\..\\lib"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="4"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="0"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					MinimalRebuild="true"
					ExceptionHandling="1"
					BasicRuntimeChecks="3"
					RuntimeLibrary="3"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="4"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLibrarianTool"
					AdditionalLibraryDirectories=""
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Release|Win32"
				OutputDirectory="..\\..\\..\\lib"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="4"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					ExceptionHandling="1"
					RuntimeLibrary="2"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLibrarianTool"
					AdditionalLibraryDirectories=""
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Profile|Win32"
				OutputDirectory="..\\..\\..\\lib"
				IntermediateDirectory="$(ConfigurationName)"
				ConfigurationType="4"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					AdditionalOptions=""
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					ExceptionHandling="1"
					RuntimeLibrary="2"
					EnableEnhancedInstructionSet="2"
					FloatingPointModel="2"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					UsePrecompiledHeader="0"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLibrarianTool"
					AdditionalLibraryDirectories=""
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Debug|x64"
				OutputDirectory="..\\..\\..\\lib"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="0"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					MinimalRebuild="true"
					BasicRuntimeChecks="3"
					RuntimeLibrary="3"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalLibraryDirectories=""
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="2"
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Release|x64"
				OutputDirectory="..\\..\\..\\lib"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					RuntimeLibrary="2"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalLibraryDirectories=""
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
			<Configuration
				Name="Profile|x64"
				OutputDirectory="..\\..\\..\\lib"
				IntermediateDirectory="$(PlatformName)\\$(ConfigurationName)"
				ConfigurationType="1"
				CharacterSet="1"
				>
				<Tool
					Name="VCPreBuildEventTool"
					CommandLine=""
				\\>
				<Tool
					Name="VCCustomBuildTool"
				\\>
				<Tool
					Name="VCXMLDataGeneratorTool"
				\\>
				<Tool
					Name="VCWebServiceProxyGeneratorTool"
				\\>
				<Tool
					Name="VCMIDLTool"
					TargetEnvironment="3"
				\\>
				<Tool
					Name="VCCLCompilerTool"
					Optimization="2"
					EnableIntrinsicFunctions="true"
					AdditionalIncludeDirectories=""
					PreprocessorDefinitions=""
					RuntimeLibrary="2"
					FloatingPointModel="2"
					UsePrecompiledHeader="2"
					PrecompiledHeaderThrough=""
	                WarningLevel="0"
					Detect64BitPortabilityProblems="false"
					DebugInformationFormat="3"
					ExceptionHandling="1"
					ForceConformanceInForLoopScope="true"
					OpenMP="false"
					AdditionalOptions=""
				\\>
				<Tool
					Name="VCManagedResourceCompilerTool"
				\\>
				<Tool
					Name="VCResourceCompilerTool"
				\\>
				<Tool
					Name="VCPreLinkEventTool"
				\\>
				<Tool
					Name="VCLinkerTool"
					AdditionalOptions=""
					AdditionalDependencies=""
					LinkIncremental="1"
					AdditionalLibraryDirectories=""
					GenerateManifest="true"
					DelayLoadDLLs=""
					GenerateDebugInformation="true"
					SubSystem="2"
					LargeAddressAware="2"
					OptimizeReferences="2"
					EnableCOMDATFolding="2"
					RandomizedBaseAddress="2"
					DataExecutionPrevention="2"
					TargetMachine="17"
					UACExecutionLevel="0"
				\\>
				<Tool
					Name="VCALinkTool"
				\\>
				<Tool
					Name="VCManifestTool"
					EmbedManifest="true"
				\\>
				<Tool
					Name="VCXDCMakeTool"
				\\>
				<Tool
					Name="VCBscMakeTool"
				\\>
				<Tool
					Name="VCFxCopTool"
				\\>
				<Tool
					Name="VCAppVerifierTool"
				\\>
				<Tool
					Name="VCPostBuildEventTool"
					CommandLine=""
				\\>
			<\\Configuration>
		<\\Configurations>
		<References>
		<\\References>
		<Files><\\Files>
		<Globals>
		<\\Globals>
	<\\VisualStudioProject>'''
	
	
		self.INDIELIBROOT='../../..'
	
		self.VC = "Microsoft Visual Studio"
		self.DX_SDK = "Microsoft DirectX SDK (June 2010)"
		self.PLAT_SDK = "Microsoft Platform SDK for Windows Server 2003 R2"
	
	def project2pro(self, a):
		filename=a.getname()
		exedir=a.getexedir()
		aguid=a.getguid()
	
		if len(a.getsrcs())>0:
			r='<Files>\n<Filter Name=\"src\" Filter=\"c;cc;cpp;cxx\">\n'
			for i in a.getsrcs():
				r=r+'<File RelativePath=\"..\\..\\..'
				if i[0]=='/':
					r=r+i.replace('/','\\')
				else:
					r=r+'\\'+i.replace('/','\\')
				r=r+'\"><\\File>\n'
			r=r+'<\\Filter>\n<\\Files>'
			r=r.replace('\\\\\\','\\')
			r=r.replace('\\\\','\\')
		else:
			r='<Files><\\Files>'
		asrcs=r
		r=''
	        if ccc.getwin8():
			r+='..\\..\\..\\..\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\include'
			r+=';..\\..\\..\\..\\Program Files (x86)\\Windows Kits\\8.1\\Include\\shared'
			r+=';..\\..\\..\\..\\Program Files (x86)\\Windows Kits\\8.1\\Include\\um'
	        else:
			r+='..\\..\\..\\..\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include'
		r+=';..\\..\\..\\Libraries\\BoostWorkaround'
		r+=';..\\..\\..\\Libraries\\SDL2-'+SDL2version+'\\include'
		if ccc.getgles2():
			r+=';..\\..\\..\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\include'
		else:	
			r+=';..\\..\\..\\Dependencies\\OpenGL\\include'
		r+=';..\\..\\..\\Dependencies\\Cg\\include'
		r+=';..\\..\\..\\Libraries\\openal-soft-1.15.1\\include'	
		if len(a.getincs()):
			r+=';..\\..\\..\\'+string.join(a.getincs(),';..\\..\\..\\')
		r=r.replace('/','\\')
		aincs=r
		r='PreprocessorDefinitions=\"'
		if ccc.getgles2():
			r+='USE_GLES_SHADERS'
		else:
			r+='USE_HLSL_SHADERS'	
		if ccc.getd3d9(): 
			r+=';D3D9'
		if ccc.getd3d10(): 
			r+=';D3D10'
		if ccc.getd3d11(): 
			r+=';D3D11'

		debug=';_DEBUG'
		if not ccc.getDebug():
			debug=';NDEBUG'

		if len(a.getflags()):
			r+=debug+';_HAS_ITERATOR_DEBUGGING=0;_SECURE_SCL=0;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;PROFILE;D3DXFX_LARGEADDRESS_HANDLE;'+string.join(a.getflags(), ';')
		else:
			r+=debug+';_HAS_ITERATOR_DEBUGGING=0;_SECURE_SCL=0;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;PROFILE;D3DXFX_LARGEADDRESS_HANDLE'
			
		if ccc.getbitness()==32:
			r+=';WIN32' 
			r+=';_WIN32' 
			r+=';__WIN32__'
			r+=';__32_BIT__'
		elif ccc.getbitness()==64:
			r+=';WIN64'
			r+=';_WIN64'
			r+=';__WIN64__'
			r+=';__64_BIT__'
		if ccc.getWindowsKits(): 
			r+=';WindowsKits'
		if ccc.getwin8():
			if ccc.getwin8app()=='DesktopApplication':
				r+=';WIN8_APP'
			else:
				r+=';WIN_PHONE_APP'
	
		r+='\"'
		aflags=r
		r=''
		x='SDL2.lib GDI32.lib ole32.lib Winmm.lib Ws2_32.lib Advapi32.lib User32.lib ComCtl32.lib cg.lib cgGL.lib cgD3D9.lib cgD3D10.lib cgD3D11.lib dxguid.lib d3dcompiler.lib '
		if (not ccc.getwin8()) and (not ccc.getwindowsphone()):
			x+='d3d9.lib d3dx9.lib d3d10.lib d3d10_1.lib d3dx10d.lib d3dx10.lib d3d11.lib d3dx11d.lib d3dx11.lib '
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
	
		if len(a.getlibs())>0 and len(a.getsharedlibs())>0:
			r='AdditionalDependencies=\"'+string.join(a.getlibs(),'.lib ')+string.join(a.getsharedlibs(),'.lib ')+'.lib '+x+'\"'
		elif len(a.getlibs())>0:
			r='AdditionalDependencies=\"'+string.join(a.getlibs(),'.lib ')+'.lib '+x+'\"'
		elif len(a.getsharedlibs())>0:
			r='AdditionalDependencies=\"'+string.join(a.getsharedlibs(),'.lib ')+'.lib '+x+'\"'	
		r=r.replace('.lib .lib','.lib')	
	
		alibs=r
	
		if not os.path.exists('projectsX/'+filename):
			os.makedirs('projectsX/'+filename)
		fn='projectsX/'+filename+'/'+filename+'.vcproj'
		#print fn
		f=open(fn,'w')
			
		r=''
		if ccc.getwin8():
			r+=';..\\..\\..\\..\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\lib'
			r+=';..\\..\\..\\..\\Program Files (x86)\\Windows Kits\\8.1\\Lib'
		else:
			r+=';..\\..\\..\\..\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib'
			r+=';..\\..\\..\\..\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86'
	
		if ccc.getbitness()==64:
			r+=';..\\..\\..\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win64'
		else:
			r+=';..\\..\\..\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win32'
	
		libds='AdditionalLibraryDirectories=\"..\\..\\..\\Dependencies\\Cg\\lib\\;'
	
		if a.gles2:
			libds+=';..\\..\\..\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\lib'
		else:
			libds+=';..\\..\\..\\Dependencies\\OpenGL\\lib'
	
		if ccc.getbitness()==64:
			libds+=';..\\..\\..\\Dependencies\\SDL2-'+SDL2version+'\\lib\\x64'
			libds+=';..\\..\\..\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win64'
		else:
			libds+=';..\\..\\..\\Dependencies\\SDL2-'+SDL2version+'\\lib\\x86'
			libds+=';..\\..\\..\\Dependencies\\openal-soft-1.16.0-bin\\libs\\Win32'
	
		libds+=';'+os.path.expanduser("~")+'\\Documents\\lib;..\\..\\..\\lib'+r+'\"'
	
		#..\\..\\..\\Dependencies\\DevIL-SDK-x86-1.7.8\\lib"'
		if a.gettype()=='application':
			txt=self.vcprojexe
			txt=txt.replace('PreprocessorDefinitions=""',aflags)
			txt=txt.replace('AdditionalIncludeDirectories=""','AdditionalIncludeDirectories="'+aincs+'"')
			txt=txt.replace('AdditionalLibraryDirectories=""',libds)
			txt=txt.replace('AdditionalDependencies=""',alibs)
			txt=txt.replace('<Files><\\Files>',asrcs)
			txt=txt.replace('EmptyProject',a.getname())
			txt=txt.replace('ProjectGUID=\"{}\"','ProjectGUID=\"{'+aguid+'}\"')
			if not a.getcharacterset()=='True':
				txt=txt.replace('CharacterSet=\"1\"','CharacterSet=\"2\"')
			txt=txt.replace('\\>','/>')
			txt=txt.replace('<\\','</')
			#txt=txt.replace('\\\\','\\')
			#txt=txt.replace('\\','\\')
			txt=txt.replace('hlsl2glsl.lib','')
			
			exedir2=exedir.replace('/','\\')
			txt=txt.replace('bin',exedir2)

	    		#txt=re.sub(r'\s+', ' ', txt)
			f.write(txt)
		elif a.gettype()=='static_library':
			txt=self.vcprojlib
			txt=txt.replace('PreprocessorDefinitions=""',aflags)
			txt=txt.replace('AdditionalIncludeDirectories=""','AdditionalIncludeDirectories="'+aincs+'"')
			#txt=txt.replace('AdditionalLibraryDirectories=""',libds)
			#txt=txt.replace('AdditionalDependencies=""',libs)
			txt=txt.replace('<Files><\\Files>',asrcs)
			txt=txt.replace('EmptyProject',a.getname())
			txt=txt.replace('ProjectGUID=\"{}\"','ProjectGUID=\"{'+aguid+'}\"')
			if not a.getcharacterset()=='True':
				txt=txt.replace('CharacterSet=\"1\"','CharacterSet=\"2\"')
			txt=txt.replace('\\>','/>')
			txt=txt.replace('<\\','</')
			#txt=txt.replace('\\\\','\\')
			#txt=txt.replace('\\','\\')
			#txt=txt.replace('hlsl2glsl.lib','')

	    		#txt=re.sub(r'\s+', ' ', txt)
			
			f.write(txt)
		elif a.gettype()=='shared_library':
			txt=self.vcprojdll
			txt=txt.replace('PreprocessorDefinitions=""',aflags)
			txt=txt.replace('AdditionalIncludeDirectories=""','AdditionalIncludeDirectories="'+aincs+'"')
			txt=txt.replace('AdditionalLibraryDirectories=""',libds)
			txt=txt.replace('AdditionalDependencies=""',alibs)
			txt=txt.replace('<Files><\\Files>',asrcs)
			txt=txt.replace('EmptyProject',a.getname())
			txt=txt.replace('ProjectGUID=\"{}\"','ProjectGUID=\"{'+aguid+'}\"')
			if not a.getcharacterset()=='True':
				txt=txt.replace('CharacterSet=\"1\"','CharacterSet=\"2\"')
			txt=txt.replace('\\>','/>')
			txt=txt.replace('<\\','</')
			#txt=txt.replace('\\\\','\\')
			#txt=txt.replace('\\','\\')
			txt=txt.replace('hlsl2glsl.lib','')

	    		#txt=re.sub(r'\s+', ' ', txt)
			
			f.write(txt)
	
		f.close()

	def run(self):
		#print self.__class__.__name__
		#exit(0)
		configuration, solutions, projects0=parse(ccc.getsolution(), 'VC')
		#print names(projects)
		#return
		projectnames=[]
		projects=[]
		for i in projects0:
			if i.getname() in ['SDL2','OpenAL_x']:
				continue		
			if ccc.getbuildproject()=='None':
				if i.gettype()=='static_library':
					if os.path.isfile(os.path.expanduser("~")+'\\Documents\\lib\\'+i.getname()+'.lib'):
						print 'Passing '+i.getname()
						continue
			else:
				if not ccc.getbuildproject()==i.getname():
						continue
			if i.getname()=='hlsl2glsl':
				continue
			self.project2pro(i)
			projectnames.append(i.getname())
			projects.append(i)

		ccc.resetC()
		f=open('../Makefile.sln','w')
		f.write(self.sln1+'\n')
		for i in projects:		
			if ccc.getbuildproject()=='None':
				if i.gettype()=='static_library':
					if os.path.isfile(os.path.expanduser("~")+'\\Documents\\lib\\'+i.getname()+'.lib'):
						print 'Passing '+i.getname()
						continue
			else:
				if not ccc.getbuildproject()==i.getname():
						continue
			if not i.getname()=='hlsl2glsl':
				if i.getname() in projectnames:
					f.write('Project(\"{'+ccc.getguid0()+'}\") = \"'+i.getname()+'\", \"DesktopProjects\\projectsX\\'+i.getname()+'\\'+i.getname()+'.vcproj\", "{'+i.getguid()+'}"\n')
					f.write('EndProject\n')
		f.write(self.sln2)
		f.close()

		#print "Exit ProjectsVC.py"
	
#ProjectsNDK.py:
class ProjectNDK:
	def __init__(self):
		return
	
	def project2pro(self, pre,a, prebuiltlib, NDKarch):
	
		asrcs='\t'+ccc.getINDIELIBROOT()+'/'+string.join(a.getsrcs(),' \\\n\t\t\t'             +ccc.getINDIELIBROOT()+'/')+'\n'
		r=''
		r=r+'$(LOCAL_PATH)/../Libraries/SDL2-'+SDL2version+'/include \\\n\t\t\t'
		if not 'Libraries/openil/include' in a.getincs():
			r=r+'$(LOCAL_PATH)/../Libraries/openil/include \\\n\t\t\t'
		if len(a.getincs())>0:
			r=r+'$(LOCAL_PATH)/../'+string.join(a.getincs(),' \\\n\t\t\t$(LOCAL_PATH)/'+ccc.getINDIELIBROOT()+'/')+' \\\n\t\t\t'
		aincs=r
		r=' -DANDROID -DUSE_GLES_SHADERS -D'+string.join(a.getflags(),' -D')
		if r==' -D':
			r=''
		aflags=r
		r='\n\nLOCAL_SHARED_LIBRARIES := '+string.join(a.getsharedlibs(),' ')
		if not 'SDL2' in a.getsharedlibs():#???
			if not 'SDL2' in a.getlibs():#???
				r='\n\nLOCAL_SHARED_LIBRARIES := SDL2 '+string.join(a.getsharedlibs(),' ')#???
		r+='\n\nLOCAL_STATIC_LIBRARIES := '+string.join(a.getlibs(),' ')+' '
		if a.gettype()=='shared_library' or a.gettype()=='application':
			#r+='\nLOCAL_EXPORT_LDLIBS\t:=\t-ldl -lOpenSLES -lGLESv1_CM -lGLESv2 -lGLESv3 -lEGL -llog -landroid -lstdc++ -lc -lm -Wl,--undefined=Java_org_libsdl_app_SDLActivity_nativeInit'
			r+='\nLOCAL_LDLIBS\t:=\t-ldl -lOpenSLES -lGLESv1_CM -lGLESv2 -lGLESv3 -lEGL -llog -landroid -lstdc++ -lc -lm\n'

		alibs=r
	
		filename=a.getname()
		#fn=ccc.getINDIELIBROOT()+'/DesktopProjects/android/android-project/jni/'+filename
	
		fn=ccc.gettargetdir()+'/'+filename
	
		if not os.path.exists(fn):
			os.makedirs(fn)
		fff=fn+'/Android.mk'

		name='main'
		if not a.getname()==ccc.getmainphoneproject():
			name=a.getname()
	
		if prebuiltlib:

			ff='../IndieLibX_AllLibs_NDK/'+NDKarch+'/lib'
			if NDKarch=='arm':
				ff='../IndieLibX_AllLibs_NDK/armeabi/lib'
		
			f=open(fff,'w')
			#print 'fff1:'+fff
			f.write('LOCAL_PATH := $(call my-dir)\n')
			f.write('include $(CLEAR_VARS)\n')
			f.write('LOCAL_MODULE :='+name+'\n')

			if a.gettype()=='static_library':
				f.write('LOCAL_SRC_FILES := '+ff+a.getname()+'.a\n')
				f.write('include $(PREBUILT_STATIC_LIBRARY)\n\n')
			elif a.gettype()=='shared_library':
				f.write('LOCAL_SRC_FILES := '+ff+a.getname()+'.so\n')
				f.write('include $(PREBUILT_SHARED_LIBRARY)\n\n')
			f.close()
	
			return

		debug=' -D_DEBUG -DDEBUG '
		if not ccc.getDebug():
			debug=' -DNDEBUG '
		
		txt='''LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE :=\t\t'''+name+'''
LOCAL_SRC_FILES := '''+asrcs+'''
LOCAL_SRC_FILES += <LOCAL_MAIN_SRC>
LOCAL_C_INCLUDES :=\t$(LOCAL_PATH)/'''+ccc.getINDIELIBROOT()+'''/MVC \\\n\t\t\t$(LOCAL_PATH)/'''+ccc.getINDIELIBROOT()+'''/Libraries/SDL2-'''+SDL2version+''' \\\n\t\t\t$(LOCAL_PATH)/'''+ccc.getINDIELIBROOT()+'''/Libraries/MVC/NDK/jni \\\n\t\t\t$(LOCAL_PATH)/'''+ccc.getINDIELIBROOT()+'''/Libraries/BoostWorkaround \\\n\t\t\t'''+aincs+'''
LOCAL_CFLAGS :=\t\t-w '''+ccc.getG()+''' '''+ccc.getO()+''' '''+aflags+debug+''' -fmax-errors='''+ccc.getmaxerrors()+''' -fexceptions -fpermissive 
LOCAL_CFLAGS += -include "android_fopen.h"
	
'''+alibs+'''
<GLUE_LIBRARIES>
	
<LOCAL_BUILD>
<LOCAL_GLUE>
		'''
	
		if a.gettype()=='application':
			if a.getname()==ccc.getmainphoneproject():
				#txt=txt.replace('<LOCAL_MAIN_SRC>','../SDL/src/main/android/SDL_android_main.c')
				txt=txt.replace('<LOCAL_MAIN_SRC>','')
			else:
	        		txt=txt.replace('<LOCAL_MAIN_SRC>','')
	        	##txt=txt.replace('<LOCAL_MAIN_SRC>','../Libraries/MVC/NDK/jni/main.c \\\n\t\t../Libraries/MVC/NDK/jni/android_fopen.c\n')
			#txt=txt.replace('<LOCAL_GLUE>','$(call import-module,android/native_app_glue)')
			#txt=txt.replace('<GLUE_LIBRARIES>','LOCAL_STATIC_LIBRARIES += android_native_app_glue\n')
		else:
			txt=txt.replace('<LOCAL_MAIN_SRC>','')
		txt=txt.replace('<LOCAL_GLUE>','')
		txt=txt.replace('<GLUE_LIBRARIES>','')
	
		if a.gettype()=='application':
	        	txt=txt.replace('<LOCAL_BUILD>','include $(BUILD_SHARED_LIBRARY)\n\n')
		elif a.gettype()=='shared_library':
			txt=txt.replace('<LOCAL_BUILD>','include $(BUILD_SHARED_LIBRARY)\n\n')
		else:
			txt=txt.replace('<LOCAL_BUILD>','include $(BUILD_STATIC_LIBRARY)\n\n')  
		#txt=txt.replace('-DBUILD_MESHRENDERER','-DBUILD_MESHRENDERER0')
		txt=txt.replace('//','/')
	
		txt=txt.replace('-D -D','-D')
	    	#txt=re.sub(r'\s+', ' ', txt)
		#print 'fff2:'+fff
		f=open(fff,'w')
		f.write(txt+'\n')
		f.close()

	def run(self):
		#print self.__class__.__name__
		#exit(0)
		configuration, solutions, projects=parse(ccc.getsolution(), 'NDK', True)

		v=[]
		v2=[]
		if ccc.getNDKarmeabi():
			v.append('arm')
			v2.append('armeabi')
		if ccc.getNDKx86():
			v.append('x86')
			v2.append('x86')
		if ccc.getNDKmips():
			v.append('mips')
			v2.append('mips')

		SDL2version, assimp3version, NDK=cnf()

		fff='../Libraries/SDL2-'+SDL2version+'/android-project/jni/Application.mk'

		txt='''# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
# APP_STL := stlport_static 

#APP_ABI := armeabi armeabi-v7a x86 # works on emulator
#APP_ABI := armeabi # does not work on emulator
APP_ABI := '''+string.join(v2, ' ')+'''
APP_STL := stlport_static
		'''

		f=open(fff,'w')
		f.write(txt)
		f.close()

		for i in projects:
			if i.getname() in ['SDL2']:
				continue
			for NDKarch in v:
				ff='../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK/'+NDKarch
				NDKarch2=NDKarch
				if NDKarch=='arm':
					ff='../../IndieLibX_Libraries/IndieLibX_AllLibs_NDK/armeabi'
					NDKarch2='armeabi'

				ff+='/lib'+i.getname()

				if ccc.getbuildproject()=='None':
					if i.gettype()=='static_library':
						if os.path.isfile(ff+'.a'):
							print 'Passing static_library: '+i.getname()
							self.project2pro('', i, True, NDKarch)
							continue
					elif i.gettype()=='shared_library':
						if os.path.isfile(ff+'.so'):
							print 'Passing shared_library: '+i.getname()
							self.project2pro('', i, True, NDKarch)
							continue
				else:
					if not ccc.getbuildproject()==i.getname():
						continue
				self.project2pro('', i, False, NDKarch)
	
#ProjectsiOS.py:
class ProjectiOS:
	def __init__(self):
		return
	
	def project2gcc(self, a, aGEN_UUID):
		v=[]
		if ccc.getarm6():
			v.append('armv6')
		if ccc.getarm7():
			v.append('armv7')
		if ccc.geti386():
			v.append('i386')
		if ccc.getarm64():
			v.append('arm64')
		for arch in v:
			self.project2gcc2(a, aGEN_UUID, arch)

		txt0=''
		txt1='Release'
		txt=''
		if a.gettype()=='application':
			txt0=''
		elif a.gettype()=='static_library':
			v=[]
			v.append('-a '+a.getname())
			if ccc.getarm6():
				v.append('-b y')
			else:
				v.append('-b n')
			if ccc.getarm7():
				v.append('-c y')
			else:
				v.append('-c n')
			if ccc.geti386():
				v.append('-d y')
			else:
				v.append('-d n')
			if ccc.getarm64():
				v.append('-e y')
			else:
				v.append('-e n')
			if ccc.getDebug():
				v.append('-f y')
				txt1='Debug'
			else:
				v.append('-f n')
			if ccc.getipad():
				v.append('-g y')
			else:
				v.append('-g n')
			#print '../../iOS/createframework.sh '+string.join(v, ' ')+';'
			txt0='''../../iOS/createframework.sh '''+string.join(v, ' ')+''';'''

		if not a.gettype()=='application':
			txt='''all:
	if [ -e "./Makefile.armv6" ]; then make -f ./Makefile.armv6 all; fi;
	if [ -e "./Makefile.armv7" ]; then make -f ./Makefile.armv7 all; fi;
	if [ -e "./Makefile.i386"  ]; then make -f ./Makefile.i386  all; fi;
	if [ -e "./Makefile.arm64" ]; then make -f ./Makefile.arm64 all; fi;
	'''+txt0+'''

clean:
	if [ -e "./Makefile.armv6" ]; then make -f ./Makefile.armv6 clean; fi;
	if [ -e "./Makefile.armv7" ]; then make -f ./Makefile.armv7 clean; fi;
	if [ -e "./Makefile.i386"  ]; then make -f ./Makefile.i386  clean; fi;
	if [ -e "./Makefile.arm64" ]; then make -f ./Makefile.arm64 clean; fi;
			'''
		else:
			txt='''all:
	if [ -e "./Makefile.'''+ccc.getiOSmain()+'''" ]; then make -f ./Makefile.'''+ccc.getiOSmain()+''' all; fi;
	'''+txt0+'''

clean:
	if [ -e "./Makefile.'''+ccc.getiOSmain()+'''" ]; then make -f ./Makefile.'''+ccc.getiOSmain()+''' clean; fi;
			'''
		fn='./projectsX/'+a.getname()
	    	if not os.path.exists(fn):
			os.makedirs(fn)
	    	fff=fn+'/Makefile.iOS'
	    	f=open(fff,'w')
	    	f.write(txt)
	    	f.close()

	def project2gcc2(self, a, aGEN_UUID, arch):
	
		asrcs=' '+ccc.getINDIELIBROOT()+'/'+string.join(a.getsrcs(),' \\\n\t'+ccc.getINDIELIBROOT()+'/')+'\n'
	        v=[]
		v.append('-ferror-limit='+ccc.getmaxerrors())
		v.append('-Wno-write-strings')
		v.append('-Wno-unused-value')
		v.append('-Wno-c++11-narrowing')
	        v.append('-DUSE_GLES_SHADERS')
	        for i in a.getflags():
			v.append('-D'+i)
	        v.append('-I'+ccc.getINDIELIBROOT()+'/Libraries/BoostWorkaround')
		for i in a.getincs():
	        	v.append('-I'+ccc.getINDIELIBROOT()+'/'+i)
		aflagsincs=join(v, ' ')

		v=[]
		v.append('-L'+ccc.getINDIELIBROOT()+'/lib')
		v.append('-L'+'../../../../IndieLibX_Libraries/IndieLibX_AllLibs_iOS')
		

		for i in a.getlibs():
	            	v.append('-l'+i)
	        for i in a.getsharedlibs():
	            	v.append('-l'+i)
		alibs=join(v, ' ')

		r=' -F ../../../lib -F ../../../../IndieLibX_Libraries/IndieLibX_AllLibs_iOS '
		#r=' -F ../../../lib/Debug-'+arch+' '

		v=[]
		for i in a.getlibs():
			if i=='SDL2':
				continue
			if i=='OpenAL_x':
				continue
	            	v.append('-framework '+i)
	        for i in a.getsharedlibs():
			if i=='SDL2':
				continue
			if i=='OpenAL_x':
				continue
	            	v.append('-framework '+i)
	        aframeworks=r+string.join(v, ' ')

		txt0='iPhoneOS'
		if ccc.geti386():
			txt0='iPhoneSimulator'

		al=''
		debug='CFLAGS += -D_DEBUG -DDEBUG'
		BUILD_TYPE='Debug'
		if not ccc.getDebug():
			debug='CFLAGS += -DNDEBUG'
			BUILD_TYPE='Release'


		iphoneipad='\'iPhone\''
		if ccc.getipad():
			iphoneipad='\'iPad\''

		txt=''
		txt+='''\nNAME := '''+a.getname()+'''
	
FILEEXT = \"<FILEEXT>\"
	
ifeq ($(FILEEXT), ".so")
TARGET = "'''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/$(NAME).so"
else
ifeq ($(FILEEXT), ".a")
TARGET = "'''+ccc.getINDIELIBROOT()+'''/lib/lib$(NAME)-'''+arch+'''-'''+BUILD_TYPE+'''.a"
else
TARGET = "'''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/$(NAME)"
endif
endif

'''+debug+'''
	
IOSARCH = -arch '''+arch+''' -miphoneos-version-min=7.0
	
export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer
	
FRAMEWORKS = -framework CoreMotion -framework GameController -framework OpenGLES -framework UIKit -framework CoreGraphics -framework OpenAL -framework CoreAudio -framework AudioToolbox -framework QuartzCore -framework Foundation -framework CoreFoundation

#CFLAGS += -I'''+ccc.getINDIELIBROOT()+'''/DesktopProjects/iOS -include "iOS_fabs.h"

ifneq ("'''+arch+'''", "i386")
	iPhone=iPhoneOS
	iphone=iphoneos
	CFLAGS += -DOS_IPHONE
else
	iPhone=iPhoneSimulator
	iphone=iphonesimulator
	CFLAGS += -DIPHONE_SIMULATOR
endif

export DEV := $(DEVELOPER_DIR)/Platforms/$(iPhone).platform/Developer
CFLAGS += -stdlib=libc++ -fvisibility=hidden -fvisibility-inlines-hidden
CFLAGS += -isysroot "$(DEV)/SDKs/$(iPhone)$(iOS_VERSION).sdk"
LDFLAGS += -isysroot "$(DEV)/SDKs/$(iPhone)$(iOS_VERSION).sdk"
export GPP = xcrun --sdk $(iphone)$(iOS_VERSION) g++ $(IOSARCH) -std=gnu++0x
export GCC = xcrun --sdk $(iphone)$(iOS_VERSION) gcc $(IOSARCH)
export CC  = xcrun --sdk $(iphone)$(iOS_VERSION) cc $(IOSARCH)
export AR  = xcrun --sdk $(iphone)$(iOS_VERSION) ar
export LD  = xcrun --sdk $(iphone)$(iOS_VERSION) ld
export RANLIB = xcrun --sdk $(iphone)$(iOS_VERSION) ranlib

DEL=/bin/rm -fr
	
BIN := $(DEV)/usr/bin
	
MFLAGS +=  -fobjc-arc -fobjc-abi-version=2
MFLAGS += -fobjc-legacy-dispatch
CFLAGS += -I. -I/usr/X11R6/include -I'''+ccc.getINDIELIBROOT()+'''/Libraries/SDL2-'''+SDL2version+'''/include
CFLAGS += -D__IPHONE_OS_VERSION_MIN_REQUIRED=40100
CFLAGS += -Wno-comment -Wno-import #-Werror
LDFLAGS += -L/usr/X11R6/lib
	
CFLAGS += $(PIC3) '''+aflagsincs+'''
	
MFLAGS += $(CFLAGS)
	
SRCS   = '''+asrcs+'''
	
SRCS  += <LOCAL_MAIN_SRC>

OBJS=$(patsubst %.mm,%.o,$(patsubst %.m,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.cpp %.cxx %.cc %c %.m %.mm ,$(SRCS))))))))
	
FILES=$(patsubst %.mm,%.mm~,$(patsubst %.m,%.m~,$(patsubst %.cxx,%.cxx~,$(patsubst %.cpp,%.cpp~,$(patsubst %.cc,%.cc~,$(patsubst %.c,%.c~,$(filter %.c %.cc %.cpp %.cxx %.m %.mm ,$(SRCS))))))))
	
ifeq ($(FILEEXT), ".a")
$(TARGET): $(OBJS)
	$(AR) -rcs $@ $^
	$(RANLIB) $(TARGET)
else
ifeq ($(FILEEXT), ".so")
$(TARGET): $(OBJS)
	mkdir -p '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app
	$(GCC) -shared $(LDFLAGS) '''+ccc.getO()+''' $^ '''+aframeworks+''' $(FRAMEWORKS) -lstdc++ -lc++ -lm -lc -ObjC -o $@
	
else
$(TARGET): $(OBJS)
	mkdir -p '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app
	$(GCC) $(LDFLAGS) '''+ccc.getO()+''' $^ '''+aframeworks+''' $(FRAMEWORKS) -lstdc++ -lc++ -lm -lc -ObjC -o $@
endif
endif
	
%.o: %.mm
	$(GCC) '''+ccc.getG()+''' '''+ccc.getO()+''' $(MFLAGS) -c -o $@ $<
	
%.o: %.m
	$(GCC) '''+ccc.getG()+''' '''+ccc.getO()+''' $(MFLAGS) -c -o $@ $<
	
%.o: %.cpp
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' $(CFLAGS) -c -o $@ $<
	
%.o: %.cxx
	$(GPP) '''+ccc.getG()+''' '''+ccc.getO()+''' $(CFLAGS) -c -o $@ $<
	
%.o: %.cc
	$(GCC) '''+ccc.getG()+''' '''+ccc.getO()+''' $(CFLAGS) -c -o $@ $<
	
%.o: %.c
	$(GCC) '''+ccc.getG()+''' '''+ccc.getO()+''' $(CFLAGS) -c -o $@ $<

clean: 
	$(DEL) $(OBJS) $(FILES)
		'''
	
		if a.gettype()=='application':
			txt=txt.replace('<LOCAL_MAIN_SRC>','')
	        	#txt=txt.replace('<LOCAL_MAIN_SRC>','../../../Libraries/MVC/iOS/main.m \\\n\t../../../Libraries/MVC/iOS/EAGLView.mm \\\n\t../../../Libraries/MVC/iOS/Lesson01AppDelegate.mm \\\n\t../../../Libraries/MVC/iOS/Lesson01.mm \\\n\t../../../Libraries/MVC/iOS/Lesson.mm\n')
	    	else:
			txt=txt.replace('<LOCAL_MAIN_SRC>','')
	
		if a.gettype()=='application':
			txt=txt.replace('<FILEEXT>','')
	    	elif a.gettype()=='static_library':
			txt=txt.replace('<FILEEXT>','.a')
	    	elif a.gettype()=='shared_library':
			txt=txt.replace('<FILEEXT>','.so')
	
	    	txt=txt.replace('//','/')
	    	txt=txt.replace('-D -I','-I')
	    	#txt=re.sub(r'\s+', ' ', txt)
	    	fn='./projectsX/'+a.getname()
	    	if not os.path.exists(fn):
			os.makedirs(fn)
	    	fff=fn+'/Makefile.'+arch
	    	f=open(fff,'w')
	    	f.write(txt)

		if a.gettype()=='application':

			dev='iPhone-6'
			if ccc.getipad():
				dev='iPad-2'

			txt='''
CODESIGN_ALLOCATE = $(BIN)/codesign_allocate
	
GEN_UUID := '''+a.getguid()+'''
## $(uuid | tr '[:lower:]' '[:upper:]')

code-sign: $(TARGET)
#	codesign -f -s "iPhone Developer" $(TARGET)

AppBundle: code-sign
	cp ../../iOS/Info.plist '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app
	sed -i.bu "s|>test<|>$(NAME)<|g" '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Info.plist
	sed -i.bu "s|>iOSAppFromScratch<|>$(NAME)<|g" '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Info.plist
	sed -i.bu "s|>com.seancrazy.iOSApp<|>'''+ccc.getpackage()+'''<|g" '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Info.plist
	# python -c '\\nimport plistlib\\nimport sys\\np = plistlib.readPlist(sys.argv[1])\\np["CFBundleSupportedPlatforms"] = ["iPhoneSimulator"]\\np["DTPlatformName"] = "iphonesimulator"\\np["DTSDKName"] = "iphonesimulator6.0"\\np["UIDeviceFamily"] = ['$FAMILY']\\nplistlib.writePlist(p, sys.argv[1])' "'''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/Info.plist"
	touch '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app/PkgInfo
	../../iOS/assets.sh

all: AppBundle 
	$(DEL) $(OBJS) $(FILES)
	ios-sim launch '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app --devicetypeid com.apple.CoreSimulator.SimDeviceType.'''+dev+'''

			'''
		else:
			txt='''
all: $(TARGET)
			'''
	    	f.write(txt)
	    	f.close()
	
	def pmain3(self, projects):
	    f=open('projectsX/Makefile.iOS','w')
	    f.write('\nall:\n')
	    for i in projects:
		if ccc.getiPhoneSimulator() and os.path.isdir('../../IndieLibX_Libraries/IndieLibX_AllLibs_iOS/'+i.getname()+'.framework'):
			continue
		f.write('\tmake -f Makefile.iOS -C '+i.getname()+' all\n')
	
	    f.write('\nclean:\n')
	    for i in projects:
		if ccc.getiPhoneSimulator and os.path.isdir('../../IndieLibX_Libraries/IndieLibX_AllLibs_iOS/'+i.getname()+'.framework'):
			continue
		f.write('\tmake -f Makefile.iOS -C '+i.getname()+' clean\n')
	    f.close()

	def run(self):
		#print self.__class__.__name__
		#exit(0)
		configuration, solutions, projects=parse(ccc.getsolution(), 'iOS')
		#raw_input('ProjectsLNX1.run')
		v=[]
		c=0
		for i in projects:
	
			if i.getname() in ['SDL2', 'OpenAL_x']:
				continue

			f1='../../IndieLibX_Libraries/IndieLibX_AllLibs_iOS/'+i.getname()
			if ccc.getbuildproject()=='None':
				if i.gettype()=='static_library':
					if os.path.isdir(f1+'.framework'):
						print 'Passing framework: '+i.getname()
						continue
				elif i.gettype()=='shared_library':
					if os.path.isdir(f1+'.framework'):
						print 'Passing framework: '+i.getname()
						continue
			else:
				if not ccc.getbuildproject()==i.getname():
						continue
			if not i.gettype()=='application':
				self.project2gcc(i,i.getguid())
				c=c+1
				v.append(i)
			elif i.gettype()=='application' and ccc.getmainphoneproject()==i.getname():
				self.project2gcc(i,i.getguid())
				c=c+1
				v.append(i)
				break
		self.pmain3(v)
		#print 'exit ProjectsiOS.py'
	
	
#ProjectsPRO.py:
class ProjectPRO:
	def __init__(self):
		return
	
	def project2pro(self, a):
		filename=a.getname()
		srcs=a.getsrcs()
		lsrcs=a.getlexs()
		ysrcs=a.getyaccs()
		incs=a.getincs()
		flags=a.getflags()
		libs=a.getlibs()
		exedir=a.getexedir()
	
		if not os.path.exists('projectsX/'+filename):
			os.makedirs('projectsX/'+filename)
		fn='projectsX/'+filename+'/'+filename+'.pro'
		f=open(fn,'w')
		txt=''
		if a.gettype()=='application':
			#txt+='QT           += opengl phonon\n'
			txt+='QT           += opengl\n'
		if a.gettype()=='application':
			txt+='TEMPLATE = app\n\n'
			txt+='TARGET = '+ccc.getINDIELIBROOT()+'/'+exedir+'/'+a.getname()+'_2008_d\n'
		elif a.gettype()=='static_library':
			txt+='TEMPLATE = lib\n\nCONFIG += staticlib\n\n'
			txt+='TARGET = '+ccc.getINDIELIBROOT()+'/lib/'+a.getname()+'\n'
		elif a.gettype()=='shared_library':
			txt+='TEMPLATE = lib\n\n'
			txt+='TARGET = '+ccc.getINDIELIBROOT()+'/lib/'+a.getname()+'\n'
	
		txt2=''
		if not ccc.getDebug():
			txt2='''
	DEFINES += QT_BUILD NDEBUG '''
		else:
			txt2='''
	DEFINES += QT_BUILD _DEBUG '''
		txt+=txt2+flags+'\n'+incs+'\n'+srcs+lsrcs+'\n'+ysrcs+'\nLIBS += -L'+ccc.getINDIELIBROOT()+'/lib '+libs+'\n'
	
		if a.gettype()=='application':
			#txt+='wince*{\n'
			#txt+='DEPLOYMENT_PLUGIN += phonon_ds9 phonon_waveout\n'
			#txt+='}\n'
			txt+='HEADERS       += '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/hellogl/glwidget.h \\\n'
			txt+='                '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/hellogl/window.h \\\n'
			txt+='                '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/hellogl/qtlogo.h \n'
			#txt+='                '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/qmusicplayer/mainwindow.h \n'
			txt+='SOURCES       += '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/hellogl/glwidget.cpp \\\n'
			txt+='               '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/hellogl/qtmain.cpp \\\n'
			txt+='                '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/hellogl/window.cpp \\\n'
			txt+='                '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/hellogl/qtlogo.cpp \n'
	
	
			#txt+='                '+ccc.getINDIELIBROOT()+'/Libraries/MVC/QT/qmusicplayer/mainwindow.cpp \n'
	
	
	
	#-L../DesktopProjects/QT/hellogl '+libs+' -lhellogl\n')
	
		if a.gettype()=='application':
	
			txt+='# install\n'
			txt+='target.path = $$[QT_INSTALL_EXAMPLES]/opengl/'+a.getname()+'\n'
			txt+='sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS '+a.getname()+'.pro\n'
			txt+='sources.path = $$[QT_INSTALL_EXAMPLES]/opengl/'+a.getname()+'\n'
			txt+='INSTALLS += target sources\n'
	
			txt+='symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)\n'
		

	    	#txt=re.sub(r'\s+', ' ', txt)
		f.write(txt)
		f.close()
	
	def run(file):
		#print self.__class__.__name__
		#exit(0)
		sln=ccc.getsolution()
		target=ccc.gettarget()
		aopenal=ccc.getopenal()
	    	configuration, solutions, projects=fil2projects(file, sln)
		for i in projects:
			project2pro(i)
	
		f=open('projects.pro','w')
		f.write('TEMPLATE	=	subdirs')
	
		f.write('SUBDIRS		+= projectsX\n')
	
		f.close()
		f=open('projectsX/projectsX.pro','w')
		txt='''TEMPLATE	=	subdirs
	
CFG=Debug|Win32
CF=-rdynamic '''+ccc.getG()+''' '''+ccc.getO()+''' -pipe -Wall -W -std=gnu++0x
FMOD=fmodex-4.28.17
ARFLAGS=cr -rdynamic
LDFLAGS=-rdynamic
INCLUDEPATH +=$(ROOT)usr/include/qt4/Qt 
INCLUDEPATH +=$(ROOT)usr/include/qt4
BMR=-DBUILD_MESHRENDERER
LINUXROOT=$(FILESYSTEMROOT)
	
		'''
	
		#f.write(txt)
	
		f.write('TEMPLATE	=	subdirs\n')
		for i in projects:
			f.write('SUBDIRS		+=	'+i.getname()+'\n')
			
			#f.write('SUBDIRS		+= QT\n')
	
		f.close()
		#print "Exit ProjectsPRO.py"
	
		run(ccc.getproject(), ccc.getsolution(), ccc.getmainphoneproject(), ccc.getopenal())
	
		#print 'exit ProjectsPRO.py'
#ProjectsCMake.py:
class ProjectCMake:
	def __init__(self):
		return
	
	def project2pro(self, a):
		filename=a.getname()
		srcs=a.getsrcs()
		lsrcs=a.getlexs()
		ysrcs=a.getyaccs()
		incs=a.getincs()
		flags=a.getflags()
		libs=a.getlibs()
		exedir=a.getexedir()
	
		if not os.path.exists('projectsX/'+filename):
			os.makedirs('projectsX/'+filename)
		fn='projectsX/'+filename+'/CMakeLists.txt'
		#print fn
		f=open(fn,'w')
		txt=''
		#txt+='#add_definitions(-std=gnu++0x -DLINUX '+flags+')\n'
		txt+='set( CMAKE_C_FLAGS              "${CMAKE_C_FLAGS} '+ccc.getG()+' '+ccc.getO()+' '+flags+' -trigraphs -Wno-format -Wno-enum-compare -Wno-write-strings" CACHE STRING "" )\n'
		
		txt+='set( CMAKE_CXX_FLAGS_RELEASE    "${CMAKE_C_FLAGS} '+ccc.getG()+' '+ccc.getO()+' -std=c++0x -DNDEBUG" CACHE STRING "" )\n'
		txt+='set( CMAKE_C_FLAGS_RELEASE      "${CMAKE_C_FLAGS} '+ccc.getG()+' '+ccc.getO()+' -DNDEBUG" CACHE STRING "" )\n'

		if not ccc.getDebug():
			txt+='set( CMAKE_CXX_FLAGS            "${CMAKE_C_FLAGS} '+ccc.getG()+' '+ccc.getO()+' -std=c++0x -DNDEBUG" CACHE STRING "" )\n'
			txt+='set( CMAKE_C_FLAGS              "${CMAKE_C_FLAGS} '+ccc.getG()+' '+ccc.getO()+' -DNDEBUG" CACHE STRING "" )\n'
		else:
			txt+='set( CMAKE_CXX_FLAGS            "${CMAKE_C_FLAGS} '+ccc.getG()+' '+ccc.getO()+' -std=c++0x -D_DEBUG" CACHE STRING "" )\n'
			txt+='set( CMAKE_C_FLAGS              "${CMAKE_C_FLAGS} '+ccc.getG()+' '+ccc.getO()+' -D_DEBUG" CACHE STRING "" )\n'
		
		txt+='include_directories('+incs+')\n'
		#txt+='link_directories('+ccc.getINDIELIBROOT()+'/lib '+')\n'
		if a.gettype()=='application':
			#txt+='SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../../'+exedir+'")\n'
			##txt+='SET(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../'+exedir+'")\n'
			txt+='add_executable('+a.getname()+' '+srcs+')\n'
			txt+='target_link_libraries ('+a.getname()+' '+libs+')\n'
		elif a.gettype()=='static_library':
			#txt+='SET(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../../lib")\n'
			##txt+='SET(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../lib")\n'
			txt+='add_library('+a.getname()+' STATIC '+srcs+')\n'
		elif a.gettype()=='shared_library':
			#txt+='SET(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/../../../../lib")\n'
			##txt+='SET(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../lib")\n'
			txt+='add_library('+a.getname()+' SHARED '+srcs+')\n'
	
		f.write(txt)
		f.close()
	
	def run(self):
		#print self.__class__.__name__
		#exit(0)
		file=ccc.getfile()
		sln=ccc.getsolution()
		target=ccc.gettarget()
		aopenal=ccc.getopenal()
	    	configuration, solutions, projects=fil2projects(file, sln)
		for i in projects:
			project2pro(i)
	
		f=open('../CMakeLists.txt','w')
		txt=''
		txt+='project (IndieLib)\ncmake_minimum_required(VERSION 2.8)\n'
		txt+='#/* Define to 1 if compiling for Windows. */\n'
		txt+='#cmakedefine WIN32 0\n'
		txt+='#/* Define to 1 if compiling for Mac OS X. */\n'
		txt+='#cmakedefine APPLE 0\n'
		txt+='#/* Define to 1 if compiling for GP2X */\n'
		txt+='#cmakedefine GP2X 0\n'
		txt+='#/* Define to 1 if compiling for A320 */\n'
		txt+='#cmakedefine A320 0\n'
		txt+='#/* Define to 1 if compiling for Android */\n'
		txt+='#cmakedefine ANDROID 0\n'
		txt+='#/* Define to 1 if compiling for iOS */\n'
		txt+='#cmakedefine IOS 0\n'
		txt+='#/* Define to 1 if compiling for an UNIX variant (even Mac OS X.) */\n'
		txt+='#cmakedefine UNIX 1\n'
	
		txt+='#if (ANDROID)\n'
		txt+='\t#cmakedefine NDK=~/'+os.getenv('NDK')+'\n'
		txt+='\t#cmakedefine SYSROOT=$NDK/platforms/android-19/arch-x86\n'
		txt+='\t#cmakedefine CC="$NDK/toolchains/x86-4.8/prebuilt/linux-x86/bin/i686-linux-android-gcc --sysroot=$SYSROOT"\n'
		txt+='\t#cmakedefine CXX="$NDK/toolchains/x86-4.8/prebuilt/linux-x86/bin/i686-linux-android-g++ --sysroot=$SYSROOT"\n'
		txt+='\t#cmakedefine AR="$NDK/toolchains/x86-4.8/prebuilt/linux-x86/bin/i686-linux-android-ar --sysroot=$SYSROOT"\n'
		txt+='\t#cmakedefine LD="$NDK/toolchains/x86-4.8/prebuilt/linux-x86/bin/i686-linux-android-ld --sysroot=$SYSROOT"\n'
	
		txt+='add_subdirectory (DesktopProjects)\n'
		f.write(txt)
		f.close()
	
		f=open('CMakeLists.txt','w')
		f.write('add_subdirectory (projectsX)\n')
		f.close()
	
	    	f=open('projectsX/CMakeLists.txt','w')
		#f.write('add_subdirectory (SDL2)\n')
		for i in projects:
			f.write('add_subdirectory ('+i.getname()+')\n')
			
		f.close()
		#print 'exit ProjectsCMakeGCC.py'
	
		run(ccc.getproject(), ccc.getsolution(), ccc.getmainphoneproject(), ccc.getopenal())
		#print 'exit ProjectsCMake.py'
	
#ProjectsBAT.py:
class ProjectBAT:
	def __init__(self):
		return
		#if 'TERM_PROGRAM' in os.environ and 'Apple_Terminal'==os.environ['TERM_PROGRAM']:
		#	print 'Apple'
		#elif 'TERM' in os.environ and 'xterm'==os.environ['TERM']:
		#	print 'xterm=>Linux???'
		
	def run(self):
	    #print self.__class__.__name__
	    #exit(0)
	    file=ccc.getfile()
	    sln=ccc.getsolution()
	    target=ccc.gettarget()
	    projectnames=[]
	    configuration, solutions, projects=fil2projects(file, sln)
	    #raw_input('ProjectsLNX1.run')
	    f=open(ccc.getINDIELIBROOT()+'/run3.sh','w')
	    f.write('')
	    ff=open(ccc.getINDIELIBROOT()+'/run.bat','w')
	    ff.write('')
	    cc=0
	    for i in projects:
	        if i.getname() in projectnames:
	    		if i.gettype()=='application':
	        		    filename=i.getname()
	        		    exedir=i.getexedir()
	        		    v=exedir.split('/')
	        		    ret='..'
	        		    cret='..'
	        		    c=0
				    cexedir=exedir.replace('/','\\\\')
	        		    for ii in v:
	        		    	if c>0:
	        		    		ret=ret+'/..'
	        		    		cret=cret+'\\..'
	        		    	c=c+1
				    if not cc==0:
				    	f.write(' &&\ncd ./'+exedir+';./'+i.getname()+'.bin;cd '+ret+'')
				    else:
	        		    	f.write('cd ./'+exedir+';./'+i.getname()+'.bin;cd '+ret+'') 
	        		    ff.write('cd '+cexedir+'\ncall '+i.getname()+'.exe\ncd '+cret+'\n') 
	    			    cc=cc+1
	    f.close()
	    ff.close()
	
#pbxproj.py:
class pbxproj:
	# "OTHER_CPLUSPLUSFLAGS": [ "-Wall", "-Wextra", "-pedantic", "-g", "-O0" ]
	def __init__(self):
		self.iOSpbxproj='''// !$*UTF8*$!
	{
		archiveVersion = 1;
		classes = {
		};
		objectVersion = 46;
		objects = {
	
	/* Begin PBXBuildFile section */
	<PBXBuildFile_FILES>1D60589F0D05DD5A006BFB54 /* Foundation.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 1D30AB110D05D00D00671497 /* Foundation.framework */; };
			1DF5F4E00D08C38300B7A737 /* UIKit.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 1DF5F4DF0D08C38300B7A737 /* UIKit.framework */; };
			3A5714C717848CE90060B117 /* CoreGraphics.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 3A5714C617848CE90060B117 /* CoreGraphics.framework */; };
			3A5714C917848D020060B117 /* QuartzCore.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 3A5714C817848D020060B117 /* QuartzCore.framework */; };
			3A609E3C11B9DB4700AA4C48 /* OpenGLES.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 3A609E3B11B9DB4700AA4C48 /* OpenGLES.framework */; };
			3A76BA9A177E2ABB00ECD8AF /* MainWindow-iPad.xib in Resources */ = {isa = PBXBuildFile; fileRef = 3A76BA99177E2ABB00ECD8AF /* MainWindow-iPad.xib */; };
			3AC5C4A816693B6C00FEE565 /* Default-568h@2x.png in Resources */ = {isa = PBXBuildFile; fileRef = 3AC5C4A716693B6C00FEE565 /* Default-568h@2x.png */; };
			3AE0447613D624E400F51EF4 /* MainWindow.xib in Resources */ = {isa = PBXBuildFile; fileRef = 3AE0447513D624E400F51EF4 /* MainWindow.xib */; };
	/* End PBXBuildFile section */
	
	/* Begin PBXFileReference section */
	<PBXFileReference_FILES>1D30AB110D05D00D00671497 /* Foundation.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Foundation.framework; path = System/Library/Frameworks/Foundation.framework; sourceTree = SDKROOT; };
			1DF5F4DF0D08C38300B7A737 /* UIKit.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = UIKit.framework; path = System/Library/Frameworks/UIKit.framework; sourceTree = SDKROOT; };
			32CA4F630368D1EE00C91783 /* iOSGLEssentials_Prefix.pch */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = iOSGLEssentials_Prefix.pch; sourceTree = "<group>"; };
			3A5714C617848CE90060B117 /* CoreGraphics.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = CoreGraphics.framework; path = System/Library/Frameworks/CoreGraphics.framework; sourceTree = SDKROOT; };
			3A5714C817848D020060B117 /* QuartzCore.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = QuartzCore.framework; path = System/Library/Frameworks/QuartzCore.framework; sourceTree = SDKROOT; };
			3A609E3811B9DB3300AA4C48 /* iOSGLEssentials.app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = iOSGLEssentials.app; sourceTree = BUILT_PRODUCTS_DIR; };
			3A609E3B11B9DB4700AA4C48 /* OpenGLES.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = OpenGLES.framework; path = System/Library/Frameworks/OpenGLES.framework; sourceTree = SDKROOT; };
			3A76BA99177E2ABB00ECD8AF /* MainWindow-iPad.xib */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = file.xib; name = "MainWindow-iPad.xib"; path = "Resources/iOS/MainWindow-iPad.xib"; sourceTree = "<group>"; };
			3AC5C4A716693B6C00FEE565 /* Default-568h@2x.png */ = {isa = PBXFileReference; lastKnownFileType = image.png; path = "Default-568h@2x.png"; sourceTree = "<group>"; };
			3AE0447513D624E400F51EF4 /* MainWindow.xib */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = file.xib; name = MainWindow.xib; path = Resources/iOS/MainWindow.xib; sourceTree = "<group>"; };
			8D1107310486CEB800E47090 /* iOSGLEssentials-Info.plist */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.plist.xml; path = "iOSGLEssentials-Info.plist"; plistStructureDefinitionIdentifier = "com.apple.xcode.plist.structure-definition.iphone.info-plist"; sourceTree = "<group>"; };
			AF31B2271301C52D006A0A99 /* Readme.txt */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text; path = Readme.txt; sourceTree = "<group>"; };
	/* End PBXFileReference section */
	
	/* Begin PBXFrameworksBuildPhase section */
			1D60588F0D05DD3D006BFB54 /* Frameworks */ = {
				isa = PBXFrameworksBuildPhase;
				buildActionMask = 2147483647;
				files = (
					3A5714C917848D020060B117 /* QuartzCore.framework in Frameworks */,
					3A5714C717848CE90060B117 /* CoreGraphics.framework in Frameworks */,
					1D60589F0D05DD5A006BFB54 /* Foundation.framework in Frameworks */,
					1DF5F4E00D08C38300B7A737 /* UIKit.framework in Frameworks */,
					3A609E3C11B9DB4700AA4C48 /* OpenGLES.framework in Frameworks */,
				);
				runOnlyForDeploymentPostprocessing = 0;
			};
	/* End PBXFrameworksBuildPhase section */
	
	/* Begin PBXGroup section */
			080E96DDFE201D6D7F000001 /* Classes */ = {
				isa = PBXGroup;
				children = (
				);
				path = Classes;
				sourceTree = "<group>";
			};
			19C28FACFE9D520D11CA2CBB /* Products */ = {
				isa = PBXGroup;
				children = (
					3A609E3811B9DB3300AA4C48 /* iOSGLEssentials.app */,
				);
				name = Products;
				sourceTree = "<group>";
			};
			29B97314FDCFA39411CA2CEA /* CustomTemplate */ = {
				isa = PBXGroup;
				children = (
					3AC5C4A716693B6C00FEE565 /* Default-568h@2x.png */,
					AF31B2271301C52D006A0A99 /* Readme.txt */,
					080E96DDFE201D6D7F000001 /* Classes */,
					29B97315FDCFA39411CA2CEA /* Other Sources */,
					3A609E5B11B9DB7A00AA4C48 /* Shaders */,
					29B97317FDCFA39411CA2CEA /* Resources */,
					29B97323FDCFA39411CA2CEA /* Frameworks */,
					19C28FACFE9D520D11CA2CBB /* Products */,
				);
				name = CustomTemplate;
				sourceTree = "<group>";
			};
			29B97315FDCFA39411CA2CEA /* Other Sources */ = {
				isa = PBXGroup;
				children = (
					32CA4F630368D1EE00C91783 /* iOSGLEssentials_Prefix.pch */,
	<OtherSources>
				);
				name = "Other Sources";
				sourceTree = "<group>";
			};
			29B97317FDCFA39411CA2CEA /* Resources */ = {
				isa = PBXGroup;
				children = (
					3A76BA99177E2ABB00ECD8AF /* MainWindow-iPad.xib */,
					3AE0447513D624E400F51EF4 /* MainWindow.xib */,
					8D1107310486CEB800E47090 /* iOSGLEssentials-Info.plist */,
				);
				name = Resources;
				sourceTree = "<group>";
			};
			29B97323FDCFA39411CA2CEA /* Frameworks */ = {
				isa = PBXGroup;
				children = (
					3A5714C817848D020060B117 /* QuartzCore.framework */,
					3A5714C617848CE90060B117 /* CoreGraphics.framework */,
					1DF5F4DF0D08C38300B7A737 /* UIKit.framework */,
					1D30AB110D05D00D00671497 /* Foundation.framework */,
					3A609E3B11B9DB4700AA4C48 /* OpenGLES.framework */,
				);
				name = Frameworks;
				sourceTree = "<group>";
			};
			3A609E5B11B9DB7A00AA4C48 /* Shaders */ = {
				isa = PBXGroup;
				children = (
				);
				name = Shaders;
				sourceTree = "<group>";
			};
	/* End PBXGroup section */
	
	/* Begin PBXNativeTarget section */
			1D6058900D05DD3D006BFB54 /* GLEssentials */ = {
				isa = PBXNativeTarget;
				buildConfigurationList = 1D6058960D05DD3E006BFB54 /* Build configuration list for PBXNativeTarget "GLEssentials" */;
				buildPhases = (
					1D60588D0D05DD3D006BFB54 /* Resources */,
					1D60588E0D05DD3D006BFB54 /* Sources */,
					1D60588F0D05DD3D006BFB54 /* Frameworks */,
				);
				buildRules = (
				);
				dependencies = (
				);
				name = GLEssentials;
				productName = iOSGLEssentials;
				productReference = 3A609E3811B9DB3300AA4C48 /* iOSGLEssentials.app */;
				productType = "com.apple.product-type.application";
			};
	/* End PBXNativeTarget section */
	
	/* Begin PBXProject section */
			29B97313FDCFA39411CA2CEA /* Project object */ = {
				isa = PBXProject;
				attributes = {
					LastUpgradeCheck = 0420;
				};
				buildConfigurationList = C01FCF4E08A954540054247B /* Build configuration list for PBXProject "iOSGLEssentials" */;
				compatibilityVersion = "Xcode 3.2";
				developmentRegion = English;
				hasScannedForEncodings = 1;
				knownRegions = (
					English,
					Japanese,
					French,
					German,
				);
				mainGroup = 29B97314FDCFA39411CA2CEA /* CustomTemplate */;
				projectDirPath = "";
				projectRoot = "";
				targets = (
					1D6058900D05DD3D006BFB54 /* GLEssentials */,
				);
			};
	/* End PBXProject section */
	
	/* Begin PBXResourcesBuildPhase section */
			1D60588D0D05DD3D006BFB54 /* Resources */ = {
				isa = PBXResourcesBuildPhase;
				buildActionMask = 2147483647;
				files = (
					3A76BA9A177E2ABB00ECD8AF /* MainWindow-iPad.xib in Resources */,
					3AE0447613D624E400F51EF4 /* MainWindow.xib in Resources */,
					3AC5C4A816693B6C00FEE565 /* Default-568h@2x.png in Resources */,
				);
				runOnlyForDeploymentPostprocessing = 0;
			};
	/* End PBXResourcesBuildPhase section */
	
	/* Begin PBXSourcesBuildPhase section */
			1D60588E0D05DD3D006BFB54 /* Sources */ = {
				isa = PBXSourcesBuildPhase;
				buildActionMask = 2147483647;
				files = (
	<PBXSourcesBuildPhase_FILES>
				);
				runOnlyForDeploymentPostprocessing = 0;
			};
	/* End PBXSourcesBuildPhase section */
	
	/* Begin XCBuildConfiguration section */
			1D6058940D05DD3E006BFB54 /* Debug */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ALWAYS_SEARCH_USER_PATHS = NO;
					CLANG_WARN_OBJC_MISSING_PROPERTY_SYNTHESIS = NO;
					COPY_PHASE_STRIP = NO;
					GCC_DYNAMIC_NO_PIC3 = NO;
					GCC_OPTIMIZATION_LEVEL = 0;
					GCC_PRECOMPILE_PREFIX_HEADER = YES;
					GCC_PREFIX_HEADER = iOSGLEssentials_Prefix.pch;
					GCC_PREPROCESSOR_DEFINITIONS = ESSENTIAL_GL_PRACTICES_IOS;
					INFOPLIST_FILE = "iOSGLEssentials-Info.plist";
					IPHONEOS_DEPLOYMENT_TARGET = 6.0;
					PRODUCT_NAME = iOSGLEssentials;
					TARGETED_DEVICE_FAMILY = "1,2";
				};
				name = Debug;
			};
			1D6058950D05DD3E006BFB54 /* Release */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ALWAYS_SEARCH_USER_PATHS = NO;
					CLANG_WARN_OBJC_MISSING_PROPERTY_SYNTHESIS = NO;
					COPY_PHASE_STRIP = YES;
					GCC_PRECOMPILE_PREFIX_HEADER = YES;
					GCC_PREFIX_HEADER = iOSGLEssentials_Prefix.pch;
					GCC_PREPROCESSOR_DEFINITIONS = ESSENTIAL_GL_PRACTICES_IOS;
					INFOPLIST_FILE = "iOSGLEssentials-Info.plist";
					IPHONEOS_DEPLOYMENT_TARGET = 6.0;
					PRODUCT_NAME = iOSGLEssentials;
					TARGETED_DEVICE_FAMILY = "1,2";
					VALIDATE_PRODUCT = YES;
				};
				name = Release;
			};
			C01FCF4F08A954540054247B /* Debug */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ARCHS = "$(ARCHS_STANDARD_32_BIT)";
					CLANG_WARN_OBJC_MISSING_PROPERTY_SYNTHESIS = NO;
					"CODE_SIGN_IDENTITY[sdk=iphoneos*]" = "iPhone Developer";
					GCC_C_LANGUAGE_STANDARD = c99;
					GCC_WARN_ABOUT_RETURN_TYPE = YES;
					GCC_WARN_UNUSED_VARIABLE = YES;
					IPHONEOS_DEPLOYMENT_TARGET = 4.0;
					SDKROOT = iphoneos;
				};
				name = Debug;
			};
			C01FCF5008A954540054247B /* Release */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ARCHS = "$(ARCHS_STANDARD_32_BIT)";
					CLANG_WARN_OBJC_MISSING_PROPERTY_SYNTHESIS = NO;
					"CODE_SIGN_IDENTITY[sdk=iphoneos*]" = "iPhone Developer";
					GCC_C_LANGUAGE_STANDARD = c99;
					GCC_WARN_ABOUT_RETURN_TYPE = YES;
					GCC_WARN_UNUSED_VARIABLE = YES;
					IPHONEOS_DEPLOYMENT_TARGET = 4.0;
					OTHER_CFLAGS = "-DNS_BLOCK_ASSERTIONS=1";
					SDKROOT = iphoneos;
				};
				name = Release;
			};
	/* End XCBuildConfiguration section */
	
	/* Begin XCConfigurationList section */
			1D6058960D05DD3E006BFB54 /* Build configuration list for PBXNativeTarget "GLEssentials" */ = {
				isa = XCConfigurationList;
				buildConfigurations = (
					1D6058940D05DD3E006BFB54 /* Debug */,
					1D6058950D05DD3E006BFB54 /* Release */,
				);
				defaultConfigurationIsVisible = 0;
				defaultConfigurationName = Release;
			};
			C01FCF4E08A954540054247B /* Build configuration list for PBXProject "iOSGLEssentials" */ = {
				isa = XCConfigurationList;
				buildConfigurations = (
					C01FCF4F08A954540054247B /* Debug */,
					C01FCF5008A954540054247B /* Release */,
				);
				defaultConfigurationIsVisible = 0;
				defaultConfigurationName = Release;
			};
	/* End XCConfigurationList section */
		};
		rootObject = 29B97313FDCFA39411CA2CEA /* Project object */;
	}
	'''
	
		pbxproj='''// !$*UTF8*$!
	{
		archiveVersion = 1;
		classes = {
		};
		objectVersion = 46;
		objects = {
	
	/* Begin PBXBuildFile section */
	<PBXBuildFile_FILES>3A1598EA11B9DE0A001E7AB5 /* MainMenu.xib in Resources */ = {isa = PBXBuildFile; fileRef = 3A1598E611B9DE0A001E7AB5 /* MainMenu.xib */; };
			3A1598EB11B9DE0A001E7AB5 /* InfoPlist.strings in Resources */ = {isa = PBXBuildFile; fileRef = 3A1598E711B9DE0A001E7AB5 /* InfoPlist.strings */; };
			3A1598EF11B9DE37001E7AB5 /* OpenGL.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 3A1598EE11B9DE37001E7AB5 /* OpenGL.framework */; };
			3A15990711B9DE6C001E7AB5 /* QuartzCore.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 3A15990611B9DE6C001E7AB5 /* QuartzCore.framework */; };
			8D11072F0486CEB800E47090 /* Cocoa.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 1058C7A1FEA54F0111CA2CBB /* Cocoa.framework */; };
	/* End PBXBuildFile section */
	
	/* Begin PBXFileReference section */
	<PBXFileReference_FILES>1058C7A1FEA54F0111CA2CBB /* Cocoa.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Cocoa.framework; path = /System/Library/Frameworks/Cocoa.framework; sourceTree = "<absolute>"; };
			256AC3F00F4B6AF500CF3369 /* OSXGLEssentials_Prefix.pch */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = OSXGLEssentials_Prefix.pch; sourceTree = "<group>"; };
			29B97324FDCFA39411CA2CEA /* AppKit.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = AppKit.framework; path = /System/Library/Frameworks/AppKit.framework; sourceTree = "<absolute>"; };
			29B97325FDCFA39411CA2CEA /* Foundation.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Foundation.framework; path = /System/Library/Frameworks/Foundation.framework; sourceTree = "<absolute>"; };
			3A1598E611B9DE0A001E7AB5 /* MainMenu.xib */ = {isa = PBXFileReference; lastKnownFileType = file.xib; name = MainMenu.xib; path = Resources/OSX/MainMenu.xib; sourceTree = "<group>"; };
			3A1598E711B9DE0A001E7AB5 /* InfoPlist.strings */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.plist.strings; name = InfoPlist.strings; path = Resources/OSX/InfoPlist.strings; sourceTree = "<group>"; };
			3A1598EE11B9DE37001E7AB5 /* OpenGL.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = OpenGL.framework; path = System/Library/Frameworks/OpenGL.framework; sourceTree = SDKROOT; };
			3A15990611B9DE6C001E7AB5 /* QuartzCore.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = QuartzCore.framework; path = System/Library/Frameworks/QuartzCore.framework; sourceTree = SDKROOT; };
			8D1107310486CEB800E47090 /* OSXGLEssentials-Info.plist */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.plist.xml; path = "OSXGLEssentials-Info.plist"; sourceTree = "<group>"; };
	/* End PBXFileReference section */
	
	/* Begin PBXFrameworksBuildPhase section */
			8D11072E0486CEB800E47090 /* Frameworks */ = {
				isa = PBXFrameworksBuildPhase;
				buildActionMask = 2147483647;
				files = (
					8D11072F0486CEB800E47090 /* Cocoa.framework in Frameworks */,
					3A1598EF11B9DE37001E7AB5 /* OpenGL.framework in Frameworks */,
					3A15990711B9DE6C001E7AB5 /* QuartzCore.framework in Frameworks */,
				);
				runOnlyForDeploymentPostprocessing = 0;
			};
	/* End PBXFrameworksBuildPhase section */
	
	/* Begin PBXGroup section */
			080E96DDFE201D6D7F000001 /* Classes */ = {
				isa = PBXGroup;
				children = (
				);
				name = Classes;
				sourceTree = "<group>";
			};
			1058C7A0FEA54F0111CA2CBB /* Linked Frameworks */ = {
				isa = PBXGroup;
				children = (
					1058C7A1FEA54F0111CA2CBB /* Cocoa.framework */,
					3A1598EE11B9DE37001E7AB5 /* OpenGL.framework */,
					3A15990611B9DE6C001E7AB5 /* QuartzCore.framework */,
				);
				name = "Linked Frameworks";
				sourceTree = "<group>";
			};
			1058C7A2FEA54F0111CA2CBB /* Other Frameworks */ = {
				isa = PBXGroup;
				children = (
					29B97324FDCFA39411CA2CEA /* AppKit.framework */,
					29B97325FDCFA39411CA2CEA /* Foundation.framework */,
				);
				name = "Other Frameworks";
				sourceTree = "<group>";
			};
			19C28FACFE9D520D11CA2CBB /* Products */ = {
				isa = PBXGroup;
				children = (
					3A1598D311B9DDB0001E7AB5 /* OSXGLEssentials.app */,
				);
				name = Products;
				sourceTree = "<group>";
			};
			29B97314FDCFA39411CA2CEA /* OSXGLEssentials */ = {
				isa = PBXGroup;
				children = (
					080E96DDFE201D6D7F000001 /* Classes */,
					29B97315FDCFA39411CA2CEA /* Other Sources */,
					29B97317FDCFA39411CA2CEA /* Resources */,
					3A1598C211B9DD84001E7AB5 /* Shaders */,
					29B97323FDCFA39411CA2CEA /* Frameworks */,
					19C28FACFE9D520D11CA2CBB /* Products */,
				);
				name = OSXGLEssentials;
				sourceTree = "<group>";
			};
			29B97315FDCFA39411CA2CEA /* Other Sources */ = {
				isa = PBXGroup;
				children = (
					256AC3F00F4B6AF500CF3369 /* OSXGLEssentials_Prefix.pch */,
	<OtherSources>
				);
				name = "Other Sources";
				sourceTree = "<group>";
			};
			29B97317FDCFA39411CA2CEA /* Resources */ = {
				isa = PBXGroup;
				children = (
					3A1598E611B9DE0A001E7AB5 /* MainMenu.xib */,
					3A1598E711B9DE0A001E7AB5 /* InfoPlist.strings */,
					8D1107310486CEB800E47090 /* OSXGLEssentials-Info.plist */,
				);
				name = Resources;
				sourceTree = "<group>";
			};
			29B97323FDCFA39411CA2CEA /* Frameworks */ = {
				isa = PBXGroup;
				children = (
					1058C7A0FEA54F0111CA2CBB /* Linked Frameworks */,
					1058C7A2FEA54F0111CA2CBB /* Other Frameworks */,
				);
				name = Frameworks;
				sourceTree = "<group>";
			};
			3A1598C211B9DD84001E7AB5 /* Shaders */ = {
				isa = PBXGroup;
				children = (
				);
				name = Shaders;
				sourceTree = "<group>";
			};
	/* End PBXGroup section */
	
	/* Begin PBXNativeTarget section */
			8D1107260486CEB800E47090 /* OSXGLEssentials */ = {
				isa = PBXNativeTarget;
				buildConfigurationList = C01FCF4A08A954540054247B /* Build configuration list for PBXNativeTarget "OSXGLEssentials" */;
				buildPhases = (
					8D1107290486CEB800E47090 /* Resources */,
					8D11072C0486CEB800E47090 /* Sources */,
					8D11072E0486CEB800E47090 /* Frameworks */,
				);
				buildRules = (
				);
				dependencies = (
				);
				name = OSXGLEssentials;
				productInstallPath = "$(HOME)/Applications";
				productName = OSXGLEssentials;
				productReference = 3A1598D311B9DDB0001E7AB5 /* OSXGLEssentials.app */;
				productType = "com.apple.product-type.application";
			};
	/* End PBXNativeTarget section */
	
	/* Begin PBXProject section */
			29B97313FDCFA39411CA2CEA /* Project object */ = {
				isa = PBXProject;
				attributes = {
					LastUpgradeCheck = 0450;
				};
				buildConfigurationList = C01FCF4E08A954540054247B /* Build configuration list for PBXProject "OSXGLEssentials" */;
				compatibilityVersion = "Xcode 3.2";
				developmentRegion = English;
				hasScannedForEncodings = 1;
				knownRegions = (
					English,
					Japanese,
					French,
					German,
				);
				mainGroup = 29B97314FDCFA39411CA2CEA /* OSXGLEssentials */;
				projectDirPath = "";
				projectRoot = "";
				targets = (
					8D1107260486CEB800E47090 /* OSXGLEssentials */,
				);
			};
	/* End PBXProject section */
	
	/* Begin PBXResourcesBuildPhase section */
			8D1107290486CEB800E47090 /* Resources */ = {
				isa = PBXResourcesBuildPhase;
				buildActionMask = 2147483647;
				files = (
					3A1598EA11B9DE0A001E7AB5 /* MainMenu.xib in Resources */,
					3A1598EB11B9DE0A001E7AB5 /* InfoPlist.strings in Resources */,
				runOnlyForDeploymentPostprocessing = 0;
			};
	/* End PBXResourcesBuildPhase section */
	
	/* Begin PBXSourcesBuildPhase section */
			8D11072C0486CEB800E47090 /* Sources */ = {
				isa = PBXSourcesBuildPhase;
				buildActionMask = 2147483647;
				files = (
	<PBXSourcesBuildPhase_FILES>
				);
				runOnlyForDeploymentPostprocessing = 0;
			};
	/* End PBXSourcesBuildPhase section */
	
	/* Begin XCBuildConfiguration section */
			C01FCF4B08A954540054247B /* Debug */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ALWAYS_SEARCH_USER_PATHS = NO;
					ARCHS = "$(ARCHS_STANDARD)";
					COMBINE_HIDPI_IMAGES = YES;
					COPY_PHASE_STRIP = NO;
					GCC_DYNAMIC_NO_PIC3 = NO;
					GCC_MODEL_TUNING = G5;
					GCC_OPTIMIZATION_LEVEL = 0;
					GCC_PRECOMPILE_PREFIX_HEADER = YES;
					GCC_PREFIX_HEADER = OSXGLEssentials_Prefix.pch;
					INFOPLIST_FILE = "OSXGLEssentials-Info.plist";
					INSTALL_PATH = "$(HOME)/Applications";
					PRODUCT_NAME = OSXGLEssentials;
				};
				name = Debug;
			};
			C01FCF4C08A954540054247B /* Release */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ALWAYS_SEARCH_USER_PATHS = NO;
					ARCHS = "$(ARCHS_STANDARD)";
					COMBINE_HIDPI_IMAGES = YES;
					DEBUG_INFORMATION_FORMAT = "dwarf-with-dsym";
					GCC_MODEL_TUNING = G5;
					GCC_PRECOMPILE_PREFIX_HEADER = YES;
					GCC_PREFIX_HEADER = OSXGLEssentials_Prefix.pch;
					INFOPLIST_FILE = "OSXGLEssentials-Info.plist";
					INSTALL_PATH = "$(HOME)/Applications";
					PRODUCT_NAME = OSXGLEssentials;
				};
				name = Release;
			};
			C01FCF4F08A954540054247B /* Debug */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ARCHS = "$(ARCHS_STANDARD)";
					GCC_C_LANGUAGE_STANDARD = gnu99;
					GCC_OPTIMIZATION_LEVEL = 0;
					"GCC_PREPROCESSOR_DEFINITIONS[arch=*]" = "DEBUG=1";
					GCC_WARN_ABOUT_RETURN_TYPE = YES;
					GCC_WARN_UNUSED_VARIABLE = YES;
					ONLY_ACTIVE_ARCH = YES;
					SDKROOT = "";
				};
				name = Debug;
			};
			C01FCF5008A954540054247B /* Release */ = {
				isa = XCBuildConfiguration;
				buildSettings = {
					ARCHS = "$(ARCHS_STANDARD)";
					GCC_C_LANGUAGE_STANDARD = gnu99;
					GCC_WARN_ABOUT_RETURN_TYPE = YES;
					GCC_WARN_UNUSED_VARIABLE = YES;
					SDKROOT = "";
				};
				name = Release;
			};
	/* End XCBuildConfiguration section */
	
	/* Begin XCConfigurationList section */
			C01FCF4A08A954540054247B /* Build configuration list for PBXNativeTarget "OSXGLEssentials" */ = {
				isa = XCConfigurationList;
				buildConfigurations = (
					C01FCF4B08A954540054247B /* Debug */,
					C01FCF4C08A954540054247B /* Release */,
				);
				defaultConfigurationIsVisible = 0;
				defaultConfigurationName = Release;
			};
			C01FCF4E08A954540054247B /* Build configuration list for PBXProject "OSXGLEssentials" */ = {
				isa = XCConfigurationList;
				buildConfigurations = (
					C01FCF4F08A954540054247B /* Debug */,
					C01FCF5008A954540054247B /* Release */,
				);
				defaultConfigurationIsVisible = 0;
				defaultConfigurationName = Release;
			};
	/* End XCConfigurationList section */
		};
		rootObject = 29B97313FDCFA39411CA2CEA /* Project object */;
	}
	'''
	
	def project2xcode(self, i):
		txt=OSXpbxproj
		if (iOS):
			txt=iOSpbxproj
	
		if i.gettype()=='static_library':
			txt=txt.replace('GLEssentials.app',i.getname()+'.framework')
			txt=txt.replace('wrapper.application','wrapper.framework')
			txt=txt.replace('com.apple.product-type.application','com.apple.product-type.framework')
		elif i.gettype()=='application':
			txt=txt.replace('GLEssentials.app',i.getname()+'.app')
	
		txt=txt.replace('GLEssentials',i.getname())
		txt=txt.replace(i.getname()+'_Prefix.pch','GLEssentials_Prefix.pch')
		txt=txt.replace(i.getname()+'-Info.plist','GLEssentials-Info.plist')
	
		txt=txt.replace('<PBXFileReference_FILES>',i.getsrcs1())
		txt=txt.replace('<PBXBuildFile_FILES>',i.getsrcs3())
		txt=txt.replace('<PBXSourcesBuildPhase_FILES>',i.getsrcs4())
		txt=txt.replace('<PBXGroup_FILES>',i.getsrcs2())
		txt=txt.replace('<OtherSources>',i.getsrcs2())
	
		txt=txt.replace('$(HOME)/Applications','bin')
	
		flags='OS_IPHONE;'
		if (iOS):
			flags='IPHONE_SIMULATOR;'
	
		txt=txt.replace('<GCC_PREPROCESSOR_DEFINITIONS_Release>',flags+'NDEBUG;'+i.getflags())
	
		txt=txt.replace('<GCC_PREPROCESSOR_DEFINITIONS_Debug>',flags+'DEBUG;_DEBUG;'+i.getflags())
	
		txt=txt.replace('//','/')

	    	#txt=re.sub(r'\s+', ' ', txt)
	
		fn='./projectsX/'+i.getname()+'.xcodeproj'
	    	if not os.path.exists(fn):
			os.makedirs(fn)
		f=open(fn+'/project.pbxproj','w')
	    	f.write(txt)
	    	f.close()
	
	def replaceStringInFile(self, filePath,patn1,replacemt1):
		"replaces all string by a regex substitution"
		tempName=filePath+'~~~'
		#print filePath
		input = open(filePath)
		outtext=input.read()
		input.close()
		
		output = open(tempName,'w')
		
		outtext = outtext.replace(patn1,replacemt1)
		
		output.write(outtext)
		output.close()
		
		os.unlink(filePath)
		os.rename(tempName,filePath)
	
	def project2xcode2(self, i):
		fn='projectsX/'+i.getname()+'.xcodeproj'
	    	if not os.path.exists('./'+fn):
			os.makedirs('./'+fn)
		fn+='/project.pbxproj'
		#print './'+fn
		if iOS:
			shutil.copy('./GLEssentials/iOSGLEssentials.xcodeproj/project.pbxproj', './'+fn)
		else:
			shutil.copy('./GLEssentials/OSXGLEssentials.xcodeproj/project.pbxproj', './'+fn)
		xxx=os.getcwd()+'/'+fn
		#print xxx
	
		#com.apple.product-type.library.dynamic
		#com.apple.product-type.library.static
		#com.apple.product-type.application
		#com.apple.product-type.framework
	
		#if i.gettype()=='application':
		if i.gettype()=='static_library':
			replaceStringInFile(xxx,'com.apple.product-type.application','com.apple.product-type.library.static')
		elif i.gettype()=='shared_library':
			replaceStringInFile(xxx,'com.apple.product-type.application','com.apple.product-type.library.dynamic')
		#txt=txt.replace('GLEssentials',i.getname())
		#txt=txt.replace('$(HOME)/Applications','bin')
	
		project = XcodeProject.Load(xxx)
			
		if iOS:
			#project.add_file('System/Library/OpenGLES.framework', tree='SDKROOT')
			#project.add_file('System/Library/UIKit.framework', tree='SDKROOT')
			#project.add_file('System/Library/CoreGraphics.framework', tree='SDKROOT')
			#project.add_file('System/Library/CoreAudio.framework', tree='SDKROOT')
			#project.add_file('System/Library/AudioToolbox.framework', tree='SDKROOT')
			#project.add_file('System/Library/QuartzCore.framework', tree='SDKROOT')
			#project.add_file('System/Library/Foundation.framework', tree='SDKROOT')
			project.remove_file('29B97316FDCFA39411CA2CEA')
			project.remove_file('3A609E3F11B9DB6000AA4C48')
			project.remove_file('3A609E4011B9DB6000AA4C48')
			project.remove_file('3A609E4B11B9DB7700AA4C48')
			project.remove_file('3A609E4C11B9DB7700AA4C48')
			project.remove_file('3A609E4D11B9DB7700AA4C48')
			project.remove_file('3A609E4F11B9DB7700AA4C48')
			project.remove_file('3A609E5011B9DB7700AA4C48')
			project.remove_file('3A609E5111B9DB7700AA4C48')
			project.remove_file('3A609E5211B9DB7700AA4C48')
			project.remove_file('3A609E5311B9DB7700AA4C48')
			project.remove_file('3A609E5411B9DB7700AA4C48')
			project.remove_file('3A609E5511B9DB7700AA4C48')
			project.remove_file('3A609E5C11B9DB9400AA4C48')
			project.remove_file('3A609E5D11B9DB9400AA4C48')
			project.remove_file('3A609E5E11B9DB9400AA4C48')
			project.remove_file('3A609E5F11B9DB9400AA4C48')
			project.remove_file('3A609E6511B9DBE600AA4C48')
			project.remove_file('3A609E6611B9DBE600AA4C48')
			project.remove_file('3A7DCEBC14BE4E9C00C51D27')
			project.remove_file('3A7DCEBD14BE4E9C00C51D27')
			project.remove_file('3AE0447A13D6252E00F51EF4')
			project.remove_file('3AE0447B13D6252E00F51EF4')
			project.remove_file('3AE0447C13D6252E00F51EF4')
			project.remove_file('3AE0447D13D6252E00F51EF4')
			project.remove_file('3AE9119612CAF36A005ED635')
			project.remove_file('537E51321635EC17003EA7BA')
			project.remove_file('537E51331635EC17003EA7BA')
			project.remove_file('AF31B2271301C52D006A0A99')
		else:
			if i.gettype()=='application':
				#project.add_file('System/Library/OpenGL.framework', tree='SDKROOT')
				project.add_file('/Library/Frameworks/SDL2.framework', tree='SDKROOT')
				#project.add_file('System/Library/OpenAL.framework', tree='SDKROOT')
			project.remove_file('29B97316FDCFA39411CA2CEA')
			project.remove_file('3A0BF470177B8C1F00A6C4D7')
			project.remove_file('3A0BF471177B8C1F00A6C4D7')
			project.remove_file('3A0BF478177BC80300A6C4D7')
			project.remove_file('3A0BF479177BC80300A6C4D7')
			project.remove_file('3A1598C311B9DDB0001E7AB5')
			project.remove_file('3A1598C411B9DDB0001E7AB5')
			project.remove_file('3A1598C511B9DDB0001E7AB5')
			project.remove_file('3A1598C711B9DDB0001E7AB5')
			project.remove_file('3A1598C811B9DDB0001E7AB5')
			project.remove_file('3A1598C911B9DDB0001E7AB5')
			project.remove_file('3A1598CA11B9DDB0001E7AB5')
			project.remove_file('3A1598CB11B9DDB0001E7AB5')
			project.remove_file('3A1598CC11B9DDB0001E7AB5')
			project.remove_file('3A1598CD11B9DDB0001E7AB5')
			project.remove_file('3A1598D511B9DDC5001E7AB5')
			project.remove_file('3A1598D611B9DDC5001E7AB5')
			project.remove_file('3A1598DE11B9DDF3001E7AB5')
			project.remove_file('3A1598DF11B9DDF3001E7AB5')
			project.remove_file('3A1598E011B9DDF3001E7AB5')
			project.remove_file('3A1598E111B9DDF3001E7AB5')
			project.remove_file('3A1598E811B9DE0A001E7AB5')
			project.remove_file('3A1598E911B9DE0A001E7AB5')
			project.remove_file('3A349C9112CAEA1100B73A51')
			project.remove_file('3A4554A614BE1FE300837CC3')
			project.remove_file('3A4554A714BE1FE300837CC3')
	
		for ii in i.getsrcs():
			project.add_file('../../'+ii)
	
		for ii in i.getflags2():
			project.add_other_cflags(ii)
		if not ccc.getDebug():
			project.add_other_cflags('NDEBUG')
		else:
			project.add_other_cflags('DEBUG')
			project.add_other_cflags('_DEBUG')
	
		if (iOS):
			if (True):
				project.add_other_cflags('IPHONE_SIMULATOR')
			else:
				project.add_other_cflags('OS_IPHONE')
	
		v=i.getincs()
		for ii in v:
	    		project.add_header_search_paths('../../'+ii)
		if i.gettype()=='application':
	   		v=i.getlibs()
			for ii in v:
				project.add_library_search_paths('../../'+ii)	
			project.add_library_search_paths('../../lib')
			#project.add_framework_search_paths('../../'+)
	
		if project.modified:
			project.backup()
			project.save()
			#project.save(old_format=True)

	def run(self):
	    	#print self.__class__.__name__
	    	#exit(0)
		configuration, solutions, projects=parse(solution)
		
		v=[]
		for i in projects:
			if getOS()=='Darwin':
				project2xcode2(i, openal,iOS)
			else:
				project2xcode(i, openal,iOS)
			v.append(i)
	
		pmain2(v, iOS, mainphoneproject)

def getpackagesources(target, node):
	return ''
	slns=node.findall('.//solution')
	for sln in slns:
		name=sln.find('.//name').text.strip()
		if name.strip()==target.strip():
			configuration, solutions, projects=parse(sln, ccc.getbuildtool())
		
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

def usage(i):
	x='''Solution '''+ccc.getsolution()+''' not found in (5268) '''+ccc.getProjectsxml2()+'''
Usage: ./make.sh [<'''+ccc.getsolutions()+'''>] [<'''+ccc.getbuildtools()+'''>] -> Builds an solution.
Usage: ./make.sh -> Removes Temporary Files.
	'''
	print x
	print i
	if(getOS()=='Windows'):
		os.system("set Projects_py_return_value=3")
	exit(3)

def install():
	txt='''#!/bin/bash
#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#

su root
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

CURL=`which curl`
if [ -z "$CURL" ];then
	echo "Could not find curl. Install curl:"
	if [[ $OSTYPE == darwin* ]]; then
		brew install curl
	fi
	if [[ $OSTYPE == linux* ]]; then
		apt-get -y install curl
	fi
fi

url="'''+ccc.getURL()+'''"
sdlurl="'''+ccc.getSDLURL()+'''"
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
	apt-get update
	apt-get upgrade
	apt-get dist-upgrade
	apt-get -y install gedit cvs git subversion mercurial gcc g++ build-essential valgrind filezilla cmake rar libsdl1.2-dev libsdl-image1.2-dev python bison flex libtool autoconf swig mypaint pencil blender libjpeg-dev libpng12-dev libdevil-dev libassimp3 libtinyxml-dev libsdl2-dev libsdl2-image-dev libopenal-dev openjdk-8-jdk mono-mcs lmms libusb-dev asunder flashplugin-installer
	#gsettings set org.gnome.desktop.screensaver lock-enabled false
fi

if [ ! -f $INC/SDL2/SDL.h ]; then
	rm -fr ./SDL2-$sdl2version
	$CURL -O $sdlurl/release/SDL2-$sdl2version.tar.gz
	gunzip ./SDL2-$sdl2version.tar.gz
	tar -xf ./SDL2-$sdl2version.tar
	cd ./SDL2-$sdl2version
	./configure
	make
	make install
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
	make install
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
		mv ./IndieLibX_AllLibs_LNX64/lib*.a /usr/local/lib
		rm -fr ./IndieLibX_AllLibs_LNX64
	else
		rm -fr ./IndieLibX_AllLibs_LNX32
		$CURL -O $url/IndieLibX_AllLibs_LNX32.zip
		unzip -x ./IndieLibX_AllLibs_LNX32.zip
		mv ./IndieLibX_AllLibs_LNX32/lib*.a /usr/local/lib
		rm -fr ./IndieLibX_AllLibs_LNX32
	fi
	fi
	if [[ $OSTYPE == darwin* ]]; then
		rm -fr ./IndieLibX_AllLibs_OSX64
		$CURL -O $url/IndieLibX_AllLibs_OSX64.zip
		unzip -x ./IndieLibX_AllLibs_OSX64.zip
		mv ./IndieLibX_AllLibs_OSX64/lib*.a /usr/local/lib
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
	apt-get -y install mesa-common-dev
fi
if [ ! -f $INC/GL/glu.h ]; then
	echo "Could not find $INC/GL/glu.h. Install libglu1-mesa-dev:"
	apt-get -y install libglu1-mesa-dev
fi
if [ ! -f /usr/include/Cg/cg.h ]; then
	echo "Could not find /usr/include/Cg/cg.h. Install nvidia-cg-toolkit:"
	apt-get -y install nvidia-cg-toolkit
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
	output.write(txt)
	output.close()
	make_executable(fn)

def xxx():
	x='''

	for child in g_node:
    		if child.name != "prop":
        		continue
		root.remove(child)


	slns=node.findall('.//solution')
			r=sln.find('.//name').text.strip()
	'''
	
	node0=g_node.find('.//projects')
	v=[]
	node_x=0
	if True:
		for node1 in g_node.findall('.//project'):
			for node in node1:
				if node.tag=='platform':
					print 'tag_:'+node.tag
					print 'text_:'+node.text
					if node.text=='iOS':
						node_x=node1
				elif node.tag=='name':
					#print 'tag:'+node.tag
					#print 'text:'+node.text
					if node.text=='SDL2':
						node0.remove(node1)
						v.append(node1)
						#node0.insert(0, node1)
	if False:
		for prj in v:
			name=prj.find('.//name').text.strip()
			platform=prj.find('.//platform').text.strip()
			if name=='SDL2' and platform=='iOS':
				node0.insert(0, prj)
	#node0.insert(0, node_x)
	node0.insert(0, v[len(v)-1])

	for node1 in g_node.findall('.//project'):
		for node in node1:
			if node.tag=='name':
				print 'tag:'+node.tag
				print 'text:'+node.text
		
	#exit(0)

if __name__ == "__main__":

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
	solution='None'
	buildproject='None'

	#print '#:2'

	try:
		options, remainder = getopt.getopt(sys.argv[1:], '', [	'solution=',
	                                                         	'mainphoneproject=',
									'package=',
	                                                         	'OpenAL=',
	                                                         	'SDL2=',
							        	'iOS=',
									'apps=',
									'buildtool=',
									'TargetDirectory=',
									'NDKPlatform=',
									'iPhoneSimulator=',
	                                                         	'WindowsPhone=',
	                                                         	'GLES2=',
									'MVC=',
									'buildproject='
									])
		for opt, arg in options:

			if opt=='--solution':
				solution = arg

			if opt=='--buildproject':
				buildproject = arg
		
			if opt=='--buildtool':


				if getOS()=='Darwin':
					if(not arg in ['None','LNX','OSX','iOS','MW','NDK','CMake','PRO']):
						usage(3)

				elif(getOS() in ['Ubuntu', 'Debian', 'Linux']):
					if(not arg in ['None','LNX','MW','NDK','CMake','PRO']):
						usage(4)
				elif(getOS()=='Windows'):
					if(not arg in ['None','MW','NMAKE','VC','NDK','CMake','BAT','PRO']):
						usage(5)
	        		ccc.setbuildtool(arg)
			 
	except getopt.GetoptError as err:
		print str(err)
		usage(6)
		sys.exit(2)

	#print '#:3'

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

		if not buildproject=='None':
			ccc.setbuildproject(buildproject)

		g_node = ElementTree.fromstring(txt)
		#xxx()
	else:
		with open(ccc.getProjectsxml(), 'rt') as f:
			tree = ElementTree.parse(f)
			g_node=tree.getroot()

	#print '#:5'

	packagesources=getpackagesources(solution, g_node)
	ccc.setsolutions(getslns(g_node))
	ccc.setprojects(getprjs(g_node))
	ccc.settextfromxml(g_node)
	install()

	#print '#:6'
	if not solution=='None':
		#print '#:61'
		if not checktarget(solution, g_node):
			x='''Solution '''+solution+''' not found in (5681) '''+ccc.getProjectsxml2()+'''
Usage: ./make.sh [<'''+ccc.getsolutions()+'''>] [<'''+ccc.getbuildtools()+'''>] -> Builds an solution.
Usage: ./make.sh -> Removes Temporary Files.
			'''
			print x

			#os.system("set Projects_py_return_value=3")
			#exit(3)

		iOS='iPhoneOS'
		if ccc.getiPhoneSimulator():
			iOS='iPhoneSimulator'

		NDKarmeabi='False'
		if ccc.getNDKarmeabi():
			NDKarmeabi='True'
		NDKx86='False'
		if ccc.getNDKx86():
			NDKx86='True'
		NDKmips='False'
		if ccc.getNDKmips():
			NDKmips='True'

		if(getOS()=='Windows'):
			outtext='''rem This file is automatically generated by Projects.py.
rem Do not modify this file -- YOUR CHANGES WILL BE ERASED!
set iOSplatform='''+iOS+'''
set NDKplatform='''+ccc.getNDKplatform()+'''
set NDKarmeabi='''+NDKarmeabi+'''
set NDKx86='''+NDKx86+'''
set NDKmips='''+NDKmips+'''
set solutions=\"'''+ccc.getsolutions()+'''\"
set projects=\"'''+ccc.getprojects()+'''\"
set platforms=\"'''+ccc.getbuildtools()+'''\"
set solution='''+solution+'''
set mainphoneproject='''+ccc.getmainphoneproject()+'''
set package='''+ccc.getpackage()+'''
set domain=\"'''+ccc.getDomain()+'''\"
set androidurl=\"'''+ccc.getandroidURL()+'''\"
set sdlurl=\"'''+ccc.getSDLURL()+'''\"
set adt=\"'''+ccc.getADT()+'''\"
set assimp3version=\"'''+assimp3version+'''\"
set ndk=\"'''+NDK+'''\"
set sdl2version='''+SDL2version+'''
set sdlimage2version=\"'''+ccc.getSDLIMAGE2version()+'''\"
set OSXversion=\"'''+ccc.getOSXversion()+'''\"
set Debianversion=\"'''+ccc.getDebianversion()+'''\"
set Ubuntuversion=\"'''+ccc.getUbuntuversion()+'''\"
set pythonUbuntuversion=\"'''+ccc.getpythonUbuntuversion()+'''\"
set pythonDebianversion=\"'''+ccc.getpythonDebianversion()+'''\"
set pythonOSXversion=\"'''+ccc.getpythonOSXversion()+'''\"'''
			output = open('../cnf.bat','w')		
			output.write(outtext)
			output.close()
		else:
			#print '#:62'
			outtext='''# This file is automatically generated by Projects.py.
# Do not modify this file -- YOUR CHANGES WILL BE ERASED!
iOSplatform='''+iOS+'''
NDKplatform='''+ccc.getNDKplatform()+'''
NDKarmeabi='''+NDKarmeabi+'''
NDKx86='''+NDKx86+'''
NDKmips='''+NDKmips+'''
solutions=\"'''+ccc.getsolutions()+'''\"
projects=\"'''+ccc.getprojects()+'''\"
platforms=\"'''+ccc.getbuildtools()+'''\"
solution='''+solution+'''
mainphoneproject='''+ccc.getmainphoneproject()+'''
package='''+ccc.getpackage()+'''
domain=\"'''+ccc.getDomain()+'''\"
androidurl=\"'''+ccc.getandroidURL()+'''\"
sdlurl=\"'''+ccc.getSDLURL()+'''\"
adt=\"'''+ccc.getADT()+'''\"
assimp3version=\"'''+assimp3version+'''\"
ndk=\"'''+NDK+'''\"
sdl2version='''+SDL2version+'''
sdlimage2version=\"'''+ccc.getSDLIMAGE2version()+'''\"
OSXversion=\"'''+ccc.getOSXversion()+'''\"
Debianversion=\"'''+ccc.getDebianversion()+'''\"
Ubuntuversion=\"'''+ccc.getUbuntuversion()+'''\"
pythonUbuntuversion=\"'''+ccc.getpythonUbuntuversion()+'''\"
pythonDebianversion=\"'''+ccc.getpythonDebianversion()+'''\"
pythonOSXversion=\"'''+ccc.getpythonOSXversion()+'''\"'''
			output = open('../make.cnf','w')		
			output.write(outtext)
			output.close()
		exit()

	#print '#:7'
	
	ccc=Configuration()
	ccc.settextfromxml(g_node)

	#getpackagesources(solution, g_node)
	
	p=0
	test=False

	#print '#:8'
	if not test:
		if(getOS() in ['Ubuntu', 'Debian', 'Linux']) and (ccc.getbuildtool()=='LNX'):
			p=ProjectLNX()
		elif(getOS()=='Darwin') and (ccc.getbuildtool()=='OSX'):
			p=ProjectLNX()
		#elif((sys.platform=='darwin') and (ccc.getbuildtool()=='OSX')):
		#	p=pbxproj()
		elif((getOS()=='Darwin') and (ccc.getbuildtool()=='iOS')):
			p=ProjectiOS()
			##p=ProjectCPiOS()
			#p=ProjectPMiOS()
		elif(ccc.getbuildtool()=='MW'):
			p=ProjectMW()
		elif((getOS()=='Windows') and (ccc.getbuildtool()=='NMAKE')):
			p=ProjectNMAKE()
		elif((getOS()=='Windows') and (ccc.getbuildtool()=='VC')):
			p=ProjectVC()
		elif(ccc.getbuildtool()=='NDK'):
			p=ProjectNDK()
			#return "./androidbuild0.sh "+ccc.getpackage()+" "+ccc.getsources()
		elif(ccc.getbuildtool()=='CMake'):
			p=ProjectCMake()
		elif((getOS()=='Windows') and (ccc.getbuildtool()=='BAT')):
			p=ProjectBAT()   
		elif(ccc.getbuildtool()=='PRO'):
			p=ProjectPRO()
		p.run()
	else:
		p=ProjectLNX()
		p.run()
		#p=pbxproj()
		p=ProjectiOS()
		##p=ProjectCPiOS()
		#p=ProjectPMiOS()
		p.run()
		p=ProjectMW()
		p.run()
		p=ProjectNMAKE()
		p.run()
		p=ProjectVC()
		p.run()
		p=ProjectNDK()
		p.run()
		p=ProjectCMake()
		p.run()
		#p=ProjectBAT()   
		p=ProjectPRO()
		p.run()
	#getpackagesources(target, g_node)

	#print '#:9'

x='''
Building Boost (Advanced Users)

Here's how to compile Boost yourself:
OS X & iOS:

You'll need to create ~/user-config.jam with the following (and possibly correct the paths to the iOS SDKs):

using clang : osx
: xcrun clang -arch i386 -arch x86_64 -stdlib=libc++ -std=c++11 -mmacosx-version-min=10.8
;

using clang : ios
: xcrun clang -arch armv7 -arch arm64 -stdlib=libc++ -std=c++11 -miphoneos-version-min=6.0 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.0.sdk/
;

using clang : ios_sim
: xcrun clang -arch i386 -arch x86_64 -stdlib=libc++ -std=c++11 -miphoneos-version-min=6.0 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator9.0.sdk/
;

OS X:

'''

# ios-sim launch '''+ccc.getINDIELIBROOT()+'''/'''+a.getexedir()+'''/$(NAME).app --devicetypeid com.apple.CoreSimulator.SimDeviceType.iPad-2

