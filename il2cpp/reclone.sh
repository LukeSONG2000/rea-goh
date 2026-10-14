#!/bin/bash
cd /f/PPfavorite/Work/goh/il2cpp
for i in $(seq 1 30); do
  rm -rf redux2
  if timeout 500 git clone --depth 1 --single-branch https://github.com/LukeFZ/Il2CppInspectorRedux.git redux2 2>&1 | tail -1; then
    N=$(find redux2 -name "*.cs" -not -path "*/.git/*" 2>/dev/null | wc -l)
    echo "attempt $i: $N cs files"
    [ "$N" -gt 500 ] && { echo "RECLONE_OK"; break; }
  fi
  echo "reclone retry $i"; sleep 50
done
