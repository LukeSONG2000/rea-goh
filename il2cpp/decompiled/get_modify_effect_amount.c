// ==== get_modify_effect_amount @ 0x181a73880 ====


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_181a73880(longlong param_1,undefined8 param_2,undefined8 param_3,undefined1 param_4,
             undefined1 param_5)

{
  float fVar1;
  float fVar2;
  undefined8 uVar3;
  undefined4 uVar4;
  
  if (cRam0000000185a3e0af == '\0') {
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    cRam0000000185a3e0af = '\x01';
  }
  fVar1 = (float)FUN_181a73980(param_1,*(undefined8 *)(param_1 + 0x70),param_2,0x2e,param_3,param_4,
                               param_5,0);
  uVar3 = FUN_181a73980(param_1,param_2,*(undefined8 *)(param_1 + 0x70),0x2d,param_3,param_4,param_5
                        ,0);
  fVar1 = ((float)uVar3 + _UNK_18529ef3c) * (fVar1 + _UNK_18529ef3c);
  if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
    FUN_180359940();
  }
  fVar2 = 0.0;
  uVar4 = 0;
  if (0.0 <= fVar1) {
    fVar2 = fVar1;
    uVar4 = (int)((ulonglong)uVar3 >> 0x20);
  }
  return CONCAT44(uVar4,fVar2);
}

