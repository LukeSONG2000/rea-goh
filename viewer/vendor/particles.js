// Generic particle-emitter layer for exported Unity ParticleSystems.
// window.EnvFX.build(entries, texBase) -> {group, update(dt)}  (group per scene)
// Entry: {name,pos,up,lifetime,speed,size,rate,gravity,shape,color,
//         colorOverLifetime,sizeOverLifetime,material:{tex,tint},shader}
// Missing textures fall back to a procedural radial glow.
(function(){
'use strict';

function glowTexture(){
  const c = document.createElement('canvas'); c.width = c.height = 64;
  const g = c.getContext('2d');
  const gr = g.createRadialGradient(32,32,0, 32,32,32);
  gr.addColorStop(0,'rgba(255,255,255,1)');
  gr.addColorStop(0.35,'rgba(255,255,255,.55)');
  gr.addColorStop(1,'rgba(255,255,255,0)');
  g.fillStyle = gr; g.fillRect(0,0,64,64);
  const t = new THREE.CanvasTexture(c);
  return t;
}
let FALLBACK = null;

const VERT = `
attribute float aAlpha;
attribute float aSize;
varying float vAlpha;
void main(){
  vAlpha = aAlpha;
  vec4 mv = modelViewMatrix * vec4(position,1.0);
  gl_PointSize = aSize * (160.0 / -mv.z);
  gl_Position = projectionMatrix * mv;
}`;
const FRAG = `
uniform sampler2D uTex;
uniform vec3 uColor;
varying float vAlpha;
void main(){
  vec4 t = texture2D(uTex, gl_PointCoord);
  gl_FragColor = vec4(uColor * t.rgb, t.a * vAlpha);
  if (gl_FragColor.a < 0.01) discard;
}`;

function firstColor(col){
  if (col && col.mode === 'gradient' && col.stops.length)
    return col.stops[0][1];
  if (col && col.value) return col.value;
  return null;
}
function sizeAt(s, u){
  if (s && s.mode === 'curve'){
    const k = s.keys;
    if (k && k.length){
      if (u <= k[0][0]) return k[0][1];
      for (let i=1;i<k.length;i++){
        if (u <= k[i][0]){
          const [t0,v0] = k[i-1], [t1,v1] = k[i];
          return v0 + (v1-v0) * (u-t0)/Math.max(t1-t0,1e-5);
        }
      }
      return k[k.length-1][1];
    }
    return s.scalar || 1;
  }
  return 1;
}
function colorAt(col, u){
  if (col && col.mode === 'gradient' && col.stops.length){
    const st = col.stops;
    if (u <= st[0][0]) return st[0][1];
    for (let i=1;i<st.length;i++){
      if (u <= st[i][0]){
        const [t0,c0] = st[i-1], [t1,c1] = st[i];
        const f = (u-t0)/Math.max(t1-t0,1e-5);
        return c0.map((v,j)=>v + (c1[j]-v)*f);
      }
    }
    return st[st.length-1][1];
  }
  return col && col.value || null;
}

function build(entries, texBase){
  const group = new THREE.Group();
  const emitters = [];
  let idSeq = 0;

  entries.forEach(e=>{
    // clamp wild authored values (saber trails author speed*life > 20 units;
    // point sprites can't honor them and just erupt)
    const life = Math.min(Math.max(e.lifetime || 1, 0.05), 3);
    const rate = e.rate || 0;
    const isStatic = rate <= 0;
    const tint = (e.material && e.material.tint) || [1,1,1,1];
    const c0 = firstColor(e.color) || firstColor(e.colorOverLifetime) || [1,1,1,1];
    const baseSize = Math.max(e.size || 0.5, 0.05);
    const speed = Math.max(Math.min(e.speed || 0, 3), -3);
    const vel = e.velocity || {};
    const up = e.up || [0,1,0];
    const radius = (e.shape && e.shape.radius) || 0;
    const n = isStatic
      ? Math.min(e.maxParticles || 4, 8)
      : Math.min(Math.max(Math.ceil(rate*life)*2, 6), 96);

    const texUrl = e.material && e.material.tex ? texBase + e.material.tex : null;

    const em = {
      e, life, isStatic, tint, c0, baseSize, speed, vel, up, radius, n,
      pos: new THREE.Vector3().fromArray(e.pos || [0,0,0]),
      ages: new Float32Array(n),
      seeds: new Float32Array(n*4),
      ready: false,
      id: idSeq++,
    };
    for (let i=0;i<n;i++){
      em.ages[i] = Math.random();
      em.seeds[i*4] = Math.random()*2-1;
      em.seeds[i*4+1] = Math.random()*2-1;
      em.seeds[i*4+2] = Math.random()*2-1;
      em.seeds[i*4+3] = Math.random();
    }
    const geo = new THREE.BufferGeometry();
    geo.setAttribute('position', new THREE.BufferAttribute(new Float32Array(n*3), 3));
    geo.setAttribute('aAlpha', new THREE.BufferAttribute(new Float32Array(n), 1));
    geo.setAttribute('aSize', new THREE.BufferAttribute(new Float32Array(n), 1));
    const mat = new THREE.ShaderMaterial({
      uniforms: { uTex: {value:null}, uColor: {value: new THREE.Color(1,1,1)} },
      vertexShader: VERT, fragmentShader: FRAG,
      transparent: true, depthWrite: false,
      blending: /additive/i.test(e.shader || '') ? THREE.AdditiveBlending
                                                 : THREE.NormalBlending,
    });
    const points = new THREE.Points(geo, mat);
    points.frustumCulled = false;
    points.userData.vfxSkip = true;
    group.add(points);
    em.geo = geo; em.mat = mat; em.points = points; em.entry = e;
    emitters.push(em);

    const setTex = t=>{
      em.mat.uniforms.uTex.value = t;
      em.ready = true;
      update(0, 0);   // first layout once tex known (size aspect irrelevant)
    };
    if (texUrl){
      new THREE.TextureLoader().load(texUrl, t=>{
        t.minFilter = THREE.LinearFilter;
        setTex(t);
      }, undefined, ()=>setTex(FALLBACK || (FALLBACK = glowTexture())));
    } else {
      setTex(FALLBACK || (FALLBACK = glowTexture()));
    }
  });

  const tmp = new THREE.Vector3();

  function update(dt, time){
    emitters.forEach(em=>{
      if (!em.ready) return;
      const e = em.e;
      const colOL = e.colorOverLifetime;
      const szOL = e.sizeOverLifetime;
      const alphaBase = em.tint.length > 3 ? em.tint[3] : 1;
      const p = em.geo.attributes.position.array;
      const aA = em.geo.attributes.aAlpha.array;
      const aS = em.geo.attributes.aSize.array;
      for (let i=0;i<em.n;i++){
        let u;
        if (em.isStatic){
          u = em.ages[i] < 1 ? 0.5 : 0.5;   // static glows persist
        } else {
          em.ages[i] += dt / em.life;
          if (em.ages[i] >= 1) em.ages[i] -= 1 + Math.random()*0.05;
          u = em.ages[i];
        }
        const s = em.seeds, j = i*4;
        const drift = em.speed * u * em.life;
        const rise = (em.vel.y || 0) * u * em.life;
        const swirl = 0.25 * u * em.life;
        const rr = em.radius;
        p[i*3]   = em.pos.x + (s[j]*0.5+0.0)*rr + Math.sin(time*2+s[j+3]*6.28)*swirl;
        p[i*3+1] = em.pos.y + em.up[1]*drift + rise - 0.5*9.8*(e.gravity||0)*u*u*em.life
                   + (s[j+1]*0.5)*rr;
        p[i*3+2] = em.pos.z + (s[j+2]*0.5)*rr + Math.cos(time*1.7+s[j+3]*6.28)*swirl;
        const fade = em.isStatic ? (0.6 + 0.4*Math.sin(time*1.3+s[j+3]*6.28)) : (1-u)*(1-u);
        aA[i] = alphaBase * fade;
        aS[i] = em.baseSize * sizeAt(szOL, u);
        const c = colorAt(colOL, u);
        if (c && i === 0) em.mat.uniforms.uColor.value.setRGB(
          c[0]*em.tint[0], c[1]*em.tint[1], c[2]*em.tint[2]);
        else if (c && !em._colSet){
          em.mat.uniforms.uColor.value.setRGB(
            c[0]*em.tint[0], c[1]*em.tint[1], c[2]*em.tint[2]);
          em._colSet = true;
        }
      }
      em.geo.attributes.position.needsUpdate = true;
      em.geo.attributes.aAlpha.needsUpdate = true;
      em.geo.attributes.aSize.needsUpdate = true;
    });
  }

  return { group, update, emitters };
}

window.EnvFX = { build };
})();
