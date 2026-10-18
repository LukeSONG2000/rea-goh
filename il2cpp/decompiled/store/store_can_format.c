// store_can_format VA 0x181c91f00 RVA 0x1c91f00

ulonglong FUN_181c91f00(longlong param_1,longlong param_2)

{
  code *pcVar1;
  longlong lVar2;
  ulonglong uVar3;
  
  if ((param_2 == 0) || (*(longlong *)(param_1 + 0x28) == 0)) {
    FUN_180352450();
    pcVar1 = (code *)swi(3);
    uVar3 = (*pcVar1)();
    return uVar3;
  }
  lVar2 = FUN_18114c870(*(longlong *)(param_1 + 0x28),*(undefined8 *)(param_2 + 0x18),0);
  uVar3 = 0;
  if (((lVar2 != 0) && (uVar3 = *(ulonglong *)(lVar2 + 0x20), uVar3 != 0)) &&
     (*(int *)(uVar3 + 0x10) != 0)) {
    return CONCAT71((int7)(uVar3 >> 8),1);
  }
  return uVar3 & 0xffffffffffffff00;
}

