// sweep_affordable_attempts VA 0x1813a5e10 RVA 0x13a5e10

ulonglong FUN_1813a5e10(longlong param_1,longlong param_2)

{
  char cVar1;
  code *pcVar2;
  int iVar3;
  undefined4 uVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  ulonglong uVar8;
  undefined4 auStackX_10 [2];
  
  if (param_2 == 0) {
    return 0x7fffffff;
  }
  cVar1 = *(char *)(param_2 + 0x12);
  if (cVar1 == '\x01') {
    if ((((*(longlong *)(param_1 + 0x18) != 0) &&
         (lVar5 = FUN_181be3680(*(longlong *)(param_1 + 0x18),*(undefined8 *)(param_2 + 0x28),0),
         lVar5 != 0)) && (*(longlong *)(param_1 + 0x10) != 0)) &&
       ((lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0xd8), lVar7 != 0 &&
        (*(longlong *)(lVar7 + 0x48) != 0)))) {
      lVar6 = FUN_1810230a0(*(longlong *)(lVar7 + 0x48),*(undefined8 *)(lVar5 + 0x58),0);
      if ((lVar6 != 0) &&
         ((*(int *)(lVar6 + 0x10) != 0 && (lVar7 = FUN_180ffdde0(lVar7,lVar6,0), lVar7 != 0)))) {
        if (*(longlong *)(lVar7 + 0x40) == 0) goto DAT_1813a5f8d;
        if (*(byte *)(lVar5 + 0xc5) <= *(byte *)(*(longlong *)(lVar7 + 0x40) + 0xc5)) {
          return 0;
        }
      }
      return 0x7fffffff;
    }
  }
  else if (cVar1 == '\x02') {
    if (*(int *)(param_2 + 0x20) < 1) {
      return 0x7fffffff;
    }
    auStackX_10[0] = 0;
    FUN_1817f5f60(*(undefined8 *)(param_2 + 0x28),auStackX_10,0);
    if ((*(longlong *)(param_1 + 0x10) != 0) &&
       (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0xd8), lVar5 != 0)) {
      iVar3 = FUN_180fffb20(lVar5,auStackX_10[0],0);
      return (longlong)iVar3 / (longlong)*(int *)(param_2 + 0x20) & 0xffffffff;
    }
  }
  else {
    if (cVar1 != '\b') {
      return 0;
    }
    if (*(longlong *)(param_1 + 0x48) != 0) {
      uVar4 = FUN_1818f56d0(*(longlong *)(param_1 + 0x48),*(undefined8 *)(param_2 + 0x28),0);
      if (*(int *)(param_2 + 0x20) < 1) {
        return 0x7fffffff;
      }
      if (*(longlong *)(param_1 + 0x20) != 0) {
        iVar3 = FUN_1818da1c0(*(longlong *)(param_1 + 0x20),uVar4,0);
        return (longlong)iVar3 / (longlong)*(int *)(param_2 + 0x20) & 0xffffffff;
      }
    }
  }
DAT_1813a5f8d:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar8 = (*pcVar2)();
  return uVar8;
}

