import os
from os import path
#from os import unlink
import sys
import re
import string

def load(file):
	ffile = os.path.abspath(file)
	f=open(ffile,'r')
	lines=f.readlines()
	f.close()
	return string.join(lines,'')

def main():
	txt=load('../projects/projectsX/IndieLib_Static/IndieLib_Static.vcproj')
	p = re.compile(r"\bRelativePath\b\s*\=\s*\".*?\.cpp\"")
	l=p.findall(txt)
	#print l
	for i in l:
		p2 = re.compile(r"\bRelativePath\b\s*\=\s*\"(.*?)\.cpp\"")
		m=p2.match(i)
		if m:
			#print m.group(1)+'.cpp'
			fn=m.group(1)+'.cpp'
			fn=fn.replace('..\\..\\..\\IndieLib\\IndieLib\\Common\\','..\\IndieLib\\IndieLib\\Common\\')
			fn=fn.replace('\\','/')
			txt2=load(fn)
			p3 = re.compile(r'''\binclude\b\s+[\"\<]+.*?\n''')
			l3=p3.findall(txt2)
			print l3
	

main()

