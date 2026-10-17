import assert from 'node:assert/strict';
import {acquireRuntimeSession} from './runtime_session_lock.mjs';
const id=`test-${process.pid}`;
const release=acquireRuntimeSession(id);
assert.throws(()=>acquireRuntimeSession(id),/already owned/);
release();const again=acquireRuntimeSession(id);again();
console.log('PASS: live owner cannot be replaced; graceful release permits next owner.');
