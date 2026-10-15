// ==== BattleController_Initialize @ 0x1817dfa00 ====

void BattleController_Initialize(longlong *param_1,longlong param_2)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 uVar5;
  
  if (DAT_185a3d04e == '\0') {
    FUN_18035da50(&DAT_1857a3910);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c6810);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806e68);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806e48);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c31c8);
    LOCK();
    UNLOCK();
    DAT_185a3d04e = '\x01';
  }
  uVar5 = DAT_1857c31c8;
  if (param_2 == 0) {
    if (DAT_185a38afc == '\0') {
      FUN_18035da50(&DAT_18582cc68);
      LOCK();
      UNLOCK();
      DAT_185a38afc = '\x01';
    }
    if (param_1 != (longlong *)0x0) {
      plVar2 = (longlong *)FUN_1803a7d00(*param_1 + 0x20);
      if (plVar2 != (longlong *)0x0) {
        uVar3 = (**(code **)(*plVar2 + 0x168))(plVar2,*(undefined8 *)(*plVar2 + 0x170));
        if (*(int *)(DAT_18582cc68 + 0xe4) == 0) {
          FUN_180359940();
        }
        Log_Error(uVar5,0xffffffff,uVar3,0);
        return;
      }
    }
  }
  else {
    lVar4 = *(longlong *)(param_2 + 0x70);
    param_1[0x13] = lVar4;
    if ((lVar4 != 0) && (*(longlong *)(lVar4 + 0xa8) != 0)) {
      uVar5 = *(undefined8 *)(lVar4 + 0x288);
      FUN_180032760();
      param_1[0x1a] = *(longlong *)(param_2 + 0x10);
      param_1[0x1b] = *(longlong *)(param_2 + 0x20);
      param_1[0x1d] = *(longlong *)(param_2 + 0x30);
      param_1[0x1c] = *(longlong *)(param_2 + 0x28);
      param_1[0x1e] = *(longlong *)(param_2 + 0x38);
      param_1[0x1f] = *(longlong *)(param_2 + 0x48);
      param_1[0x20] = *(longlong *)(param_2 + 0x60);
      *(undefined4 *)(param_1 + 0x21) = *(undefined4 *)(param_2 + 0x58);
      param_1[0x26] = *(longlong *)(param_2 + 0x80);
      param_1[0x27] = *(longlong *)(param_2 + 0x88);
      param_1[0x29] = *(longlong *)(param_2 + 0x98);
      *(undefined1 *)((longlong)param_1 + 0x144) = *(undefined1 *)(param_2 + 0x94);
      param_1[0x2a] = *(longlong *)(param_2 + 0xa0);
      param_1[0x2b] = *(longlong *)(param_2 + 0xa8);
      param_1[0x2c] = *(longlong *)(param_2 + 0xb0);
      param_1[0x2d] = *(longlong *)(param_2 + 0xb8);
      *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(param_2 + 0x90);
      param_1[0x2e] = *(longlong *)(param_2 + 0xc0);
      param_1[0x2f] = *(longlong *)(param_2 + 200);
      param_1[0x30] = *(longlong *)(param_2 + 0xd0);
      *(undefined4 *)(param_1 + 0x31) = *(undefined4 *)(param_2 + 0xd8);
      param_1[0x33] = *(longlong *)(param_2 + 0xe8);
      param_1[0x34] = *(longlong *)(param_2 + 0xf0);
      param_1[0x35] = *(longlong *)(param_2 + 0xf8);
      *(undefined1 *)(param_1 + 0x36) = *(undefined1 *)(param_2 + 0x100);
      param_1[0x22] = *(longlong *)(param_2 + 0x128);
      param_1[0x23] = *(longlong *)(param_2 + 0x130);
      *(undefined1 *)((longlong)param_1 + 0x121) = *(undefined1 *)(param_2 + 0x18);
      *(undefined1 *)(param_1 + 0x24) = *(undefined1 *)(param_2 + 0x19);
      param_1[0x37] = *(longlong *)(param_2 + 0x110);
      lVar1 = FUN_1803a51b0(DAT_185806e48);
      FUN_182d09b30(lVar1,DAT_185806e68);
      param_1[0x25] = lVar1;
      if (lVar1 != 0) {
        FUN_1836f5a70(lVar1,*(undefined4 *)(lVar1 + 0x18),*(undefined8 *)(param_2 + 0x138),
                      *(undefined8 *)
                       (*(longlong *)(*(longlong *)(DAT_1857c6810 + 0x20) + 0xc0) + 0x90),lVar4,
                      uVar5);
        if (param_1[0x13] != 0) {
          *(undefined8 *)(param_1[0x13] + 0x140) = *(undefined8 *)(param_2 + 0x108);
          *(undefined1 *)((longlong)param_1 + 0x18c) = *(undefined1 *)(param_2 + 0x11c);
          param_1[0x32] = *(longlong *)(param_2 + 0x120);
                    /* WARNING: Could not recover jumptable at 0x0001817dfcea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (**(code **)(*param_1 + 0x188))(param_1,*(undefined8 *)(*param_1 + 400));
          return;
        }
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

