#!/bin/bash
cd /f/PPfavorite/Work/goh/il2cpp
for i in $(seq 1 40); do
  rm -rf redux
  if timeout 240 git clone --depth 1 --filter=blob:none --sparse https://github.com/LukeFZ/Il2CppInspectorRedux.git redux 2>&1; then
    cd redux
    for j in 1 2 3 4 5 6 7 8; do
      if timeout 240 git sparse-checkout set Il2CppInspector.Common/Metadata Il2CppInspector.Common/Il2CppBinary 2>&1; then echo "CLONE_OK"; ls Il2CppInspector.Common/Metadata/ | head; exit 0; fi
      sleep 30
    done
  fi
  echo "attempt $i failed, sleeping 60s"
  sleep 60
done
echo "CLONE_GAVE_UP"
