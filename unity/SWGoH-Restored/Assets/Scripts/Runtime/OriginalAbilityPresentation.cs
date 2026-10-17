using System;
using System.Collections.Generic;
using System.Linq;
using Newtonsoft.Json.Linq;
using UnityEngine;

namespace SwgohRestored.Battle
{
    // Interprets exported PlayMaker states. State names and hero names are data,
    // not branches. Missing actions stop playback rather than inventing a timeline.
    public sealed class OriginalAbilityPresentation : MonoBehaviour
    {
        readonly Dictionary<string, object> variables = new();
        readonly Dictionary<string, JObject> states = new();
        readonly List<Action<float>> updates = new();
        readonly Queue<string> events = new();
        JObject state, document, ability;
        Transform actor, target;
        Animator animator;
        OriginalAbilityObjectIds objects;
        readonly List<Action<float>> persistentUpdates = new();
        readonly Dictionary<string, Action<float>> tweenIds = new();
        public Transform[] AffectedUnits = Array.Empty<Transform>();
        public readonly Dictionary<int, Transform> AreaMarkers = new();
        Action apply;
        float elapsed;
        int generation;
        bool dispatching;
        public bool Finished { get; private set; }
        public bool Applied { get; private set; }
        public string Failure { get; private set; }
        public string StateName => (string)state?["name"];
        public float PlaybackSpeed = 1;
        public readonly List<string> Trace = new();

        public static JObject Load(string unit)
        {
            var asset = Resources.Load<TextAsset>("AbilityPresentation/" + unit);
            return asset ? JObject.Parse(asset.text) : null;
        }
        public void BindAreaMarkers(JObject data, BattleStage stage, int sourceSide)
        {
            // PlayerUnitMoveController.AOEMarkers -> EnemyAreaOfEffect; opponent
            // getter -> PlayerAreaOfEffect (original interface slots 6 and 5).
            string side = sourceSide == 0 ? "_enemyAreaOfEffect" : "_playerAreaOfEffect";
            foreach (var marker in data["areaOfEffectLayouts"][0]["markers"].Where(m => (string)m["sideField"] == side))
            {
                string field = (string)marker["field"];
                int kind = field == "_all" ? 1 : field == "_frontRow" ? 2 : field == "_backRow" ? 3 : 0;
                if (kind == 0) continue;
                var node = new GameObject("OriginalAOE" + field).transform;
                node.SetParent(transform, false);
                var pos = marker["layoutWorldPosition"];
                node.position = stage.LayoutRoot.TransformPoint(new Vector3((float)pos["x"], (float)pos["y"], (float)pos["z"]));
                var mat = marker["layoutWorldMatrix"];
                var forward = new Vector3((float)mat[0][2], (float)mat[1][2], (float)mat[2][2]);
                var up = new Vector3((float)mat[0][1], (float)mat[1][1], (float)mat[2][1]);
                node.rotation = stage.LayoutRoot.rotation * Quaternion.LookRotation(forward, up);
                AreaMarkers[kind] = node;
            }
        }

        public void Begin(JObject data, string abilityId, Transform source, Transform selected,
            Animator sourceAnimator, bool evaded, Action applyEffects)
        {
            document = data; actor = source; target = selected; animator = sourceAnimator; apply = applyEffects;
            ability = data["abilities"].OfType<JObject>().Single(x => (string)x["id"] == abilityId);
            var fsm = ability["fsm"][0];
            var prefab = Resources.Load<GameObject>("AbilityPresentation/" + abilityId);
            if (prefab) objects = Instantiate(prefab, transform).GetComponent<OriginalAbilityObjectIds>();
            foreach (JArray group in ((JObject)fsm["raw"]["fsm"]["variables"]).Properties().Select(x => x.Value).OfType<JArray>())
                foreach (var variable in group.OfType<JObject>())
                    variables[(string)variable["name"]] = Unwrap(variable["value"] ?? variable["intValue"]);
            variables["RootGameObject"] = actor; variables["Animator"] = animator.transform;
            variables["Source"] = actor; variables["Target"] = target;
            variables["OriginalPosition"] = actor.position; variables["OriginalRotation"] = actor.eulerAngles;
            variables["TargetEvaded"] = evaded; variables["AllyCameraGameObject"] = Camera.main?.transform;
            foreach (JObject s in fsm["decodedStates"]) states.Add((string)s["name"], s);
            Enter((string)fsm["raw"]["fsm"]["startState"]);
            Send("PlayAbility");
        }

