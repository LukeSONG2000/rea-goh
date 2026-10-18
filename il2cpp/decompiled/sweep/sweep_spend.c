// sweep_spend VA 0x18164f580 RVA 0x164f580

void FUN_18164f580(longlong param_1,undefined4 param_2)

{
  undefined1 uVar1;
  char cVar2;
  int iVar3;
  longlong lVar4;
  code *pcVar5;
  undefined8 uVar6;
  longlong lVar7;
  undefined *puVar8;
  longlong lVar9;
  longlong lVar10;
  undefined8 uVar11;
  uint uStack_40;
  
  if (cRam0000000185a3c63b == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd7a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3490);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd770);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd778);
    LOCK();
    UNLOCK();
    cRam0000000185a3c63b = '\x01';
  }
  if (*(char *)(param_1 + 0x125) == '\0') {
code_r0x00018164f76c:
    *(undefined4 *)(param_1 + 0x120) = param_2;
    if (*(longlong *)(param_1 + 0x118) != 0) {
      FUN_180fce590(*(undefined8 *)(param_1 + 0x118),0);
    }
    if (*(longlong *)(param_1 + 0x28) != 0) {
      uVar6 = FUN_180fcd800(*(longlong *)(param_1 + 0x28),2,param_1,0);
      *(undefined8 *)(param_1 + 0x118) = uVar6;
      lVar4 = *(longlong *)(param_1 + 0x20);
      uVar6 = *(undefined8 *)(param_1 + 0xf0);
      uVar1 = *(undefined1 *)(param_1 + 0x110);
      lVar7 = FUN_1803a51b0(uRam000000018580d858);
      lVar10 = lRam00000001857fd770;
      puVar8 = *(undefined **)(lRam00000001857fd770 + 8);
      *(undefined **)(lVar7 + 0x10) = puVar8;
      *(longlong *)(lVar7 + 0x28) = lVar10;
      *(longlong *)(lVar7 + 0x20) = param_1;
      cVar2 = *(char *)(lVar10 + 0x52);
      *(longlong *)(lVar7 + 0x40) = lVar7;
      if (((*(byte *)(lVar10 + 0x4c) & 0x10) == 0) || (cVar2 != '\0')) {
        *(longlong *)(lVar7 + 0x40) = param_1;
      }
      else {
        puVar8 = &UNK_180006440;
      }
      *(undefined **)(lVar7 + 0x18) = puVar8;
      *(code **)(lVar7 + 0x38) = FUN_1800063e0;
      lVar9 = FUN_1803a51b0(uRam00000001857fd7a0);
      lVar10 = lRam00000001857fd778;
      puVar8 = *(undefined **)(lRam00000001857fd778 + 8);
      *(undefined **)(lVar9 + 0x10) = puVar8;
      *(longlong *)(lVar9 + 0x28) = lVar10;
      *(longlong *)(lVar9 + 0x20) = param_1;
      cVar2 = *(char *)(lVar10 + 0x52);
      *(longlong *)(lVar9 + 0x40) = lVar9;
      if (((*(byte *)(lVar10 + 0x4c) & 0x10) == 0) || (cVar2 != '\x02')) {
        *(longlong *)(lVar9 + 0x40) = param_1;
      }
      else {
        puVar8 = &UNK_180013380;
      }
      *(undefined **)(lVar9 + 0x18) = puVar8;
      *(code **)(lVar9 + 0x38) = FUN_180013320;
      if (lVar4 != 0) {
        if (cRam0000000185a3f120 == '\0') {
          FUN_18035da50(0x1857f50c8);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x18582a470);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857f50d8);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857f50b8);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857f50c0);
          LOCK();
          UNLOCK();
          cRam0000000185a3f120 = '\x01';
        }
        lVar10 = FUN_1803a51b0(uRam00000001857f50c0);
        if (lVar10 != 0) {
          *(longlong *)(lVar10 + 0x10) = lVar4;
          *(longlong *)(lVar10 + 0x18) = lVar7;
          *(longlong *)(lVar10 + 0x20) = lVar9;
          lVar4 = *(longlong *)(lVar4 + 0x60);
          uVar11 = FUN_1803a51b0(uRam00000001857f50c8);
          FUN_180c7c3a0(uVar11,lVar10,uRam00000001857f50d8,0);
          lVar9 = FUN_1803a51b0(uRam000000018582a470);
          lVar7 = lRam00000001857f50b8;
          puVar8 = *(undefined **)(lRam00000001857f50b8 + 8);
          *(undefined **)(lVar9 + 0x10) = puVar8;
          *(longlong *)(lVar9 + 0x28) = lVar7;
          *(longlong *)(lVar9 + 0x20) = lVar10;
          cVar2 = *(char *)(lVar7 + 0x52);
          *(longlong *)(lVar9 + 0x40) = lVar9;
          if (((*(byte *)(lVar7 + 0x4c) & 0x10) == 0) || (cVar2 != '\x02')) {
            *(longlong *)(lVar9 + 0x40) = lVar10;
          }
          else {
            puVar8 = &UNK_180013380;
          }
          *(undefined **)(lVar9 + 0x18) = puVar8;
          *(code **)(lVar9 + 0x38) = FUN_180013320;
          if (lVar4 != 0) {
            FUN_180f8ded0(lVar4,uVar6,uVar1,uVar11,lVar9,0);
            return;
          }
        }
      }
    }
  }
  else {
    lVar4 = *(longlong *)(param_1 + 0xf0);
    if (lVar4 != 0) {
      iVar3 = *(int *)(lVar4 + 0x1c);
      uStack_40 = 0;
      while (lVar4 != 0) {
        if ((iVar3 != *(int *)(lVar4 + 0x1c)) || (*(uint *)(lVar4 + 0x18) <= uStack_40)) {
          if ((*(byte *)(*(longlong *)(lRam00000001857a34c0 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam00000001857a34c0 + 0x20));
          }
          if (lVar4 == 0) goto code_r0x00018164f9cf;
          if (iVar3 == *(int *)(lVar4 + 0x1c)) goto code_r0x00018164f76c;
          goto code_r0x00018164f9c8;
        }
        lVar10 = *(longlong *)(lVar4 + 0x10);
        if (lVar10 == 0) goto code_r0x00018164f9c3;
        if (*(uint *)(lVar10 + 0x18) <= uStack_40) goto code_r0x00018164f9b8;
        lVar10 = *(longlong *)(lVar10 + 0x20 + (longlong)(int)uStack_40 * 8);
        uStack_40 = uStack_40 + 1;
        if (lVar10 == 0) goto code_r0x00018164f9be;
        *(undefined4 *)(lVar10 + 0x20) = param_2;
      }
      goto code_r0x00018164f9d4;
    }
  }
  FUN_180352450();
code_r0x00018164f9b8:
  FUN_180352460();
code_r0x00018164f9be:
  FUN_180352450();
code_r0x00018164f9c3:
  FUN_180352450();
code_r0x00018164f9c8:
  FUN_183aed800(0);
code_r0x00018164f9cf:
  FUN_180352450();
code_r0x00018164f9d4:
  FUN_180352450();
  FUN_180352440();
  pcVar5 = (code *)swi(3);
  (*pcVar5)();
  return;
}

