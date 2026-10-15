// ==== BattleFsmController_ResetSpeedForCurrentUnit @ 0x18193b7b0 ====

void BattleFsmController_ResetSpeedForCurrentUnit(longlong param_1)

{
  longlong lVar1;
  
  if ((*(longlong *)(param_1 + 0x78) != 0) &&
     (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 != 0)) {
    BattleUnitController_SetSpeedFill(lVar1,0,0);
    if ((*(longlong *)(param_1 + 0x78) != 0) &&
       (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 != 0)) {
      *(undefined4 *)(lVar1 + 0x20c) = 0;
      BattleFsmController_StartPlayerUnitMove(param_1,0);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== BattleFsmController_TriggerTurnStarted @ 0x18193b880 ====

void BattleFsmController_TriggerTurnStarted(longlong param_1)

{
  longlong lVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  
  if (DAT_185a3d90c == '\0') {
    FUN_18035da50(&DAT_1857c2ef8);
    LOCK();
    UNLOCK();
    DAT_185a3d90c = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x78) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x110) != 0)) {
    iVar4 = FUN_1800084f0(0,DAT_1857c2ef8);
    if ((*(longlong *)(param_1 + 0x78) != 0) &&
       (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x20), lVar1 != 0)) {
      if (DAT_185a3f0c6 == '\0') {
        FUN_18035da50(&DAT_1857f5f78);
        LOCK();
        UNLOCK();
        DAT_185a3f0c6 = '\x01';
      }
      if (*(longlong *)(lVar1 + 0x80) != 0) {
        bVar2 = FUN_1800084f0(6,DAT_1857f5f78);
        bVar2 = bVar2 ^ 1;
        if (iVar4 != 0) {
          bVar2 = 1;
        }
        if (bVar2 == 0) {
          if ((*(longlong *)(param_1 + 0x78) == 0) ||
             (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 == 0))
          goto LAB_18193ba0d;
          if (0 < *(int *)(lVar1 + 0xc0)) {
            if (*(longlong *)(lVar1 + 0x60) == 0) goto LAB_18193ba0d;
            cVar3 = BattlePersistentEffectsController_IsEffectActive
                              (*(longlong *)(lVar1 + 0x60),0,0,0);
            if ((cVar3 == '\0') && (*(char *)(lVar1 + 0x1e8) == '\0')) {
              BattleFsmController_StartAbilitySelectPhase(param_1,0);
              return;
            }
          }
        }
        if ((*(longlong *)(param_1 + 0x78) != 0) &&
           (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 != 0)) {
          BattleUnitController_SetSpeedFill(lVar1,0,0);
          if ((*(longlong *)(param_1 + 0x78) != 0) &&
             (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 != 0)) {
            *(undefined4 *)(lVar1 + 0x20c) = 0;
            BattleFsmController_StartPlayerUnitMove(param_1,0);
            return;
          }
        }
      }
    }
  }
LAB_18193ba0d:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

