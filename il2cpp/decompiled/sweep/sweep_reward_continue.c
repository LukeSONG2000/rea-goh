// sweep_reward_continue VA 0x181650770 RVA 0x1650770

void FUN_181650770(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong *plVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined *puVar9;
  undefined *puVar10;
  longlong lVar11;
  bool bVar12;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  lVar2 = *(longlong *)(param_1 + 0xb8);
  if (lVar2 == 0) {
    FUN_180352450();
    pcVar3 = (code *)swi(3);
    (*pcVar3)();
    return;
  }
  if (cRam0000000185a3ac3a == '\0') {
    FUN_18035da50(0x185788a00);
    LOCK();
    UNLOCK();
    cRam0000000185a3ac3a = '\x01';
  }
  uStack_18 = 0x146;
  uStack_10 = 0;
  FUN_18001f4d0(lVar2,&uStack_18);
  if (cRam0000000185a3c646 == '\0') {
    FUN_18035da50(0x1857fe308,0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd730);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd738);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd728);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a3c646 = '\x01';
  }
  lVar2 = *(longlong *)(param_1 + 0xe8);
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5056e == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5056e = '\x01';
  }
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5054a == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5054a = '\x01';
  }
  if (lVar2 != 0) {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar2 + 0x10) != 0) {
      lVar2 = *(longlong *)(param_1 + 0xe8);
      lVar4 = FUN_1803a51b0(lRam000000018580d858);
      lVar11 = lRam00000001857fd730;
      puVar9 = *(undefined **)(lRam00000001857fd730 + 8);
      *(undefined **)(lVar4 + 0x10) = puVar9;
      *(longlong *)(lVar4 + 0x28) = lVar11;
      *(longlong *)(lVar4 + 0x20) = param_1;
      cVar1 = *(char *)(lVar11 + 0x52);
      *(longlong *)(lVar4 + 0x40) = lVar4;
      if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
        *(longlong *)(lVar4 + 0x40) = param_1;
      }
      else {
        puVar9 = &UNK_180006440;
      }
      *(undefined **)(lVar4 + 0x18) = puVar9;
      *(code **)(lVar4 + 0x38) = FUN_1800063e0;
      if (lVar2 != 0) {
        FUN_181388640(lVar2,lVar4,0);
        lVar2 = *(longlong *)(param_1 + 0xe8);
        uVar5 = FUN_1803a51b0(uRam00000001857fe308);
        FUN_180c84ee0(uVar5,param_1,uRam00000001857fd738,0);
        if (lVar2 != 0) {
          FUN_181388780(lVar2,uVar5,0);
          lVar2 = *(longlong *)(param_1 + 0xe8);
          lVar4 = FUN_1803a51b0(lRam000000018580d858);
          lVar11 = lRam00000001857fd728;
          puVar9 = *(undefined **)(lRam00000001857fd728 + 8);
          *(undefined **)(lVar4 + 0x10) = puVar9;
          *(longlong *)(lVar4 + 0x28) = lVar11;
          *(longlong *)(lVar4 + 0x20) = param_1;
          cVar1 = *(char *)(lVar11 + 0x52);
          *(longlong *)(lVar4 + 0x40) = lVar4;
          if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) || (puVar10 = &UNK_180006440, cVar1 != '\0'))
          {
            *(longlong *)(lVar4 + 0x40) = param_1;
            puVar10 = puVar9;
          }
          *(undefined **)(lVar4 + 0x18) = puVar10;
          *(code **)(lVar4 + 0x38) = FUN_1800063e0;
          if (lVar2 != 0) {
            if (cRam0000000185a3b3f3 == '\0') {
              FUN_18035da50(0x18580d858);
              LOCK();
              UNLOCK();
              cRam0000000185a3b3f3 = '\x01';
            }
            lVar11 = *(longlong *)(lVar2 + 0x110);
            do {
              plVar6 = (longlong *)FUN_183b250a0(lVar11,lVar4,0);
              plVar8 = (longlong *)0x0;
              if (plVar6 != (longlong *)0x0) {
                if (*plVar6 == lRam000000018580d858) {
                  plVar8 = plVar6;
                }
                if (plVar8 == (longlong *)0x0) {
                  FUN_180352e70(plVar6);
                  pcVar3 = (code *)swi(3);
                  (*pcVar3)();
                  return;
                }
              }
              LOCK();
              lVar7 = *(longlong *)(lVar2 + 0x110);
              bVar12 = lVar11 == lVar7;
              if (bVar12) {
                *(longlong *)(lVar2 + 0x110) = (longlong)plVar8;
                lVar7 = lVar11;
              }
              UNLOCK();
              lVar11 = lVar7;
            } while (!bVar12);
            if (*(longlong *)(param_1 + 0xe8) != 0) {
              FUN_1812b9d00(*(longlong *)(param_1 + 0xe8),0);
              *(undefined8 *)(param_1 + 0xe8) = 0;
              return;
            }
          }
        }
      }
      FUN_180352450();
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
  }
  return;
}

