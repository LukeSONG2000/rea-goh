// start_encounter_callback VA 0x181a5ce70 RVA 0x1a5ce70

void FUN_181a5ce70(longlong param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  longlong lVar4;
  code *pcVar5;
  char cVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 uVar9;
  longlong lVar10;
  undefined8 uVar11;
  longlong lVar12;
  uint uStack_38;
  
  if (cRam0000000185a3e022 == '\0') {
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3910);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9638);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e90);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810ec0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810eb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185830d88);
    LOCK();
    UNLOCK();
    cRam0000000185a3e022 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x10) == 0) ||
     (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x28), lVar7 == 0)) {
code_r0x000181a5d56d:
    FUN_180352450();
code_r0x000181a5d573:
    FUN_180352460();
code_r0x000181a5d579:
    FUN_180352460();
code_r0x000181a5d57e:
    FUN_180352450();
code_r0x000181a5d583:
    FUN_180352450();
code_r0x000181a5d588:
    FUN_180352450();
code_r0x000181a5d58d:
    FUN_180352450();
code_r0x000181a5d592:
    FUN_183aed800(0);
code_r0x000181a5d599:
    FUN_180352450();
  }
  else {
    lVar7 = FUN_18199d520(lVar7,0);
    if (lVar7 == 0) goto code_r0x000181a5d56d;
    if (0 < *(int *)(lVar7 + 0x18)) {
      if (((*(longlong *)(param_1 + 0x10) == 0) ||
          (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x28), lVar7 == 0)) ||
         (*(longlong *)(lVar7 + 0x10) == 0)) goto code_r0x000181a5d56d;
      cVar6 = FUN_18199d780(lVar7,*(undefined8 *)(*(longlong *)(lVar7 + 0x10) + 0xd00),0);
      if (cVar6 == '\0') {
        if (*(int *)(lRam0000000185830d88 + 0xe4) == 0) {
          FUN_180359940();
        }
        FUN_180c9b1c0(0,0,0);
      }
    }
    lVar7 = FUN_1803a51b0(uRam0000000185810eb0);
    FUN_182d09b30(lVar7,uRam0000000185810eb8);
    if (((*(longlong *)(param_1 + 0x10) == 0) ||
        (lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar8 == 0)) ||
       ((lVar8 = *(longlong *)(lVar8 + 0x20), lVar8 == 0 ||
        (lVar8 = *(longlong *)(lVar8 + 0x98), lVar8 == 0)))) goto code_r0x000181a5d56d;
    uVar11 = 0;
    iVar1 = *(int *)(lVar8 + 0x1c);
    uStack_38 = 0;
    while (lVar10 = lRam0000000185810ec0, lVar8 != 0) {
      if (iVar1 != *(int *)(lVar8 + 0x1c)) {
code_r0x000181a5d15f:
        if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
          FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
        }
        if (lVar8 == 0) goto code_r0x000181a5d599;
        if (iVar1 != *(int *)(lVar8 + 0x1c)) goto code_r0x000181a5d592;
        if (((*(longlong *)(param_1 + 0x10) == 0) ||
            (lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar8 == 0)) ||
           (*(longlong *)(lVar8 + 0x80) == 0)) goto code_r0x000181a5d56d;
        lVar8 = FUN_1800084f0(10,uRam0000000185806e90);
        if (lVar8 == 0) goto code_r0x000181a5d56d;
        iVar1 = *(int *)(lVar8 + 0x1c);
        uStack_38 = 0;
        goto code_r0x000181a5d242;
      }
      if (*(uint *)(lVar8 + 0x18) <= uStack_38) goto code_r0x000181a5d15f;
      lVar4 = *(longlong *)(lVar8 + 0x10);
      if (lVar4 == 0) goto code_r0x000181a5d58d;
      if (*(uint *)(lVar4 + 0x18) <= uStack_38) goto code_r0x000181a5d573;
      lVar4 = *(longlong *)(lVar4 + 0x20 + (longlong)(int)uStack_38 * 8);
      uStack_38 = uStack_38 + 1;
      if (lVar4 == 0) goto code_r0x000181a5d588;
      if (lVar7 == 0) goto code_r0x000181a5d583;
      uVar9 = *(undefined8 *)(lVar4 + 0x1e0);
      *(int *)(lVar7 + 0x1c) = *(int *)(lVar7 + 0x1c) + 1;
      lVar4 = *(longlong *)(lVar7 + 0x10);
      if (lVar4 == 0) goto code_r0x000181a5d57e;
      uVar2 = *(uint *)(lVar7 + 0x18);
      if (uVar2 < *(uint *)(lVar4 + 0x18)) {
        *(uint *)(lVar7 + 0x18) = uVar2 + 1;
        if (*(uint *)(lVar4 + 0x18) <= uVar2) goto code_r0x000181a5d579;
        *(undefined8 *)(lVar4 + 0x20 + (longlong)(int)uVar2 * 8) = uVar9;
      }
      else {
        FUN_1836f50c0(lVar7,uVar9,
                      *(undefined8 *)(*(longlong *)(*(longlong *)(lVar10 + 0x20) + 0xc0) + 0x70));
      }
    }
  }
  FUN_180352450();
  FUN_180352440();
code_r0x000181a5d5aa:
  FUN_180352460();
code_r0x000181a5d5af:
  FUN_180352460();
code_r0x000181a5d5b4:
  FUN_180352450();
code_r0x000181a5d5b9:
  FUN_180352450();
code_r0x000181a5d5be:
  FUN_180352450();
code_r0x000181a5d5c3:
  FUN_180352450();
code_r0x000181a5d5c8:
  FUN_183aed800(0);
code_r0x000181a5d5cf:
  FUN_180352450();
