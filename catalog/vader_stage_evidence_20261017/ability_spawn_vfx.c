// ability_spawn_vfx VA 0x180fd5730 RVA 0xfd5730

void FUN_180fd5730(longlong param_1)

{
  longlong lVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 uVar9;
  code *pcVar10;
  longlong lVar11;
  longlong *plVar12;
  longlong unaff_R14;
  bool bVar13;
  uint uStack_50;
  
  if (cRam0000000185a39eba == '\0') {
    FUN_18035da50(0x1857a9710);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9408);
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
    FUN_18035da50(0x18582ff08);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a39eba = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x50) != 0) &&
     (plVar5 = (longlong *)FUN_183c9efd0(*(longlong *)(param_1 + 0x50),0), plVar5 != (longlong *)0x0
     )) {
    if ((*(byte *)(lRam00000001857a9710 + 0x130) <= *(byte *)(*plVar5 + 0x130)) &&
       (lVar1 = *(longlong *)(*plVar5 + 200),
       *(longlong *)(lVar1 + -8 + (ulonglong)*(byte *)(lRam00000001857a9710 + 0x130) * 8) ==
       lRam00000001857a9710)) {
      plVar12 = (longlong *)0x0;
      plVar6 = plVar12;
      if (*(longlong *)(lVar1 + -8 + (ulonglong)*(byte *)(lRam00000001857a9710 + 0x130) * 8) ==
          lRam00000001857a9710) {
        plVar6 = plVar5;
      }
      lVar1 = plVar6[5];
      bVar13 = false;
      if (*(longlong *)(param_1 + 0x60) != 0) {
        lVar7 = FUN_183c9dd40(*(undefined8 *)(param_1 + 0x60),0);
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
        bVar2 = bVar13;
        if (lVar7 != 0) {
          if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
            FUN_180359940();
          }
          bVar2 = *(longlong *)(lVar7 + 0x10) != 0;
        }
        if (bVar2) {
          if (lVar1 == 0) goto code_r0x000180fd5c5f;
          iVar3 = FUN_1800084f0(4,uRam00000001857a97e0,lVar1);
          if (0 < iVar3) {
            lVar7 = FUN_1800084f0(6,uRam00000001857a97e0,lVar1);
            if (((*(longlong *)(param_1 + 0x60) == 0) ||
                (lVar8 = FUN_183c9dd40(*(longlong *)(param_1 + 0x60),0), lVar8 == 0)) ||
               (uVar9 = FUN_182b23520(lVar8,uRam000000018582ff08), lVar7 == 0))
            goto code_r0x000180fd5c5f;
            FUN_180fe6590(lVar7,uVar9,0);
          }
        }
      }
      if (*(longlong *)(param_1 + 0x58) == 0) {
code_r0x000180fd5c47:
        FUN_183cd87d0(param_1,0);
        return;
      }
      lVar7 = FUN_183c9dd40(*(undefined8 *)(param_1 + 0x58),0);
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
      if (lVar7 != 0) {
        if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
          FUN_180359940();
        }
        bVar13 = *(longlong *)(lVar7 + 0x10) != 0;
      }
      if (!bVar13) goto code_r0x000180fd5c47;
      if (((*(longlong *)(param_1 + 0x58) != 0) &&
          (lVar7 = FUN_183c9dd40(*(longlong *)(param_1 + 0x58),0), lVar7 != 0)) &&
         ((unaff_R14 = FUN_1844a54d0(lVar7,0), lVar1 != 0 &&
          (lVar7 = FUN_1800084f0(2,uRam00000001857a97e0,lVar1), lVar7 != 0)))) {
        iVar3 = *(int *)(lVar7 + 0x1c);
        uStack_50 = 0;
        while (lVar7 != 0) {
          if ((iVar3 != *(int *)(lVar7 + 0x1c)) || (*(uint *)(lVar7 + 0x18) <= uStack_50)) {
            if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
              FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
            }
            if (lVar7 == 0) goto code_r0x000180fd5ca6;
            if (iVar3 == *(int *)(lVar7 + 0x1c)) goto code_r0x000180fd5c47;
            goto code_r0x000180fd5c9f;
          }
          lVar8 = *(longlong *)(lVar7 + 0x10);
          if (lVar8 == 0) goto code_r0x000180fd5c9a;
          if (*(uint *)(lVar8 + 0x18) <= uStack_50) goto code_r0x000180fd5c65;
          lVar8 = *(longlong *)(lVar8 + 0x20 + (longlong)(int)uStack_50 * 8);
          uStack_50 = uStack_50 + 1;
          if (unaff_R14 == 0) goto code_r0x000180fd5c95;
          if (cRam0000000185a50966 == '\0') {
            FUN_18035da50(0x1857b8340);
            LOCK();
            UNLOCK();
            cRam0000000185a50966 = '\x01';
          }
          lVar11 = *(longlong *)(unaff_R14 + 0x10);
          if (lVar11 == 0) goto code_r0x000180fd5c8b;
          pcVar10 = pcRam0000000185a50ab0;
          if ((pcRam0000000185a50ab0 == (code *)0x0) &&
             (pcVar10 = (code *)FUN_1803679a0(&UNK_184b5ee70), pcVar10 == (code *)0x0))
          goto code_r0x000180fd5c6b;
          pcRam0000000185a50ab0 = pcVar10;
          iVar4 = (*pcRam0000000185a50ab0)(lVar11);
          if (iVar4 <= (int)plVar12) goto code_r0x000180fd5c47;
          if (lVar8 != lVar1) {
            if (lVar8 == 0) goto code_r0x000180fd5c86;
            if ((*(char *)(lVar8 + 0x1e8) == '\0') && (0 < *(int *)(lVar8 + 0xc0))) {
              lVar11 = FUN_1844be810(unaff_R14,plVar12,0);
              if (lVar11 == 0) goto code_r0x000180fd5c81;
              uVar9 = FUN_182add4e0(lVar11,uRam00000001857a9408);
              plVar12 = (longlong *)(ulonglong)((int)plVar12 + 1);
              FUN_181bcb390(lVar8,uVar9);
            }
          }
        }
        goto code_r0x000180fd5cab;
      }
    }
  }
code_r0x000180fd5c5f:
  FUN_180352450();
code_r0x000180fd5c65:
  FUN_180352460();
code_r0x000180fd5c6b:
  uVar9 = func_0x000180367810(&UNK_184b5ee70);
  FUN_1803660c0(uVar9,0);
code_r0x000180fd5c81:
  FUN_180352450();
code_r0x000180fd5c86:
  FUN_180352450();
code_r0x000180fd5c8b:
  FUN_1844c5660(unaff_R14,0);
code_r0x000180fd5c95:
  FUN_180352450();
code_r0x000180fd5c9a:
  FUN_180352450();
code_r0x000180fd5c9f:
  FUN_183aed800(0);
code_r0x000180fd5ca6:
  FUN_180352450();
code_r0x000180fd5cab:
  FUN_180352450();
  FUN_180352440();
  pcVar10 = (code *)swi(3);
  (*pcVar10)();
  return;
}

