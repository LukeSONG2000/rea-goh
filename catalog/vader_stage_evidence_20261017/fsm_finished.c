// fsm_finished VA 0x180fd2c60 RVA 0xfd2c60

void FUN_180fd2c60(longlong param_1)

{
  undefined8 uVar1;
  char cVar2;
  uint uVar3;
  int iVar4;
  longlong lVar5;
  code *pcVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  undefined8 uVar12;
  ulonglong uVar13;
  bool bVar14;
  
  if (cRam0000000185a39ea9 == '\0') {
    FUN_18035da50(0x1857a9610);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ff470);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9608);
    LOCK();
    UNLOCK();
    cRam0000000185a39ea9 = '\x01';
  }
  lVar8 = *(longlong *)(param_1 + 0x98);
  if (lVar8 == 0) goto code_r0x000180fd3074;
  uVar13 = 0;
  while (uVar3 = (uint)uVar13, (int)uVar3 < *(int *)(lVar8 + 0x18)) {
    if (*(uint *)(lVar8 + 0x18) <= uVar3) {
      FUN_180352460();
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    lVar9 = *(longlong *)(lVar8 + 0x20 + (longlong)(int)uVar3 * 8);
    if ((lVar9 == 0) || (lVar5 = FUN_1844a1660(lVar9), lVar5 == 0)) goto code_r0x000180fd3074;
    if (cRam0000000185a50966 == '\0') {
      FUN_18035da50(0x1857b8340);
      LOCK();
      UNLOCK();
      cRam0000000185a50966 = '\x01';
    }
    lVar7 = *(longlong *)(lVar5 + 0x10);
    if (lVar7 == 0) {
      FUN_1844c5660(lVar5,0);
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    pcVar6 = pcRam0000000185a50ab0;
    if ((pcRam0000000185a50ab0 == (code *)0x0) &&
       (pcVar6 = (code *)FUN_1803679a0(&UNK_184b5ee70), pcVar6 == (code *)0x0)) {
      uVar12 = func_0x000180367810(&UNK_184b5ee70);
      FUN_1803660c0(uVar12,0);
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    pcRam0000000185a50ab0 = pcVar6;
    iVar4 = (*pcRam0000000185a50ab0)(lVar7);
    while (iVar4 = iVar4 + -1, -1 < iVar4) {
      if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x000180fd3074;
      lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x2d0);
      lVar7 = FUN_1844a1660(lVar9,0);
      if (((lVar7 == 0) || (lVar7 = FUN_1844be810(lVar7,iVar4,0), lVar7 == 0)) ||
         (FUN_1844a1720(lVar7,0), lVar5 == 0)) goto code_r0x000180fd3074;
      FUN_180dd7c80(lVar5);
    }
    uVar13 = (ulonglong)(uVar3 + 1);
  }
  FUN_1811851f0(*(undefined8 *)(param_1 + 0x60),0);
  if (*(longlong *)(param_1 + 0x28) == 0) goto code_r0x000180fd3074;
  lVar8 = FUN_180015b30();
  uVar12 = FUN_1803a51b0(uRam00000001857ff470);
  FUN_180c84ee0(uVar12,param_1,uRam00000001857a9610,0);
  if (lVar8 == 0) goto code_r0x000180fd3074;
  if (cRam0000000185a39f5e == '\0') {
    FUN_18035da50(0x1857ff470);
    LOCK();
    UNLOCK();
    cRam0000000185a39f5e = '\x01';
  }
  uVar13 = *(ulonglong *)(lVar8 + 0x10);
  do {
    lVar9 = FUN_183b250a0(uVar13,uVar12,0);
    uVar1 = uRam00000001857ff470;
    uVar10 = 0;
    if ((lVar9 != 0) && (uVar10 = FUN_1803a50f0(lVar9,uRam00000001857ff470), uVar10 == 0)) {
      FUN_180352e70(lVar9,uVar1);
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    LOCK();
    uVar11 = *(ulonglong *)(lVar8 + 0x10);
    bVar14 = uVar13 == uVar11;
    if (bVar14) {
      *(ulonglong *)(lVar8 + 0x10) = uVar10;
      uVar11 = uVar13;
    }
    UNLOCK();
    uVar13 = uVar11;
  } while (!bVar14);
  if (*(longlong *)(param_1 + 0x28) == 0) goto code_r0x000180fd3074;
  lVar8 = FUN_180015b30();
  if ((((*(longlong *)(param_1 + 0x38) == 0) ||
       (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x30), lVar9 == 0)) ||
      ((lVar9 = *(longlong *)(lVar9 + 0x98), lVar9 == 0 ||
       (((*(longlong *)(lVar9 + 0x40) == 0 ||
         (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x38), lVar9 == 0)) ||
        (*(longlong *)(lVar9 + 0x70) == 0)))))) || (lVar8 == 0)) goto code_r0x000180fd3074;
  if (cRam0000000185a39f68 == '\0') {
    FUN_18035da50(0x1857ad6e8);
    LOCK();
    UNLOCK();
    cRam0000000185a39f68 = '\x01';
  }
  if (*(longlong *)(lVar8 + 0x18) != 0) {
    FUN_180005650(10,uRam00000001857ad6e8,*(undefined8 *)(lVar8 + 0x18));
  }
  if (*(longlong *)(param_1 + 0x68) == 0) {
    return;
  }
  if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x000180fd3074;
  lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x130);
  if (*(char *)(param_1 + 0x80) == '\0') {
code_r0x000180fd2fb3:
    lVar9 = *(longlong *)(param_1 + 0x68);
    *(undefined8 *)(param_1 + 0x68) = 0;
    if (lVar9 == 0) goto code_r0x000180fd3074;
    (**(code **)(lVar9 + 0x18))(*(undefined8 *)(lVar9 + 0x40),*(undefined8 *)(lVar9 + 0x28));
  }
  else {
    *(undefined1 *)(param_1 + 0x81) = 1;
    if (*(char *)(param_1 + 0x82) != '\0') {
      lVar9 = *(longlong *)(param_1 + 0x70);
      *(undefined8 *)(param_1 + 0x70) = 0;
      if (lVar9 == 0) goto code_r0x000180fd3074;
      (**(code **)(lVar9 + 0x18))(*(undefined8 *)(lVar9 + 0x40),*(undefined8 *)(lVar9 + 0x28));
      goto code_r0x000180fd2fb3;
    }
  }
  uVar12 = *(undefined8 *)(param_1 + 0x78);
  *(undefined8 *)(param_1 + 0x78) = 0;
  if (lVar8 != 0) {
    FUN_182de1d70(lVar8,uRam00000001857a9608);
    lVar8 = FUN_1844a1720(param_1,0);
    if (lVar8 != 0) {
      cVar2 = FUN_1844a5790(lVar8,0);
      if (cVar2 != '\0') {
        if (cRam0000000185a39eaa == '\0') {
          FUN_18035da50(0x1857a9600);
          LOCK();
          UNLOCK();
          cRam0000000185a39eaa = '\x01';
        }
        lVar8 = FUN_1803a51b0(uRam00000001857a9600);
        *(undefined4 *)(lVar8 + 0x10) = 0;
        *(longlong *)(lVar8 + 0x20) = param_1;
        *(undefined8 *)(lVar8 + 0x28) = uVar12;
        FUN_1844a8030(param_1,lVar8,0);
      }
      return;
    }
  }
code_r0x000180fd3074:
  FUN_180352450();
  pcVar6 = (code *)swi(3);
  (*pcVar6)();
  return;
}

