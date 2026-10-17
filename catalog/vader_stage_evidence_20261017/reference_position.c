// reference_position VA 0x180fe6660 RVA 0xfe6660

undefined8 * FUN_180fe6660(undefined8 *param_1,undefined8 param_2,undefined4 param_3)

{
  longlong lVar1;
  longlong lVar2;
  code *pcVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  undefined8 uStack_18;
  undefined4 uStack_10;
  
  if (cRam0000000185a39f31 == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a39f31 = '\x01';
  }
  lVar2 = FUN_180fe65f0(param_2,param_3,0);
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
  if (lVar2 != 0) {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar2 + 0x10) != 0) {
      lVar2 = FUN_1844a54d0(lVar2,0);
      if (lVar2 == 0) {
        FUN_180352450();
        pcVar3 = (code *)swi(3);
        puVar5 = (undefined8 *)(*pcVar3)();
        return puVar5;
      }
      if (cRam0000000185a50930 == '\0') {
        FUN_18035da50(0x1857b8340);
        LOCK();
        UNLOCK();
        cRam0000000185a50930 = '\x01';
      }
      uStack_18 = 0;
      uStack_10 = 0;
      lVar1 = *(longlong *)(lVar2 + 0x10);
      if (lVar1 == 0) {
        FUN_1844c5660(lVar2,0);
        pcVar3 = (code *)swi(3);
        puVar5 = (undefined8 *)(*pcVar3)();
        return puVar5;
      }
      pcVar3 = pcRam0000000185a50980;
      if ((pcRam0000000185a50980 == (code *)0x0) &&
         (pcVar3 = (code *)FUN_1803679a0(&UNK_184b5e060), pcVar3 == (code *)0x0)) {
        uVar4 = func_0x000180367810(&UNK_184b5e060);
        FUN_1803660c0(uVar4,0);
        pcVar3 = (code *)swi(3);
        puVar5 = (undefined8 *)(*pcVar3)();
        return puVar5;
      }
      pcRam0000000185a50980 = pcVar3;
      (*pcRam0000000185a50980)(lVar1,&uStack_18);
      goto code_r0x000180fe67fd;
    }
  }
  if (cRam0000000185a33cad == '\0') {
    FUN_18035da50(0x1858208a0);
    LOCK();
    UNLOCK();
    cRam0000000185a33cad = '\x01';
  }
  uStack_18 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
  uStack_10 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
code_r0x000180fe67fd:
  *param_1 = uStack_18;
  *(undefined4 *)(param_1 + 1) = uStack_10;
  return param_1;
}

