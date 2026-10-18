// store_setup_items VA 0x181c971d0 RVA 0x1c971d0

void FUN_181c971d0(longlong param_1)

{
  uint uVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong lVar7;
  code *pcVar8;
  uint uVar9;
  longlong lStackX_8;
  
  if (cRam0000000185a3ee55 == '\0') {
    FUN_18035da50(0x1857fe308);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858100a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e25e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185797d28);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e2488);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185797d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857bfac8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d95b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858213b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185797d10);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185797cc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e2480);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e1770);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e1758);
    LOCK();
    UNLOCK();
    cRam0000000185a3ee55 = '\x01';
  }
  uVar6 = FUN_1803a51b0(uRam0000000185797d10);
  FUN_182d09b30(uVar6,uRam0000000185797d00);
  lVar7 = *(longlong *)(param_1 + 0x368);
  uVar9 = 0;
  *(undefined8 *)(param_1 + 0x378) = uVar6;
  do {
    if (lVar7 == 0) {
code_r0x000181c9761e:
      FUN_180352450();
      pcVar8 = (code *)swi(3);
      (*pcVar8)();
      return;
    }
    if (*(int *)(lVar7 + 0x18) <= (int)uVar9) {
      lVar7 = *(longlong *)(param_1 + 0x378);
      uVar6 = FUN_1803a51b0(uRam00000001857e25e0);
      FUN_1832562a0(uVar6,0,uRam00000001857e2480,0);
      if (lVar7 != 0) {
        FUN_1836de080(lVar7,uVar6,uRam00000001857e2488);
        lVar7 = *(longlong *)(param_1 + 0x338);
        if (lVar7 != 0) {
          if (cRam0000000185a502f7 == '\0') {
            FUN_18035da50(0x1857c8288);
            LOCK();
            UNLOCK();
            FUN_18035da50(0x1857c81f8);
            LOCK();
            UNLOCK();
            cRam0000000185a502f7 = '\x01';
          }
          lVar3 = *(longlong *)(lVar7 + 0x10);
          if (lVar3 == 0) {
            FUN_1844c5660(lVar7,0);
            pcVar8 = (code *)swi(3);
            (*pcVar8)();
            return;
          }
          pcVar8 = pcRam0000000185a50308;
          if ((pcRam0000000185a50308 == (code *)0x0) &&
             (pcVar8 = (code *)FUN_1803679a0(&UNK_184b59e40), pcVar8 == (code *)0x0)) {
            uVar6 = func_0x000180367810(&UNK_184b59e40);
            FUN_1803660c0(uVar6,0);
            pcVar8 = (code *)swi(3);
            (*pcVar8)();
            return;
          }
          pcRam0000000185a50308 = pcVar8;
          uVar6 = (*pcRam0000000185a50308)(lVar3);
          lVar7 = FUN_182c7e680(uVar6,uRam00000001857c81f8);
          if ((*(longlong *)(param_1 + 0x368) != 0) && (lVar7 != 0)) {
            iVar2 = *(int *)(*(longlong *)(param_1 + 0x368) + 0x18);
            if (cRam0000000185a5034e == '\0') {
              FUN_18035da50(0x1857b8000);
              LOCK();
              UNLOCK();
              cRam0000000185a5034e = '\x01';
            }
            lVar3 = *(longlong *)(lVar7 + 0x10);
            if (lVar3 != 0) {
              pcVar8 = pcRam0000000185a503c8;
              if ((pcRam0000000185a503c8 == (code *)0x0) &&
                 (pcVar8 = (code *)FUN_1803679a0(&UNK_184b5af80), pcVar8 == (code *)0x0)) {
                uVar6 = func_0x000180367810(&UNK_184b5af80);
                FUN_1803660c0(uVar6,0);
                pcVar8 = (code *)swi(3);
                (*pcVar8)();
                return;
              }
              pcRam0000000185a503c8 = pcVar8;
                    /* WARNING: Could not recover jumptable at 0x000181c97610. Too many branches */
                    /* WARNING: Treating indirect jump as call */
              (*pcRam0000000185a503c8)(lVar3,iVar2 == 0);
              return;
            }
            FUN_1844c5660(lVar7,0);
            pcVar8 = (code *)swi(3);
            (*pcVar8)();
            return;
          }
        }
      }
      goto code_r0x000181c9761e;
    }
    lVar7 = *(longlong *)(param_1 + 0x368);
    if (lVar7 == 0) goto code_r0x000181c9761e;
    if (*(uint *)(lVar7 + 0x18) <= uVar9) {
code_r0x000181c9762a:
      FUN_183aeda10(0);
      pcVar8 = (code *)swi(3);
      (*pcVar8)();
      return;
    }
    lVar7 = *(longlong *)(lVar7 + 0x10);
    if (lVar7 == 0) goto code_r0x000181c9761e;
    if (*(uint *)(lVar7 + 0x18) <= uVar9) {
code_r0x000181c97624:
      FUN_180352460();
      pcVar8 = (code *)swi(3);
      (*pcVar8)();
      return;
    }
    lStackX_8 = (ulonglong)*(uint *)(param_1 + 0x360) << 0x20;
    lVar3 = *(longlong *)(param_1 + 0x370);
    lStackX_8 = CONCAT71(lStackX_8._1_7_,1);
    if (lVar3 == 0) goto code_r0x000181c9761e;
    if (*(uint *)(lVar3 + 0x18) <= uVar9) goto code_r0x000181c9762a;
    lVar3 = *(longlong *)(lVar3 + 0x10);
    if (lVar3 == 0) goto code_r0x000181c9761e;
    if (*(uint *)(lVar3 + 0x18) <= uVar9) goto code_r0x000181c97624;
    if (*(longlong *)(param_1 + 0x348) == 0) goto code_r0x000181c9761e;
    lVar7 = FUN_181c917c0(*(longlong *)(param_1 + 0x348),
                          *(undefined8 *)(lVar7 + 0x20 + (longlong)(int)uVar9 * 8),uVar9,lStackX_8,
                          *(undefined4 *)(lVar3 + 0x20 + (longlong)(int)uVar9 * 4),0,0);
    uVar6 = FUN_1803a51b0(uRam00000001858100a0);
    FUN_180c7c3a0(uVar6,param_1,uRam00000001857e1758,0);
    if (lVar7 == 0) goto code_r0x000181c9761e;
    *(undefined8 *)(lVar7 + 0x70) = uVar6;
    uVar6 = FUN_1803a51b0(uRam00000001857fe308);
    FUN_180c84ee0(uVar6,param_1,uRam00000001857e1770,0);
    *(undefined8 *)(lVar7 + 0x58) = uVar6;
    lVar5 = lRam0000000185797d28;
    lVar3 = *(longlong *)(param_1 + 0x378);
    if (lVar3 == 0) goto code_r0x000181c9761e;
    *(int *)(lVar3 + 0x1c) = *(int *)(lVar3 + 0x1c) + 1;
    lVar4 = *(longlong *)(lVar3 + 0x10);
    if (lVar4 == 0) goto code_r0x000181c9761e;
    uVar1 = *(uint *)(lVar3 + 0x18);
    if (uVar1 < *(uint *)(lVar4 + 0x18)) {
      *(uint *)(lVar3 + 0x18) = uVar1 + 1;
      if (*(uint *)(lVar4 + 0x18) <= uVar1) goto code_r0x000181c97624;
      *(longlong *)(lVar4 + 0x20 + (longlong)(int)uVar1 * 8) = lVar7;
    }
    else {
      FUN_1836f50c0(lVar3,lVar7,
                    *(undefined8 *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x70));
    }
    lVar7 = *(longlong *)(param_1 + 0x368);
    uVar9 = uVar9 + 1;
  } while( true );
}

