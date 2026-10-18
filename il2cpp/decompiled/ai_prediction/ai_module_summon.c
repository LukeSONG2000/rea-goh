// ai_module_summon VA 0x1816a5e10 RVA 0x16a5e10

undefined8
FUN_1816a5e10(longlong param_1,undefined8 param_2,undefined8 param_3,longlong param_4,
             undefined8 *param_5)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  ulonglong uVar4;
  longlong lVar5;
  code *pcVar6;
  undefined8 uVar7;
  undefined4 uVar8;
  uint uStack_30;
  
  if (cRam0000000185a3c85f == '\0') {
    FUN_18035da50(0x185806bf0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ba8);
    LOCK();
    UNLOCK();
    cRam0000000185a3c85f = '\x01';
  }
  *param_5 = param_3;
  if (param_4 == 0) goto code_r0x0001816a6084;
  if (*(longlong *)(param_4 + 0x40) == 0) {
code_r0x0001816a5ee1:
    lVar3 = *(longlong *)(param_4 + 0x38);
    if (lVar3 == 0) goto code_r0x0001816a6084;
    iVar2 = *(int *)(lVar3 + 0x1c);
    uStack_30 = 0;
    do {
      if (lVar3 == 0) goto code_r0x0001816a60a6;
      if ((iVar2 != *(int *)(lVar3 + 0x1c)) || (*(uint *)(lVar3 + 0x18) <= uStack_30)) {
        if ((*(byte *)(*(longlong *)(lRam0000000185806bc8 + 0x20) + 0x135) & 1) == 0) {
          FUN_18034f1d0(*(longlong *)(lRam0000000185806bc8 + 0x20));
        }
        if (lVar3 == 0) goto code_r0x0001816a60a1;
        if (iVar2 == *(int *)(lVar3 + 0x1c)) {
          return 0;
        }
        goto code_r0x0001816a609a;
      }
      lVar5 = *(longlong *)(lVar3 + 0x10);
      if (lVar5 == 0) goto code_r0x0001816a6095;
      if (*(uint *)(lVar5 + 0x18) <= uStack_30) goto code_r0x0001816a608a;
      lVar5 = *(longlong *)(lVar5 + 0x20 + (longlong)(int)uStack_30 * 8);
      uStack_30 = uStack_30 + 1;
      if (lVar5 == 0) goto code_r0x0001816a6090;
      uVar8 = *(undefined4 *)(lVar5 + 0x50);
      lVar5 = *(longlong *)(lVar5 + 0x48);
    } while (lVar5 == 0);
    cVar1 = *(char *)(lVar5 + 0x17);
    uVar4 = *(ulonglong *)(lVar5 + 0xf0);
  }
  else {
    uVar8 = *(undefined4 *)(*(longlong *)(param_4 + 0x40) + 0x50);
    lVar3 = *(longlong *)(*(longlong *)(param_4 + 0x40) + 0x48);
    if (lVar3 == 0) goto code_r0x0001816a5ee1;
    cVar1 = *(char *)(lVar3 + 0x17);
    uVar4 = *(ulonglong *)(lVar3 + 0xf0);
  }
  if ((uVar4 & 0x80000000000000) != 0) {
    return 1;
  }
  if (cVar1 == '\0') {
    return 0;
  }
  if ((*(longlong *)(param_1 + 0x10) != 0) &&
     (lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x88), lVar3 != 0)) {
    uVar7 = FUN_18138e9b0(lVar3,uVar8,cVar1,0);
    return uVar7;
  }
code_r0x0001816a6084:
  FUN_180352450();
code_r0x0001816a608a:
  FUN_180352460();
code_r0x0001816a6090:
  FUN_180352450();
code_r0x0001816a6095:
  FUN_180352450();
code_r0x0001816a609a:
  FUN_183aed800(0);
code_r0x0001816a60a1:
  FUN_180352450();
code_r0x0001816a60a6:
  FUN_180352450();
  FUN_180352440();
  pcVar6 = (code *)swi(3);
  uVar7 = (*pcVar6)();
  return uVar7;
}

