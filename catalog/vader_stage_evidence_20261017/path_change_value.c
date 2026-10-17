// path_change_value VA 0x18249bf10 RVA 0x249bf10

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_18249bf10(undefined8 param_1,longlong param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  undefined1 uVar4;
  int iVar5;
  undefined4 uVar6;
  longlong *plVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined8 uVar19;
  undefined8 uVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  undefined8 uVar23;
  undefined8 uVar24;
  undefined8 uVar25;
  char cVar26;
  int iVar27;
  longlong lVar28;
  undefined8 uVar29;
  float *pfVar30;
  longlong lVar31;
  code *pcVar32;
  longlong lVar33;
  longlong *plVar34;
  uint uVar35;
  int iVar36;
  longlong lVar37;
  uint uVar38;
  ulonglong uVar39;
  bool bVar40;
  float fVar41;
  float fVar42;
  float fVar43;
  char cStackX_10;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  
  if (cRam0000000185a42368 == '\0') {
    FUN_18035da50(0x18582bf28);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185838a18);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185823e40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185828348);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858274c0);
    LOCK();
    UNLOCK();
    FUN_18035da50();
    LOCK();
    UNLOCK();
    cRam0000000185a42368 = '\x01';
  }
  if (param_2 == 0) goto code_r0x00018249c67b;
  plVar7 = *(longlong **)(param_2 + 0x48);
  if (plVar7 == (longlong *)0x0) {
code_r0x00018249c028:
    if (plVar7 == (longlong *)0x0) goto code_r0x00018249c67b;
    if ((*(byte *)(*plVar7 + 0x130) < *(byte *)(lRam000000018582bf28 + 0x130)) ||
       (*(longlong *)
         (*(longlong *)(*plVar7 + 200) + -8 + (ulonglong)*(byte *)(lRam000000018582bf28 + 0x130) * 8
         ) != lRam000000018582bf28)) {
      FUN_180352e70(plVar7,lRam000000018582bf28);
      pcVar32 = (code *)swi(3);
      (*pcVar32)();
      return;
    }
    lVar28 = FUN_1844a1660(plVar7,0);
  }
  else {
    plVar34 = (longlong *)0x0;
    if (*plVar7 == lRam0000000185828348) {
      plVar34 = plVar7;
    }
    if (plVar34 == (longlong *)0x0) goto code_r0x00018249c028;
    lVar28 = FUN_1844a54d0(plVar34,0);
  }
  if (*(int *)(param_2 + 0x144) == 1) {
    if (lVar28 == 0) goto code_r0x00018249c67b;
    uVar29 = FUN_1844b9290(lVar28,0);
    *(undefined8 *)(param_2 + 0x188) = uVar29;
  }
  lVar33 = *(longlong *)(param_2 + 0x130);
  if (lVar33 == 0) goto code_r0x00018249c67b;
  if (*(char *)(lVar33 + 0x3c) != '\0') goto code_r0x00018249c5fc;
  lVar33 = *(longlong *)(param_2 + 0x1b0);
  if (lVar33 == 0) goto code_r0x00018249c67b;
  pfVar30 = (float *)(**(code **)(lVar33 + 0x18))
                               (&uStack_b8,*(undefined8 *)(lVar33 + 0x40),
                                *(undefined8 *)(lVar33 + 0x28));
  lVar33 = *(longlong *)(param_2 + 0x130);
  if (lVar33 == 0) goto code_r0x00018249c67b;
  uVar19 = *(undefined8 *)(param_2 + 0x148);
  fVar1 = *pfVar30;
  fVar2 = pfVar30[1];
  uVar29 = *(undefined8 *)pfVar30;
  fVar3 = pfVar30[2];
  uVar6 = *(undefined4 *)(param_2 + 0x150);
  uVar8 = *(undefined4 *)(param_2 + 0x154);
  uVar9 = *(undefined4 *)(param_2 + 0x158);
  uVar10 = *(undefined4 *)(param_2 + 0x15c);
  uVar11 = *(undefined4 *)(param_2 + 0x160);
  uVar12 = *(undefined4 *)(param_2 + 0x164);
  uVar13 = *(undefined4 *)(param_2 + 0x168);
  uVar14 = *(undefined4 *)(param_2 + 0x16c);
  uVar15 = *(undefined4 *)(param_2 + 0x170);
  uVar16 = *(undefined4 *)(param_2 + 0x174);
  uVar17 = *(undefined4 *)(param_2 + 0x178);
  uVar18 = *(undefined4 *)(param_2 + 0x17c);
  uVar20 = *(undefined8 *)(param_2 + 0x180);
  uVar21 = *(undefined8 *)(param_2 + 0x188);
  uVar22 = *(undefined8 *)(param_2 + 400);
  uVar23 = *(undefined8 *)(param_2 + 0x198);
  uVar24 = *(undefined8 *)(param_2 + 0x1a0);
  uVar25 = *(undefined8 *)(param_2 + 0x1a8);
  *(undefined8 *)(lVar33 + 0x58) = *(undefined8 *)(param_2 + 0x140);
  *(undefined8 *)(lVar33 + 0x60) = uVar19;
  *(undefined4 *)(lVar33 + 0x68) = uVar6;
  *(undefined4 *)(lVar33 + 0x6c) = uVar8;
  *(undefined4 *)(lVar33 + 0x70) = uVar9;
  *(undefined4 *)(lVar33 + 0x74) = uVar10;
  *(undefined4 *)(lVar33 + 0x78) = uVar11;
  *(undefined4 *)(lVar33 + 0x7c) = uVar12;
  *(undefined4 *)(lVar33 + 0x80) = uVar13;
  *(undefined4 *)(lVar33 + 0x84) = uVar14;
  *(undefined4 *)(lVar33 + 0x88) = uVar15;
  *(undefined4 *)(lVar33 + 0x8c) = uVar16;
  *(undefined4 *)(lVar33 + 0x90) = uVar17;
  *(undefined4 *)(lVar33 + 0x94) = uVar18;
  *(undefined8 *)(lVar33 + 0x98) = uVar20;
  *(undefined8 *)(lVar33 + 0xa0) = uVar21;
  *(undefined8 *)(lVar33 + 0xa8) = uVar22;
  *(undefined8 *)(lVar33 + 0xb0) = uVar23;
  *(undefined8 *)(lVar33 + 0xb8) = uVar24;
  *(undefined8 *)(lVar33 + 0xc0) = uVar25;
  if ((*(longlong *)(lVar33 + 0x18) == 0) ||
     (plVar7 = *(longlong **)(lVar33 + 0xd8), plVar7 == (longlong *)0x0)) goto code_r0x00018249c67b;
  iVar5 = *(int *)(*(longlong *)(lVar33 + 0x18) + 0x18);
  lVar37 = (longlong)iVar5;
  iVar27 = (**(code **)(*plVar7 + 0x198))(plVar7,*(undefined8 *)(*plVar7 + 0x1a0));
  iVar36 = 1;
  if (iVar5 <= iVar27) goto code_r0x00018249c1fb;
  if (*(longlong *)(lVar33 + 0x18) == 0) goto code_r0x00018249c67b;
  if (*(int *)(*(longlong *)(lVar33 + 0x18) + 0x18) == 0) goto code_r0x00018249c681;
  if (*(int *)(lRam0000000185838a18 + 0xe4) == 0) {
    FUN_180359940();
  }
  cVar26 = FUN_180c75680();
  if ((cVar26 == '\0') || (cVar26 = FUN_180c75680(), cVar26 == '\0')) {
code_r0x00018249c1fb:
    cStackX_10 = '\x01';
  }
  else {
    cStackX_10 = '\0';
    cVar26 = FUN_180c75680();
    if (cVar26 == '\0') goto code_r0x00018249c1fb;
  }
  cVar26 = '\0';
  if (*(char *)(param_2 + 0x150) != '\0') {
    lVar31 = *(longlong *)(lVar33 + 0x18);
    if (lVar31 == 0) goto code_r0x00018249c67b;
    if (*(uint *)(lVar31 + 0x18) <= iVar5 - 1U) goto code_r0x00018249c681;
    uStack_b8 = *(undefined8 *)(lVar31 + 0x14 + lVar37 * 0xc);
    fVar43 = (float)uStack_b8;
    fVar42 = *(float *)(lVar31 + 0x1c + lVar37 * 0xc);
    if (*(int *)(lVar33 + 0x20) == 2) {
      if (iVar5 < 3) {
        if (*(int *)(lRam0000000185823e40 + 0xe4) == 0) {
          FUN_180359940();
        }
        FUN_184462a40(uRam00000001857cf160,0);
      }
      else {
        if (lVar31 == 0) goto code_r0x00018249c67b;
        if (*(uint *)(lVar31 + 0x18) <= iVar5 - 3U) goto code_r0x00018249c681;
        uStack_b8 = *(undefined8 *)(lVar31 + -4 + lVar37 * 0xc);
        fVar43 = (float)uStack_b8;
        fVar42 = *(float *)(lVar31 + 4 + lVar37 * 0xc);
      }
    }
    fVar43 = fVar43 - fVar1;
    fVar41 = uStack_b8._4_4_ - fVar2;
    if (_UNK_18529ec24 <= fVar41 * fVar41 + fVar43 * fVar43 + (fVar42 - fVar3) * (fVar42 - fVar3)) {
      cVar26 = '\x01';
    }
  }
  lVar37 = FUN_1803a3dc0(uRam00000001858274c0);
  if (cStackX_10 == '\0') {
    iVar36 = 0;
  }
  else {
    if (lVar37 == 0) goto code_r0x00018249c67b;
    if (*(int *)(lVar37 + 0x18) == 0) goto code_r0x00018249c681;
    *(undefined8 *)(lVar37 + 0x20) = uVar29;
    *(float *)(lVar37 + 0x28) = fVar3;
  }
  uVar39 = 0;
  if (0 < iVar5) {
    do {
      lVar31 = *(longlong *)(lVar33 + 0x18);
      if (lVar31 == 0) goto code_r0x00018249c67b;
      uVar38 = (uint)uVar39;
      if (*(uint *)(lVar31 + 0x18) <= uVar38) goto code_r0x00018249c681;
      if (lVar37 == 0) goto code_r0x00018249c67b;
      uVar35 = uVar38 + iVar36;
      if (*(uint *)(lVar37 + 0x18) <= uVar35) goto code_r0x00018249c681;
      uVar6 = *(undefined4 *)(lVar31 + 0x28 + uVar39 * 0xc);
      *(undefined8 *)(lVar37 + 0x20 + (longlong)(int)uVar35 * 0xc) =
           *(undefined8 *)(lVar31 + 0x20 + uVar39 * 0xc);
      *(undefined4 *)(lVar37 + 0x28 + (longlong)(int)uVar35 * 0xc) = uVar6;
      uVar39 = (ulonglong)(uVar38 + 1);
    } while ((int)(uVar38 + 1) < iVar5);
  }
  if (cVar26 != '\0') {
    if (lVar37 == 0) goto code_r0x00018249c67b;
    if ((*(int *)(lVar37 + 0x18) == 0) ||
       (lVar31 = (longlong)*(int *)(lVar37 + 0x18) + -1, *(uint *)(lVar37 + 0x18) <= (uint)lVar31))
    {
code_r0x00018249c681:
      FUN_180352460();
      pcVar32 = (code *)swi(3);
      (*pcVar32)();
      return;
    }
    uVar6 = *(undefined4 *)(lVar37 + 0x28);
    *(undefined8 *)(lVar37 + 0x20 + lVar31 * 0xc) = *(undefined8 *)(lVar37 + 0x20);
    *(undefined4 *)(lVar37 + 0x28 + lVar31 * 0xc) = uVar6;
  }
  *(longlong *)(lVar33 + 0x18) = lVar37;
  *(char *)(lVar33 + 0x54) = cStackX_10;
  *(char *)(lVar33 + 0x55) = cVar26;
  uVar38 = *(uint *)(param_2 + 0x148);
  uVar4 = *(undefined1 *)(param_2 + 0x150);
  if (uVar38 == 0) {
code_r0x00018249c4b1:
    plVar7 = *(longlong **)(lVar33 + 0xd8);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)(*plVar7 + 0x178))
                (plVar7,lVar33,*(undefined8 *)(lVar33 + 0x18),uVar4,*(undefined8 *)(*plVar7 + 0x180)
                );
      *(undefined1 *)(lVar33 + 0x3c) = 1;
      if (lVar28 != 0) {
        if (cRam0000000185a50937 == '\0') {
          FUN_18035da50(0x1857b8340);
          LOCK();
          UNLOCK();
          cRam0000000185a50937 = '\x01';
        }
        uStack_b8 = 0;
        uStack_b0 = 0;
        lVar33 = *(longlong *)(lVar28 + 0x10);
        if (lVar33 == 0) {
          FUN_1844c5660(lVar28,0);
          pcVar32 = (code *)swi(3);
          (*pcVar32)();
          return;
        }
        pcVar32 = pcRam0000000185a509b8;
        if ((pcRam0000000185a509b8 == (code *)0x0) &&
           (pcVar32 = (code *)FUN_1803679a0(&UNK_184b5d760), pcVar32 == (code *)0x0)) {
          uVar29 = func_0x000180367810(&UNK_184b5d760);
          FUN_1803660c0(uVar29,0);
          pcVar32 = (code *)swi(3);
          (*pcVar32)();
          return;
        }
        pcRam0000000185a509b8 = pcVar32;
        (*pcRam0000000185a509b8)(lVar33,&uStack_b8);
        bVar40 = cRam0000000185a50937 == '\0';
        uStack_98 = 0;
        uStack_90 = 0;
        *(undefined8 *)(param_2 + 0x194) = uStack_b8;
        *(undefined8 *)(param_2 + 0x19c) = uStack_b0;
        if (bVar40) {
          FUN_18035da50(0x1857b8340);
          LOCK();
          UNLOCK();
          cRam0000000185a50937 = '\x01';
        }
        uStack_a8 = 0;
        uStack_a0 = 0;
        lVar33 = *(longlong *)(lVar28 + 0x10);
        if (lVar33 == 0) {
          FUN_1844c5660(lVar28,0);
          pcVar32 = (code *)swi(3);
          (*pcVar32)();
          return;
        }
        pcVar32 = pcRam0000000185a509b8;
        if ((pcRam0000000185a509b8 == (code *)0x0) &&
           (pcVar32 = (code *)FUN_1803679a0(&UNK_184b5d760), pcVar32 == (code *)0x0)) {
          uVar29 = func_0x000180367810(&UNK_184b5d760);
          FUN_1803660c0(uVar29,0);
          pcVar32 = (code *)swi(3);
          (*pcVar32)();
          return;
        }
        pcRam0000000185a509b8 = pcVar32;
        (*pcRam0000000185a509b8)(lVar33,&uStack_a8);
        uStack_98 = uStack_a8;
        uStack_90 = uStack_a0;
        lVar28 = FUN_180d6b050(&uStack_b8,&uStack_98);
        *(undefined4 *)(param_2 + 0x1a4) = *(undefined4 *)(lVar28 + 8);
        lVar33 = *(longlong *)(param_2 + 0x130);
code_r0x00018249c5fc:
        *(longlong *)(param_2 + 0x138) = lVar33;
        return;
      }
    }
  }
  else {
    uVar35 = 0;
    while (lVar37 != 0) {
      if (*(int *)(lVar37 + 0x18) <= (int)uVar35) goto code_r0x00018249c4b1;
      lVar37 = *(longlong *)(lVar33 + 0x18);
      if (lVar37 == 0) break;
      if (*(uint *)(lVar37 + 0x18) <= uVar35) goto code_r0x00018249c681;
      lVar31 = (longlong)(int)uVar35;
      fVar42 = fVar1;
      if ((uVar38 & 2) == 0) {
        fVar42 = *(float *)(lVar37 + 0x20 + lVar31 * 0xc);
      }
      fVar43 = fVar2;
      if ((uVar38 & 4) == 0) {
        fVar43 = *(float *)(lVar37 + 0x24 + lVar31 * 0xc);
      }
      fVar41 = fVar3;
      if ((uVar38 & 8) == 0) {
        fVar41 = *(float *)(lVar37 + 0x28 + lVar31 * 0xc);
      }
      lVar31 = (longlong)(int)uVar35;
      uVar35 = uVar35 + 1;
      *(float *)(lVar37 + 0x20 + lVar31 * 0xc) = fVar42;
      *(float *)(lVar37 + 0x24 + lVar31 * 0xc) = fVar43;
      *(float *)(lVar37 + 0x28 + lVar31 * 0xc) = fVar41;
      lVar37 = *(longlong *)(lVar33 + 0x18);
    }
  }
code_r0x00018249c67b:
  FUN_180352450();
  pcVar32 = (code *)swi(3);
  (*pcVar32)();
  return;
}

