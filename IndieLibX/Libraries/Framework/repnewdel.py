import os, sys, re

def replaceStringInFile(filePath):
    print filePath
    "replaces all string by a regex substitution"
    tempName=filePath+'~~~'
    input = open(filePath)
    outtext=input.read()
    input.close()

    #outtext=re.sub(r'\"\.\.\/Framework\/', '\"../Framework/Framework/', outtext)
    outtext=re.sub(r'\bVArray\b', 'VArray_', outtext)

    outtext2='''#endif\n'''
    output = open(tempName,'w')
    #output.write(outtext1)
    output.write(outtext)
    #output.write(outtext2)
    output.close()
    os.unlink(filePath)
    os.rename(tempName,filePath)

def myfun(dummy, dirr, filess):
	for child in filess:
		if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInFile(dirr+'/'+child)
		if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInFile(dirr+'/'+child)
			#print child

files=['./Framework']
for i in files:
	os.path.walk(i, myfun, 13)

