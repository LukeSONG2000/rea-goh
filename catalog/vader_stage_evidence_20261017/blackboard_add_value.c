// blackboard_add_value VA 0x181bcb090 RVA 0x1bcb090

void FUN_181bcb090(longlong param_1,undefined8 param_2,undefined8 param_3,float param_4)

{
  longlong lVar1;
  code *pcVar2;
  float fVar3;
  
  if (cRam0000000185a3e943 == '\0') {
    FUN_18035da50(0x1857a96a0);
    LOCK();
    UNLOCK();
    cRam0000000185a3e943 = '\x01';
  }
  lVar1 = *(longlong *)(param_1 + 0x80);
  if (lVar1 != 0) {
    fVar3 = (float)FUN_1816a8bb0(lVar1,param_2,0);
    FUN_1816a8930(lVar1,param_2,param_3,fVar3 + param_4,0);
    if ((*(longlong *)(param_1 + 0x88) != 0) &&
       (*(longlong *)(*(longlong *)(param_1 + 0x88) + 0x180) != 0)) {
      FUN_180037e10();
      return;
    }
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

