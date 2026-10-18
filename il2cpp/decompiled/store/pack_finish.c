// pack_finish VA 0x1818c29c0 RVA 0x18c29c0

void FUN_1818c29c0(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  code *pcVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  if (cRam0000000185a3d59c == '\0') {
    FUN_18035da50(0x1857ff328);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858017e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d3da0);
    LOCK();
    UNLOCK();
    cRam0000000185a3d59c = '\x01';
  }
  lVar4 = *(longlong *)(param_1 + 0xb0);
  if (lVar4 != 0) {
    if (cRam0000000185a3ac3a == '\0') {
      FUN_18035da50(0x185788a00);
      LOCK();
      UNLOCK();
      cRam0000000185a3ac3a = '\x01';
    }
    uStack_18 = 0x27;
    uStack_10 = 0;
    FUN_18001f4d0(lVar4,&uStack_18);
    if (*(longlong *)(param_1 + 0x90) != 0) {
      FUN_181af9f10(*(longlong *)(param_1 + 0x90),0);
      if (*(char *)(param_1 + 0x10a) == '\0') {
        lVar4 = *(longlong *)(param_1 + 0x120);
        if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
          FUN_180359940();
        }
        lVar1 = *(longlong *)(param_1 + 0x58);
        if (lVar4 == 0) {
          if (lVar1 != 0) {
            FUN_180fcd110(lVar1,1,0);
            if (*(longlong *)(lVar1 + 0x20) != 0) {
              FUN_1812a57f0(*(longlong *)(lVar1 + 0x20),0,0);
              return;
            }
          }
        }
        else {
          uVar2 = *(undefined8 *)(param_1 + 0x120);
          lVar4 = FUN_1803a51b0(uRam00000001857d3da0);
          if (lVar4 != 0) {
            *(undefined4 *)(lVar4 + 0x1c) = 0;
            *(undefined4 *)(lVar4 + 0x24) = 4;
            lVar5 = FUN_1803a51b0(uRam00000001858017e0);
            *(undefined8 *)(lVar5 + 0x10) = uVar2;
            *(undefined4 *)(lVar5 + 0x18) = 0;
            *(longlong *)(lVar5 + 0x20) = lVar4;
            *(undefined1 *)(lVar5 + 0x1c) = 1;
            if (lVar1 != 0) {
              FUN_180fcd110(lVar1,1,0);
              if (*(longlong *)(lVar1 + 0x20) != 0) {
                FUN_1812a5980(*(longlong *)(lVar1 + 0x20),*(undefined8 *)(lVar5 + 0x10),
                              *(undefined8 *)(lVar5 + 0x20),0);
                return;
              }
            }
          }
        }
      }
      else if (*(longlong *)(param_1 + 0x58) != 0) {
        FUN_182b8cfb0(*(longlong *)(param_1 + 0x58),0,3,uRam00000001857ff328);
        return;
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

