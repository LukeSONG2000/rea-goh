# Stage UI data for the Unity roster/detail screens.
#
# Joins (all ORIGINAL game data, no invented values):
#   unity/SWGoH-Restored/Assets/StreamingAssets/roster.json   (unit list + stats)
#   catalog/units.json            -> thumbnailName (portrait), categoryId (tags),
#                                    skillReference/limitBreakRef (skills), maxRarity
#   catalog/character_names.json  -> folder -> defId, name_zh
#   catalog/gamedata/skill.json   -> skillId -> abilityReference
#   catalog/gamedata/ability.json -> abilityId -> nameKey, icon
#   catalog/gamedata/category.json-> categoryId -> descKey
#   catalog/loc_cn.json           -> all Chinese strings (official loc)
#
# Outputs into StreamingAssets:
#   ui_units.json   one entry per roster unit (roster grid + detail screen)
#   ui_labels.json  screen labels with their loc keys as provenance
import json
import os
import re

ROOT = r"F:\PPfavorite\Work\goh"
CAT = os.path.join(ROOT, "catalog")
SA = os.path.join(ROOT, "unity", "SWGoH-Restored", "Assets", "StreamingAssets")
PORTRAITS = os.path.join(ROOT, "extracted_ui", "charui")
ICONS = os.path.join(ROOT, "extracted_ui", "atlas_icons")


def load(p):
    with open(p, encoding="utf-8") as f:
        return json.load(f)


roster = load(os.path.join(SA, "roster.json"))
units = load(os.path.join(CAT, "units.json"))
units = units if isinstance(units, list) else units.get("data", [])
names = load(os.path.join(CAT, "character_names.json"))
skills = load(os.path.join(CAT, "gamedata", "skill.json"))["data"]
abilities = load(os.path.join(CAT, "gamedata", "ability.json"))["data"]
cats = load(os.path.join(CAT, "gamedata", "category.json"))["data"]
loc = load(os.path.join(CAT, "loc_cn.json"))

skill_by_id = {s["id"]: s for s in skills}
ability_by_id = {a["id"]: a for a in abilities}
cat_by_id = {c["id"]: c for c in cats}

# units.json carries star variants ("DEFID:SEVEN_STAR"); prefer 7-star, else any
unit_by_def = {}
for u in units:
    base = str(u.get("id", "")).split(":")[0]
    stars = str(u.get("id", "")).split(":")[1] if ":" in str(u.get("id", "")) else ""
    rank = {"ONE_STAR": 1, "TWO_STAR": 2, "THREE_STAR": 3, "FOUR_STAR": 4,
            "FIVE_STAR": 5, "SIX_STAR": 6, "SEVEN_STAR": 7}.get(stars, 0)
    cur = unit_by_def.get(base)
    if cur is None or rank >= cur[0]:
        unit_by_def[base] = (rank, u)


def cn(key, fallback=""):
    v = loc.get(key)
    if isinstance(v, str) and v:
        return v
    return fallback


def cat_cn(cat_id):
    c = cat_by_id.get(cat_id)
    if not c:
        return None
    dk = c.get("descKey") or ""
    v = cn(dk)
    if v and v != dk:
        return v
    return None  # untranslated/internal category


TAG_PREFIXES = ("alignment_", "role_", "affiliation_", "profession_")
TAG_EXACT = {"galactic_legend", "unaligned_force_user"}


def unit_tags(u):
    out, seen = [], set()
    ids = [c for c in u.get("categoryId", []) if isinstance(c, str)]
    # P4 order: 银河传奇 leads, everything else keeps the unit's own
    # categoryId order (unaligned_force_user sits last for Kylo, as in P4)
    ordered = [c for c in ids if c == "galactic_legend"] + \
              [c for c in ids if c != "galactic_legend"]
    for c in ordered:
        keep = c in TAG_EXACT or any(c.startswith(p) for p in TAG_PREFIXES) \
            or c.startswith("unaligned_")
        if not keep or c in seen:
            continue
        seen.add(c)
        t = cat_cn(c)
        if t:
            out.append(t)
        if len(out) >= 8:
            break
    return out


def alignment(u):
    ids = u.get("categoryId", []) or []
    for a, val in (("alignment_light", "light"), ("alignment_dark", "dark")):
        if a in ids:
            return val
    return "neutral"


