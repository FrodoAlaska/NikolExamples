struct vs_in {
  float3 position   : POS; 
  float3 normal     : NORMAL; 
  float2 tex_coords : TEX;
};

struct vs_out {
  float3 position   : SV_POSITION; 
  float2 tex_coords : TEX;
};

cbuff Matrices {
  float4x4 view_projection;
};

vs_out vs_main(vs_in input) {
  vs_out out;

  out.position   = mul(view_projection, input.position); 
  out.tex_coords = input.tex_coords;

  return out;
}
    
Texture2D text    : register(t0);
SamplerState samp : register(s0);

float4 ps_main(vs_out input) {
  return float4(1.0, 1.0, 1.0, 1.0);
}
