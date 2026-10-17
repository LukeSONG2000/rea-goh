"""Emit a static, provenance-bearing trial-entry contract; never attach or invoke game code.

py tools/runtime_reuse/inspect_trial_contract.py > logs/high_reuse/trial-contract.json
The report is deliberately not a launchable fixture: missing enemy/state payloads
must not be replaced with inventory units, default stats, or a single encounter.
"""
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "il2cpp/dump/cs/il2cpp.cs"
TEXT = SOURCE.read_text(encoding="utf-8-sig")


def declaration(name):
    match = re.search(r"^public (?:abstract )?(?:class|enum) " + re.escape(name) + r"(?:\s|:)", TEXT, re.M)
    if not match:
        raise ValueError("Missing original metadata type: " + name)
    start = TEXT.index("{", match.start())
    depth, end = 1, start + 1
    while depth:
        depth += (TEXT[end] == "{") - (TEXT[end] == "}")
        end += 1
    return {"line": TEXT.count("\n", 0, match.start()) + 1,
            "declaration": TEXT[match.start():end]}


def source(path):
    file = ROOT / path
    return {"path": path, "sha256": hashlib.sha256(file.read_bytes()).hexdigest()}


def report():
    names = ["PreBattleController", "PreBattleStateTransitionData", "PvePreBattleController",
             "PvePreBattleState", "PvpPreBattleController", "PvpPreBattleState",
             "ConquestPreBattleController", "GuildRaidPreBattleController",
             "TerritoryBattlePreBattleController", "TerritoryTournamentPreBattleController",
             "BattleGameState", "BattleDataDO", "BattleDomainSimulationController"]
    return {
        "scope": "Static inspection only; no hooks, process attachment, RPC, or game mutation",
        "launchable": False,
        "sources": [source(p) for p in [
            "il2cpp/dump/cs/il2cpp.cs", "tools/runtime_reuse/original_fixture.js",
            "tools/runtime_reuse/probe_visual.js",
            "logs/high_reuse/native/trial_pve_click.annotated.txt",
            "logs/high_reuse/native/trial_battledata_factory.annotated.txt",
            "logs/high_reuse/native/simulation_domain_start.annotated.txt",
            "logs/high_reuse/native/simulation_domain_finish.annotated.txt"]],
        "verified": [
            {"claim": "PVE prebattle holds chosen squad, forced squad, support and mission identity",
             "source": "PreBattleController fields +158/+160/+168/+180; PvePreBattleController saved transition +228"},
            {"claim": "Original PVE click is not a side-effect-free transition-data builder",
             "rva": "196B310", "body": "trial_pve_click.annotated.txt",
             "effects": ["entry affordability and restrictions", "remove button event subscription",
                         "Condense current squad", "possibly clear expired datacron",
                         "UpdateSquad", "SetLastCampaignIdentifier", "CreateUnitReplacements",
                         "Factory.Get flags0 then enter battle"]},
            {"claim": "PVE transition copies mission/player squad/support/event/combat type; no enemy BattleData is constructed here",
             "rva": "196B310", "bodyLines": [350, 414]},
            {"claim": "BattleData factory consumes original BattleEncounter, UnitBattleStat, crew, multiplier overrides, datacron and unit states",
             "rva": "1D0BFF0", "body": "trial_battledata_factory.annotated.txt",
             "note": "Full decompile body; first listing export ends early at 1D0D010 and is not full function evidence"},
            {"claim": "Simulation StartBattle does not obtain or construct BattleData; caller must supply it",
             "rva": "1D11160", "field": "BattleDomainSimulationController.BattleData +88"},
            {"claim": "Simulation FinishBattle invokes local completion delegates without BattleService",
             "rva": "1D11260"},
        ],
        "requiredOriginalInputs": [
            "Exact selected mission/mode/event/zone/match/modifier identity, not fixed PVE/CHARACTER",
            "Detached copy of selected original SquadDO preserving order, roles, reinforcement, crew, datacron, support and replacements",
            "Original complete BattleData for that identity: every encounter squad/stat, granted abilities, seed, unit state and score",
            "Original transition-specific state: persistent HP/shields, consumables, AI decision trees, relic overrides and mode state when applicable",
            "Original mode module dependencies initialized; simulation domain must remain installed through finish",
            "Fresh injected UnityGameLoop and isolated mutable data; original shared loop Initialize does not reset shutdown",
            "Ownership-aware return to exact source screen and its original selection; current probe returns Home/Campaign only",
        ],
        "boundaries": {
            "PVE_character": "Native engine fixture validated previously; faithful selected mission replay pending original complete enemy payload",
            "PVE_ship": "Mission CombatType and squad support exist; full crew/commander/reinforcements + multi-encounter payload and visual run pending",
            "PVP": "Prebattle transition has MatchMakingDO opponent; preview completeness, seed and mode module contract not proven",
            "Conquest": "Separate controller and simulation mission resolver exist; node state, disk/consumable/granted effects and energy isolation pending",
            "GuildRaid": "Separate controller; phase/score/persistent unit state and raid-specific completion side effects pending",
            "TerritoryBattle_Tournament": "Separate controllers; zone/match/state/locked squads and no-submit lifecycle pending",
            "mission_selection_screen": "No finalized squad transition yet; must read original selection, not invent a squad",
        },
        "notEstablished": [
            "A generic prebattle-only source for complete original enemy battle stats without starting a paid battle",
            "Any guarantee that mission preview definitions equal server-resolved BattleData",
            "Safe deep-clone semantics of all mode-specific mutable references",
            "All mode modules avoid service calls outside the simulation domain",
        ],
        "recommendedNextReadOnlyProbe": [
            "Read current StateController.CurrentState and its feature controller; do not invoke HandleOnBattleButtonClicked",
            "Report presence/type only for selected squad, saved transition and mission/mode; avoid account serialization",
            "Determine whether an already retained original BattleData/response matches this exact selected identity",
            "If absent, refuse faithful launch and identify missing payload; do not call formal StartBattle to fill it",
        ],
        "metadata": {name: declaration(name) for name in names},
    }


if __name__ == "__main__":
    print(json.dumps(report(), ensure_ascii=False, indent=2))
