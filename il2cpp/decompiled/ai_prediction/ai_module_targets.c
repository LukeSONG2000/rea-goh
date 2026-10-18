// ai_module_targets VA 0x1816a51a0 RVA 0x16a51a0

longlong FUN_1816a51a0(undefined8 param_1,int param_2,char param_3,longlong param_4)

{
  int iVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong lVar6;
  uint uStack_28;
  
  if (cRam0000000185a3c857 == '\0') {
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858213e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857c2450);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858107d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858213d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9410);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a47d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858213b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9428);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9648);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a47c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a47e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a47d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a47f0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c857 = '\x01';
  }
  lVar4 = FUN_1803a51b0(uRam00000001857a47d8);
  if (param_2 == 0) {
    lVar4 = FUN_1803a51b0(uRam00000001857a9428);
    FUN_182d09b30(lVar4,uRam00000001857a9410);
    if ((param_4 != 0) && (lVar4 != 0)) {
      FUN_18000d200(lVar4,*(undefined8 *)(param_4 + 0x18),uRam00000001857a9420);
code_r0x0001816a5626:
      if (param_3 == '\0') {
        return lVar4;
      }
      if (lVar4 != 0) {
        FUN_1836f5f00(lVar4,*(undefined8 *)(param_4 + 0x18),uRam00000001858107d8);
        return lVar4;
      }
    }
  }
  else if (param_2 == 1) {
    if (param_4 != 0) {
      lVar4 = *(longlong *)(param_4 + 0x30);
      if (*(int *)(lRam00000001857a47f0 + 0xe4) == 0) {
        FUN_180359940();
      }
      lVar6 = *(longlong *)(*(longlong *)(lRam00000001857a47f0 + 0xb8) + 8);
      if (lVar6 == 0) {
        if (*(int *)(lRam00000001857a47f0 + 0xe4) == 0) {
          FUN_180359940();
        }
        uVar5 = **(undefined8 **)(lRam00000001857a47f0 + 0xb8);
        lVar6 = FUN_1803a51b0(uRam00000001857a9648);
        FUN_180e639d0(lVar6,uVar5,uRam00000001857a47c8,0);
        *(longlong *)(*(longlong *)(lRam00000001857a47f0 + 0xb8) + 8) = lVar6;
      }
      if (lVar4 != 0) {
        lVar4 = FUN_1836f55d0(lVar4,lVar6,uRam00000001857c2450);
        goto code_r0x0001816a5626;
      }
    }
  }
  else if ((param_2 == 2) || (param_2 != 3)) {
    if (param_4 != 0) {
      uVar5 = *(undefined8 *)(param_4 + 0x20);
      lVar4 = FUN_1803a51b0(uRam00000001857a9428);
      FUN_1836f4d00(lVar4,uVar5,uRam00000001857a47d0);
      goto code_r0x0001816a5626;
    }
  }
  else {
    uVar5 = FUN_1803a51b0(uRam00000001858213b0);
    FUN_1800073e0(uVar5,uRam00000001858213d0);
    if (((lVar4 != 0) && (*(undefined8 *)(lVar4 + 0x10) = uVar5, param_4 != 0)) &&
       (lVar6 = *(longlong *)(param_4 + 0x30), lVar6 != 0)) {
      iVar1 = *(int *)(lVar6 + 0x1c);
      uStack_28 = 0;
      while (lVar6 != 0) {
        if ((iVar1 != *(int *)(lVar6 + 0x1c)) || (*(uint *)(lVar6 + 0x18) <= uStack_28)) {
          if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
          }
          if (lVar6 == 0) goto code_r0x0001816a567e;
          if (iVar1 != *(int *)(lVar6 + 0x1c)) goto code_r0x0001816a5677;
          lVar6 = *(longlong *)(param_4 + 0x30);
          uVar5 = FUN_1803a51b0(uRam00000001857a9648);
          FUN_180e639d0(uVar5,lVar4,uRam00000001857a47e8,0);
          if (lVar6 == 0) goto code_r0x0001816a565c;
          lVar4 = FUN_1836f55d0(lVar6,uVar5,uRam00000001857c2450);
          goto code_r0x0001816a5626;
        }
        lVar2 = *(longlong *)(lVar6 + 0x10);
        if (lVar2 == 0) goto code_r0x0001816a5672;
        if (*(uint *)(lVar2 + 0x18) <= uStack_28) goto code_r0x0001816a5662;
        lVar2 = *(longlong *)(lVar2 + 0x20 + (longlong)(int)uStack_28 * 8);
        uStack_28 = uStack_28 + 1;
        if (lVar2 == 0) goto code_r0x0001816a566d;
        if ((*(char *)(lVar2 + 0xc4) != '\0') && (*(char *)(lVar2 + 0xc5) == '\0')) {
          if (*(longlong *)(lVar4 + 0x10) == 0) goto code_r0x0001816a5668;
          FUN_180007460(*(longlong *)(lVar4 + 0x10),*(undefined4 *)(lVar2 + 0x90),
                        uRam00000001858213e0);
        }
      }
      goto code_r0x0001816a5683;
    }
  }
code_r0x0001816a565c:
  FUN_180352450();
code_r0x0001816a5662:
  FUN_180352460();
code_r0x0001816a5668:
  FUN_180352450();
code_r0x0001816a566d:
  FUN_180352450();
code_r0x0001816a5672:
  FUN_180352450();
code_r0x0001816a5677:
  FUN_183aed800(0);
code_r0x0001816a567e:
  FUN_180352450();
code_r0x0001816a5683:
  FUN_180352450();
  FUN_180352440();
  pcVar3 = (code *)swi(3);
  lVar4 = (*pcVar3)();
  return lVar4;
}

