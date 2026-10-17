// blackboard_get_min VA 0x1816a9270 RVA 0x16a9270

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_1816a9270(longlong param_1,undefined8 param_2)

{
  float fVar1;
  longlong lVar2;
  code *pcVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  uint uVar7;
  ulonglong uVar8;
  undefined4 uStack_20;
  
  if (cRam0000000185a3c87a == '\0') {
    FUN_18035da50(0x1857a45a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a45b0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c87a = '\x01';
  }
  if (*(longlong *)(param_1 + 0x10) == 0) {
code_r0x0001816a9324:
    FUN_180352450();
    pcVar3 = (code *)swi(3);
    uVar8 = (*pcVar3)();
    return uVar8;
  }
  iVar4 = FUN_18332e340(*(longlong *)(param_1 + 0x10),param_2,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(lRam00000001857a45a0 + 0x20) + 0xc0) + 0x108))
  ;
  if (iVar4 < 0) {
    return (ulonglong)_UNK_18529f560;
  }
  if (*(longlong *)(param_1 + 0x10) == 0) goto code_r0x0001816a9324;
  lVar5 = FUN_18332db60(*(longlong *)(param_1 + 0x10),param_2,uRam00000001857a45b0);
  if (lVar5 == 0) goto code_r0x0001816a9324;
  if (cRam0000000185a3c883 == '\0') {
    FUN_18035da50(0x1857a4540,0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4538);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4528);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4520);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4530);
    LOCK();
    UNLOCK();
    cRam0000000185a3c883 = '\x01';
  }
  if (*(longlong *)(lVar5 + 0x28) != 0) {
    lVar5 = FUN_18002f710();
    if (lVar5 != 0) {
      lVar5 = *(longlong *)(lVar5 + 0x10);
      if (lVar5 != 0) {
        uStack_20 = 0;
        uVar8 = (ulonglong)_UNK_18529f560;
        while( true ) {
          if (lVar5 == 0) break;
          if (*(int *)(lVar5 + 0x2c) != *(int *)(lVar5 + 0x2c)) goto code_r0x0001816a9c86;
          do {
            if (lVar5 == 0) goto code_r0x0001816a9c81;
            if (*(uint *)(lVar5 + 0x20) <= uStack_20) {
              if (lVar5 != 0) {
                return uVar8;
              }
              goto code_r0x0001816a9c7c;
            }
            lVar2 = *(longlong *)(lVar5 + 0x18);
            lVar6 = (longlong)(int)uStack_20;
            uVar7 = uStack_20 + 1;
            if (lVar2 == 0) goto code_r0x0001816a9c77;
            if (*(uint *)(lVar2 + 0x18) <= uStack_20) goto code_r0x0001816a9c71;
            uStack_20 = uVar7;
          } while (*(int *)(lVar2 + 0x20 + lVar6 * 0x18) < 0);
          fVar1 = *(float *)(lVar2 + 0x30 + lVar6 * 0x18);
          if ((float)uVar8 < fVar1) {
            uVar8 = (ulonglong)(uint)fVar1;
          }
        }
        goto code_r0x0001816a9c8d;
      }
    }
  }
  FUN_180352450();
code_r0x0001816a9c71:
  FUN_180352460();
code_r0x0001816a9c77:
  FUN_180352450();
code_r0x0001816a9c7c:
  FUN_180352450();
code_r0x0001816a9c81:
  FUN_180352450();
code_r0x0001816a9c86:
  FUN_183aed800(0);
code_r0x0001816a9c8d:
  FUN_180352450();
  FUN_180352440();
  pcVar3 = (code *)swi(3);
  uVar8 = (*pcVar3)();
  return uVar8;
}

