// ability_rotation VA 0x180fd4f40 RVA 0xfd4f40

float * FUN_180fd4f40(float *param_1,longlong param_2,longlong param_3)

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
  longlong *plStack_b0;
  undefined8 uStack_a8;
  longlong lStack_a0;
  undefined8 uStack_98;
  longlong lStack_90;
  undefined1 auStack_88 [16];
  undefined8 uStack_78;
  undefined8 uStack_70;
  
  if (cRam0000000185a39eb7 == '\0') {
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
    cRam0000000185a39eb7 = '\x01';
  }
  lStack_a0 = 0;
  uStack_98 = 0;
  lStack_90 = 0;
  uStack_78 = 0;
  uStack_70 = 0;
  if (*(int *)(param_2 + 0x20) == 0) {
    lVar2 = FUN_180fd54e0(param_2,param_3,0);
    if ((lVar2 != 0) && (lVar2 = FUN_1800084f0(6,uRam00000001857a97e0,lVar2), lVar2 != 0)) {
      puVar3 = (undefined8 *)FUN_180fe6840(auStack_88,lVar2,*(undefined4 *)(param_2 + 0x24),0);
      goto code_r0x000180fd5038;
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
    lVar2 = FUN_180fd55d0(param_2,param_3,0);
    if (lVar2 != 0) {
      iVar7 = 0;
      uStack_a8 = 0;
      plStack_b0 = (longlong *)((ulonglong)*(uint *)(lVar2 + 0x1c) << 0x20);
      uStack_98 = (longlong)plStack_b0;
      lStack_90 = 0;
      uStack_b8 = 0;
      plStack_b0 = &lStack_a0;
      lStack_a0 = lVar2;
      while (lStack_a0 != 0) {
        if (uStack_98._4_4_ != *(int *)(lStack_a0 + 0x1c)) {
code_r0x000180fd5225:
          if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
          }
          if (lStack_a0 == 0) goto code_r0x000180fd5482;
          if (uStack_98._4_4_ != *(int *)(lStack_a0 + 0x1c)) goto code_r0x000180fd547b;
          uStack_98 = CONCAT44(uStack_98._4_4_,*(int *)(lStack_a0 + 0x18) + 1);
          lStack_90 = 0;
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
          goto code_r0x000180fd545c;
          if (cRam0000000185a50937 == '\0') {
            FUN_18035da50(0x1857b8340);
            LOCK();
            UNLOCK();
            cRam0000000185a50937 = '\x01';
          }
          uStack_b8 = 0;
          plStack_b0 = (longlong *)0x0;
          lVar8 = *(longlong *)(lVar2 + 0x10);
          if (lVar8 == 0) {
            FUN_1844c5660(lVar2,0);
            pcVar4 = (code *)swi(3);
            pfVar5 = (float *)(*pcVar4)();
            return pfVar5;
          }
          pcVar4 = pcRam0000000185a509b8;
          if ((pcRam0000000185a509b8 == (code *)0x0) &&
             (pcVar4 = (code *)FUN_1803679a0(&UNK_184b5d760), pcVar4 == (code *)0x0)) {
            uVar6 = func_0x000180367810(&UNK_184b5d760);
            FUN_1803660c0(uVar6,0);
            pcVar4 = (code *)swi(3);
            pfVar5 = (float *)(*pcVar4)();
            return pfVar5;
          }
          goto code_r0x000180fd5353;
        }
        if (*(uint *)(lStack_a0 + 0x18) <= (uint)uStack_98) goto code_r0x000180fd5225;
        lVar2 = *(longlong *)(lStack_a0 + 0x10);
        if (lVar2 == 0) goto code_r0x000180fd5476;
        if (*(uint *)(lVar2 + 0x18) <= (uint)uStack_98) goto code_r0x000180fd5462;
        lVar2 = *(longlong *)(lVar2 + 0x20 + (longlong)(int)(uint)uStack_98 * 8);
        uStack_98 = CONCAT44(uStack_98._4_4_,(uint)uStack_98 + 1);
        lStack_90 = lVar2;
        if (lVar2 == 0) goto code_r0x000180fd5471;
        if (cRam0000000185a3e905 == '\0') {
          FUN_18035da50(0x1857a97f8);
          LOCK();
          UNLOCK();
          cRam0000000185a3e905 = '\x01';
        }
        if (*(longlong *)(lVar2 + 0x1d8) == 0) goto code_r0x000180fd546c;
        cVar1 = FUN_1800084f0(4,uRam00000001857a97f8);
        if ((cVar1 != '\0') && (cVar1 = FUN_181bbc130(lVar2,0), cVar1 == '\0')) {
          if (*(longlong *)(lVar2 + 0x1e0) == 0) goto code_r0x000180fd5467;
          pfVar5 = (float *)FUN_180fe6840(auStack_88,*(longlong *)(lVar2 + 0x1e0),
                                          *(undefined4 *)(param_2 + 0x24),0);
          fVar10 = fVar10 + *pfVar5;
          fVar11 = fVar11 + pfVar5[1];
          fVar12 = fVar12 + pfVar5[2];
          iVar7 = iVar7 + 1;
        }
      }
      goto code_r0x000180fd5487;
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
      if (iVar7 != 3) goto code_r0x000180fd545c;
      lVar2 = *(longlong *)(lVar2 + 0x20);
    }
    if ((lVar2 != 0) && (lVar2 = FUN_1844a54d0(lVar2,0), lVar2 != 0)) {
      if (cRam0000000185a50937 == '\0') {
        FUN_18035da50(0x1857b8340);
        LOCK();
        UNLOCK();
        cRam0000000185a50937 = '\x01';
      }
      uStack_b8 = 0;
      plStack_b0 = (longlong *)0x0;
      lVar8 = *(longlong *)(lVar2 + 0x10);
      if (lVar8 == 0) {
        FUN_1844c5660(lVar2,0);
        pcVar4 = (code *)swi(3);
        pfVar5 = (float *)(*pcVar4)();
        return pfVar5;
      }
      pcVar4 = pcRam0000000185a509b8;
      if ((pcRam0000000185a509b8 == (code *)0x0) &&
         (pcVar4 = (code *)FUN_1803679a0(&UNK_184b5d760), pcVar4 == (code *)0x0)) {
        uVar6 = func_0x000180367810(&UNK_184b5d760);
        FUN_1803660c0(uVar6,0);
        pcVar4 = (code *)swi(3);
        pfVar5 = (float *)(*pcVar4)();
        return pfVar5;
      }
code_r0x000180fd5353:
      pcRam0000000185a509b8 = pcVar4;
      (*pcRam0000000185a509b8)(lVar8,&uStack_b8);
      uStack_78 = uStack_b8;
      uStack_70 = plStack_b0;
      puVar3 = (undefined8 *)FUN_180d6b050(auStack_88,&uStack_78);
code_r0x000180fd5038:
      *(undefined8 *)param_1 = *puVar3;
      param_1[2] = *(float *)(puVar3 + 1);
      return param_1;
    }
  }
code_r0x000180fd545c:
  FUN_180352450();
code_r0x000180fd5462:
  FUN_180352460();
code_r0x000180fd5467:
  FUN_180352450();
code_r0x000180fd546c:
  FUN_180352450();
code_r0x000180fd5471:
  FUN_180352450();
code_r0x000180fd5476:
  FUN_180352450();
code_r0x000180fd547b:
  FUN_183aed800(0);
code_r0x000180fd5482:
  FUN_180352450();
code_r0x000180fd5487:
  FUN_180352450();
  FUN_180352440();
  pcVar4 = (code *)swi(3);
  pfVar5 = (float *)(*pcVar4)();
  return pfVar5;
}