SKILL_TAG = {
    "ultimateability": "UNITDETAILSVIEW_ABILITY_INFOPANEL_ULTIMATE_TAG",
    "basicskill": "UnitDetailsView_Ability_InfoPanel_Primary_Tag",
    "basicability": "UnitDetailsView_Ability_InfoPanel_Primary_Tag",
    "specialskill": "UnitDetailsView_Ability_InfoPanel_LimitBreak_Tag",
    "specialability": "UnitDetailsView_Ability_InfoPanel_LimitBreak_Tag",
    "leaderskill": "UnitDetailsView_Ability_InfoPanel_Leader_Tag",
    "leaderability": "UnitDetailsView_Ability_InfoPanel_Leader_Tag",
    "uniqueskill": "UnitDetailsView_Ability_InfoPanel_Unique_Tag",
    "uniqueability": "UnitDetailsView_Ability_InfoPanel_Unique_Tag",
}


def skill_entry(ability_id):
    a = ability_by_id.get(ability_id)
    if not a:
        return None
    tag_key = None
    for p, k in SKILL_TAG.items():
        if ability_id.startswith(p):
            tag_key = k
            break
    icon = a.get("icon") or ""
    icon_file = icon + ".png"
    return {
        "id": ability_id,
        "name": cn(a.get("nameKey", ""), a.get("nameKey", "")),
        "icon": icon,
        "icon_ok": os.path.exists(os.path.join(ICONS, icon_file)),
        "tag": cn(tag_key, "") if tag_key else "",
    }


def unit_skills(u):
    out = []
    # ultimate abilities live in limitBreakRef (P4 lists the ultimate first)
    for r in u.get("limitBreakRef", []) or []:
        aid = r.get("abilityId", "")
        if aid.startswith("ultimateability") or aid.startswith("ultimate"):
            e = skill_entry(aid)
            if e:
                out.append(e)
    seen = {e["id"] for e in out}
    for r in u.get("skillReference", []) or []:
        sid = r.get("skillId", "")
        s = skill_by_id.get(sid)
        if not s:
            continue
        aid = s.get("abilityReference") or ""
        if aid.startswith("hidden"):
            continue
        e = skill_entry(aid)
        if e and e["id"] not in seen:
            seen.add(e["id"])
            out.append(e)
    return out


def portrait_of(u, def_id):
    tn = u.get("thumbnailName") or ""
    if tn and os.path.exists(os.path.join(PORTRAITS, tn + ".png")):
        return tn, "thumbnail"
    # fuzzy: folder fragment from character_names mapping
    frag = None
    for folder, info in names.items():
        if info.get("defId") == def_id:
            frag = re.sub(r"^char_", "", folder)
            frag = re.sub(r"_pre$", "", frag)
            break
    if frag:
        for c in (frag, frag.split("_")[0]):
            for cand in (f"tex.charui_{c}",):
                if os.path.exists(os.path.join(PORTRAITS, cand + ".png")):
                    return cand, "fuzzy:" + frag
    return None, "miss"


out = []
stat_direct = stat_fuzzy = stat_miss = 0
icon_ok = icon_total = 0
for r in roster:
    def_id = r.get("def_id")
    u = unit_by_def.get(def_id, (0, {}))[1]
    name = None
    for folder, info in names.items():
        if info.get("defId") == def_id:
            name = info.get("name_zh") or info.get("name_en")
            break
    if not name:
        name = r.get("name") or def_id
    power = 0
    for kv in r.get("stats", []) or []:
        if kv.get("k") == "25":
            power = int(round(kv.get("v", 0) / 10.0))
    thumb, how = portrait_of(u, def_id)
    if how == "thumbnail":
        stat_direct += 1
    elif how.startswith("fuzzy"):
        stat_fuzzy += 1
    else:
        stat_miss += 1
    sk = unit_skills(u)
    for e in sk:
        icon_total += 1
        if e["icon_ok"]:
            icon_ok += 1
    entry = {
        "def_id": def_id,
        "folder": r.get("folder"),
        "name": name,
        "level": 85,
        "stars": u.get("maxRarity") or 7,
        "power": power,
        "alignment": alignment(u) if u else "neutral",
        "legend": bool(u.get("legend")) if u else False,
        "tags": unit_tags(u) if u else [],
        "skills": sk,
        "portrait": thumb,
        "combat_type": u.get("combatType") if u else 1,
    }
    out.append(entry)

