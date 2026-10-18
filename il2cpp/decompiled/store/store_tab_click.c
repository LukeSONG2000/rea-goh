// store_tab_click VA 0x181c97e60 RVA 0x1c97e60

void FUN_181c97e60(longlong param_1,undefined4 param_2,int param_3)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  
  if (param_3 == 6) {
    if (*(longlong *)(param_1 + 0x140) == 0) goto code_r0x000181c97f06;
    cVar3 = FUN_1811d3690(*(longlong *)(param_1 + 0x140),0x4d,1,0);
    if (cVar3 != '\0') {
      return;
    }
  }
  if (*(longlong *)(param_1 + 800) != 0) {
    FUN_18118ee90(*(longlong *)(param_1 + 800),CONCAT44(0xffffffff,param_2),1,1,0,1,0,0);
    if (*(longlong *)(param_1 + 0x2f8) != 0) {
      lVar1 = *(longlong *)(param_1 + 0x2f8);
      (**(code **)(lVar1 + 0x18))
                (*(undefined8 *)(lVar1 + 0x40),param_3,*(undefined8 *)(lVar1 + 0x28));
    }
    return;
  }
code_r0x000181c97f06:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

