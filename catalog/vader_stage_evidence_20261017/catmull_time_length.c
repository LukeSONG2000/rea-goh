// catmull_time_length VA 0x1824aa360 RVA 0x24aa360

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1824aa360(longlong *param_1,longlong param_2,int param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  code *pcVar4;
  longlong lVar5;
  longlong lVar6;
  float *pfVar7;
  ulonglong uVar8;
  uint uVar9;
  double dVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  undefined1 auStack_c8 [176];
  
  if (cRam0000000185a423a8 == '\0') {
    FUN_18035da50(0x185821900);
    LOCK();
    UNLOCK();
    cRam0000000185a423a8 = '\x01';
  }
  fVar16 = _UNK_18529ef3c / (float)param_3;
  lVar5 = FUN_1803a3dc0(uRam0000000185821900,param_3);
  lVar6 = FUN_1803a3dc0(uRam0000000185821900,param_3);
  if (param_2 != 0) {
    fVar14 = 0.0;
    pfVar7 = (float *)(**(code **)(*param_1 + 0x188))
                                (auStack_c8,param_1,0,*(undefined8 *)(param_2 + 0x18),param_2,
                                 *(undefined8 *)(param_2 + 0x30),*(undefined8 *)(*param_1 + 400));
    if (1 < param_3 + 1) {
      uVar9 = 1;
      fVar12 = *pfVar7;
      fVar11 = pfVar7[1];
      fVar13 = pfVar7[2];
      do {
        fVar15 = (float)(int)uVar9 * fVar16;
        pfVar7 = (float *)(**(code **)(*param_1 + 0x188))
                                    (auStack_c8,param_1,fVar15,*(undefined8 *)(param_2 + 0x18),
                                     param_2,*(undefined8 *)(param_2 + 0x30),
                                     *(undefined8 *)(*param_1 + 400));
        fVar1 = *pfVar7;
        fVar2 = pfVar7[1];
        fVar3 = pfVar7[2];
        if (cRam0000000185a33cbb == '\0') {
          FUN_18035da50(0x185827178);
          LOCK();
          UNLOCK();
          cRam0000000185a33cbb = '\x01';
        }
        fVar12 = fVar1 - fVar12;
        fVar11 = fVar2 - fVar11;
        fVar13 = fVar3 - fVar13;
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        dVar10 = (double)(fVar11 * fVar11 + fVar12 * fVar12 + fVar13 * fVar13);
        if (dVar10 < 0.0) {
          dVar10 = (double)FUN_180babb60();
        }
        else {
          dVar10 = SQRT(dVar10);
        }
        if (lVar5 == 0) goto code_r0x0001824aa652;
        if (*(uint *)(lVar5 + 0x18) <= (uint)((ulonglong)uVar9 - 1)) {
code_r0x0001824aa658:
          FUN_180352460();
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        *(float *)(lVar5 + 0x20 + ((ulonglong)uVar9 - 1) * 4) = fVar15;
        if (lVar6 == 0) goto code_r0x0001824aa652;
        uVar8 = (ulonglong)uVar9;
        if (*(uint *)(lVar6 + 0x18) <= (uint)(uVar8 - 1)) goto code_r0x0001824aa658;
        fVar14 = fVar14 + (float)dVar10;
        uVar9 = uVar9 + 1;
        *(float *)(lVar6 + 0x20 + (uVar8 - 1) * 4) = fVar14;
        fVar12 = fVar1;
        fVar11 = fVar2;
        fVar13 = fVar3;
      } while ((int)uVar9 < param_3 + 1);
    }
    *(float *)(param_2 + 0x38) = fVar14;
    *(longlong *)(param_2 + 0x40) = lVar5;
    *(longlong *)(param_2 + 0x48) = lVar6;
    return;
  }
code_r0x0001824aa652:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

