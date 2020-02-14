/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

#if 1
#define defaultshader eSimpleShader
//define alternativeshader eAmbientLighting
//define alternativeshader eDiffuseLighting
//define alternativeshader ePhongLighting1
#define alternativeshader ePhongLighting2
#else
#define defaultshader eCelShader
#define alternativeshader eCelShader
#endif

float AngleX = 0.0f;
float AngleY = 0.0f;

const char* filename[] =
{
#if 0
"/PMilitaryPRO/P-TankHistory/german/e_series/e10/e10.3DS",
"/PMilitaryPRO/P-TankHistory/german/panther/color1/PANTHER.3DS",
"/PMilitaryPRO/P-TankHistory/german/jpanther/JPANTHER.3DS",
"/PMilitaryPRO/P-TankHistory/german/a7v/A7V.3DS",
"/PMilitaryPRO/P-TankHistory/allied/m4a4/M4A4.3DS",
"/PMilitaryPRO/P-TankHistory/allied/austin/austin.3DS",
"/PMilitaryPRO/P-TankHistory/allied/ford/FORD1918.3DS",
"/PMilitaryPRO/P-TankHistory/allied/m5a1/early/M5A1E.3DS",
"/PMilitaryPRO/P-TankHistory/allied/t34_76/T34-76.3DS",
"/PMilitaryPRO/P-TankHistory/allied/kv2/kv2.3DS",
"/PMilitaryPRO/P-TankHistory/allied/Renault/renaultFT18.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb2.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Bf109g/Models/bf109g.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Mc202/Models/MC202.3DS",
"/PMilitaryPRO/Vehicles/Ch_low.3DS",
"/PMilitaryPRO/Aircraft/sabre.3DS",
"/PMilitaryPRO/P-TankHistory/german/tiger1/first/tiger1f.3ds",
#else
"/PMilitaryPRO/P-TankHistory/german/tiger1/early/t1eobj/tiger1e.obj",
"/PMilitaryPRO/P-TankHistory/kha_go/kg3ds/kha-go.3ds",
"/PMilitaryPRO/P-TankHistory/german/e_series/e25/e253ds/E25.3ds",
"/PMilitaryPRO/P-TankHistory/german/TigerWittmann/Tiger1.3ds",
"/PMilitaryPRO/P-TankHistory/german/panther/color2/pt13ds/Panter.3ds",
"/PMilitaryPRO/P-TankHistory/german/sdkfz251/2513ds/sdkfz251.3ds",
"/PMilitaryPRO/P-TankHistory/german/sdkfz250/2503ds/sdkfz250.3ds",
"/PMilitaryPRO/P-TankHistory/german/stiger/st3ds/stiger.3ds",
"/PMilitaryPRO/P-TankHistory/german/tiger1/first/3ds/tiger1f.3ds",
"/PMilitaryPRO/P-TankHistory/german/pz38/p383ds/pz38g.3ds",
"/PMilitaryPRO/P-TankHistory/german/pz3/bef_wg/3ds/pzbefwg.3ds",
"/PMilitaryPRO/P-TankHistory/german/pz3/ausf_l/p33ds/pz3l.3ds",
"/PMilitaryPRO/P-TankHistory/german/pz3/ausf_e/3ds/pz3e.3ds",
"/PMilitaryPRO/P-TankHistory/german/jtiger/jt3ds/Jtiger.3ds",
"/PMilitaryPRO/P-TankHistory/german/sdkfz222/2223ds/sdkfz222.3ds",
"/PMilitaryPRO/P-TankHistory/german/tiger2/porsche/t23ds/tiger2p.3ds",
"/PMilitaryPRO/P-TankHistory/german/tiger2/henschel/3ds/tiger2h.3ds",
"/PMilitaryPRO/P-TankHistory/german/pz1/p13ds/pz1a.3ds",
"/PMilitaryPRO/P-TankHistory/german/pz4/p43ds/pz4d.3ds",
"/PMilitaryPRO/P-TankHistory/allied/mk1/MkI.3ds",
"/PMilitaryPRO/P-TankHistory/allied/firefly/ff3ds/firefly.3ds",
"/PMilitaryPRO/P-TankHistory/allied/bz35/3ds/bz35.3ds",
"/PMilitaryPRO/P-TankHistory/allied/m5a1/late/m53ds/M5a1l.3ds",
"/PMilitaryPRO/P-TankHistory/allied/is-4/is-4.3ds",
"/PMilitaryPRO/P-TankHistory/german/e_series/e10/e103ds/e10.3DS",
"/PMilitaryPRO/P-TankHistory/german/panther/color1/pt33ds/PANTHER.3DS",
"/PMilitaryPRO/P-TankHistory/german/jpanther/jp3ds/JPANTHER.3DS",
"/PMilitaryPRO/P-TankHistory/german/a7v/a7v3ds/A7V.3DS",
"/PMilitaryPRO/P-TankHistory/allied/m4a4/m43ds/M4A4.3DS",
"/PMilitaryPRO/P-TankHistory/allied/austin/austin.3DS",
"/PMilitaryPRO/P-TankHistory/allied/ford/ford3ds/FORD1918.3DS",
"/PMilitaryPRO/P-TankHistory/allied/m5a1/early/3ds/M5A1E.3DS",
"/PMilitaryPRO/P-TankHistory/allied/t34_76/T34-76.3DS",
"/PMilitaryPRO/P-TankHistory/allied/kv2/kv2/kv2.3DS",
"/PMilitaryPRO/P-TankHistory/allied/Renault/renault/renaultFT18.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb.obj",
"/PMilitaryPRO/P-WarplaneHistory/Fw190A8/Fw190A8/Models/3ds4/Fw190A8.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/cr.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/sl1600.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb3c.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb3d.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/sl5000.3ds",
"/PMilitaryPRO/P-WarplaneHistory/p-51/Models/P-51D.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Fw190D/Models/Fw190D.3ds",
"/PMilitaryPRO/P-WarplaneHistory/Bf109F/Models/Bf109f4.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Spit_Vb/Spitfire_Vb/Models/3ds4/spit_Vb2.3DS",
"/PMilitaryPRO/P-WarplaneHistory/fokker/fokker/fokker.3DS",
"/PMilitaryPRO/P-WarplaneHistory/mig15/mig15/mig15.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Bf109g/Models/bf109g.3DS",
"/PMilitaryPRO/P-WarplaneHistory/Mc202/Models/MC202.3DS",
"/PMilitaryPRO/Aircraft/ju87.obj",
"/PMilitaryPRO/Aircraft/b17g.3ds",
"/PMilitaryPRO/Aircraft/F15.3ds",
"/PMilitaryPRO/Aircraft/b17g_lp.3ds",
"/PMilitaryPRO/Aircraft/harrier.3ds",
"/PMilitaryPRO/Aircraft/a10.3ds",
"/PMilitaryPRO/Aircraft/BF109E.3ds",
"/PMilitaryPRO/Aircraft/p51.3ds",
"/PMilitaryPRO/Aircraft/F16.3ds",
"/PMilitaryPRO/Aircraft/eurocopter-sa342.3ds",
"/PMilitaryPRO/Aircraft/ka58/3ds/ka58.3ds",
"/PMilitaryPRO/Aircraft/p51_lp.3ds",
"/PMilitaryPRO/Aircraft/F14.3ds",
"/PMilitaryPRO/Aircraft/262_lp.3ds",
"/PMilitaryPRO/Aircraft/Helic_afnew.3ds",
"/PMilitaryPRO/Aircraft/Helic_af.3ds",
"/PMilitaryPRO/Aircraft/blackhawk.3ds",
"/PMilitaryPRO/Aircraft/apache-lo.3ds",
"/PMilitaryPRO/Aircraft/dauntless.3ds",
"/PMilitaryPRO/Aircraft/apache.3ds",
"/PMilitaryPRO/Aircraft/zeroA6M2.3ds",
"/PMilitaryPRO/Aircraft/AH64.3ds",
"/PMilitaryPRO/Aircraft/stukaju87.3ds",
"/PMilitaryPRO/Aircraft/F18.3ds",
"/PMilitaryPRO/Aircraft/wildcat.3ds",
"/PMilitaryPRO/Aircraft/262.3ds",
"/PMilitaryPRO/Aircraft/mirage.3ds",
"/PMilitaryPRO/Aircraft/a10-2.3ds",
"/PMilitaryPRO/Aircraft/dc6.3ds",
"/PMilitaryPRO/Aircraft/sabre.3DS",
"/PMilitaryPRO/Vehicles/GMTKFAS.3ds",
"/PMilitaryPRO/Vehicles/merkava4.3ds",
"/PMilitaryPRO/Vehicles/GMTR3FAL.3ds",
"/PMilitaryPRO/Vehicles/GMTR430L.3ds",
"/PMilitaryPRO/Vehicles/pt76.3ds",
"/PMilitaryPRO/Vehicles/GP15L.3ds",
"/PMilitaryPRO/Vehicles/FURR.3ds",
"/PMilitaryPRO/Vehicles/VFK.3ds",
"/PMilitaryPRO/Vehicles/zsu34shilka.3ds",
"/PMilitaryPRO/Vehicles/TurmFAL.3ds",
"/PMilitaryPRO/Vehicles/VF.3ds",
"/PMilitaryPRO/Vehicles/LAGsk.3ds",
"/PMilitaryPRO/Vehicles/challenger2.3ds",
"/PMilitaryPRO/Vehicles/sherman.3ds",
"/PMilitaryPRO/Vehicles/t90.3ds",
"/PMilitaryPRO/Vehicles/KP.3ds",
"/PMilitaryPRO/Vehicles/hummer.3ds",
"/PMilitaryPRO/Vehicles/Arm-car.3ds",
"/PMilitaryPRO/Vehicles/SC.3ds",
"/PMilitaryPRO/Vehicles/LAG.3ds",
"/PMilitaryPRO/Vehicles/CRA3.3ds",
"/PMilitaryPRO/Vehicles/bradlo.3ds",
"/PMilitaryPRO/Vehicles/GART.3ds",
"/PMilitaryPRO/Vehicles/ARS.3ds",
"/PMilitaryPRO/Vehicles/GKFR.3ds",
"/PMilitaryPRO/Vehicles/GKKFK.3ds",
"/PMilitaryPRO/Vehicles/bradley.3ds",
"/PMilitaryPRO/Vehicles/GPT.3ds",
"/PMilitaryPRO/Vehicles/Turm30S.3ds",
"/PMilitaryPRO/Vehicles/LuchsTank.3ds",
"/PMilitaryPRO/Vehicles/CST.3ds",
"/PMilitaryPRO/Vehicles/Turm30L.3ds",
"/PMilitaryPRO/Vehicles/GMTR330L.3ds",
"/PMilitaryPRO/Vehicles/shilka.3ds",
"/PMilitaryPRO/Vehicles/GMTR360L.3ds",
"/PMilitaryPRO/Vehicles/leopard1.3ds",
"/PMilitaryPRO/Vehicles/GWF.3ds",
"/PMilitaryPRO/Vehicles/GKFK.3ds",
"/PMilitaryPRO/Vehicles/SLAP.3ds",
"/PMilitaryPRO/Vehicles/CSS.3ds",
"/PMilitaryPRO/Vehicles/TurmFUS.3ds",
"/PMilitaryPRO/Vehicles/GDSF.3ds",
"/PMilitaryPRO/Vehicles/Tank3.3ds",
"/PMilitaryPRO/Vehicles/btr60.3ds",
"/PMilitaryPRO/Vehicles/GAR3.3ds",
"/PMilitaryPRO/Vehicles/Turm60L.3ds",
"/PMilitaryPRO/Vehicles/m198.3ds",
"/PMilitaryPRO/Vehicles/GMTK60S.3ds",
"/PMilitaryPRO/Vehicles/t72-2.3ds",
"/PMilitaryPRO/Vehicles/cscout.3ds",
"/PMilitaryPRO/Vehicles/CRT2.3ds",
"/PMilitaryPRO/Vehicles/SAG.3ds",
"/PMilitaryPRO/Vehicles/JPlow.3ds",
"/PMilitaryPRO/Vehicles/GP30L.3ds",
"/PMilitaryPRO/Vehicles/Apc.3ds",
"/PMilitaryPRO/Vehicles/GLTK.3ds",
"/PMilitaryPRO/Vehicles/Tank4.3ds",
"/PMilitaryPRO/Vehicles/UN.3ds",
"/PMilitaryPRO/Vehicles/GHTsk.3ds",
"/PMilitaryPRO/Vehicles/Turm15L.3ds",
"/PMilitaryPRO/Vehicles/GMTKFUS.3ds",
"/PMilitaryPRO/Vehicles/JP.3ds",
"/PMilitaryPRO/Vehicles/BP.3ds",
"/PMilitaryPRO/Vehicles/BLP.3ds",
"/PMilitaryPRO/Vehicles/PST.3ds",
"/PMilitaryPRO/Vehicles/Turm15K.3ds",
"/PMilitaryPRO/Vehicles/GLTR3.3ds",
"/PMilitaryPRO/Vehicles/GHHT.3ds",
"/PMilitaryPRO/Vehicles/CRT2L.3ds",
"/PMilitaryPRO/Vehicles/KMT.3ds",
"/PMilitaryPRO/Vehicles/GP60L.3ds",
"/PMilitaryPRO/Vehicles/Turm60S.3ds",
"/PMilitaryPRO/Vehicles/CRA2.3ds",
"/PMilitaryPRO/Vehicles/zsu-2.3ds",
"/PMilitaryPRO/Vehicles/PTK.3ds",
"/PMilitaryPRO/Vehicles/LKP.3ds",
"/PMilitaryPRO/Vehicles/GMTR415L.3ds",
"/PMilitaryPRO/Vehicles/Tiger.3ds",
"/PMilitaryPRO/Vehicles/uazHP.3ds",
"/PMilitaryPRO/Vehicles/TurmFAS.3ds",
"/PMilitaryPRO/Vehicles/SKP.3ds",
"/PMilitaryPRO/Vehicles/GMTK30S.3ds",
"/PMilitaryPRO/Vehicles/CRT3L.3ds",
"/PMilitaryPRO/Vehicles/merkava1.3ds",
"/PMilitaryPRO/Vehicles/GLTR4.3ds",
"/PMilitaryPRO/Vehicles/CRT3.3ds",
"/PMilitaryPRO/Vehicles/t72.3ds",
"/PMilitaryPRO/Vehicles/t54.3ds",
"/PMilitaryPRO/Vehicles/GMTR460L.3ds",
"/PMilitaryPRO/Vehicles/leopard2.3ds",
"/PMilitaryPRO/Vehicles/lohum.3ds",
"/PMilitaryPRO/Vehicles/abrams.3ds",
"/PMilitaryPRO/Vehicles/GMTR315L.3ds",
"/PMilitaryPRO/Vehicles/GWF2.3ds",
"/PMilitaryPRO/Vehicles/leclerc.3ds",
"/PMilitaryPRO/Vehicles/GKKFR.3ds",
"/PMilitaryPRO/Vehicles/VFT.3ds",
"/PMilitaryPRO/Vehicles/GMTR4FAL.3ds",
"/PMilitaryPRO/Vehicles/GAR2.3ds",
"/PMilitaryPRO/Vehicles/GHT.3ds",
"/PMilitaryPRO/Vehicles/TurmFUSS.3ds",
"/PMilitaryPRO/Vehicles/PTL.3ds",
"/PMilitaryPRO/Vehicles/j-t-90.3ds",
"/PMilitaryPRO/Vehicles/Ch_low.3DS",
"/PMilitaryPRO/Warships/corvette.3ds",
"/PMilitaryPRO/Warships/carrier.3ds",
"/PMilitaryPRO/Warships/frigate2.3ds",
"/PMilitaryPRO/Warships/viking.3ds",
"/PMilitaryPRO/Warships/trireme.3ds",
"/PMilitaryPRO/Warships/vikingShip.3ds",
"/PMilitaryPRO/Warships/corvette+.3ds",
"/PMilitaryPRO/Warships/destroyer.3ds",
"/PMilitaryPRO/Warships/triere.3ds",
"/PMilitaryPRO/Warships/frigate1.3ds",
"/PMilitaryPRO/Warships/frigate2+.3ds",
#endif
};
#if 0
tShader shaders[] =
{
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
};

