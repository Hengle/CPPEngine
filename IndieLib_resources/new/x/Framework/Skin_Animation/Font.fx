
BlendState blendState
{
	BlendEnable[0] = true;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	RenderTargetWriteMask[0] = 0xF;
};

DepthStencilState dsState
{
	DepthEnable = false;
	DepthWriteMask = false;
	StencilEnable = false;
};

RasterizerState rsState
{
	CullMode = None;
	DepthClipEnable = true;
};

Texture2D tex;
SamplerState smpState
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

cbuffer perPass
{
	float4 color;
};

struct InOut
{
	float4 pos: SV_Position;
	float2 texCoord: texCoord;
};

InOut vsMain(InOut In)
{
	return In;
}

float4 psMain(InOut In): SV_Target
{
	return color * tex.Sample(smpState, In.texCoord);
}

/////////////////////////////////////////////////////////////////////////////
[techniques]
<Textures>
	<Texture sampler="Base" file="/test.bmp" dimension="2" mipmap="false" SamplerState="linear" />
</Textures>
<Techniques>
<technique name="t0">
	<pass name="font">
		<RasterizerState name="rsState" />
		<BlendState name="blendState" R="0.0" G="0.0" B="0.0" A="0.0" X="0xFFFFFFFF" />
		<DepthStencilState name="dsState" X="0" />
		<VertexShader profile="vs_4_0" name="vsMain" />
		<GeometryShader profile="gs_4_0" name="" />
		<PixelShader profile="ps_4_0" name="psMain" />
		<VertexFormat>
        		<FormatDesc Type="VERTEX" 	Format="FLOAT4" />
			<FormatDesc Type="NORMAL"	Format="FLOAT4" />
		</VertexFormat>
    	</pass>
	</technique>
</Techniques>

