// ai_default_result VA 0x1816a3bd0 RVA 0x16a3bd0

void FUN_1816a3bd0(longlong param_1,longlong param_2,undefined2 param_3)

{
  undefined8 uVar1;
  code *pcVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined1 uStackX_19;
  
  if (cRam0000000185a3c84f == '\0') {
    FUN_18035da50(0x1857a48e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96f0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c84f = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x20) != 0) &&
     (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308), lVar5 != 0)) {
    uVar3 = FUN_18366e0e0(lVar5,uRam00000001857a96f0);
    if ((*(longlong *)(param_1 + 0x20) != 0) &&
       (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308), lVar5 != 0)) {
      uVar4 = FUN_18366e0e0(lVar5,uRam00000001857a96f0);
      if (param_2 != 0) {
        if (*(longlong *)(param_2 + 0x40) != 0) {
          uStackX_19 = (undefined1)((ushort)param_3 >> 8);
          FUN_1816a4540(param_1,*(undefined8 *)(param_2 + 0x40),param_2,0,
                        *(undefined8 *)(param_2 + 0x20),uVar3,uStackX_19,0);
          FUN_1816a3d20(param_1,*(undefined8 *)(param_2 + 0x40),param_2,uVar4,(char)param_3,0);
        }
        lVar5 = FUN_1803a51b0(uRam00000001857a48e8);
        if (lVar5 != 0) {
          uVar1 = *(undefined8 *)(param_2 + 0x40);
          *(undefined8 *)(lVar5 + 0x10) = uVar3;
          *(undefined8 *)(lVar5 + 0x20) = uVar4;
          *(undefined8 *)(lVar5 + 0x18) = uVar1;
          return;
        }
      }
    }
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

