"""Read local source contracts; never creates game units, stats, or network calls.

Writes a provenance-bearing planning fixture, NOT a fabricated BattleStartResponse.
Usage: py tools/runtime_reuse/inspect_local_battle_contract.py > logs/high_reuse/local-battle-contract.json
"""
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
metadata_path = ROOT / "il2cpp/dump/cs/il2cpp.cs"
campaign_path = ROOT / "catalog/gamedata/campaign.json"
text = metadata_path.read_text(encoding="utf-8-sig")


def contract(name):
    match = re.search(r"^public (?:class|enum) " + re.escape(name) + r"(?:\s|:)", text, re.M)
    if not match:
        raise ValueError(name)
    start = text.index("{", match.start())
    depth = 1
    end = start + 1
    while depth:
        depth += (text[end] == "{") - (text[end] == "}")
        end += 1
    block = text[match.start():end]
    return {"line": text.count("\n", 0, match.start()) + 1, "declaration": block}


campaigns = json.loads(campaign_path.read_text(encoding="utf-8-sig"))["data"]
missions = []
for campaign in campaigns:
    if campaign["id"] not in ("C01L", "C01D"):
        continue
    for map_ in campaign["campaignMap"]:
        for group in map_["campaignNodeDifficultyGroup"]:
            for node in group["campaignNode"]:
                for mission in node["campaignNodeMission"]:
                    encounters = mission.get("campaignNodeEncounter", [])
                    if len(encounters) != 1 or not encounters[0].get("environment"):
                        continue
                    missions.append({
                        "campaignId": campaign["id"], "mapId": map_["id"],
                        "wireDifficulty": group["campaignNodeDifficulty"], "nodeId": node["id"],
                        "missionId": mission["id"], "combatTypeWire": mission["combatType"],
                        "missionOriginal": mission,
                        "note": "Original mission definition only; previews do not supply BattleData opponent UnitBattleStat payloads."
                    })
                    break
                if len(missions) >= 2:
                    break
            if len(missions) >= 2:
                break
        if len(missions) >= 2:
            break
    if len(missions) >= 2:
        break

print(json.dumps({
    "scope": "Read-only local planning contract; no generated stats, skills, BattleStartResponse, or game invocation",
    "sources": [{"path": str(p.relative_to(ROOT)), "sha256": hashlib.sha256(p.read_bytes()).hexdigest()}
                for p in (metadata_path, campaign_path)],
    "contracts": {name: contract(name) for name in (
        "BattleDataDO", "SquadDO", "SquadCellDO", "BattleUnitStateDO", "CampaignElementIdentifierDO",
        "SquadTypeEnum", "SquadUnitTypeEnum", "CampaignNodeDifficultyEnum", "CombatTypeEnum")},
    "missionCandidates": missions,
    "remainingRuntimeInputs": [
        "Injected and initialized original Factory/domain objects",
        "Original inventory UnitDO snapshots or locally retained original protobuf Unit/UnitBattleStat",
        "Original mission lookup must succeed; explicitly map wire enums through original translators",
        "No captured full BattleData fixture was identified in the searched local catalog/log paths"
    ]
}, indent=2, ensure_ascii=False))