        void Update()
        {
            if (Finished || Failure != null) return;
            float dt = Time.deltaTime * PlaybackSpeed;
            elapsed += dt;
            if (elapsed > 60) { Fail("Original FSM did not finish within watchdog"); return; }
            try { foreach (var update in persistentUpdates.ToArray()) update(dt); foreach (var update in updates.ToArray()) update(dt); Pump(); }
            catch (Exception e) { Fail(e.Message); }
        }
        public void FireAbilityFSMEvent(string value) => Send(value);
        // Audio events remain explicit until the original sound-bank player is connected.
        public void PostAudioEvent(string value) => Trace.Add("PENDING-ORIGINAL audio:" + value);
        public void Send(string value)
        {
            if (string.IsNullOrEmpty(value) || Finished || Failure != null) return;
            events.Enqueue(value); Pump();
        }
        void Pump()
        {
            if (dispatching) return;
            dispatching = true;
            try
            {
                int guard = 0;
                while (events.Count > 0 && !Finished && Failure == null)
                {
                    if (++guard > 128) throw new InvalidOperationException("Original FSM immediate transition loop");
                    var name = events.Dequeue();
                    var transition = state["transitions"].FirstOrDefault(t => (string)t["fsmEvent"]["name"] == name);
                    Trace.Add($"{elapsed:F4} event {name}");
                    if (transition != null) Enter((string)transition["toState"]);
                }
            }
            catch (Exception e) { Fail(e.Message); }
            finally { dispatching = false; }
        }
        void Enter(string name)
        {
            state = states[name]; generation++; updates.Clear();
            Trace.Add($"{elapsed:F4} state {name}");
            bool waits = false;
            foreach (JObject action in state["actions"])
            {
                if ((int?)action["enabled"] == 0) continue;
                waits |= Execute(action);
                if (events.Count > 0 || Finished) break;
            }
            if (!waits && events.Count == 0 && !Finished) events.Enqueue("FINISHED");
        }
        void Fail(string reason)
        {
            Failure = "PENDING-ORIGINAL " + StateName + ": " + reason;
            updates.Clear(); Debug.LogError("[OriginalPresentation] " + Failure);
        }
        object Unwrap(JToken v)
        {
            if (v == null || v.Type == JTokenType.Null) return null;
            if (v is JObject o)
            {
                if (o["useVariable"] != null)
                {
                    string n = (string)o["name"];
                    if ((int?)o["useVariable"] != 0 && !string.IsNullOrEmpty(n) && variables.TryGetValue(n, out var current)) return current;
                    return Unwrap(o["value"] ?? o["intValue"]);
                }
                if (o["ownerOption"] != null) return (int)o["ownerOption"] == 0 ? transform : Unwrap(o["gameObject"]);
                if (o["m_PathID"] != null) return objects ? objects.Resolve(o["m_PathID"].ToString()) : null;
                if (o["x"] != null) return new Vector3((float)o["x"], (float)o["y"], (float)o["z"]);
                return o;
            }
            return ((JValue)v).Value;
        }
        sealed class Args
        {
            readonly OriginalAbilityPresentation owner;
            readonly JArray parameters;
            public Args(OriginalAbilityPresentation o, JObject a) { owner = o; parameters = (JArray)a["params"]; }
            public JToken Param(string n) => parameters.FirstOrDefault(p => (string)p["name"] == n);
            public object Get(string n) => owner.Unwrap(Param(n)?["value"]);
            public float F(string n) => Convert.ToSingle(Get(n) ?? 0);
            public int I(string n) => Convert.ToInt32(Get(n) ?? 0);
            public bool B(string n) => Convert.ToBoolean(Get(n) ?? false);
            public string S(string n) => Convert.ToString(Get(n)) ?? "";
            public Vector3 V(string n) => Get(n) is Vector3 v ? v : Vector3.zero;
            public Transform T(string n) => Get(n) as Transform;
            public bool Assigned(string n)
            {
                var v = Param(n)?["value"];
                return v != null && !(v is JObject o && (int?)o["useVariable"] == 1 && (string)o["name"] == "");
            }
            public void Put(string n, object value)
            {
                var p = Param(n); string variable = (string)p?["variableName"] ?? (string)p?["value"]?["name"];
                if (!string.IsNullOrEmpty(variable)) owner.variables[variable] = value;
            }
            public List<object> Array(string n)
            {
                int i = parameters.IndexOf(Param(n)); var result = new List<object>();
                while (++i < parameters.Count && (string)parameters[i]["name"] == "") result.Add(owner.Unwrap(parameters[i]["value"]));
                return result;
            }
        }
        void Wait(float duration, string finish, Action<float> step = null)
        {
            float time = 0; int token = generation;
            Action<float> update = null;
            update = dt =>
            {
                if (generation != token) return;
                time += dt; step?.Invoke(duration <= 0 ? 1 : Mathf.Clamp01(time / duration));
                if (time >= duration) { updates.Remove(update); Send(finish); }
            };
            updates.Add(update);
        }
        Vector3 Components(Args p, Vector3 value, string x, string y, string z)
        { if (p.Assigned(x)) value.x = p.F(x); if (p.Assigned(y)) value.y = p.F(y); if (p.Assigned(z)) value.z = p.F(z); return value; }

