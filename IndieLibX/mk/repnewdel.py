import os, sys, re

def replaceStringInFile(filePath):
	"replaces all string by a regex substitution"
	tempName=filePath+'~~~'
	print filePath
	input = open(filePath)
	outtext=input.read()
	input.close()
	
	output = open(tempName,'w')

	#outtext=re.sub(r'\bstrcpy \(mText\b','stx_strlcpy (mText', outtext)
	#outtext=re.sub(r'\bstrcpy \(mFpsString\b','stx_strlcpy (mFpsString', outtext)

	#outtext=re.sub(r'\bstrcat \(mText\b','stx_strlcat (mText', outtext)
	#outtext=re.sub(r'\bstrcat \(mFpsString\b','stx_strlcat (mFpsString', outtext)

	#outtext=re.sub(r'\bTexture\b','TextureOTL', outtext)
	#outtext=re.sub(r'\bShader\b','ShaderOTL', outtext)

	#outtext=re.sub(r'\bSDL_2_0_3_BUILD\b','SDL_2_0_4_BUILD', outtext)

	#outtext=re.sub(r'\bLOG_PRINT(\".*?\",__FILE__,__FUNCTION__,__LINE__);','', outtext)

	outtext=re.sub(r'\bINT32\b','INT32_JPEG_', outtext)

	output.write(outtext)
	output.close()
	
	os.unlink(filePath)
	os.rename(tempName,filePath)

def myfun2(dummy, dirr, filess):
	#print dummy, dirr, filess
	for child in filess:
	    print child
	    
	    if '.py' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    replaceStringInFile(dirr+'/'+child)
	    	    print child
	
def myfun(dummy, dirr, filess):
	#print dummy, dirr, filess
	for child in filess:
	    print child
	    
	    if '.py' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    replaceStringInFile(dirr+'/'+child)
	    	    print child
	    
	    if '.sh' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    replaceStringInFile(dirr+'/'+child)
	    	    print child
	    
	    if '.bat' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    replaceStringInFile(dirr+'/'+child)
	    	    print child

	    
	    if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    replaceStringInFile(dirr+'/'+child)
	    	    print child
	    if '.inl' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    replaceStringInFile(dirr+'/'+child)
	    	    print child
	    if '.hxx' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    replaceStringInFile(dirr+'/'+child)
		    print child
	    if '.hpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    replaceStringInFile(dirr+'/'+child)
		    print child
	    
	    if '.c' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    replaceStringInFile(dirr+'/'+child)
		    print child
	    if '.cxx' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    replaceStringInFile(dirr+'/'+child)
		    print child
	    
	    if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    replaceStringInFile(dirr+'/'+child)
		    print child

#v=['Samples']
#f='../Libraries/STX'

#for i in v:
#os.path.walk(f, myfun, 13)

#v=['RendererGLES.cpp','RendererGLES2.cpp']
#for i in v:
#	replaceStringInFile('../Libraries/Framework3/src/'+i)

f='../Libraries/Image3/jpeg-8c'

os.path.walk(f, myfun, 13)

