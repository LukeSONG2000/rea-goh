// clear_group_effect VA 0x181a6dbc0 RVA 0x1a6dbc0

void FUN_181a6dbc0(longlong param_1,longlong param_2,uint param_3,undefined8 param_4,
                  undefined1 param_5,undefined8 param_6)

{
  longlong lVar1;
  longlong lVar2;
  code *pcVar3;
  
  if (cRam0000000185a3e094 == '\0') {
    FUN_18035da50(0x1857855c8);
    LOCK();
    UNLOCK();
    cRam0000000185a3e094 = '\x01';
  }
  if (param_2 != 0) {
    if (*(uint *)(param_2 + 0x18) <= param_3) {
      FUN_183aeda10(0);
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
    lVar1 = *(longlong *)(param_2 + 0x10);
    if (lVar1 != 0) {
      if (*(uint *)(lVar1 + 0x18) <= param_3) {
        FUN_180352460();
        pcVar3 = (code *)swi(3);
        (*pcVar3)();
        return;
      }
      lVar1 = *(longlong *)(lVar1 + 0x20 + (longlong)(int)param_3 * 8);
      FUN_181a71b40(param_1,param_2,param_3,param_4,param_5,param_6,0);
      if (*(longlong *)(param_1 + 0x10) != 0) {
        if (lVar1 == 0) goto code_r0x000181a6dc92;
        if (*(longlong *)(lVar1 + 0x38) != 0) {
          lVar2 = *(longlong *)(param_1 + 0x10);
          (**(code **)(lVar2 + 0x18))
                    (*(undefined8 *)(lVar2 + 0x40),*(undefined8 *)(lVar1 + 0x38),
                     *(undefined1 *)(*(longlong *)(lVar1 + 0x38) + 0x10),
                     *(undefined8 *)(lVar2 + 0x28));
        }
      }
      return;
    }
  }
code_r0x000181a6dc92:
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

