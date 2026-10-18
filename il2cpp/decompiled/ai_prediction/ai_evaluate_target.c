// ai_evaluate_target VA 0x1816a4380 RVA 0x16a4380

bool FUN_1816a4380(longlong param_1,longlong param_2,undefined8 param_3,longlong param_4,
                  longlong param_5)

{
  code *pcVar1;
  undefined1 uVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  ulonglong uVar6;
  longlong lVar7;
  
  if (cRam0000000185a3c851 == '\0') {
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96b8);
    LOCK();
    UNLOCK();
    cRam0000000185a3c851 = '\x01';
  }
  if ((param_2 == 0) || (*(int *)(param_2 + 0x10) == 0)) {
    return false;
  }
  if (*(longlong *)(param_1 + 0x10) == 0) goto DAT_1816a4539;
  lVar4 = FUN_181be6e80(*(longlong *)(param_1 + 0x10),param_2,0);
  if (lVar4 != 0) {
    lVar5 = FUN_1803a51b0(uRam00000001857a96b8);
    if ((param_4 == 0) || (lVar5 == 0)) goto DAT_1816a4539;
    *(undefined8 *)(lVar5 + 0x18) = *(undefined8 *)(param_4 + 0x18);
    lVar7 = *(longlong *)(param_4 + 0x18);
    if (lVar7 == 0) goto DAT_1816a4539;
    if (cRam0000000185a3e8f5 == '\0') {
      FUN_18035da50(0x1858316b0);
      LOCK();
      UNLOCK();
      cRam0000000185a3e8f5 = '\x01';
    }
    if (*(longlong *)(lVar7 + 0xe8) == 0) goto DAT_1816a4539;
    iVar3 = FUN_1800084f0(0xc,uRam00000001858316b0);
    lVar7 = *(longlong *)(param_1 + 0x20);
    if (lVar7 == 0) goto DAT_1816a4539;
    if (iVar3 == 0) {
      lVar7 = *(longlong *)(lVar7 + 0x160);
    }
    else {
      lVar7 = *(longlong *)(lVar7 + 0x168);
    }
    if (lVar7 == 0) goto DAT_1816a4539;
    uVar6 = *(longlong *)(lVar7 + 0x10) * 0x5deece66d + 0xbU & 0xffffffffffff;
    *(ulonglong *)(lVar7 + 0x10) = uVar6;
    *(int *)(lVar5 + 0x38) = (int)(uVar6 >> 0x10);
    *(undefined8 *)(lVar5 + 0x10) = param_3;
    if (*(longlong *)(param_1 + 0x18) == 0) goto DAT_1816a4539;
    FUN_181bb0200(*(longlong *)(param_1 + 0x18),lVar4,lVar5,param_5,0);
  }
  if (param_5 != 0) {
    return 0 < *(int *)(param_5 + 0x18);
  }
DAT_1816a4539:
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  uVar2 = (*pcVar1)();
  return (bool)uVar2;
}

