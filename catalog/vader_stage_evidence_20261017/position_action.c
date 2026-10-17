// position_action VA 0x18131c040 RVA 0x131c040

void FUN_18131c040(longlong param_1)

{
  code *pcVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 *puVar4;
  undefined4 uVar5;
  longlong lVar6;
  undefined8 uVar7;
  undefined1 auStack_18 [16];
  
  if (cRam0000000185a3b140 == '\0') {
    FUN_18035da50(0x1857a0da0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9018);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a3b140 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x00018131c4d2;
  lVar2 = *(longlong *)(param_1 + 0x50);
  if (lVar2 == 0) {
    lVar2 = 0;
  }
  else if (*(int *)(lVar2 + 0x10) == 0) {
    lVar2 = FUN_183cbddf0(*(longlong *)(param_1 + 0x30),0);
  }
  else {
    if (*(longlong *)(lVar2 + 0x18) == 0) goto code_r0x00018131c4d2;
    lVar2 = FUN_183c9dd40(*(longlong *)(lVar2 + 0x18),0);
  }
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
  if (lVar2 == 0) {
code_r0x00018131c1a1:
    lVar2 = 0;
  }
  else {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar2 + 0x10) == 0) goto code_r0x00018131c1a1;
    lVar2 = FUN_182b23520(lVar2,uRam00000001857a9018);
  }
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
  if (lVar2 == 0) {
    return;
  }
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (*(longlong *)(lVar2 + 0x10) == 0) {
    return;
  }
  lVar2 = *(longlong *)(lVar2 + 0x28);
  if (lVar2 == 0) {
    return;
  }
  if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x00018131c4d2;
  lVar3 = *(longlong *)(param_1 + 0x58);
  if (lVar3 == 0) {
    lVar3 = 0;
  }
  else if (*(int *)(lVar3 + 0x10) == 0) {
    lVar3 = FUN_183cbddf0(*(longlong *)(param_1 + 0x30),0);
  }
  else {
    if (*(longlong *)(lVar3 + 0x18) == 0) goto code_r0x00018131c4d2;
    lVar3 = FUN_183c9dd40(*(longlong *)(lVar3 + 0x18),0);
  }
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
  if (lVar3 == 0) {
code_r0x00018131c341:
    lVar3 = 0;
  }
  else {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    lVar6 = lRam00000001857a0da0;
    if (*(longlong *)(lVar3 + 0x10) == 0) goto code_r0x00018131c341;
    if (*(longlong *)(lRam00000001857a0da0 + 0x38) == 0) {
      FUN_18034f250(lRam00000001857a0da0);
    }
    lVar3 = FUN_182b23910(lVar3,0,**(undefined8 **)(lVar6 + 0x38));
  }
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
  if (lVar3 == 0) {
    lVar6 = *(longlong *)(param_1 + 0x60);
  }
  else {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    lVar6 = *(longlong *)(param_1 + 0x60);
    if (*(longlong *)(lVar3 + 0x10) != 0) {
      puVar4 = (undefined8 *)FUN_180fd49a0(auStack_18,lVar3,lVar2,0);
      if (lVar6 == 0) goto code_r0x00018131c4d2;
      uVar5 = *(undefined4 *)(puVar4 + 1);
      uVar7 = *puVar4;
      goto code_r0x00018131c3f7;
    }
  }
  lVar3 = FUN_1800084f0(0,uRam00000001857a97e0,lVar2);
  if (lVar3 == 0) {
code_r0x00018131c493:
    if (cRam0000000185a33cad == '\0') {
      FUN_18035da50(0x1858208a0);
      LOCK();
      UNLOCK();
      cRam0000000185a33cad = '\x01';
    }
    uVar7 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
    uVar5 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
  }
  else {
    lVar3 = FUN_1800084f0(0,uRam00000001857a97e0,lVar2);
    if (lVar3 == 0) goto code_r0x00018131c4d2;
    if (*(longlong *)(lVar3 + 0x1d8) == 0) goto code_r0x00018131c493;
    lVar2 = FUN_1800084f0(0,uRam00000001857a97e0,lVar2);
    if ((lVar2 == 0) || (*(longlong *)(lVar2 + 0x1d8) == 0)) goto code_r0x00018131c4d2;
    puVar4 = (undefined8 *)FUN_1800158b0(auStack_18,7,uRam00000001857a97f8);
    uVar7 = *puVar4;
    uVar5 = *(undefined4 *)(puVar4 + 1);
  }
  if (lVar6 != 0) {
code_r0x00018131c3f7:
    *(undefined8 *)(lVar6 + 0x38) = uVar7;
    *(undefined4 *)(lVar6 + 0x40) = uVar5;
    return;
  }
code_r0x00018131c4d2:
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

