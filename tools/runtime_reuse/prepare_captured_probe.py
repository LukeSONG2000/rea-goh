"""Prepare a reproducible original-runtime conversion probe from a direct v2 capture."""
import argparse
import hashlib
import json
from pathlib import Path
from protobuf_defaults import materialize, schema_from_dump


def prepare(capture, prefix, dump):
    capture, prefix, dump = Path(capture), Path(prefix), Path(dump)
    metadata = json.loads(Path(str(capture) + '.json').read_text(encoding='utf-8-sig'))
    if metadata.get('encoding') != 'direct-protobuf-v2':
        raise ValueError('Requires direct v2 capture; legacy DeepClone data is not recoverable')
    raw = capture.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if digest != metadata.get('sha256') or len(raw) != metadata.get('bytes'):
        raise ValueError('Capture integrity mismatch')
    normalized = materialize(raw, 'BattleData', schema_from_dump(dump))
    metadata['normalization'] = {
        'sourceSha256': digest,
        'normalizedSha256': hashlib.sha256(normalized).hexdigest(),
        'schemaSha256': hashlib.sha256(dump.read_bytes()).hexdigest(),
        'method': 'materialize omitted original struct DefaultValue fields',
    }
    modules = ['runtime_api.js', 'unity_scene_api.js', 'original_fixture.js',
               'battle_proto.js', 'captured_battle.js']
    root = Path(__file__).resolve().parent
    source = '\n'.join((root / name).read_text(encoding='utf-8') for name in modules)
    source += '\nconst capturedBytes=new Uint8Array(' + json.dumps(list(normalized)) + ');\n'
    source += 'const capturedMetadata=' + json.dumps(metadata) + ';\n'
    source += (root / 'probe_captured_squad.js').read_text(encoding='utf-8')
    outputs = {Path(str(prefix) + '.pb'): normalized,
               Path(str(prefix) + '.json'): (json.dumps(metadata, indent=2) + '\n').encode(),
               Path(str(prefix) + '.js'): source.encode()}
    if any(path.exists() for path in outputs):
        raise FileExistsError('Use a new output prefix; existing evidence is preserved')
    for path, contents in outputs.items():
        with path.open('xb') as target:
            target.write(contents)
    return metadata['normalization']


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture', required=True)
    parser.add_argument('--output-prefix', required=True)
    parser.add_argument('--dump', required=True)
    args = parser.parse_args()
    print(json.dumps(prepare(args.capture, args.output_prefix, args.dump)))
