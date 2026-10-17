// Material-state bridge only. PENDING-ORIGINAL: custom planet/cloud shader program.
// Blend/depth/cull and source textures are loaded verbatim from serialized assets.
Shader "SWGoH/OriginalStateUnlitBridge"
{
    Properties
    {
        _MainTex("Original main texture", 2D) = "white" {}
        _Color("Original tint", Color) = (1,1,1,1)
        _SrcBlend("Source blend", Float) = 1
        _DstBlend("Destination blend", Float) = 0
        _SrcBlendAlpha("Source alpha blend", Float) = 1
        _DstBlendAlpha("Destination alpha blend", Float) = 0
        _ZWrite("Depth write", Float) = 1
        _ZTest("Depth test", Float) = 4
        _Cull("Cull", Float) = 2
        _ColorMask("Color mask", Float) = 15
    }
    SubShader
    {
        Pass
        {
            Blend [_SrcBlend] [_DstBlend], [_SrcBlendAlpha] [_DstBlendAlpha]
            ZWrite [_ZWrite]
            ZTest [_ZTest]
            Cull [_Cull]
            ColorMask [_ColorMask]
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            sampler2D _MainTex;
            float4 _MainTex_ST, _Color;
            struct appdata { float4 vertex : POSITION; float2 uv : TEXCOORD0; };
            struct v2f { float4 vertex : SV_POSITION; float2 uv : TEXCOORD0; };
            v2f vert(appdata v)
            {
                v2f o; o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex); return o;
            }
            fixed4 frag(v2f i) : SV_Target { return tex2D(_MainTex, i.uv) * _Color; }
            ENDCG
        }
    }
}
