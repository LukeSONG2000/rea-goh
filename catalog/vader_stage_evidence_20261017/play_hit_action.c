// play_hit_action VA 0x18131d830 RVA 0x131d830

void FUN_18131d830(longlong param_1)

{
  uint uVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong lVar6;
  undefined8 uVar7;
  undefined8 unaff_RBX;
  
  if (cRam0000000185a3b148 == '\0') {
    FUN_18035da50(0x1857b3df8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c7d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a0d60);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a0d50);
    LOCK();
    UNLOCK();
    cRam0000000185a3b148 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x50) == 0) {
    if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x00018131da5d;
    uVar5 = FUN_183cbdb80(*(longlong *)(param_1 + 0x30),0);
    uVar7 = uRam00000001857a0d60;
  }
  else {
    lVar4 = FUN_183c9dd40(*(undefined8 *)(param_1 + 0x50),0);
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
    if (lVar4 != 0) {
      if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
        FUN_180359940();
      }
      if (*(longlong *)(lVar4 + 0x10) != 0) {
        lVar4 = FUN_182b23520(lVar4,uRam00000001857b3df8);
        if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
          FUN_180359940();
        }
        if (cRam0000000185a5056f == '\0') {
          FUN_18035da50(0x185814ea8);
          LOCK();
          UNLOCK();
          cRam0000000185a5056f = '\x01';
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
        if (lVar4 != 0) {
          if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
            FUN_180359940();
          }
          if (*(longlong *)(lVar4 + 0x10) != 0) {
            FUN_180fe84d0(lVar4,0);
          }
        }
        goto code_r0x00018131da49;
      }
    }
    if (*(longlong *)(param_1 + 0x30) == 0) {
code_r0x00018131da5d:
      FUN_180352450();
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
    uVar5 = FUN_183cbdb80(*(longlong *)(param_1 + 0x30),0);
    uVar7 = uRam00000001857a0d50;
  }
  FUN_182b6d2e0(param_1,uVar7,uVar5,0xffffffff,uRam000000018582c7d0);
code_r0x00018131da49:
  if (*(char *)(param_1 + 0x1b) != '\0') {
    return;
  }
  lVar4 = *(longlong *)(param_1 + 0x28);
  *(undefined2 *)(param_1 + 0x1a) = 0x100;
  if (lVar4 != 0) {
    if (cRam0000000185a4a45a == '\0') {
      FUN_18035da50(0x185767f68);
      LOCK();
      UNLOCK();
      cRam0000000185a4a45a = '\x01';
    }
    lVar6 = FUN_183cd5030(lVar4,0);
    lVar4 = lRam0000000185767f68;
    if (lVar6 != 0) {
      *(int *)(lVar6 + 0x1c) = *(int *)(lVar6 + 0x1c) + 1;
      lVar2 = *(longlong *)(lVar6 + 0x10);
      if (lVar2 != 0) {
        uVar1 = *(uint *)(lVar6 + 0x18);
        if (*(uint *)(lVar2 + 0x18) <= uVar1) {
          FUN_1836f50c0(lVar6,param_1,
                        *(undefined8 *)(*(longlong *)(*(longlong *)(lVar4 + 0x20) + 0xc0) + 0x70),
                        lVar6,unaff_RBX);
          return;
        }
        *(uint *)(lVar6 + 0x18) = uVar1 + 1;
        if (*(uint *)(lVar2 + 0x18) <= uVar1) {
          FUN_180352460();
          pcVar3 = (code *)swi(3);
          (*pcVar3)();
          return;
        }
        *(longlong *)(lVar2 + 0x20 + (longlong)(int)uVar1 * 8) = param_1;
        return;
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

