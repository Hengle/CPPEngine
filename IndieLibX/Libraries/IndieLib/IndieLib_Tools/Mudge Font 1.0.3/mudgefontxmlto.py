##
## mudgefontxmlto.py
##
## Converts the .XML file output by mudGEfont into
## another format, in case you didn't actually want
## to include an XML parser in your game.
##
## Written by Larry Hastings.
## You are free to use, modify, and redistribute it,
## also for commercial use.. no GPL or anything...
## just completely free.
##
## Find my mudGEfont home page here:
##	http://www.midwinter.com/~lch/programming/mudgefont/
##


import sys
import time
import xml.dom.minidom


##
## The almost-non-existant (and probably not needed) base class
## for the writers below.
##
class writer:
	def commentText(self):
		return "Generated by mudgefontxmlto.py at " + time.strftime("%Y/%m/%d %H:%M:%S") + "\r\n"

	def close(self):
		self.f.close()


##
## This class writes out the output of the XML file as a .txt file,
## in my own funny "Twiddle" file format.  It's probably not useful
## for anyone else.
##
class twiddleWriter(writer):
	def __init__(self, filename):
		self.f = open(filename + ".txt", "wb")
		self.f.write("// " + "\r\n")
		self.f.write("// " + self.commentText())
		self.f.write("// " + "\r\n\r\n")

	def write(self, char, spacing):
		self.f.write("".join([char.getAttribute("id"), "\r\n\t{\r\n"]))
		for attribute in [ "x", "y", "width", "height" ] :
			self.f.write("".join(["\t", attribute, " = ", char.getAttribute(attribute), "\r\n"]))
		self.f.write("\t}\r\n\r\n")



##
## This class writes out the output of the XML file as a .ini file.
## The "section" is the character number (e.g. "[65]" for capital A),
## and each section gets four key/value pairs: x, y, width, and height.
##
class iniWriter(writer):
	def __init__(self, filename):
		self.f = open(filename + ".ini", "wb")
		self.f.write("; " + "\r\n")
		self.f.write("; " + self.commentText())
		self.f.write("; " + "\r\n\r\n")

	def write(self, char, spacing):
		self.f.write("".join(["[", char.getAttribute("id"), "]\r\n"]))
		for attribute in [ "x", "y", "width", "height" ] :
			self.f.write("".join([attribute, " = ", char.getAttribute(attribute), "\r\n"]))
		self.f.write("\r\n")



##
## This class writes out the output of the XML file as a .c file
## containing a font info structure.  It's not useful as is--hack away.
##
class cWriter(writer):
	def __init__(self, filename):
		self.characters = {}
		self.f = open(filename + ".c", "wb")
		self.f.write("/* " + "\r\n")
		self.f.write("** " + self.commentText())
		self.f.write("*/ " + "\r\n\r\n")
		self.f.write("""
struct fontInfo
	{
	int x;
	int y;
	int width;
	int height;
	};

fontInfo font[256] =
	{
""")

	def write(self, char, spacing):
		hash = {}
		for attribute in [ "x", "y", "width", "height" ] :
			hash[attribute] = char.getAttribute(attribute)
		self.characters[char.getAttribute("id")] = hash

	def close(self):
		blankHash = { "x": "0", "y": "0", "width": "0", "height": "0" }
		for i in range(255):
			char = str(i)
			if char in self.characters:
				hash = self.characters[char]
			else:
				hash = blankHash
			self.f.write("".join(["\t{ ", hash["x"], ", ", hash["y"], ", ", hash["width"], ", ", hash["height"], " }, /* ", char, " */\r\n"]))

		self.f.write("\t};\r\n\r\n")
		self.f.close()


##
## Here's the actual live code that chews through the XML files.
##
if len(sys.argv) < 2:
	print "usage: python mudgefontxmlto.py file1.xml [ file2.xml ... ]\n"
	sys.exit(0)

for filename in sys.argv[1:]:
	basename = filename
	if (basename[-4:] == ".xml"):
		basename = basename[:-4]


##
## Here's where you pick the output!  Just uncomment the one you want,
## and comment out the rest.
##

	writer = twiddleWriter(basename)
#	writer = iniWriter(basename)
#	writer = cWriter(basename)

	dom = xml.dom.minidom.parse(basename + ".xml")
	char = None
	# this code is dependent on the mudGEfont xml not changing particularly.
	# in particular, it assumes it always alternates char/spacing/char/spacing.
	for node in dom.firstChild.childNodes:
		if node.nodeType != xml.dom.Node.ELEMENT_NODE:
			continue
		if node.tagName == "char":
			char = node
			continue
		writer.write(char, node)
	writer.close()

