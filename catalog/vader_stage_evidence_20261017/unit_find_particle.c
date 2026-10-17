// unit_find_particle VA 0x180fe1040 RVA 0xfe1040

undefined8 FUN_180fe1040(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  uint uVar7;
  
  if (cRam0000000185a39f00 == '\0') {
    FUN_18035da50(0x1857a8de8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8de0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8dd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8dc8);
    LOCK();
    UNLOCK();
    cRam0000000185a39f00 = '\x01';
  }
  lVar4 = FUN_1803a51b0(uRam00000001857a8dc8);
  if (lVar4 != 0) {
    *(undefined8 *)(lVar4 + 0x10) = param_2;
    lVar1 = *(longlong *)(param_1 + 0x28);
    lVar5 = FUN_1803a51b0(uRam00000001857a8de0);
    FUN_180e639d0(lVar5,lVar4,uRam00000001857a8dd8,0);
    if (lVar1 != 0) {
      if (lVar5 == 0) {
        FUN_183aecdf0(8,0,uRam00000001857a8de8);
        pcVar2 = (code *)swi(3);
        uVar6 = (*pcVar2)();
        return uVar6;
      }
      uVar7 = 0;
      if (0 < *(int *)(lVar1 + 0x18)) {
        do {
          lVar4 = *(longlong *)(lVar1 + 0x10);
          if (lVar4 == 0) {
code_r0x000183673296:
            FUN_180352450();
            pcVar2 = (code *)swi(3);
            uVar6 = (*pcVar2)();
            return uVar6;
          }
          if (*(uint *)(lVar4 + 0x18) <= uVar7) {
DAT_183673290:
            FUN_180352460();
            pcVar2 = (code *)swi(3);
            uVar6 = (*pcVar2)();
            return uVar6;
          }
          cVar3 = (**(code **)(lVar5 + 0x18))
                            (*(undefined8 *)(lVar5 + 0x40),
                             *(undefined8 *)(lVar4 + 0x20 + (longlong)(int)uVar7 * 8),
                             *(undefined8 *)(lVar5 + 0x28));
          if (cVar3 != '\0') {
            lVar4 = *(longlong *)(lVar1 + 0x10);
            if (lVar4 == 0) goto code_r0x000183673296;
            if (uVar7 < *(uint *)(lVar4 + 0x18)) {
              return *(undefined8 *)(lVar4 + 0x20 + (longlong)(int)uVar7 * 8);
            }
            goto DAT_183673290;
          }
          uVar7 = uVar7 + 1;
        } while ((int)uVar7 < *(int *)(lVar1 + 0x18));
      }
      return 0;
    }
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar6 = (*pcVar2)();
  return uVar6;
}

