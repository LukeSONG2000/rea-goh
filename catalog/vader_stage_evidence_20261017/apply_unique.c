// apply_unique VA 0x181bbf410 RVA 0x1bbf410

void FUN_181bbf410(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  longlong *plVar4;
  code *pcVar5;
  int iVar6;
  undefined8 *puVar7;
  longlong lVar8;
  ushort uVar9;
  uint uStack_40;
  ulonglong uVar10;
  
  if (cRam0000000185a3e910 == '\0') {
    FUN_18035da50(0x185806bf0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810af8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ba8);
    LOCK();
    UNLOCK();
    cRam0000000185a3e910 = '\x01';
  }
  lVar2 = *(longlong *)(param_1 + 0x110);
  if (lVar2 == 0) {
    FUN_180352450();
    pcVar5 = (code *)swi(3);
    (*pcVar5)();
    return;
  }
  iVar1 = *(int *)(lVar2 + 0x1c);
  uStack_40 = 0;
  do {
    if (lVar2 == 0) {
code_r0x000181bbf777:
      FUN_180352450();
      FUN_180352440();
      pcVar5 = (code *)swi(3);
      (*pcVar5)();
      return;
    }
    if (iVar1 != *(int *)(lVar2 + 0x1c)) {
code_r0x000181bbf6dd:
      if ((*(byte *)(*(longlong *)(lRam0000000185806bc8 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(lRam0000000185806bc8 + 0x20));
      }
      if (lVar2 != 0) {
        if (iVar1 == *(int *)(lVar2 + 0x1c)) {
          return;
        }
code_r0x000181bbf76b:
        FUN_183aed800(0);
      }
      FUN_180352450();
      goto code_r0x000181bbf777;
    }
    if (*(uint *)(lVar2 + 0x18) <= uStack_40) goto code_r0x000181bbf6dd;
    lVar3 = *(longlong *)(lVar2 + 0x10);
    if (lVar3 == 0) {
code_r0x000181bbf766:
      FUN_180352450();
      goto code_r0x000181bbf76b;
    }
    if (*(uint *)(lVar3 + 0x18) <= uStack_40) {
      FUN_180352460();
code_r0x000181bbf73e:
      FUN_180352450();
code_r0x000181bbf743:
      FUN_180352450();
code_r0x000181bbf748:
      FUN_180352450();
code_r0x000181bbf74d:
      FUN_180352450();
code_r0x000181bbf752:
      FUN_180352450();
code_r0x000181bbf757:
      FUN_180352450();
code_r0x000181bbf75c:
      FUN_180352450();
code_r0x000181bbf761:
      FUN_180352450();
      goto code_r0x000181bbf766;
    }
    lVar3 = *(longlong *)(lVar3 + 0x20 + (longlong)(int)uStack_40 * 8);
    uStack_40 = uStack_40 + 1;
    plVar4 = *(longlong **)(param_1 + 0x58);
    if (plVar4 == (longlong *)0x0) goto code_r0x000181bbf761;
    lVar8 = *plVar4;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      uVar10 = 0;
      do {
        if (*(longlong *)(*(longlong *)(lVar8 + 0xb0) + uVar10 * 0x10) == lRam0000000185810af8) {
          puVar7 = (undefined8 *)
                   ((longlong)*(int *)(*(longlong *)(lVar8 + 0xb0) + 8 + uVar10 * 0x10) * 0x10 +
                    0x138 + lVar8);
          goto code_r0x000181bbf586;
        }
        uVar9 = (short)uVar10 + 1;
        uVar10 = (ulonglong)uVar9;
      } while (uVar9 < *(ushort *)(lVar8 + 0x12e));
    }
    puVar7 = (undefined8 *)FUN_18034f630(plVar4,lRam0000000185810af8,0);
code_r0x000181bbf586:
    lVar8 = (*(code *)*puVar7)(plVar4,0,0,0,puVar7[1]);
    if (lVar3 == 0) goto code_r0x000181bbf75c;
    if (lVar8 == 0) goto code_r0x000181bbf757;
    FUN_18184a110(lVar8,*(undefined8 *)(lVar3 + 0x38),6,param_1,lVar3,
                  *(undefined8 *)(param_1 + 0x88),1,0);
    FUN_181bc4890(param_1,lVar8,0);
    FUN_18184b450(lVar8,0,0,0,0);
    if (*(longlong *)(param_1 + 0x88) == 0) goto code_r0x000181bbf752;
    lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x20);
    if (cRam0000000185a3e8f5 == '\0') {
      FUN_18035da50(0x1858316b0);
      LOCK();
      UNLOCK();
      cRam0000000185a3e8f5 = '\x01';
    }
    if (*(longlong *)(param_1 + 0xe8) == 0) goto code_r0x000181bbf74d;
    iVar6 = FUN_1800084f0(0xc,uRam00000001858316b0);
    if (lVar3 == 0) goto code_r0x000181bbf748;
    if (cRam0000000185a3f0bb == '\0') {
      FUN_18035da50(0x185810af0);
      LOCK();
      UNLOCK();
      cRam0000000185a3f0bb = '\x01';
    }
    if (iVar6 == 0) {
      if (*(longlong *)(lVar3 + 0xd8) == 0) goto code_r0x000181bbf743;
      FUN_180035cd0(*(longlong *)(lVar3 + 0xd8),lVar8);
    }
    else {
      if (*(longlong *)(lVar3 + 0xe0) == 0) goto code_r0x000181bbf73e;
      FUN_180035cd0(*(longlong *)(lVar3 + 0xe0),lVar8);
    }
  } while( true );
}

