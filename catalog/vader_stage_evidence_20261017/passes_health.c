// passes_health VA 0x181bb2f80 RVA 0x1bb2f80

bool FUN_181bb2f80(undefined8 param_1,char param_2,longlong param_3)

{
  code *pcVar1;
  undefined1 uVar2;
  
  if (param_2 == '\0') {
    if (param_3 == 0) {
code_r0x000181bb2fbf:
      FUN_180352450();
      pcVar1 = (code *)swi(3);
      uVar2 = (*pcVar1)();
      return (bool)uVar2;
    }
    if (*(int *)(param_3 + 0xc0) < 1) {
      return false;
    }
  }
  else if (param_2 == '\x01') {
    if (param_3 != 0) {
      return *(int *)(param_3 + 0xc0) < 1;
    }
    goto code_r0x000181bb2fbf;
  }
  return true;
}

