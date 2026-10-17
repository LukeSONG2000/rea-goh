// catmull_get_point VA 0x1824a9f00 RVA 0x24a9f00

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float * FUN_1824a9f00(float *param_1,undefined8 param_2,float param_3,longlong param_4,
                     undefined8 param_5,longlong param_6)

{
  int iVar1;
  uint uVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  int iVar7;
  code *pcVar8;
  uint uVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  uint uVar14;
  longlong lVar15;
  longlong lVar16;
  float *pfVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  double dVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  undefined8 uVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  float fVar31;
  float fStack_cc;
  
  if (cRam0000000185a423a7 == '\0') {
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    cRam0000000185a423a7 = '\x01';
  }
  if (param_4 == 0) {
DAT_1824aa350:
    FUN_180352450();
    pcVar8 = (code *)swi(3);
    pfVar17 = (float *)(*pcVar8)();
    return pfVar17;
  }
  iVar7 = *(int *)(param_4 + 0x18);
  iVar1 = iVar7 + -1;
  if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
    FUN_180359940();
  }
  dVar22 = (double)func_0x000180ba8d60((double)((float)iVar1 * param_3));
  uVar14 = _UNK_18529fe80;
  fVar13 = _UNK_18529f198;
  fVar12 = _UNK_18529f170;
  fVar11 = _UNK_18529f0f0;
  fVar10 = _UNK_18529ee14;
  uVar2 = iVar7 - 2;
  uVar9 = (int)dVar22;
  if ((int)uVar2 <= (int)dVar22) {
    uVar9 = uVar2;
  }
  if (uVar9 == 0) {
    if (param_6 == 0) goto DAT_1824aa350;
    if (*(int *)(param_6 + 0x18) == 0) goto code_r0x0001824aa356;
    fVar28 = (float)*(undefined8 *)(param_6 + 0x20);
    fVar29 = (float)((ulonglong)*(undefined8 *)(param_6 + 0x20) >> 0x20);
    fVar19 = *(float *)(param_6 + 0x28);
  }
  else {
    if (*(uint *)(param_4 + 0x18) <= uVar9 - 1) goto code_r0x0001824aa356;
    uVar27 = *(undefined8 *)(param_4 + 0x20 + ((longlong)(int)uVar9 + -1) * 0xc);
    fVar28 = (float)uVar27;
    fVar29 = (float)((ulonglong)uVar27 >> 0x20);
    fVar19 = *(float *)(param_4 + 0x28 + ((longlong)(int)uVar9 + -1) * 0xc);
  }
  if ((uVar9 < *(uint *)(param_4 + 0x18)) && (uVar9 + 1 < *(uint *)(param_4 + 0x18))) {
    if (*(int *)(param_4 + 0x18) + -1 < (int)(uVar9 + 2)) {
      if (param_6 == 0) goto DAT_1824aa350;
      if (*(uint *)(param_6 + 0x18) < 2) goto code_r0x0001824aa356;
      uVar27 = *(undefined8 *)(param_6 + 0x38);
      fVar18 = *(float *)(param_6 + 0x40);
    }
    else {
      if (*(uint *)(param_4 + 0x18) <= uVar9 + 2) goto code_r0x0001824aa356;
      uVar27 = *(undefined8 *)(param_4 + 0x20 + ((longlong)(int)uVar9 + 2) * 0xc);
      fVar18 = *(float *)(param_4 + 0x28 + ((longlong)(int)uVar9 + 2) * 0xc);
    }
    fStack_cc = (float)((ulonglong)uVar27 >> 0x20);
    fVar23 = (float)iVar1 * param_3 - (float)(int)uVar9;
    lVar15 = (longlong)(int)uVar9;
    fVar30 = fVar23 * fVar23 * fVar23;
    fVar3 = *(float *)(param_4 + 0x20 + lVar15 * 0xc);
    fVar4 = *(float *)(param_4 + 0x24 + lVar15 * 0xc);
    lVar16 = (longlong)(int)uVar9 + 1;
    fVar31 = fVar23 * fVar23;
    fVar5 = *(float *)(param_4 + 0x20 + lVar16 * 0xc);
    fVar6 = *(float *)(param_4 + 0x24 + lVar16 * 0xc);
    fVar20 = (float)((uint)fVar29 ^ _UNK_18529fe80);
    fVar24 = fVar4 * _UNK_18529f0f0;
    fVar21 = fVar4 * _UNK_18529f198;
    fVar25 = fVar6 * _UNK_18529f0f0;
    fVar26 = fVar6 * _UNK_18529f170;
    *param_1 = (fVar3 + fVar3 +
               (fVar5 + (float)((uint)fVar28 ^ _UNK_18529fe80)) * fVar23 +
               ((fVar5 * _UNK_18529f170 + ((fVar28 + fVar28) - fVar3 * _UNK_18529f198)) -
               (float)uVar27) * fVar31 +
               ((float)uVar27 +
               ((fVar3 * _UNK_18529f0f0 + (float)((uint)fVar28 ^ _UNK_18529fe80)) -
               fVar5 * _UNK_18529f0f0)) * fVar30) * _UNK_18529ee14;
    fVar28 = *(float *)(param_4 + 0x28 + lVar16 * 0xc);
    fVar3 = *(float *)(param_4 + 0x28 + lVar15 * 0xc);
    param_1[1] = (fVar4 + fVar4 +
                 (fVar6 + (float)((uint)fVar29 ^ uVar14)) * fVar23 +
                 ((fVar26 + ((fVar29 + fVar29) - fVar21)) - fStack_cc) * fVar31 +
                 (fStack_cc + ((fVar24 + fVar20) - fVar25)) * fVar30) * fVar10;
    param_1[2] = (fVar3 + fVar3 +
                 (fVar28 + (float)((uint)fVar19 ^ uVar14)) * fVar23 +
                 ((fVar28 * fVar12 + ((fVar19 + fVar19) - fVar3 * fVar13)) - fVar18) * fVar31 +
                 (fVar18 + ((fVar3 * fVar11 + (float)((uint)fVar19 ^ uVar14)) - fVar28 * fVar11)) *
                 fVar30) * fVar10;
    return param_1;
  }
code_r0x0001824aa356:
  FUN_180352460();
  pcVar8 = (code *)swi(3);
  pfVar17 = (float *)(*pcVar8)();
  return pfVar17;
}