        bool Execute(JObject action)
        {
            string type = ((string)action["type"]).Split('.').Last(); var p = new Args(this, action);
            switch (type)
            {
                case "Wait": Wait(p.F("time"), p.S("finishEvent")); return true;
                case "SetAnimatorTrigger": animator.SetTrigger(p.S("trigger")); break;
                case "BoolTest": Send(p.S(p.B("boolVariable") ? "isTrue" : "isFalse")); break;
                case "GameObjectIsNull": Send(p.S(p.T("gameObject") ? "isNotNull" : "isNull")); break;
                case "EnumSwitch":
                    var choices = p.Array("compareTo"); var signals = p.Array("sendEvent");
                    int at = choices.FindIndex(x => Convert.ToInt32(x) == p.I("enumVariable"));
                    if (at >= 0) Send(Convert.ToString(signals[at])); break;
                case "SetIntValue": p.Put("intVariable", p.I("intValue")); break;
                case "SetBoolValue": p.Put("boolVariable", p.B("boolValue")); break;
                case "SetGuidStringAction": p.Put("Store", Guid.NewGuid().ToString()); break;
                case "SetVector3Value": p.Put("vector3Variable", p.V("vector3Value")); break;
                case "SetVector3XYZ":
                    var value = p.Assigned("vector3Value") ? p.V("vector3Value") : p.V("vector3Variable");
                    p.Put("vector3Variable", Components(p, value, "x", "y", "z")); break;
                case "GetVector3XYZ":
                    var xyz = p.V("vector3Variable"); p.Put("storeX", xyz.x); p.Put("storeY", xyz.y); p.Put("storeZ", xyz.z); break;
                case "Vector3Add": p.Put("vector3Variable", p.V("vector3Variable") + p.V("addVector")); break;
                case "Vector3Subtract": p.Put("vector3Variable", p.V("vector3Variable") - p.V("subtractVector")); break;
                case "Vector3Multiply": p.Put("vector3Variable", p.V("vector3Variable") * p.F("multiplyBy")); break;
                case "Vector3Normalize": p.Put("vector3Variable", p.V("vector3Variable").normalized); break;
                case "Vector3Operator":
                    if (p.I("operation") != 7) throw new NotSupportedException("Vector3Operator " + p.I("operation"));
                    p.Put("storeVector3Result", p.V("vector1") - p.V("vector2")); break;
                case "GetVectorLength": p.Put("storeLength", p.V("vector3").magnitude); break;
                case "FloatMultiply": p.Put("floatVariable", p.F("floatVariable") * p.F("multiplyBy")); break;
                case "FloatOperator": case "IntOperator":
                    bool integer = type == "IntOperator"; float a = p.F(integer ? "integer1" : "float1"), b = p.F(integer ? "integer2" : "float2");
                    float r = p.I("operation") switch { 0 => a + b, 1 => a - b, 2 => a * b, 3 => a / b, _ => throw new NotSupportedException("arithmetic opcode") };
                    p.Put("storeResult", integer ? (object)(int)r : r); break;
                case "GetPosition": case "GetRotation":
                    var from = p.T("gameObject"); if (!from) break;
                    var v = type == "GetPosition" ? (p.I("space") == 0 ? from.position : from.localPosition) : (p.I("space") == 0 ? from.eulerAngles : from.localEulerAngles);
                    p.Put("vector", v); p.Put(type == "GetPosition" ? "x" : "xAngle", v.x); p.Put(type == "GetPosition" ? "y" : "yAngle", v.y); p.Put(type == "GetPosition" ? "z" : "zAngle", v.z); break;
                case "SetPosition": case "SetRotation":
                    var to = p.T("gameObject"); if (!to) break;
                    bool rotation = type == "SetRotation", world = p.I("space") == 0;
                    var before = rotation ? (world ? to.eulerAngles : to.localEulerAngles) : (world ? to.position : to.localPosition);
                    var next = Components(p, p.Assigned("vector") ? p.V("vector") : before, rotation ? "xAngle" : "x", rotation ? "yAngle" : "y", rotation ? "zAngle" : "z");
                    if (rotation) { if (world) to.eulerAngles = next; else to.localEulerAngles = next; }
                    else { if (world) to.position = next; else to.localPosition = next; } break;
                case "GetParent": p.Put("storeResult", p.T("gameObject")?.parent); break;
                case "SetParent":
                    var child = p.T("gameObject"); if (!child) break;
                    child.SetParent(p.T("parent"), p.B("worldPositionStays"));
                    if (p.B("resetLocalPosition")) child.localPosition = Vector3.zero;
                    if (p.B("resetLocalRotation")) child.localRotation = Quaternion.identity; break;
                case "GetChildCount": p.Put("storeResult", p.T("gameObject")?.childCount ?? 0); break;
                case "GetChildNum":
                    var parent = p.T("gameObject"); int index = p.I("childIndex");
                    p.Put("store", parent && index >= 0 && index < parent.childCount ? parent.GetChild(index) : null); break;
                case "GetUnitComponentReferencePointAction":
                    var unit = p.T("unit"); if (!unit) throw new InvalidOperationException("Unbound original UnitComponent");
                    var point = ReferencePoint(unit, p.I("referencePoint"));
                    p.Put("store", point); p.Put("storePosition", point ? (p.I("space") == 0 ? point.position : point.localPosition) : Vector3.zero); break;
                case "GetAbilityPositionAction": case "GetAbilityRotationAction":
                    bool angles = type == "GetAbilityRotationAction";
                    p.Put(angles ? "AbilityRotation" : "AbilityPosition", AbilityPoint(p.T("PositionComponent"), angles)); break;
                case "AbilitySpawnVfxAction":
                    var sourceFx = p.T("SourceVfx");
                    if (sourceFx) { sourceFx.position = AbilityPoint(sourceFx, false); sourceFx.GetComponent<ParticleSystem>()?.Play(true); }
                    var targetPool = p.T("TargetVfxParent");
                    if (targetPool)
                    {
                        if (AffectedUnits.Length > targetPool.childCount) throw new InvalidOperationException("Original VFX target pool exhausted");
                        for (int k = 0; k < AffectedUnits.Length; k++)
                        {
                            var fx = targetPool.GetChild(k); var component = PositionComponent(fx);
                            if (component == null) throw new InvalidOperationException("Missing VFX position component");
                            var refPoint = ReferencePoint(AffectedUnits[k], (int)component["_targetHitArea"]);
                            fx.position = refPoint ? refPoint.position : Vector3.zero;
                            fx.GetComponent<ParticleSystem>()?.Play(true);
                        }
                    } break;
                case "DORotateAction":
                    if (p.I("EaseType") != 1 || !p.B("Looping") || p.I("LoopType") != 2 || p.I("NumLoops") != -1)
                        throw new NotSupportedException("Unrecovered rotation tween configuration");
                    var spinner = p.T("Transform"); if (!spinner) throw new InvalidOperationException("Unbound weapon");
                    var initial = spinner.eulerAngles; var rotationDelta = p.V("TargetRotation") - initial; float spinTime = 0;
                    Action<float> spin = dt => { spinTime += dt; spinner.eulerAngles = initial + rotationDelta * (spinTime / p.F("Duration")); };
                    persistentUpdates.Add(spin); tweenIds[p.S("TweenID")] = spin; break;
                case "DOPathAction":
                    if (p.I("PathType") != 1 || p.B("ClosedPath") || p.I("PathOrientType") != 0 || p.B("Looping") || p.B("UseCustomEaseCurve"))
                        throw new NotSupportedException("Unrecovered path tween configuration");
                    var projectile = p.T("Transform"); if (!projectile) throw new InvalidOperationException("Unbound original projectile");
                    bool localPath = p.B("Local");
                    var path = new OriginalAbilityPath(localPath ? projectile.localPosition : projectile.position, p.Array("Waypoints").Cast<Vector3>().ToArray());
                    Wait(p.F("Duration"), p.S("FinishEvent"), t =>
                    {
                        var position = path.Evaluate(t, p.S("EaseTypeOverride"), p.I("EaseType"));
                        if (localPath) projectile.localPosition = position; else projectile.position = position;
                    }); return true;
                case "DOTweenKillAction":
                    string tweenId = p.S("TweenID");
                    if (tweenIds.TryGetValue(tweenId, out var oldTween)) { persistentUpdates.Remove(oldTween); tweenIds.Remove(tweenId); } break;
                case "StartParticleSystem":
                    var particle = p.T("particleSystem"); if (particle) particle.GetComponent<ParticleSystem>()?.Play(true); break;
                case "LookAt":
                    var look = p.T("gameObject"); if (look) look.LookAt(p.T("targetObject") ? p.T("targetObject").position : p.V("targetPosition")); break;
                case "MoveTowardsFinishedDistanceAction":
                    var mover = p.T("GameObject"); var dest = p.T("TargetObject") ? p.T("TargetObject").position : p.V("TargetPosition");
                    if (!mover) throw new InvalidOperationException("Missing original mover");
                    int token = generation;
                    updates.Add(dt => { if (token != generation) return; if (p.B("IgnoreVertical")) dest.y = mover.position.y;
                        float distance = Vector3.Distance(mover.position, dest);
                        mover.position = Vector3.MoveTowards(mover.position, dest, Mathf.Min(p.F("MaxSpeed") * dt, Mathf.Max(0, distance - p.F("FinishDistance"))));
                        if (distance <= p.F("FinishDistance")) Send(p.S("FinishEvent")); }); return true;
                case "iTweenMoveTo": case "iTweenRotateTo": case "iTweenLookTo":
                    if (p.I("easeType") != 21) throw new NotSupportedException("iTween easing " + p.I("easeType"));
                    var tr = p.T("gameObject"); if (!tr) throw new InvalidOperationException("Missing tween transform");
                    float duration = p.F("time");
                    if (type == "iTweenMoveTo") { var start = tr.position; var end = p.T("transformPosition") ? p.T("transformPosition").position : p.V("vectorPosition"); Wait(duration, p.S("finishEvent"), t => tr.position = Vector3.Lerp(start, end, t)); }
                    else { var start = tr.rotation;
                        var facing = p.T("transformTarget") ? p.T("transformTarget").position : p.V("vectorTarget");
                        var end = type == "iTweenRotateTo" ? Quaternion.Euler(p.V("vectorRotation")) : Quaternion.LookRotation(facing - tr.position, Vector3.up);
                        if (type == "iTweenLookTo" && p.I("axis") == 2) end = Quaternion.Euler(tr.eulerAngles.x, end.eulerAngles.y, tr.eulerAngles.z);
                        Wait(duration, p.S("finishEvent"), t => tr.rotation = Quaternion.Slerp(start, end, t)); }
                    return true;
                case "AbilityFsmAction":
                    switch (p.I("actionType"))
                    {
                        case 2: if (Applied) throw new InvalidOperationException("Duplicate original effect application"); Applied = true; apply(); Send("EffectsApplied"); return true;
                        case 3: Finished = true; break;
                        // Native AbilityFsmAction.OnEnter only dispatches 2/3/6/7.
                        // Enum value 5 falls through to Finish; it is not PlayHit.
                        case 5: Trace.Add("ORIGINAL_NOOP AbilityFsmAction 5"); break;
                        default: throw new NotSupportedException("AbilityFsmAction " + p.I("actionType"));
                    } break;
                case "AKAudioPlay": case "PlayShipAudioAction": PostAudioEvent(p.S("audioEvent")); break;
                case "DOShakePosition": Trace.Add("PENDING-ORIGINAL camera shake"); break;
                default: throw new NotSupportedException(type);
            }
            return false;
        }
        Transform ReferencePoint(Transform unit, int point)
        {
            var original = unit ? unit.GetComponentInParent<OriginalCharacterPresentation>() : null;
            if (original) return original.ReferencePoint(point);
            var areas = document["character"]["unitReferenceAreas"];
            string field = point switch { 0 => "_weapon", 1 => "_offhand", 2 => "_hitAreas", 3 => "_center", 4 => "_head", 5 => "_feet", 6 => "_uiArea", 7 => "_targetReticle", _ => "" };
            if (point == 2) throw new NotSupportedException("Random hit-area selection requires original presentation RNG");
            var row = areas.FirstOrDefault(a => (string)a["field"] == field && (int?)a["index"] == 0);
            if (row == null) throw new InvalidOperationException("Missing reference point " + point);
            string path = (string)row["hierarchyPath"];
            if (string.IsNullOrEmpty(path)) return null;
            var names = path.Split('/');
            foreach (Transform t in unit.GetComponentsInChildren<Transform>(true))
            {
                if (t.name != names.Last()) continue;
                var candidate = t; int i = names.Length - 1;
                while (candidate != null && i >= 0 && candidate.name == names[i]) { i--; candidate = candidate.parent; }
                if (i <= 0) return t;
            }
            throw new InvalidOperationException("Original reference transform not imported: " + path);
        }
        JObject PositionComponent(Transform obj)
        {
            foreach (JObject component in ability["components"])
            {
                if ((string)component["class"] != "AbilityPositionComponent") continue;
                var raw = (JObject)component["raw"];
                var bound = objects ? objects.Resolve(raw["m_GameObject"]["m_PathID"].ToString()) : null;
                if (bound == obj || (obj == transform && (string)component["hierarchyPath"] == (string)ability["prefabName"])) return raw;
            }
            return null;
        }
        Vector3 AbilityPoint(Transform obj, bool rotation)
        {
            var component = PositionComponent(obj);
            if (component == null) return target ? (rotation ? target.eulerAngles : target.position) : Vector3.zero;
            int area = (int)component["_abilityPosition"], targetType = (int)component["_targetType"];
            if (area == 0)
            {
                var selected = targetType == 3 ? actor : target;
                if (targetType == 1) selected = AffectedUnits.FirstOrDefault() ?? actor;
                var point = ReferencePoint(selected, (int)component["_targetHitArea"]);
                return point ? (rotation ? point.eulerAngles : point.position) : Vector3.zero;
            }
            if (area >= 1 && area <= 3 && AreaMarkers.TryGetValue(area, out var marker)) return rotation ? marker.eulerAngles : marker.position;
            throw new NotSupportedException("Original area marker/average not bound: " + area);
        }
    }
}