float angles[] =
{
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f,
0.0f, 0.0f
};
#endif
class _3DAnimation
#if 0
 : public STXGUI 
#endif
{
public:
	_3DAnimation()
	{
		m_i=0;
		shdname = defaultshader;
	}
	virtual ~_3DAnimation(){}
	D3DXFROMWINEVECTOR3 T, R, S;
	D3DXFROMWINEMATRIX Tm, Rm, Sm;
	unsigned int m_i;
	std::string shdname;
	MeshRenderer2 m_Mesh[elementsOf(filename)];
#if 0
	virtual void actionPerformed(GUIEvent &evt)
{
  const NSString &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int widgetType      = sourceRectangle->getWidgetType();

  if(widgetType == RADIO_BUTTON)
  {
	GUIRadioButton   *radioButton = (GUIRadioButton*)sourceRectangle;
	if(radioButton->isPressed() && radioButton->isChecked())
	{
		//LOG_PRINT("callbackString=%s\n", callbackString.data);
		if(callbackString == "e10") 
		{
			m_i=0;
		}
		else if(callbackString == "PANTHER") 
		{
			m_i=1;
		}
		else if(callbackString == "JPANTHER") 
		{
			m_i=2;
		}
		else if(callbackString == "A7V") 
		{
			m_i=3;
		}
		else if(callbackString == "M4A4") 
		{
			m_i=4;
		}
		else if(callbackString == "austin") 
		{
			m_i=5;
		}
		else if(callbackString == "FORD1918") 
		{
			m_i=6;
		}
		else if(callbackString == "M5A1E") 
		{
			m_i=7;
		}
		else if(callbackString == "T34-76") 
		{
			m_i=8;
		}
		else if(callbackString == "kv2") 
		{
			m_i=9;
		}
		else if(callbackString == "renaultFT18") 
		{
			m_i=10;
		}
		else if(callbackString == "spit_Vb") 
		{
			m_i=11;
		}
		else if(callbackString == "spit_Vb2") 
		{
			m_i=12;
		}
		else if(callbackString == "bf109g") 
		{
			m_i=13;
		}
		else if(callbackString == "MC202") 
		{
			m_i=14;
		}
		else if(callbackString == "Ch_low") 
		{
			m_i=15;
		}
		else if(callbackString == "sabre") 
		{
			m_i=16;
		}
		else if(callbackString == "tiger1f") 
		{
			m_i=17;
		}
	}
	}
}
#endif
void LuaRead()
{
#if 0
		LuaScript script;
		script.Load("/Orcs/Bonfire_wOrcs.lua");
		T.x = script.get<float>("x");
		T.y = script.get<float>("y");
		T.z = script.get<float>("z");
		S.x = script.get<float>("sx");
		S.y = script.get<float>("sy");
		S.z = script.get<float>("sz");
		R.x = script.get<float>("rx");
		R.y = script.get<float>("ry");
		R.z = script.get<float>("rz");
#else
	std::string fn=stx_convertpath("/Orcs/Bonfire_wOrcs.xml");
	LOG_PRINT("fn.c_str()=%s\n", fn.c_str());

	TiXmlDocument document(fn.c_str());
	document.LoadFile();
	TiXmlHandle docHandle( &document );
	TiXmlHandle BodyHandle = docHandle.FirstChild("Body");
	TiXmlElement* BodyElement = BodyHandle.Element();
	if(BodyElement) {LOG_PRINT("!BodyElement\n");stx_exit(0);}

	TiXmlElement* CameraElement = BodyHandle.FirstChild("Camera").Element();
		T.x=atof(CameraElement->Attribute("x"));
		T.y=atof(CameraElement->Attribute("y"));
		T.z=atof(CameraElement->Attribute("z"));
		S.x=atof(CameraElement->Attribute("sx"));
		S.y=atof(CameraElement->Attribute("sy"));
		S.z=atof(CameraElement->Attribute("sz"));
		R.x=atof(CameraElement->Attribute("rx"));
		R.y=atof(CameraElement->Attribute("ry"));
		R.z=atof(CameraElement->Attribute("rz"));
#endif

		D3DXFROMWINEMatrixTranslation(&Tm, T.x, T.y, T.z); 
		D3DXFROMWINEMatrixScaling(&Sm, S.x, S.y, S.z);
		D3DXFROMWINEMatrixRotationYawPitchRoll(&Rm, R.x, R.y, R.z); 

}
	virtual void init(const char* aTitle="")
	{
#if 0			
		STXGUI::init("/3DAnimation/GUILayout2.xml");
#endif
		m_i=0;
		//LuaRead();
		for(unsigned int i=0;i<elementsOf(filename);i++)
		{
			printf("3DAnimation2[%d]=%s\n", i, filename[i]);
#if 0
			FW3::GetInstance()->GetMesh(m_Mesh[i], filename[i], shaders[i]);
#else
#if 0
			if(0)//((i==13) && (i==15))
				m_Mesh[i].Load(filename[i], shaders[i], false, true);
			else
				m_Mesh[i].Load(filename[i], shaders[i], true, true);
#else
			m_Mesh[i].Load(filename[i], eSimpleShader, true, true);
#endif
#endif
			if(1)
			{
				m_Mesh[i].SetPlay(false);
				m_Mesh[i].SetAnimIndex(0);
				m_Mesh[i].SetPlay(true);
#if 0
				if(shaders[i]!=eSimpleShader)
				{
					D3DXFROMWINEVECTOR4 a(0.1f, 0.0f, 0.0f, 1.0f);
					D3DXFROMWINEVECTOR4 d(1.0f, 0.0f, 0.0f, 1.0f);
					D3DXFROMWINEVECTOR4 l(0.0f, 1.0f, -1.0f, 1.0f);
					D3DXFROMWINEVECTOR4 e(0.0f, 0.0f, -1.0f, 1.0f);
					D3DXFROMWINEVECTOR4 gold=stx_GetRGBA(eGold);
					D3DXFROMWINEVECTOR4 silver=stx_GetRGBA(eSilver);
					m_Mesh[i].SetAmbientColor(silver*0.1f);
					m_Mesh[i].SetDiffuseColor(silver);
					m_Mesh[i].SetLightDir(l);
					m_Mesh[i].SetEyeDir(e);
				}
#endif
			}
		}
		//m_Mesh[elementsOf(filename)-1]=m_Mesh[elementsOf(filename)-2];
	}
	virtual void render(void)
	{
		IRenderer::GetRendererInstance()->Clear(true,true, D3DXFROMWINEVECTOR4(0.35f, 0.53f, 0.7, 1.0f));
		IRenderer::GetRendererInstance()->BeginScene();

	float mAngleX=0.0f;
	float mAngleY=0.0f;

		D3DXFROMWINEMATRIX mvp;
		if(0)//(m_i==16)
		{
	stx_GetAngles(mAngleX, mAngleY, 50.0f);
{
    D3DXFROMWINEMATRIX mView, mProj, mWorld, R, S, T;
    float aspectRatio = 1.0f*IRenderer::GetRendererInstance()->GetViewportWidth()/IRenderer::GetRendererInstance()->GetViewportHeight();
    //g_Camera.SetProjParams(D3DX_PI / 3, aspectRatio, 1.0f, 1000.0f);
	//D3DXFROMWINEMatrixPerspectiveLH(&mProj, IRenderer::GetRendererInstance()->GetViewportWidth(), IRenderer::GetRendererInstance()->GetViewportHeight(), 1.0f, 1000.0f);
	D3DXFROMWINEMatrixPerspectiveFovLH(&mProj, D3DXFROMWINE_PI / 3, aspectRatio, 1.0f, 1000.0f);

    // Setup camera
    D3DXFROMWINEVECTOR3 vEye3(0.0f, 0.1f, -15.0f);
    D3DXFROMWINEVECTOR3 vAt(0.0f, 0.1f, 0.0f);
	D3DXFROMWINEVECTOR3 vup(0.0f, 1.0f, 0.0f);
	D3DXFROMWINEMatrixLookAtLH(&mView, &vEye3, &vAt, &vup);

#if 0
    AngleX = mAngleX+angles[2*m_i+0];
    AngleY = mAngleY+angles[2*m_i+1];
#else
    AngleX = mAngleX;
    AngleY = mAngleY;
#endif
    float s=1.0f;
    D3DXFROMWINEVECTOR3 t(0.0f, 0.0f, -7.5f);
#if 0
#if 0
    LuaScript script;
    script.Load("/Orcs/Bonfire_wOrcs.lua");
    AngleX = script.get<float>("Orcs.aX");
    AngleY = script.get<float>("Orcs.aY");
    s = script.get<float>("Orcs.S");
    t.x = script.get<float>("Orcs.Tx");
    t.y = script.get<float>("Orcs.Ty");
    t.z = script.get<float>("Orcs.Tz");
#else
	TiXmlDocument* document;
	TiXmlHandle* docHandle;
	TiXmlHandle* BodyHandle;
	std::string fn=stx_convertpath("/Orcs/Bonfire_wOrcs.xml");
	LOG_PRINT("fn.c_str()=%s\n", fn.c_str());
	document=new TiXmlDocument(fn.c_str());
	document->LoadFile();
	docHandle=new TiXmlHandle( document );
	BodyHandle = &docHandle->FirstChild("Body");
	TiXmlElement* CameraElement = BodyHandle->FirstChild("Camera").Element();
		t.x=atof(CameraElement->Attribute("x"));
		t.y=atof(CameraElement->Attribute("y"));
		t.z=atof(CameraElement->Attribute("z"));
		s=atof(CameraElement->Attribute("sx"));
		AngleX=atof(CameraElement->Attribute("rx"));
		AngleY=atof(CameraElement->Attribute("ry"));
#endif
#endif
		D3DXFROMWINEMatrixRotationYawPitchRoll( &R,
		                            D3DXFROMWINEToRadian(AngleX),
		                            D3DXFROMWINEToRadian(AngleY),
		                            0.0f );
		D3DXFROMWINEMatrixScaling( &S, s, s, s); 
		D3DXFROMWINEMatrixTranslation( &T, t.x, t.y, t.z); 
    mvp= R*S*T;
}
		} else {
	stx_GetAngles(mAngleX, mAngleY, 0.5f);
#if 0
    AngleX = mAngleX+angles[2*m_i+0];
    AngleY = mAngleY+angles[2*m_i+1];
#else
    AngleX = mAngleX;
    AngleY = mAngleY;
#endif
		D3DXFROMWINEMatrixRotationYawPitchRoll( &mvp,
		                            D3DXFROMWINEToRadian(AngleX),
		                            D3DXFROMWINEToRadian(AngleY),
		                            0.0f );
		}
	if(1)
	{
#if 1
		m_Mesh[m_i].render(&mvp);
#else
	m_Mesh[m_i].BeginDraw(&mvp);//, -1, g_pCurrentTechnique,g_pCurrentLayout);

    IRenderer::GetRendererInstance()->setShaderConstant4x4f("WorldViewProj", mWorldViewProj );
    IRenderer::GetRendererInstance()->setShaderConstant4f("EyePos", vEye );
    IRenderer::GetRendererInstance()->setShaderConstant1f("LightIntensity", rnd );

	m_Mesh[m_i].EndDraw();
#endif
	}

#if 0
  		STXGUI::update();
#endif

	if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F1))
	{
		if(m_i<elementsOf(filename)-1)
			m_i++;
			//if(m_i>=elementsOf(filename))
			//	m_i=0;
		else
			m_i=0;
	}
	if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F2))
	{
		if(!m_i)
			m_i=elementsOf(filename)-1;
		else
			m_i--;
	}	

		const char* txt = "Use mouse buttons to rotate the model.";
		IRenderer::GetRendererInstance()->drawText(txt, 10, 10, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		txt = "CTRL-r resets the scene.";
		IRenderer::GetRendererInstance()->drawText(txt, 10, 30, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		char txt2[128];
		stx_snprintf(txt2, 128, "Model: %d/%d: %s\n", m_i, elementsOf(filename), filename[m_i]);
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 50, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());
#if 0
		stx_snprintf(txt2, 128, "AngleX=%f\n", AngleX);
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 70, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		stx_snprintf(txt2, 128, "AngleY=%f\n", AngleY);
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 90, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());
#endif
		IRenderer::GetRendererInstance()->EndScene();
		IRenderer::GetRendererInstance()->Present();
	}
};
int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("3DAnimation6");		
	IInput*    i=STX_Service::GetInputInstance();
	{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();

		const char* txt = "Initializing. Please wait a moment.";
		IRenderer::GetRendererInstance()->drawText(txt, 250, 250, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
	}
	_3DAnimation app;
	app.init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		app.render();
	}
	return 0;
}

