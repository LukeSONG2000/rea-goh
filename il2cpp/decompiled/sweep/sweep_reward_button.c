// sweep_reward_button VA 0x181384a30 RVA 0x1384a30

void FUN_181384a30(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  code *pcVar5;
  uint uVar6;
  
  lVar2 = *(longlong *)(param_1 + 0xe0);
  if (lVar2 != 0) {
    (**(code **)(lVar2 + 0x18))(*(undefined8 *)(lVar2 + 0x40));
  }
  if (cRam0000000185a3b3fb == '\0') {
    FUN_18035da50(0x185801b48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ee978);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ee940);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a3b3fb = '\x01';
  }
  lVar2 = *(longlong *)(param_1 + 0x1e0);
  uVar6 = 0;
  do {
    if (lVar2 == 0) {
code_r0x000181385a84:
      FUN_180352450();
      pcVar5 = (code *)swi(3);
      (*pcVar5)();
      return;
    }
    lVar3 = *(longlong *)(param_1 + 0x1e0);
    if (*(int *)(lVar2 + 0x18) <= (int)uVar6) {
      if (lVar3 != 0) {
        iVar1 = *(int *)(lVar3 + 0x18);
        *(int *)(lVar3 + 0x1c) = *(int *)(lVar3 + 0x1c) + 1;
        *(undefined4 *)(lVar3 + 0x18) = 0;
        if (0 < iVar1) {
          FUN_183af4330(*(undefined8 *)(lVar3 + 0x10),0,iVar1,0);
        }
        return;
      }
      goto code_r0x000181385a84;
    }
    if (lVar3 == 0) goto code_r0x000181385a84;
    if (*(uint *)(lVar3 + 0x18) <= uVar6) {
      FUN_183aeda10(0);
      pcVar5 = (code *)swi(3);
      (*pcVar5)();
      return;
    }
    lVar2 = *(longlong *)(lVar3 + 0x10);
    if (lVar2 == 0) goto code_r0x000181385a84;
    if (*(uint *)(lVar2 + 0x18) <= uVar6) {
      FUN_180352460();
      pcVar5 = (code *)swi(3);
      (*pcVar5)();
      return;
    }
    uVar4 = *(undefined8 *)(lVar2 + 0x20 + (longlong)(int)uVar6 * 8);
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (cRam0000000185a50550 == '\0') {
      FUN_18035da50(0x185814ea8);
      LOCK();
      UNLOCK();
      cRam0000000185a50550 = '\x01';
    }
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    FUN_1844adc40(uVar4,0,0);
    lVar2 = *(longlong *)(param_1 + 0x1e0);
    uVar6 = uVar6 + 1;
  } while( true );
}

