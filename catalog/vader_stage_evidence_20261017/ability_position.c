// ability_position VA 0x180fd49a0 RVA 0xfd49a0

float * FUN_180fd49a0(float *param_1,longlong param_2,longlong param_3)

{
  char cVar1;
  longlong lVar2;
  undefined8 *puVar3;
  code *pcVar4;
  float *pfVar5;
  undefined8 uVar6;
  int iVar7;
  longlong lVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  undefined8 uStack_b8;
  float fStack_b0;
  float fStack_a8;
  float fStack_a4;
  float fStack_a0;
  longlong lStack_98;
  undefined8 uStack_90;
  longlong lStack_88;
  undefined8 uStack_80;
  longlong *plStack_78;
  undefined8 uStack_70;
  
  if (cRam0000000185a39eb6 == '\0') {
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    cRam0000000185a39eb6 = '\x01';
  }
  lStack_98 = 0;
  uStack_90 = 0;
  lStack_88 = 0;
  if (*(int *)(param_2 + 0x20) == 0) {
    lVar2 = FUN_180fd54e0(param_2,param_3,0);
    if ((lVar2 != 0) && (lVar2 = FUN_1800084f0(6,uRam00000001857a97e0,lVar2), lVar2 != 0)) {
      puVar3 = (undefined8 *)FUN_180fe6660(&uStack_b8,lVar2,*(undefined4 *)(param_2 + 0x24),0);
      *(undefined8 *)param_1 = *puVar3;
      fStack_b0 = *(float *)(puVar3 + 1);
      goto code_r0x000180fd4a9e;
    }
  }
  else if (*(int *)(param_2 + 0x20) == 4) {
    if (cRam0000000185a33cad == '\0') {
      FUN_18035da50(0x1858208a0);
      LOCK();
      UNLOCK();
      cRam0000000185a33cad = '\x01';
    }
    pfVar5 = *(float **)(lRam00000001858208a0 + 0xb8);
    fVar10 = *pfVar5;
    fVar11 = pfVar5[1];
    fVar12 = pfVar5[2];
    fStack_a8 = fVar12;
    fStack_a4 = fVar11;
    fStack_a0 = fVar10;
    lVar2 = FUN_180fd55d0(param_2,param_3,0);
    if (lVar2 != 0) {
      iVar7 = 0;
      uStack_70 = 0;
      plStack_78 = (longlong *)((ulonglong)*(uint *)(lVar2 + 0x1c) << 0x20);
      uStack_90 = (longlong)plStack_78;
      lStack_88 = 0;
      uStack_80 = 0;
      plStack_78 = &lStack_98;
      lStack_98 = lVar2;
      while (lStack_98 != 0) {
        if (uStack_90._4_4_ != *(int *)(lStack_98 + 0x1c)) {
code_r0x000180fd4c85:
          if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
          }
          if (lStack_98 == 0) goto code_r0x000180fd4edd;
          if (uStack_90._4_4_ != *(int *)(lStack_98 + 0x1c)) goto code_r0x000180fd4ed6;
          uStack_90 = CONCAT44(uStack_90._4_4_,*(int *)(lStack_98 + 0x18) + 1);
          lStack_88 = 0;
          if (0 < iVar7) {
            fVar9 = (float)iVar7;
            *param_1 = fVar10 / fVar9;
            param_1[1] = fVar11 / fVar9;
            param_1[2] = fVar12 / fVar9;
            return param_1;
          }
          if ((((param_3 == 0) ||
               (lVar2 = FUN_1800084f0(3,uRam00000001857a97e0,param_3), lVar2 == 0)) ||
              (*(longlong *)(lVar2 + 0x10) == 0)) ||
             (lVar2 = FUN_1844a54d0(*(longlong *)(lVar2 + 0x10),0), lVar2 == 0))
          goto code_r0x000180fd4eb7;
          if (cRam0000000185a50930 == '\0') {
            FUN_18035da50(0x1857b8340);
            LOCK();
            UNLOCK();
            cRam0000000185a50930 = '\x01';
          }
          uStack_b8 = 0;
          fStack_b0 = 0.0;
          lVar8 = *(longlong *)(lVar2 + 0x10);
          if (lVar8 == 0) {
            FUN_1844c5660(lVar2,0);
            pcVar4 = (code *)swi(3);
            pfVar5 = (float *)(*pcVar4)();
            return pfVar5;
          }
          pcVar4 = pcRam0000000185a50980;
          if ((pcRam0000000185a50980 == (code *)0x0) &&
             (pcVar4 = (code *)FUN_1803679a0(&UNK_184b5e060), pcVar4 == (code *)0x0)) {
            uVar6 = func_0x000180367810(&UNK_184b5e060);
            FUN_1803660c0(uVar6,0);
            pcVar4 = (code *)swi(3);
            pfVar5 = (float *)(*pcVar4)();
            return pfVar5;
          }
          goto code_r0x000180fd4db6;
        }
        if (*(uint *)(lStack_98 + 0x18) <= (uint)uStack_90) goto code_r0x000180fd4c85;
        lVar2 = *(longlong *)(lStack_98 + 0x10);
        if (lVar2 == 0) goto code_r0x000180fd4ed1;
        if (*(uint *)(lVar2 + 0x18) <= (uint)uStack_90) goto code_r0x000180fd4ebd;
        lVar2 = *(longlong *)(lVar2 + 0x20 + (longlong)(int)(uint)uStack_90 * 8);
        uStack_90 = CONCAT44(uStack_90._4_4_,(uint)uStack_90 + 1);
        lStack_88 = lVar2;
        if (lVar2 == 0) goto code_r0x000180fd4ecc;
        if (cRam0000000185a3e905 == '\0') {
          FUN_18035da50(0x1857a97f8);
          LOCK();
          UNLOCK();
          cRam0000000185a3e905 = '\x01';
        }
        if (*(longlong *)(lVar2 + 0x1d8) == 0) goto code_r0x000180fd4ec7;
        cVar1 = FUN_1800084f0(4,uRam00000001857a97f8);
        if ((cVar1 != '\0') && (cVar1 = FUN_181bbc130(lVar2,0), cVar1 == '\0')) {
          if (*(longlong *)(lVar2 + 0x1e0) == 0) goto code_r0x000180fd4ec2;
          pfVar5 = (float *)FUN_180fe6660(&uStack_b8,*(longlong *)(lVar2 + 0x1e0),
                                          *(undefined4 *)(param_2 + 0x24),0);
          fVar10 = fVar10 + *pfVar5;
          fVar11 = fVar11 + pfVar5[1];
          fVar12 = fVar12 + pfVar5[2];
          iVar7 = iVar7 + 1;
          fStack_a8 = fVar12;
          fStack_a4 = fVar11;
          fStack_a0 = fVar10;
        }
      }
      goto code_r0x000180fd4ee2;
    }
  }
  else if ((param_3 != 0) && (lVar2 = FUN_1800084f0(3,uRam00000001857a97e0,param_3), lVar2 != 0)) {
    iVar7 = *(int *)(param_2 + 0x20);
    if (iVar7 == 1) {
      lVar2 = *(longlong *)(lVar2 + 0x10);
    }
    else if (iVar7 == 2) {
      lVar2 = *(longlong *)(lVar2 + 0x18);
    }
    else {
      if (iVar7 != 3) goto code_r0x000180fd4eb7;
      lVar2 = *(longlong *)(lVar2 + 0x20);
    }
    if ((lVar2 != 0) && (lVar2 = FUN_1844a54d0(lVar2,0), lVar2 != 0)) {
      if (cRam0000000185a50930 == '\0') {
        FUN_18035da50(0x1857b8340);
        LOCK();
        UNLOCK();
        cRam0000000185a50930 = '\x01';
      }
      uStack_b8 = 0;
      fStack_b0 = 0.0;
      lVar8 = *(longlong *)(lVar2 + 0x10);
      if (lVar8 == 0) {
        FUN_1844c5660(lVar2,0);
        pcVar4 = (code *)swi(3);
        pfVar5 = (float *)(*pcVar4)();
        return pfVar5;
      }
      pcVar4 = pcRam0000000185a50980;
      if ((pcRam0000000185a50980 == (code *)0x0) &&
         (pcVar4 = (code *)FUN_1803679a0(&UNK_184b5e060), pcVar4 == (code *)0x0)) {
        uVar6 = func_0x000180367810(&UNK_184b5e060);
        FUN_1803660c0(uVar6,0);
        pcVar4 = (code *)swi(3);
        pfVar5 = (float *)(*pcVar4)();
        return pfVar5;
      }
code_r0x000180fd4db6:
      pcRam0000000185a50980 = pcVar4;
      (*pcRam0000000185a50980)(lVar8,&uStack_b8);
      *(undefined8 *)param_1 = uStack_b8;
code_r0x000180fd4a9e:
      param_1[2] = fStack_b0;
      return param_1;
    }
  }
code_r0x000180fd4eb7:
  FUN_180352450();
code_r0x000180fd4ebd:
  FUN_180352460();
code_r0x000180fd4ec2:
  FUN_180352450();
code_r0x000180fd4ec7:
  FUN_180352450();
code_r0x000180fd4ecc:
  FUN_180352450();
code_r0x000180fd4ed1:
  FUN_180352450();
code_r0x000180fd4ed6:
  FUN_183aed800(0);
code_r0x000180fd4edd:
  FUN_180352450();
code_r0x000180fd4ee2:
  FUN_180352450();
  FUN_180352440();
  pcVar4 = (code *)swi(3);
  pfVar5 = (float *)(*pcVar4)();
  return pfVar5;
}

