// reference_rotation VA 0x180fe6840 RVA 0xfe6840

undefined8 * FUN_180fe6840(undefined8 *param_1,undefined8 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong lVar3;
  code *pcVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  if (cRam0000000185a39f32 == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a39f32 = '\x01';
  }
  uStack_18 = 0;
  uStack_10 = 0;
  lVar3 = FUN_180fe65f0(param_2,param_3,0);
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
  if (lVar3 != 0) {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar3 + 0x10) != 0) {
      lVar3 = FUN_1844a54d0(lVar3,0);
      if (lVar3 == 0) {
        FUN_180352450();
        pcVar4 = (code *)swi(3);
        puVar5 = (undefined8 *)(*pcVar4)();
        return puVar5;
      }
      if (cRam0000000185a50937 == '\0') {
        FUN_18035da50(0x1857b8340);
        LOCK();
        UNLOCK();
        cRam0000000185a50937 = '\x01';
      }
      uStack_28 = 0;
      uStack_20 = 0;
      lVar2 = *(longlong *)(lVar3 + 0x10);
      if (lVar2 == 0) {
        FUN_1844c5660(lVar3,0);
        pcVar4 = (code *)swi(3);
        puVar5 = (undefined8 *)(*pcVar4)();
        return puVar5;
      }
      pcVar4 = pcRam0000000185a509b8;
      if ((pcRam0000000185a509b8 == (code *)0x0) &&
         (pcVar4 = (code *)FUN_1803679a0(&UNK_184b5d760), pcVar4 == (code *)0x0)) {
        uVar6 = func_0x000180367810(&UNK_184b5d760);
        FUN_1803660c0(uVar6,0);
        pcVar4 = (code *)swi(3);
        puVar5 = (undefined8 *)(*pcVar4)();
        return puVar5;
      }
      pcRam0000000185a509b8 = pcVar4;
      (*pcRam0000000185a509b8)(lVar2,&uStack_28);
      uStack_18 = uStack_28;
      uStack_10 = uStack_20;
      puVar5 = (undefined8 *)FUN_180d6b050(&uStack_28,&uStack_18);
      goto code_r0x000180fe69ea;
    }
  }
  if (cRam0000000185a33cad == '\0') {
    FUN_18035da50(0x1858208a0);
    LOCK();
    UNLOCK();
    cRam0000000185a33cad = '\x01';
  }
  puVar5 = *(undefined8 **)(lRam00000001858208a0 + 0xb8);
code_r0x000180fe69ea:
  uVar1 = *(undefined4 *)(puVar5 + 1);
  *param_1 = *puVar5;
  *(undefined4 *)(param_1 + 1) = uVar1;
  return param_1;
}

