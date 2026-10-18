// ai_module_evaluate VA 0x1816a5050 RVA 0x16a5050

ulonglong FUN_1816a5050(longlong *param_1,longlong param_2,undefined8 param_3,char param_4,
                       longlong *param_5)

{
  int iVar1;
  code *pcVar2;
  byte bVar3;
  undefined8 uVar4;
  longlong lVar5;
  ulonglong uVar6;
  
  if (cRam0000000185a3c856 == '\0') {
    FUN_18035da50(0x1857a4808);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9410);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9428);
    LOCK();
    UNLOCK();
    cRam0000000185a3c856 = '\x01';
  }
  if (param_4 == '\0') {
    lVar5 = FUN_1803a51b0(uRam00000001857a9428);
    uVar6 = FUN_182d09b30(lVar5,uRam00000001857a9410);
    *param_5 = lVar5;
    return uVar6 & 0xffffffffffffff00;
  }
  if (param_2 != 0) {
    uVar4 = FUN_1816a51a0(param_1,*(undefined4 *)(param_2 + 0x14),*(undefined1 *)(param_2 + 0x18),
                          param_3,0);
    bVar3 = (**(code **)(*param_1 + 0x1a8))
                      (param_1,param_2,uVar4,param_3,param_5,*(undefined8 *)(*param_1 + 0x1b0));
    if (*(char *)(param_2 + 0x44) != '\0') {
      bVar3 = bVar3 == 0;
    }
    if (bVar3 == 0) {
      lVar5 = *param_5;
      if (lVar5 == 0) goto DAT_1816a518d;
      iVar1 = *(int *)(lVar5 + 0x18);
      *(int *)(lVar5 + 0x1c) = *(int *)(lVar5 + 0x1c) + 1;
      *(undefined4 *)(lVar5 + 0x18) = 0;
      if (0 < iVar1) {
        FUN_183af4330(*(undefined8 *)(lVar5 + 0x10),0,iVar1,0);
      }
    }
    return (ulonglong)bVar3;
  }
DAT_1816a518d:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar6 = (*pcVar2)();
  return uVar6;
}

