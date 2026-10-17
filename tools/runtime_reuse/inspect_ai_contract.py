"""Local metadata inspection only; no process access, native calls or writes.
Run: py tools/runtime_reuse/inspect_ai_contract.py
"""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'il2cpp/dump/cs/il2cpp.cs'


def inspect():
    lines = SOURCE.read_text(encoding='utf-8').splitlines()
    wanted = {
        'BattleAbilityAIController': ['GetAbilityAIResult', 'ProcessResult', 'GetAbilityDecisionTree', 'GetDefaultAIResult', 'GetEnemyTargets'],
        'BattleUnitController': ['PerformAISuggestedMove', 'ManuallyActivateMove'],
        'BattleInputProvider': ['DoMove'],
        'AbilityDecisionTreeProvider': ['Get()', 'GetPriorityCheck', 'GetUltimatePriorityCheck'],
        'AbilityDecisionTreeTranslator': ['Translate(', 'EnsureLastNodeSelectsBasicAttackOnFailure'],
    }
    entries = []
    for typ, selectors in wanted.items():
        start = next(i for i, line in enumerate(lines) if line.startswith('public class '+typ+' '))
        end = next(i for i in range(start+1, len(lines)) if lines[i] == '}')
        for i in range(start, end):
            line = lines[i].strip()
            if '(' in line and '; // 0x' in line and any(s in line for s in selectors):
                address = re.search(r'// 0x([0-9A-Fa-f]+)', line)
                if address:
                    entries.append({'type': typ, 'signature': line.split('; //')[0],
                                    'rva': hex(int(address[1], 16)-0x180000000), 'line': i+1,
                                    'evidence': 'metadata; body not implied'})
    body = ROOT / 'logs/effect_phase2/ai_decision.c'
    assert '0x5deece66d' in body.read_text(), 'Expected original RNG body missing'
    return {
        'metadata': str(SOURCE), 'entrypoints': entries,
        'observation': {
            'mode': 'Observe natural native call only; never invoke decision for preview.',
            'entryRva': '0x16a2ec0',
            'contextFields': {'PreferredTarget':'0x10','Self':'0x18','EnemyTargetOptions':'0x20','Enemies':'0x28','Allies':'0x30','SpecialAbilities':'0x38','PrimaryAbility':'0x40','Chance':'0x48'},
            'resultFields': {'EnemyTargets':'0x10','Ability':'0x18','AllyTargets':'0x20'},
            'lifetime': 'Copy ids and target identities before IDisposable.Dispose RVA0x16a4bf0; lists are pooled.',
            'label': 'Observed original AI selection, not a future prediction',
        },
        'rng': {
            'body': str(body), 'bodyRva':'0x16a30c0',
            'battleContextFields': {'BattleRandom':'0x158','PlayerAbilitySelectionRandom':'0x160','OpponentAbilitySelectionRandom':'0x168','PlayerTargetRandom':'0x170','OpponentTargetRandom':'0x178'},
            'JavaRandomSeed': {'type':'Int64','offset':'0x10'},
            'confirmed': 'Node evaluation mutates side ability RNG: nextInt(100), then another draw if first < context.Chance. Node Chance is compared to second draw. Rejection sampling can advance seed more than once per draw.',
            'forbiddenPreview': 'Do not invoke live AI or restore just its seed after invocation; not a complete rollback.',
            'unknown': 'Full target/default-selection draw order requires ProcessResult/GetDefaultAIResult/GetEnemyTargets bodies.'
        },
        'priority': {
            'treeFields': {'_rootNode':'0x10','_id':'0x18','ShouldEvaluateBasicAbility':'0x20'},
            'nodeFields': {'_decisionType':'0x10','_decisionTarget':'0x14','_decisionTargetExcludeSelf':'0x18','_params':'0x20','_paramModifier':'0x28','SuccessResult':'0x30','FailureResult':'0x38','Chance':'0x40','Negate':'0x44'},
            'resultFields': {'_nextNode':'0x10','_choosenAbilityTag':'0x18','_resultTarget':'0x20','SelectBasic':'0x24'},
            'battleDataTreeIds': {'PlayerAbilityDecisionTreeId':'0x58','OpponentAbilityDecisionTreeId':'0x60'},
            'meaning': 'Priority is tree topology/branch outcomes and ability tags; no generic priority integer on node. Provider method names do not prove traversal order.',
            'isolation': 'Deep-copy the selected tree and all reachable nodes/results into a separate simulation context. Never alter GameDataDomain shared definitions or live BattleData tree ids. Compare unchanged isolated replay first.',
            'pending': 'Provider.Get construction order, tree-id fallback and exact ProcessResult behavior need bodies.'
        },
        'inputBoundary': {
            'battleContextInputProviderOffset':'0x120', 'doMoveRva':'0x1a64420',
            'performAISuggestedMoveRva':'0x1bc2f40','manuallyActivateMoveRva':'0x1bc37f0',
            'enemyControl': 'Inspect provider and current actor ownership; do not change unit allegiance or swap player/opponent stats. Routing enemy input requires DoMove and PerformAISuggestedMove bodies before implementation.'
        },
        'noRuntimeAccess': True,
    }


if __name__ == '__main__':
    print(json.dumps(inspect(), ensure_ascii=False, separators=(',', ':')))
