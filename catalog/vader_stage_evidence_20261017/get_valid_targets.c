// get_valid_targets VA 0x181baecf0 RVA 0x1baecf0

longlong FUN_181baecf0(longlong param_1,undefined8 param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  longlong lVar4;
  code *pcVar5;
  char cVar6;
  undefined8 uVar7;
  longlong lVar8;
  longlong lVar9;
  float fVar10;
  float fVar11;
  undefined4 uStack_50;
  
  if (cRam0000000185a3e8ab == '\0') {
    FUN_18035da50(0x1858318c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4808);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8fc0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857c2450);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9410);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9428);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185830790);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9648);
    LOCK();
    UNLOCK();
    cRam0000000185a3e8ab = '\x01';
  }
  lVar8 = *(longlong *)(param_1 + 0x18);
  uVar7 = FUN_1803a51b0(uRam00000001857a9648);
  FUN_180e639d0(uVar7,param_1,uRam00000001858318c8,0);
  if (lVar8 != 0) {
    lVar8 = FUN_1836f55d0(lVar8,uVar7,uRam00000001857c2450);
    lVar9 = FUN_1803a51b0(uRam00000001857a9428);
    FUN_182d09b30(lVar9,uRam00000001857a9410);
    if (lVar8 != 0) {
      iVar1 = *(int *)(lVar8 + 0x1c);
      uStack_50 = 0;
      fVar11 = 0.0;
      while (lVar8 != 0) {
        if (iVar1 != *(int *)(lVar8 + 0x1c)) {
code_r0x000181baefd5:
          if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
          }
          if (lVar8 == 0) goto code_r0x000181baf103;
          if (iVar1 != *(int *)(lVar8 + 0x1c)) goto code_r0x000181baf0fc;
          if (lVar9 == 0) goto code_r0x000181baf0e1;
          cVar6 = FUN_180014a60(lVar9,param_2);
          if (cVar6 != '\0') {
            lVar9 = FUN_1803a51b0(uRam00000001857a9428);
            FUN_182d09b30(lVar9,uRam00000001857a9410);
            lVar8 = lRam00000001857a9420;
            if (lVar9 == 0) goto code_r0x000181baf0e1;
            *(int *)(lVar9 + 0x1c) = *(int *)(lVar9 + 0x1c) + 1;
            lVar4 = *(longlong *)(lVar9 + 0x10);
            if (lVar4 == 0) goto code_r0x000181baf0e1;
            uVar3 = *(uint *)(lVar9 + 0x18);
            if (uVar3 < *(uint *)(lVar4 + 0x18)) {
              *(uint *)(lVar9 + 0x18) = uVar3 + 1;
              if (*(uint *)(lVar4 + 0x18) <= uVar3) {
                FUN_180352460();
                pcVar5 = (code *)swi(3);
                lVar8 = (*pcVar5)();
                return lVar8;
              }
              *(undefined8 *)(lVar4 + 0x20 + (longlong)(int)uVar3 * 8) = param_2;
            }
            else {
              FUN_1836f50c0(lVar9,param_2,
                            *(undefined8 *)
                             (*(longlong *)(*(longlong *)(lVar8 + 0x20) + 0xc0) + 0x70));
            }
          }
          return lVar9;
        }
        if (*(uint *)(lVar8 + 0x18) <= uStack_50) goto code_r0x000181baefd5;
        lVar4 = *(longlong *)(lVar8 + 0x10);
        if (lVar4 == 0) goto code_r0x000181baf0f7;
        if (*(uint *)(lVar4 + 0x18) <= uStack_50) goto code_r0x000181baf0e7;
        lVar4 = *(longlong *)(lVar4 + 0x20 + (longlong)(int)uStack_50 * 8);
        uStack_50 = uStack_50 + 1;
        if (lVar4 == 0) goto code_r0x000181baf0f2;
        fVar10 = (float)FUN_181bbae00(lVar4,CONCAT71((uint7)(uint3)((uint)iVar1 >> 8),0x3e),0);
        if (lVar9 == 0) goto code_r0x000181baf0ed;
        if (*(int *)(lVar9 + 0x18) == 0) {
code_r0x000181baefa4:
          FUN_18000d200(lVar9,lVar4);
          fVar11 = fVar10;
        }
        else {
          if (*(int *)(lRam0000000185830790 + 0xe4) == 0) {
            FUN_180359940();
          }
          cVar6 = FUN_180cadfd0(fVar10,fVar11);
          if (cVar6 == '\0') {
            if (fVar11 < fVar10) {
              *(int *)(lVar9 + 0x1c) = *(int *)(lVar9 + 0x1c) + 1;
              iVar2 = *(int *)(lVar9 + 0x18);
              *(undefined4 *)(lVar9 + 0x18) = 0;
              if (0 < iVar2) {
                FUN_183af4330(*(undefined8 *)(lVar9 + 0x10),0,iVar2,0);
              }
              goto code_r0x000181baefa4;
            }
          }
          else {
            FUN_18000d200(lVar9,lVar4);
          }
        }
      }
      goto code_r0x000181baf108;
    }
  }
code_r0x000181baf0e1:
  FUN_180352450();
code_r0x000181baf0e7:
  FUN_180352460();
code_r0x000181baf0ed:
  FUN_180352450();
code_r0x000181baf0f2:
  FUN_180352450();
code_r0x000181baf0f7:
  FUN_180352450();
code_r0x000181baf0fc:
  FUN_183aed800(0);
code_r0x000181baf103:
  FUN_180352450();
code_r0x000181baf108:
  FUN_180352450();
  FUN_180352440();
  pcVar5 = (code *)swi(3);
  lVar8 = (*pcVar5)();
  return lVar8;
}

