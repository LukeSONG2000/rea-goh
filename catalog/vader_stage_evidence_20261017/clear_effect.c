// clear_effect VA 0x181a6d990 RVA 0x1a6d990

void FUN_181a6d990(longlong param_1,longlong param_2,undefined8 param_3,undefined1 param_4,
                  undefined8 param_5)

{
  byte bVar1;
  code *pcVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  byte *pbVar6;
  longlong lVar7;
  longlong lVar8;
  int iVar9;
  
  if (cRam0000000185a3e093 == '\0') {
    FUN_18035da50(0x1857c1bf8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810450);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810448);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810440);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858104a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857855c8);
    LOCK();
    UNLOCK();
    cRam0000000185a3e093 = '\x01';
  }
  if ((((param_2 != 0) && (lVar4 = FUN_1800084f0(1,uRam00000001857c1bf8,param_2), lVar4 != 0)) &&
      (*(longlong *)(lVar4 + 0x38) != 0)) && (lVar5 = *(longlong *)(param_1 + 0x58), lVar5 != 0)) {
    bVar1 = *(byte *)(*(longlong *)(lVar4 + 0x38) + 0x20);
    if (cRam0000000185a3d889 == '\0') {
      FUN_18035da50(0x185785618);
      LOCK();
      UNLOCK();
      cRam0000000185a3d889 = '\x01';
    }
    if (*(longlong *)(lVar5 + 0x28) != 0) {
      lVar4 = FUN_180033eb0();
      if ((*(longlong *)(param_1 + 0x58) != 0) &&
         (lVar5 = FUN_1819269a0(*(longlong *)(param_1 + 0x58),0), lVar5 != 0)) {
        iVar9 = 0;
        do {
          iVar3 = FUN_1800084f0(0,uRam0000000185810450,lVar5);
          if (iVar3 <= iVar9) {
            return;
          }
          if (lVar4 == 0) break;
          pbVar6 = (byte *)FUN_1800361b0();
          if (bVar1 <= *pbVar6) {
            if (*pbVar6 != bVar1) {
              return;
            }
            lVar7 = FUN_180036120();
            iVar3 = 0;
            if (lVar7 == 0) break;
            for (; iVar3 < *(int *)(lVar7 + 0x18); iVar3 = iVar3 + 1) {
              lVar8 = FUN_180007510(lVar7,iVar3);
              if (lVar8 == param_2) {
                FUN_181a6dbc0(param_1,lVar7,iVar3,param_3,param_4,param_5,0);
                return;
              }
            }
          }
          iVar9 = iVar9 + 1;
        } while( true );
      }
    }
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

