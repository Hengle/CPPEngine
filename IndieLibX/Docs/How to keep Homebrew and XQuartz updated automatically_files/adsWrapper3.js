if (!window.ATW3_AdObj){
try {
if (parent.window.ATW3_AdObj)var ATW3_AdObj=parent.window.ATW3_AdObj;
else {
var ATW3_AdObj=new Object();
parent.window.ATW3_AdObj=ATW3_AdObj;
}}
catch (e){
var ATW3_AdObj=new Object();
}}
 var atw3Ping=function(f,s,t){
	var w=window;
	var fail=function(){
		if (w.removeEventListener){w.removeEventListener("message",atw3Listen,false)}
		else if (w.attachEvent){w.detachEvent("onmessage",atw3Listen)}
		var xx=(/allowedSizes=(.*?);/.test(s))?(RegExp.$1).split(','):'';
		if (xx)s=s.replace(/allowedSizes=.*?;/,"size="+xx[0]+";");
		if (s.indexOf('size=')==-1)s=s.replace(/\/0\/-1\//, "\/0\/-1\/size=300x250;");
		s=s.replace(/noperf=1;/,"noperf=1;artexc=art_exp;");
		document.write("<script type='text/javascript' src='"+s+"'></script>");
  	};
	var r=0;
	var atw3Listen=function(e){
		if (!r){
			if (!e){
				fail();
			}
			else if (e.source===f.contentWindow){
				if (e.data==="fifgood") {
					f.contentWindow.postMessage(s,e.origin);
				}
				else 
				{
					fail();
				}
				r=1;
			}
		}
	}
	if (w.addEventListener){w.addEventListener("message",atw3Listen,false)}
	else if (w.attachEvent){w.attachEvent("onmessage",atw3Listen)}
	setTimeout(function(){setTimeout(atw3Listen,t);},1);
}
var atw3Resize=function(){
	var atw3GetSize=function(w,d){
		var f,fi,wi='0',h='0',z=0,c=0,a=0,aD1='',col;
		try {
			f=w.frameElement;
			fi=d.getElementById('atwFilelessI');
			if (fi)f=fi;
			aD1=f.contentDocument.documentElement.innerHTML;
		}
		catch (e){return '0x0';}
		col=((aD1.indexOf('AOL - HTML - Blank HTML Ad')!=-1)||(aD1.indexOf('ATCollapse.gif')!=-1));
		if (!col){
			if (/ACE_AR(.*?)possible_size(.*?)[,}]/i.test(aD1))a=1
			else	if (/ACE_AR(.*?)Size(.*?)['"](.*?)['"]/i.test(aD1)){
				if (unescape(RegExp.$3).indexOf(',')>0)a=1;
			}
			if (!a){
				if ((/aolSize=["']([\d]*?)\|([\d]*)["']/i.test(aD1))&&(unescape(RegExp.$2)>1)){
					wi=unescape(RegExp.$1);
					h=unescape(RegExp.$2);
				}
				else if (/ACE_AR(.*?)Size(.*?)[,}]/i.test(aD1)){
					var as=unescape(RegExp.$2).replace(/[^\d\+]/g,"");
					wi=parseInt(as.substring(0,3),10);
					h=parseInt(as.substring(3,as.length),10);
	 			}
				else {
					wi=f.contentDocument.body.scrollWidth;
					h=f.contentDocument.body.scrollHeight;
					if (f.atwSizes){
						var s=f.atwSizes.split(','),l=s.length,lD=100,ma=0,di;
						for (var i=0;i<l;i++){
							x=s[i].split('x');
							di=Math.abs(x[0]-wi)+Math.abs(x[1]-h);
							if (di<lD){
								lD=di;
								ma=i;
							}
						}
						var zz=s[ma].split('x');
						wi=zz[0];
						h=zz[1];
					}
				}
			}
		}
		else {
			wi='collapse';
		}
		return (wi+'x'+h);
	}
	var sz=atw3GetSize(self,self.document);
	if (sz!='0x0'){
		var	wh=sz.split('x'),
			wi=wh[0],
			h=wh[1],
			prev=self,
			cur=parent,
			atTop=false,
			c=0;
		if (wi=='collapse'){
			wi=0;
			h=0;
		}
		try {
			f=self.frameElement;
			var fi=self.document.getElementById('atwFilelessI');
			if (fi)f=fi;
			f.width=wi;
			f.height=h;
			f.style.width=wi+'px';
			f.style.height=h+'px';
		}
		catch(e){}
		while (!atTop&&c<5){
			try {
				atTop=(cur==top);
				var f=cur.document.getElementsByTagName('iframe'),l=f.length;
				for (var i=0;i<l;i++){
					if (f[i].contentWindow==prev&&(f[i].width<=wi||f[i].height<=h||wi==0)){
						f[i].width=wi;
						f[i].height=h;
						f[i].style.width=wi+'px';
						f[i].style.height=h+'px';
					}
				}
				prev=cur;
				cur=cur.parent;
				c++;
			}
			catch(e){return 0;}
		}
	}
}
function html3AdWH(m,w,h){
var u='',pg='',d=document,s,wi=window,pr=location.protocol,sec=(pr=='https:')?1:0,ugc='',dt=new Date(),ifV=2;
if (wi.atwMOAT=='1'&&!ATW3_AdObj.moat){
	ATW3_AdObj.moat=1;
	var sc=d.createElement("script"),hd=d.getElementsByTagName("head")[0],sr;
	sr='http://o.aolcdn.com/os/moat/prod/moatuac.js';
	sc.src=sr;
	hd.appendChild(sc);
}
if (m)m=m.toString()
else return 0;
try {
	var ur=wi.top.location.href; 
	if (!ur||ur==='undefined'){
		ifV=2;
	}
	else if (wi.top!==wi.self){
		ifV=1;
	}
	else {
		ifV=0;
	}
}
catch (e){}
if (!pr||pr.indexOf('http')<0)pr='http:';
try {
	u=top.location.href;
	if (!u||u=="undefined"){u=d.referrer}
	else {
		pg=top.location.href.substr(7).toLowerCase();
		pg=pg.replace(/www\./,'');
		pg=pg.replace(/\.com/,'');
		pg=pg.replace(/[?#].*$/,'');
		var l=pg.length;
		if (l>65)l=65;
		pg="kvpg="+escape(pg.substr(0,l))+";";
		pg=pg.replace(/\/;$/,';');
		pg=pg.replace(/\//g,'%2F');
	}
}
catch (e){u=d.referrer}
u=u||""
var uac=u.search(/atw3UAC=/i),n,n1,src;
if (uac>0&&!ATW3_AdObj.uac){
	n=u.substring(uac+8,u.length).replace(/&.*$/,'').split(/\||;/);
	if (n[1]&&n[1]=='c')src='http://cdn.atwola.com/_media/uac/'
	else src='http://browsertest.web.aol.com/ads/';
	n1=n[0];
	if(/^[0-9A-Za-z/.]+$/.test(unescape(n1)))d.write('<script type="text/javascript" src="'+src+n1+'"></scr','ipt>')
	ATW3_AdObj.uac=1;
}
else{
var ipU=u.search(/atw3IP=/i),ip='',mn,crU=u.search(/atw3CrPr=/i),cr,cr1='',exU=u.search(/atw3Exc=/i),exc='',ln=u.length;
if (ipU>0)ip='ip='+u.substring(ipU+7,ln).replace(/&.*$/,'')+';';
if (!(/^[a-z0-9\.=;]+$/.test(unescape(ip))))ip='';
if (exU>0)exc=u.substring(exU+8,ln).replace(/&.*$/,'');
var ht='',nt='5113.1',pl='221794',ot='',imgOnly='artexc=art_flash,art_rrflash;',
tp='J',nv=navigator,ua=nv.userAgent.toLowerCase(),ie=(nv.appName=="Microsoft Internet Explorer"),dyn='',inc='',chn='',
dev=(typeof wi.onorientationchange!='undefined')?'1':'2';
if ((ua.indexOf('mobile')>-1)||(/android|iphone|ipad|playbook|hp-tablet|kindle|silk|webos|blackberry|opera mini/i).test(navigator.appVersion))dev='1';
if (!dev)dev='2';
if (wi.s_265&&wi.s_265.channel)chn='kvoch='+escape(wi.s_265.channel)+';'
if (!wi.ATW3_AdObj.scr){
	ATW3_AdObj.scr=dt.getTime()%0x3b9aca00;
	ATW3_AdObj.tile=1;
}
else ATW3_AdObj.tile++;
var scr=ATW3_AdObj.scr,tile=ATW3_AdObj.tile;
mn=(/(\?|&)atw3[Mm][Nn]=(.*?)(&|$)/.test(u))?(RegExp.$2).split(/\||;/):'';
if (!(/^[0-9A-Za-z,-]+$/.test(unescape(mn))))mn='';
if (mn){
	var mL=mn.length,wxh=w+'x'+h,num,all=0;
	for (var i=0;i<mL;i=i+2){
		num=mn[i+1];
		if (num.indexOf('a')>0){
			num=num.replace('a','');
			all=1;
		}
		else {
			all=0;
		}
		if (num.indexOf('only')>-1){
			num=num.replace('only','');
			only=1;
		} 
		else {
			only=0; 
		}
		if ((ATW3_AdObj.tile==num)||(wxh==num)){
			m=mn[i];
			if (!all)mn[i+1]='';
			if (only)mn[i+1]='only';
		}
		else if (only){m='0';}
	}
}
if (m=='0'){ATW3_AdObj.tile++;return 0}
if (wi.atwHtNmAT){
	v=atwHtNmAT;
	ht=(/^https?/i.test(v))?v:((/^\/\//.test(v))?pr+v:pr+'//'+v);
	if (ht.charAt(ht.length-1)=='/')ht=ht.substring(0,ht.length-1);
}
if (wi.atwType==''){tp='J'}else if (wi.atwType){tp=atwType.toUpperCase();}
kv=(/(\?|&)atw3[Kk][Vv]=(.*?)(&|$)/.test(u))?escape(RegExp.$2):'';
if (!(/^[0-9A-Za-z,;=]+$/.test(unescape(kv))))kv='';
if (kv&&kv[kv.length-1]!=';')kv+=';'
if (wi.atwOthAT){
	v=atwOthAT;
	if (v.charAt(v.length-1)!=';')v+=';'
	var x=v.split(';'),l=x.length,y;
	for (i=0;i<l-1;i++){
		if (x[i].charAt(x[i].length-1)!='='){
			y=x[i].split('=');
			ot+=escape(y[0])+"="+escape(y[1])+';';
		}
	}
}
else if (ATW3_AdObj&&ATW3_AdObj.adsATOth)ot=ATW3_AdObj.adsATOth;
ot+=kv;
if (ot.indexOf('kvugc')==-1){
	try { 
		if (parent.window.adSetUGC)ugc=parent.window.adSetUGC
		else if (wi.adSetUGC)ugc=wi.adSetUGC;
	}
	catch (e){if (wi.adSetUGC){ugc=wi.adSetUGC;}}
	if (!ugc){
		if (ot.indexOf('cmsid')==-1)ugc='0'
		else ugc='1'
	}
	ot+='kvugc='+ugc+';';
}
if (wi.atwNetId){nt=atwNetId}
var nt1=u.search(/atwNt=/i);
if (nt1>0)nt=u.substring(nt1+6,ln).replace(/&.*$/,'');
if (wi.atwPlId){pl=atwPlId}
if (ua.indexOf('ipad')!=-1)inc=imgOnly
if (exc=='blank')inc='artexc=all;'
if (!ATW3_AdObj.sniff){
	if (!inc&&tp!='IMAGE'){
		var dF='',a,d1;
		if (ie&&(ua.indexOf('win')!=-1)){
 			try {
				a=new ActiveXObject("ShockwaveFlash.ShockwaveFlash");
				if (a){
					d1=a.GetVariable("$version").split(" ")[1].split(",");
					if (d1[0]>=10)dF=d1[0];
				}
			}
			catch(e){}
		}
		else{
			var p=nv.plugins;
			if (p){
				var l=p.length;
				if (l>1){
					var mt=nv.mimeTypes,fl=mt['application/x-shockwave-flash'];
					if (mt&&((fl&&fl.enabledPlugin&&(fl.suffixes.indexOf('swf')!=-1)))){
						var ds,f="Flash ",fS;
						for (var i=0;i<l;i++){
							ds=p[i].description;
							fS=ds.indexOf(f);
							if (fS!=-1){
								if (ds.substring(fS+6,fS+8)>=10){dF=ds.substring(fS+6,fS+8)}
							}
						}
					}
					if (fl==null)dF=''
				}
			}
		}
		if (!dF)inc=imgOnly;
		ATW3_AdObj.inc=inc;
		ATW3_AdObj.sniff=1;
	}
}
else inc=ATW3_AdObj.inc;
if (w=='RR'||w=='rr'){
	dyn='r';
	wi.atwSizes='300x250,300x600,300x1050';
	w='300';
	h='250';
}
else if (w=='LB'||w=='lb'){
	wi.atwSizes='728x90,948x250,970x66,970x90,950x252,970x250,940x230';
	w='728';
	h='90';
}
else if (w=='MM'||w=='mm'){
	wi.atwSizes='300x250,320x480,300x600,320x50';
	w='300';
	h='250';
}
var s1='',s2='';
if (wi.atwSizes){
	if (tp!='FILELESS'&&tp!='IMAGE'&&tp!='AFIF'){
		tp='J';
	}
	if (ifV==2&&tp!='AFIF'){
		s2+="size="+w+"x"+h+";";
	}
	else if (dyn!='r')s2+="allowedSizes="+wi.atwSizes+";";
}
else if (dyn!='r')s2+="size="+w+"x"+h+";";
s1=ot.toLowerCase()+ip+pg+'kvmn='+m+';kv3puac=1;kvgrp='+scr+';kvismob='+dev+';'+chn+'extmirroring=0;kvtile='+tile+';aduho='+(-1*dt.getTimezoneOffset())+';';
s2+='noperf=1;alias='+m+';noaddonpl=y;'+s1+inc+'grp='+scr;
if ((m.indexOf('-')>-1)&&(/^[0-9a-fm\-]+$/i.test(m))){
 if (m.substring(0,1).toLowerCase()=='m')m=m.substring(1,m.length);
 if (ht&&ht!='https://at.atwola.com')s=ht
 else {
  s=pr+'//mads';
  if ((wi.atwCo&&wi.atwCo.toLowerCase()!='us')||(wi.atwNetId&&wi.atwNetId!='5113.1'))s+='uk'
  s+='.at.atwola.com';
 }
 var kf='kvmflash=',swh='',inI=false,inSD=true,pU,sd='';
 if (dF)kf+='true;'
 else kf+='false;';
 if (wi.screen&&wi.screen.width&&wi.screen.height)swh='swh='+wi.screen.width+'x'+wi.screen.height+';screenwidth='+wi.screen.width+';screenheight='+wi.screen.height+';';
 if (wi.devicePixelRatio)sd='screendensity='+wi.devicePixelRatio+';';
 try {
  if (wi.top!==wi.self)inI=true;
  pU=wi.top.location.href.toString();
 } 
 catch (e){}
 if (!pU||pU==="undefined"){
  inI=true;
  inSD=false;
 }
 var f1="f="+(inI?(inSD?"1":"2"):"0")+";",f2="fv="+(dF?dF:"0")+";"; 
 s+='/adcall?mpid='+m+';rettype=js;width='+w+';height='+h+';'   
 s+=s1+kf+swh+sd+f1+f2+'optn=1;grp='+scr+';random='+scr;
}
else {
if (dev=='1'&&tp!='IMAGE'){
 var sm='grp='+scr+';random='+scr+';sizeId=-1;'
 sm+=s2;
 s=pr+'//';
 if (tp=='I'){
  var co='';
  if (wi.atwCo&&wi.atwCo.toLowerCase()!='us')co=wi.atwCo
  sm+='|'+nt+'|'+pl+'|'+co;
  sm=unescape(sm);
  if (sec)s+='s'
  else s+='o';
  s+='.aolcdn.com/ads/mobileIframe.html?s='+escape(sm);
 }
 else {
  s+='mads';
  if ((wi.atwCo&&wi.atwCo.toLowerCase()!='us')||(wi.atwNetId&&wi.atwNetId!='5113.1'))s+='uk'
  s+='.at.atwola.com/adcall?mpid=348-d-d-e;rettype=js;callProtocol=3.0;networkId='+nt+';placementid='+pl+';'+sm;
 }
}
else {
if (ht)s=ht
else {
 s=pr+'//';
 if (wi.atwCo&&wi.atwCo.toLowerCase()!='us')s+='uk.';
 s+='at.atwola.com';
}
s+="/addyn/3.0/"+nt+"/"+pl+"/0/-1/";
s+=s2;
}
if (crU>0){
 cr=u.substring(crU+9,ln).replace(/&.*$/,'').split(/\||;/);
 cr1=cr[0];
 if ((/http[s]{0,1}:\/\/[^\/]*?aol.com(:[0-9]*?){0,1}\//.test(cr1))&&(/^[0-9A-Za-z/.:]+$/.test(unescape(cr1)))){
  if ((dyn=='r'&&(cr[1]==300&&(cr[2]==250||cr[2]==600||cr[2]==1050)))||
      (cr[1]==w&&cr[2]==h)||(cr[3]==tile)){
    if (tp!='J')s=cr1+'.html'
    else s=cr1+'.js'
  }
 }
}
}
if (m!='0'){
if (tp!='IMAGE'){
if (tp=='J'||(tp=='AFIF'&&(!wi.atwFIF||ifV!=2||!wi.postMessage))){
	if (ifV==1&&wi.atwResize&&wi.atwSizes){
		d.write("<div id='atwAdJS'>");
	}
	d.write("<script type='text/javascript' src='"+s+"'></script>");
	if (ifV==1&&wi.atwResize&&wi.atwSizes){
		self.atwSizes=wi.atwSizes;
		var ii=document.getElementById('atwAdJS');
		ii.atwSizes=wi.atwSizes;
		if (wi.addEventListener){wi.frameElement.addEventListener("load",atw3Resize,false)}
		else if (wi.attachEvent){wi.frameElement.attachEvent("onload",atw3Resize)}
		d.write('</div>');
	}
 }
 else if (tp=='FILELESS'){
	var pd='';
	if (wi.atwDiv){
		pd=document.getElementById(atwDiv);
	}
	if (pd){
		var dd=document.createElement('div');
		dd.id='atwFilelessDiv';
		pd.appendChild(dd);
		var f=document.createElement('iframe');
		f.width=w;
		f.height=h;
		f.title='Ad';
		f.marginWidth=0;
		f.marginHeight=0;
		f.setAttribute('allowtransparency','true');
		f.frameBorder=0;
		f.scrolling="no";
		f.id="atwFilelessI";
		dd.appendChild(f);
	}
	else {
		d.write("<div id='atwFilelessDiv'><iframe id='atwFilelessI' width="+w+" height="+h+" title=Ad scrolling=no frameborder=0 marginheight=0 marginwidth=0></iframe></div>");
	}
	var ii=document.getElementById('atwFilelessI');
	ii.atwSizes=wi.atwSizes;
	var i2=(ii.contentWindow)?ii.contentWindow:(ii.contentDocument.document)?ii.contentDocument.document:ii.contentDocument;
	i2.document.open();
	var iStr='<html><head></head><body';
	if (wi.atwResize&&wi.atwSizes){
		iStr+=' onload=parent.atw3Resize();';
	}
	iStr+='><script type="text/javascript">\n';
	iStr+='var inDapIF=true;inFIF=true;\n';
	iStr+='</script>\n';
	iStr+='<script type="text/javascript" src="'+s+'"></script>\n';	
	iStr+='</body></html>';
	i2.document.write(iStr);
	i2.document.close(); 
 }
 else if (tp=='AFIF'&&wi.atwFIF&&ifV==2){
	var af=document.createElement("iframe");
	af.src=wi.atwFIF+'#'+s;
	af.width=0;
	af.height=0; 
	af.style.width="0px";
	af.style.height="0px";
	af.style.display='none';
	document.body.appendChild(af);
	if (wi.postMessage){
		atw3Ping(af,s,200);
	}
}
else{
	s=s.replace(/addyn/,'adiframe')
	d.write("<iframe src='"+s+"' width="+w+" height="+h+" scrolling=no frameborder=0 marginheight=0 marginwidth=0></iframe>")
}
}
else{
	d.write("<a href='"+s.replace(/addyn/,"adlink")+"' target=_blank><img src='"+s.replace(/addyn/,"adserv")+"' width="+w+" height="+h+" border=0 alt='advertisement'></a>")
}
}
atwHtNmAT='';atwType='';atwOthAT='';atwNetId='';atwPlId='',atwSizes='',atwDiv='';atwMN='';atwWidth='';atwHeight='';
ATW3_AdObj.uac=0
}}
(function(w) {
var c=w['mraid']||w['ormma'];
if (c)w['open']=c['open'];
})(window);
if (!window.atwWidth)atwWidth='300';
if (!window.atwHeight)atwHeight='250';
if (window.atwMN)html3AdWH(atwMN,atwWidth,atwHeight);