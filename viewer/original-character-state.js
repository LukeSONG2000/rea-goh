/* Original serialized object state. Shared with Unity; no name-based variant rules. */
(function(global) {
  'use strict';
  const cache = new Map();
  const clean = n => THREE.PropertyBinding.sanitizeNodeName(n);
  function visible(node) { for (let p=node;p;p=p.parent) if (!p.visible) return false; return true; }
  function find(root, path) {
    if (!path) return null;
    const parts = path.split('/').slice(1).map(clean);
    let matches = [];
    root.traverse(node => {
      let p = node, i = parts.length - 1;
      while (p && i >= 0 && p.name === parts[i]) { p = p.parent; i--; }
      if (i < 0 && (p === root || p?.parent === root)) matches.push(node);
    });
    if (!parts.length) return root;
    return matches.length === 1 ? matches[0] : null;
  }
  function authored(root) {
    root.traverse(node => {
      const ext = node.userData?.gltfExtensions;
      if (ext && typeof ext.SWGOH_active === 'boolean') node.visible = ext.SWGOH_active;
      if (node.isMesh && ext && typeof ext.SWGOH_renderer_enabled === 'boolean')
        node.visible = node.visible && ext.SWGOH_renderer_enabled;
    });
  }
  async function attach(root, bundle, context = 'unitDetail') {
    authored(root);
    if (!cache.has(bundle)) cache.set(bundle, fetch('/unity/SWGoH-Restored/Assets/Resources/CharacterPresentation/' + encodeURIComponent(bundle) + '.json')
      .then(r => r.ok ? r.json() : null));
    const data = await cache.get(bundle);
    if (!data) return null;
    const pending = [];
    for (const entry of data.gameObjects) {
      const node = find(root, entry.hierarchyPath);
      if (node && node !== root) node.visible = entry.activeSelf;
    }
    for (const entry of data.renderers) {
      const node = find(root, entry.hierarchyPath);
      if (!node) continue;
      // GLTFLoader may wrap a multiple-primitive renderer in a Group.
      const meshes = node.isMesh ? [node] : node.children.filter(c => c.isMesh);
      meshes.forEach(mesh => { mesh.userData.originalRendererEnabled = !!entry.raw.m_Enabled; mesh.visible = mesh.visible && !!entry.raw.m_Enabled; });
    }
    for (const component of data.components.filter(c => c.class === 'UnitComponent')) {
      (component.raw._propStates || []).forEach((entry, i) => {
        const binding = component.referenceBindings.find(b => b.fieldPath === `_propStates[${i}].prop`);
        const node = find(root, binding?.hierarchyPath);
        if (node) node.visible = !!entry[context] && node.userData.originalRendererEnabled !== false;
      });
    }
    const state = {
      pending,
      dispatch(id) {
        for (const component of data.components.filter(c => c.class === 'UnitSubObjectAnimationEventHandler')) {
          const list = component.raw._animationEvents;
          const index = list.findIndex(e => e.Id === id);
          if (index < 0) continue;
          list[index].Actions.forEach((action, i) => {
            // The binding path carries the exact 64-bit identity as a string;
            // JSON Number values inside the raw object are never used as IDs.
            const binding = component.referenceBindings.find(b => b.fieldPath === `_animationEvents[${index}].Actions[${i}].UnitSubObject`);
            const node = find(root, binding?.hierarchyPath);
            if (!node) { pending.push('Missing original event object: ' + id); return; }
            if (action.Type === 3 || action.Type === 4) node.visible = action.Type === 3 && node.userData.originalRendererEnabled !== false;
            else pending.push('Original sub-object action pending: ' + action.Type);
          });
          return;
        }
      },
      action: null, time: 0,
      update(action) {
        if (!action) return;
        if (this.action !== action) { this.action = action; this.time = -1e-8; }
        const clip = action.getClip(), now = action.time;
        const embedded = clip.userData?.SWGOH_events;
        const events = embedded?.length ? embedded : (data.clips || []).find(c => c.animationName === clip.name)?.events || [];
        const fire = (a,b) => events.filter(e => e.time > a && e.time <= b && e.functionName === 'PlaySubObjectAnimationEvent').forEach(e => this.dispatch(e.data));
        if (now < this.time) { fire(this.time, clip.duration); fire(-1e-8, now); }
        else fire(this.time, now);
        this.time = now;
      }
    };
    root.userData.originalCharacterState = state;
    return state;
  }
  function bindClipEvents(gltf) {
    const original = gltf.parser?.json?.animations || [];
    for (const clip of gltf.animations || []) {
      const raw = original.find(a => a.name === clip.name);
      clip.userData = {...clip.userData, SWGOH_events: raw?.extras?.SWGOH_events || []};
    }
  }
  global.OriginalCharacterState = {attach, authored, find, bindClipEvents, visible};
})(window);
