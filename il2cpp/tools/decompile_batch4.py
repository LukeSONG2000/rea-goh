# Batch decompile: GetContextValue + GetModifyEffectAmount (wrapper/private)
# + ProcessDamageModification, via the existing Ghidra project.
import os
os.environ.setdefault("GHIDRA_INSTALL_DIR",
                      "F:/PPfavorite/Work/goh/il2cpp/ghidra/ghidra_12.1.4_PUBLIC")
import pyghidra
pyghidra.start()

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

proj_dir = "F:/PPfavorite/Work/goh/il2cpp/ghidra/projects"
with pyghidra.open_program("E:/Games/EA Games/SWGoH/GameAssembly.dll",
                           project_location=proj_dir,
                           project_name="goh_assembly", analyze=False) as flat:
    program = flat.getCurrentProgram()
    fm = program.getFunctionManager()
    di = DecompInterface()
    di.openProgram(program)
    mon = ConsoleTaskMonitor()

    targets = [
        (0x18136A570, "get_context_value"),
        (0x181A73880, "get_modify_effect_amount"),
        (0x181A73980, "get_modify_effect_amount_priv"),
        (0x181365FC0, "process_damage_modification"),
    ]
    out_dir = "F:/PPfavorite/Work/goh/il2cpp/dump/decompiled"
    for va, name in targets:
        addr = program.getAddressFactory().getDefaultAddressSpace().getAddress(va)
        fn = fm.getFunctionContaining(addr)
        if fn is None:
            print(f"NO FUNC at {va:#x} for {name}")
            continue
        res = di.decompileFunction(fn, 180, mon)
        if not res.decompileCompleted():
            print(f"FAIL {name}: {res.getErrorMessage()}")
            continue
        text = "// ==== %s @ 0x%x ====\n\n%s" % (name, va, res.getDecompiledFunction().getC())
        with open(os.path.join(out_dir, name + ".c"), "w") as f:
            f.write(text)
        print(f"OK {name}: {len(text)} bytes")