labels = {
    # key -> [loc_key, loc_cn value]  (provenance travels with the data)
    "inventory_title": ["FeatureTitle_ShipInventory", cn("FeatureTitle_ShipInventory", "物品栏")],
    "inventory_subtitle": ["InventoryView_HUDSubTitle", cn("InventoryView_HUDSubTitle", "")],
    "galactic_power_fmt": ["InventoryView_HUDGalacticPower", cn("InventoryView_HUDGalacticPower", "银河力量：{0}")],
    "galactic_power_name": ["STAT_GALACTIC_POWER_ACQUIRED_NAME", cn("STAT_GALACTIC_POWER_ACQUIRED_NAME", "银河力量：")],
    "unit_power_fmt": ["UnitDetailsView_UnitPower", cn("UnitDetailsView_UnitPower", "强度：{0}")],
    "gear_tier_13": ["Unit_Tier13", cn("Unit_Tier13", "装备等级XIII")],
    "maxed_fmt": ["UnitDetailsView_Ability_Maxed_Format", cn("UnitDetailsView_Ability_Maxed_Format", "满级{0}")],
    "tab_characters": ["INVENTORY_VIEW_CHARACTER_TAB_NAME", cn("INVENTORY_VIEW_CHARACTER_TAB_NAME", "角色")],
    "tab_all": ["INVENTORY_VIEW_ALL_TITLE", cn("INVENTORY_VIEW_ALL_TITLE", "全部")],
    "tab_favorites": ["INVENTORY_VIEW_FAVORITES_TITLE", cn("INVENTORY_VIEW_FAVORITES_TITLE", "收藏")],
    "tab_squads": ["INVENTORY_VIEW_SQUADS_TAB_NAME", cn("INVENTORY_VIEW_SQUADS_TAB_NAME", "小队")],
    "tab_ships": ["INVENTORY_VIEW_SHIP_TAB_NAME", cn("INVENTORY_VIEW_SHIP_TAB_NAME", "飞船")],
    "tab_fleets": ["CampaignView_Fleets", cn("CampaignView_Fleets", "舰队")],
    "tab_datacrons": ["INVENTORY_DATACRON_TAB_NAME", cn("INVENTORY_DATACRON_TAB_NAME", "数据全像记录器")],
    "tag_basic": ["UnitDetailsView_Ability_InfoPanel_Primary_Tag", cn("UnitDetailsView_Ability_InfoPanel_Primary_Tag", "（基本）")],
    "tag_special": ["UnitDetailsView_Ability_InfoPanel_LimitBreak_Tag", cn("UnitDetailsView_Ability_InfoPanel_LimitBreak_Tag", "（特殊）")],
    "tag_leader": ["UnitDetailsView_Ability_InfoPanel_Leader_Tag", cn("UnitDetailsView_Ability_InfoPanel_Leader_Tag", "（首领）")],
    "tag_unique": ["UnitDetailsView_Ability_InfoPanel_Unique_Tag", cn("UnitDetailsView_Ability_InfoPanel_Unique_Tag", "（独有）")],
    "tag_ultimate": ["UNITDETAILSVIEW_ABILITY_INFOPANEL_ULTIMATE_TAG", cn("UNITDETAILSVIEW_ABILITY_INFOPANEL_ULTIMATE_TAG", "（终极）")],
    # P3 shows a first tab "时代单位" (Era Units) and "光速代币" that do not exist
    # in the current loc dump (older-version strings) -- REFERENCE-MEASURED.
    "tab_era_units": ["(not in loc)", "时代单位"],
    "tab_lightspeed": ["(not in loc)", "光速代币"],
}

with open(os.path.join(SA, "ui_units.json"), "w", encoding="utf-8") as f:
    json.dump(out, f, ensure_ascii=False)
with open(os.path.join(SA, "ui_labels.json"), "w", encoding="utf-8") as f:
    json.dump({"labels": labels}, f, ensure_ascii=False, indent=1)

print(f"[stage] units={len(out)} portrait direct={stat_direct} fuzzy={stat_fuzzy} miss={stat_miss}")
print(f"[stage] skill icons ok={icon_ok}/{icon_total}")
print(f"[stage] sample:", json.dumps(out[0], ensure_ascii=False)[:400])
ky = [e for e in out if e["def_id"] == "SUPREMELEADERKYLOREN"]
if ky:
    print("[stage] kylo:", json.dumps(ky[0], ensure_ascii=False)[:900])
