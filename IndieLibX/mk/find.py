import os, sys, re

def find(filePath):
	input = open(filePath)
	text=input.read()
	input.close()

	v=re.findall(r"\bstr\w+", text)
	if len(v)>0:
		print filePath+':'
	c=0
	for i in v:
		if i in ['string','struct','str1','str2','stream','strLastSlash','straight','strafe','stringdata','stringLength','stringBuffer','streamedVals','stripped','stride','strIndex','strafing','strides','strafes','strictly','stripPath','stretching','strength','strFileName','strideSize','strafeMove','strTexName','strPathName','strips','strMsg','strFile','strErr']:
			continue
		if i.startswith('struct'):
			continue
		if i.startswith('strLastSlash'):
			continue
		c=c+1
		if c==0:
			continue
		print i
	
def myfun(dummy, dirr, filess):
	#print dummy, dirr, filess
	for child in filess:
	    #print child
	    
	    x='''
	    if '.py' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    find(dirr+'/'+child)
	    	    #print child
	    
	    if '.sh' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    find(dirr+'/'+child)
	    	    #print child
	    
	    if '.bat' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    find(dirr+'/'+child)
	    	    #print child
	    '''
	    
	    if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    find(dirr+'/'+child)
	    	    #print child
	    if '.inl' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    find(dirr+'/'+child)
	    	    #print child
	    if '.hxx' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    find(dirr+'/'+child)
		    #print child
	    if '.hpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    find(dirr+'/'+child)
		    #print child
	    
	    if '.c' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    find(dirr+'/'+child)
		    #print child
	    if '.cxx' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    find(dirr+'/'+child)
		    #print child
	    
	    if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
		    find(dirr+'/'+child)
		    #print child

v=['../Libraries','../Samples']

for i in v:
	os.path.walk(i, myfun, 13)

