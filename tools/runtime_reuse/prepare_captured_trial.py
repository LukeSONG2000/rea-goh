"""Prepare a reproducible original-runtime trial from explicit captured snapshots."""
import argparse
import hashlib
import json
from pathlib import Path
from protobuf_defaults import materialize, schema_from_dump


def read_capture(path, kind):
    path = Path(path)
    meta = json.loads(Path(str(path) + '.json').read_text(encoding='utf-8-sig'))
    data = path.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if meta.get('encoding') != 'direct-protobuf-v2':
        raise ValueError('Only direct v2 captures are supported; legacy clone data is lossy')
    if meta.get('kind') != kind:
        raise ValueError('Wrong capture type: ' + str(path))
    if meta.get('sha256') != digest or meta.get('bytes') != len(data):
        raise ValueError('Capture integrity mismatch: ' + str(path))
    return data, meta, {'path': str(path.resolve()), 'sha256': digest, 'bytes': len(data)}


def prepare(battle, inventory, dump, prefix, squad_type, mode, duration, auto, trace_ai=False, enemy_basic_demo=False, enemy_control=False, ai_priority_order=None, current_selection=False, experimental_prediction=False):
    if experimental_prediction:
        if mode != 'visual' or enemy_basic_demo or enemy_control or ai_priority_order is not None:
            raise ValueError('Prediction experiment requires an unmodified visual AI trial')
        trace_ai = True
    if current_selection and mode != 'visual':
        raise ValueError('Current selection requires visual trial')
    if ai_priority_order is not None:
        if mode != 'visual' or not isinstance(ai_priority_order, list) or not ai_priority_order or len(ai_priority_order) > 512 or any(type(n) is not int or n < 0 for n in ai_priority_order) or sorted(ai_priority_order) != list(range(len(ai_priority_order))):
            raise ValueError('AI priority requires a visual trial and a complete node-index permutation')
        trace_ai = True
    if enemy_basic_demo and enemy_control:
        raise ValueError('Choose either enemy demo or explicit enemy control')
    if (enemy_basic_demo or enemy_control) and mode != 'visual':
        raise ValueError('Enemy control demo requires the original visual loop')
    if trace_ai and mode != 'visual':
        raise ValueError('AI tracing requires the original visual loop')
    if mode not in ('headless', 'visual') or not 0 <= squad_type <= 255:
        raise ValueError('Explicit mode and saved squad type byte required')
    if not 15 <= duration <= 600 or (auto and mode != 'visual'):
        raise ValueError('Duration must be 15..600 seconds; auto is visual-only')
    raw_battle, metadata, battle_info = read_capture(battle, 'original-battle-proto-captured')
    raw_inventory, inventory_info = b'', None
    if not current_selection:
        if not inventory:
            raise ValueError('Saved squad trial requires an inventory capture')
        raw_inventory, _, inventory_info = read_capture(inventory, 'inventory-proto-captured')
    schemas = schema_from_dump(dump)
    battle_bytes = materialize(raw_battle, 'BattleData', schemas)
    inventory_bytes = b'' if current_selection else materialize(raw_inventory, 'Inventory', schemas)
    root = Path(__file__).resolve().parent
    modules = ['runtime_api.js', 'unity_scene_api.js', 'offline_socket_guard.js',
               'original_fixture.js', 'battle_proto.js', 'isolated_inventory.js',
               'captured_battle.js', 'definition_scope.js', 'selected_squad.js', 'captured_trial.js']
    if mode == 'visual':
        modules += ['battle_status_reader.js', 'battle_info_ui.js', 'battle_attributes_reader.js',
                    'battle_input.js', 'runtime_observer.js', 'trial_input.js']
    if trace_ai:
        modules += ['original_ai_trace.js']
    if ai_priority_order is not None:
        modules += ['original_ai_priority.js']
    if enemy_basic_demo or enemy_control:
        modules += ['enemy_input_gate.js']
    source = '\n'.join((root / n).read_text(encoding='utf-8-sig') for n in modules)
    for name, value in [('capturedBytes', battle_bytes), ('inventoryBytes', inventory_bytes)]:
        source += '\nglobalThis.' + name + '=new Uint8Array(' + json.dumps(list(value), separators=(',', ':')) + ');\n'
    source += 'globalThis.capturedMetadata=' + json.dumps(metadata) + ';\n'
    flags = 7 if mode == 'headless' else 6
    source += f'globalThis.createOriginalFixture=()=>createCapturedTrial({{flags:{flags},squadType:{squad_type},currentSelection:{str(current_selection).lower()},inventoryBytes,capturedBytes,capturedMetadata}});\n'
    diagnostic = {'kind': 'captured-trial-assembled', 'squadType': squad_type,
                  'mission': metadata.get('mission'), 'support': 'none',
                  'playerSource': 'explicit captured inventory snapshot; not a live refresh',
                  'battleSha256': battle_info['sha256'], 'inventorySha256': inventory_info['sha256'] if inventory_info else None}
    if current_selection:
        diagnostic.update(playerSource='current original PVE selection; private native copies', support='current original selection')
    source += 'globalThis.originalFixtureDiagnostics=f=>Object.assign(' + json.dumps(diagnostic) + ',{selectedSquad:f.selection?.selection,selectedSupport:f.selection?.support});\n'
    if mode == 'visual':
        source += 'globalThis.visualProbeOptions=' + json.dumps({
            'autoClick': auto, 'autoAfterClick': auto, 'traceAI': trace_ai, 'experimentalPrediction': experimental_prediction, 'enemyBasicDemo': enemy_basic_demo, 'enemyControl': enemy_control,
            'aiPriorityOrder': ai_priority_order, 'maxDurationMs': duration * 1000, 'capturePath': None}) + ';\n'
    source += (root / ('probe_visual.js' if mode == 'visual' else 'probe_simulation.js')).read_text(encoding='utf-8-sig')
    source_bytes = source.encode('utf-8')
    prefix = Path(prefix)
    js, manifest_path = Path(str(prefix) + '.js'), Path(str(prefix) + '.json')
    manifest = {'schemaVersion': 1, 'mode': mode, 'durationSeconds': duration, 'auto': auto,
                'informationDisplay': mode == 'visual', 'currentSelection': current_selection, 'experimentalPrediction': experimental_prediction,
                'aiPriorityOrder': ai_priority_order, 'battleCli': mode == 'visual', 'traceAI': trace_ai, 'enemyBasicDemo': enemy_basic_demo, 'enemyControl': enemy_control,
                'squadType': squad_type, 'mission': metadata.get('mission'),
                'battle': battle_info, 'inventory': inventory_info,
                'snapshotPolicy': 'Current original PVE squad/support plus matching captured enemy data; no inventory snapshot used' if current_selection else 'Use supplied captured snapshot; no automatic account refresh or spending',
                'schemaSha256': hashlib.sha256(Path(dump).read_bytes()).hexdigest(),
                'source': str(js.resolve()), 'sourceSha256': hashlib.sha256(source_bytes).hexdigest(),
                'originalAssemblySha256': '12cc254525a58aedea122a2967d0476da05b98ef0c219dc18cfc0b7207538db6'}
    if js.exists() or manifest_path.exists():
        raise FileExistsError('Use a new output prefix to preserve prior evidence')
    with js.open('xb') as target:
        target.write(source_bytes)
    with manifest_path.open('x', encoding='utf-8') as target:
        json.dump(manifest, target, ensure_ascii=False, indent=2)
    return manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--battle', required=True)
    parser.add_argument('--inventory', help='Required for saved squads; unused with --current-selection')
    parser.add_argument('--dump', required=True)
    parser.add_argument('--output-prefix', required=True)
    parser.add_argument('--squad-type', required=True, type=int)
    parser.add_argument('--mode', choices=['headless', 'visual'], default='visual')
    parser.add_argument('--duration', type=int, default=300)
    parser.add_argument('--auto', action='store_true')
    parser.add_argument('--current-selection', action='store_true', help='Use current matching PVE selection with support; visual only')
    parser.add_argument('--trace-ai', action='store_true')
    parser.add_argument('--experimental-prediction', action='store_true')
    parser.add_argument('--enemy-basic-demo', action='store_true')
    parser.add_argument('--enemy-control', action='store_true')
    parser.add_argument('--ai-priority-order', help='JSON file containing a complete original node-index permutation; isolated enemy only')
    a = parser.parse_args()
    print(json.dumps(prepare(a.battle, a.inventory, a.dump, a.output_prefix, a.squad_type, a.mode, a.duration, a.auto, a.trace_ai, a.enemy_basic_demo, a.enemy_control, json.loads(Path(a.ai_priority_order).read_text(encoding='utf-8-sig')) if a.ai_priority_order else None, a.current_selection, a.experimental_prediction), ensure_ascii=False))
