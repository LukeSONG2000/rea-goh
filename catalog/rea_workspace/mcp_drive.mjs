// Persistent MCP client for driving REA's Ghidra provider in ONE session.
// Usage: node mcp_drive.mjs <plan.json> <outdir>
// plan.json: [{ "tool": "...", "args": {...}, "save": "name.json" }, ...]
import { spawn } from 'node:child_process';
import { writeFileSync, mkdirSync, readFileSync } from 'node:fs';
import { join } from 'node:path';

const SDK = 'C:/Users/PPfavorite/AppData/Roaming/npm/node_modules/rea-agents/node_modules/@modelcontextprotocol/client/dist/index.mjs';
const STDIO = 'C:/Users/PPfavorite/AppData/Roaming/npm/node_modules/rea-agents/node_modules/@modelcontextprotocol/client/dist/stdio.mjs';
const { Client } = await import('file://' + SDK);
const { StdioClientTransport } = await import('file://' + STDIO);

const planPath = process.argv[2];
const outDir = process.argv[3];
if (!planPath || !outDir) { console.error('usage: node mcp_drive.mjs <plan.json> <outdir>'); process.exit(1); }
const plan = JSON.parse(readFileSync(planPath, 'utf-8'));
mkdirSync(outDir, { recursive: true });

const REA_CMD = 'C:\\Users\\PPfavorite\\AppData\\Roaming\\npm\\rea.cmd';
const env = {
  ...process.env,
  GHIDRA_INSTALL_DIR: 'F:\\PPfavorite\\Work\\goh\\il2cpp\\ghidra\\ghidra_12.1.4_PUBLIC',
  JAVA_HOME: 'D:\\Program Files\\Java\\jdk-24',
  REA_GHIDRA_STARTUP_TIMEOUT_MS: '14400000', // 4h: import+auto-analysis of a 95MB IL2CPP DLL
  GHIDRA_HEADLESS_MAXMEM: '12G',
};

const transport = new StdioClientTransport({
  command: 'cmd.exe',
  args: ['/d', '/c', REA_CMD, 'mcp'],
  env,
  stderr: 'pipe',
});
transport.stderr?.on('data', (d) => process.stderr.write('[rea-mcp] ' + d.toString()));

const client = new Client({ name: 'goh-rea-driver', version: '1.0.0' });
const CALL_OPTS = { timeout: 14_400_000, resetTimeoutOnProgress: false }; // 4h per call

console.log('[driver] connecting to rea mcp ...');
// stderr is line-buffered by Node; stdout may be block-buffered when redirected.
const t_connected = Date.now();
const hb = setInterval(() => {
  const s = Math.round((Date.now() - t_connected) / 1000);
  console.error(`[driver] heartbeat: connected ${s}s ago, plan ${step}/${plan.length} done`);
}, 60000);
await client.connect(transport);
console.log('[driver] connected');

// 0) save tool list
const tools = await client.listTools();
writeFileSync(join(outDir, '00_tools_list.json'), JSON.stringify(tools, null, 1));
console.log(`[driver] ${tools.tools.length} tools listed -> 00_tools_list.json`);

let step = 0;
for (const s of plan) {
  step += 1;
  const tag = String(step).padStart(2, '0');
  console.log(`[driver] step ${tag}: ${s.tool} ${JSON.stringify(s.args).slice(0, 200)}`);
  const t0 = Date.now();
  try {
    // NOTE: rea-agents bundles a modified MCP SDK: callTool(params, options) — options is the 2nd arg.
    const res = await client.callTool({ name: s.tool, arguments: s.args }, CALL_OPTS);
    const ms = ((Date.now() - t0) / 1000).toFixed(1);
    const content = res.content ?? [];
    let text = content.map((c) => (c.type === 'text' ? c.text : JSON.stringify(c))).join('\n');
    let parsed = null;
    try { parsed = JSON.parse(text); } catch { /* keep raw text */ }
    const payload = { _tool: s.tool, _args: s.args, _elapsed_s: ms, isError: !!res.isError, parsed, raw: parsed === null ? text : undefined };
    const file = join(outDir, s.save ?? `${tag}_${s.tool}.json`);
    writeFileSync(file, JSON.stringify(payload, null, 1));
    console.log(`[driver] step ${tag} done in ${ms}s -> ${file} (isError=${!!res.isError})`);
  } catch (e) {
    const ms = ((Date.now() - t0) / 1000).toFixed(1);
    console.error(`[driver] step ${tag} FAILED after ${ms}s: ${e.message}`);
    writeFileSync(join(outDir, `ERROR_${tag}_${s.tool}.json`), JSON.stringify({ tool: s.tool, args: s.args, error: String(e.message ?? e) }, null, 1));
  }
}

console.log('[driver] closing session ...');
clearInterval(hb);
try { await client.callTool({ name: 'close_binary', arguments: {} }, { timeout: 300000 }); console.log('[driver] closed'); }
catch (e) { console.error('[driver] close_binary:', e.message); }
try { await client.close(); } catch {}
process.exit(0);