code_r0x000181a5d5d4:
  FUN_180352450();
  FUN_180352440();
  pcVar5 = (code *)swi(3);
  (*pcVar5)();
  return;
code_r0x000181a5d242:
  lVar10 = lRam0000000185810ec0;
  if (lVar8 != 0) {
    if (iVar1 != *(int *)(lVar8 + 0x1c)) {
code_r0x000181a5d301:
      if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
      }
      if (lVar8 == 0) goto code_r0x000181a5d5cf;
      if (iVar1 != *(int *)(lVar8 + 0x1c)) goto code_r0x000181a5d5c8;
      if (((*(longlong *)(param_1 + 0x10) == 0) ||
          (lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar8 == 0)) ||
         (*(longlong *)(lVar8 + 0xa8) == 0)) goto code_r0x000181a5d56d;
      lVar8 = *(longlong *)(lVar8 + 0x58);
      uVar9 = FUN_1800084f0(6,uRam00000001857a3910);
      if (lVar8 == 0) goto code_r0x000181a5d56d;
      if (cRam0000000185a3e874 == '\0') {
        FUN_18035da50(0x185779a40);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185806db0);
        LOCK();
        UNLOCK();
        cRam0000000185a3e874 = '\x01';
      }
      lVar10 = lRam0000000185779a40;
      if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
        FUN_180359940();
      }
      if (lVar10 != 0) {
        if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
          FUN_180359940();
        }
        lVar10 = FUN_1803ab790(lVar10,1);
        uVar11 = FUN_1803a7d00(lVar10 + 0x20);
      }
      cVar6 = FUN_181ba8230(lVar8,uVar11,0);
      if (cVar6 != '\0') {
        if ((*(longlong *)(lVar8 + 0x10) == 0) ||
           (lVar10 = *(longlong *)(*(longlong *)(lVar8 + 0x10) + 0x250), lVar10 == 0))
        goto code_r0x000181a5d56d;
        lVar10 = *(longlong *)(lVar10 + 0x30);
        lVar4 = *(longlong *)(lVar8 + 0x28);
        uVar3 = *(undefined4 *)(lVar8 + 0x30);
        lVar12 = FUN_1803a51b0(uRam0000000185806db0);
        *(longlong *)(lVar12 + 0x30) = lVar7;
        *(undefined8 *)(lVar12 + 0x28) = uVar9;
        *(longlong *)(lVar12 + 0x18) = lVar10 - lVar4;
        *(undefined4 *)(lVar12 + 0x20) = uVar3;
        *(undefined1 *)(lVar12 + 0x24) = 0;
        FUN_181ba80a0(lVar8,lVar12,0);
      }
      *(int *)(lVar8 + 0x30) = *(int *)(lVar8 + 0x30) + 1;
      if (((*(longlong *)(param_1 + 0x10) != 0) &&
          (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar7 != 0)) &&
         (*(longlong *)(lVar7 + 0x30) != 0)) {
        FUN_180005650(0xb,uRam00000001857a9638);
        if (((*(longlong *)(param_1 + 0x10) != 0) &&
            (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar7 != 0)) &&
           (lVar7 = *(longlong *)(lVar7 + 0x180), lVar7 != 0)) {
          FUN_18000a350(0x37,uRam00000001857a96a0,lVar7,1);
          if (((*(longlong *)(param_1 + 0x10) != 0) &&
              (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar7 != 0)) &&
             (lVar7 = *(longlong *)(lVar7 + 0x180), lVar7 != 0)) {
            FUN_18000a350(0x38,uRam00000001857a96a0,lVar7,1);
            lVar7 = *(longlong *)(param_1 + 0x18);
            if (lVar7 != 0) {
              (**(code **)(lVar7 + 0x18))
                        (*(undefined8 *)(lVar7 + 0x40),*(undefined8 *)(lVar7 + 0x28));
            }
            return;
          }
        }
      }
      goto code_r0x000181a5d56d;
    }
    if (*(uint *)(lVar8 + 0x18) <= uStack_38) goto code_r0x000181a5d301;
    lVar4 = *(longlong *)(lVar8 + 0x10);
    if (lVar4 == 0) goto code_r0x000181a5d5c3;
    if (*(uint *)(lVar4 + 0x18) <= uStack_38) goto code_r0x000181a5d5aa;
    lVar4 = *(longlong *)(lVar4 + 0x20 + (longlong)(int)uStack_38 * 8);
    uStack_38 = uStack_38 + 1;
    if (lVar4 == 0) goto code_r0x000181a5d5be;
    if (lVar7 == 0) goto code_r0x000181a5d5b9;
    uVar9 = *(undefined8 *)(lVar4 + 0x1e0);
    *(int *)(lVar7 + 0x1c) = *(int *)(lVar7 + 0x1c) + 1;
    lVar4 = *(longlong *)(lVar7 + 0x10);
    if (lVar4 == 0) goto code_r0x000181a5d5b4;
    uVar2 = *(uint *)(lVar7 + 0x18);
    if (uVar2 < *(uint *)(lVar4 + 0x18)) {
      *(uint *)(lVar7 + 0x18) = uVar2 + 1;
      if (*(uint *)(lVar4 + 0x18) <= uVar2) goto code_r0x000181a5d5af;
      *(undefined8 *)(lVar4 + 0x20 + (longlong)(int)uVar2 * 8) = uVar9;
    }
    else {
      FUN_1836f50c0(lVar7,uVar9,
                    *(undefined8 *)(*(longlong *)(*(longlong *)(lVar10 + 0x20) + 0xc0) + 0x70));
    }
    goto code_r0x000181a5d242;
  }
  goto code_r0x000181a5d5d4;
}

