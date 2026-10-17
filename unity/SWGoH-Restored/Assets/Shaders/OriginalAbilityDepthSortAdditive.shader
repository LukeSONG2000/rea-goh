// Recovered from shared_shadercontainer pathID -7765911051955890343.
// DXBC vs/ps and serialized render states: logs/vader_complete/shaders/depth_*.
Shader "Custom/VFX/Depth Sort Additive"
{
    Properties
    {
        _MainTex ("Particle Texture", 2D) = "white" {}
        _CameraDepth ("Camera Depth", Range(-1,1)) = -0.2
    }
    SubShader
    {
        Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
        Pass
        {
            Blend SrcAlpha One
            ZWrite Off
            ZTest LEqual
            Cull Off
            ColorMask RGBA
            CGPROGRAM
            #pragma target 3.0
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            sampler2D _MainTex;
            float4 _MainTex_ST;
            float _CameraDepth;
            struct Input { float4 vertex : POSITION; float2 uv : TEXCOORD0; float4 color : COLOR; };
            struct Varying { float4 vertex : SV_POSITION; float2 uv : TEXCOORD0; float4 color : COLOR; };
            Varying vert(Input v)
            {
                Varying o;
                o.vertex = mul(UNITY_MATRIX_VP, mul(unity_ObjectToWorld, float4(v.vertex.xyz,1)));
                // Original D3D11 instruction is subtraction in clip space, not
                // camera-depth texture sampling or a view-space distance offset.
                o.vertex.z -= _CameraDepth;
                o.uv = v.uv * _MainTex_ST.xy + _MainTex_ST.zw;
                o.color = v.color;
                return o;
            }
            float4 frag(Varying i) : SV_Target { return tex2D(_MainTex,i.uv) * i.color; }
            ENDCG
        }
    }
    Fallback Off
}
