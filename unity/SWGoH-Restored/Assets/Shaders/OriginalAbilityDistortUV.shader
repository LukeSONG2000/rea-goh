// Recovered from shared_shadercontainer pathID 7506758614639811023.
// No approximation with an Unlit shader: math follows the original D3D11
// program and serialized constant-buffer bindings; see shaders/distort_*.asm.
Shader "Custom/VFX/DistortUV Alpha Blended (Probes)"
{
    Properties
    {
        _MainTex ("Base (RGB)", 2D) = "white" {}
        _MainBrightness ("Main Brightness", Float) = 1
        _MainScroll ("Main Scroll", Vector) = (0,0,0,0)
        _DistortFactor ("Distortion Factor", Float) = 1
        _DistortMap ("Distortion Map", 2D) = "white" {}
        _DistortScroll ("Distortion Scroll", Vector) = (0,0,0,0)
        _Mask ("Mask: (R) Distortion (G) Distorted Alpha (B) Alpha", 2D) = "white" {}
        _MaskScrollG ("Mask Scroll (Green)", Vector) = (0,0,0,0)
        _Color ("Color: (RGBA)", Color) = (1,1,1,1)
    }
    SubShader
    {
        Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
        Pass
        {
            Tags { "LightMode"="ForwardBase" }
            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            ZTest LEqual
            Cull Off
            ColorMask RGBA
            CGPROGRAM
            #pragma target 3.0
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            sampler2D _MainTex, _DistortMap, _Mask;
            float4 _MainTex_ST, _DistortMap_ST, _Mask_ST;
            float4 _MainScroll, _DistortScroll, _MaskScrollG, _Color;
            float _MainBrightness, _DistortFactor;
            struct Input { float4 vertex:POSITION; float3 normal:NORMAL; float2 uv:TEXCOORD0; float4 color:COLOR; };
            struct Varying
            {
                float4 vertex:SV_POSITION;
                float3 probe:COLOR0;
                float4 color:COLOR1;
                float2 mainUV:TEXCOORD0;
                float2 distortUV:TEXCOORD1;
                float2 maskRawUV:TEXCOORD2;
                float2 maskGreenUV:TEXCOORD3;
            };
            Varying vert(Input v)
            {
                Varying o;
                o.vertex = mul(UNITY_MATRIX_VP,mul(unity_ObjectToWorld,float4(v.vertex.xyz,1)));
                // Original normal transform uses ObjectToWorld, then normalization.
                float3 n = normalize(mul((float3x3)unity_ObjectToWorld,v.normal));
                float4 n4 = float4(n,1);
                float3 sh = float3(dot(unity_SHAr,n4),dot(unity_SHAg,n4),dot(unity_SHAb,n4));
                float4 terms = n.yzzx * n.xyzz;
                sh += float3(dot(unity_SHBr,terms),dot(unity_SHBg,terms),dot(unity_SHBb,terms));
                sh += unity_SHC.rgb * (n.x*n.x-n.y*n.y);
                o.probe = max(1.055 * pow(max(sh,0),0.416666667) - 0.055,0);
                o.color = v.color;
                float phase = unity_ObjectToWorld._m03 + unity_ObjectToWorld._m13 + unity_ObjectToWorld._m23;
                o.mainUV = v.uv * _MainTex_ST.xy + _MainTex_ST.zw + frac(_MainScroll.xy * _Time.y + phase);
                o.distortUV = v.uv * _DistortMap_ST.xy + _DistortMap_ST.zw + frac(_DistortScroll.xy * _Time.y + phase);
                o.maskRawUV = v.uv;
                o.maskGreenUV = v.uv * _Mask_ST.xy + _Mask_ST.zw + frac(_MaskScrollG.xy * _Time.y);
                return o;
            }
            float4 frag(Varying i):SV_Target
            {
                float2 distortion = (tex2D(_DistortMap,i.distortUV).rg - .5) * 2 * _DistortFactor;
                float4 rawMask = tex2D(_Mask,i.maskRawUV);
                float2 offset = distortion * rawMask.r;
                float3 rgb = tex2D(_MainTex,i.mainUV+offset).rgb * _MainBrightness * _Color.rgb;
                float alpha = tex2D(_Mask,i.maskGreenUV+offset).g * rawMask.b * _Color.a;
                // Original PS does not read COLOR0 (probe), nor MainTex alpha.
                return float4(rgb,alpha) * i.color;
            }
            ENDCG
        }
    }
    Fallback Off
}
