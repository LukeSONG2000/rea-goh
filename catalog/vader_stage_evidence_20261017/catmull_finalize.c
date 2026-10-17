// catmull_finalize VA 0x1824a9be0 RVA 0x24a9be0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1824a9be0(longlong *param_1,longlong param_2,longlong param_3,char param_4)

{
  int iVar1;
  undefined8 uVar2;
  int iVar3;
  undefined4 uVar4;
  code *pcVar5;
  char cVar6;
  undefined8 uVar7;
  longlong lVar8;
  undefined8 *puVar9;
  ulonglong uVar10;
  undefined4 uVar11;
  int iVar12;
  longlong lVar13;
  uint uVar14;
  longlong lVar15;
  int iVar16;
  longlong lVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  undefined4 uVar21;
  undefined8 unaff_XMM6_Qa;
  undefined4 uVar22;
  undefined8 unaff_XMM6_Qb;
  float fVar23;
  undefined8 uStack_c8;
  undefined4 uStack_c0;
  undefined8 uStack_b8;
  undefined4 uStack_b0;
  undefined1 auStack_a8 [16];
  undefined1 auStack_98 [8];
  undefined *puStack_90;
  undefined8 uStack_68;
  float fStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  
  if (cRam0000000185a423a6 == '\0') {
    puStack_90 = &UNK_1824a9c11;
    FUN_18035da50(0x1857cf068);
    LOCK();
    UNLOCK();
    cRam0000000185a423a6 = '\x01';
  }
  if ((param_3 != 0) && (param_2 != 0)) {
    iVar16 = *(int *)(param_3 + 0x18);
    lVar17 = (longlong)iVar16;
    uStack_38 = unaff_XMM6_Qa;
    uStack_30 = unaff_XMM6_Qb;
    if ((*(longlong *)(param_2 + 0x30) == 0) ||
       (*(int *)(*(longlong *)(param_2 + 0x30) + 0x18) != 2)) {
      puStack_90 = &UNK_1824a9c72;
      uVar7 = FUN_1803a3dc0(uRam00000001857cf068,2);
      *(undefined8 *)(param_2 + 0x30) = uVar7;
    }
    lVar13 = *(longlong *)(param_2 + 0x30);
    if (param_4 == '\0') {
      if (*(uint *)(param_3 + 0x18) < 2) goto code_r0x0001824a9eec;
      uVar7 = *(undefined8 *)(param_3 + 0x2c);
      fVar23 = *(float *)(param_3 + 0x34);
      if (cRam0000000185a33cad == '\0') {
        puStack_90 = &UNK_1824a9caf;
        FUN_18035da50(0x1858208a0);
        LOCK();
        UNLOCK();
        cRam0000000185a33cad = '\x01';
      }
      cVar6 = cRam0000000185a33cad;
      uVar2 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
      uStack_54 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
      uStack_5c = (undefined4)uVar2;
      uStack_58 = (undefined4)((ulonglong)uVar2 >> 0x20);
      uStack_68 = uVar7;
      fStack_60 = fVar23;
      if (lVar13 == 0) goto code_r0x0001824a9ee6;
      if (*(int *)(lVar13 + 0x18) == 0) goto code_r0x0001824a9eec;
      lVar15 = lVar17 + -1;
      *(undefined8 *)(lVar13 + 0x20) = uVar7;
      *(ulonglong *)(lVar13 + 0x28) = CONCAT44(uStack_5c,fVar23);
      *(ulonglong *)(lVar13 + 0x30) = CONCAT44(uStack_54,uStack_58);
      if ((*(uint *)(param_3 + 0x18) <= (uint)lVar15) || (*(uint *)(param_3 + 0x18) <= iVar16 - 2U))
      goto code_r0x0001824a9eec;
      lVar13 = *(longlong *)(param_2 + 0x30);
      fVar23 = *(float *)(param_3 + 0x28 + lVar15 * 0xc);
      uVar7 = *(undefined8 *)(param_3 + 0x20 + lVar15 * 0xc);
      fVar19 = (float)uVar7;
      fVar20 = (float)((ulonglong)uVar7 >> 0x20);
      fVar18 = *(float *)(param_3 + 0x28 + (lVar17 + -2) * 0xc);
      uVar7 = *(undefined8 *)(param_3 + 0x20 + (lVar17 + -2) * 0xc);
      if (cVar6 == '\0') {
        puStack_90 = &UNK_1824a9d71;
        FUN_18035da50(0x1858208a0);
        LOCK();
        UNLOCK();
        cRam0000000185a33cad = '\x01';
      }
      uStack_68 = CONCAT44((fVar20 - (float)((ulonglong)uVar7 >> 0x20)) + fVar20,
                           (fVar19 - (float)uVar7) + fVar19);
      fStack_60 = (fVar23 - fVar18) + fVar23;
    }
    else {
      lVar17 = lVar17 + -2;
      if (*(uint *)(param_3 + 0x18) <= (uint)lVar17) goto code_r0x0001824a9eec;
      uVar7 = *(undefined8 *)(param_3 + 0x20 + lVar17 * 0xc);
      fVar23 = *(float *)(param_3 + 0x28 + lVar17 * 0xc);
      if (cRam0000000185a33cad == '\0') {
        puStack_90 = &UNK_1824a9dc3;
        FUN_18035da50(0x1858208a0);
        LOCK();
        UNLOCK();
        cRam0000000185a33cad = '\x01';
      }
      cVar6 = cRam0000000185a33cad;
      uVar2 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
      uStack_54 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
      uStack_5c = (undefined4)uVar2;
      uStack_58 = (undefined4)((ulonglong)uVar2 >> 0x20);
      uStack_68 = uVar7;
      fStack_60 = fVar23;
      if (lVar13 == 0) goto code_r0x0001824a9ee6;
      if (*(int *)(lVar13 + 0x18) == 0) goto code_r0x0001824a9eec;
      *(undefined8 *)(lVar13 + 0x20) = uVar7;
      *(ulonglong *)(lVar13 + 0x28) = CONCAT44(uStack_5c,fVar23);
      *(ulonglong *)(lVar13 + 0x30) = CONCAT44(uStack_54,uStack_58);
      if (*(uint *)(param_3 + 0x18) < 2) goto code_r0x0001824a9eec;
      uVar2 = *(undefined8 *)(param_3 + 0x2c);
      fVar18 = *(float *)(param_3 + 0x34);
      lVar13 = *(longlong *)(param_2 + 0x30);
      uStack_68 = uVar2;
      fStack_60 = fVar18;
      if (cVar6 == '\0') {
        puStack_90 = &UNK_1824a9e44;
        uStack_68 = uVar7;
        fStack_60 = fVar23;
        FUN_18035da50(0x1858208a0);
        LOCK();
        UNLOCK();
        cRam0000000185a33cad = '\x01';
        uStack_68 = uVar2;
        fStack_60 = fVar18;
      }
    }
    uVar7 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
    uStack_54 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
    uStack_5c = (undefined4)uVar7;
    uStack_58 = (undefined4)((ulonglong)uVar7 >> 0x20);
    if (lVar13 != 0) {
      if (1 < *(uint *)(lVar13 + 0x18)) {
        *(undefined8 *)(lVar13 + 0x38) = uStack_68;
        *(ulonglong *)(lVar13 + 0x40) = CONCAT44(uStack_5c,fStack_60);
        *(ulonglong *)(lVar13 + 0x48) = CONCAT44(uStack_54,uStack_58);
        iVar16 = iVar16 * *(int *)(param_2 + 0x24);
        *(int *)(param_2 + 0x28) = iVar16;
        puStack_90 = &UNK_1824a9eac;
        FUN_1824aa360(param_1,param_2,iVar16,0);
        iVar16 = *(int *)(param_2 + 0x24);
        if (cRam0000000185a423a9 == '\0') {
          FUN_18035da50(0x1857cf050,param_2,iVar16,0);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x185821900);
          LOCK();
          UNLOCK();
          cRam0000000185a423a9 = '\x01';
        }
        uStack_48 = uStack_38;
        uStack_40 = uStack_30;
        if ((param_2 != 0) && (*(longlong *)(param_2 + 0x18) != 0)) {
          iVar3 = *(int *)(*(longlong *)(param_2 + 0x18) + 0x18);
          lVar17 = FUN_1803a3dc0(uRam0000000185821900,iVar3);
          if (lVar17 != 0) {
            if (*(int *)(lVar17 + 0x18) == 0) {
code_r0x0001824aab10:
              FUN_180352460();
              pcVar5 = (code *)swi(3);
              (*pcVar5)();
              return;
            }
            *(undefined4 *)(lVar17 + 0x20) = 0;
            if (1 < iVar3) {
              uVar14 = 1;
              do {
                if (*(int *)(lRam00000001857cf050 + 0xe4) == 0) {
                  FUN_180359940(lRam00000001857cf050);
                }
                lVar13 = **(longlong **)(lRam00000001857cf050 + 0xb8);
                if (lVar13 == 0) goto code_r0x0001824aab0a;
                if (*(int *)(lVar13 + 0x18) == 0) goto code_r0x0001824aab10;
                if (uVar14 == 1) {
                  lVar15 = *(longlong *)(param_2 + 0x30);
                  if (lVar15 == 0) goto code_r0x0001824aab0a;
                  if (*(int *)(lVar15 + 0x18) == 0) goto code_r0x0001824aab10;
                  uVar7 = *(undefined8 *)(lVar15 + 0x20);
                  uVar11 = *(undefined4 *)(lVar15 + 0x28);
                }
                else {
                  lVar15 = *(longlong *)(param_2 + 0x18);
                  if (lVar15 == 0) goto code_r0x0001824aab0a;
                  lVar8 = (ulonglong)uVar14 - 2;
                  if (*(uint *)(lVar15 + 0x18) <= (uint)lVar8) goto code_r0x0001824aab10;
                  uVar7 = *(undefined8 *)(lVar15 + 0x20 + lVar8 * 0xc);
                  uVar11 = *(undefined4 *)(lVar15 + 0x28 + lVar8 * 0xc);
                }
                *(undefined8 *)(lVar13 + 0x20) = uVar7;
                *(undefined4 *)(lVar13 + 0x28) = uVar11;
                if (*(int *)(lRam00000001857cf050 + 0xe4) == 0) {
                  FUN_180359940();
                }
                lVar13 = *(longlong *)(param_2 + 0x18);
                if (lVar13 == 0) goto code_r0x0001824aab0a;
                lVar15 = (ulonglong)uVar14 - 1;
                if (*(uint *)(lVar13 + 0x18) <= (uint)lVar15) goto code_r0x0001824aab10;
                lVar8 = *(longlong *)(*(longlong *)(lRam00000001857cf050 + 0xb8) + 8);
                if (lVar8 == 0) goto code_r0x0001824aab0a;
                if (*(int *)(lVar8 + 0x18) == 0) goto code_r0x0001824aab10;
                uVar11 = *(undefined4 *)(lVar13 + 0x28 + lVar15 * 0xc);
                *(undefined8 *)(lVar8 + 0x20) = *(undefined8 *)(lVar13 + 0x20 + lVar15 * 0xc);
                *(undefined4 *)(lVar8 + 0x28) = uVar11;
                lVar13 = *(longlong *)(param_2 + 0x18);
                if (lVar13 == 0) goto code_r0x0001824aab0a;
                if (*(uint *)(lVar13 + 0x18) <= uVar14) goto code_r0x0001824aab10;
                lVar15 = *(longlong *)(*(longlong *)(lRam00000001857cf050 + 0xb8) + 8);
                if (lVar15 == 0) goto code_r0x0001824aab0a;
                if (*(uint *)(lVar15 + 0x18) < 2) goto code_r0x0001824aab10;
                uVar11 = *(undefined4 *)(lVar13 + 0x28 + (ulonglong)uVar14 * 0xc);
                *(undefined8 *)(lVar15 + 0x2c) =
                     *(undefined8 *)(lVar13 + 0x20 + (ulonglong)uVar14 * 0xc);
                *(undefined4 *)(lVar15 + 0x34) = uVar11;
                lVar13 = **(longlong **)(lRam00000001857cf050 + 0xb8);
                if (lVar13 == 0) goto code_r0x0001824aab0a;
                if (*(uint *)(lVar13 + 0x18) < 2) goto code_r0x0001824aab10;
                if (uVar14 == iVar3 - 1U) {
                  lVar15 = *(longlong *)(param_2 + 0x30);
                  if (lVar15 == 0) goto code_r0x0001824aab0a;
                  if (*(uint *)(lVar15 + 0x18) < 2) goto code_r0x0001824aab10;
                  uVar7 = *(undefined8 *)(lVar15 + 0x38);
                  uVar11 = *(undefined4 *)(lVar15 + 0x40);
                }
                else {
                  lVar15 = *(longlong *)(param_2 + 0x18);
                  if (lVar15 == 0) goto code_r0x0001824aab0a;
                  lVar8 = (ulonglong)uVar14 + 1;
                  if (*(uint *)(lVar15 + 0x18) <= (uint)lVar8) goto code_r0x0001824aab10;
                  uVar7 = *(undefined8 *)(lVar15 + 0x20 + lVar8 * 0xc);
                  uVar11 = *(undefined4 *)(lVar15 + 0x28 + lVar8 * 0xc);
                }
                *(undefined8 *)(lVar13 + 0x38) = uVar7;
                *(undefined4 *)(lVar13 + 0x40) = uVar11;
                fVar23 = 0.0;
                if (*(int *)(lRam00000001857cf050 + 0xe4) == 0) {
                  FUN_180359940(lRam00000001857cf050);
                }
                puVar9 = (undefined8 *)
                         (**(code **)(*param_1 + 0x188))
                                   (auStack_a8,param_1,0,
                                    (*(undefined8 **)(lRam00000001857cf050 + 0xb8))[1],param_2,
                                    **(undefined8 **)(lRam00000001857cf050 + 0xb8),
                                    *(undefined8 *)(*param_1 + 400));
                iVar1 = iVar16 + 1;
                iVar12 = 1;
                uVar21 = (undefined4)*puVar9;
                uVar22 = (undefined4)((ulonglong)*puVar9 >> 0x20);
                uVar11 = *(undefined4 *)(puVar9 + 1);
                if (1 < iVar1) {
                  do {
                    if (*(int *)(lRam00000001857cf050 + 0xe4) == 0) {
                      FUN_180359940(lRam00000001857cf050);
                    }
                    lVar13 = *param_1;
                    puVar9 = (undefined8 *)
                             (**(code **)(lVar13 + 0x188))
                                       (auStack_98,param_1,lVar13,
                                        (*(undefined8 **)(lRam00000001857cf050 + 0xb8))[1],param_2,
                                        **(undefined8 **)(lRam00000001857cf050 + 0xb8),
                                        *(undefined8 *)(lVar13 + 400));
                    uStack_c8 = CONCAT44(uVar22,uVar21);
                    uVar7 = *puVar9;
                    uVar4 = *(undefined4 *)(puVar9 + 1);
                    uStack_c0 = uVar11;
                    uStack_b8 = uVar7;
                    uStack_b0 = uVar4;
                    fVar18 = (float)FUN_180ceb490(&uStack_b8,&uStack_c8);
                    iVar12 = iVar12 + 1;
                    fVar23 = fVar23 + fVar18;
                    uVar21 = (undefined4)uVar7;
                    uVar22 = (undefined4)((ulonglong)uVar7 >> 0x20);
                    uVar11 = uVar4;
                  } while (iVar12 < iVar1);
                }
                if (*(uint *)(lVar17 + 0x18) <= uVar14) goto code_r0x0001824aab10;
                uVar10 = (ulonglong)uVar14;
                uVar14 = uVar14 + 1;
                *(float *)(lVar17 + 0x20 + uVar10 * 4) = fVar23;
              } while ((int)uVar14 < iVar3);
            }
            *(longlong *)(param_2 + 0x10) = lVar17;
            return;
          }
        }
code_r0x0001824aab0a:
        FUN_180352450();
        pcVar5 = (code *)swi(3);
        (*pcVar5)();
        return;
      }
code_r0x0001824a9eec:
      puStack_90 = &UNK_1824a9ef1;
      FUN_180352460();
      pcVar5 = (code *)swi(3);
      (*pcVar5)();
      return;
    }
  }
code_r0x0001824a9ee6:
  puStack_90 = &UNK_1824a9eeb;
  FUN_180352450();
  pcVar5 = (code *)swi(3);
  (*pcVar5)();
  return;
}

