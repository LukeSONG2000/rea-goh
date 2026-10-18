// sweep_remaining VA 0x181650730 RVA 0x1650730

ulonglong FUN_181650730(undefined8 param_1,longlong param_2)

{
  code *pcVar1;
  ulonglong uVar2;
  
  if (param_2 != 0) {
    if (*(int *)(param_2 + 0x20) < 1) {
      uVar2 = 0x7fffffff;
    }
    else {
      uVar2 = (ulonglong)(uint)(*(int *)(param_2 + 0x20) - *(int *)(param_2 + 0x24));
    }
    if ((*(char *)(param_2 + 0x10) != '\0') && (1 < (int)uVar2)) {
      uVar2 = 1;
    }
    return uVar2;
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  uVar2 = (*pcVar1)();
  return uVar2;
}

