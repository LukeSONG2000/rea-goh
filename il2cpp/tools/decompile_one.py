# Decompile one function by VA: py decompile_one.py 0x180FDE5C0 stat_calculated_value
import os, sys
os.environ.setdefault("GHIDRA_INSTALL_DIR",
                      "F:/PPfavorite/Work/goh/il2cpp/ghidra/ghidra_12.1.4_PUBLIC")
import pyghidra
pyghidra.start()

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

va = int(sys.argv[1], 16)
name = sys.argv[2]

with pyghidra.open_program("E:/Games/EA Games/SWGoH/GameAssembly.dll",
                           project_location="F:/PPfavorite/Work/goh/il2cpp/ghidra/projects",
                           project_name="goh_assembly", analyze=False) as flat:
    program = flat.getCurrentProgram()
    fm = program.getFunctionManager()
    di = DecompInterface()
    di.openProgram(program)
    mon = ConsoleTaskMonitor()
    addr = program.getAddressFactory().getDefaultAddressSpace().getAddress(va)
    fn = fm.getFunctionContaining(addr)
    if fn is None:
        sys.exit(f"NO FUNC at {va:#x}")
    res = di.decompileFunction(fn, 300, mon)
    if not res.decompileCompleted():
        sys.exit(f"FAIL {name}: {res.getErrorMessage()}")
    out = f"F:/PPfavorite/Work/goh/il2cpp/dump/decompiled/{name}.c"
    with open(out, "w") as f:
        f.write("// ==== %s @ 0x%x ====\n\n%s" % (name, va,
                                                  res.getDecompiledFunction().getC()))
    print(f"OK {name}: {os.path.getsize(out)} bytes")
